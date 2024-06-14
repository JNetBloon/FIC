#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#define TAM_MAX 1000

typedef int ** matriz;

matriz crearMatriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}
/* Inicializacion pseudoaleatoria [1..TAM_MAX] de un grafo completo
no dirigido con n nodos, representado por su matriz de adayencia */
void iniMatriz(matriz m, int n) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % TAM_MAX + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}
void liberarMatriz(matriz m, int n) {
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}

void printMatriz(matriz m, int n){
    int i, j;
    for (i=0; i<n; i++){
        for (j=0; j<n; j++){
            printf("%d  ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void dijkstra(matriz grafo, matriz distancias, int tam) {
    int n, i, j, min, v=0;
    int *noVisitados = malloc(tam*sizeof(int));

    for (n=0; n<tam; n++) {
        noVisitados[n] = 0;
        for (i=0; i<tam; i++) {
            noVisitados[i] = 1;
            distancias[n][i] = grafo[n][i];
        }
        noVisitados[n] = 0;
        for(i=0; i<tam-2; i++){
            min = TAM_MAX;

            // Nodo de noVisitados que minimiza Distancias[m, v]
            for (j = 0; j < tam; j++) {
                if (noVisitados[j] == 1 && distancias[n][j] < min) {
                    min = distancias[n][j];
                    v = j;
                }
            }
            noVisitados[v]=0;
            //Para cada w no visitado
            for (j = 0; j < tam; j++) {
                if (noVisitados[j] == 1 && distancias[n][v] + grafo[v][j] < distancias[n][j]) {
                    distancias[n][j] = distancias[n][v] + grafo[v][j];
                }
            }
        }
    }
    free(noVisitados);
}

void iniMatrizTest(matriz m, int n, int v[n][n]) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
            m[i][j] = v[i][j];
}

//---PARTE DE CALCULOS---

void inicializar_semilla() {
    srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void algoritmoSub(int n[],double x,double y,double z){
    double t, ta, tb;
    double t1, t2;
    int K = 1000; //Debe ser multiplo de 10
    int i,j,l;

    printf("%10s   %12s   %7s%.3lf  %7s%.3lf  %7s%.3lf\n", "n", "t(n)", "t(n)/n^",
           x, "t(n)/n^", y, "t(n)/n^", z);

    for(i=0; i<8; i++) {
        matriz m = crearMatriz(n[i]);
        matriz dist = crearMatriz(n[i]);
        iniMatriz(m, n[i]);

        ta = microsegundos();
        dijkstra(m, dist, n[i]);
        tb = microsegundos();
        t = tb - ta;

        if (t < 500) { //Necesario volver a calcular el tiempo
            ta = microsegundos();
            for(j=0; j<K; j++){
                iniMatriz(m, n[i]);
                dijkstra(m, dist, n[i]);
            }
            tb=microsegundos();
            t1=tb-ta;

            ta=microsegundos();
            for(l=0; l<K; l++){
                iniMatriz(m, n[i]);
            }
            tb=microsegundos();
            t2=tb-ta;
            t=(t1-t2)/K;
            printf("(*)");
        } else printf("   ");

        printf("%7d   %12.4lf   %10.8lf   %10.8lf   %10.8lf\n", n[i], t, t / pow(n[i], x),
               t / pow(n[i], y), t / pow(n[i], z));

        liberarMatriz(m, n[i]);
        liberarMatriz(dist, n[i]);
    }
    puts("\n");
}

void tiempos(){
    int n[8] = {9,18,37, 75, 125, 250, 500, 1000}; //Establece el tamaño del vector que se usa
    int i;

    printf("*************************************\n");
    printf("---CALCULO DE TIEMPOS DE EJECUCION---\n");
    printf("*************************************\n\n\n");

    for(i=0; i<5; i++){
        printf("->ITERACION NUMERO %d\n\n", i+1);
        printf("Tiempo de inicializacion y ordenacion\n");
        algoritmoSub(n,2.5,2.775,3); //Despues asegurar lo de las columnas
    }
}

void test(int n1, int v1[n1][n1], int n2, int v2[n2][n2]) {

    matriz m1 = crearMatriz(n1);
    matriz distancia1 = crearMatriz(n1);
    matriz m2 = crearMatriz(n2);
    matriz distancia2 = crearMatriz(n2);

    printf("---TEST PRIMER EJEMPLO GRAFO / FIGURA 2---\n");
    iniMatrizTest(m1, n1, v1);
    printf("<MATRIZ DE ADYACENCIAS>\n");
    printMatriz(m1, n1);
    printf("<MATRIZ DISTANCIAS MINIMAS>\n");
    dijkstra(m1, distancia1, n1);
    printMatriz(distancia1, n1);

    printf("---TEST SEGUNDO EJEMPLO GRAFO / FIGURA 3---\n");
    iniMatrizTest(m2, n2, v2);
    printf("<MATRIZ DE ADYACENCIAS>\n");
    printMatriz(m2, n2);
    printf("<MATRIZ DISTACIAS MINIMAS>\n");
    dijkstra(m2, distancia2, n2);
    printMatriz(distancia2, n2);

    liberarMatriz(m1, n1);
    liberarMatriz(distancia1, n1);
    liberarMatriz(m2, n2);
    liberarMatriz(distancia2, n2);

}

int main() {
    int n1 = 4;
    int v1[4][4] = {
            {0, 1, 4, 7},
            {1, 0, 2, 8},
            {4, 2, 0, 3},
            {7, 8, 3, 0}
    };
    int n2 = 5;
    int v2[5][5] = {
            {0, 1, 8, 4, 7},
            {1, 0, 2, 6, 5},
            {8, 2, 0, 9, 5},
            {4, 6, 9, 0, 3},
            {7, 5, 5, 3, 0}

    };
    inicializar_semilla();

    test(n1, v1, n2, v2);
    tiempos();
}



