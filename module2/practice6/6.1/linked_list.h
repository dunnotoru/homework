#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct ListNode {
  void* value;
  struct ListNode* prev;
  struct ListNode* next;
} ListNode;

typedef int (*cmp_func)(void*, void*);

ListNode* create_node(void* value);
ListNode* insert_node(ListNode* head, void* value, cmp_func cmp);
ListNode* remove_node(ListNode* head, void* value);
void* get_node_value(ListNode* node);

typedef struct ListIterator {
  ListNode* head;
  ListNode* current;
} ListIterator;

ListIterator it_begin(ListNode* head);
ListNode* it_current(ListIterator* iterator);
ListNode* it_next(ListIterator* iterator);
int it_has_next(ListIterator* iterator);

#endif