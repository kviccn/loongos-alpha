#include <mmu.h>
#include <types.h>

extern void kernel_main(void);

#define KERNEL_STACK_SIZE 0x4000

uint8_t kernel_stack[KERNEL_STACK_SIZE];

__attribute__((__section__(".init.data"), __aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES];

__attribute__((__section__(".init.data"), __aligned__(PGSIZE)))
pde_t pte_low[NPTENTRIES];

__attribute__((__section__(".init.data"), __aligned__(PGSIZE)))
pde_t pte_high[NPTENTRIES];

__attribute__((__section__(".init.text"))) void kernel_init(void) {
  entrypgdir[0] = (uint32_t)pte_low | PTE_P | PTE_W;
  entrypgdir[KERNBASE >> PDXSHIFT] = (uint32_t)pte_high | PTE_P | PTE_W;

  for (int i = 0; i < 1024; i++) {
    pte_low[i] = (i << 12) | PTE_P | PTE_W;
  }

  for (int i = 0; i < 1024; i++) {
    pte_high[i] = (i << 12) | PTE_P | PTE_W;
  }

  // 设置临时页表
  asm volatile("mov %0, %%cr3" : : "r"(entrypgdir));

  // 启用分页，将 cr0 寄存器的分页位置为 1
  asm volatile(
      "mov %cr0, %eax\n\t"
      "bts $31, %eax\n\t"
      "mov %eax, %cr0\n\t");

  // 切换内核栈
  uint32_t kernel_stack_top =
      ((uint32_t)kernel_stack + KERNEL_STACK_SIZE) & 0xFFFFFFF0;
  asm volatile(
      "mov %0, %%esp\n\t"
      "xor %%ebp, %%ebp"
      :
      : "r"(kernel_stack_top));

  kernel_main();
}
