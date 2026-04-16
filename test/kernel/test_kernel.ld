PHDRS
{
	text PT_LOAD FLAGS(5); /* RX Permissions */
	data PT_LOAD FLAGS(6); /* RW Permissions */
}

SECTIONS
{
	. = CONFIG_KERNEL_ADDR;
	.text :
	{
		*(.text*)
	} :text

	.asm :
	{
		*(.asm*)
	} :text

	.rodata :
	{
		*(.rodata*)
	}

	.data :
	{
		*(.data*)
	} :data

	.bss :
	{
		*(.COMMON*)
		*(.bss*)
	} :data

	.init_array :
	{
		__start_init_array = .;
		*(.init_array*)
		__stop_init_array = .;
	}
}
