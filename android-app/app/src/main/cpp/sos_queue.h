#ifndef SOS_QUEUE_H
#define SOS_QUEUE_H

#include <stddef.h>

typedef struct {
    int values[64];
    int head;
    int tail;
    int size;
} SosQueue;

void queue_init(SosQueue *q);
int queue_push(SosQueue *q, int value);
int queue_pop(SosQueue *q, int *value);
double queue_average(const SosQueue *q);

#endif
