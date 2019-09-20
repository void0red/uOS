#include <debug.h>

void print_status()
{
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;
    __asm__ volatile("mov %%cs, %0;"
                     "mov %%ds, %1;"
                     "mov %%es, %2;"
                     "mov %%ss, %3;"
                     : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));

    printk("%d: @ring %d\n", round, reg1 & 0x3);
    printk("%d: cs = %x\n", round, reg1);
    printk("%d: ds = %x\n", round, reg2);
    printk("%d: es = %x\n", round, reg3);
    printk("%d: ss = %x\n", round, reg4);
    ++round;
}

void print_stack_trace()
{
    uint32_t *ebp, *eip;
    __asm__ volatile("mov %%ebp, %0"
                     : "=r"(ebp));
    while (ebp)
    {
        eip = ebp + 1;
        // print_debuginfo(*eip);
        printk("[%#X]: %s\n", *eip, find_symbol(*eip));
        ebp = (uint32_t *)*ebp;
    }
}

void panic(const char *msg)
{
    printk("*** System panic: %s\n", msg);
    print_stack_trace();
    printk("***\n");
}


static void stab_binsearch(const elf_stab_t *stabs, int *region_left, int *region_right, int type, uint32_t addr)
{
    int l = *region_left, r = *region_right, any_matches = 0;

    while (l <= r)
    {
        int true_m = (l + r) / 2, m = true_m;

        while (m >= l && stabs[m].n_type != type)
        {
            m--;
        }
        if (m < l)
        {
            l = true_m + 1;
            continue;
        }

        any_matches = 1;
        if (stabs[m].n_value < addr)
        {
            *region_left = m;
            l = true_m + 1;
        }
        else if (stabs[m].n_value > addr)
        {
            *region_right = m - 1;
            r = m - 1;
        }
        else
        {
            *region_left = m;
            l = m;
            addr++;
        }
    }

    if (!any_matches)
    {
        *region_right = *region_left - 1;
    }
    else
    {
        l = *region_right;
        for (; l > *region_left && stabs[l].n_type != type; l--)
            ;
        *region_left = l;
    }
}

static const char *find_symbol(uint32_t addr)
{
    const elf_stab_t *stabs = __STAB_BEGIN__;
    const elf_stab_t *stabs_end = __STAB_END__;
    const char *stabstr = __STABSTR_BEGIN__;
    const char *stabstr_end = __STABSTR_END__;

    int lfile = 0;
    int rfile = stabs_end - stabs - 1;

    stab_binsearch(stabs, &lfile, &rfile, N_FUN, addr);

    if (lfile <= rfile)
        return stabstr + stabs[lfile].n_strx;
    else
        return NULL;
}

// int debuginfo_eip(uint32_t addr, debug_info_t *info)
// {
//     const elf_stab_t *stabs, *stab_end;
//     const char *stabstr, *stabstr_end;

//     info->eip_file = "<unknown>";
//     info->eip_line = 0;
//     info->eip_fn_name = "<unknown>";
//     info->eip_fn_name_len = 9;
//     info->eip_fn_addr = addr;
//     info->eip_fn_narg = 0;

//     stabs = __STAB_BEGIN__;
//     stab_end = __STAB_END__;
//     stabstr = __STABSTR_BEGIN__;
//     stabstr_end = __STABSTR_END__;

//     if (stabstr_end <= stabstr || stabstr_end[-1] != 0)
//     {
//         return -1;
//     }

//     int lfile = 0, rfile = (stab_end - stabs) - 1;
//     stab_binsearch(stabs, &lfile, &rfile, N_SO, addr);
//     if (lfile == 0)
//         return -1;

//     int lfun = lfile, rfun = rfile;
//     int lline, rline;
//     stab_binsearch(stabs, &lfun, &rfun, N_FUN, addr);

//     if (lfun <= rfun)
//     {

//         if (stabs[lfun].n_strx < stabstr_end - stabstr)
//         {
//             info->eip_fn_name = stabstr + stabs[lfun].n_strx;
//         }
//         info->eip_fn_addr = stabs[lfun].n_value;
//         addr -= info->eip_fn_addr;
//         lline = lfun;
//         rline = rfun;
//     }
//     else
//     {

//         info->eip_fn_addr = addr;
//         lline = lfile;
//         rline = rfile;
//     }

//     stab_binsearch(stabs, &lline, &rline, N_SLINE, addr);
//     if (lline <= rline)
//     {
//         info->eip_line = stabs[rline].n_desc;
//     }
//     else
//     {
//         return -1;
//     }

//     while (lline >= lfile && stabs[lline].n_type != N_SOL && (stabs[lline].n_type != N_SO || !stabs[lline].n_value))
//     {
//         lline--;
//     }
//     if (lline >= lfile && stabs[lline].n_strx < stabstr_end - stabstr)
//     {
//         info->eip_file = stabstr + stabs[lline].n_strx;
//     }

//     if (lfun < rfun)
//     {
//         for (lline = lfun + 1;
//              lline < rfun && stabs[lline].n_type == N_PSYM;
//              lline++)
//         {
//             info->eip_fn_narg++;
//         }
//     }
//     return 0;
// }

// void print_debuginfo(uint32_t eip)
// {
//     debug_info_t info;
//     if (debuginfo_eip(eip, &info) != 0)
//     {
//         printk("    <unknow>: -- 0x%08x --\n", eip);
//     }
//     else
//     {
//         char fnname[256];
//         int j;
//         for (j = 0; j < info.eip_fn_name_len; j++)
//         {
//             fnname[j] = info.eip_fn_name[j];
//         }
//         fnname[j] = '\0';
//         printk("    %s:%d: %s+%d\n", info.eip_file, info.eip_line,
//                fnname, eip - info.eip_fn_addr);
//     }
// }
