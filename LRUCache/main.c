#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lru.h"

int main(void)
{
    int capacity = 3;
    LRUCache *cache = lRUCacheCreate(capacity);
    printf("This cache has a capacity of 3.\n");

    lRUCachePut(cache, 1, 12);
    printf("Get key = 1 value : %d\n", lRUCacheGet(cache, 1));
    lRUCachePut(cache, 2, 24);
    printf("Get key = 2 value : %d\n", lRUCacheGet(cache, 2));
    lRUCachePut(cache, 3, 36);
    printf("Get key = 3 value : %d\n", lRUCacheGet(cache, 3));
    lRUCachePut(cache, 4, 48);
    printf("Get key = 4 value : %d\n", lRUCacheGet(cache, 4));
    printf("Get key = 1 value : %d\n", lRUCacheGet(cache, 1));

    lRUCacheFree(cache);
}