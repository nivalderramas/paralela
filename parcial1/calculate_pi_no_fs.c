#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 2e9;
double sums[20][8];
int NUM_THREADS;

void sum() {
  int id = omp_get_thread_num();
  int block = N / NUM_THREADS;
  double partialSum = 0;
  for (int i = id * block; i < (id + 1) * block; i++) {
    if (i & 1)
      partialSum -= 1.0 / ((2 * i) + 1);
    else
      partialSum += 1.0 / ((2 * i) + 1);
  }
  sums[id][0] = partialSum;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <num_threads>\n", argv[0]);
    return 1;
  }

  NUM_THREADS = atoi(argv[1]);
  omp_set_num_threads(NUM_THREADS);
  double pi = 0;
#pragma omp parallel
  { sum(); }
  for (int i = 0; i < NUM_THREADS; i++) {
    pi += sums[i][0];
  }
  printf("%f \n", pi * 4.0);
  return 0;
}
