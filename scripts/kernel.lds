ENTRY(kernel_entry)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
SECTIONS {
    . = 0xC0100000;
	PROVIDE(kernel_begin = .);
	.text : {
		*(.text .stub .text.* .gnu.linkonce.t.*)
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

	. = ALIGN(0x1000);
	.data : {
		*(.data)
        *(.rodata .rodata.*)
	}

	. = ALIGN(0x1000);
	.data.pgdir : {
		*(.data.pgdir)
	}

	.bss : {
		*(.bss)
	}

	PROVIDE(kernel_end = .);

	/DISCARD/ : {
		*(.eh_frame .note.GNU-stack .comment)
	}
}
