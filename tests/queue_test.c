//
// Created by Xgreg on 17.05.2022.
//
#include "queue.h"
#include <assert.h>

int main() {
    Queue *q = queue_create(5);

    int p1 = 10;
    int p2 = 12;
    int p3 = 14;

    queue_put(q, &p1);
    queue_put(q, &p2);
    queue_put(q, &p3);

    int r1 = *(int *) queue_get(q);
    int r2 = *(int *) queue_get(q);
    int r3 = *(int *) queue_get(q);

    assert(r1 == 10);
    assert(r2 == 12);
    assert(r3 == 14);

    queue_destroy(q);
}
