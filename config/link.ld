ENTRY(start)
SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
   . = 0x200000;
   .heap : { *(.heap) }
   . = 0x300000;
 }
