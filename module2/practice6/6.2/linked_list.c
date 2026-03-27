#include "linked_list.h"

#include <stdlib.h>

ListNode* create_node(void* value) {
  ListNode* node = calloc(1, sizeof(ListNode));
  node->value = value;
  node->next = node;
  node->prev = node;
  return node;
}

void insert_between(ListNode* prev, ListNode* next, ListNode* insert) {
  insert->prev = prev;
  insert->next = next;

  prev->next = insert;
  next->prev = insert;
}

ListNode* insert_node(ListNode* head, void* value, cmp_func cmp) {
  if (head == NULL) {
    return create_node(value);
  }

  ListNode* node = create_node(value);

  if (cmp(value, head->value) < 0) {
    insert_between(head->prev, head, node);
    return node;
  }

  ListNode* cur = head;
  do {
    if (cmp(value, cur->value) < 0) {
      insert_between(cur->prev, cur, node);
      return head;
    }

    cur = cur->next;
  } while (cur != head);

  insert_between(head->prev, head, node);

  return head;
}

ListNode* remove_node(ListNode* head, void* value) {
  if (head == NULL) {
    return NULL;
  }

  ListNode* cur = head;

  do {
    if (cur->value == value) {
      if (cur->next == cur) {
        free(cur);
        return NULL;
      }

      ListNode* prev = cur->prev;
      ListNode* next = cur->next;

      prev->next = next;
      next->prev = prev;

      if (cur == head) {
        head = next;
      }

      free(cur);
      return head;
    }

    cur = cur->next;
  } while (cur != head);

  return head;
}

void* get_node_value(ListNode* node) {
  if (node == NULL) {
    return NULL;
  }

  return node->value;
}

ListIterator it_begin(ListNode* head) {
  ListIterator it = {
      .head = head,
      .current = head,
  };

  return it;
}

ListNode* it_current(ListIterator* iterator) { return iterator->current; }

ListNode* it_next(ListIterator* iterator) {
  if (it_has_next(iterator) == 0) {
    return NULL;
  }

  iterator->current = iterator->current->next;
  return iterator->current;
}

int it_has_next(ListIterator* iterator) {
  if (iterator->current->next == iterator->head) {
    return 0;
  }

  return 1;
}