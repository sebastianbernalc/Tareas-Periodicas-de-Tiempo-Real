#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "consumer.h"
#include "periodic_settings.h"

#define NUM_PRODUCERS 3

void* consumer_thread_func(void* arg) {
    struct periodic_thread* t = (struct periodic_thread*)arg;
    int num_producers = NUM_PRODUCERS;

    while (1) {
        wait_next_activation(t);
	// Bloquear la sección crítica para acceder al data_buffer
        pthread_mutex_lock(t->mutex);

        // Consume data del buffer produced by the current producer
        int consumed_data = t->data_buffer[t->thread_id];
        printf("Consumer Thread consumed from Producer %d: %d\n", t->thread_id, consumed_data);

        // Cambia al siguiente productor en el búfer circular
        t->thread_id = (t->thread_id + 1) % num_producers;
	// Liberar el mutex para que otros hilos puedan acceder
        pthread_mutex_unlock(t->mutex);
    }

    return NULL;
}
