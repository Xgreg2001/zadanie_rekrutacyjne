//
// Created by Xgreg on 28.05.2022.
//


#include <unistd.h>
#include "reader.h"
#include <signal.h>

extern volatile sig_atomic_t done;

#define BUFFER_SIZE 4096

void *reader_run(void *arg) {
    Queue *queue = *(Queue **) arg;

    FILE *stats_file = fopen("/proc/stat", "r"); // TODO zamknąć plik gdy już ogarnę przechwytywanie sygnałów

    if (stats_file == NULL) {
        printf("Nie można otworzyć pliku %s\n", "/proc/stat");
        exit(1); // TODO sprawdzić czy to ma wogle sens XD
    }

    while (!done) {
        char *buffer = reader_read(stats_file);

        queue_lock(queue);
        if (queue_is_full(queue)) {
            queue_wait_for_consumer(queue);
        }

        queue_put(queue, buffer);
        queue_call_consumer(queue);

        queue_unlock(queue);
    }

    fclose(stats_file);


    return 0;
}

char *reader_read(FILE *file) {

    rewind(file);

    char *buffer = malloc(BUFFER_SIZE + 1); // + 1 bo '\0' na koniec TODO upewnić się że kolejny wątek zwolni pamięć
    if (buffer == NULL) {
        printf("Malloc error !\n");
    }
    size_t bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file);

    if (bytes_read == BUFFER_SIZE) {
        printf("We could lose some data\n");
    }
    if (bytes_read == 0) {
        printf("printf didn't read any data\n");
    }

    buffer[bytes_read] = '\0';

    return buffer;
}
