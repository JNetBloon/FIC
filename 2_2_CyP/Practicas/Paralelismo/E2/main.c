#include <stdio.h>
#include <math.h>
#include <mpi/mpi.h>

int MPI_BinomialColectiva(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int envia, recibe = 1;
    int i;

    if (rank != root) {
        int temp = MPI_Recv(buffer, count, datatype, MPI_ANY_SOURCE, 0, comm, MPI_STATUS_IGNORE);
        if (temp != MPI_SUCCESS) {
            return temp;
        }
    }

    for (i = 0;  ; i++) { //Sin condicion final
        if (rank < recibe) {
            envia = rank+recibe;
            if (envia >= size) break; //Comprueba que exista el proceso al que se pretende hacer un Send
            MPI_Send(buffer, count, datatype, envia, 0, comm);
        }
        recibe = recibe*2;
    }

    return  MPI_SUCCESS;
}

int MPI_FlattreeColectiva(void* send_data, void* recv_data, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    double tmp;

    if(rank == root){
        tmp += * (double *) send_data; //Primero almacenamos el valor del rank 0
        //double* convierte a send_data en un puntero double, y el *() hace una desreferenciacion del puntero al valor del double
        for (int i = 1; i < size; i++) {
            int error = MPI_Recv((double *) send_data,1,MPI_DOUBLE,i,0,comm,MPI_STATUS_IGNORE);
            if(error != MPI_SUCCESS)
                return error;
            tmp += * (double* ) send_data;
        }
    } else MPI_Send(send_data,count,datatype,0,0,comm);

    * (double *) recv_data = tmp;

    return MPI_SUCCESS;
}

int main(int argc, char *argv[])
{
    int i, done = 0, n;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x;

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    while(!done){
        if(rank == 0) {
            printf("--Enter the number of intervals: (0 quits)--\n");
            scanf("%d",&n);
            printf("-----------------------------\n");
        }

        MPI_BinomialColectiva(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        //MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

        if (n == 0) break;

        printf("|-->Proceso %d (ID: %d) de un total de %d procesos empieza a calcular -- VALOR DE N %d\n", rank+1, rank, size, n);
        h   = 1.0 / (double) n;
        sum = 0.0;

        for (i = rank + 1; i <= n; i += size) {
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
       // double local_pi = h * sum;

        MPI_FlattreeColectiva(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        //MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        pi = pi * h;
        if(rank==0) {
            printf("<> pi is approximately %.16f, Error is %.16f <>\n\n", pi, fabs(pi - PI25DT));
        }
    }

    MPI_Finalize();
}