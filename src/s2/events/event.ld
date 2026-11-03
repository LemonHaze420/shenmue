OUTPUT_FORMAT("elf32-shl")
OUTPUT_ARCH(sh)
ENTRY(__event_base)

SECTIONS
{
  . = __event_base;
  .text : { *(.text) }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss : { *(.bss) *(COMMON) }
  /DISCARD/ : { *(.comment) }
}
