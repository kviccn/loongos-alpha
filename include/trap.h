#ifndef __TRAPS_H__
#define __TRAPS_H__

#include <types.h>

// x86 trap_dispatcher and interrupt constants.

// Processor-defined:
#define T_DIVIDE 0  // divide error
#define T_DEBUG 1   // debug exception
#define T_NMI 2     // non-maskable interrupt
#define T_BRKPT 3   // breakpoint
#define T_OFLOW 4   // overflow
#define T_BOUND 5   // bounds check
#define T_ILLOP 6   // illegal opcode
#define T_DEVICE 7  // device not available
#define T_DBLFLT 8  // double fault
// #define T_COPROC      9      // reserved (not used since 486)
#define T_TSS 10    // invalid task switch segment
#define T_SEGNP 11  // segment not present
#define T_STACK 12  // stack exception
#define T_GPFLT 13  // general protection fault
#define T_PGFLT 14  // page fault
// #define T_RES        15      // reserved
#define T_FPERR 16    // floating point error
#define T_ALIGN 17    // aligment check
#define T_MCHK 18     // machine check
#define T_SIMDERR 19  // SIMD floating point error

// These are arbitrarily chosen, but with care not to overlap
// processor defined exceptions or interrupt vectors.
#define T_SYSCALL 64   // system call
#define T_DEFAULT 500  // catchall

#define T_IRQ0 32  // IRQ 0 corresponds to int T_IRQ

#define IRQ_TIMER 0
#define IRQ_KBD 1
#define IRQ_COM1 4
#define IRQ_IDE 14
#define IRQ_ERROR 19
#define IRQ_SPURIOUS 31

#define NINTERS 256

struct trapframe {
  // 由 pusha 压栈的寄存器 (trapasm.S)
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t oesp;  // useless & ignored
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  // 由 push 压栈的寄存器 (trapasm.S)
  uint16_t gs;
  uint16_t padding1;
  uint16_t fs;
  uint16_t padding2;
  uint16_t es;
  uint16_t padding3;
  uint16_t ds;
  uint16_t padding4;
  uint32_t trapno;

  // x86 硬件压入的寄存器
  // 错误码：如果是无错误码的中断则手动压栈一个 0，保持 trapframe 结构的一致
  uint32_t err;
  uint32_t eip;
  uint16_t cs;
  uint16_t padding5;
  uint32_t eflags;

  // 跨特权级时会压栈的寄存器
  uint32_t esp;
  uint16_t ss;
  uint16_t padding6;
};

// 中断处理函数指针
typedef void (*interrupt_handler_t)(struct trapframe *tf);

// 注册中断处理函数
void register_interrupt_handler(uint8_t trapno, interrupt_handler_t handler);

void tvinit(void);
void idtinit(void);
void picinit(void);
void eoi(uint8_t);

#endif
