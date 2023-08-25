#include <gmp.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "params.h"

#define PROBA_MASK1 0b01111110
#define PROBA_MASK2 0b00000001

/* ctx have to be flipped. Output a fmpz_t integer z such that 0 <= z < q2
   FDH is for Full Domain Hash
   ctx have to be flipped. output a a R poly mod q2.
   (i.e a fmpz_mod_poly_t polynomial P of degree < d ( =DIM_R )) */
int
FDH_polyq2 (polyq2_t polq2_res, shake256_context *shake_context)
{
  uint8_t u8_buf_V8[8];
  uint64_t u64_coeff_res_V3[3] = { 0, 0, 0 };

  for (int k = 0; k < DIM_R; k++)
    {
      while (1)
        {
          shake256_extract (shake_context, (void *)u8_buf_V8, 8);
          u64_coeff_res_V3[0] = (uint64_t)u8_buf_V8[0]
                                | (uint64_t)u8_buf_V8[1] << 8
                                | (uint64_t)u8_buf_V8[2] << 16
                                | (uint64_t)u8_buf_V8[3] << 24
                                | (uint64_t)u8_buf_V8[4] << 32
                                | (uint64_t)u8_buf_V8[5] << 40
                                | (uint64_t)u8_buf_V8[6] << 48
                                | (uint64_t)u8_buf_V8[7] << 56;
          shake256_extract (shake_context, (void *)u8_buf_V8, 8);
          u64_coeff_res_V3[1] = (uint64_t)u8_buf_V8[0]
                                 | (uint64_t)u8_buf_V8[1] << 8
                                 | (uint64_t)u8_buf_V8[2] << 16
                                 | (uint64_t)u8_buf_V8[3] << 24
                                 | (uint64_t)u8_buf_V8[4] << 32
                                 | (uint64_t)u8_buf_V8[5] << 40
                                 | (uint64_t)u8_buf_V8[6] << 48
                                 | (uint64_t)u8_buf_V8[7] << 56;
          shake256_extract (shake_context, (void *)u8_buf_V8,
                            SIZE_Q2_S8 - 16);
          u64_coeff_res_V3[2] = 0;
          for (int i = 0; i < SIZE_Q2_S8 - 16; i++)
            {
              u64_coeff_res_V3[2]
                  = u64_coeff_res_V3[2] | (uint64_t)u8_buf_V8[i] << 8 * i;
            }
          /* Returns a negative value if the intq2 encoded by p_u64_x is < q2. Positive value if it is > q2, otherwise returns 0. */
          if (p_uint64_cmp_q2((mp_limb_t*)u64_coeff_res_V3) < 0)
            {
              conv_p_uint64_to_intq2 (COEFF(polq2_res,k), u64_coeff_res_V3);
              break;
            }
        }
    }
  return EXIT_SUCCESS;
}

int
FDH_challenges (challenge_t chall_res, shake256_context *shake_context)
{
  uint8_t u8_buf;
  for (int k = 0; k < DIM_R; k++)
    {
      shake256_extract (shake_context, (void*)(&u8_buf), 1);
      /* We out 0 with a proba of 1-2^(-64), this is why there is 6 "1" in PROBA_MASK1. */
      if  ((uint8_t)(u8_buf & PROBA_MASK1) != (uint8_t)0)
        {
          intq1_set_ui (COEFF(chall_res,k), (unsigned long)0);
        }
      else
        {
      /* We out 1 and -1 with an equal probability. */
          intq1_set_ui (COEFF(chall_res,k), 1);
          intq1_sub_ui (COEFF(chall_res,k), COEFF(chall_res,k), 2*((signed long)(u8_buf & PROBA_MASK2)));
        }
    }
  return EXIT_SUCCESS;
}
