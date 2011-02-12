/*
 * $Id: v_encryption.h 4533 2008-08-11 15:00:51Z jiri $ 
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
 * Verse encryption routines. There are two distinct flavors of encryption
 * in use: one "heavy" for login/connection establishment security, and
 * a far lighter symmetrical one that is applied to each data packet after
 * the key has been exchanged during connection.
*/

#include "verse.h"

/* Internal key size definitions. *MUST* be kept in sync with V_HOST_ID_SIZE in verse_header.h! */
#define	V_ENCRYPTION_LOGIN_KEY_BITS	 512
#define V_ENCRYPTION_LOGIN_KEY_SIZE	 (V_ENCRYPTION_LOGIN_KEY_BITS / 8)
#define V_ENCRYPTION_LOGIN_KEY_FULL_SIZE (3 * V_ENCRYPTION_LOGIN_KEY_SIZE)
#define V_ENCRYPTION_LOGIN_KEY_HALF_SIZE (2 * V_ENCRYPTION_LOGIN_KEY_SIZE)

#define V_ENCRYPTION_LOGIN_PUBLIC_START	 (0 * V_ENCRYPTION_LOGIN_KEY_SIZE)
#define V_ENCRYPTION_LOGIN_PRIVATE_START (1 * V_ENCRYPTION_LOGIN_KEY_SIZE)
#define V_ENCRYPTION_LOGIN_N_START	 (2 * V_ENCRYPTION_LOGIN_KEY_SIZE)

#define V_ENCRYPTION_DATA_KEY_SIZE	 (V_ENCRYPTION_LOGIN_KEY_BITS / 8)

/* Connection encryption. Heavy, and symmetrical, so encrypt() does both encryption
 * and decryption given the proper key. Current algorithm used is RSA.
*/
extern void v_e_connect_create_key(uint8 *private_key, uint8 *public_key, uint8 *n);
extern void v_e_connect_encrypt(uint8 *output, const uint8 *data, const uint8 *key, const uint8 *key_n);

/* Actual data traffic encryption. Also symmetrical, with a single key. Uses XOR. */
extern const uint8 *	v_e_data_create_key(void);
extern void		v_e_data_encrypt_command(uint8 *packet, size_t packet_length,
						 const uint8 *command, size_t command_length, const uint8 *key);
extern void		v_e_data_decrypt_packet(uint8 *to, const uint8 *from, size_t size, const uint8 *key);
