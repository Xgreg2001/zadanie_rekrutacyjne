#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "queue.h"
#include "reader.h"
#include "analyzer.h"

volatile sig_atomic_t done = 0;

void term() {
    done = 1;
}

int main() {
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    Queue *reader_analyzer_queue = queue_create(10);
    Queue *analyzer_printer_queue = queue_create(10);

    Queue *args_for_analyzer[2] = {reader_analyzer_queue,
                                   analyzer_printer_queue}; // czy trzeba to zwalniać? wydaje mi się że nie to powinny być pointery na stosie

    pthread_t reader, analyzer;
    pthread_create(&reader, NULL, reader_run, (void *) &reader_analyzer_queue);
    pthread_create(&analyzer, NULL, analyzer_run, (void *) &args_for_analyzer);

    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);

    queue_destroy(reader_analyzer_queue);
    queue_destroy(analyzer_printer_queue);

    return 0;
}
