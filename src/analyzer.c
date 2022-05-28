//
// Created by Xgreg on 28.05.2022.
//

#include <stdio.h>
#include "analyzer.h"
#include <signal.h>

extern volatile sig_atomic_t done;

void *analyzer_run(void *arg) {

    Queue *input_queue = *(Queue **) arg;
//    Queue *output_queue = *((Queue **) arg + 1); // mam nadzije że to zadziała XD


    while (!done) {
        // grab data from queue
        char *data = analyzer_get_data(input_queue);

        printf("%s\n", data);

        free(data);

        // analyze data


        // send data to printer


    }

    // make sure to deallocate all data inside the queue
    while (true) {
        queue_lock(input_queue);
        if (!queue_is_empty(input_queue)) {
            queue_unlock(input_queue);
            char *buffer = analyzer_get_data(input_queue);
            free(buffer);
        } else {
            queue_unlock(input_queue);
            break;
        }
    }
    
    return 0;
}

char *analyzer_get_data(Queue *queue) {
    queue_lock(queue);

    if (queue_is_empty(queue)) {
        queue_wait_for_producer(queue);
    }

    char *data = queue_get(queue);

    queue_call_producer(queue);
    queue_unlock(queue);

    return data;
}
