#include "extra_tools.h"
#include "hash.h"
#include "samplers.h"
#include "shake.h"
#include "assert.h"

#include "protocol.h"

polyq2_t NTTq2_fake_hash_V256[256];

  void
  fake_hash_init ()
  {
    for (int i = 0; i < 256; i++)
      {
        polyq2_init (NTTq2_fake_hash_V256[i]);
      }
  }

  void
  fake_hash_clear ()
  {
    for (int i = 0; i < 256; i++)
      {
        polyq2_clear (NTTq2_fake_hash_V256[i]);
      }
  }

  /* Initiate the randomness and the fake hash function. */
  int
  protocol_init ()
  {
    rand_init ();
    fake_hash_init ();
    return EXIT_SUCCESS;
  }

  /* Clear the randomness and the fake hash function. */
  int
  protocol_clear ()
  {
    rand_clear ();
    fake_hash_clear ();
    return EXIT_SUCCESS;
  }

  int
  issuer_pk_init (s_issuer_pk_t ipk)
  {
    polyq1_init (ipk->NTTq1_commit_a1);
    polyq1_init (ipk->NTTq1_commit_a2);
    polyq2_init (ipk->NTTq2_commit_a3);
    polyq2_init_V (ipk->NTTq2_a_V2,2);
    polyq2_init_V (ipk->NTTq2_b_V2,2);

    polyq1_init (ipk->NTTq1_sigma1_commit_a1);
    polyq1_init (ipk->NTTq1_sigma1_commit_a2);
    polyq2_init (ipk->NTTq2_sigma1_commit_a3);
    polyq1_init (ipk->NTTq1_sigma5_commit_a1);
    polyq1_init (ipk->NTTq1_sigma5_commit_a2);
    polyq2_init (ipk->NTTq2_sigma5_commit_a3);
    return EXIT_SUCCESS;
  }

  int
  issuer_sk_init (s_issuer_sk_t isk)
  {
    polyq2_init (isk->NTTq2_R_M22[0][0]);
    polyq2_init (isk->NTTq2_R_M22[0][1]);
    polyq2_init (isk->NTTq2_R_M22[1][0]);
    polyq2_init (isk->NTTq2_R_M22[1][1]);
    return EXIT_SUCCESS;
  }

  int
  issuer_parameters_init (s_issuer_keys_t ikeys)
  {

    issuer_pk_init (ikeys->pk);
    issuer_sk_init (ikeys->sk);
    return EXIT_SUCCESS;
  }

  int
  issuer_pk_clear (s_issuer_pk_t ipk)
  {
    polyq1_clear (ipk->NTTq1_commit_a1);
    polyq1_clear (ipk->NTTq1_commit_a2);
    polyq2_clear (ipk->NTTq2_commit_a3);
    polyq2_clear_V (ipk->NTTq2_a_V2, 2);
    polyq2_clear_V (ipk->NTTq2_b_V2, 2);

    polyq1_clear (ipk->NTTq1_sigma1_commit_a1);
    polyq1_clear (ipk->NTTq1_sigma1_commit_a2);
    polyq2_clear (ipk->NTTq2_sigma1_commit_a3);
    polyq1_clear (ipk->NTTq1_sigma5_commit_a1);
    polyq1_clear (ipk->NTTq1_sigma5_commit_a2);
    polyq2_clear (ipk->NTTq2_sigma5_commit_a3);
    return EXIT_SUCCESS;
  }

  int
  issuer_sk_clear (s_issuer_sk_t isk)
  {
    polyq2_clear (isk->NTTq2_R_M22[0][0]);
    polyq2_clear (isk->NTTq2_R_M22[0][1]);
    polyq2_clear (isk->NTTq2_R_M22[1][0]);
    polyq2_clear (isk->NTTq2_R_M22[1][1]);
    return EXIT_SUCCESS;
  }

  int
  issuer_parameters_clear (s_issuer_keys_t ikeys)
  {
    issuer_pk_clear (ikeys->pk);
    issuer_sk_clear (ikeys->sk);
    return EXIT_SUCCESS;
  }

  int
  issuer_parameters_generation (s_issuer_keys_t ikeys)
  {
    /* The non-zero/non-one coefficients of the commitment matrix
       are taken uniformly. */
    get_random_polyq1 (ikeys->pk->NTTq1_commit_a1);
    get_random_polyq1 (ikeys->pk->NTTq1_commit_a2);
    get_random_polyq2 (ikeys->pk->NTTq2_commit_a3);

    /* Computation of sigma_-1 and sigma_5 of the commitment matrix. */
    polyq1_sigma_1 (ikeys->pk->NTTq1_sigma1_commit_a1,
                    ikeys->pk->NTTq1_commit_a1);
    polyq1_sigma_1 (ikeys->pk->NTTq1_sigma1_commit_a2,
                    ikeys->pk->NTTq1_commit_a2);
    polyq2_sigma_1 (ikeys->pk->NTTq2_sigma1_commit_a3,
                    ikeys->pk->NTTq2_commit_a3);

    polyq1_sigma_5 (ikeys->pk->NTTq1_sigma5_commit_a1,
                    ikeys->pk->NTTq1_commit_a1);
    polyq1_sigma_5 (ikeys->pk->NTTq1_sigma5_commit_a2,
                    ikeys->pk->NTTq1_commit_a2);
    polyq2_sigma_5 (ikeys->pk->NTTq2_sigma5_commit_a3,
                    ikeys->pk->NTTq2_commit_a3);

    /* We compute the NTT form. */
    conv_R_polyq1_to_NTT_polyq1 (ikeys->pk->NTTq1_commit_a1);
    conv_R_polyq1_to_NTT_polyq1 (ikeys->pk->NTTq1_commit_a2);
    conv_R_polyq2_to_NTT_polyq2 (ikeys->pk->NTTq2_commit_a3);

    conv_R_polyq1_to_NTT_polyq1 (ikeys->pk->NTTq1_sigma1_commit_a1);
    conv_R_polyq1_to_NTT_polyq1 (ikeys->pk->NTTq1_sigma1_commit_a2);
    conv_R_polyq2_to_NTT_polyq2 (ikeys->pk->NTTq2_sigma1_commit_a3);

    conv_R_polyq1_to_NTT_polyq1 (ikeys->pk->NTTq1_sigma5_commit_a1);
    conv_R_polyq1_to_NTT_polyq1 (ikeys->pk->NTTq1_sigma5_commit_a2);
    conv_R_polyq2_to_NTT_polyq2 (ikeys->pk->NTTq2_sigma5_commit_a3);

    /* Computation of a. */
    get_random_polyq2 (ikeys->pk->NTTq2_a_V2[0]);
    get_random_polyq2 (ikeys->pk->NTTq2_a_V2[1]);

    /* Computation of M. */
    get_random_challenge_NTTq2 (ikeys->sk->NTTq2_R_M22[0][0]);
    get_random_challenge_NTTq2 (ikeys->sk->NTTq2_R_M22[0][1]);
    get_random_challenge_NTTq2 (ikeys->sk->NTTq2_R_M22[1][0]);
    get_random_challenge_NTTq2 (ikeys->sk->NTTq2_R_M22[1][1]);

    return EXIT_SUCCESS;
  }

  int
  opener_pk_init (s_opener_pk_t opk)
  {
    polyQ_init (opk->NTTQ_a);
    polyQ_init_V (opk->NTTQ_b_V3, 3);
    return EXIT_SUCCESS;
  };

  int
  opener_sk_init (s_opener_sk_t osk)
  {
    polyQ_init_V (osk->NTTQ_s_V3,3);
    polyQ_init_V (osk->NTTQ_e_V3,3);

    return EXIT_SUCCESS;
  };

  int
  opener_parameters_init (s_opener_keys_t okeys)
  {
    opener_pk_init (okeys->pk);
    opener_sk_init (okeys->sk);
    return EXIT_SUCCESS;
  };

  int
  opener_pk_clear (s_opener_pk_t opk)
  {
    polyQ_clear (opk->NTTQ_a);
    polyQ_clear_V (opk->NTTQ_b_V3,3);
    return EXIT_SUCCESS;
  };

  int
  opener_sk_clear (s_opener_sk_t osk)
  {
    polyQ_clear_V (osk->NTTQ_s_V3,3);
    polyQ_clear_V (osk->NTTQ_e_V3,3);

    return EXIT_SUCCESS;
  };

  int
  opener_parameters_clear (s_opener_keys_t okeys)
  {
    opener_pk_clear (okeys->pk);
    opener_sk_clear (okeys->sk);
    return EXIT_SUCCESS;
  };

  int
  opener_parameters_generation (s_opener_keys_t okeys)
  {
    get_random_polyQ (okeys->pk->NTTQ_a);

    /* Computation of s. */
    get_random_challenge_NTTQ (okeys->sk->NTTQ_s_V3[0]);
    get_random_challenge_NTTQ (okeys->sk->NTTQ_s_V3[1]);
    get_random_challenge_NTTQ (okeys->sk->NTTQ_s_V3[2]);

    /* Computation of e. */
    get_random_challenge_NTTQ (okeys->sk->NTTQ_e_V3[0]);
    get_random_challenge_NTTQ (okeys->sk->NTTQ_e_V3[1]);
    get_random_challenge_NTTQ (okeys->sk->NTTQ_e_V3[2]);

    /* Computation of a*s, stored in b. */
    polyQ_mul_NTT (okeys->pk->NTTQ_b_V3[0], okeys->pk->NTTQ_a,
                   okeys->sk->NTTQ_s_V3[0]);
    polyQ_mul_NTT (okeys->pk->NTTQ_b_V3[1], okeys->pk->NTTQ_a,
                   okeys->sk->NTTQ_s_V3[1]);
    polyQ_mul_NTT (okeys->pk->NTTQ_b_V3[2], okeys->pk->NTTQ_a,
                   okeys->sk->NTTQ_s_V3[2]);

    /* Computation of b = as + e. */
    polyQ_add (okeys->pk->NTTQ_b_V3[0], okeys->pk->NTTQ_b_V3[0],
               okeys->sk->NTTQ_e_V3[0]);
    polyQ_add (okeys->pk->NTTQ_b_V3[1], okeys->pk->NTTQ_b_V3[1],
               okeys->sk->NTTQ_e_V3[1]);
    polyQ_add (okeys->pk->NTTQ_b_V3[2], okeys->pk->NTTQ_b_V3[2],
               okeys->sk->NTTQ_e_V3[2]);

    return EXIT_SUCCESS;
  }

  int
  credential_init (s_credential_t cred)
  {
    polyq2_init_V (cred->NTTq2_s1_V2,2);
    polyq2_init_V (cred->NTTq2_s2_V2,2);
    polyq2_init_V (cred->NTTq2_s3_V2,2);
    return EXIT_SUCCESS;
  }

  int
  credential_clear (s_credential_t cred)
  {
    polyq2_clear_V (cred->NTTq2_s1_V2,2);
    polyq2_clear_V (cred->NTTq2_s2_V2,2);
    polyq2_clear_V (cred->NTTq2_s3_V2,2);
    return EXIT_SUCCESS;
  }

/* res is the mult [a | b + [id   delta*id] | a_2 ]] * (x0, x1, x2, x3, 0, x4, x5, x6)). */
/* There is no x4 in arguments because it is multiplied by 0 (a2 begins by a 0). */
inline void _mul_BIGMAT_CRED_NTTq2(polyq2_t res, s_issuer_pk_t ipk, 
                                   intq2_t id, intq2_t delta_id,
                                   polyq2_t x0, polyq2_t x1, polyq2_t x2,
                                   polyq2_t x3, polyq2_t x5, polyq2_t x6,
                                   polyq2_t NTTq2_tmp)
{
  /* Mult by a^T. */
  polyq2_mul_NTT(res, ipk->NTTq2_a_V2[0], x0);
  polyq2_mul_NTT(NTTq2_tmp, ipk->NTTq2_a_V2[1], x1);
  polyq2_add(res, res, NTTq2_tmp);

  /* Mult by b^T + [id  delta*id]. */
  /* First term is (b^0 + id) * x2. */
  polyq2_add_intq2_NTT(NTTq2_tmp, ipk->NTTq2_b_V2[0], id);
  polyq2_mul_NTT(NTTq2_tmp, NTTq2_tmp, x2);
  polyq2_add(res, res, NTTq2_tmp);
  /* Second term is (b^1 + delta id) * x3 */
  polyq2_add_intq2_NTT(NTTq2_tmp, ipk->NTTq2_b_V2[1], delta_id);
  polyq2_mul_NTT(NTTq2_tmp, NTTq2_tmp, x3);
  polyq2_add(res, res, NTTq2_tmp);

  /* Mult by A_2^t = [0  1  commit_a3]^t */
  polyq2_add(res, res, x5);
  polyq2_mul_NTT(NTTq2_tmp, ipk->NTTq2_commit_a3, x6);
  polyq2_add(res, res, NTTq2_tmp);
}

/* res = the mult [a | b + [commit_t2   commmit_t2_prime] | a_2 ]] * (x0, x1, x2, x3, 0, x4, x5, x6) )*/
/* There is no x4 in arguments because it is multilied by 0 (a2 begins by a 0). */
inline void _mul_BIGMAT_PRES_NTTq2(polyq2_t res, s_issuer_pk_t ipk, 
                                   polyq2_t commit_t2, polyq2_t commit_t2_prime,
                                   polyq2_t x0, polyq2_t x1, polyq2_t x2,
                                   polyq2_t x3, polyq2_t x5, polyq2_t x6,
                                   polyq2_t NTTq2_tmp)
{
  /* mult by a^T */
  polyq2_mul_NTT(res, ipk->NTTq2_a_V2[0], x0);
  polyq2_mul_NTT(NTTq2_tmp, ipk->NTTq2_a_V2[1], x1);
  polyq2_add(res, res, NTTq2_tmp);

  /* mult by b^T + [commit_t2  commit_t2_prime] */
  /*** first term is (b^0 + commit_t2) * x2 */
  polyq2_add(NTTq2_tmp, ipk->NTTq2_b_V2[0], commit_t2);
  polyq2_mul_NTT(NTTq2_tmp, NTTq2_tmp, x2);
  polyq2_add(res, res, NTTq2_tmp);
  /*** second term is (b^1 + commit_t2_prime) * x3 */
  polyq2_add(NTTq2_tmp, ipk->NTTq2_b_V2[1], commit_t2_prime);
  polyq2_mul_NTT(NTTq2_tmp, NTTq2_tmp, x3);
  polyq2_add(res, res, NTTq2_tmp);

  /* mult by A_2^t = [0  1  commit_a3]^t */
  polyq2_add(res, res, x5);
  polyq2_mul_NTT(NTTq2_tmp, ipk->NTTq2_commit_a3, x6);
  polyq2_add(res, res, NTTq2_tmp);
}

  int
  credential_generation (s_credential_t cred, s_issuer_keys_t ikeys,
                         intq2_t id, const attribute_t attr)
  {
    /* e sample element as R_poly and then we will use the NTT to convert
       them to NTT_poly */
    polyq2_t NTTq2_tmp;
    polyq2_init (NTTq2_tmp);
    intq2_t delta_id;
    intq2_mul(delta_id, g_intq2_delta, id);


    get_gaussian_s_NTTq2 (cred->NTTq2_s1_V2[0]);
    get_gaussian_s_NTTq2 (cred->NTTq2_s1_V2[1]);
    get_gaussian_s_NTTq2 (cred->NTTq2_s2_V2[0]);
    get_gaussian_s_NTTq2 (cred->NTTq2_s2_V2[1]);
    /* Note that s3 contains only two elements and not three: one of it is multiplied by 0 in the next computation (because the first coefficient of a2 is 0), so there is no need to define it.  */
    get_gaussian_r_NTTq2 (cred->NTTq2_s3_V2[0]);
    get_gaussian_r_NTTq2 (cred->NTTq2_s3_V2[1]);

    /* computation of fake_hash_V256[attr] = [a|b + [id  delta*id]| a_2] [s1 s2 s3]^T */
    _mul_BIGMAT_CRED_NTTq2(NTTq2_fake_hash_V256[attr], ikeys->pk, 
                           id, delta_id,
                           cred->NTTq2_s1_V2[0],
                           cred->NTTq2_s1_V2[1],
                           cred->NTTq2_s2_V2[0],
                           cred->NTTq2_s2_V2[1],
                           cred->NTTq2_s3_V2[0],
                           cred->NTTq2_s3_V2[1],
                           NTTq2_tmp);
    polyq2_clear (NTTq2_tmp);
    return EXIT_SUCCESS;
  }

  int
  credential_verify (s_credential_t cred, s_issuer_pk_t ipk,
                     intq2_t id, const attribute_t attr)
  {
    int result = 0;

    polyq2_t NTTq2_tmp1;
    polyq2_t NTTq2_tmp2;

    polyq2_init (NTTq2_tmp1);
    polyq2_init (NTTq2_tmp2);

    intq2_t delta_id;
    intq2_mul(delta_id, g_intq2_delta, id);

    /* computation NTTq2_tmp1 <- [a|b + id g^T| a_2] [s1 s2 s3]^T */
    _mul_BIGMAT_CRED_NTTq2(NTTq2_tmp1, ipk, 
                           id, delta_id,
                           cred->NTTq2_s1_V2[0],
                           cred->NTTq2_s1_V2[1],
                           cred->NTTq2_s2_V2[0],
                           cred->NTTq2_s2_V2[1],
                           cred->NTTq2_s3_V2[0],
                           cred->NTTq2_s3_V2[1],
                           NTTq2_tmp2);

    /* we then check it is equal to fake_hash[attr]: 1 is equal, 0 else */
    result = polyq2_equal (NTTq2_tmp1, NTTq2_fake_hash_V256[attr]);

    polyq2_clear (NTTq2_tmp1);
    polyq2_clear (NTTq2_tmp2);
    return result;
  }

  int
  presentation_init (s_presentation_t pres)
  {
    polyq1_init (pres->NTTq1_commit_t1);
    polyq2_init (pres->NTTq2_commit_t2);
    polyq1_init (pres->NTTq1_commit_t1_prime);
    polyq2_init (pres->NTTq2_commit_t2_prime);
    polyQ_init (pres->NTTQ_enc1);
    polyQ_init_V (pres->NTTQ_enc2_V3,3);
    polyq1_init_V (pres->Rq1_z_V3,3);
    polyq1_init_V (pres->Rq1_z_prime_V3,3);
    polyq1_init_V (pres->Rq1_z1_V3,3);
    polyq1_init_V (pres->Rq1_z5_V3,3);
    polyQ_init_V (pres->RQ_zenc_V5,5);
    polyq2_init_V (pres->Rq2_zs1_V4,4);
    polyq2_init_V (pres->Rq2_zs2_V2,2);
    polyq1_init (pres->challenge_chall);
    return EXIT_SUCCESS;
  }

  int
  presentation_clear (s_presentation_t pres)
  {
    polyq1_clear (pres->NTTq1_commit_t1);
    polyq2_clear (pres->NTTq2_commit_t2);
    polyq1_clear (pres->NTTq1_commit_t1_prime);
    polyq2_clear (pres->NTTq2_commit_t2_prime);
    polyQ_clear (pres->NTTQ_enc1);
    polyQ_clear_V (pres->NTTQ_enc2_V3,3);
    polyq1_clear_V (pres->Rq1_z_V3,3);
    polyq1_clear_V (pres->Rq1_z_prime_V3,3);
    polyq1_clear_V (pres->Rq1_z1_V3,3);
    polyq1_clear_V (pres->Rq1_z5_V3,3);
    polyQ_clear_V (pres->RQ_zenc_V5,5);
    polyq2_clear_V (pres->Rq2_zs1_V4,4);
    polyq2_clear_V (pres->Rq2_zs2_V2,2);
    polyq1_clear (pres->challenge_chall);
    return EXIT_SUCCESS;
  }

  /* computation of res = A1 * [u1 u2 u3]^t */
  inline void
  _mul_A1_NTTq1 (polyq1_t res, const s_issuer_pk_t ipk, polyq1_t u1, polyq1_t u2,
                 polyq1_t u3, polyq1_t tmp)
  {
    polyq1_mul_NTT (tmp, ipk->NTTq1_commit_a1, u2);
    polyq1_add (res, tmp, u1);
    polyq1_mul_NTT (tmp, ipk->NTTq1_commit_a2, u3);
    polyq1_add (res, res, tmp);
  }

  inline void
  _mul_sigma1_A1_NTTq1 (polyq1_t res, const s_issuer_pk_t ipk, polyq1_t u1,
                        polyq1_t u2, polyq1_t u3, polyq1_t tmp)
  {
    polyq1_mul_NTT (tmp, ipk->NTTq1_sigma1_commit_a1, u2);
    polyq1_add (res, tmp, u1);
    polyq1_mul_NTT (tmp, ipk->NTTq1_sigma1_commit_a2, u3);
    polyq1_add (res, res, tmp);
  }

  inline void
  _mul_sigma5_A1_NTTq1 (polyq1_t res, const s_issuer_pk_t ipk, polyq1_t u1,
                        polyq1_t u2, polyq1_t u3, polyq1_t tmp)
  {
    polyq1_mul_NTT (tmp, ipk->NTTq1_sigma5_commit_a1, u2);
    polyq1_add (res, tmp, u1);
    polyq1_mul_NTT (tmp, ipk->NTTq1_sigma5_commit_a2, u3);
    polyq1_add (res, res, tmp);
  }

  /* computation of  res = A2 * [0 u2 u3]^t (there is no u1 because the first component of A2 is 0). */
  inline void
  _mul_A2_NTTq2 (polyq2_t res, const s_issuer_pk_t ipk, polyq2_t u2, polyq2_t u3,
                 polyq2_t tmp)
  {
    polyq2_mul_NTT (tmp, ipk->NTTq2_commit_a3, u3);
    polyq2_add (res, tmp, u2);
  }

  inline void
  _mul_sigma1_A2_NTTq2 (polyq2_t res, const s_issuer_pk_t ipk, polyq2_t u2,
                        polyq2_t u3, polyq2_t tmp)
  {
    polyq2_mul_NTT (tmp, ipk->NTTq2_sigma1_commit_a3, u3);
    polyq2_add (res, tmp, u2);
  }

  inline void
  _mul_sigma5_A2_NTTq2 (polyq2_t res, const s_issuer_pk_t ipk, polyq2_t u2,
                        polyq2_t u3, polyq2_t tmp)
  {
    polyq2_mul_NTT (tmp, ipk->NTTq2_sigma5_commit_a3, u3);
    polyq2_add (res, tmp, u2);
  }


  inline void
  _computation_w1_NTTq1 (polyq1_t NTTq1_w1, s_issuer_pk_t ipk,
                         polyq1_t *NTTq1_sample_xi_y_V3, polyq1_t NTTq1_tmp)
  {
    _mul_A1_NTTq1 (NTTq1_w1, ipk, NTTq1_sample_xi_y_V3[0],
                   NTTq1_sample_xi_y_V3[1], NTTq1_sample_xi_y_V3[2], NTTq1_tmp);
  }

  inline void
  _recomputation_w1_NTTq1 (polyq1_t NTTq1_w1, s_issuer_pk_t ipk,
                           polyq1_t NTTq1_chall, polyq1_t NTTq1_commit_t1,
                           polyq1_t *NTTq1_z_V3, polyq1_t NTTq1_tmp)
  {
    _mul_A1_NTTq1 (NTTq1_w1, ipk, NTTq1_z_V3[0], NTTq1_z_V3[1], NTTq1_z_V3[2],
                   NTTq1_tmp);
    polyq1_mul_NTT (NTTq1_tmp, NTTq1_chall, NTTq1_commit_t1);
    polyq1_neg (NTTq1_tmp);
    polyq1_add (NTTq1_w1, NTTq1_w1, NTTq1_tmp);
  }

  inline void
  _computation_w1_prime_NTTq1 (polyq1_t NTTq1_w1_prime, s_issuer_pk_t ipk,
                               polyq1_t *NTTq1_sample_xi_y_prime_V3,
                               polyq1_t NTTq1_tmp)
  {
    _mul_A1_NTTq1 (NTTq1_w1_prime, ipk, NTTq1_sample_xi_y_prime_V3[0],
                   NTTq1_sample_xi_y_prime_V3[1], NTTq1_sample_xi_y_prime_V3[2],
                   NTTq1_tmp);
  }

  inline void
  _recomputation_w1_prime_NTTq1 (polyq1_t NTTq1_w1_prime, s_issuer_pk_t ipk,
                                 polyq1_t NTTq1_chall,
                                 polyq1_t NTTq1_commit_t1_prime,
                                 polyq1_t *NTTq1_z_prime_V3, polyq1_t NTTq1_tmp)
  {
  _computation_w1_prime_NTTq1 (NTTq1_w1_prime, ipk,
                               NTTq1_z_prime_V3,
                               NTTq1_tmp);
    polyq1_mul_NTT (NTTq1_tmp, NTTq1_chall, NTTq1_commit_t1_prime);
    polyq1_neg (NTTq1_tmp);
    polyq1_add (NTTq1_w1_prime, NTTq1_w1_prime, NTTq1_tmp);
  }

  inline void
  _computation_w2_NTTq2 (polyq2_t NTTq2_w2, s_issuer_pk_t ipk,
                         polyq2_t *NTTq2_sample_xi_y_V3,
                         polyq2_t *NTTq2_sample_xi_y_prime_V3,
                         polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2,
                         polyq2_t NTTq2_tmp3)
  {
    /* w_2 = A_2 (delta y - y_prime) */
    /* Note that we only need the 2 last component because A_2 = (0 1 commit_a3)*/
    /* delta y[1] - y_prime[1] */
    polyq2_scalar_mul (NTTq2_tmp, NTTq2_sample_xi_y_V3[1], g_intq2_delta);
    polyq2_sub (NTTq2_tmp, NTTq2_tmp, NTTq2_sample_xi_y_prime_V3[1]);
    /* delta y[2] - y_prime[2] */
    polyq2_scalar_mul (NTTq2_tmp2, NTTq2_sample_xi_y_V3[2], g_intq2_delta);
    polyq2_sub (NTTq2_tmp2, NTTq2_tmp2, NTTq2_sample_xi_y_prime_V3[2]);
    _mul_A2_NTTq2 (NTTq2_w2, ipk, NTTq2_tmp, NTTq2_tmp2, NTTq2_tmp3);
  }

inline void
_recomputation_w2_NTTq2 (polyq2_t NTTq2_w2, s_issuer_pk_t ipk,
                         polyq2_t NTTq2_chall, polyq2_t NTTq2_commit_t2,
                         polyq2_t NTTq2_commit_t2_prime, polyq2_t *NTTq2_z_V3,
                         polyq2_t *NTTq2_z_prime_V3, polyq2_t NTTq2_tmp,
                         polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3)
{
  _computation_w2_NTTq2 (NTTq2_w2, ipk,
                         NTTq2_z_V3,
                         NTTq2_z_prime_V3,
                         NTTq2_tmp, NTTq2_tmp2,
                         NTTq2_tmp3);

  /* we substract (delta t2 - t2')c */
  polyq2_scalar_mul (NTTq2_tmp, NTTq2_commit_t2, g_intq2_delta);
  polyq2_sub (NTTq2_tmp, NTTq2_tmp, NTTq2_commit_t2_prime);
  polyq2_mul_NTT (NTTq2_tmp, NTTq2_tmp, NTTq2_chall);
  polyq2_sub (NTTq2_w2, NTTq2_w2, NTTq2_tmp);
}

inline void
_computation_w11_NTTq1 (polyq1_t NTTq1_w11, s_issuer_pk_t ipk,
                        polyq1_t *NTTq1_sample_xi_y_1_V3, polyq1_t NTTq1_tmp)
{
  /* w_-1_1 = sigma_-1(A_1) y_-1 */
  _mul_sigma1_A1_NTTq1 (NTTq1_w11, ipk, NTTq1_sample_xi_y_1_V3[0],
                        NTTq1_sample_xi_y_1_V3[1], NTTq1_sample_xi_y_1_V3[2],
                        NTTq1_tmp);
}

inline void
_recomputation_w11_NTTq1 (polyq1_t NTTq1_w11, s_issuer_pk_t ipk,
                          polyq1_t NTTq1_chall,
                          polyq1_t NTTq1_sigma_1_commit_t1,
                          polyq1_t *NTTq1_z1_V3, polyq1_t NTTq1_tmp)
{
  _computation_w11_NTTq1 (NTTq1_w11, ipk,
                          NTTq1_z1_V3, NTTq1_tmp);

  /* we substract sigma_-1(t_1) c */
  polyq1_mul_NTT (NTTq1_tmp, NTTq1_sigma_1_commit_t1, NTTq1_chall);
  polyq1_neg (NTTq1_tmp);
  polyq1_add (NTTq1_w11, NTTq1_w11, NTTq1_tmp);
}

inline void
_computation_w51_NTTq1 (polyq1_t NTTq1_w51, s_issuer_pk_t ipk,
                        polyq1_t *NTTq1_sample_xi_y_5_V3, polyq1_t NTTq1_tmp)
{
  /* w_-1_1 = sigma_-1(A_1) y_-1 */
  _mul_sigma5_A1_NTTq1 (NTTq1_w51, ipk, NTTq1_sample_xi_y_5_V3[0],
                        NTTq1_sample_xi_y_5_V3[1], NTTq1_sample_xi_y_5_V3[2],
                        NTTq1_tmp);
}

inline void
_recomputation_w51_NTTq1 (polyq1_t NTTq1_w51, s_issuer_pk_t ipk,
                          polyq1_t NTTq1_chall,
                          polyq1_t NTTq1_sigma_5_commit_t1,
                          polyq1_t *NTTq1_z5_V3, polyq1_t NTTq1_tmp)
{
  _computation_w51_NTTq1 (NTTq1_w51, ipk,
                          NTTq1_z5_V3, NTTq1_tmp);

  /* we substract sigma_5(t_1) c */
  polyq1_mul_NTT (NTTq1_tmp, NTTq1_sigma_5_commit_t1, NTTq1_chall);
  polyq1_neg (NTTq1_tmp);
  polyq1_add (NTTq1_w51, NTTq1_w51, NTTq1_tmp);
}


inline void
_computation_w12_NTTq2 (polyq2_t NTTq2_w12, s_issuer_pk_t ipk,
                        polyq2_t *NTTq2_sample_xi_y_V3, polyq2_t *NTTq2_sample_xi_y_1_V3, 
                        polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3)
{
  /* A2 y -  sigma1(A2) sigma_1(y) */
  _mul_A2_NTTq2 (NTTq2_tmp, ipk, NTTq2_sample_xi_y_V3[1],
                 NTTq2_sample_xi_y_V3[2], NTTq2_tmp3);
  _mul_sigma1_A2_NTTq2 (NTTq2_tmp2, ipk, NTTq2_sample_xi_y_1_V3[1],
                        NTTq2_sample_xi_y_1_V3[2], NTTq2_tmp3);
  polyq2_neg (NTTq2_tmp2);
  polyq2_add (NTTq2_w12, NTTq2_tmp, NTTq2_tmp2);
}

inline void
_recomputation_w12_NTTq2 (polyq2_t NTTq2_w12, s_issuer_pk_t ipk,
                          polyq2_t NTTq2_chall, 
                          polyq2_t NTTq2_commit_t2, polyq2_t NTTq2_sigma_1_commit_t2,
                          polyq2_t *NTTq2_z_V3, polyq2_t *NTTq2_z1_V3,
                          polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3)
{
  _computation_w12_NTTq2 (NTTq2_w12, ipk,
                          NTTq2_z_V3, NTTq2_z1_V3, 
                          NTTq2_tmp, NTTq2_tmp2, NTTq2_tmp3);

  /* we substract (t_2 - sigma_-1(t2))c */
  polyq2_copy(NTTq2_tmp, NTTq2_commit_t2);
  polyq2_neg (NTTq2_tmp);
  polyq2_add(NTTq2_tmp, NTTq2_tmp, NTTq2_sigma_1_commit_t2);
  polyq2_mul_NTT (NTTq2_tmp, NTTq2_tmp, NTTq2_chall);
  polyq2_add (NTTq2_w12,NTTq2_w12, NTTq2_tmp);
}

inline void
_computation_w52_NTTq2 (polyq2_t NTTq2_w52, s_issuer_pk_t ipk,
                        polyq2_t *NTTq2_sample_xi_y_V3, polyq2_t *NTTq2_sample_xi_y_5_V3, 
                        polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3)
{
  /* A2 y -  sigma5(A2) sigma_5(y) */
  _mul_A2_NTTq2 (NTTq2_tmp, ipk, NTTq2_sample_xi_y_V3[1],
                 NTTq2_sample_xi_y_V3[2], NTTq2_tmp3);
  _mul_sigma5_A2_NTTq2 (NTTq2_tmp2, ipk, NTTq2_sample_xi_y_5_V3[1],
                        NTTq2_sample_xi_y_5_V3[2], NTTq2_tmp3);
  polyq2_neg (NTTq2_tmp2);
  polyq2_add (NTTq2_w52, NTTq2_tmp, NTTq2_tmp2);
}

inline void
_recomputation_w52_NTTq2 (polyq2_t NTTq2_w52, s_issuer_pk_t ipk,
                          polyq2_t NTTq2_chall, 
                          polyq2_t NTTq2_commit_t2, polyq2_t NTTq2_sigma_5_commit_t2,
                          polyq2_t *NTTq2_z_V3, polyq2_t *NTTq2_z1_V3,
                          polyq2_t NTTq2_tmp, polyq2_t NTTq2_tmp2, polyq2_t NTTq2_tmp3)
{
  _computation_w52_NTTq2 (NTTq2_w52, ipk,
                          NTTq2_z_V3, NTTq2_z1_V3, 
                          NTTq2_tmp, NTTq2_tmp2, NTTq2_tmp3);
  /* we substract (t_2 - sigma_-1(t2))c */
  polyq2_copy(NTTq2_tmp, NTTq2_commit_t2);
  polyq2_neg (NTTq2_tmp);
  polyq2_add(NTTq2_tmp, NTTq2_tmp, NTTq2_sigma_5_commit_t2);
  polyq2_mul_NTT (NTTq2_tmp, NTTq2_tmp, NTTq2_chall);
  polyq2_add (NTTq2_w52,NTTq2_w52, NTTq2_tmp);
}

inline void
_computation_wenc_NTTQ (polyQ_t *NTTQ_wenc_V4, s_opener_pk_t opk,
                        polyQ_t *NTTQ_sample_xi_y_enc_V5,
                        polyQ_t *NTTQ_sample_xi_y_V3, polyQ_t NTTQ_tmp)
{
_mul_B_enc(NTTQ_wenc_V4[0], NTTQ_wenc_V4[1], NTTQ_wenc_V4[2], NTTQ_wenc_V4[3], opk, 
           NTTQ_sample_xi_y_enc_V5[0], NTTQ_sample_xi_y_enc_V5[1],
           NTTQ_sample_xi_y_enc_V5[2], NTTQ_sample_xi_y_enc_V5[3],
           NTTQ_sample_xi_y_enc_V5[4],
           NTTQ_sample_xi_y_V3[0], NTTQ_sample_xi_y_V3[1], NTTQ_sample_xi_y_V3[2], NTTQ_tmp);
}

inline void
_recomputation_wenc_NTTQ (polyQ_t *NTTQ_wenc_V4, s_opener_pk_t opk, polyQ_t NTTQ_chall,
                         polyQ_t *NTTQ_z_V3, polyQ_t *NTTQ_zenc_V5,
                         polyQ_t NTTQ_enc1,
                         polyQ_t *NTTQ_enc2_V3, polyQ_t NTTQ_tmp)
{
_computation_wenc_NTTQ (NTTQ_wenc_V4, opk,
                        NTTQ_zenc_V5,
                        NTTQ_z_V3, NTTQ_tmp);

  /* we substract (enc1, enc2) */
  polyQ_mul_NTT(NTTQ_tmp, NTTQ_enc1, NTTQ_chall);
  polyQ_neg(NTTQ_tmp);
  polyQ_add (NTTQ_wenc_V4[0], NTTQ_wenc_V4[0], NTTQ_tmp);

  polyQ_mul_NTT(NTTQ_tmp, NTTQ_enc2_V3[0], NTTQ_chall);
  polyQ_neg(NTTQ_tmp);
  polyQ_add (NTTQ_wenc_V4[1], NTTQ_wenc_V4[1], NTTQ_tmp);

  polyQ_mul_NTT(NTTQ_tmp, NTTQ_enc2_V3[1], NTTQ_chall);
  polyQ_neg(NTTQ_tmp);
  polyQ_add (NTTQ_wenc_V4[2], NTTQ_wenc_V4[2], NTTQ_tmp);

  polyQ_mul_NTT(NTTQ_tmp, NTTQ_enc2_V3[2], NTTQ_chall);
  polyQ_neg(NTTQ_tmp);
  polyQ_add (NTTQ_wenc_V4[3], NTTQ_wenc_V4[3], NTTQ_tmp);
}



inline void
_computation_ws_NTTq2 (polyq2_t NTTq2_ws, s_issuer_pk_t ipk,
                       polyq2_t NTTq2_commit_t2,
                       polyq2_t NTTq2_commit_t2_prime,
                       polyq2_t *NTTq2_sample_xi1_y_s1_V4,
                       polyq2_t *NTTq2_sample_xi2_y_s2_V2, polyq2_t NTTq2_tmp)
{

_mul_BIGMAT_PRES_NTTq2(NTTq2_ws, ipk, NTTq2_commit_t2, NTTq2_commit_t2_prime,
                                   NTTq2_sample_xi1_y_s1_V4[0],
                                   NTTq2_sample_xi1_y_s1_V4[1],
                                   NTTq2_sample_xi1_y_s1_V4[2],
                                   NTTq2_sample_xi1_y_s1_V4[3],
                                   NTTq2_sample_xi2_y_s2_V2[0],
                                   NTTq2_sample_xi2_y_s2_V2[1],
                                   NTTq2_tmp);
}

inline void
_recomputation_ws_NTTq2 (polyq2_t NTTq2_ws, s_issuer_pk_t ipk,
                         polyq2_t NTTq2_chall, polyq2_t NTTq2_commit_t2,
                         polyq2_t NTTq2_commit_t2_prime,
                         polyq2_t *NTTq2_zs1_V4, polyq2_t *NTTq2_zs2_V2,
                         polyq2_t NTTq2_tmp, attribute_t attr)
{
  _computation_ws_NTTq2 (NTTq2_ws, ipk,
                         NTTq2_commit_t2,
                         NTTq2_commit_t2_prime,
                         NTTq2_zs1_V4,
                         NTTq2_zs2_V2, NTTq2_tmp);

  /* we substract fake_hash(attr) * c */
  polyq2_mul_NTT (NTTq2_tmp, NTTq2_chall, NTTq2_fake_hash_V256[attr]);
  polyq2_neg (NTTq2_tmp);
  polyq2_add (NTTq2_ws, NTTq2_ws, NTTq2_tmp);
}

inline void
_computation_z_rc_NTTq1 (polyq1_t *NTTq1_z_V3, polyq1_t *NTTq1_rc_V3,
                         polyq1_t NTTq1_r1, polyq1_t NTTq1_r2, polyq1_t NTTq1_r3,
                      polyq1_t NTTq1_chall, polyq1_t *NTTq1_sample_xi_y_V3)
{
  /* z = cr + y */
  polyq1_mul_NTT (NTTq1_rc_V3[0], NTTq1_r1, NTTq1_chall);
  polyq1_add (NTTq1_z_V3[0], NTTq1_rc_V3[0], NTTq1_sample_xi_y_V3[0]);
  polyq1_mul_NTT (NTTq1_rc_V3[1], NTTq1_r2, NTTq1_chall);
  polyq1_add (NTTq1_z_V3[1], NTTq1_rc_V3[1], NTTq1_sample_xi_y_V3[1]);
  polyq1_mul_NTT (NTTq1_rc_V3[2], NTTq1_r3, NTTq1_chall);
  polyq1_add (NTTq1_z_V3[2], NTTq1_rc_V3[2], NTTq1_sample_xi_y_V3[2]);
}

inline void
_computation_z_rc_prime_NTTq1 (polyq1_t *NTTq1_z_prime_V3, polyq1_t *NTTq1_rc_prime_V3,
                            polyq1_t NTTq1_r1_prime, polyq1_t NTTq1_r2_prime,
                            polyq1_t NTTq1_r3_prime, polyq1_t NTTq1_chall,
                            polyq1_t *NTTq1_sample_xi_y_prime_V3)
{
  /* z' = cr' + y' */
  polyq1_mul_NTT (NTTq1_rc_prime_V3[0], NTTq1_r1_prime, NTTq1_chall);
  polyq1_add (NTTq1_z_prime_V3[0], NTTq1_rc_prime_V3[0],
              NTTq1_sample_xi_y_prime_V3[0]);

  polyq1_mul_NTT (NTTq1_rc_prime_V3[1], NTTq1_r2_prime, NTTq1_chall);
  polyq1_add (NTTq1_z_prime_V3[1], NTTq1_rc_prime_V3[1],
              NTTq1_sample_xi_y_prime_V3[1]);

  polyq1_mul_NTT (NTTq1_rc_prime_V3[2], NTTq1_r3_prime, NTTq1_chall);
  polyq1_add (NTTq1_z_prime_V3[2], NTTq1_rc_prime_V3[2],
              NTTq1_sample_xi_y_prime_V3[2]);
}

inline void
_computation_z_rc_1_NTTq1 (polyq1_t *NTTq1_z1_V3,  polyq1_t *NTTq1_rc1_V3,
                           polyq1_t NTTq1_sigma1_r1, polyq1_t NTTq1_sigma1_r2, polyq1_t NTTq1_sigma1_r3,
                       polyq1_t NTTq1_chall, polyq1_t *NTTq1_sample_xi_y_1_V3)
{
  polyq1_mul_NTT (NTTq1_rc1_V3[0], NTTq1_sigma1_r1, NTTq1_chall);
  polyq1_add (NTTq1_z1_V3[0], NTTq1_rc1_V3[0], NTTq1_sample_xi_y_1_V3[0]);

  polyq1_mul_NTT (NTTq1_rc1_V3[1], NTTq1_sigma1_r2, NTTq1_chall);
  polyq1_add (NTTq1_z1_V3[1], NTTq1_rc1_V3[1], NTTq1_sample_xi_y_1_V3[1]);

  polyq1_mul_NTT (NTTq1_rc1_V3[2], NTTq1_sigma1_r3, NTTq1_chall);
  polyq1_add (NTTq1_z1_V3[2], NTTq1_rc1_V3[2], NTTq1_sample_xi_y_1_V3[2]);
}

inline void
_computation_z_rc_5_NTTq1 (polyq1_t *NTTq1_z5_V3,  polyq1_t *NTTq1_rc5_V3,
                           polyq1_t NTTq1_sigma5_r1, polyq1_t NTTq1_sigma5_r2, polyq1_t NTTq1_sigma5_r3,
                       polyq1_t NTTq1_chall, polyq1_t *NTTq1_sample_xi_y_5_V3)
{
  polyq1_mul_NTT (NTTq1_rc5_V3[0], NTTq1_sigma5_r1, NTTq1_chall);
  polyq1_add (NTTq1_z5_V3[0], NTTq1_rc5_V3[0], NTTq1_sample_xi_y_5_V3[0]);

  polyq1_mul_NTT (NTTq1_rc5_V3[1], NTTq1_sigma5_r2, NTTq1_chall);
  polyq1_add (NTTq1_z5_V3[1], NTTq1_rc5_V3[1], NTTq1_sample_xi_y_5_V3[1]);

  polyq1_mul_NTT (NTTq1_rc5_V3[2], NTTq1_sigma5_r3, NTTq1_chall);
  polyq1_add (NTTq1_z5_V3[2], NTTq1_rc5_V3[2], NTTq1_sample_xi_y_5_V3[2]);
}


inline void
_computation_z_rc_enc_NTTQ (polyQ_t *NTTQ_zenc_V5, polyQ_t *NTTQ_rcenc_V5,
                            polyQ_t NTTQ_chall, polyQ_t NTTQ_r_enc, polyQ_t NTTQ_e1_enc,
                         polyQ_t NTTQ_e2_1_enc, polyQ_t NTTQ_e2_2_enc,
                         polyQ_t NTTQ_e2_3_enc,
                         polyQ_t *NTTQ_sample_xi_y_enc_V5)
{
  /* z_enc = c [r_enc e1_enc e2_1_enc e2_2_enc e2_3_enc] + y_enc */
  polyQ_mul_NTT (NTTQ_rcenc_V5[0], NTTQ_r_enc, NTTQ_chall);
  polyQ_add (NTTQ_zenc_V5[0], NTTQ_rcenc_V5[0], NTTQ_sample_xi_y_enc_V5[0]);
  polyQ_mul_NTT (NTTQ_rcenc_V5[1], NTTQ_e1_enc, NTTQ_chall);
  polyQ_add (NTTQ_zenc_V5[1], NTTQ_rcenc_V5[1], NTTQ_sample_xi_y_enc_V5[1]);
  polyQ_mul_NTT (NTTQ_rcenc_V5[2], NTTQ_e2_1_enc, NTTQ_chall);
  polyQ_add (NTTQ_zenc_V5[2], NTTQ_rcenc_V5[2], NTTQ_sample_xi_y_enc_V5[2]);
  polyQ_mul_NTT (NTTQ_rcenc_V5[3], NTTQ_e2_2_enc, NTTQ_chall);
  polyQ_add (NTTQ_zenc_V5[3], NTTQ_rcenc_V5[3], NTTQ_sample_xi_y_enc_V5[3]);
  polyQ_mul_NTT (NTTQ_rcenc_V5[4], NTTQ_e2_3_enc, NTTQ_chall);
  polyQ_add (NTTQ_zenc_V5[4], NTTQ_rcenc_V5[4], NTTQ_sample_xi_y_enc_V5[4]);
}

inline void
_computation_z_rc_s1_NTTq2 (polyq2_t *NTTq2_zs1_V4, polyq2_t *NTTq2_rcs1_V4,
                            s_credential_t cred, polyq2_t NTTq2_chall,
                        polyq2_t *NTTq2_sample_xi1_y_s1_V4)
{
  /* z_s1[0] = y_s1[0] + c [cred->NTTq2_s1_V2[0]] */
  polyq2_mul_NTT (NTTq2_rcs1_V4[0], cred->NTTq2_s1_V2[0], NTTq2_chall);
  polyq2_add (NTTq2_zs1_V4[0], NTTq2_rcs1_V4[0], NTTq2_sample_xi1_y_s1_V4[0]);

  /* z_s1[1] = y_s1[1] + c [cred->NTTq2_s1_V2[1]] */
  polyq2_mul_NTT (NTTq2_rcs1_V4[1], cred->NTTq2_s1_V2[1], NTTq2_chall);
  polyq2_add (NTTq2_zs1_V4[1], NTTq2_rcs1_V4[1], NTTq2_sample_xi1_y_s1_V4[1]);

  /* z_s1[2] = y_s1[2] + c [cred->NTTq2_s2_V2[0]] */
  polyq2_mul_NTT (NTTq2_rcs1_V4[2], cred->NTTq2_s2_V2[0], NTTq2_chall);
  polyq2_add (NTTq2_zs1_V4[2], NTTq2_rcs1_V4[2], NTTq2_sample_xi1_y_s1_V4[2]);

  /* z_s1[3] = y_s1[3] + c [cred->NTTq2_s2_V2[1]] */
  polyq2_mul_NTT (NTTq2_rcs1_V4[3], cred->NTTq2_s2_V2[1], NTTq2_chall);
  polyq2_add (NTTq2_zs1_V4[3], NTTq2_rcs1_V4[3], NTTq2_sample_xi1_y_s1_V4[3]);
}

/* used for computation of z_s2 and rc_s2 */
/*  (res1 res2)^t = (s3_1 s3_2)^t - [s2_1 * (r2 r3)^t + s2_2 * (r2_prime r3_prime)^t] */ 
/* entry have to be in NTTq2 form */
inline void
_computation_intermediate_rc_zs2_NTT2(polyq2_t res1, polyq2_t res2, 
                          polyq2_t r2, polyq2_t r3, 
                          polyq2_t r2_prime, polyq2_t r3_prime,
                          polyq2_t s2_1, polyq2_t s2_2, 
                          polyq2_t s3_1, polyq2_t s3_2, 
                          polyq2_t NTTq2_tmp)
{
  polyq2_mul_NTT (NTTq2_tmp, s2_1, r2);
  polyq2_mul_NTT (res1, s2_2, r2_prime);
  polyq2_add (res1, res1, NTTq2_tmp);
  polyq2_neg (res1);
  polyq2_add (res1, res1, s3_1);

  polyq2_mul_NTT (NTTq2_tmp, s2_1, r3);
  polyq2_mul_NTT (res2, s2_2, r3_prime);
  polyq2_add (res2, res2, NTTq2_tmp);
  polyq2_neg (res2);
  polyq2_add (res2, res2, s3_2);
}

inline void
_computation_z_rc_s2_NTTq2 (polyq2_t *NTTq2_zs2_V2, polyq2_t *NTTq2_rcs2_V2, 
                            s_credential_t cred, polyq2_t NTTq2_chall,
                        polyq2_t *NTTq2_sample_xi2_y_s2_V2, polyq2_t NTTq2_r2,
                        polyq2_t NTTq2_r3, polyq2_t NTTq2_r2_prime,
                        polyq2_t NTTq2_r3_prime, polyq2_t NTTq2_tmp)
{

  /* z_s2[0] = *  y_s2[0] + c * [ s3_V2[0] - (s2_V2[0]*r2 + s2_V2[1] * r2_prime)] */
_computation_intermediate_rc_zs2_NTT2(NTTq2_rcs2_V2[0], NTTq2_rcs2_V2[1], 
                          NTTq2_r2, NTTq2_r3, 
                          NTTq2_r2_prime, NTTq2_r3_prime,
                          cred->NTTq2_s2_V2[0], cred->NTTq2_s2_V2[1],
                          cred->NTTq2_s3_V2[0], cred->NTTq2_s3_V2[1],
                          NTTq2_tmp);

  polyq2_mul_NTT (NTTq2_rcs2_V2[0], NTTq2_rcs2_V2[0], NTTq2_chall);
  polyq2_add (NTTq2_zs2_V2[0], NTTq2_rcs2_V2[0], NTTq2_sample_xi2_y_s2_V2[0]);

  polyq2_mul_NTT (NTTq2_rcs2_V2[1], NTTq2_rcs2_V2[1], NTTq2_chall);
  polyq2_add (NTTq2_zs2_V2[1], NTTq2_rcs2_V2[1], NTTq2_sample_xi2_y_s2_V2[1]);
}

inline void
_mul_B_enc(polyQ_t res0, polyQ_t res1, polyQ_t res2, polyQ_t res3,
           s_opener_pk_t opk, 
           polyQ_t x0, polyQ_t x1, polyQ_t x2, polyQ_t x3, polyQ_t x4, 
           polyQ_t NTTQ_r1, polyQ_t NTTQ_r2,polyQ_t NTTQ_r3, 
           polyQ_t NTTQ_tmp)
{
  /* computation of enc1 = p(ax0 + x1) */
  polyQ_mul_NTT (NTTQ_tmp, opk->NTTQ_a, x0);
  polyQ_add (res0, NTTQ_tmp, x1);
  polyQ_scalar_mul (res0, res0, g_intQ_p);

  /* computation of enc2 = p(x0 * b + [x2 x3 x4]^t) + [NTTQ_r1 NTTQ_r2 NTTQ_r3]^t */  
  /*** first component */
  polyQ_mul_NTT (NTTQ_tmp, opk->NTTQ_b_V3[0], x0);
  polyQ_add (res1, NTTQ_tmp, x2);
  polyQ_scalar_mul (res1, res1, g_intQ_p);
  polyQ_add (res1, res1, NTTQ_r1);
  /*** second component */
  polyQ_mul_NTT (NTTQ_tmp, opk->NTTQ_b_V3[1], x0);
  polyQ_add (res2, NTTQ_tmp, x3);
  polyQ_scalar_mul (res2, res2, g_intQ_p);
  polyQ_add (res2, res2, NTTQ_r2);
  /*** third component */
  polyQ_mul_NTT (NTTQ_tmp, opk->NTTQ_b_V3[2], x0);
  polyQ_add (res3, NTTQ_tmp, x4);
  polyQ_scalar_mul (res3, res3, g_intQ_p);
  polyQ_add (res3, res3, NTTQ_r3);
}

int
presentation_generation (s_presentation_t pres, s_credential_t cred,
                         intq2_t intq2_id, s_issuer_pk_t ipk, s_opener_pk_t opk,
                         attribute_t attr, message_t message)
{
  /* context used for the hash function that compute the challenge */
  shake256_context shake256_context_hash_challenge;

  intq2_t intq2_delta_id;

  /* vars for the rc */
  polyq1_t Rq1_rc_V3[3];
  polyq1_init_V(Rq1_rc_V3,3);
  polyq1_t Rq1_rc_prime_V3[3];
  polyq1_init_V(Rq1_rc_prime_V3,3);
  polyq1_t Rq1_rc1_V3[3];
  polyq1_init_V(Rq1_rc1_V3,3);
  polyq1_t Rq1_rc5_V3[3];
  polyq1_init_V(Rq1_rc5_V3,3);
  polyQ_t RQ_rcenc_V5[5];
  polyQ_init_V(RQ_rcenc_V5,5);
  polyq2_t Rq2_rcs1_V4[4];
  polyq2_init_V(Rq2_rcs1_V4,4);
  polyq2_t Rq2_rcs2_V2[2];
  polyq2_init_V(Rq2_rcs2_V2,2);

  /* random vars for commitments t and t_prime */
  challenge_t challenge_r1;
  challenge_t challenge_r2;
  challenge_t challenge_r3;
  challenge_init (challenge_r1);
  challenge_init (challenge_r2);
  challenge_init (challenge_r3);

  polyq1_t NTTq1_sigma1_r1;
  polyq1_t NTTq1_sigma1_r2;
  polyq1_t NTTq1_sigma1_r3;
  polyq1_init (NTTq1_sigma1_r1);
  polyq1_init (NTTq1_sigma1_r2);
  polyq1_init (NTTq1_sigma1_r3);
  challenge_t challenge_sigma1_r1;
  challenge_t challenge_sigma1_r2;
  challenge_t challenge_sigma1_r3;
  challenge_init (challenge_sigma1_r1);
  challenge_init (challenge_sigma1_r2);
  challenge_init (challenge_sigma1_r3);
  polyq1_t NTTq1_sigma5_r1;
  polyq1_t NTTq1_sigma5_r2;
  polyq1_t NTTq1_sigma5_r3;
  polyq1_init (NTTq1_sigma5_r1);
  polyq1_init (NTTq1_sigma5_r2);
  polyq1_init (NTTq1_sigma5_r3);
  challenge_t challenge_sigma5_r1;
  challenge_t challenge_sigma5_r2;
  challenge_t challenge_sigma5_r3;
  challenge_init (challenge_sigma5_r1);
  challenge_init (challenge_sigma5_r2);
  challenge_init (challenge_sigma5_r3);

  polyq1_t NTTq1_r1;
  polyq1_t NTTq1_r2;
  polyq1_t NTTq1_r3;
  polyq2_t NTTq2_r2;
  polyq2_t NTTq2_r3;
  polyq1_init (NTTq1_r1);
  polyq1_init (NTTq1_r2);
  polyq1_init (NTTq1_r3);
  polyq2_init (NTTq2_r2);
  polyq2_init (NTTq2_r3);
  polyQ_t NTTQ_r1;
  polyQ_t NTTQ_r2;
  polyQ_t NTTQ_r3;
  polyQ_init (NTTQ_r1);
  polyQ_init (NTTQ_r2);
  polyQ_init (NTTQ_r3);

  challenge_t challenge_r1_prime;
  challenge_t challenge_r2_prime;
  challenge_t challenge_r3_prime;
  challenge_init (challenge_r1_prime);
  challenge_init (challenge_r2_prime);
  challenge_init (challenge_r3_prime);

  polyq1_t NTTq1_r1_prime;
  polyq1_t NTTq1_r2_prime;
  polyq1_t NTTq1_r3_prime;
  polyq2_t NTTq2_r2_prime;
  polyq2_t NTTq2_r3_prime;
  polyq1_init (NTTq1_r1_prime);
  polyq1_init (NTTq1_r2_prime);
  polyq1_init (NTTq1_r3_prime);
  polyq2_init (NTTq2_r2_prime);
  polyq2_init (NTTq2_r3_prime);

  /* tmp vars */
  polyq1_t NTTq1_tmp;
  polyq1_init (NTTq1_tmp);
  polyq2_t NTTq2_tmp;
  polyq2_t NTTq2_tmp2;
  polyq2_t NTTq2_tmp3;
  polyq2_init (NTTq2_tmp);
  polyq2_init (NTTq2_tmp2);
  polyq2_init (NTTq2_tmp3);
  polyQ_t NTTQ_tmp;
  polyQ_init (NTTQ_tmp);

  /* vars for verifiable encryption */
  polyQ_t NTTQ_r_enc;
  polyQ_t NTTQ_e1_enc;
  polyQ_t NTTQ_e2_1_enc;
  polyQ_t NTTQ_e2_2_enc;
  polyQ_t NTTQ_e2_3_enc;
  polyQ_init (NTTQ_r_enc);
  polyQ_init (NTTQ_e1_enc);
  polyQ_init (NTTQ_e2_1_enc);
  polyQ_init (NTTQ_e2_2_enc);
  polyQ_init (NTTQ_e2_3_enc);

  /* vars for gaussian samples with std dev xi*/
  polyq1_t NTTq1_sample_xi_y_V3[3];
  polyq1_init_V (NTTq1_sample_xi_y_V3,3);
  polyq2_t NTTq2_sample_xi_y_V3[3];
  polyq2_init_V (NTTq2_sample_xi_y_V3,3);
  polyQ_t NTTQ_sample_xi_y_V3[3];
  polyQ_init_V (NTTQ_sample_xi_y_V3,3);
  polyq1_t NTTq1_sample_xi_y_prime_V3[3];
  polyq1_init_V (NTTq1_sample_xi_y_prime_V3,3);
  polyq2_t NTTq2_sample_xi_y_prime_V3[3];
  polyq2_init_V (NTTq2_sample_xi_y_prime_V3,3);
  polyq1_t NTTq1_sample_xi_y_1_V3[3];
  polyq1_init_V (NTTq1_sample_xi_y_1_V3,3);
  polyq2_t NTTq2_sample_xi_y_1_V3[3];
  polyq2_init_V (NTTq2_sample_xi_y_1_V3,3);
  polyq1_t NTTq1_sample_xi_y_5_V3[3];
  polyq1_init_V (NTTq1_sample_xi_y_5_V3,3);
  polyq2_t NTTq2_sample_xi_y_5_V3[3];
  polyq2_init_V (NTTq2_sample_xi_y_5_V3,3);
  polyQ_t NTTQ_sample_xi_y_enc_V5[5];
  polyQ_init_V (NTTQ_sample_xi_y_enc_V5,5);

  /* vars for gaussian samples with std dev xi1*/
  polyq2_t NTTq2_sample_xi1_y_s1_V4[4];
  polyq2_init_V (NTTq2_sample_xi1_y_s1_V4,4);

  /* vars for gaussian samples with std dev xi2*/
  polyq2_t NTTq2_sample_xi2_y_s2_V2[2];
  polyq2_init_V (NTTq2_sample_xi2_y_s2_V2,2);


  /* vars for the w of the ZK proofs */
  polyq1_t NTTq1_w1_gen;
  polyq1_init (NTTq1_w1_gen);
  polyq2_t NTTq2_w2_gen;
  polyq2_init (NTTq2_w2_gen);
  polyq1_t NTTq1_w1_prime_gen;
  polyq1_init (NTTq1_w1_prime_gen);
  polyq1_t NTTq1_w11_gen;
  polyq1_init (NTTq1_w11_gen);
  polyq2_t NTTq2_w12_gen;
  polyq2_init (NTTq2_w12_gen);
  polyq1_t NTTq1_w51_gen;
  polyq1_init (NTTq1_w51_gen);
  polyq2_t NTTq2_w52_gen;
  polyq2_init (NTTq2_w52_gen);
  polyq2_t NTTq2_ws_gen;
  polyq2_init (NTTq2_ws_gen);
  polyQ_t NTTQ_wenc_gen_V4[4];
  polyQ_init_V (NTTQ_wenc_gen_V4,4);

  /* vars for the challenge */
  polyq1_t NTTq1_chall;
  polyq1_init (NTTq1_chall);
  polyq2_t NTTq2_chall;
  polyq2_init (NTTq2_chall);
  polyQ_t NTTQ_chall;
  polyQ_init (NTTQ_chall);

  /*********************************************************/
  /* computation of the commitment [commit_t1 commit_t2]^t */
  /*********************************************************/
  /* We sample the randomness */
  get_random_challenge_chall_NTTq1_NTTQ (challenge_r1, NTTq1_r1, NTTQ_r1);
  get_random_challenge_chall_NTTq1_NTTq2_NTTQ (challenge_r2, NTTq1_r2, NTTq2_r2, NTTQ_r2);
  get_random_challenge_chall_NTTq1_NTTq2_NTTQ (challenge_r3, NTTq1_r3, NTTq2_r3, NTTQ_r3);

  /* we compute commit_t1 = r1 + commit_a1 * r2 + commit_a2 * r3 (mod q1) */
  _mul_A1_NTTq1 (pres->NTTq1_commit_t1, ipk, NTTq1_r1, NTTq1_r2, NTTq1_r3,
                 NTTq1_tmp);

  /* we compute commit_t2 = r2 + commit_a3 * r3 (mod q2) + id*/
  _mul_A2_NTTq2 (pres->NTTq2_commit_t2, ipk, NTTq2_r2, NTTq2_r3, NTTq2_tmp);
  polyq2_add_intq2_NTT (pres->NTTq2_commit_t2, pres->NTTq2_commit_t2, intq2_id);

  /*********************************************************/
  /* computation of the commitment [commit_t1_prime commit_t2_prime]^t */
  /*********************************************************/
  /* We sample the randomness */
  get_random_challenge_chall_NTTq1 (challenge_r1_prime, NTTq1_r1_prime);
  get_random_challenge_chall_NTTq1_NTTq2 (challenge_r2_prime, NTTq1_r2_prime, NTTq2_r2_prime);
  get_random_challenge_chall_NTTq1_NTTq2 (challenge_r3_prime, NTTq1_r3_prime, NTTq2_r3_prime);

  /* we compute commit_t1_prime = r1_prime + commit_a1 * r2_prime + commit_a2 * r3_prime
   * (mod q1) */
  _mul_A1_NTTq1 (pres->NTTq1_commit_t1_prime, ipk, NTTq1_r1_prime,
                 NTTq1_r2_prime, NTTq1_r3_prime, NTTq1_tmp);

  /* we compute commit_t2_prime = r2_prime + commit_a3 * r3_prime (mod q2) + delta * id*/
  _mul_A2_NTTq2 (pres->NTTq2_commit_t2_prime, ipk, NTTq2_r2_prime,
                 NTTq2_r3_prime, NTTq2_tmp);
  intq2_mul(intq2_delta_id, g_intq2_delta, intq2_id);
  polyq2_add_intq2_NTT (pres->NTTq2_commit_t2_prime,
                        pres->NTTq2_commit_t2_prime, intq2_delta_id);

  /*********************************************************/
  /* computation of the verifable encryption */
  /*********************************************************/
  /* we sample the randomness */
  get_random_challenge_NTTQ (NTTQ_r_enc);
  get_random_challenge_NTTQ (NTTQ_e1_enc);
  get_random_challenge_NTTQ (NTTQ_e2_1_enc);
  get_random_challenge_NTTQ (NTTQ_e2_2_enc);
  get_random_challenge_NTTQ (NTTQ_e2_3_enc);

_mul_B_enc(pres->NTTQ_enc1, pres->NTTQ_enc2_V3[0], pres->NTTQ_enc2_V3[1], pres->NTTQ_enc2_V3[2], opk, 
           NTTQ_r_enc, NTTQ_e1_enc, NTTQ_e2_1_enc, NTTQ_e2_2_enc, NTTQ_e2_3_enc, 
           NTTQ_r1, NTTQ_r2,NTTQ_r3, NTTQ_tmp);

  int accept = 0;
  /* while rejection sampling is not true */
  while(accept == 0)
  {

    /* sampling of the gaussians with std derivation xi */
    get_gaussian_xi_NTTq1_NTTq2_NTTQ_V (NTTq1_sample_xi_y_V3, NTTq2_sample_xi_y_V3, NTTQ_sample_xi_y_V3,3);
    get_gaussian_xi_NTTq1_NTTq2_V (NTTq1_sample_xi_y_prime_V3, NTTq2_sample_xi_y_prime_V3,3);
    get_gaussian_xi_NTTq1_NTTq2_V (NTTq1_sample_xi_y_1_V3, NTTq2_sample_xi_y_1_V3,3);
    get_gaussian_xi_NTTq1_NTTq2_V (NTTq1_sample_xi_y_5_V3, NTTq2_sample_xi_y_5_V3,3);
    get_gaussian_xi_NTTQ_V (NTTQ_sample_xi_y_enc_V5,5);

    /* sampling of the gaussians with std derivation xi1 */
    get_gaussian_xi1_NTTq2_V (NTTq2_sample_xi1_y_s1_V4,4);

    /* sampling of the gaussians with std dev xi2*/
    get_gaussian_xi2_NTTq2_V (NTTq2_sample_xi2_y_s2_V2,2);

    /*********************************************************/
    /* computation of the ZK proofs */
    /*********************************************************/

    /* computation of the w */
    _computation_w1_NTTq1 (NTTq1_w1_gen, ipk, NTTq1_sample_xi_y_V3, NTTq1_tmp);
    _computation_w1_prime_NTTq1 (NTTq1_w1_prime_gen, ipk, NTTq1_sample_xi_y_prime_V3,
                                 NTTq1_tmp);
    _computation_w2_NTTq2 (NTTq2_w2_gen, ipk, NTTq2_sample_xi_y_V3,
                           NTTq2_sample_xi_y_prime_V3, NTTq2_tmp, NTTq2_tmp2,
                           NTTq2_tmp3);
    _computation_w11_NTTq1 (NTTq1_w11_gen, ipk, NTTq1_sample_xi_y_1_V3, NTTq1_tmp);
    _computation_w51_NTTq1 (NTTq1_w51_gen, ipk, NTTq1_sample_xi_y_5_V3, NTTq1_tmp);
    _computation_w12_NTTq2 (NTTq2_w12_gen, ipk, NTTq2_sample_xi_y_V3,
                            NTTq2_sample_xi_y_1_V3, NTTq2_tmp, NTTq2_tmp2,
                            NTTq2_tmp3);
    _computation_w52_NTTq2 (NTTq2_w52_gen, ipk, NTTq2_sample_xi_y_V3,
                            NTTq2_sample_xi_y_5_V3, NTTq2_tmp, NTTq2_tmp2,
                            NTTq2_tmp3);
    _computation_wenc_NTTQ (NTTQ_wenc_gen_V4, opk, NTTQ_sample_xi_y_enc_V5,
                            NTTQ_sample_xi_y_V3, NTTQ_tmp);
    _computation_ws_NTTq2 (NTTq2_ws_gen, ipk, pres->NTTq2_commit_t2,
                           pres->NTTq2_commit_t2_prime, NTTq2_sample_xi1_y_s1_V4,
                           NTTq2_sample_xi2_y_s2_V2, NTTq2_tmp);

    /* COMPUTATION OF THE CHALLENGE */
    /* We hash, in the following order :      
     * the commitment t :
     ************* NTTq1_commit_t1
     ************* NTTq2_commit_t2
     * the commitment t'
     ************* NTTq1_commit_t1_prime
     ************* NTTq2_commit_t2_prime
     * the verifiable encryption
     ************* NTTQ_enc1
     ************* NTTQ_enc2_V3[0]
     ************* NTTQ_enc2_V3[1]
     ************* NTTQ_enc2_v2[2])
     * NTTq1_w1
     * NTTq1_w1_prime
     * NTTq1_w11
     * NTTq1_w51
     * NTTq2_w2
     * NTTq2_w12
     * NTTq2_w52
     * NTTq2_ws
     * NTTQ_wenc_V4[0]
     * NTTQ_wenc_V4[1]
     * NTTQ_wenc_V4[2]
     * NTTQ_wenc_V4[3]
     * the message M
     * the attribute attr
     */
    shake256_init (&shake256_context_hash_challenge);

    shake256_inject_polyq1 (&shake256_context_hash_challenge,
                            pres->NTTq1_commit_t1);
    shake256_inject_polyq2 (&shake256_context_hash_challenge,
                            pres->NTTq2_commit_t2);
    shake256_inject_polyq1 (&shake256_context_hash_challenge,
                            pres->NTTq1_commit_t1_prime);
    shake256_inject_polyq2 (&shake256_context_hash_challenge,
                            pres->NTTq2_commit_t2_prime);
    shake256_inject_polyQ (&shake256_context_hash_challenge, pres->NTTQ_enc1);
    shake256_inject_polyQ (&shake256_context_hash_challenge,
                           pres->NTTQ_enc2_V3[0]);
    shake256_inject_polyQ (&shake256_context_hash_challenge,
                           pres->NTTQ_enc2_V3[1]);
    shake256_inject_polyQ (&shake256_context_hash_challenge,
                           pres->NTTQ_enc2_V3[2]);
    shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w1_gen);
    shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w1_prime_gen);
    shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w11_gen);
    shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w51_gen);
    shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_w2_gen);
    shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_w12_gen);
    shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_w52_gen);
    shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_ws_gen);
     shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_gen_V4[0]);
     shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_gen_V4[1]);
     shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_gen_V4[2]);
     shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_gen_V4[3]);
    shake256_inject (&shake256_context_hash_challenge, (void *)message,
                     SIZE_MESS_S8);
    shake256_inject (&shake256_context_hash_challenge, (void *)&attr, 1);

    shake256_flip (&shake256_context_hash_challenge);

    /* computation of the hash value and conversion in NTTq1/NTTq2/NTTQ forms */
    FDH_challenges (pres->challenge_chall, &shake256_context_hash_challenge);
    conv_challenge_to_polyq1 (NTTq1_chall, pres->challenge_chall);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_chall);
    conv_challenge_to_polyq2 (NTTq2_chall, pres->challenge_chall);
    conv_R_polyq2_to_NTT_polyq2 (NTTq2_chall);
    conv_challenge_to_polyQ (NTTQ_chall, pres->challenge_chall);
    conv_R_polyQ_to_NTT_polyQ (NTTQ_chall);


    /* computation of z and rc */
    _computation_z_rc_NTTq1 (pres->Rq1_z_V3, Rq1_rc_V3,
                              NTTq1_r1, NTTq1_r2, NTTq1_r3,
                          NTTq1_chall, NTTq1_sample_xi_y_V3);
    _computation_z_rc_prime_NTTq1 (pres->Rq1_z_prime_V3, Rq1_rc_prime_V3,
                                   NTTq1_r1_prime,
                                NTTq1_r2_prime, NTTq1_r3_prime, NTTq1_chall,
                                NTTq1_sample_xi_y_prime_V3);
    /* we compute sigma_-1 of r, usefull for z_1*/
    challenge_sigma_1 (challenge_sigma1_r1, challenge_r1);
    challenge_sigma_1 (challenge_sigma1_r2, challenge_r2);
    challenge_sigma_1 (challenge_sigma1_r3, challenge_r3);
    conv_challenge_to_polyq1 (NTTq1_sigma1_r1, challenge_sigma1_r1);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma1_r1);
    conv_challenge_to_polyq1 (NTTq1_sigma1_r2, challenge_sigma1_r2);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma1_r2);
    conv_challenge_to_polyq1 (NTTq1_sigma1_r3, challenge_sigma1_r3);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma1_r3);
    /* computation of z_1 and rc_1 */
    _computation_z_rc_1_NTTq1 (pres->Rq1_z1_V3, Rq1_rc1_V3,
                               NTTq1_sigma1_r1, NTTq1_sigma1_r2,
                           NTTq1_sigma1_r3, NTTq1_chall, NTTq1_sample_xi_y_1_V3);

    /* we compute sigma_5 of r, usefull for z_5*/
    challenge_sigma_5 (challenge_sigma5_r1, challenge_r1);
    challenge_sigma_5 (challenge_sigma5_r2, challenge_r2);
    challenge_sigma_5 (challenge_sigma5_r3, challenge_r3);
    conv_challenge_to_polyq1 (NTTq1_sigma5_r1, challenge_sigma5_r1);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma5_r1);
    conv_challenge_to_polyq1 (NTTq1_sigma5_r2, challenge_sigma5_r2);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma5_r2);
    conv_challenge_to_polyq1 (NTTq1_sigma5_r3, challenge_sigma5_r3);
    conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma5_r3);
    /* computation of z_5 and rc_5 */
    _computation_z_rc_5_NTTq1 (pres->Rq1_z5_V3, Rq1_rc5_V3,
                               NTTq1_sigma5_r1, NTTq1_sigma5_r2,
                           NTTq1_sigma5_r3, NTTq1_chall, NTTq1_sample_xi_y_5_V3);

    /* computation of z_enc and rc_enc */
    _computation_z_rc_enc_NTTQ (pres->RQ_zenc_V5,  RQ_rcenc_V5,
                                NTTQ_chall, NTTQ_r_enc,
                             NTTQ_e1_enc, NTTQ_e2_1_enc, NTTQ_e2_2_enc,
                             NTTQ_e2_3_enc, NTTQ_sample_xi_y_enc_V5);
    /* computation of z_s1 and rc_s1 */
    _computation_z_rc_s1_NTTq2 (pres->Rq2_zs1_V4,  Rq2_rcs1_V4, 
                                cred, NTTq2_chall,
                            NTTq2_sample_xi1_y_s1_V4);

    /* computation of z_s2 and rc_s2 */
    _computation_z_rc_s2_NTTq2 (pres->Rq2_zs2_V2, Rq2_rcs2_V2,
                                cred, NTTq2_chall,
                            NTTq2_sample_xi2_y_s2_V2, NTTq2_r2, NTTq2_r3,
                            NTTq2_r2_prime, NTTq2_r3_prime, NTTq2_tmp);

    /* we put the z in R form */
    conv_NTT_polyq1_to_R_polyq1_V(pres->Rq1_z_V3,3);
    conv_NTT_polyq1_to_R_polyq1_V(pres->Rq1_z_prime_V3,3);
    conv_NTT_polyq1_to_R_polyq1_V(pres->Rq1_z1_V3,3);
    conv_NTT_polyq1_to_R_polyq1_V(pres->Rq1_z5_V3,3);
    conv_NTT_polyQ_to_R_polyQ_V(pres->RQ_zenc_V5,5);
    conv_NTT_polyq2_to_R_polyq2_V(pres->Rq2_zs1_V4,4);
    conv_NTT_polyq2_to_R_polyq2_V(pres->Rq2_zs2_V2,2);

    /* we put the rc in R form */
    conv_NTT_polyq1_to_R_polyq1_V(Rq1_rc_V3,3);
    conv_NTT_polyq1_to_R_polyq1_V(Rq1_rc_prime_V3,3);
    conv_NTT_polyq1_to_R_polyq1_V(Rq1_rc1_V3,3);
    conv_NTT_polyq1_to_R_polyq1_V(Rq1_rc5_V3,3);
    conv_NTT_polyQ_to_R_polyQ_V(RQ_rcenc_V5,5);
    conv_NTT_polyq2_to_R_polyq2_V(Rq2_rcs1_V4,4);
    conv_NTT_polyq2_to_R_polyq2_V(Rq2_rcs2_V2,2);
    
    accept = rejection_sampling (pres->Rq1_z_V3, pres->Rq1_z_prime_V3, 
                                 pres->Rq1_z1_V3, pres->Rq1_z5_V3,
                                 pres->RQ_zenc_V5, Rq1_rc_V3,
                                 Rq1_rc_prime_V3, Rq1_rc1_V3,
                                 Rq1_rc5_V3, RQ_rcenc_V5,
                                 pres->Rq2_zs1_V4, Rq2_rcs1_V4,
                                 pres->Rq2_zs2_V2, Rq2_rcs2_V2);
  }

  /* we clear the variables */
  /* random vars for commitments t and t_prime */
  polyq1_clear (NTTq1_r1);
  polyq1_clear (NTTq1_r2);
  polyq1_clear (NTTq1_r3);
  polyq2_clear (NTTq2_r2);
  polyq2_clear (NTTq2_r3);
  polyQ_clear (NTTQ_r1);
  polyQ_clear (NTTQ_r2);
  polyQ_clear (NTTQ_r3);

  polyq1_clear (NTTq1_r1_prime);
  polyq1_clear (NTTq1_r2_prime);
  polyq1_clear (NTTq1_r3_prime);
  polyq2_clear (NTTq2_r2_prime);
  polyq2_clear (NTTq2_r3_prime);

  /* tmp vars */
  polyq1_clear (NTTq1_tmp);
  polyq2_clear (NTTq2_tmp);
  polyq2_clear (NTTq2_tmp2);
  polyq2_clear (NTTq2_tmp3);
  polyQ_clear (NTTQ_tmp);

  /* vars for verifiable encryption */
  polyQ_clear (NTTQ_r_enc);
  polyQ_clear (NTTQ_e1_enc);
  polyQ_clear (NTTQ_e2_1_enc);
  polyQ_clear (NTTQ_e2_2_enc);
  polyQ_clear (NTTQ_e2_3_enc);

  /* challenges */
  challenge_clear (challenge_r1);
  challenge_clear (challenge_r2);
  challenge_clear (challenge_r3);
  challenge_clear (challenge_r1_prime);
  challenge_clear (challenge_r2_prime);
  challenge_clear (challenge_r3_prime);

  /* gaussian samples with xi */
  polyq1_clear_V (NTTq1_sample_xi_y_V3,3);
  polyq2_clear_V (NTTq2_sample_xi_y_V3,3);
  polyq1_clear_V (NTTq1_sample_xi_y_prime_V3,3);
  polyq2_clear_V (NTTq2_sample_xi_y_prime_V3,3);
  polyq1_clear_V (NTTq1_sample_xi_y_1_V3,3);
  polyq2_clear_V (NTTq2_sample_xi_y_1_V3,3);
  polyq1_clear_V (NTTq1_sample_xi_y_5_V3,3);
  polyq2_clear_V (NTTq2_sample_xi_y_5_V3,3);

  /* gaussian samples with xi1 */
  polyq2_clear_V (NTTq2_sample_xi1_y_s1_V4,4);

  /* gaussian samples with xi2 */
  polyq2_clear_V (NTTq2_sample_xi2_y_s2_V2,2);

  /* vars for the w of the ZK proofs */
  polyq1_clear (NTTq1_w1_gen);
  polyq2_clear (NTTq2_w2_gen);
  polyq1_clear (NTTq1_w1_prime_gen);
  polyq1_clear (NTTq1_w11_gen);
  polyq2_clear (NTTq2_w12_gen);
  polyq1_clear (NTTq1_w51_gen);
  polyq2_clear (NTTq2_w52_gen);
  polyq2_clear (NTTq2_ws_gen);
  polyQ_clear_V (NTTQ_wenc_gen_V4,4);

  /* vars for the rc */
  polyq1_clear_V(Rq1_rc_V3,3);
  polyq1_clear_V(Rq1_rc_prime_V3,3);
  polyq1_clear_V(Rq1_rc1_V3,3);
  polyq1_clear_V(Rq1_rc5_V3,3);
  polyQ_clear_V(RQ_rcenc_V5,5);
  polyq2_clear_V(Rq2_rcs1_V4,4);
  polyq2_clear_V(Rq2_rcs2_V2,2);

  /* vars for the challenge */
  polyq1_clear (NTTq1_chall);
  polyq2_clear (NTTq2_chall);
  polyQ_clear (NTTQ_chall);
  return EXIT_SUCCESS;
}

int
presentation_verify (s_presentation_t pres, s_issuer_pk_t ipk,
                     s_opener_pk_t opk, attribute_t attr, message_t message)
{
  /* set to false at the begining */
  int result_verification = 0;

  /* the challenge we will recompute, in order to check it is equal to
     pres->challenge_chall */
  challenge_t challenge_recomputed_chall;
  challenge_init (challenge_recomputed_chall);

  /* vars for the challenge */
  polyq1_t NTTq1_chall;
  polyq2_t NTTq2_chall;
  polyQ_t NTTQ_chall;
  polyq1_init (NTTq1_chall);
  polyq2_init (NTTq2_chall);
  polyQ_init (NTTQ_chall);

  polyq1_t NTTq1_sigma_1_commit_t1;
  polyq1_init (NTTq1_sigma_1_commit_t1);
  polyq1_t NTTq1_sigma_5_commit_t1;
  polyq1_init (NTTq1_sigma_5_commit_t1);
  polyq2_t NTTq2_sigma_1_commit_t2;
  polyq2_init (NTTq2_sigma_1_commit_t2);
  polyq2_t NTTq2_sigma_5_commit_t2;
  polyq2_init (NTTq2_sigma_5_commit_t2);
  /* context used for the hash function that compute the challenge */
  shake256_context shake256_context_hash_challenge;

  /* w vars */
  polyq1_t NTTq1_w1_recomp;
  polyq1_init (NTTq1_w1_recomp);
  polyq2_t NTTq2_w2_recomp;
  polyq2_init (NTTq2_w2_recomp);
  polyq1_t NTTq1_w1_prime_recomp;
  polyq1_init (NTTq1_w1_prime_recomp);
  polyq1_t NTTq1_w11_recomp;
  polyq1_init (NTTq1_w11_recomp);
  polyq2_t NTTq2_w12_recomp;
  polyq2_init (NTTq2_w12_recomp);
  polyq1_t NTTq1_w51_recomp;
  polyq1_init (NTTq1_w51_recomp);
  polyq2_t NTTq2_w52_recomp;
  polyq2_init (NTTq2_w52_recomp);
  polyq2_t NTTq2_ws_recomp;
  polyq2_init (NTTq2_ws_recomp);
  polyQ_t NTTQ_wenc_recomp_V4[4];
  polyQ_init_V (NTTQ_wenc_recomp_V4,4);

  polyQ_t NTTQ_z_V3[3];
  polyQ_init_V (NTTQ_z_V3,3);
  conv_polyq1_to_polyQ (NTTQ_z_V3[0], pres->Rq1_z_V3[0]);
  conv_polyq1_to_polyQ (NTTQ_z_V3[1], pres->Rq1_z_V3[1]);
  conv_polyq1_to_polyQ (NTTQ_z_V3[2], pres->Rq1_z_V3[2]);
  conv_R_polyQ_to_NTT_polyQ_V (NTTQ_z_V3,3);

  polyq2_t NTTq2_z_V3[3];
  polyq2_init_V (NTTq2_z_V3,3);
  conv_polyq1_to_polyq2 (NTTq2_z_V3[0], pres->Rq1_z_V3[0]);
  conv_polyq1_to_polyq2 (NTTq2_z_V3[1], pres->Rq1_z_V3[1]);
  conv_polyq1_to_polyq2 (NTTq2_z_V3[2], pres->Rq1_z_V3[2]);
  conv_R_polyq2_to_NTT_polyq2_V (NTTq2_z_V3,3);

  polyq2_t NTTq2_z_prime_V3[3];
  polyq2_init_V (NTTq2_z_prime_V3,3);
  conv_polyq1_to_polyq2 (NTTq2_z_prime_V3[0], pres->Rq1_z_prime_V3[0]);
  conv_polyq1_to_polyq2 (NTTq2_z_prime_V3[1], pres->Rq1_z_prime_V3[1]);
  conv_polyq1_to_polyq2 (NTTq2_z_prime_V3[2], pres->Rq1_z_prime_V3[2]);
  conv_R_polyq2_to_NTT_polyq2_V (NTTq2_z_prime_V3,3);

  polyq1_t NTTq1_z_V3[3];
  polyq1_init_V (NTTq1_z_V3,3);
  polyq1_copy(NTTq1_z_V3[0], pres->Rq1_z_V3[0]);
  polyq1_copy(NTTq1_z_V3[1], pres->Rq1_z_V3[1]);
  polyq1_copy(NTTq1_z_V3[2], pres->Rq1_z_V3[2]);
  conv_R_polyq1_to_NTT_polyq1_V(NTTq1_z_V3,3);

  polyq1_t NTTq1_z_prime_V3[3];
  polyq1_init_V (NTTq1_z_prime_V3,3);
  polyq1_copy(NTTq1_z_prime_V3[0], pres->Rq1_z_prime_V3[0]);
  polyq1_copy(NTTq1_z_prime_V3[1], pres->Rq1_z_prime_V3[1]);
  polyq1_copy(NTTq1_z_prime_V3[2], pres->Rq1_z_prime_V3[2]);
  conv_R_polyq1_to_NTT_polyq1_V(NTTq1_z_prime_V3,3);

  polyq1_t NTTq1_z1_V3[3];
  polyq1_init_V (NTTq1_z1_V3,3);
  polyq1_copy(NTTq1_z1_V3[0], pres->Rq1_z1_V3[0]);
  polyq1_copy(NTTq1_z1_V3[1], pres->Rq1_z1_V3[1]);
  polyq1_copy(NTTq1_z1_V3[2], pres->Rq1_z1_V3[2]);
  polyq2_t NTTq2_z1_V3[3];
  polyq2_init_V (NTTq2_z1_V3,3);
  conv_polyq1_to_polyq2(NTTq2_z1_V3[0], NTTq1_z1_V3[0]);
  conv_polyq1_to_polyq2(NTTq2_z1_V3[1], NTTq1_z1_V3[1]);
  conv_polyq1_to_polyq2(NTTq2_z1_V3[2], NTTq1_z1_V3[2]);
  conv_R_polyq1_to_NTT_polyq1_V(NTTq1_z1_V3,3);
  conv_R_polyq2_to_NTT_polyq2_V(NTTq2_z1_V3,3);


  polyq1_t NTTq1_z5_V3[3];
  polyq1_init_V (NTTq1_z5_V3,3);
  polyq1_copy(NTTq1_z5_V3[0], pres->Rq1_z5_V3[0]);
  polyq1_copy(NTTq1_z5_V3[1], pres->Rq1_z5_V3[1]);
  polyq1_copy(NTTq1_z5_V3[2], pres->Rq1_z5_V3[2]);
  polyq2_t NTTq2_z5_V3[3];
  polyq2_init_V (NTTq2_z5_V3,3);
  conv_polyq1_to_polyq2(NTTq2_z5_V3[0], NTTq1_z5_V3[0]);
  conv_polyq1_to_polyq2(NTTq2_z5_V3[1], NTTq1_z5_V3[1]);
  conv_polyq1_to_polyq2(NTTq2_z5_V3[2], NTTq1_z5_V3[2]);
  conv_R_polyq1_to_NTT_polyq1_V(NTTq1_z5_V3,3);
  conv_R_polyq2_to_NTT_polyq2_V(NTTq2_z5_V3,3);

  polyQ_t NTTQ_zenc_V5[5];
  polyQ_init_V (NTTQ_zenc_V5,5);
  polyQ_copy(NTTQ_zenc_V5[0], pres->RQ_zenc_V5[0]);
  polyQ_copy(NTTQ_zenc_V5[1], pres->RQ_zenc_V5[1]);
  polyQ_copy(NTTQ_zenc_V5[2], pres->RQ_zenc_V5[2]);
  polyQ_copy(NTTQ_zenc_V5[3], pres->RQ_zenc_V5[3]);
  polyQ_copy(NTTQ_zenc_V5[4], pres->RQ_zenc_V5[4]);
  conv_R_polyQ_to_NTT_polyQ_V(NTTQ_zenc_V5,5);

  polyq2_t NTTq2_zs1_V4[4];
  polyq2_init_V (NTTq2_zs1_V4,4);
  polyq2_copy(NTTq2_zs1_V4[0], pres->Rq2_zs1_V4[0]);
  polyq2_copy(NTTq2_zs1_V4[1], pres->Rq2_zs1_V4[1]);
  polyq2_copy(NTTq2_zs1_V4[2], pres->Rq2_zs1_V4[2]);
  polyq2_copy(NTTq2_zs1_V4[3], pres->Rq2_zs1_V4[3]);
  conv_R_polyq2_to_NTT_polyq2_V(NTTq2_zs1_V4,4);

  polyq2_t NTTq2_zs2_V2[2];
  polyq2_init_V (NTTq2_zs2_V2,2);
  polyq2_copy(NTTq2_zs2_V2[0], pres->Rq2_zs2_V2[0]);
  polyq2_copy(NTTq2_zs2_V2[1], pres->Rq2_zs2_V2[1]);
  conv_R_polyq2_to_NTT_polyq2_V(NTTq2_zs2_V2,2);

  /* tmp vars */
  polyq1_t NTTq1_tmp;
  polyq1_init (NTTq1_tmp);
  polyq2_t NTTq2_tmp, NTTq2_tmp2, NTTq2_tmp3;
  polyq2_init (NTTq2_tmp);
  polyq2_init (NTTq2_tmp2);
  polyq2_init (NTTq2_tmp3);
  polyQ_t NTTQ_tmp;
  polyQ_init (NTTQ_tmp);

  conv_challenge_to_polyq1 (NTTq1_chall, pres->challenge_chall);
  conv_R_polyq1_to_NTT_polyq1 (NTTq1_chall);
  conv_challenge_to_polyq2 (NTTq2_chall, pres->challenge_chall);
  conv_R_polyq2_to_NTT_polyq2 (NTTq2_chall);
  conv_challenge_to_polyQ (NTTQ_chall, pres->challenge_chall);
  conv_R_polyQ_to_NTT_polyQ (NTTQ_chall);

  _recomputation_w1_NTTq1 (NTTq1_w1_recomp, ipk, NTTq1_chall, pres->NTTq1_commit_t1,
                           NTTq1_z_V3, NTTq1_tmp);
  _recomputation_w1_prime_NTTq1 (NTTq1_w1_prime_recomp, ipk, NTTq1_chall,
                                 pres->NTTq1_commit_t1_prime,
                                 NTTq1_z_prime_V3, NTTq1_tmp);
  _recomputation_w2_NTTq2 (NTTq2_w2_recomp, ipk, NTTq2_chall, pres->NTTq2_commit_t2,
                           pres->NTTq2_commit_t2_prime, NTTq2_z_V3,
                           NTTq2_z_prime_V3, NTTq2_tmp, NTTq2_tmp2,
                           NTTq2_tmp3);

  /* we need to have sigma_-1 of commit_t_1 */
  polyq1_copy (NTTq1_sigma_1_commit_t1, pres->NTTq1_commit_t1);
  conv_NTT_polyq1_to_R_polyq1 (NTTq1_sigma_1_commit_t1);
  polyq1_sigma_1 (NTTq1_sigma_1_commit_t1, NTTq1_sigma_1_commit_t1);
  conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma_1_commit_t1);
  _recomputation_w11_NTTq1 (NTTq1_w11_recomp, ipk, NTTq1_chall,
                            NTTq1_sigma_1_commit_t1, NTTq1_z1_V3,
                            NTTq1_tmp);
  /* we need to have sigma_5 of commit_t_1 */
  polyq1_copy (NTTq1_sigma_5_commit_t1, pres->NTTq1_commit_t1);
  conv_NTT_polyq1_to_R_polyq1 (NTTq1_sigma_5_commit_t1);
  polyq1_sigma_5 (NTTq1_sigma_5_commit_t1, NTTq1_sigma_5_commit_t1);
  conv_R_polyq1_to_NTT_polyq1 (NTTq1_sigma_5_commit_t1);
  _recomputation_w51_NTTq1 (NTTq1_w51_recomp, ipk, NTTq1_chall,
                            NTTq1_sigma_5_commit_t1, NTTq1_z5_V3,
                            NTTq1_tmp);
  /* we need to have sigma_-1 of commit_t_2 */
  polyq2_copy (NTTq2_sigma_1_commit_t2, pres->NTTq2_commit_t2);
  conv_NTT_polyq2_to_R_polyq2 (NTTq2_sigma_1_commit_t2);
  polyq2_sigma_1 (NTTq2_sigma_1_commit_t2, NTTq2_sigma_1_commit_t2);
  conv_R_polyq2_to_NTT_polyq2 (NTTq2_sigma_1_commit_t2);

  _recomputation_w12_NTTq2 (NTTq2_w12_recomp, ipk, NTTq2_chall, pres->NTTq2_commit_t2,
                            NTTq2_sigma_1_commit_t2, NTTq2_z_V3, NTTq2_z1_V3,
                            NTTq2_tmp, NTTq2_tmp2, NTTq2_tmp3);

  /* we need to have sigma_5 of commit_t_2 */
  polyq2_copy (NTTq2_sigma_5_commit_t2, pres->NTTq2_commit_t2);
  conv_NTT_polyq2_to_R_polyq2 (NTTq2_sigma_5_commit_t2);
  polyq2_sigma_5 (NTTq2_sigma_5_commit_t2, NTTq2_sigma_5_commit_t2);
  conv_R_polyq2_to_NTT_polyq2 (NTTq2_sigma_5_commit_t2);

  _recomputation_w52_NTTq2 (NTTq2_w52_recomp, ipk, NTTq2_chall, pres->NTTq2_commit_t2,
                            NTTq2_sigma_5_commit_t2, NTTq2_z_V3,
                            NTTq2_z5_V3, NTTq2_tmp,
                            NTTq2_tmp2, NTTq2_tmp3);

  /* we need to have z mod Q */
  conv_NTT_polyq1_to_NTT_polyQ(NTTQ_z_V3[0], NTTq1_z_V3[0]);
  conv_NTT_polyq1_to_NTT_polyQ(NTTQ_z_V3[1], NTTq1_z_V3[1]);
  conv_NTT_polyq1_to_NTT_polyQ(NTTQ_z_V3[2], NTTq1_z_V3[2]);

  _recomputation_wenc_NTTQ (NTTQ_wenc_recomp_V4, opk, NTTQ_chall, NTTQ_z_V3, NTTQ_zenc_V5, pres->NTTQ_enc1, pres->NTTQ_enc2_V3, NTTQ_tmp);
  _recomputation_ws_NTTq2 (NTTq2_ws_recomp, ipk, NTTq2_chall, pres->NTTq2_commit_t2,
                           pres->NTTq2_commit_t2_prime, NTTq2_zs1_V4,
                           NTTq2_zs2_V2, NTTq2_tmp, attr);

   /* COMPUTATION OF THE CHALLENGE */
   /* We hash, in the following order :      
   * the commitment t :
   ************* NTTq1_commit_t1
   ************* NTTq2_commit_t2
   * the commitment t'
   ************* NTTq1_commit_t1_prime
   ************* NTTq2_commit_t2_prime
   * the verifiable encryption
   ************* NTTQ_enc1
   ************* NTTQ_enc2_V3[0]
   ************* NTTQ_enc2_V3[1]
   ************* NTTQ_enc2_v2[2])
   * NTTq1_w1_recomp
   * NTTq1_w1_prime_recomp
   * NTTq1_w11_recomp
   * NTTq1_w51_recomp
   * NTTq2_w2_recomp
   * NTTq2_w1_recomp2
   * NTTq2_w52_recomp
   * NTTq2_ws_recomp
   * NTTQ_wenc_recomp_V4[0]
   * NTTQ_wenc_recomp_V4[1]
   * NTTQ_wenc_recomp_V4[2]
   * NTTQ_wenc_recomp_V4[3]
   * the message M
   * the attribute attr
   */
  shake256_init (&shake256_context_hash_challenge);

  shake256_inject_polyq1 (&shake256_context_hash_challenge,
                          pres->NTTq1_commit_t1);
  shake256_inject_polyq2 (&shake256_context_hash_challenge,
                          pres->NTTq2_commit_t2);
  shake256_inject_polyq1 (&shake256_context_hash_challenge,
                          pres->NTTq1_commit_t1_prime);
  shake256_inject_polyq2 (&shake256_context_hash_challenge,
                          pres->NTTq2_commit_t2_prime);
  shake256_inject_polyQ (&shake256_context_hash_challenge, pres->NTTQ_enc1);
  shake256_inject_polyQ (&shake256_context_hash_challenge,
                         pres->NTTQ_enc2_V3[0]);
  shake256_inject_polyQ (&shake256_context_hash_challenge,
                         pres->NTTQ_enc2_V3[1]);
  shake256_inject_polyQ (&shake256_context_hash_challenge,
                         pres->NTTQ_enc2_V3[2]);
  shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w1_recomp);
   shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w1_prime_recomp);
  shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w11_recomp);
  shake256_inject_polyq1 (&shake256_context_hash_challenge, NTTq1_w51_recomp);
  shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_w2_recomp);
  shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_w12_recomp);
  shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_w52_recomp);
  shake256_inject_polyq2 (&shake256_context_hash_challenge, NTTq2_ws_recomp);
   shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_recomp_V4[0]);
   shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_recomp_V4[1]);
   shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_recomp_V4[2]);
   shake256_inject_polyQ (&shake256_context_hash_challenge, NTTQ_wenc_recomp_V4[3]);
   
  shake256_inject (&shake256_context_hash_challenge, (void *)message,
                   SIZE_MESS_S8);
  shake256_inject (&shake256_context_hash_challenge, (void *)&attr, 1);

  shake256_flip (&shake256_context_hash_challenge);
  /* computation of the hash value */
  FDH_challenges (challenge_recomputed_chall,
                  &shake256_context_hash_challenge);
  result_verification
      = challenge_equal (challenge_recomputed_chall, pres->challenge_chall);


  /* We clear the variables. */
  polyq1_clear (NTTq1_tmp);
  polyq2_clear (NTTq2_tmp);
  polyq2_clear (NTTq2_tmp2);
  polyq2_clear (NTTq2_tmp3);
  polyQ_clear (NTTQ_tmp);
  /* the w */
  polyq1_clear (NTTq1_w1_recomp);
  polyq2_clear (NTTq2_w2_recomp);
  polyq1_clear (NTTq1_w1_prime_recomp);
  polyq1_clear (NTTq1_w11_recomp);
  polyq2_clear (NTTq2_w12_recomp);
  polyq1_clear (NTTq1_w51_recomp);
  polyq2_clear (NTTq2_w52_recomp);
  polyq2_clear (NTTq2_ws_recomp);
  polyQ_clear_V (NTTQ_wenc_recomp_V4,4);

  polyQ_clear_V (NTTQ_z_V3,3);
  polyq2_clear_V (NTTq2_z_prime_V3,3);
  polyq2_clear_V (NTTq2_z_V3,3);
  polyq2_clear_V (NTTq2_z1_V3,3);
  polyq2_clear_V (NTTq2_z5_V3,3);

  /* vars for the challenge */
  polyq1_clear (NTTq1_chall);
  polyq2_clear (NTTq2_chall);
  polyQ_clear (NTTQ_chall);

  polyq1_clear (NTTq1_sigma_1_commit_t1);
  polyq1_clear (NTTq1_sigma_5_commit_t1);
  polyq2_clear (NTTq2_sigma_1_commit_t2);
  polyq2_clear (NTTq2_sigma_5_commit_t2);

  challenge_clear (challenge_recomputed_chall);

  return result_verification;
}

int cred_equal(s_credential_t cred1, s_credential_t cred2)
{
  int result;
  result = polyq2_equal(cred1->NTTq2_s1_V2[0], cred2->NTTq2_s1_V2[0]);
  result &= polyq2_equal(cred1->NTTq2_s1_V2[1], cred2->NTTq2_s1_V2[1]);
  result &= polyq2_equal(cred1->NTTq2_s2_V2[0], cred2->NTTq2_s2_V2[0]);
  result &= polyq2_equal(cred1->NTTq2_s2_V2[1], cred2->NTTq2_s2_V2[1]);
  result &= polyq2_equal(cred1->NTTq2_s3_V2[0], cred2->NTTq2_s3_V2[0]);
  result &= polyq2_equal(cred1->NTTq2_s3_V2[1], cred2->NTTq2_s3_V2[1]);
  return result;
}

int pres_equal(s_presentation_t pres1, s_presentation_t pres2)
{
  int result;
  result = polyq1_equal(pres1->NTTq1_commit_t1, pres2->NTTq1_commit_t1);
  result &= polyq1_equal(pres1->NTTq1_commit_t1_prime, pres2->NTTq1_commit_t1_prime);
  result &= polyq2_equal(pres1->NTTq2_commit_t2, pres2->NTTq2_commit_t2);
  result &= polyq2_equal(pres1->NTTq2_commit_t2_prime, pres2->NTTq2_commit_t2_prime);
  result &= polyQ_equal(pres1->NTTQ_enc1, pres2->NTTQ_enc1);
  result &= polyQ_equal(pres1->NTTQ_enc2_V3[0], pres2->NTTQ_enc2_V3[0]);
  result &= polyQ_equal(pres1->NTTQ_enc2_V3[1], pres2->NTTQ_enc2_V3[1]);
  result &= polyQ_equal(pres1->NTTQ_enc2_V3[2], pres2->NTTQ_enc2_V3[2]);
  result &= polyq1_equal(pres1->Rq1_z_V3[0], pres2->Rq1_z_V3[0]);
  result &= polyq1_equal(pres1->Rq1_z_V3[1], pres2->Rq1_z_V3[1]);
  result &= polyq1_equal(pres1->Rq1_z_V3[2], pres2->Rq1_z_V3[2]);
  result &= polyq1_equal(pres1->Rq1_z_prime_V3[0], pres2->Rq1_z_prime_V3[0]);
  result &= polyq1_equal(pres1->Rq1_z_prime_V3[1], pres2->Rq1_z_prime_V3[1]);
  result &= polyq1_equal(pres1->Rq1_z_prime_V3[2], pres2->Rq1_z_prime_V3[2]);
  result &= polyq1_equal(pres1->Rq1_z1_V3[0], pres2->Rq1_z1_V3[0]);
  result &= polyq1_equal(pres1->Rq1_z1_V3[1], pres2->Rq1_z1_V3[1]);
  result &= polyq1_equal(pres1->Rq1_z1_V3[2], pres2->Rq1_z1_V3[2]);
  result &= polyq1_equal(pres1->Rq1_z5_V3[0], pres2->Rq1_z5_V3[0]);
  result &= polyq1_equal(pres1->Rq1_z5_V3[1], pres2->Rq1_z5_V3[1]);
  result &= polyq1_equal(pres1->Rq1_z5_V3[2], pres2->Rq1_z5_V3[2]);
  result &= polyQ_equal(pres1->RQ_zenc_V5[0], pres2->RQ_zenc_V5[0]);
  result &= polyQ_equal(pres1->RQ_zenc_V5[1], pres2->RQ_zenc_V5[1]);
  result &= polyQ_equal(pres1->RQ_zenc_V5[2], pres2->RQ_zenc_V5[2]);
  result &= polyQ_equal(pres1->RQ_zenc_V5[3], pres2->RQ_zenc_V5[3]);
  result &= polyQ_equal(pres1->RQ_zenc_V5[4], pres2->RQ_zenc_V5[4]);
  result &= polyq2_equal(pres1->Rq2_zs1_V4[0], pres2->Rq2_zs1_V4[0]);
  result &= polyq2_equal(pres1->Rq2_zs1_V4[1], pres2->Rq2_zs1_V4[1]);
  result &= polyq2_equal(pres1->Rq2_zs1_V4[2], pres2->Rq2_zs1_V4[2]);
  result &= polyq2_equal(pres1->Rq2_zs1_V4[3], pres2->Rq2_zs1_V4[3]);
  result &= polyq2_equal(pres1->Rq2_zs2_V2[0], pres2->Rq2_zs2_V2[0]);
  result &= polyq2_equal(pres1->Rq2_zs2_V2[1], pres2->Rq2_zs2_V2[1]);
  result &= challenge_equal(pres1->challenge_chall, pres2->challenge_chall);
  return result;
}

int opk_equal(s_opener_pk_t opk1, s_opener_pk_t opk2)
{
  int result;
  result =  polyQ_equal(opk1->NTTQ_a, opk2->NTTQ_a);
  result &=  polyQ_equal(opk1->NTTQ_b_V3[0], opk2->NTTQ_b_V3[0]);
  result &=  polyQ_equal(opk1->NTTQ_b_V3[1], opk2->NTTQ_b_V3[1]);
  result &=  polyQ_equal(opk1->NTTQ_b_V3[2], opk2->NTTQ_b_V3[2]);
  return result;
}

int ipk_equal(s_issuer_pk_t ipk1, s_issuer_pk_t ipk2)
{
  int result;
  result = polyq1_equal(ipk1->NTTq1_commit_a1, ipk2->NTTq1_commit_a1);
  result &= polyq1_equal(ipk1->NTTq1_sigma1_commit_a1, ipk2->NTTq1_sigma1_commit_a1);
  result &= polyq1_equal(ipk1->NTTq1_sigma5_commit_a1, ipk2->NTTq1_sigma5_commit_a1);

  result &= polyq1_equal(ipk1->NTTq1_commit_a2, ipk2->NTTq1_commit_a2);
  result &= polyq1_equal(ipk1->NTTq1_sigma1_commit_a2, ipk2->NTTq1_sigma1_commit_a2);
  result &= polyq1_equal(ipk1->NTTq1_sigma5_commit_a2, ipk2->NTTq1_sigma5_commit_a2);

  result &= polyq2_equal(ipk1->NTTq2_commit_a3, ipk2->NTTq2_commit_a3);
  result &= polyq2_equal(ipk1->NTTq2_sigma1_commit_a3, ipk2->NTTq2_sigma1_commit_a3);
  result &= polyq2_equal(ipk1->NTTq2_sigma5_commit_a3, ipk2->NTTq2_sigma5_commit_a3);

  result &= polyq2_equal(ipk1->NTTq2_a_V2[0], ipk2->NTTq2_a_V2[0]);
  result &= polyq2_equal(ipk1->NTTq2_a_V2[1], ipk2->NTTq2_a_V2[1]);
  result &= polyq2_equal(ipk1->NTTq2_b_V2[0], ipk2->NTTq2_b_V2[0]);
  result &= polyq2_equal(ipk1->NTTq2_b_V2[1], ipk2->NTTq2_b_V2[1]);
  return result;
}
