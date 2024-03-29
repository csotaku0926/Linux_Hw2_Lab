#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "qsort.h"


void list_add(node_t **list, node_t *node_t)
{
    node_t->next = *list;
    *list = node_t;
}

node_t *list_tail(node_t **left)
{
    while ((*left) && (*left)->next)
        left = &(*left)->next;
    return *left;
}

int list_length(node_t **left)
{
    int n = 0;
    while (*left) {
        ++n;
        left = &(*left)->next;
    }
    return n;
}

node_t *list_construct(node_t *list, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->next = list;
    node->value = n;
    return node;
}

void list_free(node_t **list)
{
    node_t *node = (*list)->next;
    while (*list) {
        free(*list);
        *list = node;
        if (node)
            node = node->next;
    }
}

// swap the random value with head
void swap_random_pivot(node_t **list) {
    if (!list)
        return;
    
    int len = list_length(list);
    srand(time(NULL));
    int r = rand() % len;

    node_t **rand_node = list;
    while (r > 1) {
        rand_node = &(*rand_node)->next;
        r--;
    }

    node_t *tmp = (*rand_node)->next;
    (*rand_node)->next = tmp->next;
    list_add(list, tmp);
}

/* Original implementation: https://alienryderflex.com/quicksort/ */
void quick_sort(node_t **list, int *max_time)
{
    int n = list_length(list);
    int value;
    int i = 0;
    int max_level = n * 0.25 + 20;
    node_t *begin[max_level], *end[max_level];
    node_t *result = NULL, *left = NULL, *right = NULL;
    
    begin[0] = *list;
    end[0] = list_tail(list);
            
    while (i >= 0) {
        node_t *L = begin[i], *R = end[i];
        if (L != R) {
            swap_random_pivot(&L);
            node_t *pivot = L;
            value = pivot->value;
            node_t *p = pivot->next;
            pivot->next = NULL;
    
            while (p) {
                node_t *n = p;
                p = p->next;
                list_add(n->value > value ? &right : &left, n);
            }

            begin[i] = left;
            end[i] = list_tail(&left);
            begin[i + 1] = pivot;
            end[i + 1] = pivot;
            begin[i + 2] = right;
            end[i + 2] = list_tail(&right);

            left = right = NULL;
            i += 2;
            *max_time = MAX(*max_time, i);
        } else {
            if (L)
                list_add(&result, L);
            i--;
        }
    }
    *list = result;
}