#ifndef API_H
#define API_H

#define SIZE_IPK_S8 (6 * SIZE_POLYQ1_S8 + 7 * SIZE_POLYQ2_S8) // = 1 769 472 bytes
#define SIZE_OPK_S8 (4* SIZE_POLYQ_S8) // = 524 288 bytes
#define SIZE_IPK_OPK_S8 (SIZE_IPK_S8 + SIZE_OPK_S8) // = 2 293 760 bytes
#define SIZE_CRED_S8 (6 * SIZE_POLYQ2_S8) // = 1 179 648 bytesprvt
#define SIZE_PRES_S8 (15 * SIZE_POLYQ1_S8 + 8 * SIZE_POLYQ2_S8 + 9 * SIZE_POLYQ_S8) // = 3 735 552 bytes


#define DEMO_CRED_BYTELENGTH (SIZE_IPK_OPK_S8 + 1 + SIZE_INTQ2_S8 + SIZE_CRED_S8) // |public keys| + |attribut| + |identity| + |private credential| = 3 473 432 bytes
#define DEMO_PRES_BYTELENGTH (SIZE_MESS_S8 + 1 + SIZE_PRES_S8)    // |message| + |attribut| + |presentation token| = 3 735 563 bytes

#include <stdint.h>

#include "new_arithm_int.h"
#include "new_arithm_poly.h"
#include "params.h"
#include "protocol.h"

extern uint8_t fixed_demo_public_keys[2293760];
extern uint8_t demo_hash[983040];

#define BENCHMARK

#ifdef BENCHMARK
extern unsigned long long compressed_poly_sizes[23];
#endif


//////////////////////////////////////////////////////////////
/////////////            External API            /////////////
//////////////////////////////////////////////////////////////

/* start the anonymous credential service */
int anonymous_credential_start ();

/* stop the anonymous credential service */
int anonymous_credential_stop ();

/* initialize data files:
   - prvt_phoebe.ac prvt_emma.ac prvt_julian.ac prvt_edward.ac with credentials
   - pblc_data.ac with attribute values, and issuer and opener public keys */
int demonstrator_init ();

/* generate a presentation token demo_pres_out of DEMO_PRES_BYTELENGTH bytes
   from a credential demo_cred_in of DEMO_CRED_BYTELENGTH bytes
   and a message demo_msg_in of SIZE_MESS_S8 bytes (see protocol.h) */
int get_presentation_token (uint8_t *demo_pres_out, uint8_t *demo_cred_in, uint8_t *demo_msg_in);

/* verify a presentation token demo_pres_in of DEMO_PRES_BYTELENGTH bytes
   with public keys demo_pk_in of SIZE_IPK_OPK_S8 bytes */
int verify_presentation_token (uint8_t *demo_pres_in, uint8_t *demo_pks_in);

/* verify a presentation token demo_pres_in of DEMO_PRES_BYTELENGTH bytes */
int verify_presentation_token_demo (uint8_t *demo_pres_in);

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


int conv_credential_to_p_uint8 (uint8_t *p_uint8_out, s_credential_t cred_in);
int conv_p_uint8_to_credential (s_credential_t cred_out, uint8_t *p_uint8_in);

int conv_presentation_to_p_uint8 (uint8_t *p_uint8_out, s_presentation_t pres_in);
int conv_p_uint8_to_presentation (s_presentation_t pres_out, uint8_t *p_uint8_in);

int conv_ipk_opk_to_p_uint8 (uint8_t *p_u8_out, s_issuer_pk_t ipk_in, s_opener_pk_t opk_in);
int conv_p_uint8_to_ipk_opk (s_issuer_pk_t ipk_out, s_opener_pk_t opk_out, uint8_t *p_u8_in);

void conv_ipk_opk_attr_cred_id_to_p_uint8 (uint8_t *p_u8_out, s_issuer_pk_t ipk, s_opener_pk_t opk, attribute_t attr, 
                                   s_credential_t cred, intq2_t id);
void conv_p_uint8_to_ipk_opk_attr_cred_id (s_issuer_pk_t ipk, s_opener_pk_t opk, attribute_t *attr, s_credential_t cred, 
                                      intq2_t id, uint8_t *p_u8_in);
void conv_mess_attr_pres_to_p_uint8 (uint8_t *p_u8_out, message_t mess, s_presentation_t pres, attribute_t attr);

void conv_p_uint8_to_mess_attr_pres (message_t mess, s_presentation_t pres, attribute_t *attr, uint8_t *p_u8_in);

unsigned long long compression_presentation(uint8_t *compression_space, s_presentation_t presentation);

unsigned long long decompression_presentation(s_presentation_t presentation, uint8_t *compression_space);

#endif /* API_H */
