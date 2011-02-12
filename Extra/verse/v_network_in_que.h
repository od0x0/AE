/*
 * $Id: v_network_in_que.h 4533 2008-08-11 15:00:51Z jiri $ 
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

typedef struct{
	void	*oldest;
	void	*newest;
	uint32	packet_id;
	uint32	seconds, fractions;		/* Current time. */
	uint32	acc_seconds, acc_fractions;	/* Accumulated time. */
}VNetInQueue;

typedef struct{
	void	*newer;
	void	*older;
	char	data[1500];
	size_t	size;
}VNetInPacked;

extern void		v_niq_clear(VNetInQueue *queue);
extern void		v_niq_timer_update(VNetInQueue *queue);

extern VNetInPacked *	v_niq_get(VNetInQueue *queue, size_t *length);
extern void		v_niq_release(VNetInQueue *queue, VNetInPacked *p);
extern char *		v_niq_store(VNetInQueue *queue, size_t length, unsigned int packet_id);
unsigned int		v_niq_free(VNetInQueue *queue);
extern uint32		v_niq_time_out(VNetInQueue *queue);
