#ifndef __INCLUDE_HEAP_H__
#define __INCLUDE_HEAP_H__

#include <types.h>
#include <vmm.h>
#include <list.h>

#define HEAP_START 0xe0000000

/*
* the heap is aligned by 8 bytes and the smallest chunk is 16 bytes
* the header is 4*4 = 16 bytes
*/
typedef struct header
{
    list_head_t entry;
    uint32_t size;
    uint32_t flag;
} header_t;

#define CHUNK_ALLOC 0x1
#define CHUNK_MAIN 0x10

#define IS_ALLOC(flag) (flag & CHUNK_ALLOC)
#define IS_MAIN(flag) (flag & CHUNK_MAIN)

#define SET_FLAG(header, s) \
    ((header_t *)(header))->flag |= s

#define CLR_FLAG(header, s) \
    ((header_t *)(header))->flag &= ~s

#define TEST_FLAG(header, s) \
    (((header_t *)(header))->flag & s)

#define SET_ALLOC(header) SET_FLAG(header, CHUNK_ALLOC)
#define CLR_ALLOC(header) CLR_FLAG(header, CHUNK_ALLOC)
#define TEST_ALLOC(header) TEST_FLAG(header, CHUNK_ALLOC)

#define GET_HEADER(chunk) (header_t *)((uint32_t)chunk - sizeof(header_t))
#define GET_CHUNK(header) (uint32_t)((uint32_t)header + sizeof(header_t))

static header_t chunklist;
static uint32_t heap_top = HEAP_START;

static void alloc_chunk(uint32_t start, uint32_t len);
static void free_chunk(header_t *chunk);
static void split_chunk(header_t *chunk, uint32_t len);
static void glue_chunk(header_t *chunk);

void init_heap();
void *kmalloc(uint32_t len);
void kfree(void *start);

#endif