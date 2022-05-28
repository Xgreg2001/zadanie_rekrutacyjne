//
// Created by Xgreg on 17.05.2022.
//
#include "queue.h"
#include <assert.h>

int main() {
    Queue *q = queue_create(5);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    int p1 = 10;
    int p2 = 12;
    int p3 = 14;
    int p4 = 22;
    int p5 = 26;

    queue_put(q, &p1);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_put(q, &p2);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_put(q, &p3);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_put(q, &p4);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_put(q, &p5);
    assert(!queue_is_empty(q));
    assert(queue_is_full(q));

    int r1 = *(int *) queue_get(q);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    int r2 = *(int *) queue_get(q);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    int r3 = *(int *) queue_get(q);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    int r4 = *(int *) queue_get(q);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    int r5 = *(int *) queue_get(q);
    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(r1 == 10);
    assert(r2 == 12);
    assert(r3 == 14);
    assert(r4 == 22);
    assert(r5 == 26);
    
    queue_destroy(q);
}
