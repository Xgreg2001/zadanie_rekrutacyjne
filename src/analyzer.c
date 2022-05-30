//
// Created by Xgreg on 28.05.2022.
//


#include "analyzer.h"
#include "reader.h"

size_t core_count = 0;

void *analyzer_run(void *arg) {

    Queue *input_queue = *(Queue **) arg;
    Queue *output_queue = *((Queue **) arg + 1);

    Proc_stat_data *data = analyzer_get_data(input_queue);

    core_count = analyzer_get_core_count(data->buffer);

    free(data);

    Cpu_usage_data previous_data[core_count];
    memset(previous_data, 0, core_count * sizeof(Cpu_usage_data));

    struct timespec previous_timestamp;
    clock_gettime(CLOCK_REALTIME, &previous_timestamp);

    while (!done) {
        // grab data from queue
        data = analyzer_get_data(input_queue);


        if ((data->time_stamp.tv_sec * 1000 + data->time_stamp.tv_nsec / 1000000) -
            (previous_timestamp.tv_sec * 1000 + previous_timestamp.tv_nsec / 1000000) > 1000) {
            // analyze data

            double *usage_percent = malloc(core_count * sizeof(double));

            analyzer_analyze_data(data->buffer, previous_data, usage_percent);

            // send data to printer
            queue_lock(output_queue);
            if (queue_is_full(output_queue)) {
                queue_wait_for_consumer(output_queue);
            }

            queue_put(output_queue, usage_percent);
            queue_call_consumer(output_queue);

            queue_unlock(output_queue);

            previous_timestamp = data->time_stamp;
        }

        free(data);
    }

    //TODO make sure that there is no bugs here
    queue_lock(input_queue);
    queue_call_producer(input_queue);
    queue_unlock(input_queue);

    queue_lock(output_queue);
    queue_call_consumer(output_queue);
    queue_unlock(output_queue);

    return 0;
}

int analyzer_get_core_count(char *data) {
    char *str = "cpu";
    int cpu_count = 0;
    size_t size = strlen(data);
    for (size_t i = 0; i < size; i++) {
        if (strncmp(&data[i], str, 3) == 0) {
            cpu_count++;
        }
    }
    return cpu_count;
}

void analyzer_analyze_data(char *data, Cpu_usage_data p_data[], double usage_percent[]) {
    char delim[] = "\n";

    char *line = strtok(data, delim);
    size_t counter = 0;
    while (counter < core_count) {
        Cpu_usage_data core_data = {0};
        analyzer_analyze_line(line, &core_data);

        unsigned long total_diff = core_data.total - p_data[counter].total;
        unsigned long idle_diff = core_data.idle - p_data[counter].idle;

        usage_percent[counter] = ((double) total_diff - (double) idle_diff) / (double) total_diff;


        p_data[counter].total = core_data.total;
        p_data[counter].idle = core_data.idle;

        line = strtok(NULL, delim);
        counter++;
    }
}

void analyzer_analyze_line(char *line, Cpu_usage_data *out) {
    unsigned long user = strtol(line + 5, &line, 10);
    unsigned long nice = strtol(line + 1, &line, 10);
    unsigned long system = strtol(line + 1, &line, 10);
    unsigned long idle = strtol(line + 1, &line, 10);
    unsigned long io_wait = strtol(line + 1, &line, 10);
    unsigned long irq = strtol(line + 1, &line, 10);
    unsigned long soft_irq = strtol(line + 1, &line, 10);
    unsigned long steal = strtol(line + 1, &line, 10);
    unsigned long guest = strtol(line + 1, &line, 10);
    unsigned long guest_nice = strtol(line + 1, &line, 10);

    user -= guest;
    nice -= guest_nice;

    unsigned long idle_all = idle + io_wait;
    unsigned long total = user + nice + system + irq + soft_irq + idle_all + steal + guest + guest_nice;

    out->total = total;
    out->idle = idle_all;
}

Proc_stat_data *analyzer_get_data(Queue *queue) {
    queue_lock(queue);

    if (queue_is_empty(queue)) {
        queue_wait_for_producer(queue);
    }

    Proc_stat_data *data = queue_get(queue);

    queue_call_producer(queue);
    queue_unlock(queue);

    return data;
}
