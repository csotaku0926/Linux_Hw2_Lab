#ifndef Lab2_LRUcache_h
#define Lab2_LRUcache_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define container_of(ptr, type, member) \
    ((type *) ((char *) (ptr) - (size_t) & (((type *) 0)->member)))

#define list_entry(ptr, type, member) container_of(ptr, type, member)

#define hlist_for_each(pos, head) \
    for (pos = (head)->first; pos; pos = pos->next)

#define hlist_for_each_safe(pos, n, head)        \
    for (pos = (head)->first; pos && ({          \
                                  n = pos->next; \
                                  true           \
                              });                \
         pos = n)

#define list_first_entry(ptr, type, field) list_entry((ptr)->next, type, field)
#define list_last_entry(ptr, type, field) list_entry((ptr)->prev, type, field)

#define list_for_each(p, head) for (p = (head)->next; p != head; p = p->next)

#define list_for_each_safe(p, n, head) \
    for (p = (head)->next, n = p->next; p != (head); p = n, n = p->next)

/* Linux kernel style hash function */
// https://github.com/torvalds/linux/blob/master/tools/include/linux/hash.h
#define GOLDEN_RATIO_32 0x61C88647

unsigned int hash_32(unsigned int key, unsigned int bits)
{
    return (key * GOLDEN_RATIO_32) >> (32 - bits);
}

struct hlist_node;

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct list_head {
    struct list_head *next, *prev;
};

typedef struct {
    int capacity;
    int count;
    struct list_head dhead;
    struct hlist_head hhead[];
} LRUCache;

typedef struct {
    int key;
    int value;
    struct hlist_node node;
    struct list_head link;
} LRUNode;

/* some list.h function */

void __list_add(struct list_head *new_,
                struct list_head *prev,
                struct list_head *next)
{
    next->prev = new_;
    new_->next = next;
    new_->prev = prev;
    prev->next = new_;
}

void __list_del(struct list_head *entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

void INIT_HLIST_HEAD(struct hlist_head *h)
{
    h->first = NULL;
}

void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    if (h->first)
        h->first->pprev = &n->next;
    n->next = h->first;
    n->pprev = &h->first;
    h->first = n;
}

void hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next, **pprev = n->pprev;
    *pprev = next;
    if (next)
        next->pprev = pprev;
}


void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list->prev = list;
}

void list_add(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head, head->next);
}

void list_del(struct list_head *entry)
{
    __list_del(entry);
    entry->next = entry->prev = NULL;
}

void list_move(struct list_head *entry, struct list_head *head)
{
    __list_del(entry);
    list_add(entry, head);
}


#endif