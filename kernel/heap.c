#include <heap.h>
void init_heap()
{
    INIT_LIST_HEAD(&chunklist.entry);
}

void *kmalloc(uint32_t len)
{
    if (len < 16)
        len = 16;
    len = (len + 7) & 0xfffffff8;
    len += sizeof(header_t);

    header_t *iter;
    list_for_each_entry(iter, &chunklist.entry, entry)
    {
        if (TEST_ALLOC(iter) == 0 && iter->size >= len)
        {
            split_chunk(iter, len);
            SET_ALLOC(iter);
            return (void *)GET_CHUNK(iter);
        }
    }

    header_t *prev = list_entry(iter->entry.prev, header_t, entry);
    uint32_t start;
    if (list_empty(&chunklist.entry))
    {
        start = HEAP_START;
    }
    else
    {
        start = (uint32_t)prev + prev->size;
    }

    alloc_chunk(start, len);
    header_t *chunk = (header_t *)start;
    INIT_LIST_HEAD(&chunk->entry);
    chunk->size = len;
    SET_ALLOC(chunk);
    list_add_tail(&chunklist.entry, &chunk->entry);

    return (void *)GET_CHUNK(chunk);
}

void kfree(void *start)
{
    header_t *header = GET_HEADER(start);
    CLR_ALLOC(header);
    glue_chunk(header);
}

static void alloc_chunk(uint32_t start, uint32_t len)
{
    while (start + len > heap_top)
    {
        uint32_t page = pmm_alloc_page();
        map(kernel_pgd, heap_top, page, PG_P | PG_W);
        heap_top += PGSIZE;
    }
}

static void free_chunk(header_t *chunk)
{
    list_del(&chunk->entry);
    while ((heap_top - PGSIZE) >= (uint32_t)chunk)
    {
        heap_top -= PGSIZE;
        uint32_t page;
        get_map(kernel_pgd, heap_top, &page);
        unmap(kernel_pgd, heap_top);
        pmm_free_page(page);
    }
}

static void split_chunk(header_t *chunk, uint32_t len)
{
    if (chunk->size - len >= 2 * sizeof(header_t))
    {
        header_t *newchunk = (header_t *)((uint32_t)chunk + chunk->size);
        INIT_LIST_HEAD(&newchunk->entry);
        newchunk->size = chunk->size - len;
        CLR_ALLOC(newchunk);
        list_add(&chunk->entry, &newchunk->entry);
    }
}

static void glue_chunk(header_t *chunk)
{
    if (list_empty(&chunk->entry))
        return;

    header_t *nextchunk = list_entry(chunk->entry.next, header_t, entry);
    if (nextchunk != chunk && !TEST_ALLOC(nextchunk) && nextchunk->size > 0)
    {
        chunk->size += nextchunk->size;
        list_del(&nextchunk->entry);
    }

    header_t *prevchunk = list_entry(chunk->entry.prev, header_t, entry);
    if (prevchunk != chunk && !TEST_ALLOC(prevchunk) && prevchunk->size > 0)
    {
        prevchunk->size += chunk->size;
        list_del(&chunk->entry);
    }

    if (list_empty(&prevchunk->entry))
    {
        free_chunk(prevchunk);
    }
}