//
// Created by Xgreg on 28.05.2022.
//


#include <unistd.h>
#include "reader.h"
#include <signal.h>
#include <sys/time.h>

extern volatile sig_atomic_t done;

void *reader_run(void *arg) {
    Queue *queue = *(Queue **) arg;

    FILE *stats_file = fopen("/proc/stat", "r"); // TODO zamknąć plik gdy już ogarnę przechwytywanie sygnałów

    if (stats_file == NULL) {
        printf("Nie można otworzyć pliku %s\n", "/proc/stat");
        done = 1;
        exit(1); // TODO sprawdzić czy to ma wogle sens XD
    }

    while (!done) {
        Proc_stat_data *data = reader_read(stats_file);

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

Proc_stat_data *reader_read(FILE *file) {

    rewind(file);

    Proc_stat_data *data = malloc(
            sizeof(Proc_stat_data)); //TODO upewnić się że kolejny wątek zwolni pamięć
    if (data == NULL) {
        printf("Malloc error !\n");
    }
    size_t bytes_read = fread(data->buffer, sizeof(char), BUFFER_SIZE, file);

    if (bytes_read == BUFFER_SIZE) {
        printf("We could lose some data\n");
    }
    if (bytes_read == 0) {
        printf("printf didn't read any data\n");
    }

    data->buffer[bytes_read] = '\0';

    clock_gettime(CLOCK_REALTIME, &data->time_stamp);

    return data;
}
