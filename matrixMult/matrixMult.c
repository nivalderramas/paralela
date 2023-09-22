#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArray(const double *array, int n) {
    for (int i = 0; i < n*n; i++) {
        printf("%f ", array[i]);
        if ((i + 1) % n == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main() {
  omp_set_num_threads(8);
  int n = 2;
  // Seed for random number
  srand(time(NULL));
  // Declare (plain) matrices
  double *A = (double *)malloc(n * n * sizeof(double));
  // TODO we're assuming that B is already transposed
  double *B = (double *)malloc(n * n * sizeof(double));
  double *AB = (double *)malloc(n * n * sizeof(double));

  // for (int i = 0; i < n * n; i++) {
  //   // Fill with random numbers between 1 and 100
  //   A[i] = rand() % 3 + 1;
  //   B[i] = rand() % 3 + 1;
  // }
  for(int i = 0; i<n*n; i++){
    scanf("%lf",&A[i]);
  }
  for(int i = 0; i<n*n; i++){
    scanf("%lf",&B[i]);
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


  printArray(A, n);
  printArray(B, n);
  printArray(AB, n);

  free(A);
  free(B);
  free(AB);
  return 0;
}
