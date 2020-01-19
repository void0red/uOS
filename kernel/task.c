#include <task.h>

task_t *current = NULL;
// task_t *ready = NULL;
// task_t *sleeping = NULL;

void init_sched()
{
    current = (task_t *)&bootstack;
    current->state = TASK_RUNNING;
    current->pid = current_pid++;
    current->stack = current;
    current->mm = NULL;
    INIT_LIST_HEAD(&current->entry);
}

void schedule()
{
    if (current)
    {
        task_t *next = list_entry(current->entry.next, task_t, entry);
        change_task(next);
    }
}

void change_task(task_t *next)
{
    if (current != next)
    {
        task_t *prev = current;
        current = next;
        switch_to(&(prev->context), &(current->context));
    }
}

int32_t kthread_spawn(fn func, void *args)
{
    task_t *newtask = (task_t *)kmalloc(STACK_SIZE);
    if (newtask == NULL)
        panic("kthread_spwan(): kmalloc error.");
    newtask->state = TASK_READY;
    newtask->stack = current;
    newtask->pid = current_pid++;
    newtask->mm = NULL;

    uint32_t *stack_top = (uint32_t *)((uint32_t)newtask + STACK_SIZE);
    *(--stack_top) = (uint32_t)args;
    *(--stack_top) = (uint32_t)kthread_exit;
    *(--stack_top) = (uint32_t)func;

    newtask->context.esp = (uint32_t)newtask + STACK_SIZE - sizeof(uint32_t) * 3;
    newtask->context.eflags = 0x200;

    list_add(&current->entry, &newtask->entry);
    return newtask->pid;
}

void kthread_exit()
{
    register uint32_t val __asm__("eax");
    printk("Thread exited with %d\n", val);
    list_del(&current->entry);
    kfree(current);
}