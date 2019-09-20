#ifndef __INCLUDE_IDT_H__
#define __INCLUDE_IDT_H__

#include <types.h>
#include <string.h>
#include <printk.h>
#include <common.h>

#define IRQ0 32  // 电脑系统计时器
#define IRQ1 33  // 键盘
#define IRQ2 34  // 与 IRQ9 相接，MPU-401 MD 使用
#define IRQ3 35  // 串口设备
#define IRQ4 36  // 串口设备
#define IRQ5 37  // 建议声卡使用
#define IRQ6 38  // 软驱传输控制使用
#define IRQ7 39  // 打印机传输控制使用
#define IRQ8 40  // 即时时钟
#define IRQ9 41  // 与 IRQ2 相接，可设定给其他硬件
#define IRQ10 42 // 建议网卡使用
#define IRQ11 43 // 建议 AGP 显卡使用
#define IRQ12 44 // 接 PS/2 鼠标，也可设定给其他硬件
#define IRQ13 45 // 协处理器使用
#define IRQ14 46 // IDE0 传输控制使用
#define IRQ15 47 // IDE1 传输控制使用

typedef struct idt_entry
{
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef struct pusha_regs
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} pusha_regs_t;

typedef struct trapframe
{
    uint32_t ds;
    pusha_regs_t pusha;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} trapframe_t;

void init_idt();

typedef void (*interrupt_handler_t)(trapframe_t *);
void register_interrupt_handler(uint8_t n, interrupt_handler_t hander);
void isr_handler(trapframe_t *regs);
void irq_handler(trapframe_t *regs);

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);

extern void idt_flush(uint32_t);

extern void isr0();  // 0 #DE 除 0 异常
extern void isr1();  // 1 #DB 调试异常
extern void isr2();  // 2 NMI
extern void isr3();  // 3 BP 断点异常
extern void isr4();  // 4 #OF 溢出
extern void isr5();  // 5 #BR 对数组的引用超出边界
extern void isr6();  // 6 #UD 无效或未定义的操作码
extern void isr7();  // 7 #NM 设备不可用(无数学协处理器)
extern void isr8();  // 8 #DF 双重故障(有错误代码)
extern void isr9();  // 9 协处理器跨段操作
extern void isr10(); // 10 #TS 无效TSS(有错误代码)
extern void isr11(); // 11 #NP 段不存在(有错误代码)
extern void isr12(); // 12 #SS 栈错误(有错误代码)
extern void isr13(); // 13 #GP 常规保护(有错误代码)
extern void isr14(); // 14 #PF 页故障(有错误代码)
extern void isr15(); // 15 CPU 保留
extern void isr16(); // 16 #MF 浮点处理单元错误
extern void isr17(); // 17 #AC 对齐检查
extern void isr18(); // 18 #MC 机器检查
extern void isr19(); // 19 #XM SIMD(单指令多数据)浮点异常

// 20 ~ 31 Intel 保留
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();  // 电脑系统计时器
extern void irq1();  // 键盘
extern void irq2();  // 与 IRQ9 相接，MPU-401 MD 使用
extern void irq3();  // 串口设备
extern void irq4();  // 串口设备
extern void irq5();  // 建议声卡使用
extern void irq6();  // 软驱传输控制使用
extern void irq7();  // 打印机传输控制使用
extern void irq8();  // 即时时钟
extern void irq9();  // 与 IRQ2 相接，可设定给其他硬件
extern void irq10(); // 建议网卡使用
extern void irq11(); // 建议 AGP 显卡使用
extern void irq12(); // 接 PS/2 鼠标，也可设定给其他硬件
extern void irq13(); // 协处理器使用
extern void irq14(); // IDE0 传输控制使用
extern void irq15(); // IDE1 传输控制使用

extern void isr255();
#endif