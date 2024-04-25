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

#define PHY_ID_BCM89881	0xae025032

static char * hndzz_md5_buf[MAXSCANLINE];
static char * opt_md5_buf[MAXSCANLINE];
u32 hndz_phy_id=0;
u32 hndz_tlv320_state=0;
u32 hndz_tca9535_start=0;
u32 hndz_mcp251x_state=0;
u32 hndz_platform=0xffffffff;

enum {
	HNDZ_MX = 0,
	HNDZ_KTC101,
	HNDZ_PINGMU,
	HNDZ_ZHONGJI,
	HNDZ_256_ZK_XS,
	HNDZ_NO_PF,
};

char *pf_name[HNDZ_NO_PF+1] = {"mx_platform",
							"ktc101_platform",
							"pingmu_platform",
							"zhongji_platform",
							"256_platform",
							"no_platform",
							}; 
//extern long do_fat_read(const char *, void *, unsigned long, int);
//extern int do_fat_fsload(cmd_tbl_t *, int, int, char * const []);
//extern int do_ext4_load(cmd_tbl_t *, int , int, char * const []);
extern int scan_spi_mcp2515(int bus);
void get_hndz_platfrom(void)
{
	hndz_mcp251x_state+=scan_spi_mcp2515(0);
	hndz_mcp251x_state+=scan_spi_mcp2515(1);

	if(hndz_phy_id == PHY_ID_BCM89881)  //256 sys
    {
        if(hndz_mcp251x_state) //mcp251x ok
        {
            hndz_platform =  HNDZ_256_ZK_XS;
			printf("zty HNDZ_256_ZK!\n");
        }
        else {
            hndz_platform =  HNDZ_256_ZK_XS;
			printf("zty HNDZ_256_XS!\n");
        }
		return;
    }
    else if(hndz_tca9535_start)  //256 sys
    {

        hndz_platform =  HNDZ_ZHONGJI;
        printf("zty HNDZ_ZHONGJI!\n");
		return;
    }
    else {
        if(hndz_mcp251x_state) //mcp251x ok;
        {
            hndz_platform =  HNDZ_MX;
			printf("zty HNDZ_MX!\n");
			return;
        }
        else {
            if(hndz_tlv320_state)  // tlv ok
            {
                hndz_platform =  HNDZ_KTC101;
				printf("zty HNDZ_KTC101!\n");
				return;
            }
            else {
                hndz_platform =  HNDZ_PINGMU;
				printf("zty HNDZ_PINGMU!\n");
            }
        }
	}
	//hndz_platform=HNDZ_NO_PF;

}
/*
*type : usbfat or mmcext4 or mmcfat
*flags : opt or hndzz
*return :0表示可以更新  1表示hndzz中md5不存在或者更新包与平台不对应
*/
static int scan_md5_platform(char * md5argv[],int flag,int type,int platform)
{
	char buf[LINELEN]={0};
	int ret,maxNum,bytes;
	int offset = 0;
	loff_t len_read;
	char* path=NULL;
	int (*read_file)(const char *, void *, int, int)=NULL;
	if (flag==OPTMD5)
	{
		if (fs_set_blk_dev("mmc", "2:2", FS_TYPE_EXT)){
			printf("Identify partition errors!!!\n");
			return 1;
		}
		read_file=ext4_read_file;
		path=OPT_MD5_NAME;
	}
	else if (flag==HNDZZMD5)
	{
		if (type==USB_DEVICE)
		{
			if (fs_set_blk_dev("usb", "0:1", FS_TYPE_FAT)){
				printf("Identify partition errors!!!\n");
				return 1;
			}
			read_file=fat_read_file;
		}
		else if (type==MMC_EXT4_DEVICE)
		{
			if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_EXT)){
				printf("Identify partition errors!!!\n");
				return 1;
			}
			read_file=ext4_read_file;
		}
		else if (type==MMC_FAT_DEVICE)
		{
			if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_FAT)){
				printf("Identify partition errors!!!\n");
				return 1;
			}
			read_file=fat_read_file;
		}
		else
		{
		}
		path=HNDZZ_MD5_NAME;

	}
	else
	{
		printf("wwwwww scan_hndzz_md5 no device flag=%d\n",flag);
	}

	if (platform)
	{
		ret = read_file(PF_FILE_PATH, buf, offset, LINELEN);
		if (ret == 0)
		{
			printf("read platform file success ret=%d\n",ret);
		}
		else if (ret < 0) {
			printf("read platform file fail ret=%d\n",ret);
			return 1;
		}

		//检测是否hndzz包的版型是否在支持版型中如果不再则更新，
		//如果在需要hndzzz中的版型与当前版型是否对应相同则更新
		if(!memcmp(buf, pf_name[HNDZ_MX], strlen(pf_name[HNDZ_MX])) 
			| !memcmp(buf, pf_name[HNDZ_KTC101], strlen(pf_name[HNDZ_KTC101]))
			| !memcmp(buf, pf_name[HNDZ_PINGMU], strlen(pf_name[HNDZ_PINGMU]))
			| !memcmp(buf, pf_name[HNDZ_ZHONGJI], strlen(pf_name[HNDZ_ZHONGJI]))
			| !memcmp(buf, pf_name[HNDZ_256_ZK_XS], strlen(pf_name[HNDZ_256_ZK_XS]))
		){
			if (memcmp(buf, pf_name[hndz_platform], strlen(pf_name[hndz_platform])))
			{
				printf("platform error!!!!!!!!\n");
				printf("ret=%d,platform-->%s hndzz_platform-->%s",ret,pf_name[hndz_platform],buf);
				return 1;
			}
		}
			
		printf("ret=%d,platform-->%s hndzz_platform-->%s",ret,pf_name[hndz_platform],buf);
	}
	else
	{
		//最多扫描10行
		for (maxNum = 0; maxNum < MAXSCANLINE; maxNum++)
		{
			ret = read_file(path, buf, offset, LINELEN);
			if (ret == 0)
			{
				printf("read md5 file success ret=%d\n",ret);
				break;
			}
			else if (ret < 0) {
				printf("read md5 file fail ret=%d\n",ret);
				return 1;
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
			//printf("offset=%d,ret=%d,www--->%s",offset,ret,md5argv[maxNum]);
		}
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

	scan_md5_platform(opt_md5_buf,OPTMD5,MMC_EXT4_DEVICE,0);
	if (device_types==USB_DEVICE)
	{
		ret = scan_md5_platform(hndzz_md5_buf,HNDZZMD5,USB_DEVICE,0);
		if (ret != 0) {
			printf("www scan md5 file\n");
			return 1;
		}
		
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
		ret = scan_md5_platform(hndzz_md5_buf,HNDZZMD5,MMC_EXT4_DEVICE,0);
		if (ret != 0) {
			printf("www scan md5 file\n");
			return 1;
		}

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
		ret = scan_md5_platform(hndzz_md5_buf,HNDZZMD5,MMC_FAT_DEVICE,0);
		if (ret != 0) {
			printf("www scan md5 file\n");
			return 1;
		}
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
	//printf("www addr=%x len=%d\n",addr,len);

	buf = map_sysmem(addr, len);
	md5_wd(buf, len, output, CHUNKSZ_MD5);
	unmap_sysmem(buf);

	//printf("md5 for %08lx ... %08lx \n", addr, addr + len - 1);
	for (i = 0; i < 16; i++){
		//printf("%02x", output[i]);
		sprintf(md5charbuf+strlen(md5charbuf), "%02x", output[i]);
	}
	//printf("\n");
	//printf("www md5charbuf=%s\n", md5charbuf);
	//printf("www hndzz_md5_buf=%s\n", hndzz_md5_buf[1]);
	if (hndzz_md5_buf[1]==NULL)
	{
		printf("www no find uboot md5 value\n");
		return 0;
	}

	md5state=strncmp(hndzz_md5_buf[1],md5charbuf,strlen(md5charbuf));
	if (md5state!=0)
		goto error;

	printf("md5 check success state=%d\n",md5state);
	return 0;
error:
	printf("md5 check fail state=%d\n",md5state);
	return 1;
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
		printf("wwwww uboot_md5 check fail\n");
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
	sprintf(nxri, "%s", HNDZZ_MD5_NAME);
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
		sprintf(nxri, "%s", OPT_MD5_NAME);
		sprintf(dev1, "%d:%d", MD5_MMC_DEV, MD5_MMC_DEV_PART);
		sprintf(addr_str, "%lx", (ulong)OPT_MD5_LOAD_ADDR);
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
				printf("wwwwww hndzz_md5_buf[0]-->%s",hndzz_md5_buf[0]);
				printf("wwwwww hndzz_md5_buf[1]-->%s",hndzz_md5_buf[1]);
				printf("wwwwww opt_md5_buf[0]-->%s",opt_md5_buf[0]);
				printf("wwwwww opt_md5_buf[1]-->%s",opt_md5_buf[1]);
				if((memcmp(hndzz_md5_buf[0], opt_md5_buf[0], strlen(hndzz_md5_buf[0])) != 0)
					||(memcmp(hndzz_md5_buf[1], opt_md5_buf[1], strlen(hndzz_md5_buf[1])) != 0))
					updatemark = 1;
			}
		}
		//updatemark = 0;
		if(updatemark == 1)  //更新
		{
			get_hndz_platfrom();
			if (hndz_platform>HNDZ_NO_PF)
			{
				printf("wwwww hndzz board error\n");
				return 1;
			}
			if (scan_md5_platform(NULL,HNDZZMD5,USB_DEVICE,1))
			{
				printf("wwwww hndzz platform error\n");
				return 1;
			}
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

static int mmc_load_rescue_image(ulong addr, int type)
{

	int size ;
	int size1;
	int (*do_mmc_load)(cmd_tbl_t *cmdtp, int flag, int argc,char *const argv[])=NULL;
	char fwdir[64];
	//char md5[64];
	char nxri[128];
	char *readcmd=(type == MMC_FAT_DEVICE) ? "fatload" : "ext4load";
	char dev[7];
	char dev1[7];
	char addr_str[16];
	char * const argv[6] = { readcmd, "mmc", dev, addr_str, nxri, NULL };
	char * const argv1[6] = { "ext4load", "mmc", dev1, addr_str, nxri, NULL };

	cmd_tbl_t *bcmd;
	int updatemark = 0;

	if (uboot_md5_check(type))
	{
		printf("wwwww uboot_md5 check fail\n");
		return 1;
	}

	if (type==MMC_EXT4_DEVICE)
		do_mmc_load=do_ext4_load;
	else
		do_mmc_load=do_fat_fsload;	

	/* Load the rescue image */
	bcmd = find_cmd(readcmd);
	if (!bcmd) {
		printf(LOG_PREFIX "Error - '%s' command not present.\n",readcmd);

		return 1;
	}

	//tmp = getenv("nx-rescue-image");
	sprintf(nxri, "%s", HNDZZ_MD5_NAME);
	sprintf(dev, "%d:%d", 0, 1);
	//sprintf(dev, "%d", devno);
	sprintf(addr_str, "%lx", addr);
	if (do_mmc_load(bcmd, 0, 5, argv) != 0) {
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
		bcmd = find_cmd("ext4load");
		if (!bcmd) {
			printf(LOG_PREFIX "Error - 'ext4load' command not present.\n");
			return 1;
		}
		sprintf(nxri, "%s", OPT_MD5_NAME);
		sprintf(dev1, "%d:%d", MD5_MMC_DEV, MD5_MMC_DEV_PART);
		sprintf(addr_str, "%lx", (ulong)OPT_MD5_LOAD_ADDR);
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
				printf("wwwwww hndzz_md5_buf[0]-->%s",hndzz_md5_buf[0]);
				printf("wwwwww hndzz_md5_buf[1]-->%s",hndzz_md5_buf[1]);
				printf("wwwwww opt_md5_buf[0]-->%s",opt_md5_buf[0]);
				printf("wwwwww opt_md5_buf[1]-->%s",opt_md5_buf[1]);
				if((memcmp(hndzz_md5_buf[0], opt_md5_buf[0], strlen(hndzz_md5_buf[0])) != 0)
					||(memcmp(hndzz_md5_buf[1], opt_md5_buf[1], strlen(hndzz_md5_buf[1])) != 0))
					updatemark = 1;

			}
		}
		//updatemark = 0;
		if(updatemark == 1)  //更新
		{
			get_hndz_platfrom();
			if (hndz_platform>HNDZ_NO_PF)
			{
				printf("wwwww hndzz board error\n");
				return 1;
			}
			if (scan_md5_platform(NULL,HNDZZMD5,type,1))
			{
				printf("wwwww hndzz platform error\n");
				return 1;
			}
			USB_DEBUG("wwwwwwwwwwwwww mmc %s update!\n",readcmd);
			sprintf(nxri, "%s", UPDATE_KERNEL);
			sprintf(addr_str, "%lx", (ulong)KERNEL_LOAD_ADDR);
			
			USB_DEBUG("%s device='%s', addr='%s', file: %s\n",readcmd,
				"mmc", addr_str, nxri);
			if (do_mmc_load(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			sprintf(nxri, "%s", UPDATE_ROOTFS);
			sprintf(addr_str, "%lx", (ulong)FS_LOAD_ADDR);
			
			USB_DEBUG("%s device='%s', addr='%s', file: %s\n",readcmd,
				"mmc", addr_str, nxri);
			if (do_mmc_load(bcmd, 0, 5, argv) != 0) {
				goto MMCERROR;
			}
			sprintf(nxri, "%s", UPDATE_DTB);
			sprintf(addr_str, "%lx", (ulong)DTB_LOAD_ADDR);
			
			USB_DEBUG("%s device='%s', addr='%s', file: %s\n",readcmd,
				"mmc", addr_str, nxri);
			if (do_mmc_load(bcmd, 0, 5, argv) != 0) {
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

	printf("\n\nstart scan mmc ext4 part\n");
	/* Check if we have a USB storage device and load image */
	if(mmc_load_rescue_image(HNDZZ_MD5_LOAD_ADDR,MMC_EXT4_DEVICE))
	{
		printf("\n\nstart scan mmc fat part\n");
		if(mmc_load_rescue_image(HNDZZ_MD5_LOAD_ADDR,MMC_FAT_DEVICE))
		{
			printf("\n\nstart scan usb fat part\n");
			if (load_rescue_image(HNDZZ_MD5_LOAD_ADDR))
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
	setenv("bootargs", profile);
	printf(LOG_PREFIX "Starting update system (bootargs=%s)...\n", profile);
	do_bootz(bcmd, 0, 4, argv);
	
}


