#ifndef __INCLUDE_DEBUG_H__
#define __INCLUDE_DEBUG_H__

#include <types.h>
#include <printk.h>
#include <elf.h>

typedef struct debug_info
{
    const char *eip_file;
    int eip_line;
    const char *eip_fn_name;
    int eip_fn_name_len;
    uint32_t eip_fn_addr;
    int eip_fn_narg;
} debug_info_t;



void print_status();
void panic(const char *msg);
void print_stack_trace();

static void stab_binsearch(const elf_stab_t *stabs, int *region_left, int *region_right, int type, uint32_t addr);
static const char *find_symbol(uint32_t addr);
// void print_debuginfo(uint32_t eip);
// int debuginfo_eip(uint32_t addr, debug_info_t *info);

extern const elf_stab_t __STAB_BEGIN__[];
extern const elf_stab_t __STAB_END__[];
extern const char __STABSTR_BEGIN__[];
extern const char __STABSTR_END__[];

#endif