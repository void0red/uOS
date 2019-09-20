#ifndef __INCLUDE_PRINTK_H__
#define __INCLUDE_PRINTK_H__

#include <console.h>
#include <string.h>
#include <vargs.h>

#define ZEROPAD (1 << 0)
#define SIGN (1 << 1)
#define PLUS (1 << 2)
#define SPACE (1 << 3)
#define LEFT (1 << 4)
#define SPECIAL (1 << 5)
#define SMALL (1 << 6)

#define divl(n, base) ({                    \
int __res;                                  \
__asm__ volatile ("divl %4"                 \
                 :"=a"(n), "=d"(__res)      \
                 :"0"(n),"1"(0),"r"(base)); \
                 __res; })

#define is_digit(c) ((c) > '0' && (c) <= '9')

int printk(const char *format, ...);
int printkc(real_color_t back, real_color_t fore, const char *format, ...);

static int skip_atoi(const char **s);
static char *number(char *str, int num, int base, int size, int precision, int type);

static char buf[1024];
#endif