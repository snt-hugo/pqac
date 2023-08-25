#ifndef NEW_ARITHM_POLY_H
#define NEW_ARITHM_POLY_H

#include <gmp.h>
#include <mpfr.h>

#include "new_arithm_int.h"

#define COEFF(X, Y) ((*(X))[(Y)])

#define SIZE_POLYQ1_S8 (DIM_R * SIZE_INTQ1_S8)
#define SIZE_POLYQ2_S8 (DIM_R * SIZE_INTQ2_S8)
#define SIZE_POLYQ_S8 (DIM_R * SIZE_INTQ_S8)

typedef intp_t *polyp_intern_t;
typedef intq1_t *polyq1_intern_t;
typedef intq2_t *polyq2_intern_t;
typedef intQ_t *polyQ_intern_t;
typedef polyq1_intern_t polyq1_t[1];
typedef polyq2_intern_t polyq2_t[1];
typedef polyQ_intern_t polyQ_t[1];
typedef polyp_intern_t polyp_t[1];
typedef polyq1_t challenge_t;

void polyq1_init (polyq1_t polq1_p);
void polyq2_init (polyq2_t polq2_p);
void polyQ_init (polyQ_t polQ_p);
void polyp_init (polyp_t polp_p);
void challenge_init (challenge_t chall);

void polyq1_init_V (polyq1_t *polq1_p_V, int size);
void polyq2_init_V (polyq2_t *polq2_p_V, int size);
void polyQ_init_V (polyQ_t *polQ_p_V, int size);
void polyp_init_V (polyp_t *polp_p_V, int size);
void challenge_init_V (challenge_t *chall_V, int size);

void polyq1_clear (polyq1_t polq1_p);
void polyq2_clear (polyq2_t polq2_p);
void polyQ_clear (polyQ_t polQ_p);
void polyp_clear (polyp_t polp_p);
void challenge_clear (challenge_t chall);

void polyq1_clear_V (polyq1_t *polq1_p_V, int size);
void polyq2_clear_V (polyq2_t *polq2_p_V, int size);
void polyQ_clear_V (polyQ_t *polQ_p_V, int size);
void polyp_clear_V (polyp_t *polp_p_V, int size);
void challenge_clear_V (challenge_t *chall_V, int size);

void polyq1_add (polyq1_t polq1_res, const polyq1_t polq1_x1,
                 const polyq1_t polq1_x2);
void polyq2_add (polyq2_t polq2_res, const polyq2_t polq2_x1,
                 const polyq2_t polq2_x2);
void polyQ_add (polyQ_t polQ_res, const polyQ_t polQ_x1,
                const polyQ_t polQ_x2);
void polyp_add (polyp_t polp_res, const polyp_t polp_x1,
                const polyp_t polp_x2);

void
polyq2_set_ui_NTT(polyq2_t polq2_res, unsigned long x);
void
polyq2_set_ui(polyq2_t polq2_res, unsigned long x);
void polyq2_add_intq2_NTT (polyq2_t NTTq2_res, const polyq2_t NTTq2_pol,
                           const intq2_t intq2_x);

void polyq1_sub (polyq1_t polq1_res, polyq1_t polq1_x1, polyq1_t polq1_x2);
void polyq2_sub (polyq2_t polq2_res, polyq2_t polq2_x1, polyq2_t polq2_x2);
void polyQ_sub (polyQ_t polQ_res, polyQ_t polQ_x1, polyQ_t polQ_x2);
void polyp_sub (polyp_t polp_res, polyp_t polp_x1, polyp_t polp_x2);

void polyq1_neg (polyq1_t polq1_x);
void polyq2_neg (polyq2_t polq2_x);
void polyQ_neg (polyQ_t polQ_x);
void polyp_neg (polyp_t polp_x);

void polyq1_scalar_mul (polyq1_t polq1_res, polyq1_t polq1_pol,
                        intq1_t intq1_scal);
void polyq2_scalar_mul (polyq2_t polq2_res, polyq2_t polq2_pol,
                        intq2_t intq2_scal);
void polyQ_scalar_mul (polyQ_t polQ_res, polyQ_t polQ_pol, intQ_t intQ_scal);
void polyp_scalar_mul (polyp_t polp_res, polyp_t polp_pol, intp_t intp_scal);

void conv_little_polyq1_R_polyq1_NTT (polyq1_t polyq1_pol);
void conv_little_polyq2_R_polyq2_NTT (polyq2_t polyq2_pol);
void conv_little_polyQ_R_polyQ_NTT (polyQ_t polyQ_pol);

void polyq1_mul_NTT (polyq1_t NTTq1_res, const polyq1_t NTTq1_pol,
                     const polyq1_t NTTq1_pol2);
void polyq2_mul_NTT (polyq2_t NTTq2_res, const polyq2_t NTTq2_pol,
                     const polyq2_t NTTq2_pol2);
void polyQ_mul_NTT (polyQ_t NTTQ_res, const polyQ_t NTTQ_pol,
                    const polyQ_t NTTQ_pol2);

void conv_challenge_to_polyq1 (polyq1_t polq1_res, challenge_t chall);
void conv_challenge_to_polyq2 (polyq2_t polq2_res, challenge_t chall);
void conv_challenge_to_polyQ (polyQ_t polQ_res, challenge_t chall);

void conv_polyq1_to_polyq2 (polyq2_t polq2_res, polyq1_t polq1_in);
void conv_polyq1_to_polyQ (polyQ_t polQ_res, polyq1_t polq1_in);

void conv_R_polyq1_to_NTT_polyq1 (polyq1_t poly);
void conv_R_polyq2_to_NTT_polyq2 (polyq2_t poly);
void conv_R_polyQ_to_NTT_polyQ (polyQ_t poly);

void conv_R_polyq1_to_NTT_polyq1_V (polyq1_t *poly_V, int size);
void conv_R_polyq2_to_NTT_polyq2_V (polyq2_t *poly_V, int size);
void conv_R_polyQ_to_NTT_polyQ_V (polyQ_t *poly_V, int size);

void conv_NTT_polyq1_to_R_polyq1 (polyq1_t poly);
void conv_NTT_polyq2_to_R_polyq2 (polyq2_t poly);
void conv_NTT_polyQ_to_R_polyQ (polyQ_t poly);

void conv_NTT_polyq1_to_R_polyq1_V (polyq1_t *poly_V, int size);
void conv_NTT_polyq2_to_R_polyq2_V (polyq2_t *poly_V, int size);
void conv_NTT_polyQ_to_R_polyQ_V (polyQ_t *poly_V, int size);

void polyq1_copy(polyq1_t polq1_res, const polyq1_t polq1_in);
void polyq2_copy(polyq2_t polq2_res, const polyq2_t polq2_in);
void polyQ_copy(polyQ_t polQ_res, const polyQ_t polQ_in);
void polyp_copy(polyp_t polp_res, const polyp_t polp_in);


/* we note sigma_1 for the automorpthism sigma_-1 
 * that replace each X^i by X^((d-1)*i) in R (mod q1, q2, Q according to the context) */
void polyq1_sigma_1 (polyq1_t polq1_res, polyq1_t polq1_pol);
void polyq1_sigma_5 (polyq1_t polq1_res, polyq1_t polq1_pol);
void challenge_sigma_1 (challenge_t polq2_res, challenge_t polq2_pol);

/* we note sigma_5 for the automorpthism
 * that replace each X^i by X^(5*i) in R (mod q1, q2, Q according to the context) */
void polyq2_sigma_1 (polyq2_t polq2_res, polyq2_t polq2_pol);
void polyq2_sigma_5 (polyq2_t polq2_res, polyq2_t polq2_pol);
void challenge_sigma_5 (challenge_t polq2_res, challenge_t polq2_pol);

/* return 1 if they are equal and 0 else */
int polyq1_equal (const polyq1_t polq1_pol1, const polyq1_t polq1_pol2);
int polyq2_equal (const polyq2_t polq2_pol1, const polyq2_t polq2_pol2);
int polyQ_equal (const polyQ_t polQ_pol1, const polyQ_t polQ_pol2);
int challenge_equal(const challenge_t challenge_1, const challenge_t challenge_2);

void conv_polyq1_to_p_uint8 (uint8_t *p_u8_res, const polyq1_t polq1_x);
void conv_polyq2_to_p_uint8 (uint8_t *p_u8_res, const polyq2_t polq2_x);
void conv_polyQ_to_p_uint8 (uint8_t *p_u8_res, const polyQ_t polq1_x);

void conv_p_uint8_to_polyq1 (const polyq1_t polq1_res, uint8_t *p_u8_x);
void conv_p_uint8_to_polyq2 (const polyq2_t polq1_res, uint8_t *p_u8_x);
void conv_p_uint8_to_polyQ (const polyQ_t polq1_res, uint8_t *p_u8_x);

void conv_NTT_polyq1_to_NTT_polyq2(polyq2_t NTT_res, polyq1_t NTT_in);
void conv_NTT_polyq1_to_NTT_polyQ(polyQ_t NTT_res, polyq1_t NTT_in);

void euclidean_norm_R_polyq1 (mpfr_t norm, polyq1_t polyq1_pol);
void euclidean_norm_R_polyq2 (mpfr_t norm, polyq2_t polyq2_pol);
void euclidean_norm_R_polyQ (mpfr_t norm, polyQ_t polyQ_pol);

void euclidean_sqrnorm_R_polyq1 (mpfr_t norm, polyq1_t polyq1_pol);
void euclidean_sqrnorm_R_polyq2 (mpfr_t norm, polyq2_t polyq2_pol);
void euclidean_sqrnorm_R_polyQ (mpfr_t norm, polyQ_t polyQ_pol);

void dot_product_R_polyq1 (mpfr_t scalar_res, polyq1_t polyq1_x, polyq1_t polyq1_y);
void dot_product_R_polyq2 (mpfr_t scalar_res, polyq2_t polyq2_x, polyq2_t polyq2_y);
void dot_product_R_polyQ (mpfr_t scalar_res, polyQ_t polyQ_x, polyQ_t polyQ_y);

int rejection_sampling (polyq1_t *polyq1_z1_V3, polyq1_t *polyq1_z2_V3,
                        polyq1_t *polyq1_z3_V3, polyq1_t *polyq1_z4_V3,
                        polyQ_t *polyQ_z5_V5, polyq1_t *polyq1_b1_V3,
                        polyq1_t *polyq1_b2_V3, polyq1_t *polyq1_b3_V3,
                        polyq1_t *polyq1_b4_V3, polyQ_t *polyQ_b5_V5,
                        polyq2_t *polyq2_z_V4, polyq2_t *polyq2_b_V4,
                        polyq2_t *polyq2_z_V2, polyq2_t *polyq2_b_V2);

#endif /* NEW_ARITHM_POLY_H */
