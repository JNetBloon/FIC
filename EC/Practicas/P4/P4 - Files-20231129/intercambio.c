#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

/* -O3 */

#ifndef N
#define N 512
#endif

#ifndef NUM_REPS
#define NUM_REPS 100
#endif

static double a[N][N];

void myGetTime(double *t, double *tlib)
{ struct rusage tmpru;

  getrusage(RUSAGE_SELF , &tmpru);
  *t = tmpru.ru_utime.tv_sec+(tmpru.ru_utime.tv_usec/1000000.0);
  *tlib = tmpru.ru_stime.tv_sec+(tmpru.ru_stime.tv_usec/1000000.0);
}

static void initData()
{ int i, j;

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      a[i][j] = (double)(i + j);
}

int main()
{ double f1, fl1, f2, fl2, t;
  int i, j, k;

#ifndef NOINIT
  initData();
#endif
 
  myGetTime(&f1, &fl1);

  for(k = 0; k < NUM_REPS; k++) {
	  t=0.0;
	  for(j = 0; j < N; j++)
		  for(i = 0; i < N ; i++)
			  t += a[i][j];
  }

  
  myGetTime(&f2, &fl2);

  printf("R:%lf\nTu: %lf  Tlib: %lf\n", t, f2-f1, fl2-fl1);

}
