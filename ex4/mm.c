#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define CHUNKSIZE 10000

void NhapMaTran(int **a, int dong, int cot){
    int i, j;
    for (i = 0; i < dong; i++)
        for (j = 0; j < cot; j++)
        {
            a[i][j] = rand() % 10;
        }
}
void XuatMaTran(int **a, int dong, int cot){
    int i, j;
    printf("\n Ma tran %d x %d\n", dong, cot);
    for (i = 0; i < dong; i++)
    {
        for (j = 0; j < cot; j++)
            printf("%5d", a[i][j]);
        printf("\n");
    }
}
int main()
{
    int **axb = NULL, **bxc = NULL;
    int a=10, b=15, c=20;
    int sum, i, j, k;
    int multiply[a][c];
    int nthreads, tid;
    double start, delta;

    axb = (int **)malloc(a * sizeof(int *));
    for (i = 0; i < a; i++)
    {
        axb[i] = (int *)malloc(b * sizeof(int));
    }
    NhapMaTran(axb, a, b);
    // XuatMaTran(axb, a, b);

    bxc = (int **)malloc(b * sizeof(int *));
    for (int j = 0; j < b; j++)
    {
        bxc[j] = (int *)malloc(c * sizeof(int));
    }
    NhapMaTran(bxc, b, c);
    // XuatMaTran(bxc, b, c);
    
    int chunk = CHUNKSIZE;
    start = omp_get_wtime();
    #pragma omp parallel shared(axb, bxc, multiply) private(i, j, k, sum)
    {   
        omp_set_num_threads(8);
        omp_set_nested(2);
        omp_set_dynamic(CHUNKSIZE);
        #pragma omp for schedule(dynamic,1) nowait
        for (i = 0; i < a; i++) {
            for (k = 0; k < c; k++) {
                for (j = 0; j < b; j++) 
                    sum = sum + axb[i][j]*bxc[j][k];
                multiply[i][k] = sum;
                // printf("On thread %d, sum is: %d\n", omp_get_thread_num(), sum);
                sum = 0;

                printf("The number of processors available: %d\n", omp_get_num_procs());
                printf("The number of threads being used: %d\n", omp_get_num_threads());
                printf("The maximum number of threads available: %d\n", omp_get_max_threads());
                printf("If being in parallel region: %d\n", omp_in_parallel());
                printf("If dynamic threads are enabled: %d\n", omp_get_dynamic());
                printf("If nested parallelism is supported: %d\n\n", omp_get_nested());
            }
        }
        
    }

    delta = omp_get_wtime() - start;
    printf("computed in %.6f seconds\n", delta); 
    
    // printf("\n Ma tran ket qua\n");
    // for (i = 0; i < a; i++)
    // {   
    //     for (j = 0; j < c; j++)
    //         printf("%5d", multiply[i][j]);
    //     printf("\n");
    // }
    return 0;
}