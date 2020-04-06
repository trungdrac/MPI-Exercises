#include <stdio.h>
#include <omp.h>

#define MAX_THREADS 4

static long num_steps = 100000000;
double step;
int main()
{
    int i, j;
    int my_dyn = 1;
    double pi, full_sum = 0.0;
    double start_time, run_time;
    double sum[MAX_THREADS];
    int num_processor = omp_get_num_procs();
    int is_parallel = omp_in_parallel();
    int maxthreads = omp_get_max_threads();
    int numthreads;

    step = 1.0 / (double)num_steps;

    // omp_set_dynamic(my_dyn);

    printf("\nnum_processor = %d\n", num_processor);
    printf("\nmax_threads = %d\n", maxthreads);
    printf("\nis_parallel: %d\n", is_parallel);
    printf("\nstart calculate PI\n");
    
    omp_set_num_threads(1); // set the number of thread for region parallel
        
    full_sum = 0.0;
    start_time = omp_get_wtime();
    #pragma omp parallel private(i)
    {
        int id = omp_get_thread_num();
        numthreads = omp_get_num_threads();
        int is_dyn = omp_get_dynamic();
        int is_nested = omp_get_nested();
        is_parallel = omp_in_parallel();
        double x;

        double partial_sum = 0;

        #pragma omp single
        {
            printf("\nnum_threads = %d\n", numthreads);
            printf("\ndynamic status: %d\n", is_dyn);
            printf("\nis_parallel = %d\n", is_parallel);
            printf("\nis_nested: %d\n", is_nested);            
        }

        for (i = id; i < num_steps; i += numthreads)
        {
            x = (i + 0.5) * step;
            partial_sum += +4.0 / (1.0 + x * x);
        }
        #pragma omp critical
        full_sum += partial_sum;
    }

    pi = step * full_sum;
    run_time = omp_get_wtime() - start_time;
    printf("\n PI is %f in %f seconds %d threads\n", pi, run_time, numthreads);
    
}