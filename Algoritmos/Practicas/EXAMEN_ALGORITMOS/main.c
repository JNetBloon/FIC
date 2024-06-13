//Camiña Caneda, Jaime	//	j.caneda@udc.es
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <sys/time.h>

void intercambiar(int v[], int a, int b){
    int tmp = v[a];
    v[a] = v[b];
    v[b] = tmp;
}

void auxOrd(int v [], int izq, int der){
    int piv, i, j;
    if(izq < der){
        piv = v[der-1];
        intercambiar(v, izq, der-1);
        i = izq + 1;
        j = der;
        while (i<=j){
            while (v[j] > piv) {j = j-1;}
            while (i <= der && v[i] < piv) {i = i+1;}
            if (i<=j) {
                intercambiar(v, i, j);
                i = i + 1;
                j = j - 1;
            }
        }
        intercambiar(v, izq, j);
        auxOrd(v, izq, j-1);
        auxOrd(v, j+1, der);
    }
}

void ordenar(int v [], int n){
    auxOrd(v, 0, n-1);
}

void inicializar_semilla() {
    srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}
void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
/* se generan números pseudoaleatorio entre -n y +n */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void imprimirV(int v [], int n){
    int i;
    for(i=0; i<n; i++){
        printf("%d  ", v[i]);
    }
    puts("\n");
}
void ascendente(int v [], int n){
    int i;
    for (i = 0; i <n ; ++i) {
        v[i]=i+1;
    }
}

void descendente(int v [], int n){
    int i,j;
    for (i = n, j=0; j <n ; i--,j++) {
        v[j]=i;
    }
}

bool ordenado(const int v[], int n){
    int i;
    for(i=0; i<n-1; i++){
        if(v[i] > v[i+1])
            return false;
    }
    return true;
}

void algoritmoSub(int (*algo)(int [],int), int (*mode)(int [], int),int vector [],
                  int n [],double x,double y,double z){
    double t, ta, tb;
    double t1, t2;
    int K = 1000; //Debe ser multiplo de 10
    int i,j,l;

    printf("%9s   %14s   %9s%.3lf  %9s%.3lf  %9s%.3lf\n", "n",
           "t(n)", "t(n)/n^", x, "t(n)/n^", y, "t(n)/n^", z);

    for(i=0; i<8; i++) {
        mode(vector, n[i]);
        ta = microsegundos();
        algo(vector,n[i]);
        tb = microsegundos();
        t = tb - ta;
        if (t < 500) { //Necesario volver a calcular el tiempo
            ta = microsegundos();
            for(j=0; j<K; j++){
                mode(vector,n[i]);
                algo(vector,n[i]);
            }
            tb=microsegundos();
            t1=tb-ta;

            ta=microsegundos();
            for(l=0; l<K; l++){
                mode(vector,n[i]);
            }
            tb=microsegundos();
            t2=tb-ta;
            t=(t1-t2)/K;
            printf("(*)");
        }
        printf("%9d   %14.5lf   %12.9lf   %12.9lf   %12.9lf\n", n[i], t, t / pow(n[i], x),
               t / pow(n[i], y), t / pow(n[i], z));
    }
    puts("\n\n");
}

void tiempos(){
    int vector[64000]; //Vector
    int n[9] = {500, 1000, 2000, 4000,8000,16000,32000, 64000}; //Dimensiones
    int i;

    for(i=0; i<5; i++){
        printf("-ITERACION NUMERO %d - ordenar()-\n", i+1);
        printf("Vector con inicializacion aleatoria\n");
        algoritmoSub((int (*)(int *, int)) ordenar, (int (*)(int *, int))aleatorio,
                     vector, n,0.9,1.11,1.3);
        printf("Vector con inicializacion ascendente\n");
        algoritmoSub((int (*)(int *, int)) ordenar, (int (*)(int *, int)) ascendente,
                     vector, n,1.8,2.0,2.2);
        printf("Vector con inicializacion descendente\n");
        algoritmoSub((int (*)(int *, int)) ordenar, (int (*)(int *, int)) descendente,
                     vector, n,1.8,1.99,2.2);
    }
}

void test(int (*algo)(int [],int)){
    int vector[17];
    int n = 17;

    printf("->INICIALIZACION ALEATORIA\n");
    aleatorio(vector, n);
    imprimirV(vector, n);
    printf("ordenado?  %s\n", ordenado(vector, n)?"1":"0");
    printf("Vector ordenado por el algoritmo:\n");
    algo(vector, n);
    imprimirV(vector, n);
    printf("ordenado? %s\n\n", ordenado(vector, n)?"1":"0");

    printf("->INICIALIZACION DESCENDENTE\n");
    descendente(vector,n);
    imprimirV(vector, n);
    printf("ordenado? %s\n\n", ordenado(vector, n)?"1":"0");
    printf("Vector ordenado por el algoritmo:\n");
    algo(vector, n);
    imprimirV(vector, n);
    printf("ordenado? %s\n\n", ordenado(vector, n)?"1":"0");
}

int main() {
    char espera[10];
    inicializar_semilla();

    puts("----TEST ORDEN----\n");
    test((int (*)(int *, int)) ordenar);
    puts("----TIEMPOS----\n");
    tiempos();

    printf("Pulse 'enter' para salir del programa\n");
    fgets(espera, sizeof(espera), stdin);

}
