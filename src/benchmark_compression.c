#include <assert.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "API.h"
#include "extra_tools.h"
#include "new_arithm_int.h"
#include "new_arithm_poly.h"
#include "params.h"
#include "protocol.h"
#include "samplers.h"
#include "shake.h"

#ifdef BENCHMARK
#define NUMBER_OF_REPETITIONS 10 
#endif

int main() {
  uint8_t * compression_space = malloc(16000000 * sizeof(uint8_t));
  attribute_t attr = 4;
  message_t message;
  intq2_t intq2_id = {3, 0, 0};
  s_issuer_keys_t ikeys;
  s_opener_keys_t okeys;
  s_credential_t cred;
  s_presentation_t pres;
  //intq2_set_ui(intq2_id, 3);
  for(int i = 0; i < SIZE_MESS_S8; i++)
    {
      message[i] = 1;
    }
  unsigned long long benchmark_sizes[NUMBER_OF_REPETITIONS][23];
  unsigned long long benchmark_presentation_sizes[NUMBER_OF_REPETITIONS];
  unsigned long long total_number_of_bits;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    protocol_init();
    issuer_parameters_init(ikeys);
    opener_parameters_init(okeys);
    issuer_parameters_generation(ikeys);
    opener_parameters_generation(okeys);
    credential_init(cred);
    credential_generation(cred, ikeys, intq2_id, attr);
    assert(credential_verify(cred, ikeys->pk, intq2_id, attr));
    presentation_init(pres);
    presentation_generation(pres, cred, intq2_id, ikeys->pk, okeys->pk, attr, message);
    total_number_of_bits = compression_presentation(compression_space, pres);
    benchmark_presentation_sizes[i] = total_number_of_bits;
    for(int j = 0; j < 23; j++) {
      benchmark_sizes[i][j] = compressed_poly_sizes[j];
    }
    s_presentation_t decompressed_presentation;
    presentation_init(decompressed_presentation);
    total_number_of_bits = decompression_presentation(decompressed_presentation, compression_space);


    assert(presentation_verify(decompressed_presentation, ikeys->pk, okeys->pk, attr, message));
    presentation_clear(pres);
    issuer_parameters_clear(ikeys);
    opener_parameters_clear(okeys);
    credential_clear(cred);
    protocol_clear();
    printf(".");
    fflush(stdout);
  }
  printf("\n");
  printf("Average size of the compressed polynomials of the presentation:\n");

  unsigned long long sum_sizes_xi_q1 = 0;
  unsigned long long min_size_xi_q1 = 0xffffffffffffffff;
  unsigned long long max_size_xi_q1 = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    for(int j = 0; j < 12; j++) {
      sum_sizes_xi_q1 += benchmark_sizes[i][j];
      if (benchmark_sizes[i][j] < min_size_xi_q1) {
        min_size_xi_q1 = benchmark_sizes[i][j];
      }
      if (benchmark_sizes[i][j] > max_size_xi_q1) {
        max_size_xi_q1 = benchmark_sizes[i][j];
      }
    }
  }
  unsigned long long average_size_xi_q1 = sum_sizes_xi_q1 / (12 * NUMBER_OF_REPETITIONS * 8) + 1;
  printf("%-90s", "- with coefficients modulo q1, sampled with the standard deviation xi: ");
  printf("%7llu bytes.\n", average_size_xi_q1);

  unsigned long long sum_sizes_xi_Q = 0;
  unsigned long long min_size_xi_Q = 0xffffffffffffffff;
  unsigned long long max_size_xi_Q = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    for(int j = 12; j < 17; j++) {
      sum_sizes_xi_Q += benchmark_sizes[i][j];
      if (benchmark_sizes[i][j] < min_size_xi_Q) {
        min_size_xi_Q = benchmark_sizes[i][j];
      }
      if (benchmark_sizes[i][j] > max_size_xi_Q) {
        max_size_xi_Q = benchmark_sizes[i][j];
      }
    }
  }
  unsigned long long average_size_xi_Q = sum_sizes_xi_Q / (5 * NUMBER_OF_REPETITIONS * 8) + 1;
  printf("%-90s", "- with coefficients modulo Q, sampled with the standard deviation xi: ");
  printf("%7llu bytes.\n", average_size_xi_Q);

  unsigned long long sum_sizes_xi1_q2 = 0;
  unsigned long long min_size_xi1_q2 = 0xffffffffffffffff;
  unsigned long long max_size_xi1_q2 = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    for(int j = 17; j < 21; j++) {
      sum_sizes_xi1_q2 += benchmark_sizes[i][j];
      if (benchmark_sizes[i][j] < min_size_xi1_q2) {
        min_size_xi1_q2 = benchmark_sizes[i][j];
      }
      if (benchmark_sizes[i][j] > max_size_xi1_q2) {
        max_size_xi1_q2 = benchmark_sizes[i][j];
      }
    }
  }
  unsigned long long average_size_xi1_q2 = sum_sizes_xi1_q2 / (4 * NUMBER_OF_REPETITIONS * 8) + 1;
  printf("%-90s", "- with coefficients modulo q2, sampled with the standard deviation xi1: ");
  printf("%7llu bytes.\n", average_size_xi1_q2);


  unsigned long long sum_sizes_xi2_q2 = 0;
  unsigned long long min_size_xi2_q2 = 0xffffffffffffffff;
  unsigned long long max_size_xi2_q2 = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    for(int j = 21; j < 23; j++) {
      sum_sizes_xi2_q2 += benchmark_sizes[i][j];
      if (benchmark_sizes[i][j] < min_size_xi2_q2) {
        min_size_xi2_q2 = benchmark_sizes[i][j];
      }
      if (benchmark_sizes[i][j] > max_size_xi2_q2) {
        max_size_xi2_q2 = benchmark_sizes[i][j];
      }
    }
  }
  unsigned long long average_size_xi2_q2 = sum_sizes_xi2_q2 / (2 * NUMBER_OF_REPETITIONS * 8) + 1;
  printf("%-90s", "- with coefficients modulo q2, sampled with the standard deviation xi2: ");
  printf("%7llu bytes.\n", average_size_xi2_q2);


  printf("\n");
  printf("Minimum size of the compressed polynomials of the presentation:\n");
  printf("%-90s", "- with coefficients modulo q1, sampled with the standard deviation xi: ");
  printf("%7llu bytes.\n", min_size_xi_q1 / 8 + 1);
  printf("%-90s", "- with coefficients modulo Q, sampled with the standard deviation xi: ");
  printf("%7llu bytes.\n", min_size_xi_Q / 8 + 1);
  printf("%-90s", "- with coefficients modulo q2, sampled with the standard deviation xi1: ");
  printf("%7llu bytes.\n", min_size_xi1_q2 / 8 + 1);
  printf("%-90s", "- with coefficients modulo q2, sampled with the standard deviation xi2: ");
  printf("%7llu bytes.\n", min_size_xi2_q2 / 8 + 1);


  printf("\n");
  printf("Maximum size of the compressed polynomials of the presentation:\n");
  printf("%-90s", "- with coefficients modulo q1, sampled with the standard deviation xi: ");
  printf("%7llu bytes.\n", max_size_xi_q1 / 8 + 1);
  printf("%-90s", "- with coefficients modulo Q, sampled with the standard deviation xi: ");
  printf("%7llu bytes.\n", max_size_xi_Q / 8 + 1);
  printf("%-90s", "- with coefficients modulo q2, sampled with the standard deviation xi1: ");
  printf("%7llu bytes.\n", max_size_xi1_q2 / 8 + 1);
  printf("%-90s", "- with coefficients modulo q2, sampled with the standard deviation xi2: ");
  printf("%7llu bytes.\n", max_size_xi2_q2 / 8 + 1);


  unsigned long long sum_sizes_presentation = 0;
  unsigned long long min_size_presentation = 0xffffffffffffffff;
  unsigned long long max_size_presentation = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    sum_sizes_presentation += benchmark_presentation_sizes[i];
    if (benchmark_presentation_sizes[i] < min_size_presentation) {
      min_size_presentation = benchmark_presentation_sizes[i];
    }
    if (benchmark_presentation_sizes[i] > max_size_presentation) {
      max_size_presentation = benchmark_presentation_sizes[i];
    }
  }
  unsigned long long average_size_presentation = sum_sizes_presentation / (NUMBER_OF_REPETITIONS * 8) + 1;
  printf("\n");
  printf("%-90s", "Average size of a compressed presentation: ");
  printf("%7llu bytes.\n", average_size_presentation);
  printf("%-90s", "Minimum size of the compressed presentations: ");
  printf("%7llu bytes.\n", min_size_presentation / 8 + 1);
  printf("%-90s", "Maximum size of the compressed presentations: ");
  printf("%7llu bytes.\n", max_size_presentation / 8 + 1);


  unsigned long long sum_sizes_xi;
  unsigned long long sum_sum_sizes_xi = 0;
  unsigned long long min_sum_sizes_xi = 0xffffffffffffffff;
  unsigned long long max_sum_sizes_xi = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    sum_sizes_xi = 0;
    for(int j = 0; j < 17; j++) {
      sum_sizes_xi += benchmark_sizes[i][j];
    }
    sum_sum_sizes_xi += sum_sizes_xi;
    if (sum_sizes_xi < min_sum_sizes_xi) {
      min_sum_sizes_xi = sum_sizes_xi;
    }
    if (sum_sizes_xi > max_sum_sizes_xi) {
      max_sum_sizes_xi = sum_sizes_xi;
    }
  }
  unsigned long long average_sum_sizes_xi = sum_sum_sizes_xi / (NUMBER_OF_REPETITIONS * 8) + 1;
  printf("\n");
  printf("%-90s", "Average size of the block of elements sampled with xi in the compressed presentation: ");
  printf("%7llu bytes.\n", average_sum_sizes_xi);
  printf("%-90s", "Minimum size of the blocks of elements sampled with xi in the compressed presentation: ");
  printf("%7llu bytes.\n", min_sum_sizes_xi / 8 + 1);
  printf("%-90s", "Maximum size of the blocks of elements sampled with xi in the compressed presentation: ");
  printf("%7llu bytes.\n", max_sum_sizes_xi / 8 + 1);

  unsigned long long sum_sizes_xi1;
  unsigned long long sum_sum_sizes_xi1 = 0;
  unsigned long long min_sum_sizes_xi1 = 0xffffffffffffffff;
  unsigned long long max_sum_sizes_xi1 = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    sum_sizes_xi1 = 0;
    for(int j = 17; j < 21; j++) {
      sum_sizes_xi1 += benchmark_sizes[i][j];
    }
    sum_sum_sizes_xi1 += sum_sizes_xi1;
    if (sum_sizes_xi1 < min_sum_sizes_xi1) {
      min_sum_sizes_xi1 = sum_sizes_xi1;
    }
    if (sum_sizes_xi1 > max_sum_sizes_xi1) {
      max_sum_sizes_xi1 = sum_sizes_xi1;
    }
  }
  unsigned long long average_sum_sizes_xi1 = sum_sum_sizes_xi1 / (NUMBER_OF_REPETITIONS * 8) + 1;
  printf("\n");
  printf("%-90s", "Average size of the block of elements sampled with xi1 in the compressed presentation: ");
  printf("%7llu bytes.\n", average_sum_sizes_xi1);
  printf("%-90s", "Minimum size of the blocks of elements sampled with xi1 in the compressed presentation: ");
  printf("%7llu bytes.\n", min_sum_sizes_xi1 / 8 + 1);
  printf("%-90s", "Maximum size of the blocks of elements sampled with xi1 in the compressed presentation: ");
  printf("%7llu bytes.\n", max_sum_sizes_xi1 / 8 + 1);

  unsigned long long sum_sizes_xi2;
  unsigned long long sum_sum_sizes_xi2 = 0;
  unsigned long long min_sum_sizes_xi2 = 0xffffffffffffffff;
  unsigned long long max_sum_sizes_xi2 = 0;
  for (int i = 0; i < NUMBER_OF_REPETITIONS; i++) {
    sum_sizes_xi2 = 0;
    for(int j = 21; j < 23; j++) {
      sum_sizes_xi2 += benchmark_sizes[i][j];
    }
    sum_sum_sizes_xi2 += sum_sizes_xi2;
    if (sum_sizes_xi2 < min_sum_sizes_xi2) {
      min_sum_sizes_xi2 = sum_sizes_xi2;
    }
    if (sum_sizes_xi2 > max_sum_sizes_xi2) {
      max_sum_sizes_xi2 = sum_sizes_xi2;
    }
  }
  unsigned long long average_sum_sizes_xi2 = sum_sum_sizes_xi2 / (NUMBER_OF_REPETITIONS * 8) + 1;
  printf("\n");
  printf("%-90s", "Average size of the block of elements sampled with xi2 in the compressed presentation: ");
  printf("%7llu bytes.\n", average_sum_sizes_xi2);
  printf("%-90s", "Minimum size of the blocks of elements sampled with xi2 in the compressed presentation: ");
  printf("%7llu bytes.\n", min_sum_sizes_xi2 / 8 + 1);
  printf("%-90s", "Maximum size of the blocks of elements sampled with xi2 in the compressed presentation: ");
  printf("%7llu bytes.\n", max_sum_sizes_xi2 / 8 + 1);


  printf("\n");
  free(compression_space);
  return EXIT_SUCCESS;
}
