#include <gdt.h>

#define GDT_LENGTH 5
gdt_entry_t gdt_entries[GDT_LENGTH];
gdt_ptr_t gdtr;
void init_gdt()
{
    gdtr.limit = sizeof(gdt_entry_t) * GDT_LENGTH - 1;
    gdtr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // null seg
    gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf); // code seg
    gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf); // data seg
    gdt_set_gate(3, 0, 0xffffffff, 0xfa, 0xcf); // user code seg
    gdt_set_gate(4, 0, 0xffffffff, 0xf2, 0xcf); // user data seg

    gdt_flush((uint32_t)&gdtr);
}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t other)
{
    num = num % GDT_LENGTH;
    gdt_entries[num].base_low = base & 0xffff;
    gdt_entries[num].base_mid = (base >> 16) & 0xff;
    gdt_entries[num].base_high = (base >> 24) & 0xff;

    gdt_entries[num].limit_low = limit & 0xffff;
    gdt_entries[num].other = ((limit >> 16) & 0xf) | other;
    gdt_entries[num].access = access;
}
