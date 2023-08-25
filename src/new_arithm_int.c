#include <assert.h>
#include <stdio.h>

#include "new_arithm_int.h"

//#define OVERFLOW_SECURITY

#define ASSERT_OVERFLOW_SECURITY(modular_integer) assert((((uint64_t) (modular_integer[(int) sizeof(modular_integer) / (int) sizeof(modular_integer[0])])) & (3llu >> ((((int) sizeof((uint64_t) (modular_integer[(int) (sizeof(modular_integer) / sizeof(modular_integer[0]))]))) * 8) - 2))) == 0)

#define P_UINT8_TO_UINT64(X, Y)\
(X) = (((uint64_t)*((Y)+7)) << 56)\
     | (((uint64_t)*((Y)+6)) << 48)\
     | (((uint64_t)*((Y)+5)) << 40)\
     | (((uint64_t)*((Y)+4)) << 32)\
     | (((uint64_t)*((Y)+3)) << 24)\
     | (((uint64_t)*((Y)+2)) << 16)\
     | (((uint64_t)*((Y)+1)) << 8)\
     | ((uint64_t)*((Y)))

#define UINT64_TO_P_UINT8(X, Y)\
*((X)+7) = (uint8_t)((Y) >> 56);\
*((X)+6) = (uint8_t)(((Y) << 8) >> 56);\
*((X)+5) = (uint8_t)(((Y) << 16) >> 56);\
*((X)+4) = (uint8_t)(((Y) << 24) >> 56);\
*((X)+3) = (uint8_t)(((Y) << 32) >> 56);\
*((X)+2) = (uint8_t)(((Y) << 40) >> 56);\
*((X)+1) = (uint8_t)(((Y) << 48) >> 56);\
*(X) = (uint8_t)(((Y) << 56) >> 56)

inline void
intQ_zero (intQ_t x)
{
  x[0] = 0;
  x[1] = 0;
}

inline void
intq1_zero (intq1_t x)
{
  x[0] = (uint64_t)0;
}

inline void
intq2_zero (intq2_t x)
{
  x[0] = 0;
  x[1] = 0;
  x[2] = 0;
}

inline void
intp_zero (intp_t x)
{
  x[0] = 0;
}

inline void
intQ_copy (intQ_t res, intQ_t x) { res[0] = x[0]; res[1] = x[1]; }
inline void
intq1_copy (intq1_t intq1_res, intq1_t intq1_x) { intq1_res[0] = intq1_x[0]; }
inline void
intq2_copy (intq2_t res, intq2_t x) { res[0] = x[0]; res[1] = x[1]; res[2] = x[2]; }
inline void
intp_copy (intp_t intp_res, intp_t intp_x) { intp_res[0] = intp_x[0]; }

inline void
intQ_add (intQ_t res, const intQ_t y, const intQ_t z)
{ 
  mpn_add_n(res, y, z, (mp_size_t)2);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq1_add (intq1_t res, const intq1_t y, const intq1_t z) 
{ 
  res[0] = (y[0] + z[0]) ; 
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}
inline void
intq2_add (intq2_t res, const intq2_t y, const intq2_t z) { mpn_add_n(res, y, z, (mp_size_t)3); }

inline void
intp_add (intp_t res, const intp_t y, const intp_t z) { 
  res[0] = y[0] + z[0]; 
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intQ_sub (intQ_t res, const intQ_t y, intQ_t z)
{
  intQ_t z_neg = {z[0], z[1]};
  intQ_neg (z_neg);
  mpn_add_n(res, y, z_neg, (mp_size_t)2);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq1_sub (intq1_t res, const intq1_t y, intq1_t z)
{
  res[0] = (y[0] +(g_intq1_q1[0] - (z[0] % g_intq1_q1[0]))) % g_intq1_q1[0];
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq2_sub (intq2_t res, const intq2_t y, intq2_t z)
{
  intq2_t z_neg = {z[0], z[1], z[2]};
  intq2_neg (z_neg);
  mpn_add_n(res, y, z_neg, (mp_size_t)3);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intp_sub (intp_t res, const intp_t y, intp_t z)
{
  res[0] = y[0] + (g_intp_p[0] - (z[0] % g_intp_p[0]));
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intQ_add_ui (intQ_t res, const intQ_t y, const unsigned long z)
{
  mpn_add_1(res, y, 2, (mp_limb_t)z);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq1_add_ui (intq1_t res, const intq1_t y, const unsigned long z)
{
  res[0] = y[0] + z;
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq2_add_ui (intq2_t res, const intq2_t y, const unsigned long z)
{
  mpn_sub_1(res, y, 3, (mp_limb_t)z);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intp_sub_ui (intp_t res, const intp_t y, const unsigned long z)
{
  res[0] = y[0] + (g_intp_p[0] - z);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intQ_sub_ui (intQ_t res, const intQ_t y, const unsigned long z)
{
  intQ_t z_neg = {z, 0ULL};
  intQ_neg (z_neg);
  mpn_add_n(res, y, z_neg, (mp_size_t)2);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq1_sub_ui (intq1_t res, const intq1_t y, const unsigned long z)
{
  res[0] = y[0] + (g_intq1_q1[0] - z);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq2_sub_ui (intq2_t res, const intq2_t y, const unsigned long z)
{
  intq2_t z_neg = {z, 0ULL, 0ULL};
  intq2_neg (z_neg);
  mpn_add_n(res, y, z_neg, (mp_size_t)3);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intp_add_ui (intp_t res, const intp_t y, const unsigned long z)
{
  res[0] = y[0] + z;
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intQ_neg (intQ_t x)
{
  intQ_canonical_form(x);
  mpn_sub_n(x, g_intQ_Q, x, 2);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(x);
  #endif
}

inline void
intq1_neg (intq1_t x)
{
  x[0] = g_intq1_q1[0] -  (x[0] % g_intq1_q1[0]);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(x);
  #endif
}

inline void
intq2_neg (intq2_t x)
{
  intq2_canonical_form(x);
  mpn_sub_n(x, g_intq2_q2, x, 3);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(x);
  #endif
}

inline void
intp_neg (intp_t x)
{
  x[0] = g_intp_p[0] -  (x[0] % g_intp_p[0]);
  x[0] = g_intp_p[0] -  (x[0] % g_intp_p[0]);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(x);
  #endif
}

inline void intQ_set_ui (intQ_t res,  const unsigned long z)
{
  res[0] = z;
  res[1] = 0;
}
inline void intq1_set_ui (intq1_t res, const unsigned long z)
{
  res[0] = z;
}
inline void intq2_set_ui (intq2_t res, const unsigned long z)
{
  res[0] = z;
  res[1] = 0;
  res[2] = 0;
}
inline void intp_set_ui (intp_t res, const unsigned long z)
{
  res[0] = z;
}

inline void
intQ_mul (intQ_t res, const intQ_t y, const intQ_t z)
{
  mp_limb_t big_res[4] = {0};
  mp_limb_t qp[3] = {0};
  mpn_mul_n (big_res , y, z, (mp_size_t)2);
  mpn_tdiv_qr (qp, res, (mp_size_t)0, big_res, (mp_size_t)4, g_intQ_Q, (mp_size_t)2);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq1_mul (intq1_t res, const intq1_t y, const intq1_t z)
{
  mp_limb_t big_res[2] = {0};
  mp_limb_t qp[2] = {0};
  mpn_mul_n (big_res , y, z, (mp_size_t)1);
  mpn_tdiv_qr (qp, res, (mp_size_t)0, big_res, (mp_size_t)2, g_intq1_q1, (mp_size_t)1);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intq2_mul (intq2_t res, const intq2_t y, const intq2_t z)
{
  mp_limb_t big_res[6] = {0};
  mp_limb_t qp[4] = {0};
  mpn_mul_n (big_res , y, z, (mp_size_t)3);
  mpn_tdiv_qr (qp, res, (mp_size_t)0, big_res, (mp_size_t)6, g_intq2_q2, (mp_size_t)3);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

inline void
intp_mul (intp_t res, const intp_t y, const intp_t z)
{
  mp_limb_t big_res[2] = {0};
  mp_limb_t qp[2] = {0};
  mpn_mul_n (big_res , y, z, (mp_size_t)1);
  mpn_tdiv_qr (qp, res, (mp_size_t)0, big_res, (mp_size_t)2, g_intp_p, (mp_size_t)1);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(res);
  #endif
}

/* Canonical form of x mod q1 is the x in [0, q1-1]. */
inline void intq1_canonical_form(intq1_t intq1_x)
{
  mp_limb_t qp[1] = {0};
  mpn_tdiv_qr (qp, intq1_x, (mp_size_t)0, intq1_x, (mp_size_t)1, g_intq1_q1, (mp_size_t)1);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(intq1_x);
  #endif
}

/* Canonical form of x mod q2 is the x in [0, q2-1]. */
inline void intq2_canonical_form(intq2_t intq2_x)
{
  mp_limb_t qp[1] = {0};
  mpn_tdiv_qr (qp, intq2_x, (mp_size_t)0, intq2_x, (mp_size_t)3, g_intq2_q2, (mp_size_t)3);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(intq2_x);
  #endif
}

/* Canonical form of x mod Q is the x in [0, Q-1]. */
inline void intQ_canonical_form(intQ_t intQ_x)
{
  mp_limb_t qp[1] = {0};
  mpn_tdiv_qr (qp, intQ_x, (mp_size_t)0, intQ_x, (mp_size_t)2, g_intQ_Q, (mp_size_t)2);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(intQ_x);
  #endif
}

/* Canonical form of x mod p is the x in [0, p-1]. */
inline void intp_canonical_form(intp_t intp_x)
{
  mp_limb_t qp[1] = {0};
  mpn_tdiv_qr (qp, intp_x, (mp_size_t)0, intp_x, (mp_size_t)1, g_intp_p, (mp_size_t)1);
  #ifdef OVERFLOW_SECURITY
  ASSERT_OVERFLOW_SECURITY(intp_x);
  #endif
}

/* Put a and b in canonical form, and then, returns a negative value if a < b, a positive value if b < a 
 * and otherwise returns 0. */
int intq1_cmp(intq1_t a, intq1_t b)
{
  intq1_canonical_form(a);
  intq1_canonical_form(b);
  return mpn_cmp(a, b, 1);
}

/* Put a and b in canonical form, and then, returns a negative value if a < b, a positive value if b < a 
 * and otherwise returns 0. */
int intq2_cmp(intq2_t a, intq2_t b)
{
  intq2_canonical_form(a);
  intq2_canonical_form(b);
  return mpn_cmp(a, b, 3);
}

/* Put a and b in canonical form, and then, returns a negative value if a < b, a positive value if b < a 
 * and otherwise returns 0. */
int intQ_cmp(intQ_t a, intQ_t b)
{
  intQ_canonical_form(a);
  intQ_canonical_form(b);
  return mpn_cmp(a, b, 2);
}

/* Put a and b in canonical form, and then, returns a negative value if a < b, a positive value if b < a 
 * and otherwise returns 0.  */
int intp_cmp(intp_t a, intp_t b)
{
  intp_canonical_form(a);
  intp_canonical_form(b);
  return mpn_cmp(a, b, 1);
}

void conv_intq1_to_intq2(intq2_t intq2_res, intq1_t intq1_x)
{
  // intq1_res will be intq1_x in canonical form between 0 and q1 - 1.
  intq1_canonical_form(intq1_x);
  /* If the number intq1_res is in [0, (q1-1)/2[, we left if as it is.
   * If the number is in [(q1-1)/2, q1-1[, we add (q2 - q1) to intq1_res
   * to have canonical form betwen 0 and q2 - 1. */
  if (intq1_x[0] >= ((g_intq1_q1[0]) >> 1) )
    {
      mpn_add_1(intq2_res, g_intq2_q2_minus_q1, 3, intq1_x[0]);
    }
  else 
    {
      intq2_res[0] = intq1_x[0];
      intq2_res[1] = 0;
      intq2_res[2] = 0;
    }
}

void conv_intq1_to_intQ(intQ_t intQ_res, intq1_t intq1_x)
{
  intq1_canonical_form(intq1_x);
  /* If the number intq1_res is in [0, q1 - 1/2[, we left if as it is. 
   * If the number is in [q1 - 1/2, q1 - 1[, we add (Q - q1) to intq1_res 
   * to have canonical form betwen 0 and Q - 1. */
  if (intq1_x[0] >= ((g_intq1_q1[0]) >> 1) )
    {
      mpn_add_1(intQ_res, g_intQ_Q_minus_q1, 3, intq1_x[0]);
    }
  else 
    {
      intQ_res[0] = intq1_x[0];
      intQ_res[1] = 0;
    }
}

void conv_mpz_to_intq1(intq1_t intq1_res, const mpz_t mpz_x)
{
  intq1_res[0] = mpz_getlimbn(mpz_x, 0);
  if (mpz_sgn(mpz_x) == -1)
    {
      intq1_neg(intq1_res);
    }
}

void conv_mpz_to_intq2(intq2_t intq2_res, const mpz_t mpz_x)
{
  intq2_res[0] = mpz_getlimbn(mpz_x, 0);
  intq2_res[1] = mpz_getlimbn(mpz_x, 1);
  intq2_res[2] = mpz_getlimbn(mpz_x, 2);
  if (mpz_sgn(mpz_x) == -1)
    {
      intq2_neg(intq2_res);
    }
}

void conv_mpz_to_intQ(intQ_t intQ_res, const mpz_t mpz_x)
{
  intQ_res[0] = mpz_getlimbn(mpz_x, 0);
  intQ_res[1] = mpz_getlimbn(mpz_x, 1);
  if (mpz_sgn(mpz_x) == -1)
    {
      intQ_neg(intQ_res);
    }
}

void conv_uint64_to_intq1(intq1_t res, const uint64_t u64_x)
{
  res[0] = u64_x;
  intq1_canonical_form(res);
}

/* intq2_res will be (p_u64[0] + 2^64 p_u64[1] + 2^128 p_u64[2]) mod q2. */
void conv_p_uint64_to_intq2(intq2_t res, const uint64_t *p_u64_x)
{
  res[0] = p_u64_x[0];
  res[1] = p_u64_x[1];
  res[2] = p_u64_x[2];
  intq2_canonical_form(res);
}
/* intQ_res will be (p_u64[0] + 2^64 p_u64[1]) mod Q. */
void conv_p_uint64_to_intQ(intQ_t res, const uint64_t *p_u64_x)
{
  res[0] = p_u64_x[0];
  res[1] = p_u64_x[1];
  intQ_canonical_form(res);
}

void conv_uint64_to_intp(intp_t res, const uint64_t u64_x)
{
  res[0] = u64_x;
  intp_canonical_form(res);
}

/* Note : we ensure unicity of the result by taking the canonical form of the element
 * before the conversion to uint64. */
void conv_intq1_to_p_uint64(uint64_t *p_u64_res, intq1_t intq1_x)
{
  intq1_canonical_form(intq1_x);
  p_u64_res[0] = intq1_x[0];
}

void conv_intq2_to_p_uint64(uint64_t *p_u64_res, intq2_t intq2_x)
{
  intq2_canonical_form(intq2_x);
  p_u64_res[0] = intq2_x[0];
  p_u64_res[1] = intq2_x[1];
  p_u64_res[2] = intq2_x[2];
}

void conv_intQ_to_p_uint64(uint64_t *p_u64_res, intQ_t intQ_x)
{
  intQ_canonical_form(intQ_x);
  p_u64_res[0] = intQ_x[0];
  p_u64_res[1] = intQ_x[1];
}

void conv_intp_to_p_uint64(uint64_t *p_u64_res, intp_t intp_x)
{
  intp_canonical_form(intp_x);
  p_u64_res[0] = intp_x[0];
}

/* Returns a negative value if the intq2 encoded by p_u64_x is < q2,
 * a positive value if it is > q2 and otherwise returns 0. */
int p_uint64_cmp_q2(const intq2_t a)
{
  return mpn_cmp(a, g_intq2_q2, 3);
}

void conv_intq1_to_p_uint8(uint8_t *p_u8_res,intq1_t intq1_x)
{
  intq1_canonical_form(intq1_x);
  UINT64_TO_P_UINT8(p_u8_res, intq1_x[0]);
}

void conv_intq2_to_p_uint8(uint8_t *p_u8_res,intq2_t intq2_x)
{
  intq2_canonical_form(intq2_x);
  UINT64_TO_P_UINT8(p_u8_res, intq2_x[0]);
  UINT64_TO_P_UINT8(p_u8_res+8, intq2_x[1]);
  UINT64_TO_P_UINT8(p_u8_res+16, intq2_x[2]);
}

void conv_intQ_to_p_uint8(uint8_t *p_u8_res, intQ_t intQ_x)
{
  intQ_canonical_form(intQ_x);
  UINT64_TO_P_UINT8(p_u8_res, intQ_x[0]);
  UINT64_TO_P_UINT8(p_u8_res+8, intQ_x[1]);
}

void conv_p_uint8_to_intq1(intq1_t intq1_res, const uint8_t *p_u8_x)
{
  P_UINT8_TO_UINT64(intq1_res[0], p_u8_x);
  P_UINT8_TO_UINT64(intq1_res[0], p_u8_x);
  intq1_canonical_form(intq1_res);
}

void conv_p_uint8_to_intq2(intq2_t intq2_res, const uint8_t *p_u8_x)
{
  P_UINT8_TO_UINT64(intq2_res[0], p_u8_x);
  P_UINT8_TO_UINT64(intq2_res[1], p_u8_x + 8);
  P_UINT8_TO_UINT64(intq2_res[2], p_u8_x + 16);
  intq2_canonical_form(intq2_res);
}

void conv_p_uint8_to_intQ(intQ_t intQ_res, const uint8_t *p_u8_x)
{
  P_UINT8_TO_UINT64(intQ_res[0], p_u8_x);
  P_UINT8_TO_UINT64(intQ_res[1], p_u8_x + 8);
  intQ_canonical_form(intQ_res);
}
