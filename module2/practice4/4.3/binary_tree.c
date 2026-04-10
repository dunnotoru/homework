#include "binary_tree.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct TreeNode {
  void *value;
  int8_t balance_factor;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

struct Tree {
  TreeNode *root;
  cmp_func cmp;
};

struct TreeIterator {
  Tree *tree;
  TreeNode *current;
};

int default_cmp(void *left, void *right) { return -1; }

Tree *tree_create(cmp_func cmp) {
  Tree *tree = calloc(1, sizeof(Tree));
  tree->root = NULL;
  tree->cmp = cmp ? cmp : default_cmp;
  return tree;
}

TreeNode *tree_node_create(void *value) {
  TreeNode *node = calloc(1, sizeof(TreeNode));
  node->value = value;
  node->balance_factor = 0;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void tree_insert(Tree *tree, void *value);
void tree_remove_value(Tree *tree, void *value);

TreeIterator *it_begin(Tree *tree);
void it_current(TreeIterator *iterator);
void *it_remove_current(TreeIterator *iterator);
void *it_next(TreeIterator *iterator);
int it_has_next(TreeIterator *iterator);
void it_delete(TreeIterator *iterator);
