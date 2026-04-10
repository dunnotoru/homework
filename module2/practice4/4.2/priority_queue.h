#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdint.h>

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *queue_create();
void queue_delete(PriorityQueue **queue);
int queue_push_tail(PriorityQueue *queue, void *value, uint8_t priority);
void *queue_pop_eq(PriorityQueue *queue, uint8_t priority);
void *queue_pop_ge(PriorityQueue *queue, uint8_t priority);
void *queue_pop_head(PriorityQueue *queue);

#endif