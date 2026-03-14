SECTIONS
{
	. = 0x7E00;
	.text :
	{
		*(.text*)
	}

	.asm :
	{
		*(.asm*)
	}

	.rodata :
	{
		*(.rodata*)
	}

	.data :
	{
		*(.data*)
	}

	.bss :
	{
		*(.COMMON*)
		*(.bss*)
	}

	/DISCARD/ :
	{
		*(.note*)
		*(.comment*)
		*(.eh_frame*)
		*(.gnu*)
	}
}
