#ifndef __INCLUDE_TYPES_H__
#define __INCLUDE_TYPES_H__

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef True
#define True 1
#define False 0
#endif

typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

#ifndef typecheck
#define typecheck(type, x)             \
    ({                                 \
        type __dummy;                  \
        typeof(x) __dummy2;            \
        (void)(&__dummy == &__dummy2); \
        1;                             \
    })
#endif

#define LOW4B(x)               \
    ({                         \
        typecheck(uint8_t, x); \
        x & 0xf;               \
    })

#define HIGH4B(x)              \
    ({                         \
        typecheck(uint8_t, x); \
        x >> 4;                \
    })

#endif
