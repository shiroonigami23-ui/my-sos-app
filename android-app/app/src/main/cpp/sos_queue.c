#include "sos_queue.h"

void queue_init(SosQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

int queue_push(SosQueue *q, int value) {
    if (q->size >= 64) return 0;
    q->values[q->tail] = value;
    q->tail = (q->tail + 1) % 64;
    q->size++;
    return 1;
}

int queue_pop(SosQueue *q, int *value) {
    if (q->size <= 0) return 0;
    *value = q->values[q->head];
    q->head = (q->head + 1) % 64;
    q->size--;
    return 1;
}

double queue_average(const SosQueue *q) {
    int i;
    int idx;
    int sum = 0;
    if (q->size == 0) return 0.0;
    for (i = 0; i < q->size; i++) {
        idx = (q->head + i) % 64;
        sum += q->values[idx];
    }
    return (double)sum / (double)q->size;
}
