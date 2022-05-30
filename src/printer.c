//
// Created by Xgreg on 30.05.2022.
//

#include <unistd.h>
#include "printer.h"
#include <math.h>

extern size_t core_count;

extern volatile sig_atomic_t done;

void *printer_run(void *arg) {
    Queue *queue = *(Queue **) arg;

    while (!done) {

        double *cpu_usage_percentage = printer_get_data(queue);

        system("clear");; // not very portable but I guess so is using /proc/stat

        printf("CPU total: %d%%\n", (int) round(cpu_usage_percentage[0] * 100));

        for (size_t i = 1; i < core_count; i++) {
            printf("CPU %zu: %d%%\n", i - 1, (int) round(cpu_usage_percentage[i] * 100));
        }

        free(cpu_usage_percentage);
    }

    return 0;
}

double *printer_get_data(Queue *queue) {
    queue_lock(queue);


    if (queue_is_empty(queue)) {
        queue_wait_for_producer(queue);
    }


    double *data = queue_get(queue);

    queue_call_producer(queue);
    queue_unlock(queue);

    return data;
}