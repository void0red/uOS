global start
extern bootmain

%define meminfo 0x8000

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

probe_memory:
    mov dword[meminfo], 0
    xor ebx, ebx
    mov edi, meminfo+4
start_probe:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 0x534d4150
    int 0x15
    jnc cont
    mov dword[meminfo], 12345
    jmp finish_probe
cont:
    add edi, 20
    mov eax, dword[meminfo]
    inc eax
    mov dword[meminfo], eax
    cmp ebx, 0
    jnz start_probe
finish_probe:

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
gdt dq 0                      ; # null seg
    dd 0x0000ffff, 0x00cf9a00 ; # code seg
    dd 0x0000ffff, 0x00cf9200 ; # data seg

gdtdesc dw $-gdt
        dd gdt
