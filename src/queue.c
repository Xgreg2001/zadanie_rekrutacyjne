//
// Created by Xgreg on 17.05.2022.
//

#include "queue.h"

struct Queue {
    size_t max_capacity;
    size_t size;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    void *buffer[];
};

Queue *queue_create(size_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    Queue *q = malloc(sizeof(*q) + sizeof(void *) * capacity);
    if (q == NULL) {
        return NULL;
    }

    *q = (Queue) {
            .max_capacity = capacity,
            .size = 0,
            .head = 0,
            .tail = 0,
            .mutex = PTHREAD_MUTEX_INITIALIZER,
            .can_produce = PTHREAD_COND_INITIALIZER,
            .can_consume = PTHREAD_COND_INITIALIZER
    };

    return q;
}

void queue_destroy(Queue *q) {
    pthread_cond_destroy(&q->can_produce);
    pthread_cond_destroy(&q->can_consume);
    pthread_mutex_destroy(&q->mutex);

    free(q);
}

bool queue_is_full(const Queue *q) {
    return q->size == q->max_capacity;
}

bool queue_is_empty(const Queue *q) {
    return q->size == 0;
}

void queue_put(Queue *q, void *product) {
    if (queue_is_full(q)) {
        return;
    }

    if (product == NULL) {
        return;
    }

    q->buffer[q->head] = product;
    q->head = (q->head + 1) % q->max_capacity;
    q->size++;
}

void *queue_get(Queue *q) {
    if (queue_is_empty(q)) {
        return NULL;
    }

    void *product = q->buffer[q->tail];
    q->tail = (q->tail + 1) % q->max_capacity;
    q->size--;

    return product;
}

void queue_lock(Queue *q) {
    pthread_mutex_lock(&q->mutex);
}

void queue_unlock(Queue *q) {
    pthread_mutex_unlock(&q->mutex);
}

void queue_call_producer(Queue *q) {
    pthread_cond_signal(&q->can_produce);
}

void queue_call_consumer(Queue *q) {
    pthread_cond_signal(&q->can_consume);
}

void queue_wait_for_producer(Queue *q) {
    pthread_cond_wait(&q->can_consume, &q->mutex);
}

void queue_wait_for_consumer(Queue *q) {
    pthread_cond_wait(&q->can_produce, &q->mutex);
}














