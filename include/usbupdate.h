/*
 * (C) Copyright 3021 tltx Inc.
 *
 * Author: zhaotongyang <ztyswa@163.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __USB_UPDATE_H
#define __USB_UPDATE_H

#define CONFIG_USBUPDATE_DEBUG

/* Default prefix for output messages */
#define LOG_PREFIX	"zty:"

/* Extra debug macro */
#ifdef CONFIG_USBUPDATE_DEBUG
#define USB_DEBUG(fmt...) printf(LOG_PREFIX fmt)
#else
#define USB_DEBUG(fmt...)
#endif

/* Name of the directory holding firmware images */
#define FW_DIR		"nx-fw"
#define RESCUE_IMAGE	"nxrs.img"
#define LOAD_ADDR	0x400000
#define RS_BOOTARGS	"ramdisk_size=8192K"


#define IMX_BOOTARGS   "console=ttymxc0,115200 rdinit=/sbin/init"

#define UPDATE_ROOT_DIR "/hndzz/"

#define HNDZZ_MD5_NAME   UPDATE_ROOT_DIR"file/md5"
#define HNDZZ_MD5_LOAD_ADDR    0x18000000

#define OPT_MD5_NAME   "/etc/md5"
#define OPT_MD5_LOAD_ADDR    0x18000100
#define MD5_MMC_DEV       	2
#define MD5_MMC_DEV_PART       2

#define LINELEN 	80
#define MAXSCANLINE 	10
#define OPTMD5 	    1
#define HNDZZMD5 	2
#define USB_DEVICE		1
#define MMC_EXT4_DEVICE		2
#define MMC_FAT_DEVICE		3
#define UBOOT_LOAD_ADDR_CHAR    "0x12000000"
#define UBOOT_LOAD_ADDR         0x12000000
#define UBOOT_PATH		"/hndzz/file/u-boot-M6708U-1GLI.imx"


#define UPDATE_KERNEL   UPDATE_ROOT_DIR"zImage"
#define KERNEL_LOAD_ADDR 0x12000000
#define UPDATE_ROOTFS   UPDATE_ROOT_DIR"uramdisk.img"
#define FS_LOAD_ADDR    0x12C00000
#define UPDATE_DTB      UPDATE_ROOT_DIR"imx6dl-sabresd.dtb"
#define DTB_LOAD_ADDR    0x18000000

#define PF_FILE_PATH        UPDATE_ROOT_DIR"platform"

/* Main function for usbupdate */
void imx6_usbupdate(void);

#endif /* __USB_UPDATE_H */
