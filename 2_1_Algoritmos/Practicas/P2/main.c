#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <sys/time.h>

void ord_ins (int v [], int n){
    int i,x,j;
    for (i = 1; i <n ; ++i) {
        x=v[i];
        j=i-1;
        while (j>=0 && v[j]>x){
            v[j+1]=v[j];
            j=j-1;
        }
        v[j+1]=x;
    }
}

void ord_shell (int v [], int n){
    int incremento=n;
    int i,j,tmp;
    bool seguir;
    do{
        incremento= incremento/2;
        for (i =incremento; i <n ; i++) {
            tmp=v[i];
            j=i;
            seguir=true;

            while (j-incremento>=0 && seguir){
                if(tmp<v[j-incremento]){
                    v[j]=v[j-incremento];
                    j=j-incremento;
                }else
                    seguir=false;
            }
            v[j] = tmp;
        }
    }while(incremento!=1);
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

    for(i=0; i<9; i++) { //Repetirá el proceso 7 veces
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
    int vector[129000]; //Vector
    int n[9] = {500, 1000, 2000, 4000,8000,16000,32000, 64000, 128000};
    int i;

    for(i=0; i<4; i++){ //Lo repetira 5 veces
        //Insercion
        puts("---Ordenacion por Insercion---\n");
        printf("Ordenacion por insercion con inicializacion aleatoria\n");
        algoritmoSub((int (*)(int *, int)) ord_ins, (int (*)(int *, int))aleatorio,
                     vector, n,1.8,2,2.2);
        printf("Ordenacion por insercion con inicializacion ascendente\n");
        algoritmoSub((int (*)(int *, int)) ord_ins, (int (*)(int *, int)) ascendente,
                     vector, n,0.6,1,1.4);
        printf("Ordenacion por insercion con inicializacion descendente\n");
        algoritmoSub((int (*)(int *, int)) ord_ins, (int (*)(int *, int)) descendente,
                     vector, n,1.8,2,2.2);
        //Shell
        puts("---Ordenacion por Shell---\n");
        printf("Ordenacion por shell con inicializacion aleatoria\n");
        algoritmoSub((int (*)(int *, int)) ord_shell, (int (*)(int *, int)) aleatorio,
                     vector, n,1,1.175,1.4);
        printf("Ordenacion por shell con inicializacion ascendente\n");
        algoritmoSub((int (*)(int *, int)) ord_shell, (int (*)(int *, int)) ascendente,
                     vector, n,0.9,1.1,1.3);
        printf("Ordenacion por shell con inicializacion descendente\n");
        algoritmoSub((int (*)(int *, int)) ord_shell, (int (*)(int *, int)) descendente,
                     vector, n,0.9,1.1,1.3);
    }
}

void test(int (*algo)(int [],int)){
    int vector[17];
    int n = 17;
    printf("Inicializacion aleatoria\n");
    aleatorio(vector, n);
    imprimirV(vector, n);
    printf("ordenado?  %s\n", ordenado(vector, n)?"1":"0");
    printf("Vector ordenado por el algoritmo\n");
    algo(vector, n);
    imprimirV(vector, n);
    printf("ordenado? %s\n\n", ordenado(vector, n)?"1":"0");

    printf("Inicializacion descendente\n");
    n=10;
    descendente(vector,n);
    imprimirV(vector, n);
    printf("ordenado? %s\n\n", ordenado(vector, n)?"1":"0");
    printf("Vector ordenado por el algoritmo\n");
    algo(vector, n);
    imprimirV(vector, n);
    printf("ordenado? %s\n\n", ordenado(vector, n)?"1":"0");

}

int main() {
    char espera[10];
    inicializar_semilla();

    puts("----TEST ORDEN POR INSERCIÓN----\n");
    test((int (*)(int *, int)) ord_ins);
    puts("----TEST ORDEN POR SHELL----\n");
    test((int (*)(int *, int)) ord_shell);
    puts("----TIEMPOS----\n");
    tiempos();

    printf("Pulse 'enter' para salir del programa\n");
    fgets(espera, sizeof(espera), stdin);

}
