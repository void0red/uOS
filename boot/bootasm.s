global start
extern bootmain
section .text
start:
[bits 16]
	cli
	cld

	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax

seta20.1:
	in al, 0x64
	test al, 0x2
	jnz seta20.1

	mov al, 0d1
	out 0x64, al

seta20.2:
	in al, 0x64
	test al, 0x2
	jnz seta20.2

	mov al, 0xdf
	out 0x60, al
	
	lgdt [gdtdesc]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp  0x8:protcseg

[bits 32]
protcseg:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0
	mov esp, 0x7c00

	call bootmain

	jmp $

align 4
gdt dq 0;						# null seg
	dd 0x0000ffff, 0x00cf9a00;	# code seg   
	dd 0x0000ffff, 0x00cf9200;	# data seg

gdtdesc dw $-gdt;
		dd gdt
