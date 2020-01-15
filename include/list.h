#ifndef __INCLUDE_LIST_H__
#define __INCLUDE_LIST_H__

#include <types.h>

typedef struct list_head
{
    struct list_head *next, *prev;
} list_head_t;

#define LIST_HEAD_INIT(name) \
    {                        \
        &(name), &(name)     \
    }

#define LIST_HEAD(named) \
    list_head_t name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(list_head_t *entry)
{
    entry->next = entry;
    entry->prev = entry;
}

static inline void __list_add(list_head_t *node, list_head_t *prev, list_head_t *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

static inline void list_add(list_head_t *head, list_head_t *node)
{
    __list_add(node, head, head->next);
}

static inline void list_add_tail(list_head_t *head, list_head_t *node)
{
    __list_add(node, head->prev, head);
}

static inline void __list_del(list_head_t *prev, list_head_t *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void __list_del_entry(list_head_t *entry)
{
    __list_del(entry->prev, entry->next);
}

static inline void list_del(list_head_t *entry)
{
    __list_del_entry(entry);
    entry->next = 0;
    entry->prev = 0;
}

static inline int list_empty(list_head_t *head)
{
    return head->next == head;
}

/*
* Example:
* struct foo* p = container_of(&p->entry, struct foo, entry);
*
* @param ptr: point to the linkedlist struct, the member of one data struct.
* @param type: data type of the list element.
* @param member: member name of list_head_t field in the list element.
* @return: a pointer to the data struct containing the list_head_t structure.
*/
#ifndef container_of
#define container_of(ptr, type, member) \ 
        (type *)((char *)(ptr) - (char *)&((type *)0)->member)
#endif

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define __container_of(ptr, sample, member) \
    (void *)container_of((ptr), typeof(*(sample)), member)

/*
* Example:
* struct foo* iter;
* list_for_each_entry(iter, &bar->entry, entry) {
*     [modify iterator]
* }
*/
#define list_for_each_entry(pos, head, member)            \
    for (pos = __container_of((head)->next, pos, member); \
         &pos->member != (head);                          \
         pos = __container_of(pos->member.next, pos, member))

/*
* safe version of list_fo_each_entry
* it can safely delete a node
*/
#define list_for_each_entry_safe(pos, tmp, head, member)     \
    for (pos = __container_of((head)->next, pos, member),    \
        tmp = __container_of(pos->member.next, pos, member); \
         &pos->member != (head);                             \
         pos = tmp, tmp = __container_of(pos->member.next, pos, member))

#define list_for_each_entry_reverse(pos, head, member)    \
    for (pos = __container_of((head)->prev, pos, member); \
         &pos->member != (head);                          \
         pos = __container_of(pos->member.prev, pos, member))

#endif