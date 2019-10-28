%define KERNBASE 0xc0000000
%define REALLOC(x) (x - KERNBASE)
%define PGSIZE 0x1000
%define KSTACKSIZE (PGSIZE*2)

%define CR0_PE          0x00000001 ; Protection Enable
%define CR0_MP          0x00000002 ; Monitor coProcessor
%define CR0_EM          0x00000004 ; Emulation
%define CR0_TS          0x00000008 ; Task Switched
%define CR0_ET          0x00000010 ; Extension Type
%define CR0_NE          0x00000020 ; Numeric Errror
%define CR0_WP          0x00010000 ; Write Protect
%define CR0_AM          0x00040000 ; Alignment Mask
%define CR0_NW          0x20000000 ; Not Writethrough
%define CR0_CD          0x40000000 ; Cache Disable
%define CR0_PG          0x80000000 ; Paging

%define PTE_P           0x001      ; Present
%define PTE_W           0x002      ; Writeable
%define PTE_U           0x004      ; User
%define PTE_PWT         0x008      ; Write-Through
%define PTE_PCD         0x010      ; Cache-Disable
%define PTE_A           0x020      ; Accessed
%define PTE_D           0x040      ; Dirty
%define PTE_PS          0x080      ; Page Size
%define PTE_MBZ         0x180      ; Bits must be zero
%define PTE_AVAIL       0xE00      ; Available for software use

section .text
global kernel_entry
extern kernel_init
kernel_entry:
    mov eax, REALLOC(__boot_pgdir)
    mov cr3, eax

    mov eax, cr0
    or eax, (CR0_PE|CR0_PG|CR0_AM|CR0_WP|CR0_NE|CR0_TS|CR0_EM|CR0_MP)
    and eax, ~(CR0_TS|CR0_EM)
    mov cr0, eax
    lea eax, [next]
    jmp eax
next:
    xor eax, eax
    mov [__boot_pgdir], eax
    mov ebp, 0
    mov esp, bootstacktop
    call kernel_init

    jmp $

section .data align=PGSIZE
global bootstack, bootstacktop
bootstack:
    times KSTACKSIZE db 0
bootstacktop:

section .data.pgdir align=PGSIZE
global __boot_pgdir
__boot_pgdir:
    dd REALLOC(__boot_pt) + (PTE_P|PTE_U|PTE_W)
    times (KERNBASE >> 22 << 2) - ($ - __boot_pgdir) db 0
    dd REALLOC(__boot_pt) + (PTE_P|PTE_U|PTE_W)
    times PGSIZE - ($ - __boot_pgdir) db 0

%assign i 0
__boot_pt:
    %rep 1024
    dd i * PGSIZE + (PTE_P|PTE_W)
    %assign i i+1
    %endrep
