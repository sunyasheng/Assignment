#include <stdio.h>
#include <omp.h> 

static long num_steps = 1000000000;
double step;
#define NUM_THREADS 2

int main(){
    int i; double pi, sum = 0.0;
	double start_time, run_time;
    step = 1.0/(double)num_steps;
	omp_set_num_threads(NUM_THREADS);
    start_time = omp_get_wtime();
#pragma omp parallel
{
    double id = omp_get_thread_num();
    double x;

#pragma omp for private(x) reduction(+:sum)
    for(i=0;i<num_steps;i++){
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step*sum;	
}
	run_time = omp_get_wtime() - start_time;
    printf("Number of Threads is %d, used %.5f seconds, pi equals %.5lf.", NUM_THREADS, run_time, pi);
}