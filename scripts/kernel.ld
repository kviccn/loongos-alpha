OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
  . = 0x100000;

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

  .data : {
    *(.data)
    *(.rodata)
  }

  .bss : {
    *(.bss)
  }

  .stab : {
    *(.stab)
  }

  .stabstr : {
    *(.stabstr)
  }

  /DISCARD/ : {
    *(.comment)
    *(.eh_frame)
  }
}
