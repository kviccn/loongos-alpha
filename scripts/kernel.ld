OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
  . = 0x100000;

  .text : {
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
