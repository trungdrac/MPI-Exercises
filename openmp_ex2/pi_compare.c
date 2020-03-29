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

    sum = 0.0;
    
    clock_t start = clock(), end;
    for (i=0; i < steps; i++) {
        x = 4.0*pow(-1, i) / (1.0+2*i);
        sum += x;
       // printf("%f", x); 
    }
    end = clock();
    printf("\nPI = %.16g computed in: %.6fs\n", sum, ((double)(end - start))/CLOCKS_PER_SEC);
}