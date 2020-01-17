#include <vmm.h>

uint32_t kernel_pgd[PGD_COUNT] __attribute__((aligned(PGSIZE)));
static uint32_t kernel_pte[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PGSIZE)));

void init_vmm()
{
    for (uint32_t i = PGD_INDEX(PAGE_OFFSET), j = 0; i < PTE_COUNT + PGD_INDEX(PAGE_OFFSET); ++i, ++j)
    {
        kernel_pgd[i] = ((uint32_t)kernel_pte[j] - PAGE_OFFSET) | PG_P | PG_W;
    }
    for (uint32_t i = 1; i < PTE_COUNT * PTE_SIZE; ++i)
    {
        ((uint32_t *)kernel_pte)[i] = (i << 12) | PG_P | PG_W;
    }
    register_interrupt_handler(14, page_fault);
    switch_page((uint32_t)kernel_pgd - PAGE_OFFSET);
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

static inline void flush_pte(uint32_t va)
{
    __asm__ volatile("invlpg (%0)" ::"a"(va));
}

void map(uint32_t *pgd, uint32_t va, uint32_t pa, uint32_t flags)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);
    uint32_t *pte = (uint32_t *)(pgd[pgd_idx] & PAGE_MASK);
    if (!pte)
    {
        pte = (uint32_t *)pmm_alloc_page();
        pgd[pgd_idx] = (uint32_t)pte | PG_P | PG_W;
    }
    pte = (uint32_t *)((uint32_t)pte + PAGE_OFFSET);
    pte[pte_idx] = (pa & PAGE_MASK) | flags;

    flush_pte(va);
}

void unmap(uint32_t *pgd, uint32_t va)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);
    uint32_t *pte = (uint32_t *)(pgd[pgd_idx] & PAGE_MASK);
    pte = (uint32_t *)((uint32_t)pte + PAGE_OFFSET);
    pte[pte_idx] = 0;

    flush_pte(va);
}

uint32_t get_map(uint32_t *pgd, uint32_t va, uint32_t *pa)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);
    uint32_t *pte = (uint32_t *)(pgd[pgd_idx] & PAGE_MASK);
    if (!pte)
        return 0;
    pte = (uint32_t *)((uint32_t)pte + PAGE_OFFSET);
    if (pte[pte_idx] != 0 && pa)
    {
        *pa = pte[pte_idx] & PAGE_MASK;
        return 1;
    }
    return 0;
}