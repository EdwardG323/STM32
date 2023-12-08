ENTRY(reset_handler);
MEMORY {
	ccram(xrw)	: ORIGIN = 0x10000000, LENGTH = 16k
	flash(rx)	: ORIGIN = 0x08000000, LENGTH = 512k
	ram(rwx)	: ORIGIN = 0x20000000, LENGTH = 64k
}

_estack    = ORIGIN(ram) + LENGTH(ram);

SECTIONS {
	.vectors   : { 
		KEEP(*(.vectors)) 
	} > flash

	.text      : { 
		*(.text*)

		.= ALIGN(4) 
	} > flash

	.rodata    : { 
		*(.rodata*)

		. = ALIGN(4); 
	} > flash

	.data : {
		_sdata = .;    /* .data section start */
		*(.first_data)
		*(.data SORT(.data.*))
		_edata = .;    /* .data section end */

		. =ALIGN(4)
	} > ram AT > flash
	_sidata = LOADADDR(.data);

	.bss : {
		_sbss = .;    /* .bss section start */
		*(.bss SORT(.bss.*) COMMON)
		_ebss = .;    /* .bss section end */

		. = ALIGN(4);
	} > ram

	_end = .;
}