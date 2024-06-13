#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

/* -O2 */

#ifndef N
#define N 1024
#endif

/*
 * Block size should be small enough to fit 2 square blocks in cache
 * e.g., 32 KiB cache, T <= 45
 */
 
#ifndef T
#define T 32
#endif

#ifndef NUM_REPS
#define NUM_REPS 1
#endif

#define min(a,b) ( ( (a) < (b) ) ? (a) : (b))

static double a[N][N], b[N][N], c[N][N];

void myGetTime(double *t, double *tlib)
{ struct rusage tmpru;

  getrusage(RUSAGE_SELF , &tmpru);
  *t = tmpru.ru_utime.tv_sec+(tmpru.ru_utime.tv_usec/1000000.0);
  *tlib = tmpru.ru_stime.tv_sec+(tmpru.ru_stime.tv_usec/1000000.0);
}

double test_reset() 
{ double t = 0.0;
  int i, j;

  for(i = 0; i < N ; i++)
    for(j = 0; j < N; j++) {
      t += c[i][j];
      c[i][j] = 0.0;
    }

  return t;
}

int main()
{ double f1, fl1, f2, fl2;
  int i, j, k, r, jj, kk;

#ifndef NOINIT
  for(i = 0; i < N ; i++)
    for(j = 0; j < N; j++) {
      c[i][j] = 0.0;
      a[i][j] = i + j;
      b[i][j] = i * j;
    }
#endif

  myGetTime(&f1, &fl1);

  for(r = 0; r < NUM_REPS; r++) {

      for(i = 0; i < N ; i++)
	for(j = 0; j < N; j++) {
	  double acum = c[i][j];
	  for(k = 0; k < N; k++)
	    acum += a[i][k] * b[k][j];
	  c[i][j] = acum;
	}
	  
  }
  
  myGetTime(&f2, &fl2);

  printf("Tu: %lf  Tlib: %lf\n", f2-f1, fl2-fl1);
  printf("Test: %lf\n", test_reset());

  myGetTime(&f1, &fl1);

  /* Código optimizado */  
  for(r = 0; r < NUM_REPS; r++) {
    for(jj = 0; jj < N ; jj += T) {
      for(kk = 0; kk < N ; kk += T) {
	for(i = 0; i < N ; i++)
	  for(j = jj; j < min(jj+T, N); j++) {
	    double acum = c[i][j];
	    for(k = kk; k < min(kk+T, N); k++)
	      acum += a[i][k] * b[k][j];
	    c[i][j] = acum;
	  }
      }    
    }
  }

  myGetTime(&f2, &fl2);
  printf("Tu: %lf  Tlib: %lf\n", f2-f1, fl2-fl1);
  printf("Test: %lf\n", test_reset());
}
