#include "extra_tools.h"

void polyq1_print_coeff (const polyq1_t polq1_pol, const size_t d)
{
  printf("%zu::", d);
  intq1_print(COEFF(polq1_pol,d));
}
void polyq2_print_coeff (const polyq2_t polq2_pol, const size_t d)
{
  printf("%zu::", d);
  intq2_print(COEFF(polq2_pol,d));
}
void polyQ_print_coeff (const polyQ_t polQ_pol, const size_t d)
{
  printf("%zu::", d);
  intQ_print(COEFF(polQ_pol,d));
}
void polyp_print_coeff (const polyp_t polp_pol, const size_t d)
{
  printf("%zu::", d);
  intp_print(COEFF(polp_pol,d));
}

void
polyq1_print_comp (const polyq1_t polq1_pol, const size_t comp)
{
  printf ("Component %zu : ", comp);
  intq1_print(COEFF(polq1_pol,4*comp));
  printf ("  ");
  intq1_print(COEFF(polq1_pol,4*comp+1));
  printf ("  ");
  intq1_print(COEFF(polq1_pol,4*comp+2));
  printf ("  ");
  intq1_print(COEFF(polq1_pol,4*comp+3));
  printf ("  ");
}

void
polyq2_print_comp (const polyq2_t polq2_pol, const size_t comp)
{
  printf ("Component %zu : ", comp);
  intq2_print(COEFF(polq2_pol,4*comp));
  printf ("  ");
  intq2_print(COEFF(polq2_pol,4*comp+1));
  printf ("  ");
  intq2_print(COEFF(polq2_pol,4*comp+2));
  printf ("  ");
  intq2_print(COEFF(polq2_pol,4*comp+3));
  printf ("  ");
}

void
polyq2_print_norm_NTT (polyq2_t pol)
{
  mpfr_t norm;
  mpfr_init(norm);
  conv_NTT_polyq2_to_R_polyq2(pol);
  euclidean_norm_R_polyq2 (norm, pol);
  mpfr_out_str (stdout, 10, 0, norm, MPFR_RNDD);
  conv_R_polyq2_to_NTT_polyq2(pol);
  mpfr_clear(norm);
}

void
polyq2_print_norm (polyq2_t pol)
{
  mpfr_t norm;
  mpfr_init(norm);
  euclidean_norm_R_polyq2 (norm, pol);
  mpfr_out_str (stdout, 10, 0, norm, MPFR_RNDD);
  mpfr_clear(norm);
}

void
polyq1_print_norm_NTT (polyq1_t pol)
{
  mpfr_t norm;
  mpfr_init(norm);
  conv_NTT_polyq1_to_R_polyq1(pol);
  euclidean_norm_R_polyq1 (norm, pol);
  mpfr_out_str (stdout, 10, 0, norm, MPFR_RNDD);
  conv_R_polyq1_to_NTT_polyq1(pol);
  mpfr_clear(norm);
}

void
polyq1_print_norm (polyq1_t pol)
{
  mpfr_t norm;
  mpfr_init(norm);
  euclidean_norm_R_polyq1 (norm, pol);
  mpfr_out_str (stdout, 10, 0, norm, MPFR_RNDD);
  mpfr_clear(norm);
}

void
polyQ_print_comp (const polyQ_t polQ_pol, const size_t comp)
{
  printf ("Component %zu : ", comp);
  intQ_print(COEFF(polQ_pol,4*comp));
  printf ("  ");
  intQ_print(COEFF(polQ_pol,4*comp+1));
  printf ("  ");
  intQ_print(COEFF(polQ_pol,4*comp+2));
  printf ("  ");
  intQ_print(COEFF(polQ_pol,4*comp+3));
  printf ("  ");
}

void intq1_print(const intq1_t x)
{
  printf("%lu", x[0]);
};

void intq2_print(const intq2_t x)
{
  mpz_t mpz_lo, mpz_mid, mpz_hi, mpz_x;
  mpz_init(mpz_lo);
  mpz_init(mpz_mid);
  mpz_init(mpz_hi);
  mpz_init(mpz_x);
  mpz_set_ui(mpz_lo, (uint64_t)x[0]);
  mpz_set_ui(mpz_mid, (uint64_t)x[1]);
  mpz_set_ui(mpz_hi, (uint64_t)x[2]);
  mpz_mul_2exp (mpz_mid, mpz_mid, 64);
  mpz_mul_2exp (mpz_hi, mpz_hi, 128);
  mpz_add(mpz_x, mpz_lo, mpz_mid);
  mpz_add(mpz_x, mpz_x, mpz_hi);
  gmp_printf ("%Zd",mpz_x);
  mpz_clear(mpz_lo);
  mpz_clear(mpz_mid);
  mpz_clear(mpz_hi);
  mpz_clear(mpz_x);
};

void intQ_print(const intQ_t x)
{
  mpz_t mpz_lo, mpz_mid, mpz_x;
  mpz_init(mpz_lo);
  mpz_init(mpz_mid);
  mpz_init(mpz_x);
  mpz_set_ui(mpz_lo, (uint64_t)x[0]);
  mpz_set_ui(mpz_mid, (uint64_t)x[1]);
  mpz_mul_2exp (mpz_mid, mpz_mid, 64);
  mpz_add(mpz_x, mpz_lo, mpz_mid);
  gmp_printf ("%Zd",mpz_x);
  mpz_clear(mpz_lo);
  mpz_clear(mpz_mid);
  mpz_clear(mpz_x);
};

void intp_print(const intp_t x)
{
  printf("%lu", x[0]);
};
