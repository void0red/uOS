ENTRY(kernel_init)
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
SECTIONS {
    . = 0x100000;
	PROVIDE(kernel_begin = .);
	.text : {
		*(.text .stub .text.* .gnu.linkonce.t.*)
	}

	.data : {
		*(.data)
        *(.rodata .rodata.*)        
	}

	.bss : {
		*(.bss)
	}

    .stab : {
        PROVIDE(__STAB_BEGIN__ = .);
		*(.stab);
        PROVIDE(__STAB_END__ = .);
		BYTE(0)
	}

    .stabstr : {
        PROVIDE(__STABSTR_BEGIN__ = .);
		*(.stabstr);
        PROVIDE(__STABSTR_END__ = .);
		BYTE(0)
	}

	PROVIDE(kernel_end = .);

	/DISCARD/ : {
		*(.eh_frame .note.GNU-stack)
	}
}
