#include "priority_queue.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct QueueNode {
  void *value;
  uint8_t priority;
  struct QueueNode *next;
} QueueNode;

struct PriorityQueue {
  QueueNode *head;
  size_t size;
};

PriorityQueue *queue_create() {
  PriorityQueue *q = calloc(1, sizeof(PriorityQueue));
  q->size = 0;
  q->head = NULL;
  return q;
}

void queue_delete(PriorityQueue **queue) {
  if (queue == NULL || *queue == NULL) {
    return;
  }

  QueueNode *cur, *prev = NULL;
  for (cur = (*queue)->head; cur != NULL; cur = cur->next) {
    free(prev);

    prev = cur;
  }

  free((*queue)->head);
  free(*queue);
  *queue = NULL;
}

int queue_push(PriorityQueue *q, void *value, uint8_t priority) {
  if (value == NULL) {
    return 0;
  }

  QueueNode *node = calloc(1, sizeof(QueueNode));
  node->value = value;
  node->priority = priority;
  node->next = NULL;

  if (q->head == NULL) {
    q->head = node;
    return 1;
  }

  if (node->priority < q->head->priority) {
    node->next = q->head;
    q->head = node;
    return 1;
  }

  QueueNode *cur;
  for (cur = q->head; cur->next != NULL; cur = cur->next) {
    if (node->priority < cur->next->priority) {
      node->next = cur->next;
      cur->next = node;
      return 1;
    }
  }

  if (cur->next == NULL) {
    cur->next = node;
  }

  return 1;
}

static void *queue_pop(PriorityQueue *queue, uint8_t priority,
                       int (*cmp)(uint8_t, uint8_t), uint8_t *actual_priority) {
  if (queue == NULL || queue->head == NULL) {
    return NULL;
  }

  void *value = NULL;
  QueueNode *cur, *prev = NULL;
  for (cur = queue->head; cur != NULL; cur = cur->next) {
    if (cmp(cur->priority, priority)) {
      value = cur->value;
      if (actual_priority != NULL) {
        *actual_priority = cur->priority;
      }
      break;
    }

    prev = cur;
  }

  if (value == NULL) {
    return NULL;
  }

  if (prev == NULL) {
    queue->head = cur->next;
  } else {
    prev->next = cur->next;
  }

  free(cur);
  cur = NULL;

  return value;
}

static int eq(uint8_t left, uint8_t right) { return left == right; }
static int ge(uint8_t left, uint8_t right) { return left >= right; }

#pragma GCC diagnostic ignored "-Wunused-parameter"
static int always_true(uint8_t left, uint8_t right) { return 1; }

void *queue_pop_eq(PriorityQueue *queue, uint8_t priority) {
  return queue_pop(queue, priority, eq, NULL);
}

void *queue_pop_ge(PriorityQueue *queue, uint8_t priority,
                   uint8_t *actual_priority) {
  return queue_pop(queue, priority, ge, actual_priority);
}

void *queue_pop_head(PriorityQueue *queue, uint8_t *actual_priority) {
  return queue_pop(queue, 0, always_true, actual_priority);
}

struct QueueIterator {
  PriorityQueue *queue;
  QueueNode *current;
};

QueueIterator *it_begin(PriorityQueue *queue) {
  QueueIterator *it = calloc(1, sizeof(QueueIterator));
  it->queue = queue;
  it->current = queue->head;

  return it;
}

bool it_has_next(QueueIterator *it) { return it->current != NULL; }

void *it_next(QueueIterator *it, uint8_t *priority) {
  if (it->current == NULL) {
    return NULL;
  }

  QueueNode *tmp = it->current;
  it->current = it->current->next;
  *priority = tmp->priority;
  return tmp->value;
}

void it_delete(QueueIterator *it) { free(it); }
