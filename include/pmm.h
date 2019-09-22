#ifndef __INCLUDE_PMM_H__
#define __INCLUDE_PMM_H__
#include <types.h>
#include <printk.h>
#include <debug.h>

#define STACK_SIZE 1 << 13
#define PMM_MAX_SIZE 512 * (1 << 20)
#define PMM_PAGE_SIZE 0x1000
#define PAGE_MAX_SIZE (PMM_MAX_SIZE / PMM_PAGE_SIZE)
#define PHY_PAGE_MASK 0xfffff000
#define QEMU_DEFAULT_MEM 128 * (1 << 20)

extern uint8_t kernel_begin[];
extern uint8_t kernel_end[];

void show_memory_map();
void init_pmm();
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t page);
#endif