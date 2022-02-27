#include <console.h>
#include <timer.h>
#include <trap.h>
#include <types.h>
#include <x86.h>

/**
 * 在工作方式3下，方波的频率是输入时钟频率的N分之一，又因为计数器的输入时钟频率是
 * 1.193180MHz=1193180Hz，所以 1193180/N = 方波的频率（Hz）
 * 如果想让计数器每10ms（=100Hz）发出一个方波上升沿用以产生中断请求信号的话，那么
 * N = 1193180/100
 * */

#define TIMER_FREQ 1193180

#define FREQUENCY 100

void timer_interrupt_handler(struct trapframe *);

void timerinit(void) {
  register_interrupt_handler(T_IRQ0 + IRQ_TIMER, timer_interrupt_handler);

  uint32_t timer_frequency = TIMER_FREQ / FREQUENCY;

  // 设置 8253/8254 芯片工作在模式 3 下
  outb(0x43, 0x36);

  // 分别写入低字节和高字节
  outb(0x40, (uint8_t)(timer_frequency & 0xff));
  outb(0x40, (uint8_t)(timer_frequency >> 8) & 0xff);
}

void timer_interrupt_handler(struct trapframe *tf) {
  static uint32_t tick = 0;
  cprintf("tick: %d\n", tick++);
  eoi(tf->trapno);
}
