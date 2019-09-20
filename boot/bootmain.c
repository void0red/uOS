#include <types.h>
#include <common.h>
#include <elf.h>

#define SECTSIZE 512
#define ELFHDR ((struct elfhdr *)0x10000)

static void waitdisk(void)
{
    while ((inb(0x1F7) & 0xC0) != 0x40)
        ;
}

static void readsect(void *dest, uint32_t secno)
{
    waitdisk();

    outb(0x1F2, 1);
    outb(0x1F3, secno & 0xFF);
    outb(0x1F4, (secno >> 8) & 0xFF);
    outb(0x1F5, (secno >> 16) & 0xFF);
    outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    outb(0x1F7, 0x20);

    waitdisk();

    insl(0x1F0, dest, SECTSIZE / 4);
}

static void readseg(uint32_t va, uint32_t count, uint32_t offset)
{
    uint32_t end_va = va + count;

    va -= offset % SECTSIZE;

    for (uint32_t secno = (offset / SECTSIZE) + 1; va < end_va; va += SECTSIZE, secno++)
    {
        readsect((void *)va, secno);
    }
}

void bootmain(void)
{
    readseg((uint32_t)ELFHDR, SECTSIZE * 8, 0);
    do
    {
        if (ELFHDR->e_magic != ELF_MAGIC)
            break;

        struct proghdr *ph = (struct proghdr *)((uint32_t)ELFHDR + ELFHDR->e_phoff);
        struct proghdr *eph = ph + ELFHDR->e_phnum;
        while (ph < eph)
        {
            readseg(ph->p_va & 0xFFFFFF, ph->p_memsz, ph->p_offset);
            ph++;
        }

        ((void (*)(void))(ELFHDR->e_entry & 0xFFFFFF))();

    } while (0);

    outw(0x8a00, 0x8a00);
    outw(0x8a00, 0x8e00);
    __asm__ volatile("hlt");
}
