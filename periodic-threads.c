#include <pthread.h>
#include "producer.h"
#include "consumer.h"
#include "periodic_settings.h"

#define NSEC_PER_SEC 1000000000ULL
#define NUM_PRODUCERS 3


static inline void timespec_add_us(struct timespec* t, uint64_t d) {
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
        t->tv_sec += 1;
    }
    t->tv_nsec = d;
}

int main() {
// Create data buffer
    int data_buffer[NUM_PRODUCERS];

    // Initialize data buffer
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        data_buffer[i] = 0;
    }

    // Create a mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_thread;

    // Create and configure periodic threads
    struct periodic_thread producers[NUM_PRODUCERS];
    struct periodic_thread consumer;

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producers[i].thread_id = i;
        producers[i].data_buffer = data_buffer;
        producers[i].period = 100000;  // 0.1 segundos
        producers[i].offset = 0;
	producers[i].mutex = &mutex; // Asignar el mutex
    }

    consumer.thread_id = 0;  // Only one consumer
    consumer.data_buffer = data_buffer;
    consumer.period = 500000;  // 3 seconds
    consumer.offset = 0;
    consumer.mutex = &mutex; // Asignar el mismo mutex

    // Create and start producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        clock_gettime(CLOCK_REALTIME, &producers[i].r);
        timespec_add_us(&producers[i].r, producers[i].offset);
        pthread_create(&producer_threads[i], NULL, producer_thread, (void*)&producers[i]);
    }

    // Create and start consumer thread
    clock_gettime(CLOCK_REALTIME, &consumer.r);
    timespec_add_us(&consumer.r, consumer.offset);
    pthread_create(&consumer_thread, NULL, consumer_thread_func, (void*)&consumer);  // Cambiamos el nombre de la función

    // Join threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    pthread_join(consumer_thread, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}