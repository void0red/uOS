#ifndef __INCLUDE_GDT_H__
#define __INCLUDE_GDT_H__
#include <types.h>

/*
* segment desciptor:
* [0...15]:     segment limit 00:15
* [16...31]:    base address 00:15
*
* [0...7]:      base address 16:23
* [8...11]:     segment type
* [12]:         descriptor type (0 = system; 1 = code or data)
* [13...14]:    descriptor privilege level
* [15]:         segment present
* [16...19]:    segment limit 16:19
* [20]:         available for use by system software
* [21]:         64-bit code segment (IA-32e mode only)
* [22]:         default operation size (0 = 16-bit segment; 1 = 32-bit segment)
* [23]:         granularity
* [24...31]     base address 24:31
*/

typedef struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t other;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t other);

extern void gdt_flush(uint32_t gdtr);
#endif