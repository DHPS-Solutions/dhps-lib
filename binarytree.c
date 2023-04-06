#include <stdlib.h>
#include <stdbool.h>

#include "binarytree.h"

void init_tree(struct tree_t *tree)
{
    tree->root = NULL;
}

struct tree_node_t *create_tree_node(void *data, free_fn_t *free_func)
{
    struct tree_node_t *new_node = (struct tree_node_t *)
                                    (malloc(sizeof(struct tree_node_t)));
    new_node->data = data;
    new_node->free_func = free_func;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    return new_node;
}

void insert_tree_node(struct tree_t *tree, void *data, compare_fn_t cmp, free_fn_t *free_func)
{
    struct tree_node_t *new_node = create_tree_node(data, free_func);

    if (!tree->root) {
        tree->root = new_node;
    } else {
        struct tree_node_t *current = tree->root;
        struct tree_node_t *parent = NULL;
        while (current) {
            parent = current;
            if (cmp(data, current->data)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        if (cmp(data, parent->data)) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        new_node->parent = parent;
    }
}

struct tree_node_t *find_tree_node(struct tree_t *tree, void *data, compare_fn_t cmp)
{
    struct tree_node_t *current = tree->root;
    while (current) {
        if (cmp(data, current->data)) {
            current = current->left;
        } else if (cmp(current->data, data)) {
            current = current->right;
        } else {
            return current;
        }
    }
    return NULL;
}

size_t find_height(struct tree_node_t *node)
{
    if (!node) {
        return 0;
    } else {
        int left_height = find_height(node->left);
        int right_height = find_height(node->right);
        if (left_height >= right_height) return left_height + 1;
        else return right_height +1;
    }
}

size_t find_height_tree(struct tree_t *tree_t)
{
    return find_height(tree_t->root);
}

bool remove_tree_node(struct tree_t *tree, void *data, compare_fn_t cmp)
{
    struct tree_node_t *current = tree->root;
    while (current) {
        if (cmp(data, current->data)) {
            current = current->left;
        } else if (cmp(current->data, data)) {
            current = current->right;
        } else {
            break;
        }
    }

    if (!current)
        return false;

    if (!current->left && !current->right) {
        if (current == tree->root) {
            tree->root = NULL;
        } else {
            if (current->parent->left == current) {
                current->parent->left = NULL;
            } else {
                current->parent->right = NULL;
            }
        }
        free_tree_node(current);
    } else if (!current->left || !current->right) {
        struct tree_node_t *child = current->left ? current->left : current->right;
        if (current == tree->root) {
            tree->root = child;
        } else {
            if (current->parent->left == current) {
                current->parent->left = child;
            } else {
                current->parent->right = child;
            }
        }
        child->parent = current->parent;
        free_tree_node(current);
    } else {
        struct tree_node_t *successor = current->right;
        while (successor->left) {
            successor = successor->left;
        }
        current->data = successor->data;
        if (successor->parent->left == successor) {
            successor->parent->left = successor->right;
        } else {
            successor->parent->right = successor->right;
        }
        if (successor->right) {
            successor->right->parent = successor->parent;
        }
        free_tree_node(successor);
    }
    return true;
}

void free_tree_node(struct tree_node_t *node)
{
    if (node) {
        free_tree_node(node->left);
        free_tree_node(node->right);
        node->free_func(node->data);
        free(node);
    }
}

void free_tree(struct tree_t *tree)
{
    free_tree_node(tree->root);
}