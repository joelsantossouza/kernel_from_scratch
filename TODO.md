Checkers
    ✅Implement code documentation checker
    ✅Implement basic assembly directives checker
    ✅Norminette Usage

Bootloader:
    ✅Set up project structure and Makefile dependencies
    ✅Set up the MBR (Master Boot Record)
    ✅Switch from real mode (16-bit) to protected mode (32-bit)
    ✅Load kernel into RAM

Video Graphics:
    ✅Implement the VGA text mode driver

Memory Management:
    Implement physical memory manager
    Implement virtual memory manager (Paging)

File Systems:
    Implement hard disk driver with IRQ Optimization
    Implement FAT16 interface (clusters read feature)
    Insert dynamic allocation on fat_table storage
    Implement VFS (Vitual File System) layer
    Add more file systems interfaces

-------\\

1. Integrate the autoconfig to remaining subsystems:
    fs, video_text

2. phy_partition should live under mbr, and be called instead mbr_part_table_entry

3. Instead of phy_fat_files, be fat_dir_entry

4. Split fat16_probe into:
    fat16_validate,
    fat16_metadata_init

5. VFS_TABLE_MAX is not being used, so it can be removed. Rename
    the g_vfs_table_hash to -> g_vfs_table_hash_mbr

6. Kconfig is buggy at first run time, appearing repetitively the config menu

7. Create all objects under obj/

8. Add copyright on header
