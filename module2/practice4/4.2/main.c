#include "priority_queue.h"
#include <stdio.h>

int main() {
  PriorityQueue *queue = queue_create();
  int a = 1;
  int b = 123;
  queue_push_tail(queue, &a, 50);
  queue_push_tail(queue, &b, 32);

  void *v = queue_pop_eq(queue, 50);
  printf("50 priority: %d\n", *(int *)v);
}