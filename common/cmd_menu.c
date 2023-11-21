#include <common.h>
#include <command.h>
#if 0
extern int writeConfigFile(char *buf, unsigned long byteNum);
char boardConfigBuf[] =  "hc595=y \
spi1=y \
spi2=y \
usdhc1=y \
usdhc3=y \
i2c1=y \
i2c2=y \
i2c3=y \
uart1=y \
uart2=y \
uart3=y \
uart4=y \
uart5=y \
can1=y \
can2=y \
usbotg=y \
usbhost=y ";

enum hndz_board_num {
	K256HOST = 0,
	K256MONITOR = 1,
	KTC101MAX = 2,
	TK200KTC101 = 3,
	ZHONGJI = 4,
	REBOOTMACHINE = 5,
	QUITTOUBOOT = 6,
	BOARD_MAX_NUM = (5+2)
};

static char boardList[BOARD_MAX_NUM][50]={
	{" [0] 256 host controller board\n"},
	{" [1] 256 monitor board\n"},
	{" [2] ktc101max\n"},
	{" [3] tk200 ktc101 A inch screen\n"},
	{" [4] ktc256 zhongji\n"},
	{" [5] reboot the machine\n"},
	{" [6] quit to uboot command line\n"},
};

static char boardListInvert[BOARD_MAX_NUM][50]={
	{"\033[0m\033[7m [0] 256 host controller board\033[0m\n"},
	{"\033[0m\033[7m [1] 256 monitor board\033[0m\n"},
	{"\033[0m\033[7m [2] ktc101max\033[0m\n"},
	{"\033[0m\033[7m [3] tk200 ktc101 A inch screen\033[0m\n"},
	{"\033[0m\033[7m [4] ktc256 zhongji\033[0m\n"},
	{"\033[0m\033[7m [5] reboot the machine\033[0m\n"},
	{"\033[0m\033[7m [6] quit to uboot command line\033[0m\n"},
};

static void boardDevConfig(int index)
{
	char cmd_buf[512];

	switch(index) {
		case K256HOST:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv hndzboard %s%s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","system=yocto");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			snprintf(cmd_buf, 512, "%s%s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","system=yocto");
			writeConfigFile(cmd_buf, strlen(cmd_buf));
		break;
		case K256MONITOR:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv hndzboard %s%s %s %s %s ",boardConfigBuf,"mipicsi=y","audio=n","screenSize=21.5","system=yocto");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			snprintf(cmd_buf, 512, "%s%s %s %s %s ",boardConfigBuf,"mipicsi=y","audio=n","screenSize=21.5","system=yocto");
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		case KTC101MAX:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,bpp32");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=hdmi,1920*1080M@60,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv hndzboard %s%s %s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","screenSize=12-10.4","system=harmony");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			snprintf(cmd_buf, 512, "%s%s %s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","screenSize=12-10.4","system=harmony");
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		case TK200KTC101:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv hndzboard %s%s %s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","screenSize=12-10.4","system=harmony");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			snprintf(cmd_buf, 512, "%s%s %s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","screenSize=12-10.4","system=harmony");
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		case ZHONGJI:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv hndzboard %s%s %s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","screenSize=10.4z","system=yocto");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			snprintf(cmd_buf, 512, "%s%s %s %s %s ",boardConfigBuf,"mipicsi=n","audio=y","screenSize=10.4z","system=yocto");
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		default:
			break;
	}
	printf("hndz board num is %d\n",index);
	snprintf(cmd_buf, 512, "setenv mmcargs setenv bootargs console=${console},${baudrate} ${smp} root=${mmcroot} ${pridis} ${secdis} ${hndzboard} consoleblank=0");
	run_command(cmd_buf, 0);
	run_command("saveenv", 0);

}

static void menu_shell(void)
{
	char c;
	char cmd_buf[512];
	int previous=BOARD_MAX_NUM,current=0,i=0;

	printf("%s",(char*)&boardListInvert[K256HOST]);
	printf("%s",(char*)&boardList[K256MONITOR]);
	printf("%s",(char*)&boardList[KTC101MAX]);
	printf("%s",(char*)&boardList[TK200KTC101]);
	printf("%s",(char*)&boardList[ZHONGJI]);
	printf("%s",(char*)&boardList[REBOOTMACHINE]);
	printf("%s",(char*)&boardList[QUITTOUBOOT]);

	while(1)
	{
		c = getc();

		if (c=='A')
		{
			current--;
			if (current<0)
				current=0;
			
			if ((previous-current)>0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[A");
			} else if ((previous-current)<0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[B");
			}

			if (current==K256HOST){
				printf("%s",(char*)&boardListInvert[K256HOST]);
				printf("%s",(char*)&boardList[K256MONITOR]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==K256MONITOR){
				printf("%s",(char*)&boardListInvert[K256MONITOR]);
				printf("%s",(char*)&boardList[KTC101MAX]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==KTC101MAX){
				printf("%s",(char*)&boardListInvert[KTC101MAX]);
				printf("%s",(char*)&boardList[TK200KTC101]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==TK200KTC101){
				printf("%s",(char*)&boardListInvert[TK200KTC101]);
				printf("%s",(char*)&boardList[ZHONGJI]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==ZHONGJI){
				printf("%s",(char*)&boardListInvert[ZHONGJI]);
				printf("%s",(char*)&boardList[REBOOTMACHINE]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==REBOOTMACHINE){
				printf("%s",(char*)&boardListInvert[REBOOTMACHINE]);
				printf("%s",(char*)&boardList[QUITTOUBOOT]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==QUITTOUBOOT){
				printf("%s",(char*)&boardListInvert[QUITTOUBOOT]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[REBOOTMACHINE]);
			}
			previous=current;
		}else if (c=='B'){
			current++;
			if (current>(BOARD_MAX_NUM-1))
				current=(BOARD_MAX_NUM-1);
			if ((previous-current)>0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[A");
			} else if ((previous-current)<0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[B");
			}

			if (current==K256HOST)
				printf("%s",(char*)&boardListInvert[K256HOST]);
			else if (current==K256MONITOR){
				printf("%s",(char*)&boardListInvert[K256MONITOR]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[K256HOST]);
			}else if (current==KTC101MAX){
				printf("%s",(char*)&boardListInvert[KTC101MAX]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[K256MONITOR]);
			}else if (current==TK200KTC101){
				printf("%s",(char*)&boardListInvert[TK200KTC101]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[KTC101MAX]);
			}else if (current==ZHONGJI){
				printf("%s",(char*)&boardListInvert[ZHONGJI]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[TK200KTC101]);
			}else if (current==REBOOTMACHINE){
				printf("%s",(char*)&boardListInvert[REBOOTMACHINE]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[ZHONGJI]);
			}else if (current==QUITTOUBOOT){
				printf("%s",(char*)&boardListInvert[QUITTOUBOOT]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&boardList[REBOOTMACHINE]);
			}
			previous=current;
		}else if (c=='\x0d'){

			for ( i = 0; i < BOARD_MAX_NUM; i++) printf("\n");

			if (current==K256HOST) boardDevConfig(0);
			else if (current==K256MONITOR) boardDevConfig(1);
			else if (current==KTC101MAX) boardDevConfig(2);
			else if (current==TK200KTC101) boardDevConfig(3);
			else if (current==ZHONGJI) boardDevConfig(4);
			else if (current==REBOOTMACHINE) run_command("reset", 0);
			else if (current==QUITTOUBOOT) return;

			for ( i = 0; i < BOARD_MAX_NUM; i++) {
				if (current==i)
					printf("%s",(char*)(char*)&boardListInvert[i]);
				else
					printf("%s",(char*)(char*)&boardList[i]);
			}
			previous=BOARD_MAX_NUM;

		}else if (c=='u'){
			snprintf(cmd_buf, 127, "usb start");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "run mmcargs");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "fatload usb 0:1 0x12000000 /hndzz/file/zImage");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "fatload usb 0:1 0x18000000 /hndzz/file/imx6dl-sabresd.dtb");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "bootz 12000000 - 18000000");
			run_command(cmd_buf, 0); 
		}else if (c=='p'){
			snprintf(cmd_buf, 127, "usb start");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "run mmcargs");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "fatload usb 0:1 0x12000000 /hndzz/file/zImage");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "fatload usb 0:1 0x12c00000 /hndzz/file/uramdisk.img");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "fatload usb 0:1 0x18000000 /hndzz/file/imx6dl-sabresd.dtb");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv bootargs console=ttymxc0,115200 rdinit=/sbin/init");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "bootz 0x12000000 0x12c00000 0x18000000");
			run_command(cmd_buf, 0); 
		}else {
			//printf("wwwww error\n");
		}
	}
}
#endif


enum uboot_cmd_num {
	REBOOTMACHINE = 0,
	QUITTOUBOOT = 1,
	START_UPAN_KERNEL = 2,
	START_UPAN_SYS = 3,
	CMD_MAX_NUM
};

static char ubootCmd[CMD_MAX_NUM][50]={
	{" [0] reboot the machine\n"},
	{" [1] quit to uboot command line\n"},
	{" [2] start u pan kernel\n"},
	{" [3] start u pan sys\n"},
};

static char ubootCmdInvert[CMD_MAX_NUM][50]={
	{"\033[0m\033[7m [0] reboot the machine\033[0m\n"},
	{"\033[0m\033[7m [1] quit to uboot command line\033[0m\n"},
	{"\033[0m\033[7m [2] start u pan kernel\033[0m\n"},
	{"\033[0m\033[7m [3] start u pan sys\033[0m\n"},
};

static void menu_shell(void)
{
	char c;
	char cmd_buf[512];
	int previous=CMD_MAX_NUM,current=0,i=0;

	printf("%s",(char*)&ubootCmdInvert[REBOOTMACHINE]);
	printf("%s",(char*)&ubootCmd[QUITTOUBOOT]);
	printf("%s",(char*)&ubootCmd[START_UPAN_KERNEL]);
	printf("%s",(char*)&ubootCmd[START_UPAN_SYS]);

	while(1)
	{
		c = getc();

		if (c=='A')
		{
			current--;
			if (current<0)
				current=0;
			
			if ((previous-current)>0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[A");
			} else if ((previous-current)<0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[B");
			}

			if (current==REBOOTMACHINE){
				printf("%s",(char*)&ubootCmdInvert[REBOOTMACHINE]);
				printf("%s",(char*)&ubootCmd[QUITTOUBOOT]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==QUITTOUBOOT){
				printf("%s",(char*)&ubootCmdInvert[QUITTOUBOOT]);
				printf("%s",(char*)&ubootCmd[START_UPAN_KERNEL]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==START_UPAN_KERNEL){
				printf("%s",(char*)&ubootCmdInvert[START_UPAN_KERNEL]);
				printf("%s",(char*)&ubootCmd[START_UPAN_SYS]);
				printf("\033[A");
				printf("\033[A");
			}else if (current==START_UPAN_SYS){
				printf("%s",(char*)&ubootCmdInvert[START_UPAN_SYS]);
				printf("%s",(char*)&ubootCmd[START_UPAN_KERNEL]);
				printf("\033[A");
				printf("\033[A");
			}
			previous=current;
		}
		else if (c=='B'){
			current++;
			if (current>(CMD_MAX_NUM-1))
				current=(CMD_MAX_NUM-1);
			if ((previous-current)>0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[A");
			} else if ((previous-current)<0) {
				for (i = 0; i < abs(previous-current); i++)
				printf("\033[B");
			}

			if (current==REBOOTMACHINE)
				printf("%s",(char*)&ubootCmdInvert[REBOOTMACHINE]);
			else if (current==QUITTOUBOOT){
				printf("%s",(char*)&ubootCmdInvert[QUITTOUBOOT]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&ubootCmd[REBOOTMACHINE]);
			}else if (current==START_UPAN_KERNEL){
				printf("%s",(char*)&ubootCmdInvert[START_UPAN_KERNEL]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&ubootCmd[QUITTOUBOOT]);
			}else if (current==START_UPAN_SYS){
				printf("%s",(char*)&ubootCmdInvert[START_UPAN_SYS]);
				printf("\033[A");
				printf("\033[A");
				printf("%s",(char*)&ubootCmd[START_UPAN_KERNEL]);
			}
			previous=current;
		}
		else if (c=='\x0d'){

			for ( i = 0; i < CMD_MAX_NUM; i++) printf("\n");

			if (current==REBOOTMACHINE){
				run_command("reset", 0);
			}
			else if (current==QUITTOUBOOT){
				return;
			}
			else if (current==START_UPAN_KERNEL)
			{
				snprintf(cmd_buf, 127, "usb start");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "run mmcargs");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "fatload usb 0:1 0x12000000 /hndzz/file/zImage");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "fatload usb 0:1 0x18000000 /hndzz/file/imx6dl-sabresd.dtb");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "bootz 12000000 - 18000000");
				run_command(cmd_buf, 0); 
			}
			else if (current==START_UPAN_SYS)
			{
				snprintf(cmd_buf, 127, "usb start");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "run mmcargs");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "fatload usb 0:1 0x12000000 /hndzz/file/zImage");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "fatload usb 0:1 0x12c00000 /hndzz/file/uramdisk.img");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "fatload usb 0:1 0x18000000 /hndzz/file/imx6dl-sabresd.dtb");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "setenv bootargs console=ttymxc0,115200 rdinit=/sbin/init");
				run_command(cmd_buf, 0); 
				snprintf(cmd_buf, 127, "bootz 0x12000000 0x12c00000 0x18000000");
				run_command(cmd_buf, 0); 
			}


			for ( i = 0; i < CMD_MAX_NUM; i++) {
				if (current==i)
					printf("%s",(char*)(char*)&ubootCmdInvert[i]);
				else
					printf("%s",(char*)(char*)&ubootCmd[i]);
			}
			previous=CMD_MAX_NUM;

		}
		else {
			//printf("wwwww error\n");
		}
	}
}


#if 0
static void menu_shell(void)
{
	char c;
	while(1)
	{
		printf("[0] 256 host controller board\n");
		printf("[1] 256 monitor board\n");
		printf("[2] ktc101max\n");
		printf("[3] tk200 ktc101 A inch screen\n");
		printf("[4] ktc256 zhongji\n");
		printf("[r] reboot the machine\n");
		//printf("[x] system firmware update !\n");
		printf("[q] quit to uboot command line\n");
		printf("Please select: ");

		c = getc();
		printf("%c\n", c);

		switch(c)
		{
			case '0':
				boardDevConfig(0);
				break;
			case '1':
				boardDevConfig(1);
				break;
			case '2':
				boardDevConfig(2);
				break;
			case '3':
				boardDevConfig(3);
				break;
			case '4':
				boardDevConfig(4);
				break;
			case 'r':
				run_command("reset", 0);
				break;

			case 'x':
				//img_update();
				break;
				
			case 'q':
				return;

			default:
				break;	
		}
	}
}
#endif

#if 0
static void img_update(void)
{
	char c;

start:
	printf("[u] update u-boot\n");
	printf("[z] update zImage\n");
	printf("[d] update dtb\n");
	printf("[q] quit\n");
	printf("Please select: ");
	c = getc();
	printf("%c\n", c);

	switch(c) {
		case 'u':
			run_command("if tftp ${loadaddr} u-boot.imx; then mmc dev ${mmcdev};mmc write ${loadaddr} 0x2 0x400; fi", 0); 
			goto start;
			break;
		case 'z':
			run_command("if tftpboot ${loadaddr} ${bootfile}; then mmc dev ${mmcdev};mmc write ${loadaddr} 0x800 0x3000; fi", 0); 
			goto start;
			break;
		case 'd':
			run_command("if tftpboot ${loadaddr} ${fdt_file}; then mmc dev ${mmcdev};mmc write ${loadaddr} 0x5000 0x300; fi", 0); 
			goto start;
			break;
		case 'q':
			break;
		default:
			goto start;
			break;
	}
}
#endif

static int do_menu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	menu_shell();
	return 0;
}

U_BOOT_CMD(
	menu, 3, 0, do_menu,
	"display a menu, to select the items to do somethings",
	"    - display a menu, to select items to do eomething\n"
);

