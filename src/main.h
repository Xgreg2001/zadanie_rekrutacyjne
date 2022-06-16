//
// Created by Xgreg on 16.06.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_MAIN_H
#define ZADANIE_REKRYTACYJNE_MAIN_H

#include <string.h>
#include <signal.h>
#include "queue.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "logger.h"
#include "watchdog.h"


void term(void);

void *kill_process(__attribute__((unused)) void *);

void initiate_finish(void);

bool should_finish(void);

#endif //ZADANIE_REKRYTACYJNE_MAIN_H
