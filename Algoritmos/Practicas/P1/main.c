//Camiña Caneda, Jaime	//	j.caneda@udc.es
//Berreco Kozlan, Ángel	//	angel.kozlan@udc.es

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include <sys/time.h>

/*      Algoritmos correspondientes al programa     */

int sumaSubMax1(int const vector[], int n){
    int sumaMax=0, estaSuma;
    int i, j;
    for(i=0; i<n; i++){
        estaSuma=0;
        for(j=i; j<n; j++){
            estaSuma += vector[j];
            if(estaSuma>sumaMax){
                sumaMax=estaSuma;
            }
        }
    }
    return sumaMax;
}

int sumaSubMax2(int const vector[], int n){
    int sumaMax=0, estaSuma=0;
    int j;

    for(j=0; j<n; j++){
        estaSuma += vector[j];
        if(estaSuma>sumaMax){
            sumaMax=estaSuma;
        } else if (estaSuma<0){
            estaSuma=0;
        }
    }
    return sumaMax;
}

/*      Funciones para medir el programa        */

void inicializar_semilla() {
    srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}
void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=1; i <= n; i++)
        v[i] = (rand() % m) - n;
/* se generan números pseudoaleatorio entre -n y +n */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

/*      Código encargado de la ejecución del programa       */

void algoritmoSub(int (*algo)(int [],int),int vector [], int n [],double x,double y,double z){
    double t, ta, tb;
    double t1, t2;
    int i, j, l;
    int K = 100; //Debe ser multiplo de 10

    for(i=0; i<7; i++) { //Repetirá el proceso 7 veces

        aleatorio(vector, n[i]);
        ta = microsegundos();
        algo(vector,n[i]);
        tb = microsegundos();
        t = tb - ta;

        if (t < 500) { //Necesario volver a calcular el tiempo
            ta = microsegundos();
            for(j=0; j<K; j++){
                aleatorio(vector,n[i]);
                algo(vector,n[i]);
            }
            tb=microsegundos();
            t1=tb-ta;

            ta=microsegundos();
            for(l=0; l<K; l++){
                aleatorio(vector,n[i]);
            }
            tb=microsegundos();
            t2=tb-ta;
            t=(t1-t2)/K;
        }
        printf("%7d   %12.3lf   %10.6lf   %10.6lf   %10.6lf\n", n[i], t, t / pow(n[i], x), t / pow(n[i], y), t / pow(n[i], z));
    }
    puts("\n\n");
}

void tiempos(){
    int vector[33000]; //Almacena la secuencia de numeros. Como usamos un máximo de 32000, reservaremos 33000 para evitar problemas en los cálculos
    int n[7] = {500, 1000, 2000, 4000,8000,16000,32000}; //Establece el tamaño del vector
    int i;

    for(i=0; i<5; i++){ //Lo repetira 5 veces
        puts("SUBMAX1\n");
        printf("%7s   %12s   %10s   %10s   %10s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");
        algoritmoSub((int (*)(int *, int)) sumaSubMax1, vector, n,1.8,2,2.2); //SubMax1
        puts("SUBMAX2\n");
        printf("%7s   %12s   %10s   %10s   %10s\n", "n", "t(n)", "t(n)/n^0.9", "t(n)/n^1", "t(n)/n^1.2");
        algoritmoSub((int (*)(int *, int)) sumaSubMax2, vector, n, 0.8,1,1.2); //SubMax2
    }
}

void auxiliar_test(int v[5], int n) {
    int i;
    printf("Secuencia: [");
    for (i=0; i<n; i++) {
        printf(" %d ", v[i]);
    }
    printf("]");
    printf("\n\tRESULTADO SUMASUBMAX1 : %d\n\t", sumaSubMax1(v, n));
    printf("RESULTADO SUMASUBMAX2 : %d\n\n", sumaSubMax2(v, n));
}

void test() {
    int n = 5;
    int s1[5] = {-9, 2, -5, -4, 6};
    int s2[5] = {4, 0, 9, 2, 5};
    int s3[5] = {-2, -1, -9, -7, -1};
    int s4[5] = {9, -2, 1, -7, -8};
    int s5[5] = {15, -2, -5, -4, 16};
    int s6[5] = {7, -5, 6, 7, -7};
    auxiliar_test(s1, n);
    auxiliar_test(s2, n);
    auxiliar_test(s3, n);
    auxiliar_test(s4, n);
    auxiliar_test(s5, n);
    auxiliar_test(s6, n);
}

void test2() {
    int i;
    int n = 9;
    int v[9];

    for(i = 0; i < 10; i++) {
        aleatorio(v, n);
        auxiliar_test(v, n);
    }

}

int main() {
    char espera[10];

    inicializar_semilla();
    printf("------------TEST 1 CON VECTORES DEFINIDOS------------\n");
    test();

    printf("------------TEST 2 CON VECTORES ALEATORIOS------------\n");
    test2();

    tiempos();

    printf("Pulse 'enter' para salir del programa\n");
    fgets(espera, sizeof(espera), stdin);

    return 0;
}
