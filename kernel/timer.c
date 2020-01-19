#include <timer.h>
void timer_callback(trapframe_t *regs)
{
    static uint32_t tick = 0;
    tick++;
    // printkc(RC_BLACK, RC_RED, "Tick %d\n", tick++);
    schedule();
}

void init_timer(uint32_t frequency)
{
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);

    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);
    outb(0x40, low);
    outb(0x40, high);
}