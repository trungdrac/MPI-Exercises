#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main();
int i4_min(int i1, int i2); //tra ve so nho hon trong i1, i2

int main()
{
  int M = 500, N = 500; //so pixel tren truc x va truc y
  int b[M][N];
  int c; //  c = X + i*Y, Z(n+1) = Z(n)^2 + c
  int count[M][N];
  int COUNT_MAX = 2000; //so lan lap toi da cho 1 pixel
  int g[M][N];
  int i, j , jhi, jlo, k;

  char *output_filename = "mandelbrot_openmp.ppm";
  FILE *output_unit;
  int r[M][N];
  double wtime;
  double x_max = 1.25, x_min = -2.25; //khoang cua x
  double y_max = 1.75, y_min = -1.75; //khoang cua y
  double x, x1, x2, y, y1, y2;

  printf("c = x + i*y\n");
  printf("voi x trong khoang [%g,%g]\n", x_min, x_max);
  printf("va y trong khoang [%g,%g]\n", y_min, y_max);
  printf("lap %d lan moi pixel\n", COUNT_MAX);
  printf ("\n");
  printf("Z(n+1) = Z(n)^2 + C.\n");
  printf("Neu cac vong lap gioi han va Z(n) bi chan (cu the Z(n)<2)\n");
  printf("thi c thuoc tap Mandelbrot.\n");
  printf ("\n");
  printf ("Da tao file ket qua %dx%d pixels\n", M, N );

  wtime = omp_get_wtime();

//Lap cho moi pixel de tinh count
#pragma omp parallel shared(b, count, COUNT_MAX, g, r, x_max, x_min, y_max, y_min) private(i, j, k, x, x1, x2, y, y1, y2)
{
  #pragma omp for
    for (i = 0; i < M; i++)
    {
      y = ((double)(i - 1) * y_max + (double)(M - i) * y_min) / (double)(M - 1);

      for (j = 0; j < N; j++)
      {
        x = ((double)(j - 1) * x_max + (double)(N - j) * x_min) / (double)(N - 1);

        count[i][j] = 0;

        x1 = x;
        y1 = y;

        for (k = 1; k <= COUNT_MAX; k++)
        {
          x2 = x1 * x1 - y1 * y1 + x;
          y2 = 2 * x1 * y1 + y;

          if (x2 < -2.0 || 2.0 < x2 || y2 < -2.0 || 2.0 < y2)
          {
            count[i][j] = k;
            break;
          }
          x1 = x2;
          y1 = y2;
        }

        if ((count[i][j] % 2) == 1)
        {
          r[i][j] = 255;
          g[i][j] = 255;
          b[i][j] = 255;
        }
        else
        {
          c = (int)(255.0 * sqrt(sqrt(sqrt(((double)(count[i][j]) / (double)(COUNT_MAX))))));
          r[i][j] = 3 * c / 5;
          g[i][j] = 3 * c / 5;
          b[i][j] = c;
        }
      }
    }
  }

  wtime = omp_get_wtime() - wtime;
  printf("\n");
  printf("Time = %g seconds.\n", wtime);

  //tao file ket qua
  output_unit = fopen(output_filename, "wt");

  fprintf(output_unit, "P3\n");
  fprintf(output_unit, "%d  %d\n", N, M);
  fprintf(output_unit, "%d\n", 255);

  for (i = 0; i < M; i++)
  {
    for (jlo = 0; jlo < N; jlo = jlo + 4)
    {
      jhi = i4_min(jlo + 4, N);
      for (j = jlo; j < jhi; j++)
      {
        fprintf(output_unit, "  %d  %d  %d", r[i][j], g[i][j], b[i][j]);
      }
      fprintf(output_unit, "\n");
    }
  }

  fclose(output_unit);
  return 0;
}

int i4_min(int i1, int i2)
{
  int value;

  if (i1 < i2)
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}