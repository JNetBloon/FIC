#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

/* -O3 */

#ifndef N
#define N 1024
#endif

#ifndef NUM_REPS
#define NUM_REPS 100
#endif

static double a[N][N], b[N][N], c[N][N], d[N][N];

void myGetTime(double *t, double *tlib)
{ struct rusage tmpru;

  getrusage(RUSAGE_SELF , &tmpru);
  *t = tmpru.ru_utime.tv_sec+(tmpru.ru_utime.tv_usec/1000000.0);
  *tlib = tmpru.ru_stime.tv_sec+(tmpru.ru_stime.tv_usec/1000000.0);
}

static void initData()
{ int i, j;

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++) {
      a[i][j] = 0.0;
      b[i][j] = (double)(i + j);
      c[i][j] = (double)(i * j);
      d[i][j] = 0.0;
    }

}

int main()
{ double f1, fl1, f2, fl2;
  int i, j, k;

#ifndef NOINIT
  initData();
#endif

  myGetTime(&f1, &fl1);

  for(k = 0; k < NUM_REPS; k++) {
      for(i = 0; i < N ; i++)
	for(j = 0; j < N; j++)
	  a[i][j] = b[i][j] / 4.0 * c[i][j];

      for(i = 0; i < N ; i++)
	for(j = 0; j < N; j++)
	  d[i][j] = a[i][j] + c[i][j];
  }

  
  myGetTime(&f2, &fl2);

  printf("Tu: %lf  Tlib: %lf\n", f2-f1, fl2-fl1);

}
