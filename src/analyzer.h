//
// Created by Xgreg on 28.05.2022.
//

#ifndef ZADANIE_REKRYTACYJNE_ANALYZER_H
#define ZADANIE_REKRYTACYJNE_ANALYZER_H

#include "queue.h"
#include "reader.h"
#include <string.h>
#include <stdio.h>
#include <signal.h>

typedef struct Cpu_usage_data {
    long long total;
    long long idle;
} Cpu_usage_data;

extern volatile sig_atomic_t done;

void *analyzer_run(void *arg);

int analyzer_get_core_count(char *data);

void analyzer_analyze_line(char *line, Cpu_usage_data *out);

Proc_stat_data *analyzer_get_data(Queue *queue);

void analyzer_analyze_data(char *data, Cpu_usage_data p_data[], double usage_percent[]);


#endif //ZADANIE_REKRYTACYJNE_ANALYZER_H
