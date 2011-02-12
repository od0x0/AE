/*
 * $Id: v_util.h 4533 2008-08-11 15:00:51Z jiri $ 
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
 * Miscellaneous utility routines for generic use throughout the code.
*/

/* Safe, buffer size limited, string copy. */
extern char *	v_strlcpy(char *dst, const char *src, size_t size);

typedef struct {
	uint32 seconds;
	uint32 fractions;
} VUtilTimer;

extern void	v_timer_start(VUtilTimer *timer);
extern void	v_timer_advance(VUtilTimer *timer, double seconds);
extern double	v_timer_elapsed(const VUtilTimer *timer);
extern void	v_timer_print(const VUtilTimer *timer);

extern int	v_quat32_valid(const VNQuat32 *q);
extern int	v_quat64_valid(const VNQuat64 *q);
extern VNQuat32*v_quat32_from_quat64(VNQuat32 *dst, const VNQuat64 *src);
extern VNQuat64*v_quat64_from_quat32(VNQuat64 *dst, const VNQuat32 *src);
