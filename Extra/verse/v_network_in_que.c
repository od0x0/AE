/*
 * $Id: v_network_in_que.c 4533 2008-08-11 15:00:51Z jiri $ 
 *
 * ***** BEGIN BSD LICENSE BLOCK *****
 *
 * Copyright (c) 2005-2008, The Uni-Verse Consortium.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END BSD LICENSE BLOCK *****
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verse_header.h"

#include "v_cmd_buf.h"
#include "v_cmd_gen.h"
#include "v_connection.h"
#include "v_internal_verse.h"
#include "v_network.h"
#include "v_pack.h"


#if !defined(V_GENERATE_FUNC_MODE)

#include "v_network_in_que.h"

static VNetInPacked *v_niq_temp = NULL;

void v_niq_clear(VNetInQueue *queue)
{
	queue->oldest = NULL;
	queue->newest = NULL;
	queue->packet_id = 2;
	v_niq_timer_update(queue);
}

/* Set queue's last-used timestamp to "now". */
void v_niq_timer_update(VNetInQueue *queue)
{
	v_n_get_current_time(&queue->seconds, &queue->fractions);
	queue->acc_seconds = queue->acc_fractions = 0;
}

uint32 v_niq_time_out(VNetInQueue *queue)
{
	uint32	fractions, f;

	/* Magic code to disregard if the clock moves forward more than one second at a time.
	 * This should help keep Verse alive on e.g. a notebook that is suspended.
	*/
	v_n_get_current_time(NULL, &fractions);
	if(fractions < queue->fractions)
		f = 0xffffffffu - queue->fractions + fractions;
	else
		f = fractions - queue->fractions;
/*	printf("now=%u last=%u -> f=%u\n", fractions, queue->fractions, f);*/
	if(queue->acc_fractions + f < queue->acc_fractions)
		queue->acc_seconds += 1;
	queue->acc_fractions += f;
	queue->fractions = fractions;
	
/*	printf("queue at %p has seconds=%u, now=%u -> diff=%u\n", queue, queue->seconds, seconds, seconds - queue->seconds);*/
	return queue->acc_seconds;
}

VNetInPacked * v_niq_get(VNetInQueue *queue, size_t *length)
{
	VNetInPacked *p;

	if(queue->oldest == NULL)
	{
		*length = 0;
		return NULL;
	}
	/* pop oldest package */
	p = queue->oldest;
	queue->oldest = p->newer;
	if(queue->oldest == NULL)
		queue->newest = NULL;
	else
		((VNetInPacked *)queue->oldest)->older = NULL;
	*length = p->size;

	return p;
}

unsigned int v_niq_free(VNetInQueue *queue)
{
	unsigned int i;
	size_t length;

	for(i = 0; v_niq_get(queue, &length) != NULL; i++)
		;
	return i;
}

void v_niq_release(VNetInQueue *queue, VNetInPacked *p)
{
	/* push on v_niq_temp for re-use */
	p->older = v_niq_temp;
	v_niq_temp = p;
}

char *v_niq_store(VNetInQueue *queue, size_t length, unsigned int packet_id)
{
	VNetInPacked	*p;

	v_niq_timer_update(queue);

	if(packet_id < queue->packet_id)
		return NULL;
	
	while(queue->packet_id != packet_id)
	{
		verse_send_packet_nak(queue->packet_id++);
		if(queue->packet_id == 0)
			queue->packet_id++;
	}
	queue->packet_id++;
	if(queue->packet_id == 0)
		queue->packet_id++;
	verse_send_packet_ack(packet_id);

	if(v_niq_temp == NULL)
		p = malloc(sizeof *p);
	else
	{
		/* pop off v_niq_temp */
		p = v_niq_temp;
		v_niq_temp = p->older;
	}
	/* push as newest */
	p->older = queue->newest;
	p->newer = NULL;

	if(queue->newest == NULL)
		queue->oldest = p;
	else
		((VNetInPacked *)queue->newest)->newer = p;
	queue->newest = p;

	p->size = length;

	return p->data;
}

#endif
