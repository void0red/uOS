#include <types.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>

int kernel_init(void)
{
    init_gdt();
    init_idt();

    ce_clear();
    
    printkc(RC_BLACK, RC_LIGHT_GREEN, "uOS is loading...\n%d.%d.%d\n", 2019, 9, 9);

    init_timer(10);

    hlt();
}
