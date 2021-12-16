/*
 * (C) Copyright 3021 tltx Inc.
 *
 * Author: zhaotongyang <ztyswa@163.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <image.h>
#include <usb.h>
#include <fat.h>

#include "usbupdate.h"

extern long do_fat_read(const char *, void *, unsigned long, int);
extern int do_fat_fsload(cmd_tbl_t *, int, int, char * const []);

static int load_rescue_image(ulong);

void imx6_usbupdate(void)
{
	cmd_tbl_t *bcmd;
	char *rsargs;
	char *tmp = NULL;
	char ka[16];
	char fs[16];
	char dtb[16];
	char * const argv[5] = { "bootz", ka,fs,dtb, NULL };

	/* Check if rescue system is disabled... */
	if (getenv("norescue")) {
		printf(LOG_PREFIX "Rescue System disabled.\n");
		return;
	}

	/* Check if we have a USB storage device and load image */
	if (load_rescue_image(MD5_LOAD_ADDR))
		return;

	bcmd = find_cmd("bootz");
	if (!bcmd)
		return;

	sprintf(ka, "%lx", (ulong)KERNEL_LOAD_ADDR);
	sprintf(fs, "%lx", (ulong)FS_LOAD_ADDR);
	sprintf(dtb, "%lx", (ulong)DTB_LOAD_ADDR);

	/* prepare our bootargs */


	tmp = malloc(strlen(IMX_BOOTARGS)+1);
	if (!tmp) {
		printf(LOG_PREFIX "Memory allocation failed\n");
		return;
	}
	sprintf(tmp, "%s", IMX_BOOTARGS);
	
	setenv("bootargs", tmp);
//	free(tmp);
	printf(LOG_PREFIX "Starting update system (bootargs=%s)...\n", tmp);
	do_bootz(bcmd, 0, 4, argv);
	
//	if (!rsargs)
//		rsargs = RS_BOOTARGS;
//	else {
//		tmp = malloc(strlen(rsargs+1));
//		if (!tmp) {
//			printf(LOG_PREFIX "Memory allocation failed\n");
//			return;
//		}
//		strcpy(tmp, rsargs);
//		rsargs = tmp;
//	}
//
//	setenv("bootargs", rsargs);
//
//	if (rsargs == tmp)
//		free(rsargs);
//
//	printf(LOG_PREFIX "Starting update system (bootargs=%s)...\n", rsargs);
//	do_bootm(bcmd, 0, 2, argv);
}

static int load_rescue_image(ulong addr)
{
	disk_partition_t info;
	int size ;
	int size1;
	int devno;
	int partno;
	int i;
	char fwdir[64];
	char md5[64];
	char nxri[128];
	char *tmp;
	char dev[7];
	char dev1[7];
	char addr_str[16];
	char * const argv[6] = { "fatload", "usb", dev, addr_str, nxri, NULL };
	char * const argv1[6] = { "fatload", "mmc", dev1, addr_str, nxri, NULL };
	block_dev_desc_t *stor_dev = NULL;
	cmd_tbl_t *bcmd;
	int updatemark = 0;
	

	/* Get name of firmware directory */
	//tmp = getenv("fw-dir");

	/* Copy it into fwdir */
	//strncpy(fwdir, tmp ? tmp : FW_DIR, sizeof(fwdir));
	//fwdir[sizeof(fwdir) - 1] = 0; /* Terminate string */

	printf(LOG_PREFIX "Checking for firmware image directory  on USB"
		" storage...\n");
	usb_stop();
	if (usb_init() != 0)
		return 1;

	/* Check for storage device */
	if (usb_stor_scan(1) != 0) {
		usb_stop();
		return 1;
	}

	/* Detect storage device */
	for (devno = 0; devno < USB_MAX_STOR_DEV; devno++) {
		stor_dev = usb_stor_get_dev(devno);
		if (stor_dev->type != DEV_TYPE_UNKNOWN)
			break;
	}
	if (!stor_dev || stor_dev->type == DEV_TYPE_UNKNOWN) {
		printf(LOG_PREFIX "No valid storage device found...\n");
		usb_stop();
		return 1;
	}

	/* Detect partition */
      /*
	for (partno = -1, i = 0; i < 6; i++) {
		if (get_partition_info(stor_dev, i, &info) == 0) {
			if (fat_register_device(stor_dev, i) == 0) {
				// Check if rescue image is present 
				USB_DEBUG("Looking for firmware directory '%s'"
					" on partition %d\n", fwdir, i);
				if (do_fat_read(fwdir, NULL, 0, LS_NO) == -1) {
					USB_DEBUG("No NX rescue image on "
						"partition %d.\n", i);
					partno = -2;
				} else {
					partno = i;
					USB_DEBUG("Partition %d contains "
						"firmware directory\n", partno);
					break;
				}
			}
		}
	}

	if (partno < 0) {
		switch (partno) {
		case -1:
			printf(LOG_PREFIX "Error: No valid (FAT) partition "
				"detected\n");
			break;
		case -2:
			printf(LOG_PREFIX "Error: No NX rescue image on FAT "
				"partition\n");
			break;
		default:
			printf(LOG_PREFIX "Error: Failed with code %d\n",
				partno);
		}
		usb_stop();
		return 1;
	}

	*/
	/* Load the rescue image */
	bcmd = find_cmd("fatload");
	if (!bcmd) {
		printf(LOG_PREFIX "Error - 'fatload' command not present.\n");
		usb_stop();
		return 1;
	}

	//tmp = getenv("nx-rescue-image");
	sprintf(nxri, "%s", MD5_FILE_NAME);
	//sprintf(dev, "%d:%d", devno, partno);
	sprintf(dev, "%d", devno);
	sprintf(addr_str, "%lx", addr);

	USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
		dev, addr_str, nxri);

	if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
		usb_stop();
		return 1;
	}
	else
	{
		size = getenv_hex("filesize", 0);
		USB_DEBUG("usb read veriosn ok %d\n",size);
		memcpy(fwdir, (void *)addr, size);
		for(i= 0; i < size; i++)
		{
			printf("%c ", fwdir[i]);
		}
		printf("\n");
		
		//
		bcmd = find_cmd("ext4load");
		if (!bcmd) {
			printf(LOG_PREFIX "Error - 'ext4load' command not present.\n");
			usb_stop();
			return 1;
		}
		sprintf(nxri, "%s", MD5_FILE_NAME1);
		sprintf(dev1, "%d:%d", MD5_MMC_DEV, MD5_MMC_DEV_PART);
		sprintf(addr_str, "%lx", MD5_LOAD_ADDR1);
			USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",
		dev1, addr_str, nxri);
		
		if (do_ext4_load(bcmd, 0, 5, argv1) != 0) {
			updatemark = 1;
		}
		else
		{
		
			size1 = getenv_hex("filesize", 0);
			if(size != size)
			{
				updatemark = 1;
			}
			else
			{
				memcpy(md5, (void *)MD5_LOAD_ADDR1, size);
				if(memcmp(md5, fwdir, size) != 0)
					updatemark = 1;
			}
		}
		updatemark = 0;
		if(updatemark == 1)  //更新
		{
			USB_DEBUG("zty usb update!\n");
			sprintf(nxri, "%s", UPDATE_KERNEL);
			sprintf(addr_str, "%lx", KERNEL_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				dev, addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto ERROR;
			}
			sprintf(nxri, "%s", UPDATE_ROOTFS);
			sprintf(addr_str, "%lx", FS_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				dev, addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto ERROR;
			}
			sprintf(nxri, "%s", UPDATE_DTB);
			sprintf(addr_str, "%lx", DTB_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				dev, addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto ERROR;
			}
			
		}
		else
			goto ERROR;

		
	}

	/* Stop USB */
	usb_stop();
	return 0;
ERROR:
	usb_stop();
	return 1;

}
