#include <pmm.h>

void show_memory_map()
{
    printk("kernel in memory begin: 0x%08X\n", kernel_begin);
    printk("kernel in memory end:   0x%08X\n", kernel_end);
    printk("kernel in memory used:   %d KB\n", (kernel_end - kernel_begin + 1023) / 1024);
}

static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];
static uint32_t pmm_stack_top;
uint32_t phy_page_count;

void init_pmm()
{
    uint32_t page_begin = ((uint32_t)(kernel_end + PMM_PAGE_SIZE - 1) & PHY_PAGE_MASK);
    uint32_t page_end = QEMU_DEFAULT_MEM & PHY_PAGE_MASK;
    for (uint32_t i = page_begin; i < page_end; i += PMM_PAGE_SIZE)
    {
        pmm_free_page(i);
        phy_page_count++;
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