#ifndef PROTOCOL_H
#define PROTOCOL_H

#define SIZE_MESS_S8 10

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "params.h"

#include "new_arithm_int.h"
#include "new_arithm_poly.h"


typedef intq2_t intq2_t;
typedef uint8_t attribute_t;
/* the message M that is added to a presentation */
typedef uint8_t message_t[SIZE_MESS_S8];

struct s_issuer_pk_intern_t
{
  /* will be for the matrix (1, a1, a2) that define the matrix A1 of the commitment pk */
  polyq1_t NTTq1_commit_a1;
  polyq1_t NTTq1_commit_a2;
  /* will be fo the matrix (1, 1, a3) that define the matrix A2 of the commitment pk */
  polyq2_t NTTq2_commit_a3;
  /* for the a that is uniformly taken in R^2_q2 */
  polyq2_t NTTq2_a_V2[2];
  /* for the b = R a with R uniformly taken in S1^{2x2} */
  polyq2_t NTTq2_b_V2[2];
  /* computation of sigma_-1 and sigma_5 of the commitment matrix. */
  polyq1_t NTTq1_sigma1_commit_a1;
  polyq1_t NTTq1_sigma1_commit_a2;
  polyq2_t NTTq2_sigma1_commit_a3;
  polyq1_t NTTq1_sigma5_commit_a1;
  polyq1_t NTTq1_sigma5_commit_a2;
  polyq2_t NTTq2_sigma5_commit_a3;
};
typedef struct s_issuer_pk_intern_t s_issuer_pk_t[1];

struct s_issuer_sk_t
{
  /* the R that is uniformly taken in S_1^{2x2} */
  polyq2_t NTTq2_R_M22[3][3];
};
typedef struct s_issuer_sk_t s_issuer_sk_t[1];

struct s_issuer_keys_intern_t
{
  s_issuer_pk_t pk;
  s_issuer_sk_t sk;
};
typedef struct s_issuer_keys_intern_t s_issuer_keys_t[1];

struct s_opener_pk_intern_t
{
  /* the a of the verifiable encryption  */
  polyQ_t NTTQ_a;
  /* the b of the verifiable encryption that is equal to as + e */
  polyQ_t NTTQ_b_V3[3];
};
typedef struct s_opener_pk_intern_t s_opener_pk_t[1];

struct s_opener_sk_intern_t
{
  /* the s of the verifiable encryption  */
  polyQ_t NTTQ_s_V3[3];
  /* the e of the verifiable encryption */
  polyQ_t NTTQ_e_V3[3];
};
typedef struct s_opener_sk_intern_t s_opener_sk_t[1];

struct s_opener_keys_intern_t
{
  s_opener_pk_t pk;
  s_opener_sk_t sk;
};
typedef struct s_opener_keys_intern_t s_opener_keys_t[1];

struct s_credential_intern_t
{
  polyq2_t NTTq2_s1_V2[2];
  polyq2_t NTTq2_s2_V2[2];
  polyq2_t NTTq2_s3_V2[2];
};
typedef struct s_credential_intern_t s_credential_t[1];

struct s_presentation_intern_t
{
  polyq1_t NTTq1_commit_t1;
  polyq2_t NTTq2_commit_t2;
  polyq1_t NTTq1_commit_t1_prime;
  polyq2_t NTTq2_commit_t2_prime;
  polyQ_t NTTQ_enc1;
  polyQ_t NTTQ_enc2_V3[3];
  polyq1_t Rq1_z_V3[3];
  polyq1_t Rq1_z_prime_V3[3];
  polyq1_t Rq1_z1_V3[3];
  polyq1_t Rq1_z5_V3[3];
  polyQ_t RQ_zenc_V5[5];
  polyq2_t Rq2_zs1_V4[4];
  polyq2_t Rq2_zs2_V2[2];
  challenge_t challenge_chall;
};
typedef struct s_presentation_intern_t s_presentation_t[1];


/* fake_hash is used instead of a real hash function 
 * for the generation of credentials : this solution allows us
 * to create valid credentials even if the credential_generation algorithm
 * is incomplete. */
extern polyq2_t NTTq2_fake_hash_V256[256];
void fake_hash_init ();
void fake_hash_clear ();

/* init randomness of the protocol and the array NTTq2_fake_hash_V256 */
int protocol_init ();
/* clear randomness of the protocol and the array NTTq2_fake_hash_V256 */
int protocol_clear ();

int issuer_pk_init (s_issuer_pk_t ipk);
int issuer_sk_init (s_issuer_sk_t isk);
int issuer_parameters_init (s_issuer_keys_t ikeys);
int issuer_pk_clear (s_issuer_pk_t ipk);
int issuer_sk_clear (s_issuer_sk_t isk);
int issuer_parameters_clear (s_issuer_keys_t ikeys);
int issuer_parameters_generation (s_issuer_keys_t ikeys);

int opener_pk_init (s_opener_pk_t opk);
int opener_sk_init (s_opener_sk_t osk);
int opener_parameters_init (s_opener_keys_t okeys);
int opener_pk_clear (s_opener_pk_t opk);
int opener_sk_clear (s_opener_sk_t osk);
int opener_parameters_clear (s_opener_keys_t okeys);
int opener_parameters_generation (s_opener_keys_t okeys);

int credential_init (s_credential_t cred);
int credential_clear (s_credential_t cred);
int credential_generation (s_credential_t cred, s_issuer_keys_t ikeys,
                            intq2_t id, const attribute_t attr);
int credential_verify (s_credential_t cred, s_issuer_pk_t ipk,
                        intq2_t id, const attribute_t attr);
int presentation_init (s_presentation_t pres);
int presentation_clear (s_presentation_t pres);
int presentation_generation (s_presentation_t pres, s_credential_t cred, intq2_t id, s_issuer_pk_t ipk, s_opener_pk_t opk, attribute_t attr, message_t message);
int presentation_verify (s_presentation_t pres, s_issuer_pk_t ipk, s_opener_pk_t opk,
                         attribute_t attr, message_t message);

 void
_mul_A1_NTTq1 (polyq1_t res, const s_issuer_pk_t ipk, polyq1_t u1, polyq1_t u2,
               polyq1_t u3, polyq1_t tmp);

 void
_mul_sigma1_A1_NTTq1 (polyq1_t res, const s_issuer_pk_t ipk, polyq1_t u1,
                      polyq1_t u2, polyq1_t u3, polyq1_t tmp);

 void
_mul_sigma5_A1_NTTq1 (polyq1_t res, const s_issuer_pk_t ipk, polyq1_t u1,
                      polyq1_t u2, polyq1_t u3, polyq1_t tmp);

 void
_mul_A2_NTTq2 (polyq2_t res, const s_issuer_pk_t ipk, polyq2_t u2, polyq2_t u3,
               polyq2_t tmp);
 void
_mul_sigma1_A2_NTTq2 (polyq2_t res, const s_issuer_pk_t ipk, polyq2_t u2,
                      polyq2_t u3, polyq2_t tmp);

 void
_mul_sigma5_A2_NTTq2 (polyq2_t res, const s_issuer_pk_t ipk, polyq2_t u2,
                      polyq2_t u3, polyq2_t tmp);

 void
_computation_w1_NTTq1 (polyq1_t NTTq1_w1, s_issuer_pk_t ipk,
                       polyq1_t *NTTq1_sample_xi_y_V3, polyq1_t NTTq1_tmp);

 void
_recomputation_w1_NTTq1 (polyq1_t NTTq1_w1, s_issuer_pk_t ipk,
                         polyq1_t NTTq1_chall, polyq1_t NTTq1_commit_t1,
                         polyq1_t *NTTq1_z_V3, polyq1_t NTTq1_tmp);

 void
_computation_w1_prime_NTTq1 (polyq1_t NTTq1_w1_prime, s_issuer_pk_t ipk,
                             polyq1_t *NTTq1_sample_xi_y_prime_V3,
                             polyq1_t NTTq1_tmp);

 void
_recomputation_w1_prime_NTTq1 (polyq1_t NTTq1_w1_prime, s_issuer_pk_t ipk,
                               polyq1_t NTTq1_chall,
                               polyq1_t NTTq1_commit_t1_prime,
                               polyq1_t *NTTq1_z_prime_V3, polyq1_t NTTq1_tmp);

 void
_computation_w2_NTTq2 (polyq2_t NTTq2_w2, s_issuer_pk_t ipk,
                       polyq2_t *NTTq2_sample_xi_y_V3,
                       polyq2_t *NTTq2_sample_xi_y_prime_V3,
                       polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2,
                       polyq2_t NTTq2_tmp3);

 void
_recomputation_w2_NTTq2 (polyq2_t NTTq2_w2, s_issuer_pk_t ipk,
                         polyq2_t NTTq2_chall, polyq2_t NTTq2_commit_t2,
                         polyq2_t NTTq2_commit_t2_prime, polyq2_t *NTTq2_z_V3,
                         polyq2_t *NTTq2_z_prime_V3, polyq2_t NTTq2_tmp,
                         polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3);

 void
_computation_w11_NTTq1 (polyq1_t NTTq1_w11, s_issuer_pk_t ipk,
                        polyq1_t *NTTq1_sample_xi_y_1_V3, polyq1_t NTTq1_tmp);

 void
_recomputation_w11_NTTq1 (polyq1_t NTTq1_w11, s_issuer_pk_t ipk,
                          polyq1_t NTTq1_chall,
                          polyq1_t NTTq1_sigma_1_commit_t1,
                          polyq1_t *NTTq1_z1_V3, polyq1_t NTTq1_tmp);

 void
_computation_w51_NTTq1 (polyq1_t NTTq1_w51, s_issuer_pk_t ipk,
                        polyq1_t *NTTq1_sample_xi_y_5_V3, polyq1_t NTTq1_tmp);

 void
_recomputation_w51_NTTq1 (polyq1_t NTTq1_w51, s_issuer_pk_t ipk,
                          polyq1_t NTTq1_chall,
                          polyq1_t NTTq1_sigma_5_commit_t1,
                          polyq1_t *NTTq1_z5_V3, polyq1_t NTTq1_tmp);

 void
_computation_w12_NTTq2 (polyq2_t NTTq2_w12, s_issuer_pk_t ipk,
                        polyq2_t *NTTq2_sample_xi_y_V3,
                        polyq2_t *NTTq2_sample_xi_y_1_V3, polyq2_t NTTq2_tmp,
                        polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3);

 void
_recomputation_w12_NTTq2 (polyq2_t NTTq2_w12, s_issuer_pk_t ipk,
                          polyq2_t NTTq2_chall, polyq2_t NTTq2_commit_t2,
                          polyq2_t NTTq2_sigma_1_commit_t2,
                          polyq2_t *NTTq2_z_V3, polyq2_t *NTTq2_z1_V3,
                          polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2,
                          polyq2_t NTTq2_tmp3);

void
_computation_w52_NTTq2 (polyq2_t NTTq2_w52, s_issuer_pk_t ipk,
                        polyq2_t *NTTq2_sample_xi_y_V3,
                        polyq2_t *NTTq2_sample_xi_y_5_V3, polyq2_t NTTq2_tmp,
                        polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3);

void
_recomputation_w52_NTTq2 (polyq2_t NTTq2_w52, s_issuer_pk_t ipk,
                          polyq2_t NTTq2_chall, polyq2_t NTTq2_commit_t2,
                          polyq2_t NTTq2_sigma_5_commit_t2,
                          polyq2_t *NTTq2_z_V3, polyq2_t *NTTq2_z5_V3, 
                          polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3);

 void
_computation_wenc_NTTQ (polyQ_t *NTTQ_wenc_V4, s_opener_pk_t opk,
                        polyQ_t *NTTQ_sample_xi_y_enc_V5,
                        polyQ_t *NTTQ_sample_xi_y_V3, polyQ_t NTTQ_tmp);

void
_recomputation_wenc_NTTQ (polyQ_t *NTTQ_wenc_V4, s_opener_pk_t opk, polyQ_t NTTQ_chall,
                         polyQ_t *NTTQ_z_V3, polyQ_t *NTTQ_zenc_V5,
                         polyQ_t NTTQ_enc1,
                         polyQ_t *NTTQ_enc2_V3, polyQ_t NTTQ_tmp);

 void
_computation_ws_NTTq2 (polyq2_t NTTq2_ws, s_issuer_pk_t ipk,
                       polyq2_t NTTq2_commit_t2,
                       polyq2_t NTTq2_commit_t2_prime,
                       polyq2_t *NTTq2_sample_xi1_y_s1_V4,
                       polyq2_t *NTTq2_sample_xi2_y_s2_V2, polyq2_t NTTq2_tmp);

 void
_recomputation_ws_NTTq2 (polyq2_t NTTq2_ws, s_issuer_pk_t ipk,
                         polyq2_t NTTq2_chall, polyq2_t NTTq2_commit_t2,
                         polyq2_t NTTq2_commit_t2_prime,
                         polyq2_t *NTTq2_zs1_V4, polyq2_t *NTTq2_zs2_V2,
                         polyq2_t NTTq2_tmp, attribute_t attr);

 void
_computation_z_rc_NTTq1 (polyq1_t *NTTq1_z_V3, polyq1_t *NTTq1_rc_V3 ,
                         polyq1_t NTTq1_r1, polyq1_t NTTq1_r2, polyq1_t NTTq1_r3,
                      polyq1_t NTTq1_chall, polyq1_t *NTTq1_sample_xi_y_V3);

 void
_computation_z_rc_prime_NTTq1 (polyq1_t *NTTq1_z_prime_V3, polyq1_t *NTTq1_rc_prime_V3,
                            polyq1_t NTTq1_r1_prime, polyq1_t NTTq1_r2_prime,
                            polyq1_t NTTq1_r3_prime, polyq1_t NTTq1_chall,
                            polyq1_t *NTTq1_sample_xi_y_prime_V3);

 void
_computation_z_rc_1_NTTq1 (polyq1_t *NTTq1_z1_V3,  polyq1_t *NTTq1_rc1_V3,
                           polyq1_t NTTq1_sigma1_r1, polyq1_t NTTq1_sigma1_r2, polyq1_t NTTq1_sigma1_r3,
                       polyq1_t NTTq1_chall, polyq1_t *NTTq1_sample_xi_y_1_V3);

 void
_computation_z_rc_5_NTTq1 (polyq1_t *NTTq1_z5_V3, polyq1_t *NTTq1_rc5_V3,
                           polyq1_t NTTq1_sigma5_r1, polyq1_t NTTq1_sigma5_r2, polyq1_t NTTq1_sigma5_r3,
                       polyq1_t NTTq1_chall, polyq1_t *NTTq1_sample_xi_y_5_V3);

void
_computation_z_rc_enc_NTTQ (polyQ_t *NTTQ_zenc_V5, polyQ_t *NTTQ_rcenc_V5, 
                           polyQ_t NTTQ_chall, polyQ_t NTTQ_r_enc, polyQ_t NTTQ_e1_enc,
                         polyQ_t NTTQ_e2_1_enc, polyQ_t NTTQ_e2_2_enc,
                         polyQ_t NTTQ_e2_3_enc,
                         polyQ_t *NTTQ_sample_xi_y_enc_V5);

 void
_computation_z_rc_s1_NTTq2 (polyq2_t *NTTq2_zs1_V4, polyq2_t *NTTq2_rcs1_V4, 
                           s_credential_t cred, polyq2_t NTTq2_chall,
                        polyq2_t *NTTq2_sample_xi1_y_s1_V4);

 void
_computation_z_rc_s2_NTTq2 (polyq2_t *NTTq2_zs2_V2, polyq2_t *NTTq2_rcs2_V2, s_credential_t cred, 
                        polyq2_t NTTq2_chall,
                        polyq2_t *NTTq2_sample_xi2_y_s2_V2, polyq2_t NTTq2_r2,
                        polyq2_t NTTq2_r3, polyq2_t NTTq2_r2_prime,
                        polyq2_t NTTq2_r3_prime, polyq2_t NTTq2_tmp);

void
_mul_B_enc(polyQ_t res0, polyQ_t res1, polyQ_t res2, polyQ_t res3,
           s_opener_pk_t opk, polyQ_t x0, polyQ_t x1, polyQ_t x2, polyQ_t x3, polyQ_t x4, 
           polyQ_t NTTQ_r1, polyQ_t NTTQ_r2,polyQ_t NTTQ_r3, polyQ_t NTTQ_tmp);

void _mul_BIGMAT_CRED_NTTq2(polyq2_t res, s_issuer_pk_t ipk, 
                            intq2_t id, intq2_t delta_id,
                            polyq2_t x0, polyq2_t x1, polyq2_t x2,
                            polyq2_t x3, polyq2_t x5, polyq2_t x6,
                            polyq2_t NTTq2_tmp);

void _mul_BIGMAT_PRES_NTTq2(polyq2_t res, s_issuer_pk_t ipk, 
                            polyq2_t commit_t2, polyq2_t commit_t2_prime,
                            polyq2_t x0, polyq2_t x1, polyq2_t x2,
                            polyq2_t x3, polyq2_t x5, polyq2_t x6,
                            polyq2_t NTTq2_tmp);
void
_computation_intermediate_rc_zs2_NTT2(polyq2_t res1, polyq2_t res2, 
                          polyq2_t r2, polyq2_t r3, 
                          polyq2_t r2_prime, polyq2_t r3_prime,
                          polyq2_t s2_1, polyq2_t s2_2, 
                          polyq2_t s3_1, polyq2_t s3_2, 
                          polyq2_t NTTq2_tmp);

/* 1 if equal, 0 else */
int cred_equal(s_credential_t cred1, s_credential_t cred2);
int pres_equal(s_presentation_t pres1, s_presentation_t pres2);
int opk_equal(s_opener_pk_t opk1, s_opener_pk_t opk2);
int ipk_equal(s_issuer_pk_t ipk1, s_issuer_pk_t opk2);
#endif /* PROTOCOL_H */
