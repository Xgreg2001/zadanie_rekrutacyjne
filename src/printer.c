//
// Created by Xgreg on 30.05.2022.
//

#include <unistd.h>
#include "printer.h"
#include "watchdog.h"
#include "logger.h"
#include "main.h"

extern size_t core_count;

void *printer_run(void *arg) {
    Queue *queue = *(Queue **) arg;
    Queue *logger_queue = *((Queue **) arg + 1);

    while (!should_finish()) {

        double *cpu_usage_percentage = printer_get_data(queue);

        if (cpu_usage_percentage != NULL) {
            watchdog_check_in(printer_id);

            int result = system("clear");
            if (result != 0) {
                perror("Failed to clear the screen");
            }

            printf("CPU total: %.1f%%\n", cpu_usage_percentage[0] * 100);

            for (size_t i = 1; i < core_count; i++) {
                printf("CPU %zu: %.1f%%\n", i - 1, cpu_usage_percentage[i] * 100);
            }

            Logger_message *msg = logger_create_message(12, "data printed");
            logger_log(msg, logger_queue);
        }

        free(cpu_usage_percentage);
    }

    return 0;
}

double *printer_get_data(Queue *const queue) {
    queue_lock(queue);


    if (queue_is_empty(queue)) {
        queue_wait_for_producer(queue);
    }


    double *data = queue_get(queue);

    queue_call_producer(queue);
    queue_unlock(queue);

    return data;
}
