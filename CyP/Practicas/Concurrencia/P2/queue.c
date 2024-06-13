#include <stdlib.h>
#include <pthread.h>

// circular array
typedef struct _queue {
    int size;
    int used;
    int first;
    void **data;
    pthread_mutex_t *mutex;
    pthread_cond_t emptyQueue;
    pthread_cond_t fullQueue;
} _queue;

#include "queue.h"

queue q_create(int size) {
    queue q = malloc(sizeof(_queue));

    q->size  = size;
    q->used  = 0;
    q->first = 0;
    q->data  = malloc(size*sizeof(void *));

    q->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(q->mutex, NULL);

    pthread_cond_init(&q->emptyQueue, NULL);
    pthread_cond_init(&q->fullQueue, NULL);

    return q;
}

int q_elements(queue q) {
    return q->used;
}

int q_insert(queue q, void *elem) {
    pthread_mutex_lock(q->mutex);
    while (q->size == q->used) {
        pthread_cond_wait(&q->fullQueue, q->mutex);
    }
    q->data[(q->first+q->used) % q->size] = elem;
    q->used++;

    if(q->used==1) pthread_cond_broadcast(&q->emptyQueue);
    pthread_mutex_unlock(q->mutex);

    return 1;
}

void *q_remove(queue q) {
    void *res;

    pthread_mutex_lock(q->mutex);
    while(q_elements(q)==0)
        pthread_cond_wait(&q->emptyQueue, q->mutex);

    res = q->data[q->first];
    q->first = (q->first+1) % q->size;
    q->used--;

    if(q->used==q->size-1)
        pthread_cond_broadcast(&q->fullQueue);
    pthread_mutex_unlock(q->mutex);

    return res;
}

void q_destroy(queue q) {
    free(q->data);
    pthread_mutex_destroy(q->mutex);
    pthread_cond_destroy(&q->emptyQueue);
    pthread_cond_destroy(&q->fullQueue);
    free(q->mutex);
    free(q);
}
