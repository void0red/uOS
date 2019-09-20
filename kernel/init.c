#include <types.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>

int kernel_init(void)
{
    ce_clear();
    init_gdt();
    init_idt();
    printkc(RC_BLACK, RC_LIGHT_GREEN, "uOS is loading...\n%d.%d.%d\n", 2019, 9, 9);
    __asm__ volatile("int $0x3;");
    __asm__ volatile("hlt");
}
