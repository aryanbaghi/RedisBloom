/*
* Copyright 2019 Redis Labs Ltd. and Contributors
*
* This file is available under the Redis Labs Source Available License Agreement
*/
#ifndef RM_TOPK_H
#define RM_TOPK_H

#include <stdint.h>     //  uint32_t
#include <stddef.h>     //  size_t
#include <stdbool.h>    //  bool
#include <string.h>     //  memcpy
#include <stdlib.h>     //  calloc

#ifdef REDIS_MODULE_TARGET // should be in .h or .c
#include "hiredis.h"
#define TOPK_CALLOC(count, size) RedisModule_Calloc(count, size)
#define TOPK_FREE(ptr) RedisModule_Free(ptr)
#else
#define TOPK_CALLOC(count, size) calloc(count, size)
#define TOPK_FREE(ptr) free(ptr)
#endif

typedef uint32_t counter_t;

typedef struct HeapBucket {
    uint32_t fp;
    uint32_t itemlen;
    char *item;
    counter_t count;
} HeapBucket;

typedef struct Bucket {
    uint32_t fp;        //  fingerprint
    counter_t count;
} Bucket;

typedef struct topk
{
    uint32_t k;
    uint32_t width;
    uint32_t depth;
    double decay;
    Bucket *data;
    struct HeapBucket *heap;
    //  TODO: add function pointers for fast vs accurate
} TopK;

TopK *TopK_Create(uint32_t k, uint32_t width, uint32_t depth, double decay);
void TopK_Destroy(TopK *topk);
void TopK_Add(TopK *topk, const char *item, size_t itemlen);
bool TopK_Query(TopK *topk, const char *item, size_t itemlen);
size_t TopK_Count(TopK *topk, const char *item, size_t itemlen);
void TopK_List(TopK *topk, char **heapList);

#endif