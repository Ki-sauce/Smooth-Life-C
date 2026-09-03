#ifndef BARRIER_H
#define BARRIER_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int max_count;
    int generation;
} thread_barrier_t;

static inline void barrier_init(thread_barrier_t *barrier, int count) {
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    barrier->count = 0;
    barrier->max_count = count;
    barrier->generation = 0;
}

static inline void barrier_wait(thread_barrier_t *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    int gen = barrier->generation;
    barrier->count++;
    
    if (barrier->count == barrier->max_count) {
        barrier->generation++;
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
    } else {
        while (gen == barrier->generation) {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
        }
    }
    pthread_mutex_unlock(&barrier->mutex);
}

static inline void barrier_destroy(thread_barrier_t *barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}

#endif