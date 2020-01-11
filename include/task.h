#ifndef __INCLUDE_TASK_H__
#define __INCLUDE_TASK_H__
#include <types.h>
#include <pmm.h>
#include <vmm.h>
typedef enum
{
    TASK_UNINT,
    TASK_SLEEPING,
    TASK_RUNNING,
    TASK_ZOMBIE
} task_state;

struct context
{
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
};

struct mm_struct
{
    uint32_t *pgd_dir;
};

typedef uint32_t pid_t;

struct task_struct
{
    volatile task_state state;
    pid_t pid;
    void *stack;
    struct mm_struct *mm;
    struct context context;
    struct task_struct *next;
};

extern pid_t current;
#endif