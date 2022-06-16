//
// Created by Xgreg on 15.06.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_WATCHDOG_H
#define ZADANIE_REKRYTACYJNE_WATCHDOG_H

#include <stdbool.h>

#define THREAD_COUNT 4

enum watchdog_threads {
    reader_id, analyzer_id, printer_id, logger_id
};

void *watchdog_run(void *arg);

void watchdog_check_in(enum watchdog_threads thread_id);

#endif //ZADANIE_REKRYTACYJNE_WATCHDOG_H
