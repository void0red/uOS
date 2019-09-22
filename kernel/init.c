#include <types.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <pmm.h>

int kernel_init(void)
{
    init_gdt();
    init_idt();
    cli();
    
    ce_clear();
    
    printkc(RC_BLACK, RC_LIGHT_GREEN, "uOS is loading...\n%d.%d.%d\n", 2019, 9, 9);

    show_memory_map();
    
    hlt();
}
