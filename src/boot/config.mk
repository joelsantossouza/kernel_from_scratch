# Boot Stage2 config
BOOT_STAGE2_ADDR		:= 0x7E00
BOOT_STAGE2_SECTORS		:= 20
BOOT_STAGE2_SECTOR		:= 2
BOOT_STAGE2_CYLINDER	:= 0
BOOT_STAGE2_HEAD		:= 0

$(eval $(call register_config,BOOT_STAGE2_ADDR))
$(eval $(call register_config,BOOT_STAGE2_SECTORS))
$(eval $(call register_config,BOOT_STAGE2_SECTOR))
$(eval $(call register_config,BOOT_STAGE2_CYLINDER))
$(eval $(call register_config,BOOT_STAGE2_HEAD))

# MBR Partitions Config
MBR_PART0_BOOT_ATTR		:= 0
MBR_PART0_TYPECODE		:= 0x0E
MBR_PART0_LBA			:= 2048
MBR_PART0_SECTORS		:= 32768

MBR_PART1_BOOT_ATTR		:= 0
MBR_PART1_TYPECODE		:= 0
MBR_PART1_LBA			:= 0
MBR_PART1_SECTORS		:= 0

MBR_PART2_BOOT_ATTR		:= 0
MBR_PART2_TYPECODE		:= 0
MBR_PART2_LBA			:= 0
MBR_PART2_SECTORS		:= 0

MBR_PART3_BOOT_ATTR		:= 0
MBR_PART3_TYPECODE		:= 0
MBR_PART3_LBA			:= 0
MBR_PART3_SECTORS		:= 0

$(eval $(call register_config,MBR_PART0_BOOT_ATTR))
$(eval $(call register_config,MBR_PART0_TYPECODE))
$(eval $(call register_config,MBR_PART0_LBA))
$(eval $(call register_config,MBR_PART0_SECTORS))

$(eval $(call register_config,MBR_PART1_BOOT_ATTR))
$(eval $(call register_config,MBR_PART1_TYPECODE))
$(eval $(call register_config,MBR_PART1_LBA))
$(eval $(call register_config,MBR_PART1_SECTORS))

$(eval $(call register_config,MBR_PART2_BOOT_ATTR))
$(eval $(call register_config,MBR_PART2_TYPECODE))
$(eval $(call register_config,MBR_PART2_LBA))
$(eval $(call register_config,MBR_PART2_SECTORS))

$(eval $(call register_config,MBR_PART3_BOOT_ATTR))
$(eval $(call register_config,MBR_PART3_TYPECODE))
$(eval $(call register_config,MBR_PART3_LBA))
$(eval $(call register_config,MBR_PART3_SECTORS))
