//Camiña Caneda, Jaime	//	j.caneda@udc.es
//Berreco Kozlan, Ángel	//	angel.kozlan@udc.es

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <sys/time.h>
#define TAM 256000

struct monticulo{
    int ultimo;
    int vector[TAM];
};

typedef struct monticulo * pmonticulo;

bool MonticuloVacio(pmonticulo m){
    return m->ultimo==-1;
}

void Hundir (pmonticulo m, int i){
    int HijoIzq, HijoDer;
    int j, temp;
    do{
        HijoIzq = 2*i + 1; //2*(i+1)-1;
        HijoDer = 2*i + 2; //2*(i+1)-1+1;
        j = i;
        if (HijoDer <= m->ultimo && m->vector[HijoDer] < m->vector[i]) {
            i = HijoDer;
        }
        if (HijoIzq <= m->ultimo && m->vector[HijoIzq] < m->vector[i]) {
            i = HijoIzq;
        }
        temp = m->vector[j];
        m->vector[j] = m->vector[i];
        m->vector[i] = temp;

    } while (j!=i);
}

void crearMonticulo (int const v[], int n, pmonticulo m){
    int i, j;

    if(n>TAM){
        printf("Tamaño maximo excedido");
        return;
    }
    m->ultimo=n-1;

    for (i = 0; i < n; ++i) {
        m->vector[i]=v[i];
    }
    for (j = (n - 1) / 2; j >= 0; j--){
        Hundir(m, j);
    }
}

int quitarMenor(pmonticulo m) {
    int x;
    if(MonticuloVacio(m))
        return -1;
    else{
        x = m->vector[0];
        m->vector[0] = m->vector[m->ultimo];
        m->ultimo--;
        if(m->ultimo>=0){
            Hundir(m, 0);
        }
    }
    return x;
}

void imprimirV(int v [], int n){
    int i;
    for(i=0; i<n; i++){
        printf("%d  ", v[i]);
    }
    printf("\n");
}

void printVector(pmonticulo  m) {
    int i;
    for (i=0; i <=m->ultimo; i++){
        printf("%d  ", m->vector[i]);
    }
    printf("\n");
}

void printMonticulo(pmonticulo m) {
    int i;
    for (i = 0; i <= m->ultimo; i++) {
        printf("%d ",  m->vector[i]);
        switch (i) {
            case 0:
            case 2:
            case 6:
            case 14:
                printf("\n");
                break;
            default:
                break;
        }
    }
    printf("\n\n");
}

void OrdenarPorMonticulosTest(int v[], int n, pmonticulo m){
    int i;
    crearMonticulo(v, n, m);

    printMonticulo(m); //Esto hay que borrarlo, es para verlo nosotros de mejor forma
    printf("Vector introducido: ");
    imprimirV(v, n);
    printf("Vector del monticulo: ");
    printVector(m);
    printf("El monticulo esta vacio? %s\n", MonticuloVacio(m)?"Si":"No");
    for(i = 0; i < n; i++){
        v[i] = quitarMenor(m);
    }
    printf("El monticulo esta vacio quitando todos los menores? %s\n",
           MonticuloVacio(m)?"Si":"No");
}

void OrdenarPorMonticulos(int v[], int n, pmonticulo m){
    int i;
    crearMonticulo(v, n, m);
    for(i = 0; i < n; i++){
        v[i] = quitarMenor(m);
    }
}


//---PARTE DE CALCULOS---

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

void ascendente(int v [], int n){
    int i;
    for (i = 0; i <n ; ++i) {
        v[i]=i;
    }
}

void descendente(int v [], int n){
    int i,j;
    for (i = n-1, j=0; j <n ; i--,j++) {
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

void algoritmoSub(int (*algo)(int [],int, pmonticulo), int (*mode)(int [], int),int vector [],
                  int n [],double x,double y,double z, pmonticulo m){
    double t, ta, tb;
    double t1, t2;
    int K = 1000; //Debe ser multiplo de 10
    int i,j,l;

    printf("%10s   %10s   %7s%.2lf  %8s%.2lf  %8s%.2lf\n", "n", "t(n)", "t(n)/n^",
           x, "t(n)/n^", y, "t(n)/n^", z);

    for(i=0; i<10; i++) { //Repetirá el proceso 7 veces

        mode(vector, n[i]);
        ta = microsegundos();
        algo(vector,n[i], m);
        tb = microsegundos();
        t = tb - ta;

        if (t < 500) { //Necesario volver a calcular el tiempo
            ta = microsegundos();
            for(j=0; j<K; j++){
                mode(vector,n[i]);
                algo(vector,n[i], m);
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
        } else printf("   ");

        printf("%7d   %10.4lf   %10.9lf   %10.9lf   %10.9lf\n", n[i], t, t / pow(n[i], x),
               t / pow(n[i], y), t / pow(n[i], z));
    }puts("\n");
}

void tiempos(pmonticulo m){
    int vector[257000]; //Vector
    int n[10] = {500, 1000, 2000, 4000,8000,16000,32000, 64000,
                 128000,256000}; //Establece el tamaño del vector que se usa
    int i;

    printf("*************************************\n");
    printf("---CALCULO DE TIEMPOS DE EJECUCION---\n");
    printf("*************************************\n\n\n");

    for(i=0; i<5; i++){ //Lo repetira 5 veces
        printf("->ITERACION NUMERO %d\n\n", i+1);
        printf("Tiempo creacion montículo con inicialización aleatoria\n");
            algoritmoSub((int (*)(int *, int, pmonticulo)) crearMonticulo,(int (*)(int *, int))
            aleatorio, vector, n,1,1,1, m);
        printf("Ordenacion por monticulos con inicializacion aleatoria\n");
            algoritmoSub((int (*)(int *, int, pmonticulo)) OrdenarPorMonticulos, (int (*)(int *, int))
            aleatorio, vector, n,0.9,1.1,1.4, m);
        printf("Tiempo creacion montículo con inicialización ascendente\n");
            algoritmoSub((int (*)(int *, int, pmonticulo)) crearMonticulo, (int (*)(int *, int))
            ascendente, vector, n,1,1,1, m);
        printf("Ordenacion por monticulos con inicializacion ascendente\n");
            algoritmoSub((int (*)(int *, int, pmonticulo)) OrdenarPorMonticulos, (int (*)(int *, int))
            ascendente, vector, n,0.8,1.055,1.2, m);
        printf("Tiempo creacion montículo con inicialización descendente\n");
            algoritmoSub((int (*)(int *, int, pmonticulo)) crearMonticulo, (int (*)(int *, int))
            descendente, vector, n,1,1,1, m);
        printf("Ordenacion por monticulos con inicializacion descendente\n");
            algoritmoSub((int (*)(int *, int, pmonticulo)) OrdenarPorMonticulos, (int (*)(int *, int))
            descendente, vector, n,0.9,1.05,1.2, m);

    }
}


void test(pmonticulo m) {
    int n = 15;
    int v[15];
    printf("---TEST MONTICULO CON INICIALIZACION ALEATORIA---\n");
    aleatorio(v, n);
    OrdenarPorMonticulosTest(v, n, m);
    printf("Vector ordenado: ");
    imprimirV(v, n);
    printf("El vector esta ordenado? %s\n\n", ordenado(v, n)?"Si":"No");

    printf("---TEST MONTICULO CON INICIALIZACION ASCENDENTE---\n");
    ascendente(v, n);
    OrdenarPorMonticulosTest(v, n, m);
    printf("Vector ordenado: ");
    imprimirV(v, n);
    printf("El vector esta ordenado? %s\n\n", ordenado(v, n)?"Si":"No");

    printf("---TEST MONTICULO CON INICIALIZACION DESCENDENTE---\n");
    descendente(v, n);
    OrdenarPorMonticulosTest(v, n, m);
    printf("Vector ordenado: ");
    imprimirV(v, n);
    printf("El vector esta ordenado? %s\n\n", ordenado(v, n)?"Si":"No");
}

int main() {
    char espera[10];
    pmonticulo m = malloc(sizeof(struct monticulo));

    inicializar_semilla();

    test(m);
    tiempos(m);

    free(m);

    printf("Pulse 'enter' para salir del programa\n");
    fgets(espera, sizeof(espera), stdin);
}
