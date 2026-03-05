ENTRY(boot_stage2)
SECTIONS
{
	. = 0x7E00;
	.text :
	{
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

	.asm :
	{
		*(.asm)
	}
}
