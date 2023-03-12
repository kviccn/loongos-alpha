#include <console.h>
#include <multiboot.h>
#include <timer.h>
#include <trap.h>
#include <vm.h>
#include <x86.h>

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

extern multiboot_info_t *global_mbi;

void print_kernel_info(void);
void print_gdt_info(void);
void print_mem_info(void);

void kernel_main(void) {
  seginit();

  cls();
  cprintf("Hello kernel!\n\n");

  print_gdt_info();
  cprintf("\n");
  print_kernel_info();
  cprintf("\n");
  print_mem_info();

  asm volatile("hlt");

  tvinit();
  idtinit();

  asm volatile("int $0x3");

  picinit();
  timerinit();

  sti();

  for (;;)
    ;
}

void print_kernel_info(void) {
  cprintf("kernel start: 0x%x\n", kernel_start);
  cprintf("kernel end: 0x%x\n", kernel_end);
  cprintf("kernel size: %dKB\n", (kernel_end - kernel_start + 1023) / 1024);
}

void print_gdt_info(void) {
  struct gdtdesc gdt_desc;
  sgdt(&gdt_desc);
  cprintf("gdt base: 0x%x, limit: 0x%x\n", gdt_desc.base, gdt_desc.lim);
}

void print_mem_info(void) {
  multiboot_info_t *mbi = global_mbi;

  cprintf("mem_lower = %dKB, mem_upper = %dKB\n", mbi->mem_lower,
          mbi->mem_upper);
  cprintf("mmap_addr = 0x%x, mmap_length = 0x%x\n", mbi->mmap_addr,
          mbi->mmap_length);

  for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
       (uint64_t)mmap < mbi->mmap_addr + mbi->mmap_length;
       mmap = (multiboot_memory_map_t *)((uint64_t)mmap + mmap->size +
                                         sizeof(mmap->size))) {
    cprintf(" base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n",
            (uint32_t)(mmap->addr >> 32), (uint32_t)(mmap->addr & 0xffffffff),
            (uint32_t)(mmap->len >> 32), (uint32_t)(mmap->len & 0xffffffff),
            (uint32_t)mmap->type);
  }
}
