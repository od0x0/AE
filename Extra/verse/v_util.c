/*
 * $Id: v_util.c 4533 2008-08-11 15:00:51Z jiri $ 
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

/*
 * Utility functions.
*/

#include <stdio.h>

#include "verse_header.h"
#include "v_network.h"
#include "v_util.h"

/* Safe string copy. Copies from <src> to <dst>, not using more than <size>
 * bytes of destination space. Always 0-terminates the destination. Returns
 * the beginning of the destination string.
*/
char * v_strlcpy(char *dst, const char *src, size_t size)
{
	char	*base = dst;

	if(size == 0)
		return NULL;
	for(size--; size > 0 && *src != '\0'; size--)
		*dst++ = *src++;
	*dst = '\0';

	return base;
}

void v_timer_start(VUtilTimer *timer)
{
	v_n_get_current_time(&timer->seconds, &timer->fractions);
}

void v_timer_advance(VUtilTimer *timer, double seconds)
{
	if(timer == NULL)
		return;
	timer->seconds   += (uint32) seconds;
	timer->fractions += (uint32) ((seconds - (int) seconds) * (double) 0xffffffff);
}

double v_timer_elapsed(const VUtilTimer *timer)
{
	uint32 cur_seconds, cur_fractions;

	v_n_get_current_time(&cur_seconds, &cur_fractions);
	return (double)(cur_seconds - timer->seconds) + ((double)cur_fractions - (double)timer->fractions) / (double) 0xffffffff;
}

void v_timer_print(const VUtilTimer *timer)
{
	uint32 cur_seconds, cur_fractions;

	v_n_get_current_time(&cur_seconds, &cur_fractions);
	printf("%f", (double)(cur_seconds - timer->seconds) + ((double)cur_fractions - (double)timer->fractions) / (double) 0xffffffff);
}

/* Compare |x| against built-in semi-magical constant, and return 1 if it's larger, 0 if not. */
static int quat_valid(real64 x)
{
	const real64	EPSILON = 0.0000001;
	return x > 0.0 ? x > EPSILON : x < -EPSILON;
}

int v_quat32_valid(const VNQuat32 *q)
{
	if(q == NULL)
		return 0;
	return quat_valid(q->x) && quat_valid(q->y) && quat_valid(q->z) && quat_valid(q->w);
}

int v_quat64_valid(const VNQuat64 *q)
{
	if(q == NULL)
		return 0;
	return quat_valid(q->x) && quat_valid(q->y) && quat_valid(q->z) && quat_valid(q->w);
}

VNQuat32 * v_quat32_from_quat64(VNQuat32 *dst, const VNQuat64 *src)
{
	if(dst == NULL || src == NULL)
		return NULL;
	dst->x = (real32) src->x;
	dst->y = (real32) src->y;
	dst->z = (real32) src->z;	
	dst->w = (real32) src->w;
	return dst;
}

VNQuat64 * v_quat64_from_quat32(VNQuat64 *dst, const VNQuat32 *src)
{
	if(dst == NULL || src == NULL)
		return NULL;
	dst->x = src->x;	
	dst->y = src->y;
	dst->z = src->z;	
	dst->w = src->w;
	return dst;
}
