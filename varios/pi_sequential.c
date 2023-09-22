#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N (int)1e9

int main() {
  long double pi = 0;int i;
  #pragma omp parallel 
  {
    #pragma omp for reduction (+:pi)
  for (i = 0; i < N; i += 2) {
    pi += 1.0 / ((2 * i) + 1);
    pi -= 1.0 / ((2 * (i + 1)) + 1);
  }
  }
  printf("%0.8Lf\n", 4.0 * pi);
  return 0;
}
