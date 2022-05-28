//
// Created by Xgreg on 28.05.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_READER_H
#define ZADANIE_REKRYTACYJNE_READER_H

#include "queue.h"
#include <stdio.h>

char *reader_read(FILE *file);

void *reader_run(void *arg);

#endif //ZADANIE_REKRYTACYJNE_READER_H
