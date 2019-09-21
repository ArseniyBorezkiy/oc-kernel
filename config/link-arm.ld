SECTIONS
{
	 . = 0x0; /* ROM */
	. = 0x20000000; /* RAM */
	.data : { *(.data) }
	.bss : { *(.bss) }
	. = ALIGN(8);
	. = . + 0x1000; /* stack 4кБ */
	stack_top = .;
}
