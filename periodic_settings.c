#include "periodic_settings.h"
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define NSEC_PER_SEC 1000000000ULL

void current_time() { //Opcional
    time_t sec;
    suseconds_t msec;
    char zeros[3];
    struct timeval tv;

    gettimeofday(&tv, 0);

    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;

    if (msec < 10)
        strcpy(zeros, "00");
    else if (msec < 100)
        strcpy(zeros, "0");
    else
        strcpy(zeros, "");

    printf("El tiempo actual en segundos es: %ld.%s%ld\n", sec, zeros, msec);
}

static inline void timespec_add_us(struct timespec* t, uint64_t d) {
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
        t->tv_sec += 1;
    }
    t->tv_nsec = d;
}

void wait_next_activation(struct periodic_thread* t) {
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->r, NULL);
    timespec_add_us(&t->r, t->period);
}
