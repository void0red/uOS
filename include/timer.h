#ifndef __INCLUDE_TIMER_H__
#define __INCLUDE_TIMER_H__

#include <types.h>
#include <idt.h>
#include <task.h>

void init_timer(uint32_t frequency);
#endif