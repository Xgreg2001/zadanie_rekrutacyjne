//
// Created by Xgreg on 30.05.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_PRINTER_H
#define ZADANIE_REKRYTACYJNE_PRINTER_H

#include "queue.h"
#include <signal.h>
#include <stdio.h>

void *printer_run(void *arg);

double *printer_get_data(Queue *queue);

#endif //ZADANIE_REKRYTACYJNE_PRINTER_H
