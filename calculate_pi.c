#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define NUM_HILOS 256
#define N 1000000000

struct datos {
  int a;
  int b;
  float res;
};

void *calcSum(void *data) {
  float res = 0;
  struct datos *d;
  d = (struct datos *)data;
  int a = d->a;
  int b = d->b;
  for (int i = a; i <= b; i++) {
    double t;
    int j = i-1;
    if (j & 1)
      t = -1;
    else
      t = 1;
    res += t / ((2.0 * j) + 1);
  }
  d->res = res;
  return NULL;
}

int main() {
  pthread_t hilo[NUM_HILOS];
  struct datos *thread_data[NUM_HILOS];
  int i = 0;
  double ans = 0.0;
  double n = ceil(N / (double)NUM_HILOS);
  for (i = 0; i < NUM_HILOS; i++) {
    struct datos *data = (struct datos *)malloc(sizeof(struct datos));
    thread_data[i] = data;
    data->a = min(N, i * n + 1);
    data->b = min(((i + 1) * n), N);
    int r = pthread_create(&hilo[i], NULL, calcSum, (void *)data);
    if (r != 0) {
      perror("\n-->pthread_create error: ");
      exit(-1);
    }
  }

  for (i = 0; i < NUM_HILOS; i++) {
    int r = pthread_join(hilo[i], NULL);
    if (r != 0) {
      printf("err");
      perror("\n-->pthread_join error: ");
      exit(-1);
    }
  }
  // Sum the individual thread results
  for (i = 0; i < NUM_HILOS; i++) {
    ans += thread_data[i]->res;
    free(thread_data[i]);
  }
  printf("Approximation of PI: %f\n", 4.0 * ans);
  return 0;
}
