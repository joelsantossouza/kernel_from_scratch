PHDRS
{
	text PT_LOAD FLAGS(5); /* RX Permissions */
	data PT_LOAD FLAGS(6); /* RW Permissions */
}

SECTIONS
{
	. = CONFIG_BOOT_STAGE2_ADDR;
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
}
