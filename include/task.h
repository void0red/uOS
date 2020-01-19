#ifndef __INCLUDE_TASK_H__
#define __INCLUDE_TASK_H__
#include <types.h>
#include <pmm.h>
#include <vmm.h>
#include <list.h>
#include <heap.h>

typedef enum
{
    TASK_UNINT,
    TASK_READY,
    TASK_SLEEPING,
    TASK_RUNNING,
    TASK_ZOMBIE
} task_state;

typedef struct _context
{
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
} context_t;

struct mm_struct
{
    uint32_t *pgd_dir;
};

typedef uint32_t pid_t;

typedef struct _task
{
    volatile task_state state;
    pid_t pid;
    void *stack;
    struct mm_struct *mm;
    context_t context;
    list_head_t entry;
} task_t;

static pid_t current_pid = 0;
extern uint32_t bootstack;

typedef int32_t (*fn)(void *);
int32_t kthread_spawn(fn func, void *args);
void kthread_exit();

void init_sched();
void schedule();
void change_task(task_t *next);
void switch_to(context_t *prev, context_t *next);

#endif