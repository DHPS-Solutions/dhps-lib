#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdlib.h>
#include <stdbool.h>

/* Function definitions */

/**
 * Free function for a tree node.
 * @param data The data to free.
 */
typedef void free_fn_t(void *);

/**
 * Compare function for a tree node.
 * @param a The first node to compare.
 * @param b The second node to compare.
 * @return True if a < b, false otherwise.
 */
typedef bool compare_fn_t(const void *a, const void *b);


/* Structs */

/**
 * A node in a binary tree.
 * @param data The data stored in the node.
 * @param free_func The function to free the data.
 * @param left The left child of the node.
 * @param right The right child of the node.
 * @param parent The parent of the node.
 */
struct tree_node_t {
    void *data;
    free_fn_t *free_func;
    struct tree_node_t *left;
    struct tree_node_t *right;
    struct tree_node_t *parent;
};

/**
 * A binary tree.
 * @param root The root of the tree.
 */
struct tree_t {
    struct tree_node_t *root;
};


/* Methods */

/**
 * Initialize a tree.
 * @param t The tree to initialize.
 */
void init_tree(struct tree_t *t);

/**
 * Create a tree node.
 * @param data The data to store in the node.
 * @param free_func The function to free the data.
 * @return The new tree node.
 */
struct tree_node_t *create_tree_node(void *data, free_fn_t *free_func);

/**
 * Insert a node into a tree.
 * @param t The tree to insert into.
 * @param data The data to insert.
 * @param cmp The compare function.
 * @param free_func The free function.
 */
void insert_tree_node(struct tree_t *t, void *data, compare_fn_t cmp, free_fn_t *free_func);

/**
 * Remove a node from a tree.
 * @param t The tree to remove from.
 * @param data The data to remove.
 * @param cmp The compare function.
 * @return True if the node was removed, false otherwise.
 */
bool remove_tree_node(struct tree_t *t, void *data, compare_fn_t cmp);

/**
 * Find a node in a tree.
 * @param t The tree to find in.
 * @param data The data to find.
 * @param cmp The compare function.
 * @return The node if found, NULL otherwise.
 */
struct tree_node_t *find_tree_node(struct tree_t *t, void *data, compare_fn_t cmp);

/**
 * Free a tree node.
 * @param node The node to free.
 */
void free_tree(struct tree_t *t);

/**
 * Free a tree node.
 * @param node The node to free.
 */
void free_tree_node(struct tree_node_t *node);

/**
 * Find the height of a tree.
 * @param t The tree to find the height of.
 * @return The height of the tree.
 */
size_t find_height_tree(struct tree_t *t);

/**
 * Find the height of a tree node.
 * @param node The node to find the height of.
 * @return The height of the node.
 */
size_t find_height(struct tree_node_t *node);

#endif