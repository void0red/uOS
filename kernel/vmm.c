#include <vmm.h>

uint32_t kernel_pgd[PGD_SIZE] __attribute__((aligned(PGSIZE)));
static uint32_t kernel_pte[PTE_COUNT][PGSIZE] __attribute__((aligned(PGSIZE)));

void init_vmm()
{
    for (uint32_t i = PGD_INDEX(PG_OFFSET), j = 0; i < PTE_COUNT + PGD_INDEX(PG_OFFSET); ++i, ++j)
    {
        kernel_pgd[i] = ((uint32_t)kernel_pte[j] - PG_OFFSET) | PG_P | PG_W;
    }
    for (uint32_t i = 1; i < PTE_COUNT * PGSIZE; ++i)
    {
        ((uint32_t *)kernel_pte)[i] = (i << 12) | PG_P | PG_W;
    }
    register_interrupt_handler(14, page_fault);
    switch_page((uint32_t)kernel_pgd - PG_OFFSET);
}

static void page_fault(trapframe_t *frame)
{
    uint32_t cr2;
    __asm__ volatile("mov %%cr2, %0"
                     : "=r"(cr2));
    printk("Page fault(%x) at 0x%08X, virtual at 0x%08X\n", frame->err_code, frame->eip, cr2);

    if (!(frame->err_code & 0x1))
    {
        printk("not present\n");
    }

    if (frame->err_code & 0x2)
    {
        printk("write error\n");
    }
    else
    {
        printk("read error\n");
    }

    if (frame->err_code & 0x4)
    {
        printk("occur in user mode\n");
    }
    else
    {
        printk("occur in kernel mode\n");
    }
    if (frame->err_code & 0x8)
    {
        printk("reserved bits were overwritted\n");
    }
    if (frame->err_code & 0x10)
    {
        printk("occur duing an instruction fetch\n");
    }
    while (1)
        ;
}

inline void switch_page(uint32_t page)
{
    __asm__ volatile("mov %0, %%cr3" ::"r"(page));
}