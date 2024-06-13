#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

/* -O3  */

#ifndef N
#define N          200000
#endif

#ifndef INDEXES
#define INDEXES     20000
#endif

#ifndef NUM_REPS
#define NUM_REPS    10000
#endif

struct registro1 {
  double strength;
  char nombre[30];
  int tipo;
  float position[3];
  int indirs[6];
  double x, y, z;
};


struct registro1 a[N];
int indireccion[INDEXES];

void myGetTime(double *t, double *tlib)
{ struct rusage tmpru;
  
  getrusage(RUSAGE_SELF , &tmpru);
  *t = tmpru.ru_utime.tv_sec+(tmpru.ru_utime.tv_usec/1000000.0);
  *tlib = tmpru.ru_stime.tv_sec+(tmpru.ru_stime.tv_usec/1000000.0);
}

static void initData()
{ int i;

  for(i = 0; i < N; i++) {
    sprintf(a[i].nombre, "Particula %d", i);
    a[i].strength = (double)i;
    a[i].tipo =  (i % 3);
    a[i].x =  (double)(i & 0xfff);
    a[i].y = 5.0;
    a[i].position[2] = (float)(N-i) / 3.0f;
    a[i].indirs[2] = (i % 6);
    a[i].z = (double)(i+1);
  }
  
}

int main()
{ double f1, fl1, f2, fl2;
  int i, j, k;
  double tr;
  
  srandom(4 + 8 + 15 + 16 + 23 + 42);
  
  for(i = 0; i < INDEXES; i++)
    indireccion[i] = random() % N;
  
  /****************** DATOS SIN ORDEN ******************/
#ifndef NOINIT
  initData();
#endif
  tr = 0.0;
  
  myGetTime(&f1, &fl1);
  
  for(k = 0; k < NUM_REPS; k++) {
    for(i = 0; i < INDEXES ; i++) {
      tr += a[indireccion[i]].strength * a[indireccion[i]].x ;
    }
  }
  
  myGetTime(&f2, &fl2);
  
  printf("R:%lf\nTu: %lf  Tlib: %lf\n", tr/NUM_REPS, f2-f1, fl2-fl1);
  
#ifndef NOINIT
  initData();
#endif
  tr = 0.0;
  
  myGetTime(&f1, &fl1);
  
/* Código optimiado */
  
  myGetTime(&f2, &fl2);
  
  printf("R:%lf\nTu: %lf  Tlib: %lf\n", tr/NUM_REPS, f2-f1, fl2-fl1);
  
}
