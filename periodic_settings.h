#ifndef PERIODIC_SETTINGS_H
#define PERIODIC_SETTINGS_H

#include <pthread.h>
#include <time.h>
#include <stdint.h>

struct periodic_thread {
    struct timespec r;
    int period;
    int offset;
    int thread_id;
    int* data_buffer;
    pthread_mutex_t* mutex; // Agregar un mutex a la estructura
};

void current_time();
void wait_next_activation(struct periodic_thread* t);
static inline void timespec_add_us(struct timespec* t, uint64_t d);

#endif
