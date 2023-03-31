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

int scanConfigFile(void)
{
	char buf[50]={0};
	char tempbuf[20]={0};
	const char *filename = "/opt/md5";
	int len_read,maxNum,bytes;

	int currentChar = 0;

	if (fs_set_blk_dev("mmc", "2:2", FS_TYPE_EXT)){
		printf("Identify partition errors!!!\n");
		return 1;
	}

	memset(tempbuf,0,sizeof(tempbuf));

	for (maxNum = 0; maxNum < 100; maxNum++)
	{
		len_read = ext4_read_file(filename, buf, currentChar, 20);
		if (len_read < 2) {
			printf("read config file success\n");
			break;
		}
		bytes=0;
		while (buf[bytes]!='\n') bytes++;
		currentChar=currentChar+bytes+1;
		memset(tempbuf,0,sizeof(tempbuf));
		memcpy(tempbuf,buf,bytes+1);
		printf("currentChar=%d,len_read=%d,www--->%s\n",currentChar,len_read,tempbuf);
	}
	
	return 0;
}
