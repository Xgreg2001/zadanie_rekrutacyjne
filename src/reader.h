//
// Created by Xgreg on 28.05.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_READER_H
#define ZADANIE_REKRYTACYJNE_READER_H

#include "queue.h"
#include <stdio.h>

#define BUFFER_SIZE 4096

typedef struct Proc_stat_data {
    char buffer[BUFFER_SIZE];
    struct timespec time_stamp;
} Proc_stat_data;

Proc_stat_data *reader_read(FILE *file);

void *reader_run(void *arg);

#endif //ZADANIE_REKRYTACYJNE_READER_H
