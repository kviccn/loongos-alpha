OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
  . = 0x100000;

  PROVIDE(kernel_start = .);

  .init.text : {
    *(.init.text)
  }

  .init.data : {
    *(.init.data)
  }

  . += 0xC0000000;
  
  .text : AT(ADDR(.text) - 0xC0000000) {
    *(.text)
  }

  PROVIDE(etext = .);

  . = ALIGN(0x1000);

  PROVIDE(data = .);

  .data : {
    *(.data)
    *(.rodata)
  }

  PROVIDE(edata = .);

  .bss : {
    *(.bss)
  }

  .stab : {
    *(.stab)
  }

  .stabstr : {
    *(.stabstr)
  }

	PROVIDE(end = . - 0xC0000000);
	PROVIDE(kernel_end = . - 0xC0000000);

  /DISCARD/ : {
    *(.comment)
    *(.eh_frame)
  }
}
