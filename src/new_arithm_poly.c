#include <stdio.h>
#include <stdlib.h>

#include "new_arithm_poly.h"
#include "params.h"

#include "samplers.h"

inline void
polyq1_init (polyq1_t polq1_p)
{
  *polq1_p = (intq1_t *)calloc (DIM_R, sizeof (intq1_t));
  if (polq1_p == NULL)
    {
      printf("ERROR : calloc of polyq1_init didn't work\n");
      exit(EXIT_FAILURE);
    }
}

inline void
polyq2_init (polyq2_t polq2_p)
{
  *polq2_p = (intq2_t *)calloc(DIM_R, sizeof (intq2_t));
  if (polq2_p == NULL)
    {
      printf("ERROR : calloc of polyq2_init didn't work\n");
      exit(EXIT_FAILURE);
    }
}
inline void
polyQ_init (polyQ_t polQ_p)
{
  *polQ_p = (intQ_t *)calloc (DIM_R, sizeof (intQ_t));
  if (polQ_p == NULL)
    {
      printf("ERROR : calloc of polyQ_init didn't work\n");
      exit(EXIT_FAILURE);
    }
}
inline void
polyp_init (polyp_t polp_p)
{
  *polp_p = (intp_t *)calloc (DIM_R, sizeof (intp_t));
  if (polp_p == NULL)
    {
      printf("ERROR : calloc of polyp_init didn't work\n");
      exit(EXIT_FAILURE);
    }
}
inline void
challenge_init (challenge_t chall)
{
  polyq1_init (chall);
}

inline void 
polyq1_init_V (polyq1_t *polq1_p_V, int size) { for (int i = 0; i<size; i++) {polyq1_init(polq1_p_V[i]); } }

inline void 
polyq2_init_V (polyq2_t *polq2_p_V, int size) { for (int i = 0; i<size; i++) {polyq2_init(polq2_p_V[i]); } }

inline void 
polyQ_init_V (polyQ_t *polQ_p_V, int size) { for (int i = 0; i<size; i++) {polyQ_init(polQ_p_V[i]); } }

inline void 
polyp_init_V (polyp_t *polp_p_V, int size){ for (int i = 0; i<size; i++) {polyp_init(polp_p_V[i]); } }

inline void 
challenge_init_V (challenge_t *chall_V, int size){ for (int i = 0; i<size; i++) {challenge_init(chall_V[i]); } }

inline void
polyq1_clear (polyq1_t polq1_p)
{
  free (*polq1_p);
}

inline void
polyq2_clear (polyq2_t polq2_p)
{
  free (*polq2_p);
}

inline void
polyQ_clear (polyQ_t polQ_p)
{
  free (*polQ_p);
}

inline void
polyp_clear (polyp_t polp_p)
{
  free (*polp_p);
}

inline void
challenge_clear (challenge_t chall)
{
  polyq1_clear (chall);
}

inline void 
polyq1_clear_V (polyq1_t *polq1_p_V, int size) { for (int i = 0; i<size; i++) {polyq1_clear(polq1_p_V[i]); } }

inline void 
polyq2_clear_V (polyq2_t *polq2_p_V, int size) { for (int i = 0; i<size; i++) {polyq2_clear(polq2_p_V[i]); } }

inline void 
polyQ_clear_V (polyQ_t *polQ_p_V, int size) { for (int i = 0; i<size; i++) {polyQ_clear(polQ_p_V[i]); } }

inline void 
polyp_clear_V (polyp_t *polp_p_V, int size){ for (int i = 0; i<size; i++) {polyp_clear(polp_p_V[i]); } }

inline void 
challenge_clear_V (challenge_t *chall_V, int size){ for (int i = 0; i<size; i++) {challenge_clear(chall_V[i]); } }

inline void
polyq1_copy(polyq1_t polq1_res, const polyq1_t polq1_in) 
{
  for(int i = 0; i<DIM_R; i++) {intq1_copy(COEFF(polq1_res,i),COEFF(polq1_in,i));}
}

inline void
polyq2_copy(polyq2_t polq2_res, const polyq2_t polq2_in) 
{
  for(int i = 0; i<DIM_R; i++) {intq2_copy(COEFF(polq2_res,i),COEFF(polq2_in,i));}
}

inline void
polyQ_copy(polyQ_t polQ_res, const polyQ_t polQ_in) 
{
  for(int i = 0; i<DIM_R; i++) {intQ_copy(COEFF(polQ_res,i),COEFF(polQ_in,i));}
}

inline void
polyp_copy(polyp_t polp_res, const polyp_t polp_in) 
{
  for(int i = 0; i<DIM_R; i++) {intp_copy(COEFF(polp_res,i),COEFF(polp_in,i));}
}

inline void
polyq1_add (polyq1_t polq1_res, const polyq1_t polq1_x1,
            const polyq1_t polq1_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_add (COEFF (polq1_res, i), COEFF (polq1_x1, i),
                 COEFF (polq1_x2, i));
    }
}

inline void
polyq2_add (polyq2_t polq2_res, const polyq2_t polq2_x1,
            const polyq2_t polq2_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_add (COEFF (polq2_res, i), COEFF (polq2_x1, i),
                 COEFF (polq2_x2, i));
    }
}

inline void
polyQ_add (polyQ_t polQ_res, const polyQ_t polQ_x1, const polyQ_t polQ_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intQ_add (COEFF (polQ_res, i), COEFF (polQ_x1, i), COEFF (polQ_x2, i));
    }
}

inline void
polyp_add (polyp_t polp_res, const polyp_t polp_x1, const polyp_t polp_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intp_add (COEFF (polp_res, i), COEFF (polp_x1, i), COEFF (polp_x2, i));
    }
}

inline void
polyq2_set_ui_NTT(polyq2_t polq2_res, unsigned long x) 
{
  for(int i = 0; i<THETA; i++) 
    {
      intq2_set_ui(COEFF(polq2_res,4*i), x);
      intq2_set_ui(COEFF(polq2_res,4*i+1), 0);
      intq2_set_ui(COEFF(polq2_res,4*i+2), 0);
      intq2_set_ui(COEFF(polq2_res,4*i+3), 0);
    }
}

inline void
polyq2_set_ui(polyq2_t polq2_res, unsigned long x) 
{
  intq2_set_ui(COEFF(polq2_res,0), x);
  for(int i = 1; i<DIM_R; i++) 
    {
      intq2_set_ui(COEFF(polq2_res,i), 0);
    }
}

inline void
polyq2_add_intq2_NTT (polyq2_t NTTq2_res, const polyq2_t NTTq2_pol,
                      const intq2_t intq2_x)
{
  for (int i = 0; i < THETA; i++)
    {
      intq2_add (COEFF(NTTq2_res, 4*i), COEFF(NTTq2_pol,4*i), intq2_x);
      intq2_copy (COEFF(NTTq2_res, 4*i+1), COEFF(NTTq2_pol,4*i+1));
      intq2_copy (COEFF(NTTq2_res, 4*i+2), COEFF(NTTq2_pol,4*i+2));
      intq2_copy (COEFF(NTTq2_res, 4*i+3), COEFF(NTTq2_pol,4*i+3));
    }
}

inline void
polyq1_sub (polyq1_t polq1_res, polyq1_t polq1_x1, polyq1_t polq1_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_sub (COEFF (polq1_res, i), COEFF (polq1_x1, i),
                 COEFF (polq1_x2, i));
    }
}

inline void
polyq2_sub (polyq2_t polq2_res, polyq2_t polq2_x1, polyq2_t polq2_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_sub (COEFF (polq2_res, i), COEFF (polq2_x1, i),
                 COEFF (polq2_x2, i));
    }
}

inline void
polyQ_sub (polyQ_t polQ_res, polyQ_t polQ_x1, polyQ_t polQ_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intQ_sub (COEFF (polQ_res, i), COEFF (polQ_x1, i), COEFF (polQ_x2, i));
    }
}

inline void
polyp_sub (polyp_t polp_res, polyp_t polp_x1, polyp_t polp_x2)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intp_sub (COEFF (polp_res, i), COEFF (polp_x1, i), COEFF (polp_x2, i));
    }
}

inline void
polyq1_neg (polyq1_t polq1_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_neg (COEFF (polq1_x, i));
    }
}

inline void
polyq2_neg (polyq2_t polq2_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_neg (COEFF (polq2_x, i));
    }
}

inline void
polyQ_neg (polyQ_t polQ_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intQ_neg (COEFF (polQ_x, i));
    }
}

inline void
polyp_neg (polyp_t polp_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intp_neg (COEFF (polp_x, i));
    }
}

inline void
polyq1_scalar_mul (polyq1_t polq1_res, polyq1_t polq1_pol, intq1_t intq1_scal)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_mul (COEFF (polq1_res, i), COEFF (polq1_pol, i), intq1_scal);
    }
}

inline void
polyq2_scalar_mul (polyq2_t polq2_res, polyq2_t polq2_pol, intq2_t intq2_scal)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_mul (COEFF (polq2_res, i), COEFF (polq2_pol, i), intq2_scal);
    }
}

inline void
polyQ_scalar_mul (polyQ_t polQ_res, polyQ_t polQ_pol, intQ_t intQ_scal)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intQ_mul (COEFF (polQ_res, i), COEFF (polQ_pol, i), intQ_scal);
    }
}

inline void
polyp_scalar_mul (polyp_t polp_res, polyp_t polp_pol, intp_t intp_scal)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intp_mul (COEFF (polp_res, i), COEFF (polp_pol, i), intp_scal);
    }
}

/* ###################################################### */
/* ########### Operations on NTT-polynomial ############# */
/* ###################################################### */
void
polyq1_mul_NTT(polyq1_t NTTq1_res, const polyq1_t NTTq1_pol1,
                    const polyq1_t NTTq1_pol2) {
  intq1_t intq1_tmp, intq1_r0, intq1_r1, intq1_r2, intq1_r3;

  for (int i = 0; i < THETA; i++) {
    /* Computation of r0. */
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+1), COEFF(NTTq1_pol2,i*4+3));
    intq1_mul(intq1_r0, COEFF(NTTq1_pol1,i*4+3), COEFF(NTTq1_pol2,i*4+1));
    intq1_add(intq1_r0, intq1_r0, intq1_tmp);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+2), COEFF(NTTq1_pol2,i*4+2));
    intq1_add(intq1_r0, intq1_r0, intq1_tmp);
    intq1_mul(intq1_r0, intq1_r0, g_pow_zeta_q1[zeta_exponents_ntt_order[i] - 1]);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4), COEFF(NTTq1_pol2,i*4));
    intq1_add(intq1_r0, intq1_r0, intq1_tmp);

    /* Computation of r1. */
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+2), COEFF(NTTq1_pol2,i*4+3));
    intq1_mul(intq1_r1, COEFF(NTTq1_pol1,i*4+3), COEFF(NTTq1_pol2,i*4+2));
    intq1_add(intq1_r1, intq1_r1, intq1_tmp);
    intq1_mul(intq1_r1, intq1_r1, g_pow_zeta_q1[zeta_exponents_ntt_order[i] - 1]);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4), COEFF(NTTq1_pol2,i*4+1));
    intq1_add(intq1_r1, intq1_r1, intq1_tmp);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+1), COEFF(NTTq1_pol2,i*4));
    intq1_add(intq1_r1, intq1_r1, intq1_tmp);

    /* Computation of r2. */
    intq1_mul(intq1_r2, COEFF(NTTq1_pol1,i*4+3), COEFF(NTTq1_pol2,i*4+3));
    intq1_mul(intq1_r2, g_pow_zeta_q1[zeta_exponents_ntt_order[i] - 1], intq1_r2);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+1), COEFF(NTTq1_pol2,i*4+1));
    intq1_add(intq1_r2, intq1_r2, intq1_tmp);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+2), COEFF(NTTq1_pol2,i*4));
    intq1_add(intq1_r2, intq1_r2, intq1_tmp);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4), COEFF(NTTq1_pol2,i*4+2));
    intq1_add(intq1_r2, intq1_r2, intq1_tmp);

    /* Computation of r3. */
    intq1_mul(intq1_r3, COEFF(NTTq1_pol1,i*4+2), COEFF(NTTq1_pol2,i*4+1));
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+1), COEFF(NTTq1_pol2,i*4+2));
    intq1_add(intq1_r3, intq1_r3, intq1_tmp);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4), COEFF(NTTq1_pol2,i*4+3));
    intq1_add(intq1_r3, intq1_r3, intq1_tmp);
    intq1_mul(intq1_tmp, COEFF(NTTq1_pol1,i*4+3), COEFF(NTTq1_pol2,i*4));
    intq1_add(intq1_r3, intq1_r3, intq1_tmp);

    intq1_copy(COEFF(NTTq1_res,i*4), intq1_r0);
    intq1_copy(COEFF(NTTq1_res,i*4+1), intq1_r1);
    intq1_copy(COEFF(NTTq1_res,i*4+2), intq1_r2);
    intq1_copy(COEFF(NTTq1_res,i*4+3), intq1_r3);
  }
}

void
polyq2_mul_NTT(polyq2_t NTTq2_res, const polyq2_t NTTq2_pol1,
                    const polyq2_t NTTq2_pol2) {
  intq2_t intq2_tmp, intq2_r0, intq2_r1, intq2_r2, intq2_r3;

  for (int i = 0; i < THETA; i++) {
    /* Computation of r0. */
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+1), COEFF(NTTq2_pol2,i*4+3));
    intq2_mul(intq2_r0, COEFF(NTTq2_pol1,i*4+3), COEFF(NTTq2_pol2,i*4+1));
    intq2_add(intq2_r0, intq2_r0, intq2_tmp);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+2), COEFF(NTTq2_pol2,i*4+2));
    intq2_add(intq2_r0, intq2_r0, intq2_tmp);
    intq2_mul(intq2_r0, intq2_r0, g_pow_zeta_q2[zeta_exponents_ntt_order[i] - 1]);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4), COEFF(NTTq2_pol2,i*4));
    intq2_add(intq2_r0, intq2_r0, intq2_tmp);
    /* Computation of r1. */
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+2), COEFF(NTTq2_pol2,i*4+3));
    intq2_mul(intq2_r1, COEFF(NTTq2_pol1,i*4+3), COEFF(NTTq2_pol2,i*4+2));
    intq2_add(intq2_r1, intq2_r1, intq2_tmp);
    intq2_mul(intq2_r1, intq2_r1, g_pow_zeta_q2[zeta_exponents_ntt_order[i] - 1]);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4), COEFF(NTTq2_pol2,i*4+1));
    intq2_add(intq2_r1, intq2_r1, intq2_tmp);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+1), COEFF(NTTq2_pol2,i*4));
    intq2_add(intq2_r1, intq2_r1, intq2_tmp);

    /* Computation of r2 */
    intq2_mul(intq2_r2, COEFF(NTTq2_pol1,i*4+3), COEFF(NTTq2_pol2,i*4+3));
    intq2_mul(intq2_r2, g_pow_zeta_q2[zeta_exponents_ntt_order[i] - 1], intq2_r2);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+1), COEFF(NTTq2_pol2,i*4+1));
    intq2_add(intq2_r2, intq2_r2, intq2_tmp);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+2), COEFF(NTTq2_pol2,i*4));
    intq2_add(intq2_r2, intq2_r2, intq2_tmp);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4), COEFF(NTTq2_pol2,i*4+2));
    intq2_add(intq2_r2, intq2_r2, intq2_tmp);

    /* Computation of r3 */
    intq2_mul(intq2_r3, COEFF(NTTq2_pol1,i*4+2), COEFF(NTTq2_pol2,i*4+1));
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+1), COEFF(NTTq2_pol2,i*4+2));
    intq2_add(intq2_r3, intq2_r3, intq2_tmp);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4), COEFF(NTTq2_pol2,i*4+3));
    intq2_add(intq2_r3, intq2_r3, intq2_tmp);
    intq2_mul(intq2_tmp, COEFF(NTTq2_pol1,i*4+3), COEFF(NTTq2_pol2,i*4));
    intq2_add(intq2_r3, intq2_r3, intq2_tmp);

    intq2_copy(COEFF(NTTq2_res,i*4), intq2_r0);
    intq2_copy(COEFF(NTTq2_res,i*4+1), intq2_r1);
    intq2_copy(COEFF(NTTq2_res,i*4+2), intq2_r2);
    intq2_copy(COEFF(NTTq2_res,i*4+3), intq2_r3);
  }
}

void
polyQ_mul_NTT(polyQ_t NTTQ_res, const polyQ_t NTTQ_pol1,
                    const polyQ_t NTTQ_pol2) {
  intQ_t intQ_tmp, intQ_r0, intQ_r1, intQ_r2, intQ_r3;

  for (int i = 0; i < THETA; i++) {
    /* Computation of r0. */
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+1), COEFF(NTTQ_pol2,i*4+3));
    intQ_mul(intQ_r0, COEFF(NTTQ_pol1,i*4+3), COEFF(NTTQ_pol2,i*4+1));
    intQ_add(intQ_r0, intQ_r0, intQ_tmp);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+2), COEFF(NTTQ_pol2,i*4+2));
    intQ_add(intQ_r0, intQ_r0, intQ_tmp);
    intQ_mul(intQ_r0, intQ_r0, g_pow_zeta_Q[zeta_exponents_ntt_order[i] - 1]);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4), COEFF(NTTQ_pol2,i*4));
    intQ_add(intQ_r0, intQ_r0, intQ_tmp);
    /* Computation of r1. */
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+2), COEFF(NTTQ_pol2,i*4+3));
    intQ_mul(intQ_r1, COEFF(NTTQ_pol1,i*4+3), COEFF(NTTQ_pol2,i*4+2));
    intQ_add(intQ_r1, intQ_r1, intQ_tmp);
    intQ_mul(intQ_r1, intQ_r1, g_pow_zeta_Q[zeta_exponents_ntt_order[i] - 1]);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4), COEFF(NTTQ_pol2,i*4+1));
    intQ_add(intQ_r1, intQ_r1, intQ_tmp);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+1), COEFF(NTTQ_pol2,i*4));
    intQ_add(intQ_r1, intQ_r1, intQ_tmp);

    /* Computation of r2. */
    intQ_mul(intQ_r2, COEFF(NTTQ_pol1,i*4+3), COEFF(NTTQ_pol2,i*4+3));
    intQ_mul(intQ_r2, g_pow_zeta_Q[zeta_exponents_ntt_order[i] - 1], intQ_r2);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+1), COEFF(NTTQ_pol2,i*4+1));
    intQ_add(intQ_r2, intQ_r2, intQ_tmp);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+2), COEFF(NTTQ_pol2,i*4));
    intQ_add(intQ_r2, intQ_r2, intQ_tmp);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4), COEFF(NTTQ_pol2,i*4+2));
    intQ_add(intQ_r2, intQ_r2, intQ_tmp);

    /* Computation of r3. */
    intQ_mul(intQ_r3, COEFF(NTTQ_pol1,i*4+2), COEFF(NTTQ_pol2,i*4+1));
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+1), COEFF(NTTQ_pol2,i*4+2));
    intQ_add(intQ_r3, intQ_r3, intQ_tmp);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4), COEFF(NTTQ_pol2,i*4+3));
    intQ_add(intQ_r3, intQ_r3, intQ_tmp);
    intQ_mul(intQ_tmp, COEFF(NTTQ_pol1,i*4+3), COEFF(NTTQ_pol2,i*4));
    intQ_add(intQ_r3, intQ_r3, intQ_tmp);

    intQ_copy(COEFF(NTTQ_res,i*4), intQ_r0);
    intQ_copy(COEFF(NTTQ_res,i*4+1), intQ_r1);
    intQ_copy(COEFF(NTTQ_res,i*4+2), intQ_r2);
    intQ_copy(COEFF(NTTQ_res,i*4+3), intQ_r3);
  }
}

void
conv_little_polyq1_R_polyq1_NTT (polyq1_t polyq1_pol)
{
  for (int i = 1; i < THETA; i++)
    {
      intq1_copy (COEFF (polyq1_pol, 4 * i), COEFF (polyq1_pol, 0));
      intq1_copy (COEFF (polyq1_pol, 4 * i + 1), COEFF (polyq1_pol, 1));
      intq1_copy (COEFF (polyq1_pol, 4 * i + 2), COEFF (polyq1_pol, 2));
      intq1_copy (COEFF (polyq1_pol, 4 * i + 3), COEFF (polyq1_pol, 3));
    }
}

void
conv_little_polyq2_R_polyq2_NTT (polyq2_t polyq2_pol)
{
  for (int i = 1; i < THETA; i++)
    {
      intq2_copy (COEFF (polyq2_pol, 4 * i), COEFF (polyq2_pol, 0));
      intq2_copy (COEFF (polyq2_pol, 4 * i + 1), COEFF (polyq2_pol, 1));
      intq2_copy (COEFF (polyq2_pol, 4 * i + 2), COEFF (polyq2_pol, 2));
      intq2_copy (COEFF (polyq2_pol, 4 * i + 3), COEFF (polyq2_pol, 3));
    }
}

void
conv_little_polyQ_R_polyQ_NTT (polyQ_t polyQ_pol)
{
  for (int i = 1; i < THETA; i++)
    {
      intQ_copy (COEFF (polyQ_pol, 4 * i), COEFF (polyQ_pol, 0));
      intQ_copy (COEFF (polyQ_pol, 4 * i + 1), COEFF (polyQ_pol, 1));
      intQ_copy (COEFF (polyQ_pol, 4 * i + 2), COEFF (polyQ_pol, 2));
      intQ_copy (COEFF (polyQ_pol, 4 * i + 3), COEFF (polyQ_pol, 3));
    }
}

inline void
conv_challenge_to_polyq1 (polyq1_t polq1_res, challenge_t chall)
{
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_copy (COEFF (polq1_res, i), COEFF (chall, i));
    }
}

inline void
conv_challenge_to_polyq2 (polyq2_t polq2_res, challenge_t chall)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intq1_to_intq2 (COEFF (polq2_res, i), COEFF (chall, i));
    }
}

inline void
conv_challenge_to_polyQ (polyQ_t polQ_res, challenge_t chall)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intq1_to_intQ (COEFF (polQ_res, i), COEFF (chall, i));
    }
}

inline void
conv_polyq1_to_polyq2 (polyq2_t polq2_res, polyq1_t polq1_in)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intq1_to_intq2 (COEFF (polq2_res, i), COEFF (polq1_in, i));
    }
}

inline void
conv_polyq1_to_polyQ (polyQ_t polQ_res, polyq1_t polq1_in)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intq1_to_intQ (COEFF (polQ_res, i), COEFF (polq1_in, i));
    }
}

void
polyq1_sigma_1 (polyq1_t polq1_res, polyq1_t polq1_pol)
{
  polyq1_t tmp;
  polyq1_init(tmp);
  polyq1_copy(tmp, polq1_pol);
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_copy (COEFF (polq1_res, i),
                  COEFF (tmp, ui_sigma_1_VDIM_R[i][0]));

      if (ui_sigma_1_VDIM_R[i][1] == 1)
        {
          intq1_neg (COEFF (polq1_res, i));
        }
    }
  polyq1_clear(tmp);
}

void
polyq1_sigma_5 (polyq1_t polq1_res, polyq1_t polq1_pol)
{
  polyq1_t tmp;
  polyq1_init(tmp);
  polyq1_copy(tmp, polq1_pol);
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_copy (COEFF (polq1_res, i),
                  COEFF (tmp, ui_sigma_5_VDIM_R[i][0]));

      if (ui_sigma_5_VDIM_R[i][1] == 1)
        {
          intq1_neg (COEFF (polq1_res, i));
        }
    }
  polyq1_clear(tmp);
}

void
polyq2_sigma_1 (polyq2_t polq2_res, polyq2_t polq2_pol)
{
  polyq2_t tmp;
  polyq2_init(tmp);
  polyq2_copy(tmp, polq2_pol);
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_copy (COEFF (polq2_res, i),
                  COEFF (tmp, ui_sigma_1_VDIM_R[i][0]));

      if (ui_sigma_1_VDIM_R[i][1] == 1)
        {
          intq2_neg (COEFF (polq2_res, i));
        }
    }
  polyq2_clear(tmp);
}

void
polyq2_sigma_5 (polyq2_t polq2_res, polyq2_t polq2_pol)
{
  polyq2_t tmp;
  polyq2_init(tmp);
  polyq2_copy(tmp, polq2_pol);
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_copy (COEFF (polq2_res, i),
                  COEFF (tmp, ui_sigma_5_VDIM_R[i][0]));

      if (ui_sigma_5_VDIM_R[i][1] == 1)
        {
          intq2_neg (COEFF (polq2_res, i));
        }
    }
  polyq2_clear(tmp);
}

void
challenge_sigma_5 (challenge_t chall_res, challenge_t chall_pol)
{
  polyq1_sigma_5 (chall_res, chall_pol);
}

void
challenge_sigma_1 (challenge_t chall_res, challenge_t chall_pol)
{
  polyq1_sigma_1 (chall_res, chall_pol);
}

/* Returns 1 if they are equal and 0 otherwise. */
int
polyq2_equal (const polyq2_t polq2_pol1, const polyq2_t polq2_pol2)
{
  int result = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      result = ((result)
                | (intq2_cmp (COEFF (polq2_pol1, i), COEFF (polq2_pol2, i))));
    }
  /* Result is 0 only if they are equal. */
  return ((result == 0) ? 1 : 0);
}
/* Returns 1 if they are equal and 0 otherwise. */
int
polyq1_equal (const polyq1_t polq1_pol1, const polyq1_t polq1_pol2)
{
  int result = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      result = ((result)
                | (intq1_cmp (COEFF (polq1_pol1, i), COEFF (polq1_pol2, i))));
    }
  /* Result is 0 only if they are equal. */
  return ((result == 0) ? 1 : 0);
}

/* Returns 1 if they are equal and 0 otherwise. */
int
polyQ_equal (const polyQ_t polQ_pol1, const polyQ_t polQ_pol2)
{
  int result = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      result = ((result)
                | (intQ_cmp (COEFF (polQ_pol1, i), COEFF (polQ_pol2, i))));
    }
  /* Result is 0 only if they are equal. */
  return ((result == 0) ? 1 : 0);
}

/* Returns 1 if they are equal and 0 otherwise. */
int challenge_equal(const challenge_t challenge_1, const challenge_t challenge_2)
{
  return polyq1_equal(challenge_1, challenge_2);
}

inline void
conv_polyq1_to_p_uint8 (uint8_t *p_u8_res, const polyq1_t polq1_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intq1_to_p_uint8 (p_u8_res + i * SIZE_INTQ1_S8, COEFF (polq1_x, i));
    }
}

inline void
conv_polyq2_to_p_uint8 (uint8_t *p_u8_res, const polyq2_t polq2_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intq2_to_p_uint8 (p_u8_res + i * SIZE_INTQ2_S8, COEFF (polq2_x, i));
    }
}

inline void
conv_polyQ_to_p_uint8 (uint8_t *p_u8_res, const polyQ_t polQ_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_intQ_to_p_uint8 (p_u8_res + i * SIZE_INTQ_S8, COEFF (polQ_x, i));
    }
}

void
conv_p_uint8_to_polyq1 (const polyq1_t polq1_res, uint8_t *p_u8_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_p_uint8_to_intq1 (COEFF (polq1_res, i), p_u8_x + i * SIZE_INTQ1_S8);
    }
}

void
conv_p_uint8_to_polyq2 (const polyq2_t polq2_res, uint8_t *p_u8_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_p_uint8_to_intq2 (COEFF (polq2_res, i), p_u8_x + i * SIZE_INTQ2_S8);
    }
}

void
conv_p_uint8_to_polyQ (const polyQ_t polQ_res, uint8_t *p_u8_x)
{
  for (int i = 0; i < DIM_R; i++)
    {
      conv_p_uint8_to_intQ (COEFF (polQ_res, i), p_u8_x + i * SIZE_INTQ_S8);
    }
}

/* NTT ALGORITHMS */
void
_nttq1_recursion (intq1_t *coeff_table, int length, int exponent_of_zeta)
{
  if (length == (DIM_R / THETA))
    return;
  int half_length = length / 2;
  int half_exponent_of_zeta = exponent_of_zeta / 2;
  intq1_t *coeff_table_left = coeff_table;
  intq1_t *coeff_table_right = coeff_table + half_length;
  intq1_t product;

  for (int i = 0; i < half_length; i++)
    {
      intq1_mul (product, *coeff_table_right,
                 g_pow_zeta_q1[half_exponent_of_zeta - 1]);
      intq1_sub (*coeff_table_right, *coeff_table_left, product);
      intq1_add (*coeff_table_left, *coeff_table_left, product);
      coeff_table_left++;
      coeff_table_right++;
    }

  _nttq1_recursion (coeff_table, half_length, half_exponent_of_zeta);
  _nttq1_recursion (coeff_table + half_length, half_length,
                    half_exponent_of_zeta + THETA);
}

void
conv_R_polyq1_to_NTT_polyq1 (polyq1_t poly)
{
  _nttq1_recursion (*poly, DIM_R, THETA);
}

void
_nttq2_recursion (intq2_t *coeff_table, int length, int exponent_of_zeta)
{
  if (length == (DIM_R / THETA))
    return;
  int half_length = length / 2;
  int half_exponent_of_zeta = exponent_of_zeta / 2;
  intq2_t *coeff_table_left = coeff_table;
  intq2_t *coeff_table_right = coeff_table + half_length;
  intq2_t product;

  for (int i = 0; i < half_length; i++)
    {
      intq2_mul (product, *coeff_table_right,
                 g_pow_zeta_q2[half_exponent_of_zeta - 1]);
      intq2_sub (*coeff_table_right, *coeff_table_left, product);
      intq2_add (*coeff_table_left, *coeff_table_left, product);
      coeff_table_left++;
      coeff_table_right++;
    }

  _nttq2_recursion (coeff_table, half_length, half_exponent_of_zeta);
  _nttq2_recursion (coeff_table + half_length, half_length,
                    half_exponent_of_zeta + THETA);
}

void
conv_R_polyq2_to_NTT_polyq2 (polyq2_t poly)
{
  _nttq2_recursion (*poly, DIM_R, THETA);
}

void
_nttQ_recursion (intQ_t *coeff_table, int length, int exponent_of_zeta)
{
  if (length == (DIM_R / THETA))
    return;
  int half_length = length / 2;
  int half_exponent_of_zeta = exponent_of_zeta / 2;
  intQ_t *coeff_table_left = coeff_table;
  intQ_t *coeff_table_right = coeff_table + half_length;
  intQ_t product;

  for (int i = 0; i < half_length; i++)
    {
      intQ_mul (product, *coeff_table_right,
                g_pow_zeta_Q[half_exponent_of_zeta - 1]);
      intQ_sub (*coeff_table_right, *coeff_table_left, product);
      intQ_add (*coeff_table_left, *coeff_table_left, product);
      coeff_table_left++;
      coeff_table_right++;
    }

  _nttQ_recursion (coeff_table, half_length, half_exponent_of_zeta);
  _nttQ_recursion (coeff_table + half_length, half_length,
                   half_exponent_of_zeta + THETA);
}

void
conv_R_polyQ_to_NTT_polyQ (polyQ_t poly)
{
  _nttQ_recursion (*poly, DIM_R, THETA);
}

inline void 
conv_R_polyq1_to_NTT_polyq1_V (polyq1_t *poly_V, int size) 
{for (int i = 0; i<size; i++){conv_R_polyq1_to_NTT_polyq1 (poly_V[i]); }};

inline void 
conv_R_polyq2_to_NTT_polyq2_V (polyq2_t *poly_V, int size)
{for (int i = 0; i<size; i++){conv_R_polyq2_to_NTT_polyq2 (poly_V[i]); }};

inline void 
conv_R_polyQ_to_NTT_polyQ_V (polyQ_t *poly_V, int size)
{for (int i = 0; i<size; i++){conv_R_polyQ_to_NTT_polyQ (poly_V[i]); }};

void
_reverse_nttq2_recursion (intq2_t *coeff_table, int length,
                          int exponent_of_zeta)
{
  if (length == (DIM_R / THETA))
    return;
  int half_length = length / 2;
  int half_exponent_of_zeta = exponent_of_zeta / 2;
  _reverse_nttq2_recursion (coeff_table, half_length, half_exponent_of_zeta);
  _reverse_nttq2_recursion (coeff_table + half_length, half_length,
                            half_exponent_of_zeta + THETA);
  intq2_t *coeff_table_left = coeff_table;
  intq2_t *coeff_table_right = coeff_table + half_length;
  intq2_t left_coeff_temp;

  for (int i = 0; i < half_length; i++)
    {
      intq2_copy (left_coeff_temp, *coeff_table_left);
      intq2_add (*coeff_table_left, left_coeff_temp, *coeff_table_right);
      intq2_sub (*coeff_table_right, left_coeff_temp, *coeff_table_right);
      intq2_mul (*coeff_table_right, *coeff_table_right,
                 g_pow_zeta_q2[DIM_R / 2 - half_exponent_of_zeta - 1]); //
      coeff_table_left++;
      coeff_table_right++;
    }
}

void
conv_NTT_polyq2_to_R_polyq2 (polyq2_t poly)
{
  _reverse_nttq2_recursion (*poly, DIM_R, THETA);
  intq2_t g_inverse_theta_q2;
  uint64_t p_u64_x[3];
  p_u64_x[0] = 15940796955696708119ul;
  p_u64_x[1] = 10694765990641826058ul;
  p_u64_x[2] = 8121226ul;
  conv_p_uint64_to_intq2 (g_inverse_theta_q2, p_u64_x);
  for (int i = 0; i < DIM_R; i++)
    {
      intq2_mul ((*poly)[i], (*poly)[i], g_inverse_theta_q2);
    }
}

void
_reverse_nttq1_recursion (intq1_t *coeff_table, int length,
                          int exponent_of_zeta)
{
  if (length == (DIM_R / THETA))
    return;
  int half_length = length / 2;
  int half_exponent_of_zeta = exponent_of_zeta / 2;
  _reverse_nttq1_recursion (coeff_table, half_length, half_exponent_of_zeta);
  _reverse_nttq1_recursion (coeff_table + half_length, half_length,
                            half_exponent_of_zeta + THETA);
  intq1_t *coeff_table_left = coeff_table;
  intq1_t *coeff_table_right = coeff_table + half_length;
  intq1_t left_coeff_temp;

  for (int i = 0; i < half_length; i++)
    {
      intq1_copy (left_coeff_temp, *coeff_table_left);
      intq1_add (*coeff_table_left, left_coeff_temp, *coeff_table_right);
      intq1_sub (*coeff_table_right, left_coeff_temp, *coeff_table_right);
      intq1_mul (*coeff_table_right, *coeff_table_right,
                 g_pow_zeta_q1[DIM_R / 2 - half_exponent_of_zeta - 1]); //
      coeff_table_left++;
      coeff_table_right++;
    }
}

void
conv_NTT_polyq1_to_R_polyq1 (polyq1_t poly)
{
  _reverse_nttq1_recursion (*poly, DIM_R, THETA);
  intq1_t g_inverse_theta_q1;
  uint64_t u64_x;
  u64_x = 1197390852735ul;
  conv_uint64_to_intq1 (g_inverse_theta_q1, u64_x);
  for (int i = 0; i < DIM_R; i++)
    {
      intq1_mul ((*poly)[i], (*poly)[i], g_inverse_theta_q1);
    }
}


void
_reverse_nttQ_recursion (intQ_t *coeff_table, int length, int exponent_of_zeta)
{
  if (length == (DIM_R / THETA))
    return;
  int half_length = length / 2;
  int half_exponent_of_zeta = exponent_of_zeta / 2;
  _reverse_nttQ_recursion (coeff_table, half_length, half_exponent_of_zeta);
  _reverse_nttQ_recursion (coeff_table + half_length, half_length,
                           half_exponent_of_zeta + THETA);
  intQ_t *coeff_table_left = coeff_table;
  intQ_t *coeff_table_right = coeff_table + half_length;
  intQ_t left_coeff_temp;

  for (int i = 0; i < half_length; i++)
    {
      intQ_copy (left_coeff_temp, *coeff_table_left);
      intQ_add (*coeff_table_left, left_coeff_temp, *coeff_table_right);
      intQ_sub (*coeff_table_right, left_coeff_temp, *coeff_table_right);
      intQ_mul (*coeff_table_right, *coeff_table_right,
                g_pow_zeta_Q[DIM_R / 2 - half_exponent_of_zeta - 1]);
      coeff_table_left++;
      coeff_table_right++;
    }
}
void
conv_NTT_polyQ_to_R_polyQ (polyQ_t poly)
{
  _reverse_nttQ_recursion (*poly, DIM_R, THETA);
  intQ_t g_inverse_theta_Q;

  uint64_t p_u64_x[2];
  p_u64_x[0] = 610546933377629547ul;
  p_u64_x[1] = 2244277;
  conv_p_uint64_to_intQ (g_inverse_theta_Q, p_u64_x);
  for (int i = 0; i < DIM_R; i++)
    {
      intQ_mul ((*poly)[i], (*poly)[i], g_inverse_theta_Q);
    }
}

inline void 
conv_NTT_polyq1_to_R_polyq1_V (polyq1_t *poly_V, int size) 
{for (int i = 0; i<size; i++){conv_NTT_polyq1_to_R_polyq1 (poly_V[i]); }};

inline void 
conv_NTT_polyq2_to_R_polyq2_V (polyq2_t *poly_V, int size)
{for (int i = 0; i<size; i++){conv_NTT_polyq2_to_R_polyq2 (poly_V[i]); }};

inline void 
conv_NTT_polyQ_to_R_polyQ_V (polyQ_t *poly_V, int size)
{for (int i = 0; i<size; i++){conv_NTT_polyQ_to_R_polyQ (poly_V[i]); }};

inline void
conv_NTT_polyq1_to_NTT_polyq2(polyq2_t NTT_res, polyq1_t NTT_in)
{
  polyq1_t R_in;
  polyq1_init(R_in);
  polyq1_copy(R_in, NTT_in);
  conv_NTT_polyq1_to_R_polyq1(R_in);
  conv_polyq1_to_polyq2(NTT_res, R_in);
  conv_R_polyq2_to_NTT_polyq2(NTT_res);
  polyq1_clear(R_in);
}

inline void
conv_NTT_polyq1_to_NTT_polyQ(polyQ_t NTT_res, polyq1_t NTT_in)
{
  polyq1_t R_in;
  polyq1_init(R_in);
  polyq1_copy(R_in, NTT_in);
  conv_NTT_polyq1_to_R_polyq1(R_in);
  conv_polyq1_to_polyQ(NTT_res, R_in);
  conv_R_polyQ_to_NTT_polyQ(NTT_res);
  polyq1_clear(R_in);
}

/* NORM, DOT PRODUCT AND REJECTION SAMPLING ALGORITHMS */
void dot_product_R_polyq1 (mpfr_t scalar_res, polyq1_t polyq1_x, polyq1_t polyq1_y)
{
  intq1_t cfx;
  intq1_t cfy;

  mpfr_t mpfr_q1;
  mpfr_t mpfr_hq1;
  mpfr_t mpfr_cfx;
  mpfr_t mpfr_cfy;

  mpfr_init2 (mpfr_q1, FPA_PREC);
  mpfr_init2 (mpfr_hq1, FPA_PREC);
  mpfr_init2 (mpfr_cfx, FPA_PREC);
  mpfr_init2 (mpfr_cfy, FPA_PREC);

  mpfr_set_ui (scalar_res, 0, MPFR_RNDN);

  mpfr_set_ui (mpfr_q1, g_intq1_q1[0], MPFR_RNDN);
  mpfr_div_ui (mpfr_hq1, mpfr_q1, 2, MPFR_RNDN);

  for (int i = 0; i < DIM_R; i++)
  {
    intq1_copy(cfx, COEFF(polyq1_x,i));
    intq1_copy(cfy, COEFF(polyq1_y,i));
    intq1_canonical_form(cfx);
    intq1_canonical_form(cfy);
    mpfr_set_ui(mpfr_cfx, cfx[0], MPFR_RNDN);
    mpfr_set_ui(mpfr_cfy, cfy[0], MPFR_RNDN);
    if (mpfr_cmp(mpfr_cfx, mpfr_hq1) > 0)
    {
      mpfr_sub(mpfr_cfx, mpfr_cfx, mpfr_q1, MPFR_RNDN);
    }
    if (mpfr_cmp(mpfr_cfy, mpfr_hq1) > 0)
    {
      mpfr_sub(mpfr_cfy, mpfr_cfy, mpfr_q1, MPFR_RNDN);
    }

    mpfr_mul(mpfr_cfx, mpfr_cfx, mpfr_cfy, MPFR_RNDN);
    mpfr_add(scalar_res, scalar_res, mpfr_cfx, MPFR_RNDN);
  }

  mpfr_clear (mpfr_q1);
  mpfr_clear (mpfr_hq1);
  mpfr_clear (mpfr_cfx);
  mpfr_clear (mpfr_cfy);
}

void dot_product_R_polyq2 (mpfr_t scalar_res, polyq2_t polyq2_x, polyq2_t polyq2_y)
{
  intq2_t cfx;
  intq2_t cfy;

  mpfr_t mpfr_q2;
  mpfr_t mpfr_hq2;
  mpfr_t mpfr_cfx;
  mpfr_t mpfr_cfy;

  mpfr_init2 (mpfr_q2, FPA_PREC);
  mpfr_init2 (mpfr_hq2, FPA_PREC);
  mpfr_init2 (mpfr_cfx, FPA_PREC);
  mpfr_init2 (mpfr_cfy, FPA_PREC);

  mpfr_set_ui (scalar_res, 0, MPFR_RNDN);

  mpfr_set_ui_2exp (mpfr_q2, g_intq2_q2[2], 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_q2, mpfr_q2, g_intq2_q2[1], MPFR_RNDN);
  mpfr_mul_2exp (mpfr_q2, mpfr_q2, 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_q2, mpfr_q2, g_intq2_q2[0], MPFR_RNDN);
  mpfr_div_ui (mpfr_hq2, mpfr_q2, 2, MPFR_RNDN);

  for (int i = 0; i < DIM_R; i++)
  {
    intq2_copy(cfx, COEFF(polyq2_x,i));
    intq2_copy(cfy, COEFF(polyq2_y,i));
    intq2_canonical_form(cfx);
    intq2_canonical_form(cfy);

    mpfr_set_ui_2exp (mpfr_cfx, cfx[2], 64, MPFR_RNDN);
    mpfr_add_ui (mpfr_cfx, mpfr_cfx, cfx[1], MPFR_RNDN);
    mpfr_mul_2exp (mpfr_cfx, mpfr_cfx, 64, MPFR_RNDN);
    mpfr_add_ui (mpfr_cfx, mpfr_cfx, cfx[0], MPFR_RNDN);

    mpfr_set_ui_2exp (mpfr_cfy, cfy[2], 64, MPFR_RNDN);
    mpfr_add_ui (mpfr_cfy, mpfr_cfy, cfy[1], MPFR_RNDN);
    mpfr_mul_2exp (mpfr_cfy, mpfr_cfy, 64, MPFR_RNDN);
    mpfr_add_ui (mpfr_cfy, mpfr_cfy, cfy[0], MPFR_RNDN);

    if (mpfr_cmp(mpfr_cfx, mpfr_hq2) > 0)
    {
      mpfr_sub(mpfr_cfx, mpfr_cfx, mpfr_q2, MPFR_RNDN);
    }
    if (mpfr_cmp(mpfr_cfy, mpfr_hq2) > 0)
    {
      mpfr_sub(mpfr_cfy, mpfr_cfy, mpfr_q2, MPFR_RNDN);
    }

    mpfr_mul(mpfr_cfx, mpfr_cfx, mpfr_cfy, MPFR_RNDN);
    mpfr_add(scalar_res, scalar_res, mpfr_cfx, MPFR_RNDN);
  }

  mpfr_clear (mpfr_q2);
  mpfr_clear (mpfr_hq2);
  mpfr_clear (mpfr_cfx);
  mpfr_clear (mpfr_cfy);
}

void dot_product_R_polyQ (mpfr_t scalar_res, polyQ_t polyQ_x, polyQ_t polyQ_y)
{
  intQ_t cfx;
  intQ_t cfy;

  mpfr_t mpfr_Q;
  mpfr_t mpfr_hQ;
  mpfr_t mpfr_cfx;
  mpfr_t mpfr_cfy;

  mpfr_init2 (mpfr_Q, FPA_PREC);
  mpfr_init2 (mpfr_hQ, FPA_PREC);
  mpfr_init2 (mpfr_cfx, FPA_PREC);
  mpfr_init2 (mpfr_cfy, FPA_PREC);

  mpfr_set_ui (scalar_res, 0, MPFR_RNDN);

  mpfr_set_ui_2exp (mpfr_Q, g_intQ_Q[1], 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_Q, mpfr_Q, g_intQ_Q[0], MPFR_RNDN);
  mpfr_div_ui (mpfr_hQ, mpfr_Q, 2, MPFR_RNDN);

  for (int i = 0; i < DIM_R; i++)
  {
    intQ_copy(cfx, COEFF(polyQ_x,i));
    intQ_copy(cfy, COEFF(polyQ_y,i));
    intQ_canonical_form(cfx);
    intQ_canonical_form(cfy);

    mpfr_set_ui_2exp (mpfr_cfx, cfx[1], 64, MPFR_RNDN);
    mpfr_add_ui (mpfr_cfx, mpfr_cfx, cfx[0], MPFR_RNDN);

    mpfr_set_ui_2exp (mpfr_cfy, cfy[1], 64, MPFR_RNDN);
    mpfr_add_ui (mpfr_cfy, mpfr_cfy, cfy[0], MPFR_RNDN);

    if (mpfr_cmp(mpfr_cfx, mpfr_hQ) > 0)
    {
      mpfr_sub(mpfr_cfx, mpfr_cfx, mpfr_Q, MPFR_RNDN);
    }
    if (mpfr_cmp(mpfr_cfy, mpfr_hQ) > 0)
    {
      mpfr_sub(mpfr_cfy, mpfr_cfy, mpfr_Q, MPFR_RNDN);
    }

    mpfr_mul(mpfr_cfx, mpfr_cfx, mpfr_cfy, MPFR_RNDN);
    mpfr_add(scalar_res, scalar_res, mpfr_cfx, MPFR_RNDN);
  }

  mpfr_clear (mpfr_Q);
  mpfr_clear (mpfr_hQ);
  mpfr_clear (mpfr_cfx);
  mpfr_clear (mpfr_cfy);
}

void euclidean_sqrnorm_R_polyq1 (mpfr_t norm, polyq1_t polyq1_pol)
{
  intq1_t intq1_cf;

  mpfr_t mpfr_q1;
  mpfr_t mpfr_hq1;
  mpfr_t coef_pol;

  mpfr_init2 (mpfr_q1, FPA_PREC);
  mpfr_init2 (mpfr_hq1, FPA_PREC);
  mpfr_init2 (coef_pol, FPA_PREC);

  mpfr_set_ui (norm, 0, MPFR_RNDN);

  mpfr_set_ui (mpfr_q1, g_intq1_q1[0], MPFR_RNDN);
  mpfr_div_ui (mpfr_hq1, mpfr_q1, 2, MPFR_RNDN);

  for (int i = 0; i < DIM_R; i++)
  {
    intq1_copy(intq1_cf, COEFF(polyq1_pol,i));
    intq1_canonical_form(intq1_cf);
    mpfr_set_ui(coef_pol, intq1_cf[0], MPFR_RNDN);
    if (mpfr_cmp(coef_pol, mpfr_hq1) > 0)
    {
      mpfr_sub(coef_pol, coef_pol, mpfr_q1, MPFR_RNDN);
    }
    mpfr_sqr(coef_pol, coef_pol, MPFR_RNDN);
    mpfr_add(norm, norm, coef_pol, MPFR_RNDN);
  }

  mpfr_clear (mpfr_q1);
  mpfr_clear (mpfr_hq1);
  mpfr_clear (coef_pol);
}

void euclidean_sqrnorm_R_polyq2 (mpfr_t norm, polyq2_t polyq2_pol)
{
  intq2_t intq2_cf;

  mpfr_t mpfr_q2;
  mpfr_t mpfr_hq2;
  mpfr_t coef_pol;

  mpfr_init2 (mpfr_q2, FPA_PREC);
  mpfr_init2 (mpfr_hq2, FPA_PREC);
  mpfr_init2 (coef_pol, FPA_PREC);

  mpfr_set_ui (norm, 0, MPFR_RNDN);

  mpfr_set_ui_2exp (mpfr_q2, g_intq2_q2[2], 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_q2, mpfr_q2, g_intq2_q2[1], MPFR_RNDN);
  mpfr_mul_2exp (mpfr_q2, mpfr_q2, 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_q2, mpfr_q2, g_intq2_q2[0], MPFR_RNDN);
  mpfr_div_ui (mpfr_hq2, mpfr_q2, 2, MPFR_RNDN);

  for (int i = 0; i < DIM_R; i++)
  {
    intq2_copy(intq2_cf, COEFF(polyq2_pol,i));
    intq2_canonical_form(intq2_cf);
    mpfr_set_ui_2exp (coef_pol, intq2_cf[2], 64, MPFR_RNDN);
    mpfr_add_ui (coef_pol, coef_pol, intq2_cf[1], MPFR_RNDN);
    mpfr_mul_2exp (coef_pol, coef_pol, 64, MPFR_RNDN);
    mpfr_add_ui (coef_pol, coef_pol, intq2_cf[0], MPFR_RNDN);
    if (mpfr_cmp(coef_pol, mpfr_hq2) > 0)
    {
      mpfr_sub(coef_pol, coef_pol, mpfr_q2, MPFR_RNDN);
    }
    mpfr_sqr(coef_pol, coef_pol, MPFR_RNDN);
    mpfr_add(norm, norm, coef_pol, MPFR_RNDN);
  }

  mpfr_clear (mpfr_q2);
  mpfr_clear (mpfr_hq2);
  mpfr_clear (coef_pol);
}

void euclidean_sqrnorm_R_polyQ (mpfr_t norm, polyQ_t polyQ_pol)
{
  intQ_t intQ_cf;

  mpfr_t mpfr_Q;
  mpfr_t mpfr_hQ;
  mpfr_t coef_pol;

  mpfr_init2 (mpfr_Q, FPA_PREC);
  mpfr_init2 (mpfr_hQ, FPA_PREC);
  mpfr_init2 (coef_pol, FPA_PREC);

  mpfr_set_ui (norm, 0, MPFR_RNDN);

  mpfr_set_ui_2exp (mpfr_Q, g_intQ_Q[1], 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_Q, mpfr_Q, g_intQ_Q[0], MPFR_RNDN);
  mpfr_div_ui (mpfr_hQ, mpfr_Q, 2, MPFR_RNDN);

  for (int i = 0; i < DIM_R; i++)
    {
      intQ_copy(intQ_cf, COEFF(polyQ_pol,i));
      intQ_canonical_form(intQ_cf);
      mpfr_set_ui_2exp (coef_pol, intQ_cf[1], 64, MPFR_RNDN);
      mpfr_add_ui (coef_pol, coef_pol, intQ_cf[0], MPFR_RNDN);
      if (mpfr_cmp(coef_pol, mpfr_hQ) > 0)
      {
        mpfr_sub(coef_pol, coef_pol, mpfr_Q, MPFR_RNDN);
      }
      mpfr_sqr(coef_pol, coef_pol, MPFR_RNDN);
      mpfr_add(norm, norm, coef_pol, MPFR_RNDN);
    }

  mpfr_clear (mpfr_Q);
  mpfr_clear (mpfr_hQ);
  mpfr_clear (coef_pol);
}

void euclidean_norm_R_polyq1 (mpfr_t norm, polyq1_t polyq1_pol)
{
  euclidean_sqrnorm_R_polyq1 (norm, polyq1_pol);
  mpfr_sqrt(norm, norm, MPFR_RNDN);
}

void euclidean_norm_R_polyq2 (mpfr_t norm, polyq2_t polyq2_pol)
{
  euclidean_sqrnorm_R_polyq2 (norm, polyq2_pol);
  mpfr_sqrt(norm, norm, MPFR_RNDN);
}

void euclidean_norm_R_polyQ (mpfr_t norm, polyQ_t polyQ_pol)
{
  euclidean_sqrnorm_R_polyQ (norm, polyQ_pol);
  mpfr_sqrt(norm, norm, MPFR_RNDN);
}

static inline void
sub_rejection_sampling_xi1 (mpfr_t p, polyq2_t *polyq2_z_V4, polyq2_t *polyq2_b_V4)
{
  mpfr_t t;
  mpfr_t xi1;

  mpfr_init2(t, FPA_PREC);
  mpfr_init2(xi1, FPA_PREC);

  mpfr_set_ui_2exp(xi1, g_intq2_xi1[1], 64, MPFR_RNDN);
  mpfr_add_ui(xi1, xi1, g_intq2_xi1[0], MPFR_RNDN);

  //p = 1/3 * exp((-2<z,b>+||b||^2)/2σ^2)
  dot_product_R_polyq2 (p, polyq2_z_V4[0], polyq2_b_V4[0]);
  dot_product_R_polyq2 (t, polyq2_z_V4[1], polyq2_b_V4[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq2 (t, polyq2_z_V4[2], polyq2_b_V4[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq2 (t, polyq2_z_V4[3], polyq2_b_V4[3]);
  mpfr_add(p, p, t, MPFR_RNDN);
  mpfr_mul_si (p, p, -2, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq2 (t, polyq2_b_V4[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq2 (t, polyq2_b_V4[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq2 (t, polyq2_b_V4[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq2 (t, polyq2_b_V4[3]);
  mpfr_add(p, p, t, MPFR_RNDN);
  mpfr_sqr(t, xi1, MPFR_RNDN);
  mpfr_mul_ui (t, t, 2, MPFR_RNDN);
  mpfr_div(p, p, t, MPFR_RNDN);
  mpfr_exp(p, p, MPFR_RNDN);
  mpfr_div_ui (p, p, 3, MPFR_RNDN);

  mpfr_clear(t);
  mpfr_clear(xi1);
}

static inline void
sub_rejection_sampling_xi2 (mpfr_t p, polyq2_t *polyq2_z_V2, polyq2_t *polyq2_b_V2)
{
  mpfr_t t;
  mpfr_t xi2;

  mpfr_init2(t, FPA_PREC);
  mpfr_init2(xi2, FPA_PREC);

  mpfr_set_ui_2exp(xi2, g_intq2_xi2[1], 64, MPFR_RNDN);
  mpfr_add_ui(xi2, xi2, g_intq2_xi2[0], MPFR_RNDN);

  //p = 1/3 * exp((-2<z,b>+||b||^2)/2σ^2)
  dot_product_R_polyq2 (p, polyq2_z_V2[0], polyq2_b_V2[0]);
  dot_product_R_polyq2 (t, polyq2_z_V2[1], polyq2_b_V2[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  mpfr_mul_si (p, p, -2, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq2 (t, polyq2_b_V2[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq2 (t, polyq2_b_V2[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  mpfr_sqr(t, xi2, MPFR_RNDN);
  mpfr_mul_ui (t, t, 2, MPFR_RNDN);
  mpfr_div(p, p, t, MPFR_RNDN);
  mpfr_exp(p, p, MPFR_RNDN);
  mpfr_div_ui (p, p, 3, MPFR_RNDN);

  mpfr_clear(t);
  mpfr_clear(xi2);
}

static inline void
sub_rejection_sampling_xi (mpfr_t p, polyq1_t *polyq1_z1_V3, polyq1_t *polyq1_z2_V3,
                           polyq1_t *polyq1_z3_V3, polyq1_t *polyq1_z4_V3,
                           polyQ_t *polyQ_z5_V5, polyq1_t *polyq1_b1_V3,
                           polyq1_t *polyq1_b2_V3, polyq1_t *polyq1_b3_V3,
                           polyq1_t *polyq1_b4_V3, polyQ_t *polyQ_b5_V5)
{
  mpfr_t t;
  mpfr_t xi;

  mpfr_init2(t, FPA_PREC);
  mpfr_init2(xi, FPA_PREC);

  mpfr_set_ui (xi, g_intq1_xi[0], MPFR_RNDN);

  //p = 1/3 * exp((-2<z,b>+||b||^2)/2σ^2)
  dot_product_R_polyq1 (p, polyq1_z1_V3[0], polyq1_b1_V3[0]);
  dot_product_R_polyq1 (t, polyq1_z1_V3[1], polyq1_b1_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z1_V3[2], polyq1_b1_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z2_V3[0], polyq1_b2_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z2_V3[1], polyq1_b2_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z2_V3[2], polyq1_b2_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z3_V3[0], polyq1_b3_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN); 
  dot_product_R_polyq1 (t, polyq1_z3_V3[1], polyq1_b3_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z3_V3[2], polyq1_b3_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z4_V3[0], polyq1_b4_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z4_V3[1], polyq1_b4_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyq1 (t, polyq1_z4_V3[2], polyq1_b4_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyQ (t, polyQ_z5_V5[0], polyQ_b5_V5[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyQ (t, polyQ_z5_V5[1], polyQ_b5_V5[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyQ (t, polyQ_z5_V5[2], polyQ_b5_V5[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyQ (t, polyQ_z5_V5[3], polyQ_b5_V5[3]);
  mpfr_add(p, p, t, MPFR_RNDN);
  dot_product_R_polyQ (t, polyQ_z5_V5[4], polyQ_b5_V5[4]);
  mpfr_add(p, p, t, MPFR_RNDN);
  mpfr_mul_si (p, p, -2, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b1_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b1_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b1_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b2_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b2_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b2_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b3_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b3_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b3_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b4_V3[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b4_V3[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyq1 (t, polyq1_b4_V3[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyQ (t, polyQ_b5_V5[0]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyQ (t, polyQ_b5_V5[1]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyQ (t, polyQ_b5_V5[2]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyQ (t, polyQ_b5_V5[3]);
  mpfr_add(p, p, t, MPFR_RNDN);
  euclidean_sqrnorm_R_polyQ (t, polyQ_b5_V5[4]);
  mpfr_add(p, p, t, MPFR_RNDN);
  mpfr_sqr(t, xi, MPFR_RNDN);
  mpfr_mul_ui (t, t, 2, MPFR_RNDN);
  mpfr_div(p, p, t, MPFR_RNDN);
  mpfr_exp(p, p, MPFR_RNDN);
  mpfr_div_ui (p, p, 3, MPFR_RNDN);

  mpfr_clear(t);
  mpfr_clear(xi);
}

int rejection_sampling (polyq1_t *polyq1_z1_V3, polyq1_t *polyq1_z2_V3,
                        polyq1_t *polyq1_z3_V3, polyq1_t *polyq1_z4_V3,
                        polyQ_t *polyQ_z5_V5, polyq1_t *polyq1_b1_V3,
                        polyq1_t *polyq1_b2_V3, polyq1_t *polyq1_b3_V3,
                        polyq1_t *polyq1_b4_V3, polyQ_t *polyQ_b5_V5,
                        polyq2_t *polyq2_z_V4, polyq2_t *polyq2_b_V4,
                        polyq2_t *polyq2_z_V2, polyq2_t *polyq2_b_V2)
{
  mpfr_t u;
  mpfr_t p_xi;
  mpfr_t p_xi1;
  mpfr_t p_xi2;
  uint8_t uni[(FPA_PREC+7)/8];

  mpfr_init2(u, FPA_PREC);
  mpfr_init2(p_xi, FPA_PREC);
  mpfr_init2(p_xi1, FPA_PREC);
  mpfr_init2(p_xi2, FPA_PREC);

  get_random_bytes (uni, (FPA_PREC+7)/8);
  mpfr_set_ui (u, 0, MPFR_RNDN);
  for(int i=0; i<(FPA_PREC+7)/8; i++)
  {
    mpfr_add_ui(u, u, uni[i], MPFR_RNDN);
    mpfr_div_2exp(u, u, 8, MPFR_RNDN);
  }

  sub_rejection_sampling_xi (p_xi, polyq1_z1_V3, polyq1_z2_V3,
                             polyq1_z3_V3, polyq1_z4_V3,
                             polyQ_z5_V5, polyq1_b1_V3,
                             polyq1_b2_V3, polyq1_b3_V3,
                             polyq1_b4_V3, polyQ_b5_V5);
  sub_rejection_sampling_xi1 (p_xi1, polyq2_z_V4, polyq2_b_V4);
  sub_rejection_sampling_xi2 (p_xi2, polyq2_z_V2, polyq2_b_V2);

  if (mpfr_cmp(u,p_xi) > 0 || mpfr_cmp(u,p_xi1) > 0 || mpfr_cmp(u,p_xi2) > 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
