#ifndef SAMPLERS_H
#define SAMPLERS_H

#include "new_arithm_int.h"
#include "new_arithm_poly.h"

int rand_init ();
int rand_clear ();

int get_random_bit (uint8_t *out);
int get_random_bytes (void *out, const size_t len);

int get_random_intq1 (intq1_t out);
int get_random_intq2 (intq2_t out);
int get_random_intQ (intQ_t out);
int get_random_intp (intp_t out);

int get_random_polyq1 (polyq1_t polyq1_res);
int get_random_polyq2 (polyq2_t polyq2_res);
int get_random_polyQ (polyQ_t polyQ_res);
int get_random_polyp (polyp_t polyp_res);

int get_random_challenge (challenge_t chall_res);
int get_random_challenge_NTTq1 (polyq1_t NTTq1_chall);
int get_random_challenge_NTTq2 (polyq2_t NTTq2_chall);
int get_random_challenge_chall_NTTq1 (challenge_t chall, polyq1_t NTTq1_chall);
int get_random_challenge_chall_NTTq1_NTTQ (challenge_t chall, polyq1_t NTTq1_chall, polyQ_t NTTQ_chall);
int get_random_challenge_chall_NTTq1_NTTq2 (challenge_t chall, polyq1_t NTTq1_chall, polyq2_t NTTq2_chall);
int get_random_challenge_chall_NTTq1_NTTq2_NTTQ (challenge_t chall, polyq1_t NTTq1_chall, polyq2_t NTTq2_chall, polyQ_t NTTQ_chall);
int get_random_challenge_NTTq1_NTTq2 (polyq1_t NTTq1_chall, polyq2_t NTTq2_chall);
int get_random_challenge_NTTQ (polyQ_t NTTQ_chall);

int get_random_little_challenge (challenge_t chall_res);
int get_random_little_positive_challenge (challenge_t chall_res);
int get_random_little_degree_little_positive_challenge (challenge_t chall_res);

int get_gaussian_s_polyq2 (polyq2_t polq2_sample);
int get_gaussian_r_polyq2 (polyq2_t polq2_sample);
int get_gaussian_xi_polyq1 (polyq1_t polq1_sample);
int get_gaussian_xi_NTTQ (polyQ_t NTTQ_sample);
void get_gaussian_xi_NTTQ_V (polyQ_t *NTTQ_sample, int size);
int get_gaussian_xi_NTTq1_NTTq2 (polyq1_t NTTq1_sample, polyq2_t NTTq2_sample);
void get_gaussian_xi_NTTq1_NTTq2_V (polyq1_t *NTTq1_sample, polyq2_t *NTTq2_sample, int size);
int get_gaussian_xi_NTTq1_NTTq2_NTTQ (polyq1_t NTTq1_sample, polyq2_t NTTq2_sample, polyQ_t NTTQ_sample);
void get_gaussian_xi_NTTq1_NTTq2_NTTQ_V (polyq1_t *NTTq1_sample, polyq2_t *NTTq2_sample, polyQ_t *NTTQ_sample, int size);
int get_gaussian_xi_polyq2 (polyq2_t polq2_sample);
int get_gaussian_xi_polyQ (polyQ_t polQ_sample);

int get_gaussian_r_NTTq2 (polyq2_t NTTq2_sample);
int get_gaussian_s_NTTq2 (polyq2_t NTTq2_sample);

int get_gaussian_xi1_polyq2 (polyq2_t polq2_sample);
int get_gaussian_xi1_NTTq2 (polyq2_t NTTq2_sample);
void get_gaussian_xi1_NTTq2_V (polyq2_t *NTTq2_sample, int size);
int get_gaussian_xi2_polyq2 (polyq2_t polq2_sample);
int get_gaussian_xi2_NTTq2 (polyq2_t NTTq2_sample);
void get_gaussian_xi2_NTTq2_V (polyq2_t *NTTq2_sample, int size);


int get_gaussian_s_intq2 (intq2_t Rq2_sample);
int get_gaussian_r_intq2 (intq2_t Rq2_sample);
int get_gaussian_xi_intq2 (intq2_t Rq2_sample);
int get_gaussian_xi1_intq2 (intq2_t Rq2_sample);
int get_gaussian_xi2_intq2 (intq2_t Rq2_sample);
#endif /* SAMPLERS_H */
