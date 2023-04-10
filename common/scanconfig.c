#include <common.h>
#include <command.h>
#include <malloc.h>
#include <image.h>
#include <usb.h>
#include <fat.h>
#include <fs.h>
#include "usbupdate.h"
#include <part.h>
#include <config.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <ext4fs.h>
#include <linux/stat.h>
#include <sandboxfs.h>
#include <asm/io.h>
#include <fdtdec.h>
#include <hush.h>
#include <menu.h>


#include <u-boot/zlib.h>

#include <libfdt.h>
#include <fdt_support.h>
#include <asm/bootm.h>
#include <linux/compiler.h>

int scanConfigFile(void)
{
	char buf[50]={0};
	char tempbuf[20]={0};
	const char *filename = "/boardconfig";
	int len_read,maxNum,bytes;
	int currentChar = 0;
	//char *old_bootargs=getenv("bootargs");
	char bootargs_buf[1024]={0};
	if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_FAT)){
		printf("Identify partition errors!!!\n");
		return 1;
	}

	memset(tempbuf,0,sizeof(tempbuf));
	//最多扫描100行
	for (maxNum = 0; maxNum < 100; maxNum++)
	{
		len_read = fat_read_file(filename, buf, currentChar, 20);
		if (len_read < 2) {
			printf("read config file success\n");
			break;
		}
		bytes=0;
		while (buf[bytes]!='\n'){
			bytes++;
			//printf("while wwwwwwww\n");
			if (bytes>=20) break;
		} 
		currentChar=currentChar+bytes+1;
		memset(tempbuf,0,sizeof(tempbuf));
		memcpy(tempbuf,buf,bytes+1);
		sprintf(bootargs_buf,"%s %s",bootargs_buf,tempbuf);

		printf("currentChar=%d,len_read=%d,www--->%s\n",currentChar,len_read,tempbuf);
	}
	setenv("wdconfig",bootargs_buf);
	return 0;
}

int writeConfigFile(char *buf, unsigned long byteNum)
{
	long size;
	const char *filename = "boardconfig";
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int dev = 0;
	int part = 1;
	int i=0;
char writeBuf[512]={0};
memcpy(writeBuf,buf,byteNum);

for ( i = 0; i < byteNum; i++)
{
	if (writeBuf[i]==' ')
		writeBuf[i]='\n';
	
}

	part = get_device_and_partition("mmc", "0:1", &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;

	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use for fatwrite to mmc 0:1**\n");
		return 1;
	}


	size = file_fat_write(filename, writeBuf, byteNum);
	if (size == -1) {
		printf("\n** Unable write file**\n");
		return 1;
	}

	printf("%ld bytes written\n", size);
	return 0;
}
