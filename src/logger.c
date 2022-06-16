//
// Created by Xgreg on 06.06.2022.
//


#include "logger.h"
#include "watchdog.h"

extern volatile sig_atomic_t done;

//TODO flush after every write or figure out a good way to close the program after SIGTERM

void *logger_run(void *arg) {
    Queue *queue = *(Queue **) arg;

    FILE *log_file = fopen("log.txt", "w"); // TODO zamknąć plik gdy już ogarnę przechwytywanie sygnałów

    if (log_file == NULL) {
        printf("Nie można otworzyć pliku log.txt\n");
        done = 1;
        exit(1); // TODO sprawdzić czy to ma wogle sens XD
    }

    while (!done) {

        Logger_message *message = logger_get_data(queue);

        watchdog_check_in(logger_id);

        char time_buff[100];
        strftime(time_buff, sizeof time_buff, "%D %T", gmtime(&message->time_stamp.tv_sec));

        fprintf(log_file, "%s.%09ld: %s\n", time_buff, message->time_stamp.tv_nsec, message->message);

        free(message);
    }

    fclose(log_file);

    return 0;
}

Logger_message *logger_get_data(Queue *queue) {
    queue_lock(queue);

    if (queue_is_empty(queue)) {
        queue_wait_for_producer(queue);
    }

    Logger_message *data = queue_get(queue);

    queue_call_producer(queue);
    queue_unlock(queue);

    return data;
}

Logger_message *logger_create_message(size_t len, char *message) {
    if (len >= MAX_MESSAGE_LENGTH) {
        return NULL;
    }

    Logger_message *result = malloc(sizeof(Logger_message));
    if (result == NULL) {
        return NULL;
    }

    strncpy(result->message, message, MAX_MESSAGE_LENGTH);

    clock_gettime(CLOCK_REALTIME, &result->time_stamp);

    return result;
}

void logger_log(Logger_message *msg, Queue *logger_queue) {
    if (msg == NULL) {
        return;
        //failed to create a message
    }

    queue_lock(logger_queue);

    if (queue_is_full(logger_queue)) {
        queue_wait_for_consumer(logger_queue);
    }

    queue_put(logger_queue, msg);

    queue_call_consumer(logger_queue);
    queue_unlock(logger_queue);
}
