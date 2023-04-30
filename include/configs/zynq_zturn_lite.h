/*
 * (C) Copyright 2013 Xilinx, Inc.
 *
 * Configuration for Zynq Evaluation and Development Board - ZedBoard
 * See zynq-common.h for Zynq common configs
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ZYNQ_ZTURN_H
#define __CONFIG_ZYNQ_ZTURN_H

#define	PHYS_SDRAM_1_SIZE	(512 * 1024 * 1024)

/*
 * QSPI flash mapping
 *
 *   start       end      size     name
 * ---------------------------------------
 * 0x0000000 - 0x0100000  0x100000  boot.bin(1MB)
 * 0x0100000 - 0x0120000  0x020000  ubootenv(128KB)
 * 0x0120000 - 0x0720000  0x600000  bitstream(6MB)
 * 0x0720000 - 0x0D20000  0x600000  uImage(6MB)
 * 0x0D20000 - 0x0D40000  0x020000  devicetree(128KB)
 * 0x0D40000 - 0x1940000  0xC00000  ramdisk(12MB)
 * 0x1940000 - 0x1FFFFFF  0x6C0000  data(6.75MB)
 */
#define CONFIG_ENV_OFFSET		0x080000
#define CONFIG_ENV_SIZE			(128*1024)
#define CONFIG_ENV_SECT_SIZE 	(4*1024) // W25Q128 is 4KB sector type


#define CONFIG_IPADDR	192.168.1.55
#define CONFIG_SERVERIP	192.168.1.13

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"qbit_size=0x600000\0"  \
	"qbit_addr=0x0120004\0" \
	"qbitsize_addr=0x0120000\0" \
	"qboot_addr=0x000000\0" \
	"qbootenv_addr=0x100000\0" \
	"qbootenv_size=0x020000\0" \
	"qkernel_addr=0x720000\0" \
	"qdevtree_addr=0xD20000\0" \
	"qramdisk_addr=0xD40000\0" \
	"kernel_size=0x600000\0"	\
	"devicetree_size=0x020000\0"	\
	"ramdisk_size=0xC00000\0"	\
	"boot_size=0x100000\0"	\
	"ethaddr=00:0a:35:00:01:22\0"	\
	"kernel_image=uImage\0"	\
	"kernel_load_address=0x2080000\0" \
	"ramdisk_image=uramdisk.image.gz\0"	\
	"ramdisk_load_address=0x4000000\0"	\
	"devicetree_image=devicetree.dtb\0"	\
	"devicetree_load_address=0x2000000\0"	\
	"bitstream_image=system.bit.bin\0"	\
	"boot_image=BOOT.bin\0"	\
	"loadbit_addr=0x100000\0"	\
	"loadbootenv_addr=0x2000000\0" \
	"fdt_high=0x20000000\0"	\
	"initrd_high=0x20000000\0"	\
	"bootenv=uEnv.txt\0" \
	"loadbootenv=fatload mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"mmc_loadbit_fat=echo Loading bitstream from SD/MMC/eMMC to RAM.. && " \
		"get_bitstream_name && mmcinfo && " \
		"fatload mmc 0 ${loadbit_addr} ${bitstream_image} && " \
		"fpga loadb 0 ${loadbit_addr} ${filesize}\0" \
	"norboot=echo Copying Linux from NOR flash to RAM... && " \
		"cp.b 0xE2100000 ${kernel_load_address} ${kernel_size} && " \
		"cp.b 0xE2600000 ${devicetree_load_address} ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"cp.b 0xE2620000 ${ramdisk_load_address} ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"sdboot=if mmcinfo; then " \
			"run uenvboot; " \
			"get_bitstream_name && " \
			"echo - load ${bitname} to PL... && " \
			"fatload mmc 0 0x200000 ${bitname} && " \
			"fpga loadb 0 0x200000 ${filesize} && " \
			"echo Copying Linux from SD to RAM... && " \
			"fatload mmc 0 ${kernel_load_address} ${kernel_image} && " \
			"fatload mmc 0 ${devicetree_load_address} ${devicetree_image} && " \
			"fatload mmc 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"usbboot=if usb start; then " \
			"run uenvboot; " \
			"echo Copying Linux from USB to RAM... && " \
			"fatload usb 0 ${kernel_load_address} ${kernel_image} && " \
			"fatload usb 0 ${devicetree_load_address} ${devicetree_image} && " \
			"fatload usb 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"nandboot=echo Copying Linux from NAND flash to RAM... && " \
		"nand read ${kernel_load_address} 0x100000 ${kernel_size} && " \
		"nand read ${devicetree_load_address} 0x600000 ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"nand read ${ramdisk_load_address} 0x620000 ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
        "mmc_rootfs_args=setenv bootargs console=ttyPS0,115200 " \
                "root=/dev/mmcblk0p1 rw earlyprintk rootfstype=ext4 rootwait " \
                "ip=none devtmpfs.mount=1 ${optargs}\0" \
	"jtagboot=echo TFTPing Linux to RAM... && " \
		"tftpboot ${kernel_load_address} ${kernel_image} && " \
		"tftpboot ${devicetree_load_address} ${devicetree_image} && " \
		"tftpboot ${ramdisk_load_address} ${ramdisk_image} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_norboot=echo Copying Image from NOR flash to RAM... && " \
		"cp.b 0xE2100000 0x100000 ${boot_size} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_nandboot=echo Copying Image from NAND flash to RAM... && " \
		"nand read 0x100000 0x0 ${boot_size} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_qspiboot=echo Copying Image from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read 0x100000 0x0 ${boot_size} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_sdboot=echo Copying Image from SD to RAM... && " \
		"fatload mmc 0 0x100000 ${boot_image} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_jtagboot=echo TFTPing Image to RAM... && " \
		"tftpboot 0x100000 ${boot_image} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"qspiboot=echo Copying Linux from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"qspi_get_bitsize 0x0A0000 && " \
		"sf read ${loadbit_addr} 0x0A0004 ${bitsize} && " \
		"fpga loadb 0 ${loadbit_addr} ${bitsize} && " \
		"sf read ${kernel_load_address} ${qkernel_addr} ${kernel_size} && " \
		"sf read ${devicetree_load_address} ${qdevtree_addr} ${devicetree_size} && " \
		"run mmc_rootfs_args && " \
		"bootm ${kernel_load_address} - ${devicetree_load_address}\0" \
	"qspiupdate=echo Update qspi images from sd card... && " \
		"echo - Init mmc... && mmc rescan && " \
		"echo - Init qspi flash... && sf probe 0 0 0 && " \
		"echo - Write boot.bin... && " \
		"fatload mmc 0 0x200000 boot.bin && " \
		"sf erase ${qboot_addr} ${boot_size} && " \
		"sf erase ${qbootenv_addr} ${qbootenv_size} && " \
		"sf write 0x200000 0 ${filesize} && " \
		"get_bitstream_name && " \
		"echo - Write ${bitstream_image}... && " \
		"fatload mmc 0 0x200000 ${bitstream_image} && " \
		"sf erase 0x0A0000 0x460000 && " \
		"mw.l 0x100000 ${filesize} && " \
		"sf write 0x100000 0x0A0000 4 && " \
		"sf write 0x200000 0x0A0004 ${filesize} && " \
		"echo - Write uImage... && " \
		"fatload mmc 0 0x200000 uImage && " \
		"sf erase ${qkernel_addr} ${kernel_size} && " \
		"sf write 0x200000 ${qkernel_addr} ${filesize} && " \
		"echo - Write device tree... && " \
		"fatload mmc 0 0x200000 devicetree.dtb && " \
		"sf erase ${qdevtree_addr} ${devicetree_size} && " \
		"sf write 0x200000 ${qdevtree_addr} ${filesize} && " \
		"echo - Write Ramdisk... && " \
		"fatload mmc 0 0x200000 uramdisk.image.gz && " \
		"sf erase ${qramdisk_addr} ${ramdisk_size} && " \
		"sf write 0x200000 ${qramdisk_addr} ${filesize} && " \
		"echo - Done.\0"
#include <configs/zynq-common.h>

#endif /* __CONFIG_ZYNQ_ZED_H */
