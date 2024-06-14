#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "options.h"

struct nums {
    long *increase;
    long *decrease;
    long total;
    long iterations_compartido;
    int sizeV;
    pthread_mutex_t mutexIterador;
    pthread_mutex_t *mutex;  //Mutex definido para proteger los contadores
};

struct args {
    int thread_num;		// application defined thread #
    long iterations;	// number of operations
    struct nums *nums;	// pointer to the counters (shared with other threads)
};

struct thread_info {
    pthread_t    id;    // id returned by pthread_create()
    struct args *args;  // pointer to the arguments
};

// Función para obtener un número aleatorio entre min y max (inclusive)
void random_range(int *pos, int *pos2, int max) {
    do {
        *pos = rand() % max;
        *pos2 = rand() % max;
    } while (*pos == *pos2);
}
// Threads run on this function
void *decrease_increase(void *ptr)
{
    struct args *args = ptr;
    struct nums *n = args->nums;
    int pos, pos2;
    while(1) {
        random_range(&pos, &pos2, args->nums->sizeV);

        // Ordenar los mutex para evitar el interbloqueo
        if (pos < pos2) {
            pthread_mutex_lock(&n->mutex[pos]);
            pthread_mutex_lock(&n->mutex[pos2]);
        } else {
            pthread_mutex_lock(&n->mutex[pos2]);
            pthread_mutex_lock(&n->mutex[pos]);
        }

        pthread_mutex_lock(&n->mutexIterador);
        if(n->iterations_compartido >= args->iterations){
            pthread_mutex_unlock(&n->mutex[pos]);
            pthread_mutex_unlock(&n->mutex[pos2]);
            pthread_mutex_unlock(&n->mutexIterador);
            break;
        }
        n->iterations_compartido++; // Aumentar el contador compartido
        pthread_mutex_unlock(&n->mutexIterador);

        n->decrease[pos]--;
        n->increase[pos2]++;
        pthread_mutex_unlock(&n->mutex[pos]);
        pthread_mutex_unlock(&n->mutex[pos2]);
    }
    return NULL;
}

void *increase(void *ptr)
{
    struct args *args = ptr;
    struct nums *n = args->nums;
    int pos, pos2;
    while(1) {
        random_range(&pos, &pos2, args->nums->sizeV);

        // Ordenar los mutex para evitar el interbloqueo
        if (pos < pos2) {
            pthread_mutex_lock(&n->mutex[pos]);
            pthread_mutex_lock(&n->mutex[pos2]);
        } else {
            pthread_mutex_lock(&n->mutex[pos2]);
            pthread_mutex_lock(&n->mutex[pos]);
        }

        pthread_mutex_lock(&n->mutexIterador);
        if(n->iterations_compartido >= args->iterations){
            pthread_mutex_unlock(&n->mutex[pos]);
            pthread_mutex_unlock(&n->mutex[pos2]);
            pthread_mutex_unlock(&n->mutexIterador);
            break;
        }
        n->iterations_compartido++; // Aumentar el contador compartido
        pthread_mutex_unlock(&n->mutexIterador);

        n->increase[pos]--;
        n->increase[pos2]++;
        pthread_mutex_unlock(&n->mutex[pos]);
        pthread_mutex_unlock(&n->mutex[pos2]);
    }
    return NULL;
}

void *decrease(void *ptr)
{
    struct args *args = ptr;
    struct nums *n = args->nums;
    int pos, pos2;
    while(1) {
        random_range(&pos, &pos2, args->nums->sizeV);

        // Ordenar los mutex para evitar el interbloqueo
        if (pos < pos2) {
            pthread_mutex_lock(&n->mutex[pos]);
            pthread_mutex_lock(&n->mutex[pos2]);
        } else {
            pthread_mutex_lock(&n->mutex[pos2]);
            pthread_mutex_lock(&n->mutex[pos]);
        }

        pthread_mutex_lock(&n->mutexIterador);
        if(n->iterations_compartido >= args->iterations){
            pthread_mutex_unlock(&n->mutex[pos]);
            pthread_mutex_unlock(&n->mutex[pos2]);
            pthread_mutex_unlock(&n->mutexIterador);
            break;
        }
        n->iterations_compartido++; // Aumentar el contador compartido
        pthread_mutex_unlock(&n->mutexIterador);

        n->decrease[pos]--;
        n->decrease[pos2]++;
        pthread_mutex_unlock(&n->mutex[pos]);
        pthread_mutex_unlock(&n->mutex[pos2]);
    }
    return NULL;
}

// start opt.num_threads threads running on decrease_incresase
struct thread_info *start_threads(struct options opt, struct nums *nums)
{
    int i;
    struct thread_info *threads;

    printf("creating %d threads\n", opt.num_threads);
    threads = malloc(sizeof(struct thread_info) * opt.num_threads);

    if (threads == NULL) {
        printf("Not enough memory\n");
        exit(1);
    }

    // Create num_thread threads running decrease_increase
    for (i = 0; i < opt.num_threads; i++) {
        threads[i].args = malloc(sizeof(struct args));

        threads[i].args->thread_num = i;
        threads[i].args->nums       = nums;
        threads[i].args->iterations = opt.iterations;

        if (0 != pthread_create(&threads[i].id, NULL, decrease_increase, threads[i].args)) {
            printf("Could not create thread #%d", i);
            exit(1);
        }
    }

    return threads;
}

struct thread_info *start_threadsE5(struct options opt, struct nums *nums)
{
    int i;
    struct thread_info *threads;

    printf("creating %d threads\n", opt.num_threads);
    threads = malloc(sizeof(struct thread_info) * opt.num_threads);

    if (threads == NULL) {
        printf("Not enough memory\n");
        exit(1);
    }

    // Create num_thread threads running decrease_increase
    for (i = 0; i < opt.num_threads; i++) {
        threads[i].args = malloc(sizeof(struct args));

        threads[i].args->thread_num = i;
        threads[i].args->nums       = nums;
        threads[i].args->iterations = opt.iterations;

        if (0 != pthread_create(&threads[i].id, NULL, decrease, threads[i].args)) {
            printf("Could not create thread #%d", i);
            exit(1);
        }
    }

    return threads;
}

void print_totals(struct nums *nums)
{
    long total_increase = 0;
    long total_decrease = 0;
    for (int i = 0; i < nums->sizeV; ++i) {
        total_increase += nums->increase[i];
        total_decrease += nums->decrease[i];
    }
    printf("Final: increasing %ld decreasing %ld diff %ld\n",
           total_increase, total_decrease, nums->total * nums->sizeV - (total_increase + total_decrease));
}

// wait for all threads to finish, print totals, and free memory
void wait(struct options opt, struct nums *nums, struct thread_info *threads, struct thread_info *threadsE4, struct thread_info *threadsE5) {
    // Wait for the threads to finish
    for (int i = 0; i < opt.num_threads; i++){
        pthread_join(threads[i].id, NULL);
        pthread_join(threadsE4[i].id, NULL);
        pthread_join(threadsE5[i].id, NULL);
    }

    print_totals(nums);

    free(nums->increase);
    free(nums->decrease);

    for (int i = 0; i < nums->sizeV; i++) {
        pthread_mutex_destroy(&nums->mutex[i]);
    }
    free(nums->mutex);
    pthread_mutex_destroy(&nums->mutexIterador);

    //Threads
    for (int i = 0; i < opt.num_threads; i++){
        free(threads[i].args);
        free(threadsE5[i].args);
    }
    free(threads);
    free(threadsE5);
}

void inicializarVectores(struct nums *n, int total, int tam) {
    int i;
    n->increase = (long *)malloc(tam * sizeof(long));
    n->decrease = (long *)malloc(tam * sizeof(long));
    n->mutex = (pthread_mutex_t *)malloc(tam * sizeof(pthread_mutex_t));
    n->sizeV = (int) tam;
    for (i = 0; i < tam; i++) {
        n->increase[i] = 0;
        n->decrease[i] = total;
        pthread_mutex_init(&n->mutex[i], NULL);
    }
    pthread_mutex_init(&n->mutexIterador, NULL);
}

int main (int argc, char **argv)
{
    struct options opt;
    struct nums nums;
    struct thread_info *thrs;
    struct thread_info *thrsE5;


    srand(time(NULL));

    // Default values for the options
    opt.num_threads  = 4;
    opt.iterations   = 100000;
    opt.size         = 10;

    read_options(argc, argv, &opt);

    nums.total = opt.iterations;
    //Arrays de increase y decrease
    inicializarVectores(&nums, opt.iterations, opt.size);

    nums.iterations_compartido = 0;

    //Thread en el stack del main
    struct thread_info thrsE4[opt.num_threads];
    struct args argE4[opt.num_threads];

    for (int i = 0; i < opt.num_threads; i++) {
        argE4[i].thread_num = i;
        argE4[i].iterations = opt.iterations;
        argE4[i].nums = &nums;
        thrsE4[i].args = &argE4[i];

        if (0 != pthread_create(&thrsE4[i].id, NULL, increase, thrsE4[i].args)) {
            printf("Could not create thread #%d", i);
            exit(1);
        }
    }

    thrs = start_threads(opt, &nums);
    thrsE5 = start_threadsE5(opt, &nums);
    wait(opt, &nums, thrs, thrsE4, thrsE5);

    return 0;
}