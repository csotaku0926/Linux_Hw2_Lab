#include <stdio.h>

struct list_head;

typedef int (*list_cmp_func_t)(void *,
                               const struct list_head *,
                               const struct list_head *);

typedef struct {
    struct list_head list;
    int val;
    int seq;
} element_t;

void timsort(void *priv, struct list_head *head, list_cmp_func_t cmp);

size_t list_length(struct list_head *head);
