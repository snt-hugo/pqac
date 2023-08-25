#ifndef EXTRA_TOOLS_H
#define EXTRA_TOOLS_H

#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>

#include "new_arithm_int.h"
#include "new_arithm_poly.h"

/* print the coeff d of polyq1/q2/Q/p types */
void polyq1_print_coeff (const polyq1_t polq1_pol, const size_t d);
void polyq2_print_coeff (const polyq2_t polq2_pol, const size_t d);
void polyQ_print_coeff (const polyQ_t polQ_pol, const size_t d);
void polyp_print_coeff (const polyp_t polp_pol, const size_t d);

/* print the value of intq1/q2/Q/p types */
void intq1_print (const intq1_t x);
void intq2_print (const intq2_t x);
void intQ_print (const intQ_t x);
void intp_print (const intp_t x);

void polyq2_print_norm_NTT (polyq2_t pol);
void polyq2_print_norm (polyq2_t pol);
void polyq1_print_norm_NTT (polyq1_t pol);
void polyq1_print_norm (polyq1_t pol);

/* print the comp component of the poly that is on NTT form */
void polyq1_print_comp (const polyq1_t polq1_pol, const size_t comp);
void polyq2_print_comp (const polyq2_t polq2_pol, const size_t comp);
void polyQ_print_comp (const polyQ_t polQ_pol, const size_t comp);

#endif /* EXTRA_TOOLS_H */
