#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "producer.h"
#include "periodic_settings.h"

void* producer_thread(void* arg) {
    struct periodic_thread* t = (struct periodic_thread*)arg;

    while (1) {
        wait_next_activation(t);

        // Generar un entero aleatorio y almacenarlo en el búfer de datos
        int random_data = rand() % 100;
	// Bloquear la sección crítica para acceder al data_buffer
        pthread_mutex_lock(t->mutex);
	//Alamacena el dato en el buffer
        t->data_buffer[t->thread_id] = random_data;
	// Liberar el mutex para que otros hilos puedan acceder
        pthread_mutex_unlock(t->mutex);
        printf("Producer Thread %d produced: %d\n", t->thread_id, random_data);
    }

    return NULL;
}
