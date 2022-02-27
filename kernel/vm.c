#include <mmu.h>
#include <x86.h>

struct segdesc gdt[NSEGS];

void seginit() {
  gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0);
  gdt[SEG_KDATA] = SEG(STA_W, 0);
  gdt[SEG_UCODE] = SEG(STA_X | STA_R, DPL_USER);
  gdt[SEG_UDATA] = SEG(STA_W, DPL_USER);

  lgdt(gdt, sizeof(gdt));

  asm volatile(
      "movw $0x10, %ax\n\t"
      "movw %ax, %ds\n\t"
      "movw %ax, %es\n\t"
      "movw %ax, %fs\n\t"
      "movw %ax, %gs\n\t"
      "movw %ax, %ss\n\t"
      "ljmp $0x8, $1f\n\t"
      "1:");
}
