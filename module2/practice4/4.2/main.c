#include "priority_queue.h"
#include <stdio.h>

int main() {
  PriorityQueue *queue = queue_create();
  int a = 5;
  int b = 123;
  int c = 1;
  int d = 13;
  int e = 165;

  queue_push(queue, &a, 50);
  queue_push(queue, &b, 12);

  void *v = NULL;
  do {
    v = queue_pop_head(queue);
    if (v != NULL) {
      printf("by priority: %d\n", *(int *)v);
    } else {
      printf("not found\n");
    }
  } while (v != NULL);

  queue_delete(&queue);
}