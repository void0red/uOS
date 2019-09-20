#ifndef __INCLUDE_ELF_H__
#define __INCLUDE_ELF_H__

#include <types.h>

#define ELF_MAGIC 0x464C457FU

struct elfhdr
{
    uint32_t e_magic;
    uint8_t e_elf[12];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct proghdr
{
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_va;
    uint32_t p_pa;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
};
typedef struct sechdr
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_align;
    uint32_t sh_entsize;
} sechdr_t;

#define N_GSYM 0x20   // global symbol
#define N_FNAME 0x22  // F77 function name
#define N_FUN 0x24    // procedure name
#define N_STSYM 0x26  // data segment variable
#define N_LCSYM 0x28  // bss segment variable
#define N_MAIN 0x2a   // main function name
#define N_PC 0x30     // global Pascal symbol
#define N_RSYM 0x40   // register variable
#define N_SLINE 0x44  // text segment line number
#define N_DSLINE 0x46 // data segment line number
#define N_BSLINE 0x48 // bss segment line number
#define N_SSYM 0x60   // structure/union element
#define N_SO 0x64     // main source file name
#define N_LSYM 0x80   // stack variable
#define N_BINCL 0x82  // include file beginning
#define N_SOL 0x84    // included source file name
#define N_PSYM 0xa0   // parameter variable
#define N_EINCL 0xa2  // include file end
#define N_ENTRY 0xa4  // alternate entry point
#define N_LBRAC 0xc0  // left bracket
#define N_EXCL 0xc2   // deleted include file
#define N_RBRAC 0xe0  // right bracket
#define N_BCOMM 0xe2  // begin common
#define N_ECOMM 0xe4  // end common
#define N_ECOML 0xe8  // end common (local name)
#define N_LENG 0xfe   // length of preceding entry

typedef struct elf_stab
{
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uint32_t n_value;
} __attribute__((packed)) elf_stab_t;
#endif
