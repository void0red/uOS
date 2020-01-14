#ifndef __INCLUDE_PMM_H__
#define __INCLUDE_PMM_H__
#include <types.h>
#include <printk.h>
#include <debug.h>

#define STACK_SIZE 1 << 13
#define PMM_MAX_SIZE 512 * (1 << 20)
#define PGSIZE 0x1000
#define PAGE_MAX_SIZE (PMM_MAX_SIZE / PGSIZE)
#define PHY_PAGE_MASK 0xfffff000
#define KERNBASE 0x100000
#define VMA 0xc0000000

// Type 1: Usable (normal) RAM
// Type 2: Reserved - unusable
// Type 3: ACPI reclaimable memory
// Type 4: ACPI NVS memory
// Type 5: Area containing bad memory
typedef enum
{
    MEM_NONE,
    MEM_NORMAL,
    MEM_RESERVED,
    MEM_ACPI_RECLAIMABLE,
    MEM_ACPI_NVS,
    MEM_BAD
} mem_type_t;

#define E820MAX 20
#define mem_map 0xc0008000

struct e820map
{
    uint32_t cnt;
    struct
    {
        uint64_t addr;
        uint64_t size;
        uint32_t type;
    } __attribute__((packed)) map[E820MAX];
};

extern uint8_t kernel_begin[];
extern uint8_t kernel_end[];

void show_memory_map();
void init_pmm();
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t page);
#endif
