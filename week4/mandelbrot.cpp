#include <bits/stdc++.h>
#include <math.h>
#include <omp.h>
#include "../libs/simple_svg_1.0.0.hpp"

int M = 500, N = 500;

int main(int argc, char** argv);
int i4_min(int i1, int i2); //tra ve so nho hon trong i1, i2
void draw(int** r, int** g, int** b, bool drawNearby);


int main(int argc, char** argv)
{
  #pragma region read commandline parameters
  bool drawNearby = false;
  if (argc == 2) {
    int p = atoi(argv[1]);
    drawNearby = (p != 0);
  } else if (argc == 4) {
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    int p = atoi(argv[3]);
    drawNearby = (p != 0);
  }
  #pragma endregion
  
  #pragma region declare variables
  int **r, **g, **b, **count;
  int c; //  c = X + i*Y, Z(n+1) = Z(n)^2 + c
  
  int COUNT_MAX = 2000; //so lan lap toi da cho 1 pixel
  int i, j , jhi, jlo, k;

  char *output_filename = "mandelbrot_openmp.ppm";
  FILE *output_unit;
  double wtime;
  double x_max = 1.25, x_min = -2.25; //khoang cua x
  double y_max = 1.75, y_min = -1.75; //khoang cua y
  double x, x1, x2, y, y1, y2;
  #pragma endregion 

  #pragma region allocate memory
  r = new int*[M];
  g = new int*[M];
  b = new int*[M];
  count = new int*[M];
  for (int i = 0; i < M; i++) {
    r[i] = new int[N];
    g[i] = new int[N];
    b[i] = new int[N];
    count[i] = new int[N];
  }
  #pragma endregion

  

  printf("c = x + i*y\n");
  printf("voi x trong khoang [%g,%g]\n", x_min, x_max);
  printf("va y trong khoang [%g,%g]\n", y_min, y_max);
  printf("lap %d lan moi pixel\n", COUNT_MAX);
  
  // Z_(n+1) = Z_n ^ 2 + c
  // Neu Z_n hoi tu khi n tien den vo cuc thi c thuoc hinh Mandelbrot

  printf ("\n");
  printf ("Da tao file ket qua %dx%d pixels\n", M, N );

  wtime = omp_get_wtime();

//Lap cho moi pixel de tinh count
#pragma omp parallel shared(b, count, COUNT_MAX, g, r, x_max, x_min, y_max, y_min) private(i, j, k, x, x1, x2, y, y1, y2)
{
  #pragma omp for
    for (i = 0; i < M; i++)
    {
      y = y_min + i*(y_max - y_min)/M;

      for (j = 0; j < N; j++)
      {
        x = x_min + j*(x_max - x_min) / N;

        count[i][j] = INT_MAX; // 0

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

        if (count[i][j] < COUNT_MAX) {
          r[i][j] = 255;
          g[i][j] = 255;
          b[i][j] = 255;
        } else {
          r[i][j] = 0;
          g[i][j] = 0;
          b[i][j] = 0;
        }
        #pragma region nearby point
        // if ((count[i][j] % 2) == 1)
        // {
        //   r[i][j] = 255;
        //   g[i][j] = 255;
        //   b[i][j] = 255;
        // }
        // else
        // {
        //   c = (int)(255.0 * sqrt(sqrt(sqrt(((double)(count[i][j]) / (double)(COUNT_MAX))))));
        //   r[i][j] = 3 * c / 5;
        //   g[i][j] = 3 * c / 5;
        //   b[i][j] = c;
        // }
        #pragma endregion
      }
    }
  }

  wtime = omp_get_wtime() - wtime;
  printf("\n");
  printf("Time = %g seconds.\n", wtime);

  #pragma region write out results
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
  draw(r, g, b, drawNearby);
  #pragma endregion

  #pragma region cleanup
  for (int i = 0; i < M; i++) {
    delete[] r[i], g[i], b[i], count[i];
  }
  delete[] r, g, b, count;
  #pragma endregion

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

void draw(int** r, int** g, int** b, bool drawNearby = false) {
  int offsetX = 20, offsetY = 20;
  svg::Dimensions dimensions(M + 2*offsetX, N + 2*offsetY);
  svg::Document* doc = new svg::Document("output.svg", svg::Layout(dimensions, svg::Layout::BottomLeft));

  svg::Rectangle background(svg::Point(0, 0), M + 2*offsetX, N + 2*offsetY, svg::Fill(svg::Color::White), svg::Stroke(0, svg::Color::White));
  *doc << background;

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if (r[i][j] == 255 && g[i][j] == 255 && b[i][j] == 255) continue;
      if (drawNearby || (r[i][j] == 0 && g[i][j] == 0 && b[i][j] == 0)) {
        svg::Point topLeft(j + offsetX, i + offsetY);
        svg::Color color(r[i][j], g[i][j], b[i][j]);
        svg::Rectangle rect(topLeft, 1, 1, svg::Fill(color), svg::Stroke(0, color));
        *doc << rect;
      }
    }
  }

  doc->save();
  delete doc;
}