#ifndef NEW_ARITHM_INT_H
#define NEW_ARITHM_INT_H

#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>
#include "params.h"





/* ###################################################### */
/* ####### Operations on intQ, intq1, intq2, intp ####### */
/* ###################################################### */

void intQ_copy (intQ_t intQ_res, intQ_t intQ_x);
void intq1_copy (intq1_t intq1_res, intq1_t intq1_x);
void intq2_copy (intq2_t intq2_res, intq2_t intq2_x);
void intp_copy (intp_t intp_res, intp_t intp_x);

void intQ_zero (intQ_t x);
void intq1_zero (intq1_t x);
void intq2_zero (intq2_t x);
void intp_zero (intp_t x);

void intQ_add (intQ_t res, const intQ_t y, const intQ_t z);
void intq1_add (intq1_t res, const intq1_t y, const intq1_t z);
void intq2_add (intq2_t res, const intq2_t y, const intq2_t z);
void intp_add (intp_t res, const intp_t y, const intp_t z);

void intQ_sub (intQ_t res, const intQ_t y, intQ_t z);
void intq1_sub (intq1_t res, const intq1_t y, intq1_t z);
void intq2_sub (intq2_t res, const intq2_t y, intq2_t z);
void intp_sub (intp_t res, const intp_t y, intp_t z);

void intQ_neg (intQ_t x);
void intq1_neg (intq1_t x);
void intq2_neg (intq2_t x);
void intp_neg (intp_t x);

void intQ_add_ui (intQ_t res, const intQ_t y, const unsigned long z);
void intq1_add_ui (intq1_t res, const intq1_t y, const unsigned long z);
void intq2_add_ui (intq2_t res, const intq2_t y, const unsigned long z);
void intp_add_ui (intp_t res, const intp_t y, const unsigned long z);

void intQ_sub_ui (intQ_t res, const intQ_t y, unsigned long z);
void intq1_sub_ui (intq1_t res, const intq1_t y, unsigned long z);
void intq2_sub_ui (intq2_t res, const intq2_t y, unsigned long z);
void intp_sub_ui (intp_t res, const intp_t y, unsigned long z);

void intQ_set_ui (intQ_t res,  const unsigned long z);
void intq1_set_ui (intq1_t res, const unsigned long z);
void intq2_set_ui (intq2_t res, const unsigned long z);
void intp_set_ui (intp_t res, const unsigned long z);

void intQ_mul (intQ_t res, const intQ_t y, const intQ_t z);
void intq1_mul (intq1_t res, const intq1_t y, const intq1_t z);
void intq2_mul (intq2_t res, const intq2_t y, const intq2_t z);
void intp_mul (intp_t res, const intp_t y, const intp_t z);

void conv_intq1_to_intq2(intq2_t intq2_res, intq1_t intq1_x);
void conv_intq1_to_intQ(intQ_t intQ_res, intq1_t intq1_x);

void conv_mpz_to_intq1(intq1_t intq1_res, const mpz_t mpz_x);
void conv_mpz_to_intq2(intq2_t intq2_res, const mpz_t mpz_x);
void conv_mpz_to_intQ(intQ_t intQ_res, const mpz_t mpz_x);
void conv_mpz_to_intp(intp_t intp_res, const mpz_t mpz_x);

void conv_uint64_to_intq1(intq1_t intq1_res, const uint64_t u64_x);
/* intq2_res will be (p_u64[0] + 2^64 p_u64[1] + 2^128 p_u64[2]) mod q2 */
void conv_p_uint64_to_intq2(intq2_t intq2_res, const uint64_t *p_u64_x);
/* intQ_res will be (p_u64[0] + 2^64 p_u64[1]) mod Q */
void conv_p_uint64_to_intQ(intQ_t intQ_res, const uint64_t *p_u64_x);
void conv_uint64_to_intp(intp_t intp_res, const uint64_t u64_x);

void intq1_canonical_form(intq1_t intq1_x);
void intq2_canonical_form(intq2_t intq2_x);
void intQ_canonical_form(intQ_t intQ_x);
void intp_canonical_form(intp_t intp_x);

int intq1_cmp(intq1_t a, intq1_t b);
int intq2_cmp(intq2_t a, intq2_t b);
int intQ_cmp(intQ_t a, intQ_t b);
int intp_cmp(intp_t a, intp_t b);

/* the intX_t can change : we set it to its canonical form 
   before the conversion to uint64. */
void conv_intq1_to_p_uint64(uint64_t *p_u64_res, intq1_t intq1_x);
void conv_intq2_to_p_uint64(uint64_t *p_u64_res, intq2_t intq2_x);
void conv_intQ_to_p_uint64(uint64_t *p_u64_res, intQ_t intQ_x);
void conv_intp_to_p_uint64(uint64_t *p_u64_res, intp_t intp_x);

/* /1* returns a negative value if the intq2 encoded by p_u64_x is < q2. Positive value if it is > q2, otherwise returns 0. *1/ */
int p_uint64_cmp_q2(const uint64_t *p_u64_x);

/* /1* used by API *1/ */
void conv_intq1_to_p_uint8(uint8_t *u8_res, intq1_t intq1_x);
void conv_intq2_to_p_uint8(uint8_t *p_u8_res, intq2_t intq2_x);
void conv_intQ_to_p_uint8(uint8_t *p_u8_res, intQ_t intQ_x);

void conv_p_uint8_to_intq1(intq1_t intq1_res, const uint8_t *p_u8_x);
void conv_p_uint8_to_intq2(intq2_t intq2_res, const uint8_t *p_u8_x);
void conv_p_uint8_to_intQ(intQ_t intQ_res, const uint8_t *p_u8_x);

#endif /* NEW_ARITHM_INT_H */
