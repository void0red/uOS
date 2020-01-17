#include <types.h>
#include <printk.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <pmm.h>
#include <vmm.h>
#include <heap.h>
int kernel_init(void)
{
    init_gdt();
    init_idt();
    init_timer(200);
    // cli();

    ce_clear();

    printkc(RC_BLACK, RC_LIGHT_GREEN, "uOS is loading...\n%d.%d.%d\n", 2019, 9, 9);

    init_pmm();
    show_memory_map();
    init_vmm();
    init_heap();

    hlt();
}
