//
// Created by Xgreg on 17.05.2022.
//

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#ifndef ZADANIE_REKRYTACYJNE_QUEUE_H
#define ZADANIE_REKRYTACYJNE_QUEUE_H

// forward declaration to enforce using API
typedef struct Queue Queue;

Queue *queue_create(size_t capacity);

void queue_destroy(Queue *q);

bool queue_is_full(const Queue *q);

bool queue_is_empty(const Queue *q);

void queue_put(Queue *q, void *product);

void *queue_get(Queue *q);

void queue_lock(Queue *q);

void queue_unlock(Queue *q);

void queue_call_producer(Queue *q);

void queue_call_consumer(Queue *q);

void queue_wait_for_producer(Queue *q);

void queue_wait_for_consumer(Queue *q);

#endif //ZADANIE_REKRYTACYJNE_QUEUE_H
