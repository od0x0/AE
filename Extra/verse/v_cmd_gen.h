/*
 * $Id: v_cmd_gen.h 4533 2008-08-11 15:00:51Z jiri $ 
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

/* Define this to at least build the code that regenerates the variable parts of the code. */
/*#define V_GENERATE_FUNC_MODE*/

typedef enum {
	VCGP_UINT8,
	VCGP_UINT16,
	VCGP_UINT32,
	VCGP_REAL32,
	VCGP_REAL64,
	VCGP_POINTER_TYPE,
	VCGP_POINTER,
	VCGP_NAME,
	VCGP_LONG_NAME,
	VCGP_NODE_ID,
	VCGP_LAYER_ID,
	VCGP_BUFFER_ID,
	VCGP_FRAGMENT_ID,
	VCGP_ENUM_NAME,
	VCGP_ENUM,
	VCGP_PACK_INLINE,
	VCGP_UNPACK_INLINE,
	VCGP_END_ADDRESS
} VCGParam;

typedef enum {
	VCGCT_NORMAL,
	VCGCT_UNIQUE,
	VCGCT_ONCE,
	VCGCT_INVISIBLE_SYSTEM,	/* In the dark we are all invisible. */
	VCGCT_ORDERED
} VCGCommandType;

extern void v_cg_new_cmd(VCGCommandType type, const char *name, unsigned int cmd_id, VCGCommandType command);
extern void v_cg_add_param(VCGParam type, const char *name);
extern void v_cg_alias(char bool_switch, const char *name, const char *qualifier,
		       unsigned int param, unsigned int *param_array);
extern void v_cg_end_cmd(void);
extern void v_cg_new_manual_cmd(unsigned int cmd_id, const char *name, const char *params, const char *alias_name, const char *alias_params);
