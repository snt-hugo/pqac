#include <gmp.h>
#include <time.h>

#include "dgs_gauss.h"
#include "extra_tools.h"
#include "rng.h"
#include "samplers.h"

#define AES_RNG

#define GAUSSIAN_METHOD_XI DGS_DISC_GAUSS_SIGMA2_LOGTABLE
#define GAUSSIAN_METHOD_XI1 DGS_DISC_GAUSS_SIGMA2_LOGTABLE
#define GAUSSIAN_METHOD_XI2 DGS_DISC_GAUSS_SIGMA2_LOGTABLE
#define GAUSSIAN_METHOD_S DGS_DISC_GAUSS_SIGMA2_LOGTABLE
#define GAUSSIAN_METHOD_R DGS_DISC_GAUSS_SIGMA2_LOGTABLE

gmp_randstate_t gmp_randstate;
dgs_disc_gauss_mp_t *gaussian_sampler_xi;
dgs_disc_gauss_mp_t *gaussian_sampler_xi1;
dgs_disc_gauss_mp_t *gaussian_sampler_xi2;
dgs_disc_gauss_mp_t *gaussian_sampler_s;
dgs_disc_gauss_mp_t *gaussian_sampler_r;

int
rand_init ()
{
  //randombytes_init((unsigned char) time (NULL), NULL, 256);
  /* Standard deviations. */
  mpfr_t mpfr_sigma_xi_std_dev;
  mpfr_t mpfr_sigma_xi1_std_dev;
  mpfr_t mpfr_sigma_xi2_std_dev;
  mpfr_t mpfr_sigma_r_std_dev;
  mpfr_t mpfr_sigma_s_std_dev;
  mpfr_t mpfr_zero_center_gaussian;
  
  mpfr_init (mpfr_sigma_xi_std_dev);
  mpfr_init (mpfr_sigma_xi1_std_dev);
  mpfr_init (mpfr_sigma_xi2_std_dev);
  mpfr_init (mpfr_sigma_r_std_dev);
  mpfr_init (mpfr_sigma_s_std_dev);
  mpfr_init (mpfr_zero_center_gaussian);
  
  mpfr_init_set_ui (mpfr_zero_center_gaussian, (unsigned long)0, MPFR_RNDN);
  mpfr_init_set_ui (mpfr_sigma_xi_std_dev, g_intq1_xi[0], MPFR_RNDN);
  
  /* 2^64 * ul_array_X_std_dev[1] + ul_array_X_std_dev[0]. */
  mpfr_set_ui_2exp (mpfr_sigma_xi1_std_dev, g_intq2_xi1[1], 64,
                    MPFR_RNDN);
  mpfr_add_ui (mpfr_sigma_xi1_std_dev, mpfr_sigma_xi1_std_dev,
               g_intq2_xi1[0], MPFR_RNDN);
  mpfr_set_ui_2exp (mpfr_sigma_xi2_std_dev, g_intq2_xi2[1], 64,
                    MPFR_RNDN);
  mpfr_add_ui (mpfr_sigma_xi2_std_dev, mpfr_sigma_xi2_std_dev,
               g_intq2_xi2[0], MPFR_RNDN);
  mpfr_set_ui_2exp (mpfr_sigma_s_std_dev, g_intq2_ssd[1], 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_sigma_s_std_dev, mpfr_sigma_s_std_dev,
               g_intq2_ssd[0], MPFR_RNDN);

  mpfr_set_ui_2exp (mpfr_sigma_r_std_dev, g_intq2_rsd[1], 64, MPFR_RNDN);
  mpfr_add_ui (mpfr_sigma_r_std_dev, mpfr_sigma_r_std_dev,
               g_intq2_rsd[0], MPFR_RNDN);

  gmp_randinit_default (gmp_randstate);
  gmp_randseed_ui (gmp_randstate, time (NULL));
  
  /* Temporary solution. */
  srand (time (NULL));
  gaussian_sampler_xi = dgs_disc_gauss_mp_init (
      mpfr_sigma_xi_std_dev, mpfr_zero_center_gaussian, TAU_XI_STD_DEV,
      GAUSSIAN_METHOD_XI);
  gaussian_sampler_xi1 = dgs_disc_gauss_mp_init (
      mpfr_sigma_xi1_std_dev, mpfr_zero_center_gaussian, TAU_XI1_STD_DEV,
      GAUSSIAN_METHOD_XI1);
  gaussian_sampler_xi2 = dgs_disc_gauss_mp_init (
      mpfr_sigma_xi2_std_dev, mpfr_zero_center_gaussian, TAU_XI2_STD_DEV,
      GAUSSIAN_METHOD_XI2);
  gaussian_sampler_s = dgs_disc_gauss_mp_init (
      mpfr_sigma_s_std_dev, mpfr_zero_center_gaussian, TAU_S_STD_DEV,
      GAUSSIAN_METHOD_S);
  gaussian_sampler_r = dgs_disc_gauss_mp_init (
      mpfr_sigma_r_std_dev, mpfr_zero_center_gaussian, TAU_R_STD_DEV,
      GAUSSIAN_METHOD_R);
  return EXIT_SUCCESS;
}

int
rand_clear ()
{
  gmp_randclear (gmp_randstate);
  return EXIT_SUCCESS;
}

int
get_random_bit (uint8_t *out)
{
  /* Temporary solution. */
  *out = (((uint8_t)(rand () % 2)));
  return EXIT_SUCCESS;
}

#ifdef AES_RNG
int
get_random_bytes (void *out, const size_t len)
{
  if (randombytes(out,len) == RNG_SUCCESS) {
    /*for (int i = 0; i<len;i++) {
      printf("%x",(((uint8_t*) out)[i]));
    }
    printf("\n");*/
    return EXIT_SUCCESS;
  }
  else {
    return EXIT_FAILURE;
  }
}

#else
int
get_random_bytes (void *out, const size_t len)
{
  for (size_t i = 0; i < len; i++)
    {
      ((uint8_t *)out)[i] = (uint8_t)(rand () % 256);
    }
  return EXIT_SUCCESS;
}
#endif

int
get_random_intq1 (intq1_t intq1_out)
{
  uint64_t ui_out = 0;
  get_random_bytes ((void *)&ui_out, 8);
  conv_uint64_to_intq1(intq1_out, ui_out);
  return EXIT_SUCCESS;
}

int
get_random_intq2 (intq2_t intq2_out)
{
  uint64_t ui_out_V3[3] = { 0, 0, 0 };
  get_random_bytes (ui_out_V3, 24);
  conv_p_uint64_to_intq2(intq2_out, ui_out_V3);
  return EXIT_SUCCESS;
}

int
get_random_intQ (intQ_t intQ_out)
{
  uint64_t ui_out_V2[2] = { 0, 0 };
  get_random_bytes (ui_out_V2, 16);
  conv_p_uint64_to_intQ(intQ_out, ui_out_V2);
  return EXIT_SUCCESS;
}

int
get_random_intp (intp_t intp_out)
{
  uint64_t ui_out = 0;
  get_random_bytes ((void *)&ui_out, 8);
  conv_uint64_to_intp(intp_out, ui_out);
  return EXIT_SUCCESS;
}

int get_random_polyq1 (polyq1_t polyq1_res)
{ for (int i = 0; i < DIM_R; i++) { get_random_intq1 (COEFF(polyq1_res,i)); } return EXIT_SUCCESS; }

int get_random_polyq2 (polyq2_t polyq2_res)
{ for (int i = 0; i < DIM_R; i++) { get_random_intq2 (COEFF(polyq2_res,i)); } return EXIT_SUCCESS; }

int get_random_polyQ (polyQ_t polyQ_res)
{ for (int i = 0; i < DIM_R; i++) { get_random_intQ (COEFF(polyQ_res,i)); } return EXIT_SUCCESS; }

int get_random_polyp (polyp_t polyp_res)
{ for (int i = 0; i < DIM_R; i++) { get_random_intp (COEFF(polyp_res,i)); } return EXIT_SUCCESS; }

inline int
get_random_challenge (challenge_t chall_res)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
        {
        /* Probability with rejection. We can try it four times because we have eight bits. */
        get_random_bytes ((void *)&ui_out, 8);
        if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
          {
            intq1_set_ui (COEFF(chall_res, i), ((unsigned long)(ui_out & 0b00000011)));
            intq1_sub_ui (COEFF(chall_res, i), COEFF(chall_res, i), (unsigned long)1);
            intq1_canonical_form(COEFF(chall_res,i));
            break;
          }
        else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
          {
            intq1_set_ui (COEFF(chall_res,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
            intq1_sub_ui (COEFF(chall_res, i), COEFF(chall_res, i), (unsigned long)1);
            intq1_canonical_form(COEFF(chall_res,i));
            break;
          }
        else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
          {
            intq1_set_ui (COEFF(chall_res,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
            intq1_sub_ui (COEFF(chall_res, i), COEFF(chall_res, i), (unsigned long)1);
            intq1_canonical_form(COEFF(chall_res,i));
            break;
          }
        else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
          {
            intq1_set_ui (COEFF(chall_res,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
            intq1_sub_ui (COEFF(chall_res, i), COEFF(chall_res, i), (unsigned long)1);
            intq1_canonical_form(COEFF(chall_res,i));
            break;
          }
        }
    }
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_NTTq1 (polyq1_t NTTq1_chall)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
         {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              intq1_set_ui (COEFF(NTTq1_chall, i), ((unsigned long)(ui_out & 0b00000011)));
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              intq1_set_ui (COEFF(NTTq1_chall,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              intq1_set_ui (COEFF(NTTq1_chall,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              intq1_set_ui (COEFF(NTTq1_chall,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall,i));
              break;
            }
         }
    }
  conv_R_polyq1_to_NTT_polyq1(NTTq1_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_NTTq2 (polyq2_t NTTq2_chall)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
         {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              intq2_set_ui (COEFF(NTTq2_chall, i), ((unsigned long)(ui_out & 0b00000011)));
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              intq2_set_ui (COEFF(NTTq2_chall,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              intq2_set_ui (COEFF(NTTq2_chall,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              intq2_set_ui (COEFF(NTTq2_chall,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
         }
    }
  conv_R_polyq2_to_NTT_polyq2(NTTq2_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_NTTQ (polyQ_t NTTQ_chall)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
         {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              intQ_set_ui (COEFF(NTTQ_chall, i), ((unsigned long)(ui_out & 0b00000011)));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              intQ_set_ui (COEFF(NTTQ_chall,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              intQ_set_ui (COEFF(NTTQ_chall,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              intQ_set_ui (COEFF(NTTQ_chall,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
         }
    }
  conv_R_polyQ_to_NTT_polyQ(NTTQ_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_chall_NTTq1 (challenge_t chall, polyq1_t NTTq1_chall)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
        {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              intq1_set_ui (COEFF(chall, i), ((unsigned long)(ui_out & 0b00000011)));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              intq1_set_ui (COEFF(chall,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              intq1_set_ui (COEFF(chall,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              intq1_set_ui (COEFF(chall,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              break;
            }
        }
    }
  polyq1_copy(NTTq1_chall, chall);
  conv_R_polyq1_to_NTT_polyq1(NTTq1_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_chall_NTTq1_NTTQ (challenge_t chall, polyq1_t NTTq1_chall, polyQ_t NTTQ_chall)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
        {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              intQ_set_ui (COEFF(NTTQ_chall, i), ((unsigned long)(ui_out & 0b00000011)));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall, i), ((unsigned long)(ui_out & 0b00000011)));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              intQ_set_ui (COEFF(NTTQ_chall,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), (((unsigned long)(ui_out & 0b00001100)) >> 2));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              intQ_set_ui (COEFF(NTTQ_chall,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), (((unsigned long)(ui_out & 0b00110000)) >> 4));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              intQ_set_ui (COEFF(NTTQ_chall,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), (((unsigned long)(ui_out & 0b11000000)) >> 6));
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
        }
    }
  polyq1_copy(NTTq1_chall, chall);
  conv_R_polyq1_to_NTT_polyq1(NTTq1_chall);
  conv_R_polyQ_to_NTT_polyQ(NTTQ_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_chall_NTTq1_NTTq2 (challenge_t chall, polyq1_t NTTq1_chall, polyq2_t NTTq2_chall)
{
  unsigned long tmp = 0;
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
        {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              tmp = (unsigned long)(ui_out & 0b00000011);
              intq2_set_ui (COEFF(NTTq2_chall, i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall, i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              tmp = ((unsigned long)(ui_out & 0b00001100)) >> 2;
              intq2_set_ui (COEFF(NTTq2_chall,i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              tmp = (((unsigned long)(ui_out & 0b00110000)) >> 4);
              intq2_set_ui (COEFF(NTTq2_chall,i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              tmp = (((unsigned long)(ui_out & 0b11000000)) >> 6);
              intq2_set_ui (COEFF(NTTq2_chall,i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              break;
            }
        }
    }
  polyq1_copy(NTTq1_chall, chall);
  conv_R_polyq2_to_NTT_polyq2(NTTq2_chall);
  conv_R_polyq1_to_NTT_polyq1(NTTq1_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_chall_NTTq1_NTTq2_NTTQ (challenge_t chall, polyq1_t NTTq1_chall, polyq2_t NTTq2_chall, polyQ_t NTTQ_chall)
{
  unsigned long tmp = 0;
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
        {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              tmp = (unsigned long)(ui_out & 0b00000011);
              intQ_set_ui (COEFF(NTTQ_chall, i), tmp);
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq2_set_ui (COEFF(NTTq2_chall, i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall, i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              tmp = ((unsigned long)(ui_out & 0b00001100)) >> 2;
              intQ_set_ui (COEFF(NTTQ_chall,i), tmp);
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq2_set_ui (COEFF(NTTq2_chall,i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              tmp = (((unsigned long)(ui_out & 0b00110000)) >> 4);
              intQ_set_ui (COEFF(NTTQ_chall,i), tmp);
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq2_set_ui (COEFF(NTTq2_chall,i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              tmp = (((unsigned long)(ui_out & 0b11000000)) >> 6);
              intQ_set_ui (COEFF(NTTQ_chall,i), tmp);
              intQ_sub_ui (COEFF(NTTQ_chall, i), COEFF(NTTQ_chall, i), (unsigned long)1);
              intq2_set_ui (COEFF(NTTq2_chall,i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(chall,i), tmp);
              intq1_sub_ui (COEFF(chall, i), COEFF(chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(chall,i));
              intq2_canonical_form(COEFF(NTTq2_chall,i));
              intQ_canonical_form(COEFF(NTTQ_chall,i));
              break;
            }
        }
    }
  polyq1_copy(NTTq1_chall, chall);
  conv_R_polyQ_to_NTT_polyQ(NTTQ_chall);
  conv_R_polyq2_to_NTT_polyq2(NTTq2_chall);
  conv_R_polyq1_to_NTT_polyq1(NTTq1_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_challenge_NTTq1_NTTq2 (polyq1_t NTTq1_chall, polyq2_t NTTq2_chall)
{
  unsigned long tmp = 0;
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      while(1)
        {
          get_random_bytes ((void *)&ui_out, 8);
          if ((uint8_t)(ui_out & 0b00000011) != (uint8_t)0b00000011)
            {
              tmp = (unsigned long)(ui_out & 0b00000011);
              intq2_set_ui (COEFF(NTTq2_chall, i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(NTTq1_chall, i), tmp);
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall, i));
              intq2_canonical_form(COEFF(NTTq2_chall, i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00001100) != (uint8_t)0b00001100)
            {
              tmp = (((unsigned long)(ui_out & 0b00001100)) >> 2);
              intq2_set_ui (COEFF(NTTq2_chall, i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(NTTq1_chall, i), tmp);
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall, i));
              intq2_canonical_form(COEFF(NTTq2_chall, i));
              break;
            }
          else if ((uint8_t)(ui_out & 0b00110000) != (uint8_t)0b00110000)
            {
              tmp = (((unsigned long)(ui_out & 0b00110000)) >> 4);
              intq2_set_ui (COEFF(NTTq2_chall, i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(NTTq1_chall, i), tmp);
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall, i));
              intq2_canonical_form(COEFF(NTTq2_chall, i));
              break;
            }
          else if (((uint8_t)(ui_out & 0b11000000) != (uint8_t)0b11000000) >> 6)
            {
              tmp = (((unsigned long)(ui_out & 0b11000000)) >> 6);
              intq2_set_ui (COEFF(NTTq2_chall, i), tmp);
              intq2_sub_ui (COEFF(NTTq2_chall, i), COEFF(NTTq2_chall, i), (unsigned long)1);
              intq1_set_ui (COEFF(NTTq1_chall, i), tmp);
              intq1_sub_ui (COEFF(NTTq1_chall, i), COEFF(NTTq1_chall, i), (unsigned long)1);
              intq1_canonical_form(COEFF(NTTq1_chall, i));
              intq2_canonical_form(COEFF(NTTq2_chall, i));
              break;
            }
        }
    }
  conv_R_polyq2_to_NTT_polyq2(NTTq2_chall);
  conv_R_polyq1_to_NTT_polyq1(NTTq1_chall);
  return EXIT_SUCCESS;
}

inline int
get_random_little_challenge (challenge_t chall_res)
{
  unsigned long v;
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      get_random_bytes ((void *)&ui_out, 8);
      if ((uint8_t)(ui_out & 0b01111110) != (uint8_t)0)
        {
          intq1_set_ui (COEFF(chall_res, i), 0);
        }
      else
        {
          intq1_set_ui (COEFF(chall_res,i), 1);
          v = 2 * ((unsigned long)(ui_out & 0b00000001));
          intq1_sub_ui (COEFF(chall_res, i), COEFF(chall_res, i), (unsigned long)v);
          intq1_canonical_form(COEFF(chall_res,i));
        }
    }
  return EXIT_SUCCESS;
}

inline int
get_random_little_positive_challenge (challenge_t chall_res)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
    {
      get_random_bytes ((void *)&ui_out, 8);
      if ((uint8_t)(ui_out & 0b01111110) != (uint8_t)0)
        {
          intq1_set_ui (COEFF(chall_res, i), 0);
        }
      else
        {
          intq1_set_ui (COEFF(chall_res,i), 1);
        }
    }
  return EXIT_SUCCESS;
}

inline int
get_random_little_degree_little_positive_challenge (challenge_t chall_res)
{
  uint64_t ui_out = 0;
  for (int i = 0; i < DIM_R; i++)
        {
          intq1_set_ui (COEFF(chall_res, i), 0);
        }
  for (int i = 0; i < 200; i++)
    {
      get_random_bytes ((void *)&ui_out, 8);
      if ((uint8_t)(ui_out & 0b01111110) != (uint8_t)0)
        {
          intq1_set_ui (COEFF(chall_res, i), 0);
        }
      else
        {
          intq1_set_ui (COEFF(chall_res,i), 1);
          printf("\nlittle degree little positive challenge put a 1 in degree %d\n", i);
          break;
        }
    }
  for (int i = 200; i < DIM_R; i++)
    {
      intq1_set_ui (COEFF(chall_res, i), 0);
    }
  return EXIT_SUCCESS;
}

int
get_gaussian_s_intq2 (intq2_t Rq2_sample)
{
  /* The sampler returns a mpz_t that we need to initialize. */
  mpz_t sample_tmp;
  mpz_init (sample_tmp);
  gaussian_sampler_s->call (sample_tmp, gaussian_sampler_s, gmp_randstate);
  /* We now copy sample_tmp to the fmpz_t sample. But the fmpz_t sample will
   * not be canonical (i.e. between 0 and q2). */
  conv_mpz_to_intq2(Rq2_sample, sample_tmp);
  mpz_clear (sample_tmp);
  return EXIT_SUCCESS;
}

int
get_gaussian_r_intq2 (intq2_t Rq2_sample)
{
  /* The sampler returns a mpz_t that we need to initialize. */
  mpz_t sample_tmp;
  mpz_init (sample_tmp);
  gaussian_sampler_s->call (sample_tmp, gaussian_sampler_r, gmp_randstate);
  /* We now copy sample_tmp to the fmpz_t sample. But the fmpz_t sample will
   * not be canonical (i.e. between 0 and q2). */
  conv_mpz_to_intq2(Rq2_sample, sample_tmp);
  mpz_clear (sample_tmp);
  return EXIT_SUCCESS;
}

int get_gaussian_s_polyq2 (polyq2_t polq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_s->call (mpz_sample, gaussian_sampler_s,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(polq2_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_r_polyq2 (polyq2_t polq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_r->call (mpz_sample, gaussian_sampler_r,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(polq2_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_s_NTTq2 (polyq2_t NTTq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_s->call (mpz_sample, gaussian_sampler_s,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(NTTq2_sample,i), mpz_sample);
    }
  conv_R_polyq2_to_NTT_polyq2(NTTq2_sample);
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_r_NTTq2 (polyq2_t NTTq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_r->call (mpz_sample, gaussian_sampler_r,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(NTTq2_sample,i), mpz_sample);
    }
  conv_R_polyq2_to_NTT_polyq2(NTTq2_sample);
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_xi_polyq1 (polyq1_t polq1_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi->call (mpz_sample, gaussian_sampler_xi,
                                 gmp_randstate);
      conv_mpz_to_intq1 (COEFF(polq1_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_xi_polyq2 (polyq2_t polq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi->call (mpz_sample, gaussian_sampler_xi,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(polq2_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_xi_polyQ (polyQ_t polQ_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi->call (mpz_sample, gaussian_sampler_xi,
                                 gmp_randstate);
      conv_mpz_to_intQ (COEFF(polQ_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_xi_NTTQ (polyQ_t NTTQ_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi->call (mpz_sample, gaussian_sampler_xi,
                                 gmp_randstate);
      conv_mpz_to_intQ (COEFF(NTTQ_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  conv_R_polyQ_to_NTT_polyQ(NTTQ_sample);
  return EXIT_SUCCESS;
}

inline void 
get_gaussian_xi_NTTQ_V (polyQ_t *NTTQ_sample, int size)
{
  for (int i = 0; i<size; i++)
    {
      get_gaussian_xi_NTTQ(NTTQ_sample[i]);
    }
}

int get_gaussian_xi_NTTq1_NTTq2_NTTQ (polyq1_t NTTq1_sample, polyq2_t NTTq2_sample, polyQ_t NTTQ_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi->call (mpz_sample, gaussian_sampler_xi,
                                 gmp_randstate);
      conv_mpz_to_intq1 (COEFF(NTTq1_sample,i), mpz_sample);
      conv_mpz_to_intq2 (COEFF(NTTq2_sample,i), mpz_sample);
      conv_mpz_to_intQ (COEFF(NTTQ_sample,i), mpz_sample);
    }
  conv_R_polyq1_to_NTT_polyq1(NTTq1_sample);
  conv_R_polyq2_to_NTT_polyq2(NTTq2_sample);
  conv_R_polyQ_to_NTT_polyQ(NTTQ_sample);
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

inline void
get_gaussian_xi_NTTq1_NTTq2_NTTQ_V (polyq1_t *NTTq1_sample, polyq2_t *NTTq2_sample, polyQ_t *NTTQ_sample, int size)
{
  for (int i = 0; i<size; i++)
    {
      get_gaussian_xi_NTTq1_NTTq2_NTTQ(NTTq1_sample[i], NTTq2_sample[i], NTTQ_sample[i]);
    }
}

int get_gaussian_xi_NTTq1_NTTq2 (polyq1_t NTTq1_sample, polyq2_t NTTq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi->call (mpz_sample, gaussian_sampler_xi,
                                 gmp_randstate);
      conv_mpz_to_intq1 (COEFF(NTTq1_sample,i), mpz_sample);
      conv_mpz_to_intq2 (COEFF(NTTq2_sample,i), mpz_sample);
    }
  conv_R_polyq1_to_NTT_polyq1(NTTq1_sample);
  conv_R_polyq2_to_NTT_polyq2(NTTq2_sample);
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

inline void
get_gaussian_xi_NTTq1_NTTq2_V (polyq1_t *NTTq1_sample, polyq2_t *NTTq2_sample, int size)
{
  for (int i = 0; i<size; i++)
    {
      get_gaussian_xi_NTTq1_NTTq2(NTTq1_sample[i], NTTq2_sample[i]);
    }
}

int get_gaussian_xi1_polyq2 (polyq2_t polq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi1->call (mpz_sample, gaussian_sampler_xi1,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(polq2_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_xi1_NTTq2 (polyq2_t NTTq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi1->call (mpz_sample, gaussian_sampler_xi1,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(NTTq2_sample,i), mpz_sample);
    }
  conv_R_polyq2_to_NTT_polyq2(NTTq2_sample);
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

inline void 
get_gaussian_xi1_NTTq2_V (polyq2_t *NTTq2_sample, int size)
{
  for (int i = 0; i < size; i++)
    {
      get_gaussian_xi1_NTTq2(NTTq2_sample[i]);
    }
}

int get_gaussian_xi2_polyq2 (polyq2_t polq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi2->call (mpz_sample, gaussian_sampler_xi2,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(polq2_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  return EXIT_SUCCESS;
}

int get_gaussian_xi2_NTTq2 (polyq2_t NTTq2_sample)
{
  mpz_t mpz_sample;
  mpz_init (mpz_sample);

  for (int i = 0; i < DIM_R; i++)
    {
      gaussian_sampler_xi2->call (mpz_sample, gaussian_sampler_xi2,
                                 gmp_randstate);
      conv_mpz_to_intq2 (COEFF(NTTq2_sample,i), mpz_sample);
    }
  mpz_clear (mpz_sample);
  conv_R_polyq2_to_NTT_polyq2(NTTq2_sample);
  return EXIT_SUCCESS;
}

inline void
get_gaussian_xi2_NTTq2_V (polyq2_t *NTTq2_sample, int size)
{
  for (int i = 0; i < size; i++)
    {
      get_gaussian_xi2_NTTq2(NTTq2_sample[i]);
    }
}
