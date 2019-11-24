/* Produced by CVXGEN, 2018-06-02 01:51:29 -0400.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: solver.h. */
/* Description: Header file with relevant definitions. */
#ifndef SOLVER_H
#define SOLVER_H
/* Uncomment the next line to remove all library dependencies. */
/*#define ZERO_LIBRARY_MODE */
#ifdef MATLAB_MEX_FILE
/* Matlab functions. MATLAB_MEX_FILE will be defined by the mex compiler. */
/* If you are not using the mex compiler, this functionality will not intrude, */
/* as it will be completely disabled at compile-time. */
#include "mex.h"
#else
#ifndef ZERO_LIBRARY_MODE
#include <stdio.h>
#endif
#endif
/* Space must be allocated somewhere (testsolver.c, csolve.c or your own */
/* program) for the global variables vars, params, work and settings. */
/* At the bottom of this file, they are externed. */
#ifndef ZERO_LIBRARY_MODE
#include <math.h>
#define pm(A, m, n) printmatrix(#A, A, m, n, 1)
#endif
typedef struct Params_t {
  double Q[16];
  double gamma[1];
  double sig_env[2];
  double sig_tra[2];
  double x0[4];
  double A[16];
  double B[4];
  double d[4];
  double F_max[1];
  double F_max_slew[1];
  double H[8];
  double G_15[2];
  double G_16[2];
  double G_17[2];
  double G_18[2];
  double G_19[2];
  double G_20[2];
  double G_21[2];
  double G_22[2];
  double G_23[2];
  double G_24[2];
  double G_25[2];
  double G_26[2];
  double Pos[2];
  double *G[27];
} Params;
typedef struct Vars_t {
  double *x_0; /* 4 rows. */
  double *x_1; /* 4 rows. */
  double *x_2; /* 4 rows. */
  double *x_3; /* 4 rows. */
  double *x_4; /* 4 rows. */
  double *x_5; /* 4 rows. */
  double *x_6; /* 4 rows. */
  double *x_7; /* 4 rows. */
  double *x_8; /* 4 rows. */
  double *x_9; /* 4 rows. */
  double *x_10; /* 4 rows. */
  double *x_11; /* 4 rows. */
  double *x_12; /* 4 rows. */
  double *x_13; /* 4 rows. */
  double *x_14; /* 4 rows. */
  double *x_15; /* 4 rows. */
  double *x_16; /* 4 rows. */
  double *x_17; /* 4 rows. */
  double *x_18; /* 4 rows. */
  double *x_19; /* 4 rows. */
  double *x_20; /* 4 rows. */
  double *x_21; /* 4 rows. */
  double *x_22; /* 4 rows. */
  double *x_23; /* 4 rows. */
  double *x_24; /* 4 rows. */
  double *x_25; /* 4 rows. */
  double *t_01; /* 1 rows. */
  double *t_02; /* 1 rows. */
  double *t_03; /* 1 rows. */
  double *t_04; /* 1 rows. */
  double *t_05; /* 1 rows. */
  double *t_06; /* 1 rows. */
  double *t_07; /* 1 rows. */
  double *t_08; /* 1 rows. */
  double *t_09; /* 1 rows. */
  double *t_10; /* 1 rows. */
  double *t_11; /* 1 rows. */
  double *t_12; /* 1 rows. */
  double *t_13; /* 1 rows. */
  double *t_14; /* 1 rows. */
  double *t_15; /* 1 rows. */
  double *t_16; /* 1 rows. */
  double *t_17; /* 1 rows. */
  double *t_18; /* 1 rows. */
  double *t_19; /* 1 rows. */
  double *t_20; /* 1 rows. */
  double *t_21; /* 1 rows. */
  double *t_22; /* 1 rows. */
  double *t_23; /* 1 rows. */
  double *t_24; /* 1 rows. */
  double *t_25; /* 1 rows. */
  double *S_env_15; /* 2 rows. */
  double *S_env_16; /* 2 rows. */
  double *S_env_17; /* 2 rows. */
  double *S_env_18; /* 2 rows. */
  double *S_env_19; /* 2 rows. */
  double *S_env_20; /* 2 rows. */
  double *S_env_21; /* 2 rows. */
  double *S_env_22; /* 2 rows. */
  double *S_env_23; /* 2 rows. */
  double *S_env_24; /* 2 rows. */
  double *S_env_25; /* 2 rows. */
  double *S_env_26; /* 2 rows. */
  double *S_tra_15; /* 2 rows. */
  double *S_tra_16; /* 2 rows. */
  double *S_tra_17; /* 2 rows. */
  double *S_tra_18; /* 2 rows. */
  double *S_tra_19; /* 2 rows. */
  double *S_tra_20; /* 2 rows. */
  double *S_tra_21; /* 2 rows. */
  double *S_tra_22; /* 2 rows. */
  double *S_tra_23; /* 2 rows. */
  double *S_tra_24; /* 2 rows. */
  double *S_tra_25; /* 2 rows. */
  double *S_tra_26; /* 2 rows. */
  double *t_26; /* 1 rows. */
  double *F_0; /* 1 rows. */
  double *t_27; /* 1 rows. */
  double *F_1; /* 1 rows. */
  double *t_28; /* 1 rows. */
  double *F_2; /* 1 rows. */
  double *t_29; /* 1 rows. */
  double *F_3; /* 1 rows. */
  double *t_30; /* 1 rows. */
  double *F_4; /* 1 rows. */
  double *t_31; /* 1 rows. */
  double *F_5; /* 1 rows. */
  double *t_32; /* 1 rows. */
  double *F_6; /* 1 rows. */
  double *t_33; /* 1 rows. */
  double *F_7; /* 1 rows. */
  double *t_34; /* 1 rows. */
  double *F_8; /* 1 rows. */
  double *t_35; /* 1 rows. */
  double *F_9; /* 1 rows. */
  double *t_36; /* 1 rows. */
  double *F_10; /* 1 rows. */
  double *t_37; /* 1 rows. */
  double *F_11; /* 1 rows. */
  double *t_38; /* 1 rows. */
  double *F_12; /* 1 rows. */
  double *t_39; /* 1 rows. */
  double *F_13; /* 1 rows. */
  double *t_40; /* 1 rows. */
  double *F_14; /* 1 rows. */
  double *t_41; /* 1 rows. */
  double *F_15; /* 1 rows. */
  double *t_42; /* 1 rows. */
  double *F_16; /* 1 rows. */
  double *t_43; /* 1 rows. */
  double *F_17; /* 1 rows. */
  double *t_44; /* 1 rows. */
  double *F_18; /* 1 rows. */
  double *t_45; /* 1 rows. */
  double *F_19; /* 1 rows. */
  double *t_46; /* 1 rows. */
  double *F_20; /* 1 rows. */
  double *t_47; /* 1 rows. */
  double *F_21; /* 1 rows. */
  double *t_48; /* 1 rows. */
  double *F_22; /* 1 rows. */
  double *t_49; /* 1 rows. */
  double *F_23; /* 1 rows. */
  double *t_50; /* 1 rows. */
  double *F_24; /* 1 rows. */
  double *t_51; /* 1 rows. */
  double *F_25; /* 1 rows. */
  double *t_52; /* 1 rows. */
  double *t_53; /* 1 rows. */
  double *t_54; /* 1 rows. */
  double *t_55; /* 1 rows. */
  double *t_56; /* 1 rows. */
  double *t_57; /* 1 rows. */
  double *t_58; /* 1 rows. */
  double *t_59; /* 1 rows. */
  double *t_60; /* 1 rows. */
  double *t_61; /* 1 rows. */
  double *t_62; /* 1 rows. */
  double *t_63; /* 1 rows. */
  double *t_64; /* 1 rows. */
  double *t_65; /* 1 rows. */
  double *t_66; /* 1 rows. */
  double *t_67; /* 1 rows. */
  double *t_68; /* 1 rows. */
  double *t_69; /* 1 rows. */
  double *t_70; /* 1 rows. */
  double *t_71; /* 1 rows. */
  double *t_72; /* 1 rows. */
  double *t_73; /* 1 rows. */
  double *t_74; /* 1 rows. */
  double *t_75; /* 1 rows. */
  double *t_76; /* 1 rows. */
  double *x_26; /* 4 rows. */
  double *x[27];
  double *F[26];
  double *S_env[27];
  double *S_tra[27];
} Vars;
typedef struct Workspace_t {
  double h[225];
  double s_inv[225];
  double s_inv_z[225];
  double b[133];
  double q[258];
  double rhs[841];
  double x[841];
  double *s;
  double *z;
  double *y;
  double lhs_aff[841];
  double lhs_cc[841];
  double buffer[841];
  double buffer2[841];
  double KKT[2160];
  double L[2220];
  double d[841];
  double v[841];
  double d_inv[841];
  double gap;
  double optval;
  double ineq_resid_squared;
  double eq_resid_squared;
  double block_33[1];
  /* Pre-op symbols. */
  int converged;
} Workspace;
typedef struct Settings_t {
  double resid_tol;
  double eps;
  int max_iters;
  int refine_steps;
  int better_start;
  /* Better start obviates the need for s_init and z_init. */
  double s_init;
  double z_init;
  int verbose;
  /* Show extra details of the iterative refinement steps. */
  int verbose_refinement;
  int debug;
  /* For regularization. Minimum value of abs(D_ii) in the kkt D factor. */
  double kkt_reg;
} Settings;
extern Vars vars;
extern Params params;
extern Workspace work;
extern Settings settings;
/* Function definitions in ldl.c: */
void ldl_solve(double *target, double *var);
void ldl_factor(void);
double check_factorization(void);
void matrix_multiply(double *result, double *source);
double check_residual(double *target, double *multiplicand);
void fill_KKT(void);

/* Function definitions in matrix_support.c: */
void multbymA(double *lhs, double *rhs);
void multbymAT(double *lhs, double *rhs);
void multbymG(double *lhs, double *rhs);
void multbymGT(double *lhs, double *rhs);
void multbyP(double *lhs, double *rhs);
void fillq(void);
void fillh(void);
void fillb(void);
void pre_ops(void);

/* Function definitions in solver.c: */
double eval_gap(void);
void set_defaults(void);
void setup_pointers(void);
void setup_indexed_params(void);
void setup_indexed_optvars(void);
void setup_indexing(void);
void set_start(void);
double eval_objv(void);
void fillrhs_aff(void);
void fillrhs_cc(void);
void refine(double *target, double *var);
double calc_ineq_resid_squared(void);
double calc_eq_resid_squared(void);
void better_start(void);
void fillrhs_start(void);
long solve(void);

/* Function definitions in testsolver.c: */
int main(int argc, char **argv);
void load_default_data(void);

/* Function definitions in util.c: */
void tic(void);
float toc(void);
float tocq(void);
void printmatrix(char *name, double *A, int m, int n, int sparse);
double unif(double lower, double upper);
float ran1(long*idum, int reset);
float randn_internal(long *idum, int reset);
double randn(void);
void reset_rand(void);

#endif
