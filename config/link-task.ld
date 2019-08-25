OUTPUT_FORMAT(elf32-i386)
ENTRY(main)
SECTIONS
 {
   . = 0x300000;
   .text : { *(.text) }
   .rodata : { *(.rodata) }
   .data : { *(.data) }
   .bss : { *(.bss)  }
 }
