#include "solver.h"
Vars vars;
Params params;
Workspace work;
Settings settings;
#define NUMTESTS 0
void print(){
  int i;
  printf("state:\n");
  printf("vehicle sideslip: \n");
  for (i = 0; i < 25; i++)
    printf("%.5f ", vars.x[i][0]);
  printf("\nyaw rate:\n");
  for (i = 0; i < 25; i++)
    printf("%.5f ", vars.x[i][1]);
  printf("\nheading deviation:\n");
  for (i = 0; i < 25; i++)
    printf("%.5f ", vars.x[i][2]);
  printf("\nlateral deviation:\n");
  for (i = 0; i < 25; i++)
    printf("%.5f,", vars.x[i][3]);
  printf("\nF:\n");
  for (i = 0; i < 25; i++)
    printf("%.5f,", vars.F[i][0]);
  printf("\n S_env \n");
  for (i = 15; i <= 25; i++)
    printf("%.5f ", vars.S_env[i][0]);
  printf("\n");
  for (i = 15; i <= 25; i++)
    printf("%.5f ", vars.S_env[i][1]);
  printf("\n S_traffic \n");
  for (i = 15; i <= 25; i++)
    printf("%.5f ", vars.S_tra[i][0]);
  printf("\n");
  for (i = 15; i <= 25; i++)
    printf("%.5f ", vars.S_tra[i][1]);
  // printf("\nA\n");
  // for (i = 0; i < 16; i++)
  //   printf("%.5f ", params.A[i]);
  // printf("\nB\n");
  // for (i = 0; i < 4; i++)
  //   printf("%.5f ", params.B[i]);
  // printf("\nd\n");
  // for (i = 0; i < 4; i++)
  //   printf("%.5f ", params.d[i]);
}
int main(int argc, char **argv) {
  int num_iters;
#if (NUMTESTS > 0)
  int i;
  double time;
  double time_per;
#endif
  set_defaults();
  setup_indexing();
  load_default_data();
  /* Solve problem instance for the record. */
  settings.verbose = 1;
  num_iters = solve();
#ifndef ZERO_LIBRARY_MODE
#if (NUMTESTS > 0)
  /* Now solve multiple problem instances for timing purposes. */
  settings.verbose = 0;
  tic();
  for (i = 0; i < NUMTESTS; i++) {
    solve();
  }
  time = tocq();
  printf("Timed %d solves over %.3f seconds.\n", NUMTESTS, time);
  time_per = time / NUMTESTS;
  if (time_per > 1) {
    printf("Actual time taken per solve: %.3g s.\n", time_per);
  } else if (time_per > 1e-3) {
    printf("Actual time taken per solve: %.3g ms.\n", 1e3*time_per);
  } else {
    printf("Actual time taken per solve: %.3g us.\n", 1e6*time_per);
  }
#endif
#endif
  print();
  return 0;
}
void load_default_data(void) {
  /* Make this a diagonal PSD matrix, even though it's not diagonal. */
  params.Q[0] = 0;
  params.Q[4] = 0;
  params.Q[8] = 0;
  params.Q[12] = 0;
  params.Q[1] = 0;
  params.Q[5] = 0;
  params.Q[9] = 0;
  params.Q[13] = 0;
  params.Q[2] = 0;
  params.Q[6] = 0;
  params.Q[10] = 0.5; //heading error
  params.Q[14] = 0;
  params.Q[3] = 0;
  params.Q[7] = 0;
  params.Q[11] = 0;
  params.Q[15] = 0.7; //lateral error
  params.gamma[0] = 0.000005; //smoothness
  double INF = 1e7;
  params.sig_env[0] = 500; //environment slack left
  params.sig_env[1] = 500; //environment slack right
  params.sig_tra[0] = INF; // traffic slack left
  params.sig_tra[1] = 50; // traffic slack right
  params.x0[0] = 0;
  params.x0[1] = 0;
  params.x0[2] = 0;
  params.x0[3] = 0;
  double U_x = 3.0;
  params.A[0] = - 170/(2009.0*U_x);// -C/(m*U_x)
  params.A[4] = 1.23*170/(2009.0*U_x*U_x) - 1;// b*C/(m*U_x^2) - 1
  params.A[8] = 0;
  params.A[12] = 0;
  params.A[1] = 1.23*170/3000.0;// b*C/I_zz
  params.A[5] = -1.23*1.23*170/(3000*U_x);// -b^2*C/(I_zz*U_x)
  params.A[9] = 0;
  params.A[13] = 0;
  params.A[2] = 0;
  params.A[6] = 1;//1
  params.A[10] = 0;
  params.A[14] = 0;
  params.A[3] = U_x;// U_x
  params.A[7] = 0;
  params.A[11] = U_x;// U_x
  params.A[15] = 0;
  params.B[0] = 1/(2009.0*U_x);// 1/(m*U_x)
  params.B[1] = 1.53/3000.0;// a/I_zz
  params.B[2] = 0;
  params.B[3] = 0;
  params.d[0] = 0;
  params.d[1] = 0;
  params.d[2] = 0;
  params.d[3] = 0;
  params.F_max[0] = 3000;
  params.F_max_slew[0] = 1500;
  params.H[0] = 0;
  params.H[1] = 0;
  params.H[2] = 0;
  params.H[3] = 0;
  params.H[4] = 0;
  params.H[5] = 0;
  params.H[6] = 1;
  params.H[7] = -1;
  double d_car = 1.63;
  double d_buffer = 1.5;
  //G turn left
  params.G_15[0] = 3 - d_car*0.5;
  params.G_15[1] = -0.8 - d_car*0.5;
  params.G_16[0] = 3 - d_car*0.5;
  params.G_16[1] = -0.8 - d_car*0.5;
  params.G_17[0] = 3 - d_car*0.5;
  params.G_17[1] = -0.8 - d_car*0.5;
  //G turn right
  params.G_15[0] = -0.8 - d_car*0.5 - d_buffer;
  params.G_15[1] = 3 - d_car*0.5 - d_buffer;
  params.G_16[0] = -0.8 - d_car*0.5 - d_buffer;
  params.G_16[1] = 3 - d_car*0.5 - d_buffer;
  params.G_17[0] = -0.8 - d_car*0.5 - d_buffer;
  params.G_17[1] = 3 - d_car*0.5 - d_buffer;

  params.G_18[0] =3 - d_car*0.5 - d_buffer;
  params.G_18[1] = 3 - d_car*0.5 - d_buffer;
  params.G_19[0] =3 - d_car*0.5 - d_buffer;
  params.G_19[1] = 3 - d_car*0.5 - d_buffer;
  params.G_20[0] =3 - d_car*0.5 - d_buffer;
  params.G_20[1] = 3 - d_car*0.5 - d_buffer;
  params.G_21[0] =3 - d_car*0.5 - d_buffer;
  params.G_21[1] = 3 - d_car*0.5 - d_buffer;
  params.G_22[0] = 3 - d_car*0.5 - d_buffer;
  params.G_22[1] = 3 - d_car*0.5 - d_buffer;
  params.G_23[0] = 3 - d_car*0.5 - d_buffer;
  params.G_23[1] = 3 - d_car*0.5 - d_buffer;
  params.G_24[0] = 3 - d_car*0.5 - d_buffer;
  params.G_24[1] = 3 - d_car*0.5 - d_buffer;
  params.G_25[0] = 3 - d_car*0.5 - d_buffer;
  params.G_25[1] = 3 - d_car*0.5 - d_buffer;
  params.G_26[0] = 3 - d_car*0.5 - d_buffer;
  params.G_26[1] = 3 - d_car*0.5 - d_buffer;
  params.Pos[0] = 0;
  params.Pos[1] = 0;
}
