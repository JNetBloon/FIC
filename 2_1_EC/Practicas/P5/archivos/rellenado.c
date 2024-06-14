#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

/* -O2 */

#ifndef M
#define M 512
#endif

#ifndef N
#define N 512
#endif

#ifndef NUM_REPS
#define NUM_REPS 10000
#endif

/*
 * To force a systematic set/line collision:
 *
 * Set N (columns) such that row size is a multiple of the cache size / associativity 
 * e.g., if cache is 32 KiB, 8-way associative,
 *        row size should be a multiple of 32 KiB / 8 = 4 KiB = 512 double elements
 * Set M (rows) not beyond the number of lines in the cache
 * e.g., if cache is 32 KiB and line size is 64 B,
 *       the number of rows should be <= 32 KiB / 64 B = 512
 */
 
static double a[M][N];
static double b[M][N]; // apply padding here

void myGetTime(double *t, double *tlib)
{ struct rusage tmpru;

  getrusage(RUSAGE_SELF , &tmpru);
  *t = tmpru.ru_utime.tv_sec+(tmpru.ru_utime.tv_usec/1000000.0);
  *tlib = tmpru.ru_stime.tv_sec+(tmpru.ru_stime.tv_usec/1000000.0);
}

static void initData()
{ int i, j;

  for(i = 0; i < M; i++)
    for(j = 0; j < N; j++)
    {
      a[i][j] = i + j;
      b[i][j] = i + j;
    }
}


int main()
{ double f1, fl1, f2, fl2, t;
  int i, j, k;

#ifndef NOINIT
  initData();
#endif
  t = 0.0;

  myGetTime(&f1, &fl1);

  for(k = 0; k < NUM_REPS; k++) {
    for(j = 0; j < N; j++)
      for(i = 0; i < M ; i++)
        t += a[i][j];
  }

  
  myGetTime(&f2, &fl2);

  printf("R:%lf\nTu: %lf  Tlib: %lf\n", t, f2-f1, fl2-fl1);

#ifndef NOINIT
  initData();
#endif
  t = 0.0;

  myGetTime(&f1, &fl1);
  
  /* Código optimizado */
    for(k = 0; k < NUM_REPS; k++) {
    for(j = 0; j < N; j++)
      for(i = 0; i < M ; i++)
        t += b[i][j];
  }
  
  myGetTime(&f2, &fl2);

   printf("R:%lf\nTu: %lf  Tlib: %lf\n", t, f2-f1, fl2-fl1);
}
