#ifndef __INCLUDE_VMM_H__
#define __INCLUDE_VMM_H__

#include <types.h>
#include <idt.h>
#include <pmm.h>

#define PAGE_OFFSET 0xc0000000
#define PG_P (1 << 0)
#define PG_W (1 << 1)
#define PG_U (1 << 2)

#define PAGE_MASK 0xfffff000
#define PGSIZE 0x1000
#define PGD_COUNT (PGSIZE / sizeof(uint32_t))
#define PTE_SIZE (PGSIZE / sizeof(uint32_t))
#define PGD_INDEX(x) (((x) >> 22) & 0x3ff)
#define PTE_INDEX(x) (((x) >> 12) & 0x3ff)
#define OFF_INDEX(x) ((x)&0xfff)

/*
* 0xC000_0000-0xE000_0000
*/
#define MEM_SIZE (512 << 20)
#define PTE_COUNT (MEM_SIZE / (PGSIZE << 10))

static void page_fault(trapframe_t *frame);
static void flush_pte(uint32_t va);

void init_vmm();
void switch_page(uint32_t page);
void map(uint32_t *pgd, uint32_t va, uint32_t pa, uint32_t flags);
void unmap(uint32_t *pgd, uint32_t va);
uint32_t get_map(uint32_t *pgd, uint32_t va, uint32_t *pa);

extern uint32_t kernel_pgd[PGD_COUNT];
#endif