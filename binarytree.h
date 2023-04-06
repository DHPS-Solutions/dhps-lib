#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdlib.h>
#include <stdbool.h>

/*
    Function used to free a generic.
    @param a void*: generic to free.
*/
typedef void free_fn_t(void *);

/* Structs */

/* Tree Node */
struct tree_node_t {
    void *data;
    free_fn_t *free_func;
    struct tree_node_t *left;
    struct tree_node_t *right;
    struct tree_node_t *parent;
};

/* Tree */
struct tree_t {
    struct tree_node_t *root;
};

/* Methods */

/* 
    Function used to assert a generic to a new value. 
    @param a const void*: a value to compare.
    @param b const void*: b value to compare.
*/
typedef bool compare_fn_t(const void *a, const void *b);

/*
    Function used to initialize a tree.
    @param t struct tree_t*: tree to initialize.
*/
void init_tree(struct tree_t *t);

/*
    Function used to create a new tree node.
    @param data void*: data to store in the node.
    @return struct tree_node_t*: new tree node.
*/
struct tree_node_t *create_tree_node(void *data, free_fn_t *free_func);

/*
    Function used to insert a new node in a tree.
    @param t struct tree_t*: tree to insert the node.
    @param data void*: data to store in the node.
    @param cmp compare_fn_t: function used to compare nodes.
*/
void insert_tree_node(struct tree_t *t, void *data, compare_fn_t cmp, free_fn_t *free_func);

/*
    Function used to remove a node from a tree.
    @param t struct tree_t*: tree to remove the node.
    @param data void*: data to remove from the tree.
    @param cmp compare_fn_t: function used to compare nodes.
*/
bool remove_tree_node(struct tree_t *t, void *data, compare_fn_t cmp);

/*
    Function used to find a node in a tree.
    @param t struct tree_t*: tree to find the node.
    @param data void*: data to find in the tree.
    @param cmp compare_fn_t: function used to compare nodes.
    @return struct tree_node_t*: node found.
*/
struct tree_node_t *find_tree_node(struct tree_t *t, void *data, compare_fn_t cmp);

/*
    Function used to free a tree.
    @param t struct tree_t*: tree to free.
*/
void free_tree(struct tree_t *t);

/*
    Function used to free a tree node.
    @param node struct tree_node_t*: node to free.
*/
void free_tree_node(struct tree_node_t *node);

/*
    Function used to find the height of a tree.
    @param t struct tree_t*: tree to find the height.
*/
size_t find_height_tree(struct tree_t *t);

/*
    Function used to find the height of a tree node.
    @param node struct tree_node_t*: node to find the height.
*/
size_t find_height(struct tree_node_t *node);

#endif