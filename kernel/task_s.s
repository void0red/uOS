global switch_to
; typedef struct _context
; {
;     uint32_t esp;
;     uint32_t ebp;
;     uint32_t ebx;
;     uint32_t esi;
;     uint32_t edi;
;     uint32_t eflags;
; } context_t;
switch_to:
    mov eax, [esp+0x04]
    mov [eax+0x00], esp
    mov [eax+0x04], ebp
    mov [eax+0x08], ebx
    mov [eax+0x0c], esi
    mov [eax+0x10], edi
    pushf
    pop ecx
    mov [eax+0x14], ecx

    mov eax, [esp+0x08]
    mov esp, [eax+0x00]
    mov ebp, [eax+0x04]
    mov ebx, [eax+0x08]
    mov esi, [eax+0x0c]
    mov edi, [eax+0x10]
    mov ecx, [eax+0x14]
    push ecx
    popf
    ret