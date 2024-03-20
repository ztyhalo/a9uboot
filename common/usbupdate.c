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
#include <ext4fs.h>
//#include <mapmem.h>
#include <u-boot/md5.h>
#include <asm/io.h>
#include <fs.h>
#include "usbupdate.h"

static char * hndzz_md5_buf[MAXSCANLINE];
static char * opt_md5_buf[MAXSCANLINE];

extern long do_fat_read(const char *, void *, unsigned long, int);
extern int do_fat_fsload(cmd_tbl_t *, int, int, char * const []);
extern int do_ext4_load(cmd_tbl_t *, int , int, char * const []);

static int load_rescue_image(ulong);
static int ext4_mmc_load_rescue_image(ulong addr);
static int fat_mmc_load_rescue_image(ulong addr);

static int scan_opt_md5(char * md5argv[])
{
	char buf[LINELEN]={0};
	int ret,maxNum,bytes;
	int offset = 0;
	loff_t len_read;
	printf("wwwwww scan_opt_md5 1111111111\n");	


	if (fs_set_blk_dev("mmc", "2:2", FS_TYPE_EXT)){
		printf("Identify partition errors!!!\n");
		return 1;
	}

	//最多扫描10行
	for (maxNum = 0; maxNum < MAXSCANLINE; maxNum++)
	{
		//ret = ext4_read_file(MD5_FILE_NAME, buf, offset, LINELEN,&len_read);
		ret = ext4_read_file("/opt/md5", buf, offset, LINELEN);
		if (ret == 0) {
			printf("read /opt/md5 file success ret=%d\n",ret);
			break;
		}
		bytes=0;
		while (buf[bytes]!='\n'){
			bytes++;
			//printf("while wwwwwwww\n");
			if (bytes>=LINELEN) break;
		} 
		offset=offset+bytes+1;
		//memset(tempbuf,0,20);
		md5argv[maxNum]=malloc(LINELEN);
		memset(md5argv[maxNum],0,LINELEN);
		memcpy(md5argv[maxNum],buf,bytes+1);
		printf("scan_opt_md5 offset=%d,ret=%d,www--->%s\n",offset,ret,md5argv[maxNum]);
	}


	return 0;
}


static int scan_hndzz_md5(char * md5argv[],int flag)
{
	char buf[LINELEN]={0};
	int ret,maxNum,bytes;
	int offset = 0;
	loff_t len_read;
	printf("wwwwww scan_hndzz_md5 1111111111\n");	
	if (flag==USB_DEVICE)
	{
		if (fs_set_blk_dev("usb", "0:1", FS_TYPE_FAT)){
			printf("Identify partition errors!!!\n");
			return 1;
		}

		//最多扫描10行
		for (maxNum = 0; maxNum < MAXSCANLINE; maxNum++)
		{
			//ret = fat_read_file(MD5_FILE_NAME, buf, offset, LINELEN,&len_read);
			ret = fat_read_file(MD5_FILE_NAME, buf, offset, LINELEN);
			if (ret == 0) {
				printf("read config file success ret=%d\n",ret);
				break;
			}
			bytes=0;
			while (buf[bytes]!='\n'){
				bytes++;
				//printf("while wwwwwwww\n");
				if (bytes>=LINELEN) break;
			} 
			offset=offset+bytes+1;
			//memset(tempbuf,0,20);
			md5argv[maxNum]=malloc(LINELEN);
			memset(md5argv[maxNum],0,LINELEN);
			memcpy(md5argv[maxNum],buf,bytes+1);
			printf("scan_hndzz_md5 offset=%d,ret=%d,www--->%s\n",offset,ret,md5argv[maxNum]);
		}
	}
	else if (flag==MMC_EXT4_DEVICE)
	{
		if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_EXT)){
			printf("Identify partition errors!!!\n");
			return 1;
		}

		//最多扫描10行
		for (maxNum = 0; maxNum < MAXSCANLINE; maxNum++)
		{
			//ret = ext4_read_file(MD5_FILE_NAME, buf, offset, LINELEN,&len_read);
			ret = ext4_read_file(MD5_FILE_NAME, buf, offset, LINELEN);
			if (ret <= 0) {
				printf("ext4 read md5 file success ret=%d\n",ret);
				break;
			}

			bytes=0;
			while (buf[bytes]!='\n'){
				bytes++;
				//printf("while wwwwwwww\n");
				if (bytes>=LINELEN) break;
			} 
			offset=offset+bytes+1;
			//memset(tempbuf,0,20);
			md5argv[maxNum]=malloc(LINELEN);
			memset(md5argv[maxNum],0,LINELEN);
			memcpy(md5argv[maxNum],buf,bytes+1);
			printf("offset=%d,ret=%d,www--->%s\n",offset,ret,md5argv[maxNum]);
		}
	}
	else if (flag==MMC_FAT_DEVICE)
	{
		if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_FAT)){
			printf("Identify partition errors!!!\n");
			return 1;
		}

		//最多扫描10行
		for (maxNum = 0; maxNum < MAXSCANLINE; maxNum++)
		{
			ret = fat_read_file(MD5_FILE_NAME, buf, offset, LINELEN);
			if (ret <= 0) {
				printf("fat read md5 file success ret=%d\n",ret);
				break;
			}

			bytes=0;
			while (buf[bytes]!='\n'){
				bytes++;
				//printf("while wwwwwwww\n");
				if (bytes>=LINELEN) break;
			} 
			offset=offset+bytes+1;
			//memset(tempbuf,0,20);
			md5argv[maxNum]=malloc(LINELEN);
			memset(md5argv[maxNum],0,LINELEN);
			memcpy(md5argv[maxNum],buf,bytes+1);
			printf("offset=%d,ret=%d,www--->%s\n",offset,ret,md5argv[maxNum]);
		}
	}
	else
	{
		printf("wwwwww scan_hndzz_md5 no device\n");
	}

	return 0;
}



static int uboot_md5_check(int device_types)
{
	unsigned long addr, len;
	u8 output[16];
	char md5charbuf[LINELEN];
	void *buf;
	cmd_tbl_t *bcmd;
	int ret,size,i,md5state;

	char * const argv_usb_uboot[6] = { "fatload", "usb", "0:1", UBOOT_LOAD_ADDR_CHAR, UBOOT_PATH, NULL };
	char * const argv_mmc_ext4_uboot[6] = { "ext4load", "mmc", "0:1", UBOOT_LOAD_ADDR_CHAR, UBOOT_PATH, NULL };
	char * const argv_mmc_fat_uboot[6] = { "fatload", "mmc", "0:1", UBOOT_LOAD_ADDR_CHAR, UBOOT_PATH, NULL };
	//scan_hndzz_md5(hndzz_md5_buf);
	scan_opt_md5(opt_md5_buf);
	if (device_types==USB_DEVICE)
	{
		//scan_opt_md5(opt_md5_buf);
		scan_hndzz_md5(hndzz_md5_buf,USB_DEVICE);
		bcmd = find_cmd("fatload");
		if (!bcmd) {
			printf("www 111 Error - 'fatload' command not present.\n");
			return 1;
		}

		memset(md5charbuf,0,LINELEN);
		ret=do_fat_fsload(bcmd, 0, 5, argv_usb_uboot);
		if (ret != 0) {
			printf("www fatload uboot Error\n");
			return 1;
		}
	}
	else if (device_types==MMC_EXT4_DEVICE)
	{
		//scan_opt_md5(opt_md5_buf);
		scan_hndzz_md5(hndzz_md5_buf,MMC_EXT4_DEVICE);
		bcmd = find_cmd("ext4load");
		if (!bcmd) {
			printf("www 111 Error - 'ext4load' command not present.\n");
			return 1;
		}

		memset(md5charbuf,0,LINELEN);
		ret=do_ext4_load(bcmd, 0, 5, argv_mmc_ext4_uboot);
		if (ret != 0) {
			printf("www ext4load uboot Error\n");
			return 1;
		}
	}
	else if (device_types==MMC_FAT_DEVICE)
	{
		//scan_opt_md5(opt_md5_buf);
		scan_hndzz_md5(hndzz_md5_buf,MMC_FAT_DEVICE);
		bcmd = find_cmd("fatload");
		if (!bcmd) {
			printf("www 111 Error - 'fatload' command not present.\n");
			return 1;
		}

		memset(md5charbuf,0,LINELEN);
		ret=do_fat_fsload(bcmd, 0, 5, argv_mmc_ext4_uboot);
		if (ret != 0) {
			printf("www fatload uboot Error\n");
			return 1;
		}
	}
	else
	{
		printf("wwwwww do_md5kernel no device\n");
	}


	addr = UBOOT_LOAD_ADDR;	//getenv_hex("fileaddr", 0);
	len = getenv_hex("filesize", 0);
	printf("www addr=%x len=%d\n",addr,len);

	buf = map_sysmem(addr, len);
	md5_wd(buf, len, output, CHUNKSZ_MD5);
	unmap_sysmem(buf);

	printf("md5 for %08lx ... %08lx \n", addr, addr + len - 1);
	for (i = 0; i < 16; i++){
		//printf("%02x", output[i]);
		sprintf(md5charbuf+strlen(md5charbuf), "%02x", output[i]);
	}
	//printf("\n");
	printf("www md5charbuf=%s\n", md5charbuf);
	printf("www hndzz_md5_buf=%s\n", hndzz_md5_buf[1]);

	md5state=strncmp(hndzz_md5_buf[1],md5charbuf,strlen(md5charbuf));
	if (md5state!=0)
		goto error;

	printf("md5 check success state=%d\n",md5state);
	return 0;
error:
	printf("md5 check fail state=%d\n",md5state);
	return 1;
}

void imx6_usbupdate(void)
{
	cmd_tbl_t *bcmd;
//	char *rsargs;
//	char *tmp = NULL;
	char ka[16];
	char fs[16];
	char dtb[16];
	char profile[64];
	char * const argv[5] = { "bootz", ka,fs,dtb, NULL };

	memset(profile, 0x00, sizeof(profile));
	
	/* Check if rescue system is disabled... */
	if (getenv("norescue")) {
		printf(LOG_PREFIX "Rescue System disabled.\n");
		return;
	}

	/* Check if we have a USB storage device and load image */
	if(ext4_mmc_load_rescue_image(MD5_LOAD_ADDR))
	{
		if(fat_mmc_load_rescue_image(MD5_LOAD_ADDR))
		{
			if (load_rescue_image(MD5_LOAD_ADDR))
				return;
			else
			{
				sprintf(profile, "%s updatedev=usb", IMX_BOOTARGS);
			}
		}
		else
		{
			sprintf(profile, "%s updatedev=mmc", IMX_BOOTARGS);
		}
	}
	else
	{
		sprintf(profile, "%s updatedev=mmc", IMX_BOOTARGS);
	}

	bcmd = find_cmd("bootz");
	if (!bcmd)
		return;

	sprintf(ka, "%lx", (ulong)KERNEL_LOAD_ADDR);
	sprintf(fs, "%lx", (ulong)FS_LOAD_ADDR);
	sprintf(dtb, "%lx", (ulong)DTB_LOAD_ADDR);

	/* prepare our bootargs */


	// tmp = malloc(strlen(IMX_BOOTARGS)+1);
	// if (!tmp) {
	// 	printf(LOG_PREFIX "Memory allocation failed\n");
	// 	return;
	// }
	// sprintf(tmp, "%s", IMX_BOOTARGS);
	
//	setenv("bootargs", tmp);
	setenv("bootargs", profile);
//	free(tmp);
	printf(LOG_PREFIX "Starting update system (bootargs=%s)...\n", profile);
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

//usb fat32
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
//	char *tmp;
	char dev[7];
	char dev1[7];
	char addr_str[16];
	char * const argv[6] = { "fatload", "usb", dev, addr_str, nxri, NULL };
	char * const argv1[6] = { "ext4load", "mmc", dev1, addr_str, nxri, NULL };
	block_dev_desc_t *stor_dev = NULL;
	cmd_tbl_t *bcmd;
	int updatemark = 0;
	
	memset(fwdir, 0, sizeof(fwdir));
	/* Get name of firmware directory */
	//tmp = getenv("fw-dir");

	/* Copy it into fwdir */
	//strncpy(fwdir, tmp ? tmp : FW_DIR, sizeof(fwdir));
	//fwdir[sizeof(fwdir) - 1] = 0; /* Terminate string */

//	printf(LOG_PREFIX "Checking for firmware image directory  on USB"
//		" storage...\n");
	//usb_stop();
	if (usb_init() != 0)
		return 1;

	/* Check for storage device */
	if (usb_stor_scan(1) != 0) {
		usb_stop();
		return 1;
	}
	if (uboot_md5_check(USB_DEVICE))
	{
		printf("wwwww uboot_md5_check fail\n");
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
      
	for (partno = -1, i = 0; i < 6; i++) {
		if (get_partition_info(stor_dev, i, &info) == 0) {
			if (fat_register_device(stor_dev, i) == 0) {
				// Check if rescue image is present 
				USB_DEBUG("Looking for firmware directory '%s'"
					" on partition %d\n", fwdir, i);
				/*
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
				*/
				partno = i;
				break;
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


	/* Load the rescue image */
	bcmd = find_cmd("fatload");
	if (!bcmd) {
		printf(LOG_PREFIX "Error - 'fatload' command not present.\n");
		usb_stop();
		return 1;
	}

	//tmp = getenv("nx-rescue-image");
	sprintf(nxri, "%s", MD5_FILE_NAME);
	sprintf(dev, "%d:%d", devno, partno);
	//sprintf(dev, "%d", devno);
	sprintf(addr_str, "%lx", addr);

	USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
		dev, addr_str, nxri);
	printf("fat_fsload device='%s', addr='%s', file: %s\n",
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
		/*
		for(i= 0; i < size; i++)
		{
			printf("%c ", fwdir[i]);
		}
		printf("\n");
		*/
		//
		bcmd = find_cmd("ext4load");
		if (!bcmd) {
			printf(LOG_PREFIX "Error - 'ext4load' command not present.\n");
			usb_stop();
			return 1;
		}
		sprintf(nxri, "%s", MD5_FILE_NAME1);
		sprintf(dev1, "%d:%d", MD5_MMC_DEV, MD5_MMC_DEV_PART);
		sprintf(addr_str, "%lx", (ulong)MD5_LOAD_ADDR1);
			USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",
		dev1, addr_str, nxri);
		
		if (do_ext4_load(bcmd, 0, 5, argv1) != 0) {
			updatemark = 1;
		}
		else
		{
		
			size1 = getenv_hex("filesize", 0);
			if(size1 != size)
			{
				updatemark = 1;
			}
			else
			{
				//memcpy(md5, (void *)MD5_LOAD_ADDR1, size);
				printf("wwwwww strlen(hndzz_md5_buf[0])=%d\n",strlen(hndzz_md5_buf[0]));
				if(memcmp(hndzz_md5_buf[0], opt_md5_buf[0], strlen(hndzz_md5_buf[0])) != 0)
					updatemark = 1;
			}
		}
		//updatemark = 0;
		if(updatemark == 1)  //更新
		{
			USB_DEBUG("zty usb update!\n");
			sprintf(nxri, "%s", UPDATE_KERNEL);
			sprintf(addr_str, "%lx", (ulong)KERNEL_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				dev, addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto ERROR;
			}
			sprintf(nxri, "%s", UPDATE_ROOTFS);
			sprintf(addr_str, "%lx", (ulong)FS_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				dev, addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto ERROR;
			}
			sprintf(nxri, "%s", UPDATE_DTB);
			sprintf(addr_str, "%lx", (ulong)DTB_LOAD_ADDR);
			
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


static int ext4_mmc_load_rescue_image(ulong addr)
{
//	disk_partition_t info;
	int size ;
	int size1;
//	int devno;
//	int partno;
//	int i;
	char fwdir[64];
	char md5[64];
	char nxri[128];
//	char cmd[32];
//	char mmcblk[32];

	char dev[7];
	char dev1[7];
	char addr_str[16];
	char * const argv[6] = { "ext4load", "mmc", dev, addr_str, nxri, NULL };
	char * const argv1[6] = { "ext4load", "mmc", dev1, addr_str, nxri, NULL };
//	block_dev_desc_t *stor_dev = NULL;
	cmd_tbl_t *bcmd;
	int updatemark = 0;

	if (uboot_md5_check(MMC_EXT4_DEVICE))
	{
		printf("wwwww uboot_md5_check fail\n");
		return 1;
	}
    //init sd1
//	sprintf(cmd, "mmc dev %d:1", 0);
//	run_command(cmd, 0);

	/* Get name of firmware directory */
	//tmp = getenv("fw-dir");

	/* Copy it into fwdir */
	//strncpy(fwdir, tmp ? tmp : FW_DIR, sizeof(fwdir));
	//fwdir[sizeof(fwdir) - 1] = 0; /* Terminate string */

//	printf("Checking for firmware image directory  on mmc"
//		" storage...\n");


	/* Load the rescue image */
	bcmd = find_cmd("ext4load");
	if (!bcmd) {
		printf(LOG_PREFIX "Error - 'ext4load' command not present.\n");

		return 1;
	}

	//tmp = getenv("nx-rescue-image");
	sprintf(nxri, "%s", MD5_FILE_NAME);
	sprintf(dev, "%d:%d", 0, 1);
	//sprintf(dev, "%d", devno);
	sprintf(addr_str, "%lx", addr);

//	printf("fat_fsload device='%s', addr='%s', file: %s\n",
//		dev, addr_str, nxri);

	if (do_ext4_load(bcmd, 0, 5, argv) != 0) {
		printf("mmc no md5 file!\n");
		return 1;
	}
	else
	{
		size = getenv_hex("filesize", 0);
		USB_DEBUG("usb read veriosn ok %d\n",size);
		memcpy(fwdir, (void *)addr, size);
		/*
		for(i= 0; i < size; i++)
		{
			printf("%c ", fwdir[i]);
		}
		printf("\n");
		*/
		//
//		bcmd = find_cmd("ext4load");
//		if (!bcmd) {
//			printf(LOG_PREFIX "Error - 'ext4load' command not present.\n");
//			return 1;
//		}
		sprintf(nxri, "%s", MD5_FILE_NAME1);
		sprintf(dev1, "%d:%d", MD5_MMC_DEV, MD5_MMC_DEV_PART);
		sprintf(addr_str, "%lx", (ulong)MD5_LOAD_ADDR1);
			USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",
		dev1, addr_str, nxri);
		
		if (do_ext4_load(bcmd, 0, 5, argv1) != 0) {
			updatemark = 1;
		}
		else
		{
		
			size1 = getenv_hex("filesize", 0);
			if(size1 != size)
			{
				updatemark = 1;
			}
			else
			{
				//memcpy(md5, (void *)MD5_LOAD_ADDR1, size);
				//if(memcmp(md5, fwdir, size) != 0)
				//	updatemark = 1;

				printf("wwwwww hndzz_md5_buf[0]_len=%d opt_md5_buf[0]_len=%d\n",strlen(hndzz_md5_buf[0]),strlen(opt_md5_buf[0]));
				if(memcmp(hndzz_md5_buf[0], opt_md5_buf[0], strlen(hndzz_md5_buf[0])) != 0)
					updatemark = 1;

			}
		}
		//updatemark = 0;
		if(updatemark == 1)  //更新
		{
			USB_DEBUG("wwwwwwwwwwwwww mmc update!\n");
			return 0;
			sprintf(nxri, "%s", UPDATE_KERNEL);
			sprintf(addr_str, "%lx", (ulong)KERNEL_LOAD_ADDR);
			
			USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",
				"mmc", addr_str, nxri);
			if (do_ext4_load(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			sprintf(nxri, "%s", UPDATE_ROOTFS);
			sprintf(addr_str, "%lx", (ulong)FS_LOAD_ADDR);
			
			USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",
				"mmc", addr_str, nxri);
			if (do_ext4_load(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			sprintf(nxri, "%s", UPDATE_DTB);
			sprintf(addr_str, "%lx", (ulong)DTB_LOAD_ADDR);
			
			USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",
				"mmc", addr_str, nxri);
			if (do_ext4_load(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			
		}
		else
			goto MMCERROR;		
	}

	return 0;
MMCERROR:

	return 1;

}

static int fat_mmc_load_rescue_image(ulong addr)
{
//	disk_partition_t info;
	int size ;
	int size1;
//	int devno;
//	int partno;
//	int i;
	char fwdir[64];
	char md5[64];
	char nxri[128];
//	char cmd[32];
//	char mmcblk[32];

	char dev[7];
	char dev1[7];
	char addr_str[16];
	char * const argv[6] = { "fatload", "mmc", dev, addr_str, nxri, NULL };
	char * const argv1[6] = { "ext4load", "mmc", dev1, addr_str, nxri, NULL };
//	block_dev_desc_t *stor_dev = NULL;
	cmd_tbl_t *bcmd;
	int updatemark = 0;

	if (uboot_md5_check(MMC_FAT_DEVICE))
	{
		printf("wwwww uboot_md5_check fail\n");
		return 1;
	}
    //init sd1
//	sprintf(cmd, "mmc dev %d:1", 0);
//	run_command(cmd, 0);

	/* Get name of firmware directory */
	//tmp = getenv("fw-dir");

	/* Copy it into fwdir */
	//strncpy(fwdir, tmp ? tmp : FW_DIR, sizeof(fwdir));
	//fwdir[sizeof(fwdir) - 1] = 0; /* Terminate string */

//	printf("Checking for firmware image directory  on mmc"
//		" storage...\n");


	/* Load the rescue image */
	bcmd = find_cmd("fatload");
	if (!bcmd) {
		printf(LOG_PREFIX "Error - 'fatload' command not present.\n");

		return 1;
	}

	//tmp = getenv("nx-rescue-image");
	sprintf(nxri, "%s", MD5_FILE_NAME);
	sprintf(dev, "%d:%d", 0, 1);
	//sprintf(dev, "%d", devno);
	sprintf(addr_str, "%lx", addr);

//	printf("fat_fsload device='%s', addr='%s', file: %s\n",
//		dev, addr_str, nxri);

	if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
		printf("mmc no md5 file!\n");
		return 1;
	}
	else
	{
		size = getenv_hex("filesize", 0);
		USB_DEBUG("usb read veriosn ok %d\n",size);
		memcpy(fwdir, (void *)addr, size);
		/*
		for(i= 0; i < size; i++)
		{
			printf("%c ", fwdir[i]);
		}
		printf("\n");
		*/
		//
//		bcmd = find_cmd("ext4load");
//		if (!bcmd) {
//			printf(LOG_PREFIX "Error - 'ext4load' command not present.\n");
//			return 1;
//		}
		sprintf(nxri, "%s", MD5_FILE_NAME1);
		sprintf(dev1, "%d:%d", MD5_MMC_DEV, MD5_MMC_DEV_PART);
		sprintf(addr_str, "%lx", (ulong)MD5_LOAD_ADDR1);
		USB_DEBUG("ext4load device='%s', addr='%s', file: %s\n",dev1, addr_str, nxri);
		
		if (do_ext4_load(bcmd, 0, 5, argv1) != 0) {
			updatemark = 1;
		}
		else
		{
		
			size1 = getenv_hex("filesize", 0);
			if(size1 != size)
			{
				updatemark = 1;
			}
			else
			{
				//memcpy(md5, (void *)MD5_LOAD_ADDR1, size);
				//if(memcmp(md5, fwdir, size) != 0)
				//	updatemark = 1;

				printf("wwwwww strlen(hndzz_md5_buf[0])=%d\n",strlen(hndzz_md5_buf[0]));
				if(memcmp(hndzz_md5_buf[0], opt_md5_buf[0], strlen(hndzz_md5_buf[0])) != 0)
					updatemark = 1;
			}
		}
		//updatemark = 0;
		if(updatemark == 1)  //更新
		{
			USB_DEBUG("wwwwwwwwwwwwww mmc fat update!\n");
			return 0;
			sprintf(nxri, "%s", UPDATE_KERNEL);
			sprintf(addr_str, "%lx", (ulong)KERNEL_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				"mmc", addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			sprintf(nxri, "%s", UPDATE_ROOTFS);
			sprintf(addr_str, "%lx", (ulong)FS_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				"mmc", addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			sprintf(nxri, "%s", UPDATE_DTB);
			sprintf(addr_str, "%lx", (ulong)DTB_LOAD_ADDR);
			
			USB_DEBUG("fat_fsload device='%s', addr='%s', file: %s\n",
				"mmc", addr_str, nxri);
			if (do_fat_fsload(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			
		}
		else
			goto MMCERROR;		
	}

	return 0;
MMCERROR:

	return 1;

}

