#ifndef __INCLUDE_X86_H__
#define __INCLUDE_X86_H__

#include <types.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t data;
    __asm__ volatile("inb %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t data;
    __asm__ volatile("inw %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

static inline void outb(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %0, %1" ::"a"(data), "d"(port));
}
static inline void outw(uint16_t port, uint16_t data)
{
    __asm__ volatile("outw %0, %1" ::"a"(data), "d"(port));
}

static inline void insl(uint32_t port, void *addr, int cnt)
{
    __asm__ volatile("cld;"
                     "repne;insl;"
                     : "=D"(addr), "=c"(cnt)
                     : "d"(port), "0"(addr), "1"(cnt)
                     : "memory", "cc");
}

static inline void sti()
{
    __asm__ volatile("sti");
}

static inline void hlt()
{
    __asm__ volatile("hlt");
}

#endif
