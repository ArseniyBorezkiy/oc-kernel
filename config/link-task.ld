OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 0x300000;
   .text : { *(.text) }
   .rodata : { *(.rodata) }
   .data : { *(.data) }
   .bss : { *(.bss)  }
 }
