#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArray(const double *array, int n) {
  for (int i = 0; i < n * n; i++) {
    printf("%f ", array[i]);
    if ((i + 1) % n == 0) {
      printf("\n");
    }
  }
  printf("\n");
}

// ./bin threads and n
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s num_threads n\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  int n = atoi(argv[2]);

  omp_set_num_threads(num_threads);
  // Seed for random number
  srand(time(NULL));


  // Declare (plain) matrices
  double *A = (double *)malloc(n * n * sizeof(double));
  double *B = (double *)malloc(n * n * sizeof(double));
  double *AB = (double *)malloc(n * n * sizeof(double));

  for (int i = 0; i < n * n; i++) {
    scanf("%lf", &A[i]);
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%lf", &B[j * n + i]); // Transpose B during input
    }
  }

#pragma omp parallel for
  for (int i = 0; i < n * n; i++) {
    int row = i / n;
    int col = i % n;
    int idxA = row * n;
    int idxB = col * n;
    double sum = 0;
    int id = omp_get_thread_num();
    // printf("id num %d process index %d \n",id,i);
    for (int j = 0; j < n; j++) {
      sum += A[idxA + j] * B[idxB + j];
    }
    AB[i] = sum;
  }

  // printArray(A, n);
  // printArray(B, n);
  // printArray(AB, n);
  printArray(AB, n);

  free(A);
  free(B);
  free(AB);
  return 0;
}
