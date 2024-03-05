#ifndef LAB1_QSORT_H
#define LAB1_QSORT_H 

#define MAX(a,b) (a>b?a:b)

/* 
    The original implementation: https://hackmd.io/@sysprog/linux2024-quiz1
*/

typedef struct __node {
    struct __node *left, *right;
    struct __node *next;
    long value;
} node_t;

// add node_t to beginning of list
void list_add(node_t **list, node_t *node_t);

node_t *list_tail(node_t **left);

int list_length(node_t **left);

node_t *list_construct(node_t *list, int n);

void list_free(node_t **list);

void swap_random_pivot(node_t **list); 

void quick_sort(node_t **list, int *max_time);

#endif