//
// Created by Xgreg on 06.06.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_LOGGER_H
#define ZADANIE_REKRYTACYJNE_LOGGER_H

#include "queue.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

#define MAX_MESSAGE_LENGTH 4092

typedef struct Logger_message {
    char message[MAX_MESSAGE_LENGTH];
    struct timespec time_stamp;
} Logger_message;

void *logger_run(void *arg);

Logger_message *logger_get_data(Queue *queue);

Logger_message *logger_create_message(size_t len, char message[len]);

void logger_log(Logger_message *msg, Queue *logger_queue);

#endif //ZADANIE_REKRYTACYJNE_LOGGER_H

