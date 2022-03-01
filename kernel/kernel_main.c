#include <console.h>
#include <timer.h>
#include <trap.h>
#include <vm.h>
#include <x86.h>

void kernel_main(void) {
  seginit();

  cls();
  cprintf("Hello kernel!\n");

  struct gdtdesc gdt_desc;
  sgdt(&gdt_desc);
  cprintf("gdt base: %x, limit: %x\n", gdt_desc.base, gdt_desc.lim);

  tvinit();
  idtinit();

  asm volatile("int $0x3");

  picinit();
  timerinit();

  sti();

  for (;;)
    ;
}
