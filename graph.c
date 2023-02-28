#include <stdlib.h>

#include "graph.h"

void graph_init(struct graph_t *graph)
{
    graph->nodes = NULL;
    graph->node_count = 0;
    graph->edge_count = 0;
}

void graph_insert_node(struct graph_t *graph, void *data)
{
    struct node_t *node = (struct node_t *)(malloc(sizeof(struct node_t)));
    node->data = data;
    node->index = graph->node_count;
    *(graph->nodes + graph->node_count++) = *node;
}

void graph_insert_edge(struct graph_t *graph, void *data, size_t from, size_t to)
{
    struct edge_t *edge = (struct edge_t *)(malloc(sizeof(struct edge_t)));
    edge->data = data;
    edge->to_node = graph->nodes + to;
    edge->next = (graph->nodes + from)->first_edge;
    (graph->nodes + from)->first_edge = edge;
    graph->edge_count++;
}

struct graph_t *graph_transpose(struct graph_t *graph)
{
    struct graph_t *t_graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    t_graph->nodes = (struct node_t *)(malloc(graph->node_count * sizeof(struct node_t)));
    t_graph->node_count = graph->node_count;
    t_graph->edge_count = graph->edge_count;

    for (int i = 0; i < t_graph->node_count; i++)
        graph_insert_node(t_graph, (graph->nodes + i)->data);

    for (int i = 0; i < t_graph->node_count; i++)
        for (struct edge_t *edge = (graph->nodes + i)->first_edge; edge; edge = edge->next)
            graph_insert_edge(t_graph, edge->data, edge->to_node->index, i);

    return t_graph;
}

struct graph_t *graph_copy(struct graph_t *graph)
{
    struct graph_t *t_graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    t_graph->nodes = (struct node_t *)(malloc(graph->node_count * sizeof(struct node_t)));
    t_graph->node_count = graph->node_count;
    t_graph->edge_count = graph->edge_count;

    for (int i = 0; i < t_graph->node_count; i++)
        graph_insert_node(t_graph, (graph->nodes + i)->data);

    for (int i = 0; i < t_graph->node_count; i++)
        for (struct edge_t *edge = (graph->nodes + i)->first_edge; edge; edge = edge->next)
            graph_insert_edge(t_graph, edge->data, i, edge->to_node->index);

    return t_graph;
}

void graph_free(struct graph_t *graph, void (*free_data)(void *))
{
    struct edge_t *edge_i, *prev;
    for (int i = 0; i < graph->node_count; i++) {
        edge_i = (graph->nodes + i)->first_edge;
        while (edge_i != NULL) {
            prev = edge_i;
            edge_i = edge_i->next;
            free(prev);
        }
        (*free_data)((graph->nodes + i)->data);
        free(graph->nodes + i);
    }
}
