#include "../include/data_structures.h"
#include <stdlib.h>
#include <stdio.h>

static BSTNode *new_node(int value) {
    BSTNode *node = malloc(sizeof(BSTNode));
    if (!node) exit(EXIT_FAILURE);
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

Set *set_new() {
    Set *set = malloc(sizeof(Set));
    if (!set) exit(EXIT_FAILURE);
    set->root = NULL;
    return set;
}

static BSTNode *insert(BSTNode *node, int value) {
    if (!node) return new_node(value);
    if (value < node->value)
        node->left = insert(node->left, value);
    else if (value > node->value)
        node->right = insert(node->right, value);
    return node;
}

void set_add(Set *set, int value) {
    set->root = insert(set->root, value);
}

static int contains(BSTNode *node, int value) {
    if (!node) return 0;
    if (value == node->value) return 1;
    return value < node->value ? contains(node->left, value) : contains(node->right, value);
}

int set_contains(Set *set, int value) {
    return contains(set->root, value);
}

static BSTNode *find_min(BSTNode *node) {
    while (node->left) node = node->left;
    return node;
}

static BSTNode *remove_node(BSTNode *node, int value) {
    if (!node) return NULL;

    if (value < node->value)
        node->left = remove_node(node->left, value);
    else if (value > node->value)
        node->right = remove_node(node->right, value);
    else {
        if (!node->left) {
            BSTNode *temp = node->right;
            free(node);
            return temp;
        } else if (!node->right) {
            BSTNode *temp = node->left;
            free(node);
            return temp;
        } else {
            BSTNode *min_node = find_min(node->right);
            node->value = min_node->value;
            node->right = remove_node(node->right, min_node->value);
        }
    }
    return node;
}

void set_remove(Set *set, int value) {
    set->root = remove_node(set->root, value);
}

static void free_tree(BSTNode *node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

void set_free(Set *set) {
    if (set) {
        free_tree(set->root);
        free(set);
    }
}
