//
// Created by Xgreg on 15.06.2022.
//

#include <pthread.h>
#include "watchdog.h"
#include "queue.h"
#include "logger.h"
#include <signal.h>

extern volatile sig_atomic_t done;

pthread_mutex_t watchdog_mutex = PTHREAD_MUTEX_INITIALIZER;

bool watchdog_check_ins[THREAD_COUNT] = {0};

void *watchdog_run(void *arg) {
    Queue *logger_queue = *((Queue **) arg);

    while (!done) {
        // sleep for 2 s
        struct timespec ts = {.tv_sec = 2, .tv_nsec = 0};
        nanosleep(&ts, NULL);

        // after waking up confirm that other threads are working
        pthread_mutex_lock(&watchdog_mutex);
        for (int i = 0; i < THREAD_COUNT; i++) {
            if (!watchdog_check_ins[i] && !done) {
                done = true;
                Logger_message *msg = logger_create_message(28, "Watchdog: thread not working");
                logger_log(msg, logger_queue);
            }
        }
        pthread_mutex_unlock(&watchdog_mutex);

        Logger_message *msg = logger_create_message(29, "Watchdog: all threads working");
        logger_log(msg, logger_queue);
    }

    pthread_mutex_destroy(&watchdog_mutex);

    return NULL;
}

void watchdog_check_in(enum watchdog_threads thread_id) {
    pthread_mutex_lock(&watchdog_mutex);
    watchdog_check_ins[thread_id] = true;
    pthread_mutex_unlock(&watchdog_mutex);
}
