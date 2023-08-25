#include <stdio.h>

#include "API.h"
#include "compression_rice.h"

#include "pblc_hash.ac"
#include "pblc_data_demo.ac"

#ifdef BENCHMARK
unsigned long long compressed_poly_sizes[23];
#endif


void 
conv_ipk_opk_attr_cred_id_to_p_uint8 (uint8_t *p_u8_out, s_issuer_pk_t ipk, s_opener_pk_t opk, attribute_t attr, 
                                   s_credential_t cred, intq2_t id)
{
  int pp = 0;
  conv_ipk_opk_to_p_uint8(p_u8_out, ipk, opk); 
  pp += SIZE_IPK_OPK_S8;
  p_u8_out[pp] = attr;
  pp++;
  conv_credential_to_p_uint8(p_u8_out + pp, cred);
  pp += SIZE_CRED_S8;
  conv_intq2_to_p_uint8(p_u8_out + pp, id);
}

void
conv_p_uint8_to_ipk_opk_attr_cred_id (s_issuer_pk_t ipk, s_opener_pk_t opk, attribute_t *attr, s_credential_t cred, 
                                      intq2_t id, uint8_t *p_u8_in)
{
  int pp = 0;
  conv_p_uint8_to_ipk_opk(ipk, opk, p_u8_in); 
  pp += SIZE_IPK_OPK_S8;
  *attr = p_u8_in[pp];
  pp++;
  conv_p_uint8_to_credential(cred, p_u8_in + pp);
  pp += SIZE_CRED_S8;
  conv_p_uint8_to_intq2(id, p_u8_in + pp);
}

void 
conv_mess_attr_pres_to_p_uint8 (uint8_t *p_u8_out, message_t mess, s_presentation_t pres, attribute_t attr)
{
  int pp = 0;
  for (int i = 0; i<SIZE_MESS_S8; i++)
    {
      p_u8_out[pp+i] = mess[i];
    }
  pp += SIZE_MESS_S8;
  p_u8_out[pp] = attr;
  pp ++;
  conv_presentation_to_p_uint8(p_u8_out + pp, pres);
}

void
conv_p_uint8_to_mess_attr_pres (message_t mess, s_presentation_t pres, attribute_t *attr, uint8_t *p_u8_in)
{
  int pp = 0;
  for (int i = 0; i<SIZE_MESS_S8; i++)
    {
      mess[i] = p_u8_in[pp+i];
    }
  pp += SIZE_MESS_S8;
  *attr = p_u8_in[pp];
  pp ++;
  conv_p_uint8_to_presentation(pres, p_u8_in + pp);
}

int conv_credential_to_p_uint8 (uint8_t *p_u8_out, s_credential_t cred_in)
{
  int pp = 0;
  conv_polyq2_to_p_uint8(p_u8_out, cred_in->NTTq2_s1_V2[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, cred_in->NTTq2_s1_V2[1]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, cred_in->NTTq2_s2_V2[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, cred_in->NTTq2_s2_V2[1]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, cred_in->NTTq2_s3_V2[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, cred_in->NTTq2_s3_V2[1]);
  return EXIT_SUCCESS;
}

/*
int conv_credential_to_p_uint8 (uint8_t *p_u8_out, s_credential_t cred_in)
{
  unsigned long long number_of_bits = 0;
  unsigned long long total_number_of_bits = 0;
  int position = 0;
  int parameter = CEIL_LOG_S;
  polyq2_t poly;
  polyq2_init(poly);
  polyq2_copy(poly, cred_in->NTTq2_s1_V2[0]);
  conv_NTT_polyq2_to_R_polyq2(poly);
  number_of_bits = compression_rice_polyq2(p_u8_out, poly, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_out += number_of_bits / 8;
  polyq2_copy(poly, cred_in->NTTq2_s1_V2[1]);
  conv_NTT_polyq2_to_R_polyq2(poly);
  number_of_bits = compression_rice_polyq2(p_u8_out, poly, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_out += number_of_bits / 8;
  polyq2_copy(poly, cred_in->NTTq2_s2_V2[0]);
  conv_NTT_polyq2_to_R_polyq2(poly);
  number_of_bits = compression_rice_polyq2(p_u8_out, poly, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_out += number_of_bits / 8;
  polyq2_copy(poly, cred_in->NTTq2_s2_V2[1]);
  conv_NTT_polyq2_to_R_polyq2(poly);
  number_of_bits = compression_rice_polyq2(p_u8_out, poly, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_out += number_of_bits / 8;
  parameter = CEIL_LOG_R;
  polyq2_copy(poly, cred_in->NTTq2_s3_V2[0]);
  conv_NTT_polyq2_to_R_polyq2(poly);
  number_of_bits = compression_rice_polyq2(p_u8_out, poly, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_out += number_of_bits / 8;
  polyq2_copy(poly, cred_in->NTTq2_s3_V2[1]);
  conv_NTT_polyq2_to_R_polyq2(poly);
  number_of_bits = compression_rice_polyq2(p_u8_out, poly, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  return EXIT_SUCCESS;
}
*/

int conv_p_uint8_to_credential (s_credential_t cred_out, uint8_t *p_u8_in)
{
  int pp = 0;
  conv_p_uint8_to_polyq2(cred_out->NTTq2_s1_V2[0], p_u8_in);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(cred_out->NTTq2_s1_V2[1], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(cred_out->NTTq2_s2_V2[0], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(cred_out->NTTq2_s2_V2[1], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(cred_out->NTTq2_s3_V2[0], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(cred_out->NTTq2_s3_V2[1], p_u8_in + pp);
  return EXIT_SUCCESS;
}

/*
int conv_p_uint8_to_credential (s_credential_t cred_out, uint8_t *p_u8_in)
{
  unsigned long long number_of_bits = 0;
  unsigned long long total_number_of_bits = 0;
  int position = 0;
  int parameter = CEIL_LOG_S;
  polyq2_t poly;
  polyq2_init(poly);
  number_of_bits = decompression_rice_polyq2(poly, p_u8_in, parameter, position);
  conv_R_polyq2_to_NTT_polyq2 (poly);
  polyq2_copy(cred_out->NTTq2_s1_V2[0], poly);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_in += number_of_bits / 8;
  number_of_bits = decompression_rice_polyq2(poly, p_u8_in, parameter, position);
  conv_R_polyq2_to_NTT_polyq2 (poly);
  polyq2_copy(cred_out->NTTq2_s1_V2[1], poly);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_in += number_of_bits / 8;
  number_of_bits = decompression_rice_polyq2(poly, p_u8_in, parameter, position);
  conv_R_polyq2_to_NTT_polyq2 (poly);
  polyq2_copy(cred_out->NTTq2_s2_V2[0], poly);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_in += number_of_bits / 8;
  number_of_bits = decompression_rice_polyq2(poly, p_u8_in, parameter, position);
  conv_R_polyq2_to_NTT_polyq2 (poly);
  polyq2_copy(cred_out->NTTq2_s2_V2[1], poly);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_in += number_of_bits / 8;
  parameter = CEIL_LOG_R;
  number_of_bits = decompression_rice_polyq2(poly, p_u8_in, parameter, position);
  conv_R_polyq2_to_NTT_polyq2 (poly);
  polyq2_copy(cred_out->NTTq2_s3_V2[0], poly);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  p_u8_in += number_of_bits / 8;
  number_of_bits = decompression_rice_polyq2(poly, p_u8_in, parameter, position);
  conv_R_polyq2_to_NTT_polyq2 (poly);
  polyq2_copy(cred_out->NTTq2_s3_V2[1], poly);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  return EXIT_SUCCESS;
}
*/

unsigned long long compression_presentation(uint8_t *compression_space, s_presentation_t presentation)
{
  unsigned long long number_of_bits = 0;
  unsigned long long total_number_of_bits = 0;
  int position = 0;
  int parameter;
  polyq1_t polyq1;
  polyq1_init(polyq1);
  polyq2_t polyq2;
  polyq2_init(polyq2);
  polyQ_t polyQ;
  polyQ_init(polyQ);

  parameter = SIZE_Q1_S1 - 1;
  polyq1_copy(polyq1, presentation->NTTq1_commit_t1);
  conv_NTT_polyq1_to_R_polyq1(polyq1);
  number_of_bits = compression_rice_polyq1(compression_space, polyq1, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q2_S1 - 1;
  polyq2_copy(polyq2, presentation->NTTq2_commit_t2);
  conv_NTT_polyq2_to_R_polyq2(polyq2);
  number_of_bits = compression_rice_polyq2(compression_space, polyq2, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q1_S1 - 1;
  polyq1_copy(polyq1, presentation->NTTq1_commit_t1_prime);
  conv_NTT_polyq1_to_R_polyq1(polyq1);
  number_of_bits = compression_rice_polyq1(compression_space, polyq1, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q2_S1 - 1;
  polyq2_copy(polyq2, presentation->NTTq2_commit_t2_prime);
  conv_NTT_polyq2_to_R_polyq2(polyq2);
  number_of_bits = compression_rice_polyq2(compression_space, polyq2, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q_S1 - 1;
  polyQ_copy(polyQ, presentation->NTTQ_enc1);
  conv_NTT_polyQ_to_R_polyQ(polyQ);
  number_of_bits = compression_rice_polyQ(compression_space, polyQ, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q_S1 - 1;
  for (int i = 0; i < 3 ; i++) {
    polyQ_copy(polyQ, presentation->NTTQ_enc2_V3[i]);
    conv_NTT_polyQ_to_R_polyQ(polyQ);
    number_of_bits = compression_rice_polyQ(compression_space, polyQ, parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = compression_rice_polyq1(compression_space, presentation->Rq1_z_V3[i],
                                             parameter, position);
    #ifdef BENCHMARK
    compressed_poly_sizes[i] = number_of_bits;
    #endif
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = compression_rice_polyq1(compression_space, presentation->Rq1_z_prime_V3[i],
                                             parameter, position);
    #ifdef BENCHMARK
    compressed_poly_sizes[3 + i] = number_of_bits;
    #endif
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = compression_rice_polyq1(compression_space, presentation->Rq1_z1_V3[i],
                                             parameter, position);
    #ifdef BENCHMARK
    compressed_poly_sizes[6 + i] = number_of_bits;
    #endif
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = compression_rice_polyq1(compression_space, presentation->Rq1_z5_V3[i],
                                             parameter, position);
    #ifdef BENCHMARK
    compressed_poly_sizes[9 + i] = number_of_bits;
    #endif
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 5; i++) {
    number_of_bits = compression_rice_polyQ(compression_space, presentation->RQ_zenc_V5[i],
                                            parameter, position);
    #ifdef BENCHMARK
    compressed_poly_sizes[12 + i] = number_of_bits;
    #endif
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI1;
  for (int i = 0; i < 4 ; i++) {
    number_of_bits = compression_rice_polyq2(compression_space, presentation->Rq2_zs1_V4[i], parameter, position);
    number_of_bits += position;
    #ifdef BENCHMARK
    compressed_poly_sizes[17 + i] = number_of_bits;
    #endif
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI2;
  for (int i = 0; i < 2 ; i++) {
    number_of_bits = compression_rice_polyq2(compression_space, presentation->Rq2_zs2_V2[i],
                                             parameter, position);
    #ifdef BENCHMARK
    compressed_poly_sizes[21 + i] = number_of_bits;
    #endif
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = 1;
  conv_challenge_to_polyq1(polyq1, presentation->challenge_chall);
  number_of_bits = compression_rice_polyq1(compression_space, polyq1, parameter, position);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  return total_number_of_bits;
}

unsigned long long decompression_presentation(s_presentation_t presentation, uint8_t *compression_space)
{
  unsigned long long number_of_bits = 0;
  unsigned long long total_number_of_bits = 0;
  int position = 0;
  int parameter;

  parameter = SIZE_Q1_S1 - 1;
  number_of_bits = decompression_rice_polyq1(presentation->NTTq1_commit_t1, compression_space,
                                             parameter, position);
  conv_R_polyq1_to_NTT_polyq1(presentation->NTTq1_commit_t1);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q2_S1 - 1;
  number_of_bits = decompression_rice_polyq2(presentation->NTTq2_commit_t2, compression_space,
                                             parameter, position);
  conv_R_polyq2_to_NTT_polyq2(presentation->NTTq2_commit_t2);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q1_S1 - 1;
  number_of_bits = decompression_rice_polyq1(presentation->NTTq1_commit_t1_prime, compression_space,
                                             parameter, position);
  conv_R_polyq1_to_NTT_polyq1(presentation->NTTq1_commit_t1_prime);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q2_S1 - 1;
  number_of_bits = decompression_rice_polyq2(presentation->NTTq2_commit_t2_prime, compression_space,
                                             parameter, position);
  conv_R_polyq2_to_NTT_polyq2(presentation->NTTq2_commit_t2_prime);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q_S1 - 1;
  number_of_bits = decompression_rice_polyQ(presentation->NTTQ_enc1, compression_space,
                                            parameter, position);
  conv_R_polyQ_to_NTT_polyQ(presentation->NTTQ_enc1);
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  position = number_of_bits % 8;
  compression_space += number_of_bits / 8;

  parameter = SIZE_Q_S1 - 1;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = decompression_rice_polyQ(presentation->NTTQ_enc2_V3[i], compression_space,
                                              parameter, position);
    conv_R_polyQ_to_NTT_polyQ(presentation->NTTQ_enc2_V3[i]);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = decompression_rice_polyq1(presentation->Rq1_z_V3[i], compression_space,
                                               parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = decompression_rice_polyq1(presentation->Rq1_z_prime_V3[i], compression_space,
                                               parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = decompression_rice_polyq1(presentation->Rq1_z1_V3[i], compression_space,
                                               parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 3 ; i++) {
    number_of_bits = decompression_rice_polyq1(presentation->Rq1_z5_V3[i], compression_space,
                                               parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI;
  for (int i = 0; i < 5 ; i++) {
    number_of_bits = decompression_rice_polyQ(presentation->RQ_zenc_V5[i], compression_space,
                                              parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI1;
  for (int i = 0; i < 4 ; i++) {
    number_of_bits = decompression_rice_polyq2(presentation->Rq2_zs1_V4[i], compression_space,
                                               parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = CEIL_LOG_XI2;
  for (int i = 0; i < 2 ; i++) {
    number_of_bits = decompression_rice_polyq2(presentation->Rq2_zs2_V2[i], compression_space,
                                               parameter, position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }

  parameter = 1;
  polyq1_t polyq1;
  polyq1_init(polyq1);
  number_of_bits = decompression_rice_polyq1(polyq1, compression_space,
                                             parameter, position);
  presentation->challenge_chall[0] = polyq1[0];
  number_of_bits += position;
  total_number_of_bits += number_of_bits;
  polyq1_clear(polyq1);

  return total_number_of_bits;
}

int conv_presentation_to_p_uint8 (uint8_t *p_u8_out, s_presentation_t pres_in)
{
  int pp = 0;
  conv_polyq1_to_p_uint8(p_u8_out, pres_in->NTTq1_commit_t1);
  pp += SIZE_POLYQ1_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->NTTq2_commit_t2);
  pp += SIZE_POLYQ2_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->NTTq1_commit_t1_prime);
  pp += SIZE_POLYQ1_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->NTTq2_commit_t2_prime);
  pp += SIZE_POLYQ2_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->NTTQ_enc1);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->NTTQ_enc2_V3[0]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->NTTQ_enc2_V3[1]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->NTTQ_enc2_V3[2]);
  pp += SIZE_POLYQ_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z_V3[0]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z_V3[1]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z_V3[2]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z_prime_V3[0]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z_prime_V3[1]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z_prime_V3[2]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z1_V3[0]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z1_V3[1]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z1_V3[2]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z5_V3[0]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z5_V3[1]);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->Rq1_z5_V3[2]);
  pp += SIZE_POLYQ1_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->RQ_zenc_V5[0]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->RQ_zenc_V5[1]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->RQ_zenc_V5[2]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->RQ_zenc_V5[3]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, pres_in->RQ_zenc_V5[4]);
  pp += SIZE_POLYQ_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->Rq2_zs1_V4[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->Rq2_zs1_V4[1]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->Rq2_zs1_V4[2]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->Rq2_zs1_V4[3]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->Rq2_zs2_V2[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, pres_in->Rq2_zs2_V2[1]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, pres_in->challenge_chall);
  return EXIT_SUCCESS;
}

int conv_p_uint8_to_presentation (s_presentation_t pres_out, uint8_t *p_u8_in)
{
  int pp = 0;
  conv_p_uint8_to_polyq1(pres_out->NTTq1_commit_t1, p_u8_in);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq2(pres_out->NTTq2_commit_t2, p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq1(pres_out->NTTq1_commit_t1_prime, p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq2(pres_out->NTTq2_commit_t2_prime, p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyQ(pres_out->NTTQ_enc1, p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->NTTQ_enc2_V3[0], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->NTTQ_enc2_V3[1], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->NTTQ_enc2_V3[2], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z_V3[0], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z_V3[1], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z_V3[2], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z_prime_V3[0], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z_prime_V3[1], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z_prime_V3[2], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z1_V3[0], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z1_V3[1], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z1_V3[2], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z5_V3[0], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z5_V3[1], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(pres_out->Rq1_z5_V3[2], p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyQ(pres_out->RQ_zenc_V5[0], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->RQ_zenc_V5[1], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->RQ_zenc_V5[2], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->RQ_zenc_V5[3], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(pres_out->RQ_zenc_V5[4], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyq2(pres_out->Rq2_zs1_V4[0], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(pres_out->Rq2_zs1_V4[1], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(pres_out->Rq2_zs1_V4[2], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(pres_out->Rq2_zs1_V4[3], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(pres_out->Rq2_zs2_V2[0], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(pres_out->Rq2_zs2_V2[1], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq1(pres_out->challenge_chall, p_u8_in + pp);
  return EXIT_SUCCESS;
}

int conv_ipk_opk_to_p_uint8 (uint8_t *p_u8_out, s_issuer_pk_t ipk_in, s_opener_pk_t opk_in)
{
  int pp = 0;
  /* conv of issuer pk */
  conv_polyq1_to_p_uint8(p_u8_out + pp, ipk_in->NTTq1_commit_a1);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, ipk_in->NTTq1_commit_a2);
  pp += SIZE_POLYQ1_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_commit_a3);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_a_V2[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_a_V2[1]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_b_V2[0]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_b_V2[1]);
  pp += SIZE_POLYQ2_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, ipk_in->NTTq1_sigma1_commit_a1);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, ipk_in->NTTq1_sigma1_commit_a2);
  pp += SIZE_POLYQ1_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_sigma1_commit_a3);
  pp += SIZE_POLYQ2_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, ipk_in->NTTq1_sigma5_commit_a1);
  pp += SIZE_POLYQ1_S8;
  conv_polyq1_to_p_uint8(p_u8_out + pp, ipk_in->NTTq1_sigma5_commit_a2);
  pp += SIZE_POLYQ1_S8;
  conv_polyq2_to_p_uint8(p_u8_out + pp, ipk_in->NTTq2_sigma5_commit_a3);
  pp += SIZE_POLYQ2_S8;

  /* conv of opener pk */
  conv_polyQ_to_p_uint8(p_u8_out + pp, opk_in->NTTQ_a);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, opk_in->NTTQ_b_V3[0]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, opk_in->NTTQ_b_V3[1]);
  pp += SIZE_POLYQ_S8;
  conv_polyQ_to_p_uint8(p_u8_out + pp, opk_in->NTTQ_b_V3[2]);
  return EXIT_SUCCESS;
}

int conv_p_uint8_to_ipk_opk (s_issuer_pk_t ipk_out, s_opener_pk_t opk_out, uint8_t *p_u8_in)
{
  int pp = 0;
  /* conv of issuer pk */
  conv_p_uint8_to_polyq1(ipk_out->NTTq1_commit_a1, p_u8_in);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(ipk_out->NTTq1_commit_a2, p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_commit_a3, p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_a_V2[0], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_a_V2[1], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_b_V2[0], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_b_V2[1], p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq1(ipk_out->NTTq1_sigma1_commit_a1, p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(ipk_out->NTTq1_sigma1_commit_a2, p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_sigma1_commit_a3, p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;
  conv_p_uint8_to_polyq1(ipk_out->NTTq1_sigma5_commit_a1, p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq1(ipk_out->NTTq1_sigma5_commit_a2, p_u8_in + pp);
  pp += SIZE_POLYQ1_S8;
  conv_p_uint8_to_polyq2(ipk_out->NTTq2_sigma5_commit_a3, p_u8_in + pp);
  pp += SIZE_POLYQ2_S8;

  /* conv of opener pk */
  conv_p_uint8_to_polyQ(opk_out->NTTQ_a, p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(opk_out->NTTQ_b_V3[0], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(opk_out->NTTQ_b_V3[1], p_u8_in + pp);
  pp += SIZE_POLYQ_S8;
  conv_p_uint8_to_polyQ(opk_out->NTTQ_b_V3[2], p_u8_in + pp);
  return EXIT_SUCCESS;
}

int anonymous_credential_start ()
{
  int pp = 0;
  protocol_init ();
  for (int i=0; i<5; i++)
  {
    conv_p_uint8_to_polyq2 (NTTq2_fake_hash_V256[i], demo_hash + pp);
      pp += SIZE_POLYQ2_S8;
  }
  
  return EXIT_SUCCESS;
}

int anonymous_credential_stop ()
{
  protocol_clear ();
  return EXIT_SUCCESS;
}

int get_presentation_token (uint8_t *demo_pres_out, uint8_t *demo_cred_in, uint8_t *demo_msg_in)
{
  int err = EXIT_SUCCESS;

  s_issuer_pk_t ipk;
  s_opener_pk_t opk;
  attribute_t attr;
  intq2_t id;
  s_credential_t cred;
  s_presentation_t pres;

  err |= issuer_pk_init (ipk);
  err |= opener_pk_init (opk);
  err |= credential_init (cred);
  err |= presentation_init (pres);

  conv_p_uint8_to_ipk_opk_attr_cred_id (ipk, opk, &attr, cred, id, demo_cred_in);

  presentation_generation (pres, cred, id, ipk, opk, attr, demo_msg_in);

  conv_mess_attr_pres_to_p_uint8 (demo_pres_out, demo_msg_in, pres, attr);

  err |= issuer_pk_clear (ipk);
  err |= opener_pk_clear (opk);
  err |= credential_clear (cred);
  err |= presentation_clear (pres);

  return err;
}

int verify_presentation_token (uint8_t *demo_pres_in, uint8_t *demo_pks_in)
{
  int ret;
  s_presentation_t pres;
  s_issuer_pk_t ipk;
  s_opener_pk_t opk;
  attribute_t attr;
  uint8_t msg[SIZE_MESS_S8];

  issuer_pk_init (ipk);
  opener_pk_init (opk);
  presentation_init (pres);
  
  conv_p_uint8_to_ipk_opk (ipk, opk, demo_pks_in);
  conv_p_uint8_to_mess_attr_pres (msg, pres, &attr, demo_pres_in);

  ret = presentation_verify (pres, ipk, opk, attr, msg);

  issuer_pk_clear (ipk);
  opener_pk_clear (opk);
  presentation_clear (pres);

  return ret;
}

int demonstrator_init ()
{
  int err = EXIT_SUCCESS, i, pp = 0;

  FILE *pblc_data, *prvt_phoebe, *prvt_emma, *prvt_julian, *prvt_edward, *pblc_hash;

  pblc_data = fopen("pblc_data.ac","w");
  pblc_hash = fopen("pblc_hash.ac","w");
  prvt_phoebe = fopen("prvt_phoebe.ac","w");
  prvt_emma = fopen("prvt_emma.ac","w");
  prvt_julian = fopen("prvt_julian.ac","w");
  prvt_edward = fopen("prvt_edward.ac","w");

  uint8_t demo_public_keys[SIZE_IPK_OPK_S8];
  uint8_t demo_cred[DEMO_CRED_BYTELENGTH];
  uint8_t inner_demo_hash[5*SIZE_POLYQ2_S8];

  intq2_t id_phoebe = {1,0,0};
  intq2_t id_emma = {2,0,0}; 
  intq2_t id_julian = {3,0,0};
  intq2_t id_edward = {4,0,0};

  attribute_t age_group_1 = 0x01;
  attribute_t age_group_2 = 0x02;
  attribute_t age_group_3 = 0x03;
  attribute_t age_group_4 = 0x04;

  s_issuer_keys_t ikeys;
  s_opener_keys_t okeys;

  s_credential_t cred_phoebe;
  s_credential_t cred_emma;
  s_credential_t cred_julian;
  s_credential_t cred_edward;

  err |= issuer_parameters_init (ikeys);
  err |= opener_parameters_init (okeys);

  err |= credential_init (cred_phoebe);
  err |= credential_init (cred_emma);
  err |= credential_init (cred_julian);
  err |= credential_init (cred_edward);

  err |= issuer_parameters_generation (ikeys);
  err |= opener_parameters_generation (okeys);

  err |= credential_generation (cred_phoebe, ikeys, id_phoebe, age_group_4);
  err |= credential_generation (cred_emma, ikeys, id_emma, age_group_1);
  err |= credential_generation (cred_julian, ikeys, id_julian, age_group_2);
  err |= credential_generation (cred_edward, ikeys, id_edward, age_group_3);

  for (i=0; i<5; i++)
  {
      conv_polyq2_to_p_uint8 (inner_demo_hash + pp, NTTq2_fake_hash_V256[i]);
      pp += SIZE_POLYQ2_S8;
  }
  
  fprintf(pblc_hash, "uint8_t demo_hash[%d] = {", 5*SIZE_POLYQ2_S8);
  for (i=0; i<5*SIZE_POLYQ2_S8; i++)
  {
    if (i>0)
    {
      fprintf(pblc_hash, ", ");
    }
    if (i%10 == 0)
    {
      fprintf(pblc_hash, "\n");
    }
    fprintf(pblc_hash, "0x%02x", (unsigned char)inner_demo_hash[i]);
  }
  fprintf(pblc_hash, "};");

  err |= conv_ipk_opk_to_p_uint8 (demo_public_keys, ikeys->pk, okeys->pk);

  fprintf(pblc_data, "uint8_t demo_public_keys[%d] = {", SIZE_IPK_OPK_S8);
  for (i=0; i<SIZE_IPK_OPK_S8; i++)
  {
    if (i>0)
    {
      fprintf(pblc_data, ", ");
    }
    if (i%10 == 0)
    {
      fprintf(pblc_data, "\n");
    }
    fprintf(pblc_data, "0x%02x", (unsigned char)demo_public_keys[i]);
  }
  fprintf(pblc_data, "};");

  for (i=0; i<SIZE_IPK_OPK_S8; i++)
  {
    demo_cred[i] = demo_public_keys[i];
  }

  // Phoebe credential
  conv_ipk_opk_attr_cred_id_to_p_uint8 (demo_cred, ikeys->pk, okeys->pk, age_group_4, cred_phoebe, id_phoebe);
  fprintf(prvt_phoebe, "uint8_t demo_cred_phoebe[%d] = {", DEMO_CRED_BYTELENGTH);

  for (i=0; i<DEMO_CRED_BYTELENGTH; i++)
  {
    if (i>0)
    {
      fprintf(prvt_phoebe, ", ");
    }
    if (i%10 == 0)
    {
      fprintf(prvt_phoebe, "\n");
    }
    fprintf(prvt_phoebe, "0x%02x", (unsigned char)demo_cred[i]);
  }
  fprintf(prvt_phoebe, "};");

  // Emma credential
  conv_ipk_opk_attr_cred_id_to_p_uint8 (demo_cred, ikeys->pk, okeys->pk, age_group_1, cred_emma, id_emma);
  fprintf(prvt_emma, "uint8_t demo_cred_emma[%d] = {", DEMO_CRED_BYTELENGTH);
  for (i=0; i<DEMO_CRED_BYTELENGTH; i++)
  {
    if (i>0)
    {
      fprintf(prvt_emma, ", ");
    }
    if (i%10 == 0)
    {
      fprintf(prvt_emma, "\n");
    }
    fprintf(prvt_emma, "0x%02x", (unsigned char)demo_cred[i]);
  }
  fprintf(prvt_emma, "};");

  // Julian credential
  conv_ipk_opk_attr_cred_id_to_p_uint8 (demo_cred, ikeys->pk, okeys->pk, age_group_2, cred_julian, id_julian);
  fprintf(prvt_julian, "uint8_t demo_cred_julian[%d] = {", DEMO_CRED_BYTELENGTH);
  for (i=0; i<DEMO_CRED_BYTELENGTH; i++)
  {
    if (i>0)
    {
      fprintf(prvt_julian, ", ");
    }
    if (i%10 == 0)
    {
      fprintf(prvt_julian, "\n");
    }
    fprintf(prvt_julian, "0x%02x", (unsigned char)demo_cred[i]);
  }
  fprintf(prvt_julian, "};");

  // Edward credential
  conv_ipk_opk_attr_cred_id_to_p_uint8 (demo_cred, ikeys->pk, okeys->pk, age_group_3, cred_edward, id_edward);
  fprintf(prvt_edward, "uint8_t demo_cred_edward[%d] = {", DEMO_CRED_BYTELENGTH);
  for (i=0; i<DEMO_CRED_BYTELENGTH; i++)
  {
    if (i>0)
    {
      fprintf(prvt_edward, ", ");
    }
    if (i%10 == 0)
    {
      fprintf(prvt_edward, "\n");
    }
    fprintf(prvt_edward, "0x%02x", (unsigned char)demo_cred[i]);
  }
  fprintf(prvt_edward, "};");

  err |= fclose(pblc_data);
  err |= fclose(prvt_phoebe);
  err |= fclose(prvt_emma);
  err |= fclose(prvt_julian);
  err |= fclose(prvt_edward);

  err |= credential_clear (cred_phoebe);
  err |= credential_clear (cred_emma);
  err |= credential_clear (cred_julian);
  err |= credential_clear (cred_edward);

  err |= issuer_parameters_clear (ikeys);
  err |= opener_parameters_clear (okeys);

  return err;
}

int verify_presentation_token_demo (uint8_t *demo_pres_in)
{
  return verify_presentation_token (demo_pres_in, fixed_demo_public_keys);
}
