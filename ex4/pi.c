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

    omp_set_num_threads(8);
    sum = 0.0;
    start = omp_get_wtime();
    printf("Before parallel: ");
    printf("4. If being in parallel region: %d\n", omp_in_parallel());
    #pragma omp parallel private(x)
    {
        omp_set_nested(2);
        // omp_set_dynamic(CHUNKSIZE);
        #pragma omp for reduction(+:sum)
        for (i=0; i < steps; i++) {
            x = 4.0*pow(-1, i) / (1.0+2*i);
            sum += x; 
            // printf("%d: On thread %d, x is: %f\n", i, omp_get_thread_num(), x);
        }
        
        #pragma omp single
        {
            printf("1. The number of processors available: %d\n", omp_get_num_procs());
            printf("2. The number of threads being used: %d\n", omp_get_num_threads());
            printf("3. The maximum number of threads available: %d\n", omp_get_max_threads());
            printf("4. If being in parallel region: %d\n", omp_in_parallel());
            printf("5. If dynamic threads are enabled: %d\n", omp_get_dynamic());
            printf("6. If nested parallelism is supported: %d\n", omp_get_nested());
        }
    }
    printf("After parallel: ");
    printf("4. If being in parallel region: %d\n\n", omp_in_parallel());
    delta = omp_get_wtime() - start;
    printf("PI = %.10g computed in %.6f seconds\n", sum, delta);
}