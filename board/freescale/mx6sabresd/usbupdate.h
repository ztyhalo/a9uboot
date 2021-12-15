/*
 * (C) Copyright 3021 tltx Inc.
 *
 * Author: zhaotongyang <ztyswa@163.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __USB_UPDATE_H
#define __USB_UPDATE_H

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

/* Main function for usbupdate */
void imx6_usbupdate(void);

#endif /* __USB_UPDATE_H */
