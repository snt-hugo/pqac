#ifndef PARAM_H
#define PARAM_H

#include <inttypes.h>
#include <gmp.h>

/* S8 means the size is counted in bytes */
/* S1 means the size is counted in bits */
#define SIZE_INTQ1_S8 8
#define SIZE_INTQ2_S8 24
#define SIZE_INTQ_S8 16
#define SIZE_INTQ1_S1 64
#define SIZE_INTP_S1 64
/* R is Z[X]/(X^(DIM_R) + 1) */
#define SIZE_DIM_R_S1 13
#define DIM_R 8192
#define SIZE_THETA_S1 11
#define THETA 2048
#define KAPPA 240
/* ceil of log(2,q1) */
#define SIZE_Q1_S1 41
/* ceil of log(2,q1)/8 */
#define SIZE_Q1_S8 6
/* ceil of log(2,q2) */
#define SIZE_Q2_S1 151
/* ceil of log(2,q2)/8 */
#define SIZE_Q2_S8 19
/* ceil of log(2,Q)*/
#define SIZE_Q_S1 86
/* ceil of log(2,Q)/8 */
#define SIZE_Q_S8 6
/* ceil of log(2,p)*/
#define SIZE_P_S1 35
/* ceil of log(2,p)/8 */
#define SIZE_Q1_S8 6
/* the TAU are used for the gaussian sampler */
#define TAU_XI_STD_DEV 20
#define TAU_XI1_STD_DEV 20
#define TAU_XI2_STD_DEV 20
#define TAU_S_STD_DEV 20
#define TAU_R_STD_DEV 20

#define FPA_PREC 200

#define CEIL_LOG_S 85
#define CEIL_LOG_R 77
#define CEIL_LOG_XI 20
#define CEIL_LOG_XI1 104 
#define CEIL_LOG_XI2 113 

/* ###################################################### */
/* ################ Types definitions ################### */
/* ###################################################### */

typedef uint64_t intq1_t[1];
typedef mp_limb_t intq2_t[3];
typedef mp_limb_t intQ_t[2];
typedef uint64_t intp_t[1];
typedef unsigned int sigma_t[2];

/* ###################################################### */
/* #################### Parameters ###################### */
/* ###################################################### */

extern intq1_t g_intq1_q1;
extern intQ_t g_intQ_Q;
extern intq2_t g_intq2_q2;
extern intp_t g_intp_p;
extern intQ_t g_intQ_Q_minus_q1;
extern intq2_t g_intq2_q2_minus_q1;
extern intq2_t g_intq2_delta;
extern intq1_t g_intq1_xi;
extern intq2_t g_intq2_xi1;
extern intq2_t g_intq2_xi2;
extern intq2_t g_intq2_ssd;
extern intq2_t g_intq2_rsd;
extern intQ_t g_intQ_p;

extern sigma_t ui_sigma_1_VDIM_R[8192];
extern sigma_t ui_sigma_5_VDIM_R[8192];

extern intq1_t g_pow_zeta_q1[4095];
extern intq2_t g_pow_zeta_q2[4095];
extern intQ_t g_pow_zeta_Q[4095];

extern int zeta_exponents_ntt_order[2048];

#endif /* PARAMS_H */
