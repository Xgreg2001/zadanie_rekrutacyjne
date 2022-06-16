//
// Created by Xgreg on 28.05.2022.
//


#include <unistd.h>
#include "reader.h"
#include <sys/time.h>
#include "logger.h"
#include "watchdog.h"
#include "main.h"


void *reader_run(void *const arg) {
    Queue *queue = *(Queue **) arg;
    Queue *logger_queue = *((Queue **) arg + 1);

    FILE *stats_file = fopen("/proc/stat", "r");

    if (stats_file == NULL) {
        perror("Nie można otworzyć pliku /proc/stat\n");
        initiate_finish();
    }

    while (!should_finish()) {
        Proc_stat_data *data = reader_read(stats_file);

        Logger_message *msg = logger_create_message(21, "Read from /proc/stat");
        logger_log(msg, logger_queue);

        watchdog_check_in(reader_id);

        queue_lock(queue);
        if (queue_is_full(queue)) {
            queue_wait_for_consumer(queue);
        }

        queue_put(queue, data);
        queue_call_consumer(queue);

        queue_unlock(queue);

        // should sleep for 0.1 s
        struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000};
        nanosleep(&ts, NULL);
    }

    fclose(stats_file);

    return 0;
}

Proc_stat_data *reader_read(FILE *const file) {

    rewind(file);

    Proc_stat_data *data = malloc(sizeof(Proc_stat_data));
    if (data == NULL) {
        perror("Malloc error !\n");
    }
    size_t bytes_read = fread(data->buffer, sizeof(char), BUFFER_SIZE, file);

    if (bytes_read == BUFFER_SIZE) {
        perror("We could lose some data\n");
    }
    if (bytes_read == 0) {
        perror("printf didn't read any data\n");
    }

    data->buffer[bytes_read] = '\0';

    clock_gettime(CLOCK_REALTIME, &data->time_stamp);

    return data;
}
