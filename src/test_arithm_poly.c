#include <stdio.h>

#include "extra_tools.h"
#include "new_arithm_int.h"
#include "new_arithm_poly.h"
#include "samplers.h"

#define OK 0
#define KO 1

//#define VERBOSE

typedef intq1_t *_double_polyq1_intern_t;
typedef _double_polyq1_intern_t _double_polyq1_t[1];
typedef intq2_t *_double_polyq2_intern_t;
typedef _double_polyq2_intern_t _double_polyq2_t[1];
typedef intQ_t *_double_polyQ_intern_t;
typedef _double_polyQ_intern_t _double_polyQ_t[1];

int test_ntt_vs_naive_multiplication_polyq1();
int _naive_polyq1_mul(polyq1_t res, polyq1_t a, polyq1_t b);
int _intq1_is_zero(intq1_t x);
void _double_polyq1_init(_double_polyq1_t polq1_p);
void _double_polyq1_clear(_double_polyq1_t polq1_p);
void _double_polyq1_add(_double_polyq1_t polq1_res,
                        const _double_polyq1_t polq1_x1,
                        const _double_polyq1_t polq1_x2);
int test_ntt_vs_naive_multiplication_polyq2();
int _naive_polyq2_mul(polyq2_t res, polyq2_t a, polyq2_t b);
int _intq2_is_zero(intq2_t x);
void _double_polyq2_init(_double_polyq2_t polq2_p);
void _double_polyq2_clear(_double_polyq2_t polq2_p);
void _double_polyq2_add(_double_polyq2_t polq2_res,
                        const _double_polyq2_t polq2_x1,
                        const _double_polyq2_t polq2_x2);
int test_ntt_vs_naive_multiplication_polyQ();
int _naive_polyQ_mul(polyQ_t res, polyQ_t a, polyQ_t b);
int _intQ_is_zero(intQ_t x);
void _double_polyQ_init(_double_polyQ_t polQ_p);
void _double_polyQ_clear(_double_polyQ_t polQ_p);
void _double_polyQ_add(_double_polyQ_t polQ_res,
                        const _double_polyQ_t polQ_x1,
                        const _double_polyQ_t polQ_x2);

int main() {
  rand_init();
  test_ntt_vs_naive_multiplication_polyq1();
  test_ntt_vs_naive_multiplication_polyq2();
  test_ntt_vs_naive_multiplication_polyQ();
  rand_clear();
  return EXIT_SUCCESS;
}

int test_ntt_vs_naive_multiplication_polyq1() {
  printf("Test: consitency of the multiplication that uses NTT algorithms\n");
  printf("with the naive multiplication (integers mod q1)…\n");
  int return_status = OK;
  polyq1_t a, b, res_naive, res_ntt;
  polyq1_init(a);
  polyq1_init(b);
  polyq1_init(res_naive);
  polyq1_init(res_ntt);
  get_random_polyq1(a);
  get_random_polyq1(b);
  _naive_polyq1_mul(res_naive, a, b);
  #ifdef VERBOSE
  printf("First factor: polynomial a, with coefficients modulo q1.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq1_print(COEFF(a, i));
    printf(" ");
  }
  printf("\n\n");
  printf("First factor: polynomial b, with coefficients modulo q1.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq1_print(COEFF(b, i));
    printf(" ");
  }
  printf("\n\n");
  printf("Result of the naive multiplication of a and b, with coefficients modulo q1.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq1_print(COEFF(res_naive, i));
    printf(" ");
  }
  printf("\n\n");
  #endif
  conv_R_polyq1_to_NTT_polyq1(a);
  conv_R_polyq1_to_NTT_polyq1(b);
  polyq1_mul_NTT(res_ntt, a, b);
  conv_NTT_polyq1_to_R_polyq1(res_ntt);
  #ifdef VERBOSE
  printf("Result of the NTT multiplication of a and b, with coefficients modulo q1.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq1_print(COEFF(res_ntt, i));
    printf(" ");
  }
  printf("\n");
  #endif
  for (int i = 0; i < DIM_R; i++) {
    if(intq1_cmp(COEFF(res_naive, i), COEFF(res_ntt, i)) != 0) {
      return_status = KO;
      break;
    }
  }
  if (return_status == OK) {
    printf("OK!\n\n");
  }
  else {
    printf("ERROR!\n\n");
  }
  polyq1_clear(a);
  polyq1_clear(b);
  polyq1_clear(res_naive);
  polyq1_clear(res_ntt);
  return return_status;
}

int _naive_polyq1_mul(polyq1_t res, polyq1_t a, polyq1_t b) {
  polyq1_t a_index_i_times_b;
  polyq1_init(a_index_i_times_b);
  _double_polyq1_t product_before_reduction;
  _double_polyq1_init(product_before_reduction);
  _double_polyq1_t shifted_a_index_i_times_b;
  _double_polyq1_init(shifted_a_index_i_times_b);
  for (int i = 0; i < DIM_R; i++) {
    if (!_intq1_is_zero(COEFF(a, i))) {
      polyq1_scalar_mul(a_index_i_times_b, b, COEFF(a, i));
      for (int j = 0; j < 2 * DIM_R; j++) {
        intq1_zero(COEFF(shifted_a_index_i_times_b, j));
      }
      for (int j = 0; j < DIM_R; j++) {
        intq1_copy(COEFF(shifted_a_index_i_times_b, j + i),
                   COEFF(a_index_i_times_b, j));
      }
      _double_polyq1_add(product_before_reduction, product_before_reduction,
                         shifted_a_index_i_times_b);
    }
  }
  for (int i = 0; i < DIM_R; i++) {
    intq1_neg(COEFF(product_before_reduction, i + DIM_R));
    intq1_add(COEFF(res, i), COEFF(product_before_reduction, i),
              COEFF(product_before_reduction, i + DIM_R));
  }
  return OK;
}

int _intq1_is_zero(intq1_t x) {
  if (x[0] == (uint64_t)0) {
    return 1;
  } else {
    return 0;
  }
}

void _double_polyq1_init(_double_polyq1_t polq1_p) {
  *polq1_p = (intq1_t *)calloc(2 * DIM_R, sizeof(intq1_t));
  if (polq1_p == NULL) {
    printf("ERROR : calloc of polyq1_init didn't work\n");
    exit(EXIT_FAILURE);
  }
}

void _double_polyq1_clear(_double_polyq1_t polq1_p) { free(*polq1_p); }

void _double_polyq1_add(_double_polyq1_t polq1_res,
                        const _double_polyq1_t polq1_x1,
                        const _double_polyq1_t polq1_x2) {
  for (int i = 0; i < 2 * DIM_R; i++) {
    intq1_add(COEFF(polq1_res, i), COEFF(polq1_x1, i), COEFF(polq1_x2, i));
  }
}

int test_ntt_vs_naive_multiplication_polyq2() {
  printf("Test: consitency of the multiplication that uses NTT algorithms\n");
  printf("with the naive multiplication (integers mod q2)…\n");
  int return_status = OK;
  polyq2_t a, b, res_naive, res_ntt;
  polyq2_init(a);
  polyq2_init(b);
  polyq2_init(res_naive);
  polyq2_init(res_ntt);
  get_random_polyq2(a);
  get_random_polyq2(b);
  _naive_polyq2_mul(res_naive, a, b);
  #ifdef VERBOSE
  printf("First factor: polynomial a, with coefficients modulo q2.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq2_print(COEFF(a, i));
    printf(" ");
  }
  printf("\n\n");
  printf("First factor: polynomial b, with coefficients modulo q2.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq2_print(COEFF(b, i));
    printf(" ");
  }
  printf("\n\n");
  printf("Result of the naive multiplication of a and b, with coefficients modulo q2.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq2_print(COEFF(res_naive, i));
    printf(" ");
  }
  printf("\n\n");
  #endif
  conv_R_polyq2_to_NTT_polyq2(a);
  conv_R_polyq2_to_NTT_polyq2(b);
  polyq2_mul_NTT(res_ntt, a, b);
  conv_NTT_polyq2_to_R_polyq2(res_ntt);
  #ifdef VERBOSE
  printf("Result of the NTT multiplication of a and b, with coefficients modulo q2.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intq2_print(COEFF(res_ntt, i));
    printf(" ");
  }
  printf("\n");
  #endif
  for (int i = 0; i < DIM_R; i++) {
    if(intq2_cmp(COEFF(res_naive, i), COEFF(res_ntt, i)) != 0) {
      return_status = KO;
      break;
    }
  }
  if (return_status == OK) {
    printf("OK!\n\n");
  }
  else {
    printf("ERROR!\n\n");
  }
  polyq2_clear(a);
  polyq2_clear(b);
  polyq2_clear(res_naive);
  polyq2_clear(res_ntt);
  return return_status;
}

int _naive_polyq2_mul(polyq2_t res, polyq2_t a, polyq2_t b) {
  polyq2_t a_index_i_times_b;
  polyq2_init(a_index_i_times_b);
  _double_polyq2_t product_before_reduction;
  _double_polyq2_init(product_before_reduction);
  _double_polyq2_t shifted_a_index_i_times_b;
  _double_polyq2_init(shifted_a_index_i_times_b);
  for (int i = 0; i < DIM_R; i++) {
    if (!_intq2_is_zero(COEFF(a, i))) {
      polyq2_scalar_mul(a_index_i_times_b, b, COEFF(a, i));
      for (int j = 0; j < 2 * DIM_R; j++) {
        intq2_zero(COEFF(shifted_a_index_i_times_b, j));
      }
      for (int j = 0; j < DIM_R; j++) {
        intq2_copy(COEFF(shifted_a_index_i_times_b, j + i),
                   COEFF(a_index_i_times_b, j));
      }
      _double_polyq2_add(product_before_reduction, product_before_reduction,
                         shifted_a_index_i_times_b);
    }
  }
  for (int i = 0; i < DIM_R; i++) {
    intq2_neg(COEFF(product_before_reduction, i + DIM_R));
    intq2_add(COEFF(res, i), COEFF(product_before_reduction, i),
              COEFF(product_before_reduction, i + DIM_R));
  }
  return OK;
}

int _intq2_is_zero(intq2_t x) {
  if (x[0] == (uint64_t)0) {
    return 1;
  } else {
    return 0;
  }
}

void _double_polyq2_init(_double_polyq2_t polq2_p) {
  *polq2_p = (intq2_t *)calloc(2 * DIM_R, sizeof(intq2_t));
  if (polq2_p == NULL) {
    printf("ERROR : calloc of polyq2_init didn't work\n");
    exit(EXIT_FAILURE);
  }
}

void _double_polyq2_clear(_double_polyq2_t polq2_p) { free(*polq2_p); }

void _double_polyq2_add(_double_polyq2_t polq2_res,
                        const _double_polyq2_t polq2_x1,
                        const _double_polyq2_t polq2_x2) {
  for (int i = 0; i < 2 * DIM_R; i++) {
    intq2_add(COEFF(polq2_res, i), COEFF(polq2_x1, i), COEFF(polq2_x2, i));
  }
}

int test_ntt_vs_naive_multiplication_polyQ() {
  printf("Test: consitency of the multiplication that uses NTT algorithms\n");
  printf("with the naive multiplication (integers mod Q)…\n");
  int return_status = OK;
  polyQ_t a, b, res_naive, res_ntt;
  polyQ_init(a);
  polyQ_init(b);
  polyQ_init(res_naive);
  polyQ_init(res_ntt);
  get_random_polyQ(a);
  get_random_polyQ(b);
  _naive_polyQ_mul(res_naive, a, b);
  #ifdef VERBOSE
  printf("First factor: polynomial a, with coefficients modulo Q.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intQ_print(COEFF(a, i));
    printf(" ");
  }
  printf("\n\n");
  printf("First factor: polynomial b, with coefficients modulo Q.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intQ_print(COEFF(b, i));
    printf(" ");
  }
  printf("\n\n");
  printf("Result of the naive multiplication of a and b, with coefficients modulo Q.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intQ_print(COEFF(res_naive, i));
    printf(" ");
  }
  printf("\n\n");
  #endif
  conv_R_polyQ_to_NTT_polyQ(a);
  conv_R_polyQ_to_NTT_polyQ(b);
  polyQ_mul_NTT(res_ntt, a, b);
  conv_NTT_polyQ_to_R_polyQ(res_ntt);
  #ifdef VERBOSE
  printf("Result of the NTT multiplication of a and b, with coefficients modulo Q.\n");
  printf("From index 0 to index 8191.\n");
  for (int i = 0; i < DIM_R; i++) {
    intQ_print(COEFF(res_ntt, i));
    printf(" ");
  }
  printf("\n");
  #endif
  for (int i = 0; i < DIM_R; i++) {
    if(intQ_cmp(COEFF(res_naive, i), COEFF(res_ntt, i)) != 0) {
      return_status = KO;
      break;
    }
  }
  if (return_status == OK) {
    printf("OK!\n\n");
  }
  else {
    printf("ERROR!\n\n");
  }
  polyQ_clear(a);
  polyQ_clear(b);
  polyQ_clear(res_naive);
  polyQ_clear(res_ntt);
  return return_status;
}

int _naive_polyQ_mul(polyQ_t res, polyQ_t a, polyQ_t b) {
  polyQ_t a_index_i_times_b;
  polyQ_init(a_index_i_times_b);
  _double_polyQ_t product_before_reduction;
  _double_polyQ_init(product_before_reduction);
  _double_polyQ_t shifted_a_index_i_times_b;
  _double_polyQ_init(shifted_a_index_i_times_b);
  for (int i = 0; i < DIM_R; i++) {
    if (!_intQ_is_zero(COEFF(a, i))) {
      polyQ_scalar_mul(a_index_i_times_b, b, COEFF(a, i));
      for (int j = 0; j < 2 * DIM_R; j++) {
        intQ_zero(COEFF(shifted_a_index_i_times_b, j));
      }
      for (int j = 0; j < DIM_R; j++) {
        intQ_copy(COEFF(shifted_a_index_i_times_b, j + i),
                   COEFF(a_index_i_times_b, j));
      }
      _double_polyQ_add(product_before_reduction, product_before_reduction,
                         shifted_a_index_i_times_b);
    }
  }
  for (int i = 0; i < DIM_R; i++) {
    intQ_neg(COEFF(product_before_reduction, i + DIM_R));
    intQ_add(COEFF(res, i), COEFF(product_before_reduction, i),
              COEFF(product_before_reduction, i + DIM_R));
  }
  return OK;
}

int _intQ_is_zero(intQ_t x) {
  if (x[0] == (uint64_t)0) {
    return 1;
  } else {
    return 0;
  }
}

void _double_polyQ_init(_double_polyQ_t polQ_p) {
  *polQ_p = (intQ_t *)calloc(2 * DIM_R, sizeof(intQ_t));
  if (polQ_p == NULL) {
    printf("ERROR : calloc of polyQ_init didn't work\n");
    exit(EXIT_FAILURE);
  }
}

void _double_polyQ_clear(_double_polyQ_t polQ_p) { free(*polQ_p); }

void _double_polyQ_add(_double_polyQ_t polQ_res,
                        const _double_polyQ_t polQ_x1,
                        const _double_polyQ_t polQ_x2) {
  for (int i = 0; i < 2 * DIM_R; i++) {
    intQ_add(COEFF(polQ_res, i), COEFF(polQ_x1, i), COEFF(polQ_x2, i));
  }
}
