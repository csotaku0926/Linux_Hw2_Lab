#include "LRUcache.h"

LRUCache *lRUCacheCreate(int capacity)
{
    LRUCache *cache = malloc(2 * sizeof(int) + sizeof(struct list_head) +
                             capacity * sizeof(struct list_head));
    cache->capacity = capacity;
    cache->count = 0;
    INIT_LIST_HEAD(&cache->dhead);
    for (int i = 0; i < capacity; i++)
        INIT_HLIST_HEAD(&cache->hhead[i]);
    return cache;
}

void lRUCacheFree(LRUCache *obj)
{
    struct list_head *pos, *n;
    list_for_each_safe (pos, n, &obj->dhead) {
        LRUNode *cache = list_entry(pos, LRUNode, link);
        list_del(&cache->link);
        free(cache);
    }
    free(obj);
}

int lRUCacheGet(LRUCache *obj, int key)
{
    int hash = key % obj->capacity;
    struct hlist_node *pos;
    hlist_for_each (pos, &obj->hhead[hash]) {
        LRUNode *cache = list_entry(pos, LRUNode, node);
        if (cache->key == key) {
            list_move(&cache->link, &obj->dhead);
            return cache->value;
        }
    }
    return -1;
}

void lRUCachePut(LRUCache *obj, int key, int value)
{
    LRUNode *cache = NULL;
    int hash = key % obj->capacity;
    struct hlist_node *pos;
    hlist_for_each (pos, &obj->hhead[hash]) {
        LRUNode *c = list_entry(pos, LRUNode, node);
        if (c->key == key) {
            list_move(&c->link, &obj->dhead);
            cache = c;
        }
    }

    // if the node with key not found
    if (!cache) {
        if (obj->count == obj->capacity) {
            cache = list_last_entry(&obj->dhead, LRUNode, link);
            list_move(&cache->link, &obj->dhead);
            hlist_del(&cache->node);
            hlist_add_head(&cache->node, &obj->hhead[hash]);
        } else {
            cache = malloc(sizeof(LRUNode));
            hlist_add_head(&cache->node, &obj->hhead[hash]);
            list_add(&cache->link, &obj->dhead);
            obj->count++;
        }
        cache->key = key;
    }

    // update value
    cache->value = value;
}

int main(void)
{
    LRUCache *lru_cache = lRUCacheCreate(2);
    
    lRUCachePut(lru_cache, 1, 111);
    lRUCachePut(lru_cache, 2, 222);
    int ret1 = lRUCacheGet(lru_cache, 1); // 1
    lRUCachePut(lru_cache, 3, 333);
    int ret2 = lRUCacheGet(lru_cache, 2); // -1

    printf("%d %d\n", ret1, ret2);

    lRUCacheFree(lru_cache);
}