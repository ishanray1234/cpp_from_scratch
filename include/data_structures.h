#ifndef DATA_STRUCTURES_H
#define data_structures_H

#include <stddef.h>

// === BST Set ===
typedef struct BSTNode {
    int value;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
} Set;

Set *set_new();
void set_add(Set *set, int value);
int set_contains(Set *set, int value);
void set_remove(Set *set, int value);
void set_free(Set *set);

#endif
