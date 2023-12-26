#define MAXV 1000000000LL
#define NB_PROCESS 16

#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define SIZE ((MAXV / 8) + 1)

typedef struct _Params {
  uint8_t *tab;
  int64_t *p;
  int64_t start;
  int64_t end;
} Params;

void build_primes(int64_t *p, int sto, int nb_primes) {
  p[0] = 2;
  int k = 1;
  int i = 3;
  while (true) {
    if (k >= nb_primes) {
      perror("nb_primes");
      exit(-1);
    }
    int j = 0;
    int st = sqrt((double)i + 0.01);
    while (true) {
      if (((i % p[j]) == 0) || (p[j] > st)) {
        break;
      }
      j++;
    }
    if (p[j] > st) {
      p[k++] = i;
      if (i > sto) {
        break;
      }
    }
    i += 2;
  }
}

static void *do_the_job(void *arg) {
  Params *pr = arg;
  uint8_t *tab = pr->tab;
  int64_t *primes = pr->p;
  int64_t a = pr->start;
  int64_t b = pr->end;

  int64_t bsup = sqrt(b + 0.01);
  for (int i = 0;; i++) {
    int p = primes[i];
    if (p > bsup) {
      break;
    }
    int64_t k = a / p;
    int64_t mp = k * p;
    if (mp < a) {
      mp = mp + p;
      k++;
    }
    while (mp <= b) {
      int64_t i1 = mp / 8, i2 = mp % 8;
      int64_t j1 = k / 8, j2 = k % 8;
      if ((tab[j1] & (1 << j2)) == 0) {
        tab[i1] = tab[i1] | (1 << i2);
      }
      mp = mp + p;
      k++;
    }
  }
  return NULL;
}

int main() {
  Params pr[NB_PROCESS];
  pthread_t pids[NB_PROCESS];

  int sto = sqrt((double)MAXV + 0.01);
  int nb_primes = 1.25 * ((double)sto) / log(sto);
  int64_t *primes = (int64_t *)malloc(sizeof(int64_t) * nb_primes);
  if (primes == NULL) {
    printf("Error in primes malloc\n");
    exit(-1);
  }
  build_primes(primes, sto, nb_primes);
  uint8_t *tab = (uint8_t *)calloc(SIZE, sizeof(uint8_t));
  if (tab == NULL) {
    printf("Error in tab malloc\n");
    exit(-1);
  }
  tab[0] = 0x2;

  int64_t a = 3;
  int64_t b;
  while (a < MAXV) {
    b = 2 * a - 1;
    printf("%lld %lld\n", (long long int)a, (long long int)b);
    if (b >= MAXV) {
      b = MAXV - 1;
    }
    if ((b - a) < 65536) {
      pr[0].tab = tab;
      pr[0].p = primes;
      pr[0].start = a;
      pr[0].end = b;
      do_the_job(&pr[0]);
    } else {
      for (int num = 0; num < NB_PROCESS; num++) {
        pr[num].tab = tab;
        pr[num].p = primes;
        if (num == 0)
          pr[num].start = a;
        else
          pr[num].start = a + ((b - a) * num) / NB_PROCESS + 1;
        pr[num].end = a + ((b - a) * (num + 1)) / NB_PROCESS;
        int res = pthread_create(&pids[num], NULL, &do_the_job, &pr[num]);
        if (res != 0) {
          printf("Error in pthread_create\n");
          exit(-1);
        }
      }
      for (int num = 0; num < NB_PROCESS; num++) {
        int res = pthread_join(pids[num], NULL);
        if (res != 0) {
          printf("Error in pthread_join\n");
          exit(-1);
        }
      }
    }
    a = b + 1;
  }

  int64_t first = 0, last = 0;
  int64_t imax = 0, maxi = -10000000;
  int64_t imin = 0, mini = 10000000;
  int64_t num = 0;
  for (int64_t i = 3; i < 8; i++) {
    int64_t i1 = i / 8, i2 = i % 8;
    if ((tab[i1] & (1 << i2)) == 0) {
      num = num - 1;
    } else {
      num = num + 1;
    }
    if (num > 0) {
      last = i;
    }
    if ((num > 0) && (first == 0)) {
      first = i;
    }
    if (num > maxi) {
      maxi = num;
      imax = i;
    }
    if (num < mini) {
      mini = num;
      imin = i;
    }
  }

  int64_t i1 = 1;
  uint8_t i2 = 0;
  while (true) {
    if ((tab[i1] & (1 << i2)) == 0)
      num--;
    else
      num++;
    if (num > 0) {
      last = 8 * i1 + i2;
      if (first == 0) {
        first = last;
      }
      if (num > maxi) {
        maxi = num;
        imax = last;
      }
    }
    if (num < mini) {
      mini = num;
      imin = 8 * i1 + i2;
    }
    i2++;
    if (i2 == 8) {
      i2 = 0;
      i1++;
      if (i1 == MAXV / 8)
        break;
    }
  }

  printf("first=%lld,imax=%lld,maxi=%lld,last=%lld,imin=%lld,mini=%lld\n",
	 (long long int)first,(long long int)imax, (long long int)maxi,
	 (long long int)last, (long long int)imin, (long long int)mini);
}
