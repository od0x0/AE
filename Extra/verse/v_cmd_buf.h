/*
 * $Id: v_cmd_buf.h 4533 2008-08-11 15:00:51Z jiri $ 
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

#include <stdlib.h>

#include "verse_header.h"
#include "v_pack.h"

#define	V_NOQ_MAX_PACKET_SIZE	1500

typedef enum {
	VCMDBS_10 = 0,
	VCMDBS_20 = 1,
	VCMDBS_30 = 2,
	VCMDBS_80 = 3,
	VCMDBS_160 = 4,
	VCMDBS_320 = 5,
	VCMDBS_1500 = 6,
	VCMDBS_COUNT = 7
} VCMDBufSize;

typedef struct {
	void			*next;
	uint32			packet;
	unsigned int	address_size;
	unsigned int	address_sum;
	VCMDBufSize		buf_size;
	unsigned int	size;
} VCMDBufHead;

typedef struct {
	VCMDBufHead	head;
	uint8		buf[10];
} VCMDBuffer10;

typedef struct {
	VCMDBufHead	head;
	uint8		buf[20];
} VCMDBuffer20;

typedef struct {
	VCMDBufHead head;
	uint8		buf[30];
} VCMDBuffer30;

typedef struct {
	VCMDBufHead	head;
	uint8		buf[80];
} VCMDBuffer80;

typedef struct {
	VCMDBufHead	head;
	uint8		buf[160];
} VCMDBuffer160;

typedef struct {
	VCMDBufHead	head;
	uint8		buf[320];
} VCMDBuffer320;


typedef struct {
	VCMDBufHead	head;
	uint8		buf[1500];
} VCMDBuffer1500;

extern VCMDBufHead	*v_cmd_buf_allocate(VCMDBufSize buf_size);
extern void		v_cmd_buf_free(VCMDBufHead *head);

extern void		v_cmd_buf_set_size(VCMDBufHead *head, unsigned int size);
extern void		v_cmd_buf_set_address_size(VCMDBufHead *head, unsigned int size);
extern void		v_cmd_buf_set_unique_address_size(VCMDBufHead *head, unsigned int size);
extern boolean		v_cmd_buf_compare(VCMDBufHead *a, VCMDBufHead *b);
