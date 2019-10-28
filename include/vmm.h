#ifndef __INCLUDE_VMM_H__
#define __INCLUDE_VMM_H__

#include <types.h>
#include <idt.h>

#define PG_OFFSET 0xc0000000
#define PG_P (1 << 0)
#define PG_W (1 << 1)
#define PG_U (1 << 2)

#define PAGE_MASK 0xfffff000
#define PGSIZE 0x1000
#define PGD_SIZE (PGSIZE / 32)
#define PTE_SIZE (PGSIZE / 32)
#define PGD_INDEX(x) (((x) >> 22) & 0x3ff)
#define PTE_INDEX(x) (((x) >> 12) & 0x3ff)
#define OFF_INDEX(x) ((x)&0xfff)

#define MEM_SIZE (512 << 20)
#define PTE_COUNT (MEM_SIZE / (PGSIZE << 10))

void init_vmm();
static void page_fault(trapframe_t *frame);
void switch_page(uint32_t page);
#endif