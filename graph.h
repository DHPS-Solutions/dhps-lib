#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

/* Structs */

/* Basic generic edge */
struct edge_t {
    void *data;
    struct edge_t *next;
    struct node_t *to_node;
};

/* Basic generic node */
struct node_t {
    void *data;
    size_t index;
    struct edge_t *first_edge;
};

/* Basic generic graph */
struct graph_t {
    struct node_t *nodes;
    size_t node_count;
    size_t edge_count;
};

/* Methods */

/*
    Method to initialize a graph.
    @param graph struct graph_t *: the graph to initialize.
*/
void graph_init(struct graph_t *graph);

/*
    Method to insert a node into a graph.
    @param graph struct graph_t *: the graph to insert the node into.
    @param data void *: the data to insert into the node.
*/
void graph_insert_node(struct graph_t *graph, void *data);

/*
    Method to insert an edge into a graph.
    @param graph struct graph_t *: the graph to insert the edge into.
    @param data void *: the data to insert into the edge.
    @param from size_t: the index of the node to insert the edge from.
    @param to size_t: the index of the node to insert the edge to.
*/
void graph_insert_edge(struct graph_t *graph, void *data, size_t from, size_t to);

/*
    Method to transpose a graph.
    @param graph struct graph_t *: the graph to transpose.
    @return struct graph_t *: the transposed graph.
*/
struct graph_t *graph_transpose(struct graph_t *graph);

/*
    Method to copy a graph.
    @param graph struct graph_t *: the graph to copy.
    @return struct graph_t *: the copied graph.
*/
struct graph_t *graph_copy(struct graph_t *graph);

/*
    Method to free a graph.
    @param graph struct graph_t *: the graph to free.
*/
void graph_free(struct graph_t *graph);

#endif