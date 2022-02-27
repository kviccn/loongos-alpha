#include <console.h>
#include <mmu.h>
#include <trap.h>
#include <types.h>
#include <x86.h>

// 中断描述符表
struct gatedesc idt[NINTERS];

// 中断处理程序入口地址构成的数组 (在 vectors.S 中)
extern uint32_t vectors[];

void register_interrupt_handler(uint8_t trapno, interrupt_handler_t handler);
static void default_interrupt_handler(struct trapframe *tf);

// 初始化中断描述符表并设置默认中断处理函数
void tvinit(void) {
  for (int i = 0; i < NINTERS; i++) {
    SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], 0);
  }

  // 设置默认中断处理函数
  for (int i = 0; i < NINTERS; i++) {
    register_interrupt_handler(i, default_interrupt_handler);
  }
}

// load IDT
void idtinit(void) { lidt(idt, sizeof(idt)); }

// 中断处理函数指针数组
static interrupt_handler_t interrupt_handlers[NINTERS]
    __attribute__((aligned(4)));

// 中断处理入口
void trap_dispatcher(struct trapframe *tf) {
  if (interrupt_handlers[tf->trapno]) {
    interrupt_handlers[tf->trapno](tf);
  } else {
    cprintf("no handler for int %d\n", tf->trapno);
  }
}

// 中断处理函数注册器
void register_interrupt_handler(uint8_t trapno, interrupt_handler_t handler) {
  interrupt_handlers[trapno] = handler;
}

// 默认中断处理函数
void default_interrupt_handler(struct trapframe *tf) {
  cprintf("trapno: %d, eip: %x, err: %d\n", tf->trapno, tf->eip, tf->err);
}
