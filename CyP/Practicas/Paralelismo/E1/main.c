#include <stdio.h>
#include <math.h>
#include <mpi/mpi.h>

int main(int argc, char *argv[]) {
    int i, done = 0, n;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x;

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    while (!done) {
        //SEPARACION
        if(rank == 0) {
            printf("--Enter the number of intervals: (0 quits)--\n");
            scanf("%d",&n);

            //Se mandan a todos los procesos las variables
            for (i = 1; i < size; i++) {
                MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (n == 0) break;

        //CALCULO
        printf("|-->Proceso %d (ID: %d) de un total de %d procesos empieza a calcular.\n", rank+1, rank, size);
        h   = 1.0 / (double) n;
        sum = 0.0;

        for (i = rank + 1; i <= n; i += size) {
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
        double local_pi = h * sum;

        //CONCLUSION
        if(rank == 0) {
            pi = local_pi;
            for (i = 1; i < size; i++) {
                MPI_Recv(&local_pi, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                pi += local_pi;
            }
            printf("<> pi is approximately %.16f, Error is %.16f <>\n\n", pi, fabs(pi - PI25DT));
        } else {
            MPI_Send(&local_pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}