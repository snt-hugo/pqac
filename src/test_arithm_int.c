#define OK 0
#define KO 1
#define VERBOSE

#include <stdio.h>

#include "extra_tools.h"
#include "new_arithm_int.h"
#include "params.h"
#include "samplers.h"

#define PRINT_NAME_TEST  printf("**** %s ****\n" , __FUNCTION__)

int test_associative_property_addition();
int test_associative_property_multiplication();
int test_basic_ring_properties(); 

	int main() {
	  rand_init();
	  test_associative_property_addition();
	  test_associative_property_multiplication();
	  test_basic_ring_properties(); 
	  rand_clear();
	  return EXIT_SUCCESS;
	}

int test_associative_property_addition() {
  PRINT_NAME_TEST;

  intq1_t a_q1, b_q1, c_q1, a_plus_b_q1, b_plus_c_q1, sum_a_plus_b_and_c_q1,
      sum_a_and_b_plus_c_q1;
  get_random_intq1(a_q1);
  get_random_intq1(b_q1);
  get_random_intq1(c_q1);
  intq1_add(a_plus_b_q1, a_q1, b_q1);
  intq1_add(sum_a_plus_b_and_c_q1, a_plus_b_q1, c_q1);
  intq1_add(b_plus_c_q1, b_q1, c_q1);
  intq1_add(sum_a_and_b_plus_c_q1, a_q1, b_plus_c_q1);
  int res_cmp_q1 = intq1_cmp(sum_a_plus_b_and_c_q1, sum_a_and_b_plus_c_q1);

  #ifdef VERBOSE
  printf("a = ");
  intq1_print(a_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("b = ");
  intq1_print(b_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("c = ");
  intq1_print(c_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("(a + b) + c = ");
  intq1_print(sum_a_plus_b_and_c_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("a + (b + c) = ");
  intq1_print(sum_a_and_b_plus_c_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  if(res_cmp_q1 != 0) {
    printf("Error: Result non-compliant with the associative property of the addition, modulo q1.\n");
  }
  if(res_cmp_q1 == 0) {
    printf("OK: Result compliant with the associative property of the addition, modulo q1.\n");
  }
  printf("\n");
  #endif

  intq2_t a_q2, b_q2, c_q2, a_plus_b_q2, b_plus_c_q2, sum_a_plus_b_and_c_q2,
      sum_a_and_b_plus_c_q2;
  get_random_intq2(a_q2);
  get_random_intq2(b_q2);
  get_random_intq2(c_q2);
  intq2_add(a_plus_b_q2, a_q2, b_q2);
  intq2_add(sum_a_plus_b_and_c_q2, a_plus_b_q2, c_q2);
  intq2_add(b_plus_c_q2, b_q2, c_q2);
  intq2_add(sum_a_and_b_plus_c_q2, a_q2, b_plus_c_q2);
  int res_cmp_q2 = intq2_cmp(sum_a_plus_b_and_c_q2, sum_a_and_b_plus_c_q2);

  #ifdef VERBOSE
  printf("a = ");
  intq2_print(a_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("b = ");
  intq2_print(b_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("c = ");
  intq2_print(c_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("(a + b) + c = ");
  intq2_print(sum_a_plus_b_and_c_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("a + (b + c) = ");
  intq2_print(sum_a_and_b_plus_c_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  if(res_cmp_q2 != 0) {
    printf("Error: Result non-compliant with the associative property of the addition, modulo q2.\n");
  }
  if(res_cmp_q2 == 0) {
    printf("OK: Result compliant with the associative property of the addition, modulo q2.\n");
  }
  printf("\n");
  #endif

  intQ_t a_Q, b_Q, c_Q, a_plus_b_Q, b_plus_c_Q, sum_a_plus_b_and_c_Q,
      sum_a_and_b_plus_c_Q;
  get_random_intQ(a_Q);
  get_random_intQ(b_Q);
  get_random_intQ(c_Q);
  intQ_add(a_plus_b_Q, a_Q, b_Q);
  intQ_add(sum_a_plus_b_and_c_Q, a_plus_b_Q, c_Q);
  intQ_add(b_plus_c_Q, b_Q, c_Q);
  intQ_add(sum_a_and_b_plus_c_Q, a_Q, b_plus_c_Q);
  int res_cmp_Q = intQ_cmp(sum_a_plus_b_and_c_Q, sum_a_and_b_plus_c_Q);

  #ifdef VERBOSE
  printf("a = ");
  intQ_print(a_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("b = ");
  intQ_print(b_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("c = ");
  intQ_print(c_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("(a + b) + c = ");
  intQ_print(sum_a_plus_b_and_c_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("a + (b + c) = ");
  intQ_print(sum_a_and_b_plus_c_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  if(res_cmp_Q != 0) {
    printf("Error: Result non-compliant with the associative property of the addition, modulo Q.\n");
  }
  if(res_cmp_Q == 0) {
    printf("OK: Result compliant with the associative property of the addition, modulo Q.\n");
  }
  printf("\n");
  #endif

  if(res_cmp_q1 != 0 || res_cmp_q2 != 0 || res_cmp_Q != 0) {
    printf("ERROR!\n");
    return KO;
  }
  printf("OK!\n");
  return OK;
}


int test_associative_property_multiplication() {
  PRINT_NAME_TEST;

  intq1_t a_q1, b_q1, c_q1, a_times_b_q1, b_times_c_q1, product_a_times_b_and_c_q1,
      product_a_and_b_times_c_q1;
  get_random_intq1(a_q1);
  get_random_intq1(b_q1);
  get_random_intq1(c_q1);
  intq1_add(a_times_b_q1, a_q1, b_q1);
  intq1_add(product_a_times_b_and_c_q1, a_times_b_q1, c_q1);
  intq1_add(b_times_c_q1, b_q1, c_q1);
  intq1_add(product_a_and_b_times_c_q1, a_q1, b_times_c_q1);
  int res_cmp_q1 = intq1_cmp(product_a_times_b_and_c_q1, product_a_and_b_times_c_q1);

  #ifdef VERBOSE
  printf("a = ");
  intq1_print(a_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("b = ");
  intq1_print(b_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("c = ");
  intq1_print(c_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("(a.b).c = ");
  intq1_print(product_a_times_b_and_c_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("a.(b.c) = ");
  intq1_print(product_a_and_b_times_c_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  if(res_cmp_q1 != 0) {
    printf("Error: Result non-compliant with the associative property of the multiplication, modulo q1.\n");
  }
  if(res_cmp_q1 == 0) {
    printf("OK: Result compliant with the associative property of the multiplication, modulo q1.\n");
  }
  printf("\n");
  #endif

  intq2_t a_q2, b_q2, c_q2, a_times_b_q2, b_times_c_q2, product_a_times_b_and_c_q2,
      product_a_and_b_times_c_q2;
  get_random_intq2(a_q2);
  get_random_intq2(b_q2);
  get_random_intq2(c_q2);
  intq2_add(a_times_b_q2, a_q2, b_q2);
  intq2_add(product_a_times_b_and_c_q2, a_times_b_q2, c_q2);
  intq2_add(b_times_c_q2, b_q2, c_q2);
  intq2_add(product_a_and_b_times_c_q2, a_q2, b_times_c_q2);
  int res_cmp_q2 = intq2_cmp(product_a_times_b_and_c_q2, product_a_and_b_times_c_q2);

  #ifdef VERBOSE
  printf("a = ");
  intq2_print(a_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("b = ");
  intq2_print(b_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("c = ");
  intq2_print(c_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("(a.b).c = ");
  intq2_print(product_a_times_b_and_c_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("a.(b.c) = ");
  intq2_print(product_a_and_b_times_c_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  if(res_cmp_q2 != 0) {
    printf("Error: Result non-compliant with the associative property of the multiplication, modulo q2.\n");
  }
  if(res_cmp_q2 == 0) {
    printf("OK: Result compliant with the associative property of the multiplication, modulo q2.\n");
  }
  printf("\n");
  #endif

  intQ_t a_Q, b_Q, c_Q, a_times_b_Q, b_times_c_Q, product_a_times_b_and_c_Q,
      product_a_and_b_times_c_Q;
  get_random_intQ(a_Q);
  get_random_intQ(b_Q);
  get_random_intQ(c_Q);
  intQ_add(a_times_b_Q, a_Q, b_Q);
  intQ_add(product_a_times_b_and_c_Q, a_times_b_Q, c_Q);
  intQ_add(b_times_c_Q, b_Q, c_Q);
  intQ_add(product_a_and_b_times_c_Q, a_Q, b_times_c_Q);
  int res_cmp_Q = intQ_cmp(product_a_times_b_and_c_Q, product_a_and_b_times_c_Q);

  #ifdef VERBOSE
  printf("a = ");
  intQ_print(a_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("b = ");
  intQ_print(b_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("c = ");
  intQ_print(c_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("(a.b).c = ");
  intQ_print(product_a_times_b_and_c_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("a.(b.c) = ");
  intQ_print(product_a_and_b_times_c_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  if(res_cmp_Q != 0) {
    printf("Error: Result non-compliant with the associative property of the multiplication, modulo Q.\n");
  }
  if(res_cmp_Q == 0) {
    printf("OK: Result compliant with the associative property of the multiplication, modulo Q.\n");
  }
  printf("\n");
  #endif

  if(res_cmp_q1 != 0 || res_cmp_q2 != 0 || res_cmp_Q != 0) {
    printf("ERROR!\n");
    return KO;
  }
  printf("OK!\n");
  return OK;
}


int test_basic_ring_properties() {
  PRINT_NAME_TEST;

  intq1_t a_q1, b_q1, minus_one_q1, a_plus_b_q1, minus_one_times_sum_a_and_b_q1, minus_a_q1, minus_a_minus_b_q1;
  get_random_intq1(a_q1);
  get_random_intq1(b_q1);
  intq1_set_ui(minus_one_q1, 1);
  intq1_neg(minus_one_q1);
  intq1_add(a_plus_b_q1, a_q1, b_q1);
  intq1_mul(minus_one_times_sum_a_and_b_q1, minus_one_q1, a_plus_b_q1);
  intq1_copy(minus_a_q1, a_q1);
  intq1_neg(minus_a_q1);
  intq1_sub(minus_a_minus_b_q1, minus_a_q1, b_q1);
  int res_cmp_q1 = intq1_cmp(minus_one_times_sum_a_and_b_q1, minus_a_minus_b_q1);

  #ifdef VERBOSE
  printf("a = ");
  intq1_print(a_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("b = ");
  intq1_print(b_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("(-1).(a + b) = ");
  intq1_print(minus_one_times_sum_a_and_b_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  printf("-a - b = ");
  intq1_print(minus_a_minus_b_q1);
  printf(" mod ");
  intq1_print(g_intq1_q1);
  printf("\n");
  if(res_cmp_q1 != 0) {
    printf("Error: (-1).(a + b) different from -a - b, modulo q1.\n");
  }
  if(res_cmp_q1 == 0) {
    printf("OK: (-1).(a + b) equal to -a - b, modulo q1.\n");
  }
  printf("\n");
  #endif

  intq2_t a_q2, b_q2, minus_one_q2, a_plus_b_q2, minus_one_times_sum_a_and_b_q2, minus_a_q2, minus_a_minus_b_q2;
  get_random_intq2(a_q2);
  get_random_intq2(b_q2);
  intq2_set_ui(minus_one_q2, 1);
  intq2_neg(minus_one_q2);
  intq2_add(a_plus_b_q2, a_q2, b_q2);
  intq2_mul(minus_one_times_sum_a_and_b_q2, minus_one_q2, a_plus_b_q2);
  intq2_copy(minus_a_q2, a_q2);
  intq2_neg(minus_a_q2);
  intq2_sub(minus_a_minus_b_q2, minus_a_q2, b_q2);
  int res_cmp_q2 = intq2_cmp(minus_one_times_sum_a_and_b_q2, minus_a_minus_b_q2);

  #ifdef VERBOSE
  printf("a = ");
  intq2_print(a_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("b = ");
  intq2_print(b_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("(-1).(a + b) = ");
  intq2_print(minus_one_times_sum_a_and_b_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  printf("-a - b = ");
  intq2_print(minus_a_minus_b_q2);
  printf(" mod ");
  intq2_print(g_intq2_q2);
  printf("\n");
  if(res_cmp_q2 != 0) {
    printf("Error: (-1).(a + b) different from -a - b, modulo q2.\n");
  }
  if(res_cmp_q2 == 0) {
    printf("OK: (-1).(a + b) equal to -a - b, modulo q2.\n");
  }
  printf("\n");
  #endif

  intQ_t a_Q, b_Q, minus_one_Q, a_plus_b_Q, minus_one_times_sum_a_and_b_Q, minus_a_Q, minus_a_minus_b_Q;
  get_random_intQ(a_Q);
  get_random_intQ(b_Q);
  intQ_set_ui(minus_one_Q, 1);
  intQ_neg(minus_one_Q);
  intQ_add(a_plus_b_Q, a_Q, b_Q);
  intQ_mul(minus_one_times_sum_a_and_b_Q, minus_one_Q, a_plus_b_Q);
  intQ_copy(minus_a_Q, a_Q);
  intQ_neg(minus_a_Q);
  intQ_sub(minus_a_minus_b_Q, minus_a_Q, b_Q);
  int res_cmp_Q = intQ_cmp(minus_one_times_sum_a_and_b_Q, minus_a_minus_b_Q);

  #ifdef VERBOSE
  printf("a = ");
  intQ_print(a_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("b = ");
  intQ_print(b_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("(-1).(a + b) = ");
  intQ_print(minus_one_times_sum_a_and_b_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  printf("-a - b = ");
  intQ_print(minus_a_minus_b_Q);
  printf(" mod ");
  intQ_print(g_intQ_Q);
  printf("\n");
  if(res_cmp_Q != 0) {
    printf("Error: (-1).(a + b) different from -a - b, modulo Q.\n");
  }
  if(res_cmp_Q == 0) {
    printf("OK: (-1).(a + b) equal to -a - b, modulo Q.\n");
  }
  printf("\n");
  #endif

  if(res_cmp_q1 != 0 || res_cmp_q2 != 0 || res_cmp_Q != 0) {
    printf("ERROR!\n");
    return KO;
  }
  printf("OK!\n");
  return OK;
}
