#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "qsort_list.h"

struct list_head *list_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head)
        INIT_LIST_HEAD(head);

    return head;
}

struct list_head *list_tail(struct list_head *head)
{
    return head->prev;
}

int list_length(struct list_head *head)
{
    if (!head)
        return 0;

    int n = 0;
    struct list_head *node;

    list_for_each(node, head) 
        n++;
    
    return n;
}

/* q_insert_head in queue.c */
struct list_head *list_construct(struct list_head *list, int n)
{
    element_t *node = malloc(sizeof(element_t)); 

    node->value = n;
    list_add(&node->list, list);

    return list;
}

void list_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *safe, *node;
    list_for_each_entry_safe(node, safe, head, list) 
        free(node);

    free(head);
}

void print_list(struct list_head *head)
{
    element_t *entry;
    list_for_each_entry(entry, head, list) 
        printf("%ld -> ", entry->value);
    
    printf("\n");
}

// swap the median (among head, center, tail) with head
// void medianThree(node_t **list, int len) {
//     if (len < 3)
//         return;
    
//     node_t **tmp = list;

//     node_t *head = *list;

//     // center point
//     int i = (len - 1) / 2 - 1;
//     while (i--) {
//         tmp = &(*tmp)->next;
//     }
//     node_t *center = (*tmp)->next;

//     // tail
//     // node_t 
//     while ((*tmp)->next) {
//         tmp = &(*tmp)->next;
//     }
//     node_t *tail = *tmp;

//     // find median and swap
//     long h = head->value, c = center->value, t = tail->value;

//     if ((h > c) ^ (h > t)) 
//         return;
//     else if ((c > h) ^ (c > t)) {
//         // swap center to head
//         node_t **tmp = &center;

//     }

//     // printf("%ld %ld %ld\n", head->value, center->value, tail->value);
// }

/* Original implementation: https://alienryderflex.com/quicksort/ */
void quick_sort(struct list_head **head, int *max_time)
{
    if (!(*head))
        return;

    int n = list_length(*head);
    int value;
    int i = 0;
    int max_level = n * 0.25 + 20;
    struct list_head *begin[max_level];
    for(int i=1; i<max_level; i++)
        begin[i] = list_new();

    struct list_head *result = list_new(), *left = list_new(), *right = list_new();
    
    begin[0] = *head;
            
    while (i >= 0) {
        struct list_head *L = begin[i]->next, *R = begin[i]->prev;
        if (L != R) {
            // medianThree();
            struct list_head *pivot = L;
            value = list_entry(pivot, element_t, list)->value;
            list_del_init(pivot);

            element_t *entry, *safe;
            list_for_each_entry_safe(entry, safe, begin[i], list) {
                list_del(&entry->list);
                list_add(&entry->list, entry->value > value ? right : left);
            }
            
            list_splice_init(left, begin[i]);
            list_add(pivot, begin[i + 1]);
            list_splice_init(right, begin[i + 2]);

            i += 2;
            *max_time = MAX(*max_time, i);
        } else {
            if (list_is_singular(begin[i]))
                list_splice_init(begin[i], result);
            i--;
        }
    }
    *head = result;

    for(int i=0; i<max_level; i++)
        list_free(begin[i]);
    list_free(left);
    list_free(right);

}