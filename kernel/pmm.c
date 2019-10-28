#include <pmm.h>

struct e820map *mem_info = (struct e820map *)mem_map;

static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];
/*
* //todo
* here is a bug.
* static variable `pmm_stack_top` is not 0 init.
*/
static uint32_t pmm_stack_top;
uint32_t phy_page_count;

void show_memory_map()
{
    for (int i = 0; i < mem_info->cnt; ++i)
    {
        uint32_t begin = mem_info->map[i].addr;
        uint32_t end = begin + mem_info->map[i].size;
        printk("[%d (%d)]: 0x%08X - 0x%08X\n", i, mem_info->map[i].type, begin, end);
    }
    printk("all available 0x%X pages\n", phy_page_count);
    printk("\n");

    printk("kernel in memory begin: 0x%08X\n", kernel_begin);
    printk("kernel in memory end:   0x%08X\n", kernel_end);
    printk("kernel in memory used:   %d KB\n", (kernel_end - kernel_begin + 1023) / 1024);
}

void init_pmm()
{
    pmm_stack_top = 0;
    printk("pmm_statck_top: %d\n", pmm_stack_top);
    for (int i = 0; i < mem_info->cnt; ++i)
    {
        if ((mem_type_t)(mem_info->map[i].type) == MEM_NORMAL && mem_info->map[i].addr >= KERNBASE)
        {
            uint32_t begin = mem_info->map[i].addr;
            uint32_t end = begin + mem_info->map[i].size;
            for (uint32_t page = begin; page < end; page += PGSIZE)
            {
                pmm_free_page(page);
                phy_page_count++;
            }
        }
    }
}

uint32_t pmm_alloc_page()
{
    if (pmm_stack_top == 0)
        panic("out of pmm_stack stack");
    uint32_t page = pmm_stack[pmm_stack_top--];
    return page;
}

void pmm_free_page(uint32_t page)
{
    if (pmm_stack_top == PAGE_MAX_SIZE)
        panic("out of pmm_stack stack");
    pmm_stack[++pmm_stack_top] = page;
}