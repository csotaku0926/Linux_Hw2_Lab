#ifndef LAB1_QSORT_H
#define LAB1_QSORT_H 

#define MAX(a,b) (a>b?a:b)


#include "list.h"

typedef struct {
    long value;
    struct list_head list;
} element_t;

struct list_head *list_new();

struct list_head *list_tail(struct list_head *head);

int list_length(struct list_head *head);

struct list_head *list_construct(struct list_head *list, int n);

void list_free(struct list_head *head);

void quick_sort(struct list_head **head, int *max_time);

void print_list(struct list_head *head);

#endif