#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "compression_rice.h"
#include "new_arithm_int.h"

// (q1 + 1) / 2 = 598987900929
intq1_t sign_threshold_q1 = {598987900929ULL};
// (q2 + 1) / 2 = 1382430116136883238249363806303881302638045185
intq2_t sign_threshold_q2 = {14660898966939871233ULL, 18074373720808557286ULL, 4062596ULL};
// (Q + 1) / 2 = 20709914292868106880178177
intQ_t sign_threshold_Q = {12975731425214617601ULL, 1122686ULL};


unsigned long long compression_rice_intq1(uint8_t *initial_value, intq1_t sampled_integer_input,
                                          int parameter, int position) {
  intq1_t sampled_integer;
  intq1_copy(sampled_integer, sampled_integer_input);
  intq1_t sign_threshold;
  intq1_copy(sign_threshold, sign_threshold_q1);
  int sign_bit = intq1_cmp(sampled_integer, sign_threshold);
  if (sign_bit < 1)
    sign_bit = 0;
  if (sign_bit == 1) {
    intq1_neg(sampled_integer);
  }
  mpz_t sampled_integer_mpz;
  mpz_init_set_ui(sampled_integer_mpz, sampled_integer[0]);
  mpz_t quotient;
  mpz_init(quotient);
  mpz_tdiv_q_2exp(quotient, sampled_integer_mpz, parameter);
  mpz_t number_of_bits_used;
  mpz_init(number_of_bits_used);
  mpz_add_ui(number_of_bits_used, quotient, parameter + 2);
  mpz_t remainder;
  mpz_init(remainder);
  mpz_tdiv_r_2exp(remainder, sampled_integer_mpz, parameter);
  uint8_t *compressed_value = initial_value;
  *compressed_value &= ~(0xff << position);
  *compressed_value |= (sign_bit << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
    *compressed_value = 0;
  }
  while (mpz_sgn(quotient) != 0) {
    *compressed_value |= (1 << position);
    position++;
    if (position == 8) {
      position = 0;
      compressed_value++;
      *compressed_value = 0;
    }
    mpz_sub_ui(quotient, quotient, 1);
  }
  *compressed_value &= ~(0xff << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
    *compressed_value = 0;
  }
  uint8_t remainder_least_significant_bits;
  remainder_least_significant_bits = (uint8_t) mpz_get_ui(remainder);
  *compressed_value |= remainder_least_significant_bits << position;
  mpz_tdiv_q_2exp(remainder, remainder, 8 - position);
  int remainder_remaining_bits = parameter - 8 + position;
  while (remainder_remaining_bits > 0) {
    compressed_value++;
    *compressed_value = (uint8_t) mpz_get_ui(remainder); // do not erase last bits ?
    mpz_tdiv_q_2exp(remainder, remainder, 8);
    remainder_remaining_bits -= 8;
  }
  unsigned long long number_of_bits = mpz_get_ui(number_of_bits_used);
  mpz_clear(sampled_integer_mpz);
  mpz_clear(quotient);
  mpz_clear(remainder);
  return number_of_bits;
}

unsigned long long decompression_rice_intq1(intq1_t integer, uint8_t *compressed_value, int parameter,
                           int position) {
  uint8_t sign_indication = *compressed_value & (1 << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
  }
  mpz_t quotient;
  mpz_init(quotient);
  uint8_t bit_of_quotient;
  bit_of_quotient = *compressed_value & (1 << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
  }
  while (bit_of_quotient != 0) {
    mpz_add_ui(quotient, quotient, 1);
    bit_of_quotient = *compressed_value & (1 << position);
    position++;
    if (position == 8) {
      position = 0;
      compressed_value++;
    }
  }
  unsigned long long quotient_ui = mpz_get_ui(quotient);
  int number_of_remaining_bytes = (parameter + position) / 8;
  if ((parameter + position) % 8 == 0) {
    number_of_remaining_bytes--;
  }
  compressed_value += number_of_remaining_bytes;
  mpz_t integer_mpz;
  mpz_init_set_ui(integer_mpz, *compressed_value);
  while (number_of_remaining_bytes != 0) {
    mpz_mul_2exp(integer_mpz, integer_mpz, 8);
    compressed_value--;
    mpz_add_ui(integer_mpz, integer_mpz, *compressed_value);
    number_of_remaining_bytes--;
  }
  mpz_tdiv_q_2exp(integer_mpz, integer_mpz, position);
  mpz_tdiv_r_2exp(integer_mpz, integer_mpz, parameter);
  mpz_mul_2exp(quotient, quotient, parameter);
  mpz_ior(integer_mpz, integer_mpz, quotient);
  conv_mpz_to_intq1(integer, integer_mpz);
  if (sign_indication != 0) {
    intq1_neg(integer);
  }
  return quotient_ui + parameter + 2;
}

unsigned long long compression_rice_polyq1(uint8_t *compression_space, polyq1_t poly_input,
                                           int parameter, int position) {
  unsigned long long number_of_bits;
  unsigned long long total_number_of_bits = 0;
  for (int i = 0; i < DIM_R; i++) {
    number_of_bits = compression_rice_intq1(compression_space, COEFF(poly_input, i), parameter,
                     position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  return total_number_of_bits;
}

unsigned long long decompression_rice_polyq1(polyq1_t polyq1_output, uint8_t *compression_space,
                                             int parameter, int position) {
  unsigned long long number_of_bits;
  unsigned long long total_number_of_bits = 0;
  for (int i = 0; i < DIM_R; i++) {
    number_of_bits = decompression_rice_intq1(COEFF(polyq1_output, i), compression_space, parameter,
                     position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  return total_number_of_bits;
}

unsigned long long compression_rice_intq2(uint8_t *initial_value, intq2_t sampled_integer_input,
                                          int parameter, int position) {
  intq2_t sampled_integer;
  intq2_copy(sampled_integer, sampled_integer_input);
  intq2_t sign_threshold;
  intq2_copy(sign_threshold, sign_threshold_q2);
  int sign_bit = intq2_cmp(sampled_integer, sign_threshold);
  if (sign_bit < 1)
    sign_bit = 0;
  if (sign_bit == 1) {
    intq2_neg(sampled_integer);
  }
  mpz_t sampled_integer_mpz;
  mpz_init_set_ui(sampled_integer_mpz, sampled_integer[2]);
  mpz_mul_2exp(sampled_integer_mpz, sampled_integer_mpz, 64);
  mpz_add_ui(sampled_integer_mpz, sampled_integer_mpz, sampled_integer[1]);
  mpz_mul_2exp(sampled_integer_mpz, sampled_integer_mpz, 64);
  mpz_add_ui(sampled_integer_mpz, sampled_integer_mpz, sampled_integer[0]);
  mpz_t quotient;
  mpz_init(quotient);
  mpz_tdiv_q_2exp(quotient, sampled_integer_mpz, parameter);
  mpz_t number_of_bits_used;
  mpz_init(number_of_bits_used);
  mpz_add_ui(number_of_bits_used, quotient, parameter + 2);
  mpz_t remainder;
  mpz_init(remainder);
  mpz_tdiv_r_2exp(remainder, sampled_integer_mpz, parameter);
  uint8_t *compressed_value = initial_value;
  *compressed_value &= ~(0xff << position);
  *compressed_value |= (sign_bit << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
    *compressed_value = 0;
  }
  while (mpz_sgn(quotient) != 0) {
    *compressed_value |= (1 << position);
    position++;
    if (position == 8) {
      position = 0;
      compressed_value++;
      *compressed_value = 0;
    }
    mpz_sub_ui(quotient, quotient, 1);
  }
  *compressed_value &= ~(0xff << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
    *compressed_value = 0;
  }
  uint8_t remainder_least_significant_bits;
  remainder_least_significant_bits = (uint8_t) mpz_get_ui(remainder);
  *compressed_value |= remainder_least_significant_bits << position;
  mpz_tdiv_q_2exp(remainder, remainder, 8 - position);
  int remainder_remaining_bits = parameter - 8 + position;
  while (remainder_remaining_bits > 0) {
    compressed_value++;
    *compressed_value = (uint8_t) mpz_get_ui(remainder); // do not erase last bits ?
    mpz_tdiv_q_2exp(remainder, remainder, 8);
    remainder_remaining_bits -= 8;
  }
  unsigned long long number_of_bits = mpz_get_ui(number_of_bits_used);
  mpz_clear(sampled_integer_mpz);
  mpz_clear(quotient);
  mpz_clear(remainder);
  return number_of_bits;
}

unsigned long long decompression_rice_intq2(intq2_t integer, uint8_t *compressed_value, int parameter,
                           int position) {
  uint8_t sign_indication = *compressed_value & (1 << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
  }
  mpz_t quotient;
  mpz_init(quotient);
  uint8_t bit_of_quotient;
  bit_of_quotient = *compressed_value & (1 << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
  }
  while (bit_of_quotient != 0) {
    mpz_add_ui(quotient, quotient, 1);
    bit_of_quotient = *compressed_value & (1 << position);
    position++;
    if (position == 8) {
      position = 0;
      compressed_value++;
    }
  }
  unsigned long long quotient_ui = mpz_get_ui(quotient);
  int number_of_remaining_bytes = (parameter + position) / 8;
  if ((parameter + position) % 8 == 0) {
    number_of_remaining_bytes--;
  }
  compressed_value += number_of_remaining_bytes;
  mpz_t integer_mpz;
  mpz_init_set_ui(integer_mpz, *compressed_value);
  while (number_of_remaining_bytes != 0) {
    mpz_mul_2exp(integer_mpz, integer_mpz, 8);
    compressed_value--;
    mpz_add_ui(integer_mpz, integer_mpz, *compressed_value);
    number_of_remaining_bytes--;
  }
  mpz_tdiv_q_2exp(integer_mpz, integer_mpz, position);
  mpz_tdiv_r_2exp(integer_mpz, integer_mpz, parameter);
  mpz_mul_2exp(quotient, quotient, parameter);
  mpz_ior(integer_mpz, integer_mpz, quotient);
  conv_mpz_to_intq2(integer, integer_mpz);
  if (sign_indication != 0) {
    intq2_neg(integer);
  }
  return quotient_ui + parameter + 2;
}

unsigned long long compression_rice_polyq2(uint8_t *compression_space, polyq2_t poly_input,
                                           int parameter, int position) {
  unsigned long long number_of_bits;
  unsigned long long total_number_of_bits = 0;
  for (int i = 0; i < DIM_R; i++) {
    number_of_bits = compression_rice_intq2(compression_space, COEFF(poly_input, i), parameter,
                     position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  return total_number_of_bits;
}

unsigned long long decompression_rice_polyq2(polyq2_t polyq2_output, uint8_t *compression_space,
                                             int parameter, int position) {
  unsigned long long number_of_bits;
  unsigned long long total_number_of_bits = 0;
  for (int i = 0; i < DIM_R; i++) {
    number_of_bits = decompression_rice_intq2(COEFF(polyq2_output, i), compression_space, parameter,
                     position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  return total_number_of_bits;
}

unsigned long long compression_rice_intQ(uint8_t *initial_value, intQ_t sampled_integer_input,
                                          int parameter, int position) {
  intQ_t sampled_integer;
  intQ_copy(sampled_integer, sampled_integer_input);
  intQ_t sign_threshold;
  intQ_copy(sign_threshold, sign_threshold_Q);
  int sign_bit = intQ_cmp(sampled_integer, sign_threshold);
  if (sign_bit < 1)
    sign_bit = 0;
  if (sign_bit == 1) {
    intQ_neg(sampled_integer);
  }
  mpz_t sampled_integer_mpz;
  mpz_init_set_ui(sampled_integer_mpz, sampled_integer[1]);
  mpz_mul_2exp(sampled_integer_mpz, sampled_integer_mpz, 64);
  mpz_add_ui(sampled_integer_mpz, sampled_integer_mpz, sampled_integer[0]);
  mpz_t quotient;
  mpz_init(quotient);
  mpz_tdiv_q_2exp(quotient, sampled_integer_mpz, parameter);
  mpz_t number_of_bits_used;
  mpz_init(number_of_bits_used);
  mpz_add_ui(number_of_bits_used, quotient, parameter + 2);
  mpz_t remainder;
  mpz_init(remainder);
  mpz_tdiv_r_2exp(remainder, sampled_integer_mpz, parameter);
  uint8_t *compressed_value = initial_value;
  *compressed_value &= ~(0xff << position);
  *compressed_value |= (sign_bit << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
    *compressed_value = 0;
  }
  while (mpz_sgn(quotient) != 0) {
    *compressed_value |= (1 << position);
    position++;
    if (position == 8) {
      position = 0;
      compressed_value++;
      *compressed_value = 0;
    }
    mpz_sub_ui(quotient, quotient, 1);
  }
  *compressed_value &= ~(0xff << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
    *compressed_value = 0;
  }
  uint8_t remainder_least_significant_bits;
  remainder_least_significant_bits = (uint8_t) mpz_get_ui(remainder);
  *compressed_value |= remainder_least_significant_bits << position;
  mpz_tdiv_q_2exp(remainder, remainder, 8 - position);
  int remainder_remaining_bits = parameter - 8 + position;
  while (remainder_remaining_bits > 0) {
    compressed_value++;
    *compressed_value = (uint8_t) mpz_get_ui(remainder); // do not erase last bits ?
    mpz_tdiv_q_2exp(remainder, remainder, 8);
    remainder_remaining_bits -= 8;
  }
  unsigned long long number_of_bits = mpz_get_ui(number_of_bits_used);
  mpz_clear(sampled_integer_mpz);
  mpz_clear(quotient);
  mpz_clear(remainder);
  return number_of_bits;
}

unsigned long long decompression_rice_intQ(intQ_t integer, uint8_t *compressed_value, int parameter,
                           int position) {
  uint8_t sign_indication = *compressed_value & (1 << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
  }
  mpz_t quotient;
  mpz_init(quotient);
  uint8_t bit_of_quotient;
  bit_of_quotient = *compressed_value & (1 << position);
  position++;
  if (position == 8) {
    position = 0;
    compressed_value++;
  }
  while (bit_of_quotient != 0) {
    mpz_add_ui(quotient, quotient, 1);
    bit_of_quotient = *compressed_value & (1 << position);
    position++;
    if (position == 8) {
      position = 0;
      compressed_value++;
    }
  }
  unsigned long long quotient_ui = mpz_get_ui(quotient);
  int number_of_remaining_bytes = (parameter + position) / 8;
  if ((parameter + position) % 8 == 0) {
    number_of_remaining_bytes--;
  }
  compressed_value += number_of_remaining_bytes;
  mpz_t integer_mpz;
  mpz_init_set_ui(integer_mpz, *compressed_value);
  while (number_of_remaining_bytes != 0) {
    mpz_mul_2exp(integer_mpz, integer_mpz, 8);
    compressed_value--;
    mpz_add_ui(integer_mpz, integer_mpz, *compressed_value);
    number_of_remaining_bytes--;
  }
  mpz_tdiv_q_2exp(integer_mpz, integer_mpz, position);
  mpz_tdiv_r_2exp(integer_mpz, integer_mpz, parameter);
  mpz_mul_2exp(quotient, quotient, parameter);
  mpz_ior(integer_mpz, integer_mpz, quotient);
  conv_mpz_to_intQ(integer, integer_mpz);
  if (sign_indication != 0) {
    intQ_neg(integer);
  }
  return quotient_ui + parameter + 2;
}

unsigned long long compression_rice_polyQ(uint8_t *compression_space, polyQ_t poly_input,
                                           int parameter, int position) {
  unsigned long long number_of_bits;
  unsigned long long total_number_of_bits = 0;
  for (int i = 0; i < DIM_R; i++) {
    number_of_bits = compression_rice_intQ(compression_space, COEFF(poly_input, i), parameter,
                     position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  return total_number_of_bits;
}

unsigned long long decompression_rice_polyQ(polyQ_t polyQ_output, uint8_t *compression_space,
                                             int parameter, int position) {
  unsigned long long number_of_bits;
  unsigned long long total_number_of_bits = 0;
  for (int i = 0; i < DIM_R; i++) {
    number_of_bits = decompression_rice_intQ(COEFF(polyQ_output, i), compression_space, parameter,
                     position);
    number_of_bits += position;
    total_number_of_bits += number_of_bits;
    position = number_of_bits % 8;
    compression_space += number_of_bits / 8;
  }
  return total_number_of_bits;
}
