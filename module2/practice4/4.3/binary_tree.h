#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef int (*cmp_func)(void *, void *);
typedef struct Tree Tree;
typedef struct TreeIterator TreeIterator;

Tree *tree_create(cmp_func cmp);
void tree_insert(Tree *tree, void *value);
void tree_remove_value(Tree *tree, void *value);

TreeIterator *it_begin(Tree *tree);
void it_current(TreeIterator *iterator);
void *it_remove_current(TreeIterator *iterator);
void *it_next(TreeIterator *iterator);
int it_has_next(TreeIterator *iterator);
void it_delete(TreeIterator *iterator);

#endif