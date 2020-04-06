#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static long steps = 1000000;
double step;

int main (int argc, const char *argv[]) {

    int i;
    double x;
    double sum = 0.0;
    double start, delta;

    omp_set_num_threads(4);

    sum = 0.0;
    start = omp_get_wtime();
    #pragma omp parallel private(i,x)

    #pragma omp for reduction(+:sum) schedule(static)
    for (i=0; i < steps; i++) {
        x = 4.0*pow(-1, i) / (1.0+2*i);
        
        sum += x; 
        // printf("%d: On thread %d, x is: %f\n", i, omp_get_thread_num(), x);
    }
    
    delta = omp_get_wtime() - start;
    printf("PI = %.10g computed in %.6f seconds\n", sum, delta);
}