OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(start)
SECTIONS {
    . = 0x7c00;
    .startup : {
        *bootasm.o(.text)
    }
    .text : {
        *(.text)
    }
    .data : {
        *(.data .rodata)
    }
    /DISCARD/ : {*(.eh_*)}
}