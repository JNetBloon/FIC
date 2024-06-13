#include <stdio.h>
#include <math.h>
#include <mpi/mpi.h>

#include <stdio.h>
#include <sys/time.h>
#include <malloc.h>

#define DEBUG 0

#define N 5

void imprimir_matriz(int n, float vector[n]) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%f  ", vector[i*N+j]);
        }
        printf("\n");
    }
}

void imprimir_vector(int n, float vector[n]) {
    for (int i = 0; i < n; ++i) {
        printf("%f  ", vector[i]);
    }
    printf("\n");
}

void imprimir_tiempo(struct timeval tv1, struct timeval tv2) {
    int microseconds = (tv2.tv_usec - tv1.tv_usec)+ 1000000 * (tv2.tv_sec - tv1.tv_sec);
    printf ("Time (seconds) = %lf\n", (double) microseconds/1E6);
}

int main(int argc, char *argv[] ) {

    int i, j;
    float *matrix, *result;
    float vector[N];
    struct timeval tv1, tv2;

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int filas_proc = N / size;
    int resto = N % size;
    int filas_vacias;

    if (resto != 0) {
        filas_proc++;
        filas_vacias = size * filas_proc - N; //Formula para saber las filas vacias: Num proc * Num filas - Tamaño matriz
    }

    if (rank == 0) {
        matrix = malloc((N+filas_vacias) * N * sizeof(float));
        result = malloc((N+filas_vacias) * sizeof(float)); //El vector resultante tambien debe reservar espacio para la basura que calcule con las filas basura
        //Inicializar con valores los vectores
        for(i=0;i<N;i++){
            for (j = 0; j < N; ++j) {
                matrix[i*N+j] = i; //Para acceder a la matriz usamos la formula i * N + j
            }
            result[i]=0;
            vector[i]=i;
        }
    }

    float buffer_aux[filas_proc]; //Servira para almacenar el resultado del calculo del problema. Despues se mandara con Gather al vector resultado
    float buffer_matriz[filas_proc][N];
    //Para no mandar la matriz completa a cada proceso, ya que cada uno calcula una parte, se usa una matriz auxiliar mas pequeña con el tamaño que necesita

    //----Envio y distribucion
    gettimeofday(&tv1, NULL);

    MPI_Scatter(matrix, N*filas_proc, MPI_FLOAT, buffer_matriz, N*filas_proc, MPI_FLOAT,0, MPI_COMM_WORLD);
    MPI_Bcast(vector,N,MPI_FLOAT,0,MPI_COMM_WORLD);

    gettimeofday(&tv2, NULL);
    imprimir_tiempo(tv1,tv2);

    //----Calculo de el resultado
    gettimeofday(&tv1, NULL);

    for(i=0;i<N;i++) {
        buffer_aux[i]=0;
        for(j=0;j<N;j++) {
            buffer_aux[i] += buffer_matriz[i][j]*vector[j];
        }
    }

    gettimeofday(&tv2, NULL);
    imprimir_tiempo(tv1,tv2);

    //----Envio del rsultado local al vector resultado
    gettimeofday(&tv1, NULL);
    MPI_Gather(buffer_aux, filas_proc, MPI_FLOAT, result, filas_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
    gettimeofday(&tv2, NULL);
    imprimir_tiempo(tv1,tv2);

    /*Display result */
    if (rank==0){
        if (DEBUG){
            for(i=0;i<N;i++) {
                printf(" %f \t ",result[i]);
            }
        } else {
            //printf ("Time (seconds) = %lf\n", (double) microseconds/1E6);
            printf("MATRIZ\n");
            imprimir_matriz(N, matrix);
            printf("VECTOR\n");
            imprimir_vector(N, vector);
            printf("RESULTADO\n");
            imprimir_vector(N, result);
        }
    }

    MPI_Finalize();
    return 0;
}