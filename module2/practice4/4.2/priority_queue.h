#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *queue_create();
void queue_delete(PriorityQueue **queue);
int queue_push(PriorityQueue *queue, void *value, uint8_t priority);
void *queue_pop_eq(PriorityQueue *queue, uint8_t priority);
void *queue_pop_ge(PriorityQueue *queue, uint8_t priority,
                   uint8_t *actual_priority);
void *queue_pop_head(PriorityQueue *queue, uint8_t *actual_priority);

typedef struct QueueIterator QueueIterator;

QueueIterator *it_begin(PriorityQueue *queue);
bool it_has_next(QueueIterator *it);
void *it_next(QueueIterator *it, uint8_t *priority);
void it_delete(QueueIterator *it);

#endif