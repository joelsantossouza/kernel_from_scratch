SECTIONS
{
	. = 0x7E00;
	.text :
	{
		*(.boot_stage2*)
		*(.asm*)
		*(.text*)
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
