#include <gmp.h>
#include <stdint.h>
#include <stdio.h>

#include "compression_rice.h"
#include "extra_tools.h"
#include "new_arithm_int.h"
#include "new_arithm_poly.h"
#include "params.h"
#include "samplers.h"

#define OK 0
#define KO 1

#define PRINT_NAME_TEST  printf("**** %s ****\n" , __FUNCTION__)

int test_compression_rice();

int main() {
  rand_init();
  test_compression_rice();
  rand_clear();
  return EXIT_SUCCESS;
}

int test_compression_rice() {
  printf("\n");
  PRINT_NAME_TEST;
  printf("\n");
  polyq1_t polyq1_in;
  polyq1_init(polyq1_in);
  polyq1_t polyq1_out;
  polyq1_init(polyq1_out);
  int position = 0;
  int parameter = CEIL_LOG_XI;
  uint8_t *compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo q1, with standard deviation xi, to form a polynomial...\n");
  get_gaussian_xi_polyq1(polyq1_in);
  printf("Compression of this polynomial to form a byte string...\n");
  unsigned long long number_of_bits_compression;
  number_of_bits_compression = compression_rice_polyq1(compression_space, polyq1_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  unsigned long long number_of_bits_decompression;
  number_of_bits_decompression = decompression_rice_polyq1(polyq1_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyq1_equal(polyq1_in, polyq1_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyq1_clear(polyq1_in);
  polyq1_clear(polyq1_out);
  free(compression_space);

  polyq2_t polyq2_in;
  polyq2_init(polyq2_in);
  polyq2_t polyq2_out;
  polyq2_init(polyq2_out);
  position = 0;
  parameter = CEIL_LOG_XI;
  compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo q2, with standard deviation xi, to form a polynomial...\n");
  get_gaussian_xi_polyq2(polyq2_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyq2(compression_space, polyq2_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyq2(polyq2_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyq2_equal(polyq2_in, polyq2_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyq2_clear(polyq2_in);
  polyq2_clear(polyq2_out);
  free(compression_space);

  polyq2_init(polyq2_in);
  polyq2_init(polyq2_out);
  position = 0;
  parameter = CEIL_LOG_XI1;
  compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo q2, with standard deviation xi1, to form a polynomial...\n");
  get_gaussian_xi1_polyq2(polyq2_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyq2(compression_space, polyq2_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyq2(polyq2_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyq2_equal(polyq2_in, polyq2_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyq2_clear(polyq2_in);
  polyq2_clear(polyq2_out);
  free(compression_space);

  polyq2_init(polyq2_in);
  polyq2_init(polyq2_out);
  position = 0;
  parameter = CEIL_LOG_XI2;
  compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo q2, with standard deviation xi2, to form a polynomial...\n");
  get_gaussian_xi2_polyq2(polyq2_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyq2(compression_space, polyq2_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyq2(polyq2_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyq2_equal(polyq2_in, polyq2_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyq2_clear(polyq2_in);
  polyq2_clear(polyq2_out);
  free(compression_space);

  polyq2_init(polyq2_in);
  polyq2_init(polyq2_out);
  position = 0;
  parameter = CEIL_LOG_S;
  compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo q2, with standard deviation s, to form a polynomial...\n");
  get_gaussian_s_polyq2(polyq2_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyq2(compression_space, polyq2_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyq2(polyq2_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyq2_equal(polyq2_in, polyq2_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyq2_clear(polyq2_in);
  polyq2_clear(polyq2_out);
  free(compression_space);

  polyq2_init(polyq2_in);
  polyq2_init(polyq2_out);
  position = 0;
  parameter = CEIL_LOG_R;
  compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo q2, with standard deviation r, to form a polynomial...\n");
  get_gaussian_r_polyq2(polyq2_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyq2(compression_space, polyq2_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyq2(polyq2_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyq2_equal(polyq2_in, polyq2_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyq2_clear(polyq2_in);
  polyq2_clear(polyq2_out);
  free(compression_space);

  polyQ_t polyQ_in;
  polyQ_init(polyQ_in);
  polyQ_t polyQ_out;
  polyQ_init(polyQ_out);
  position = 0;
  parameter = CEIL_LOG_XI;
  compression_space = malloc(DIM_R * 20 * sizeof(uint8_t));
  printf("Gaussian sampling of integers modulo Q, with standard deviation xi, to form a polynomial...\n");
  get_gaussian_xi_polyQ(polyQ_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyQ(compression_space, polyQ_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyQ(polyQ_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyQ_equal(polyQ_in, polyQ_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n\n");
  }
  polyQ_clear(polyQ_in);
  polyQ_clear(polyQ_out);
  free(compression_space);

  polyQ_init(polyQ_in);
  polyQ_init(polyQ_out);
  position = 0;
  parameter = SIZE_Q_S1 - 1;
  compression_space = malloc(DIM_R * 100 * sizeof(uint8_t));
  printf("Uniformly random sampling of integers modulo Q to form a polynomial...\n");
  get_random_polyQ(polyQ_in);
  printf("Compression of this polynomial to form a byte string...\n");
  number_of_bits_compression = compression_rice_polyQ(compression_space, polyQ_in, parameter,
                                                       position);
  printf("Decompression of this byte string to form a second polynomial...\n");
  number_of_bits_decompression = decompression_rice_polyQ(polyQ_out, compression_space,
                                                           parameter, position);
  printf("Number of bits taken by the polynomial after compression: %llu.\n", number_of_bits_compression);
  printf("Number of bits processed by the decompression function: %llu.\n", number_of_bits_decompression);
  printf("Comparison of the first and second polynomials…\n");
  if (polyQ_equal(polyQ_in, polyQ_out) == 1) {
    printf("The two polynomials are equal.\nSUCCESS.\n");
  }
  else {
    printf("The two polynomials are different.\nERROR.\n");
  }
  polyQ_clear(polyQ_in);
  polyQ_clear(polyQ_out);
  return OK;
}
