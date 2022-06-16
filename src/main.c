#include "main.h"

static pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool done = 0;

void *kill_process(void *args) {
    initiate_finish();
    return NULL;
}

void term() {
    pthread_t process_killer;
    pthread_create(&process_killer, NULL, kill_process, NULL);
    pthread_join(process_killer, NULL);
}

bool should_finish() {
    bool result;
    pthread_mutex_lock(&done_mutex);
    result = done;
    pthread_mutex_unlock(&done_mutex);
    return result;
}

void initiate_finish() {
    pthread_mutex_lock(&done_mutex);
    done = 1;
    pthread_mutex_unlock(&done_mutex);
}

int main() {
    struct sigaction action;
    sigset_t block_mask;

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGTERM);
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = (__sighandler_t) term;
    action.sa_mask = block_mask;
    action.sa_flags = 0;
    sigaction(SIGTERM, &action, NULL);

    Queue *reader_analyzer_queue = queue_create(10);
    Queue *analyzer_printer_queue = queue_create(10);
    Queue *logger_queue = queue_create(100);

    if (reader_analyzer_queue == NULL || analyzer_printer_queue == NULL || logger_queue == NULL) {
        perror("failed to create queues\n");
    }

    Queue *args_for_reader[2] = {reader_analyzer_queue,
                                 logger_queue};
    Queue *args_for_analyzer[3] = {reader_analyzer_queue,
                                   analyzer_printer_queue,
                                   logger_queue};
    Queue *args_for_printer[2] = {analyzer_printer_queue,
                                  logger_queue};


    pthread_t reader, analyzer, printer, logger, watchdog;
    pthread_create(&reader, NULL, reader_run, (void *) &args_for_reader);
    pthread_create(&analyzer, NULL, analyzer_run, (void *) &args_for_analyzer);
    pthread_create(&printer, NULL, printer_run, (void *) &args_for_printer);
    pthread_create(&logger, NULL, logger_run, (void *) &logger_queue);
    pthread_create(&watchdog, NULL, watchdog_run, (void *) &logger_queue);

    sigemptyset(&block_mask);
    sigprocmask(SIG_SETMASK, &block_mask, NULL);

    pthread_join(watchdog, NULL);
    pthread_join(reader, NULL);

    queue_lock(reader_analyzer_queue);
    queue_call_consumer(reader_analyzer_queue);
    queue_unlock(reader_analyzer_queue);

    pthread_join(analyzer, NULL);

    queue_lock(analyzer_printer_queue);
    queue_call_consumer(analyzer_printer_queue);
    queue_unlock(analyzer_printer_queue);

    pthread_join(printer, NULL);

    queue_lock(logger_queue);
    queue_call_consumer(logger_queue);
    queue_unlock(logger_queue);

    pthread_join(logger, NULL);

    while (!queue_is_empty(reader_analyzer_queue)) {
        void *data = queue_get(reader_analyzer_queue);
        free(data);
    }

    while (!queue_is_empty(analyzer_printer_queue)) {
        void *data = queue_get(analyzer_printer_queue);
        free(data);
    }

    while (!queue_is_empty(logger_queue)) {
        void *data = queue_get(logger_queue);
        free(data);
    }

    queue_destroy(reader_analyzer_queue);
    queue_destroy(analyzer_printer_queue);
    queue_destroy(logger_queue);

    return 0;
}
