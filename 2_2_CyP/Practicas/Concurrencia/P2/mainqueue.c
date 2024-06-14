#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "queue.h"

void *test1(void *ptr) {
    queue cola = ptr;
    int *e = malloc(sizeof (int));
    q_insert(cola, e);

    return NULL;
}

void *test2(void *ptr) {
    queue cola = ptr;
    q_remove(cola);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error - Usage: ./queuetest <number of threads> <size of queue>\n");
        return 1;
    }

    int num_thr = atoi(argv[1]);
    int tam_cola = atoi(argv[2]);
    pthread_t threads[num_thr];
    queue cola = q_create(tam_cola);

    for (int i = 0; i < num_thr; i+=2) {
        printf("Creating threads #%d & #%d\n", i+1, i+2);
        pthread_create(&threads[i], NULL, (void *(*)(void *)) test1, cola);
        pthread_create(&threads[i+1], NULL, (void *(*)(void *)) test2, cola);
    }

    // Esperar a que todos los threads terminen
    for (int i = 0; i < num_thr; i++) {
        pthread_join(threads[i], NULL);
    }

    q_destroy(cola);

    return 0;
}
