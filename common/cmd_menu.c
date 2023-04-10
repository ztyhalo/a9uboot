#include <common.h>
#include <command.h>

char boardConfigBuf[] =  "hc595=y \
spi1=y \
spi2=y \
usdhc1=y \
usdhc3=y \
i2c1=y \
i2c2=y \
i2c3=y \
uart1=y \
uart2=m \
uart3=m \
uart4=m \
uart5=y \
can1=y \
can2=y \
audio=y \
mipicsi=y \
usbotg=y \
usbhost=y \
screen=y ";

static void boardDevConfig(int index)
{
	char cmd_buf[512];

	switch(index) {
		case 0:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv wdconfig %s%s %s ",boardConfigBuf,"screenSize=21.5","system=yocto");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			writeConfigFile(cmd_buf, strlen(cmd_buf));
		break;
		case 1:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv wdconfig %s%s %s ",boardConfigBuf,"screenSize=21.5","system=yocto");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		case 2:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,bpp32");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=hdmi,1920*1080M@60,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv wdconfig %s%s %s ",boardConfigBuf,"screenSize=12-10.4","system=harmony");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			writeConfigFile();
			break;
		case 3:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv wdconfig %s%s %s ",boardConfigBuf,"screenSize=12-10.4","system=harmony");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		case 4:
			snprintf(cmd_buf, 127, "setenv pridis video=mxcfb0:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0); 
			snprintf(cmd_buf, 127, "setenv secdis video=mxcfb1:dev=ldb,if=RGB24");
			run_command(cmd_buf, 0);
			snprintf(cmd_buf, 512, "setenv wdconfig %s%s %s ",boardConfigBuf,"screenSize=10.4z","system=yocto");
			run_command(cmd_buf, 0);	
			run_command("saveenv", 0);
			writeConfigFile(cmd_buf, strlen(cmd_buf));
			break;
		default:
			break;
	}
}

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


#if 0
static void display_set(int index)
{
	char c;
	char cmd_buf[128];

start:
	printf("[1] HDMI 1920x1080\n");
	printf("[2] HDMI 1280x720\n");
	printf("[3] LVDS\n");
	printf("[4] LCD\n");
	printf("[0] clear %s setting\n", index==0? "primary displayer": "secondary displayer");
	printf("[q] quit\n");
	printf("Please select: ");
	c = getc();
	printf("%c\n", c);
	memset(cmd_buf, 0x0, 128);

	switch(c) {
		case '1':
			snprintf(cmd_buf, 127, "setenv %s video=mxcfb%d:dev=hdmi,1920x1080M@60,if=RGB24", index==0? "pridis": "secdis", index);
			run_command(cmd_buf, 0); 
			run_command("saveenv", 0); 
		break;
		case '2':
			snprintf(cmd_buf, 127, "setenv %s video=mxcfb%d:dev=hdmi,1280x720M@60,if=RGB24", index==0? "pridis": "secdis", index);
			run_command(cmd_buf, 0); 
			run_command("saveenv", 0); 
			break;
		case '3':
			snprintf(cmd_buf, 127, "setenv %s video=mxcfb%d:dev=ldb,if=RGB24", index==0? "pridis": "secdis", index);
			run_command(cmd_buf, 0); 
			run_command("saveenv", 0); 
			break;
		case '4':
			snprintf(cmd_buf, 127, "setenv %s video=mxcfb%d:dev=lcd,CLAA-WVGA,if=RGB24", index==0? "pridis": "secdis", index);
			run_command(cmd_buf, 0); 
			run_command("saveenv", 0); 
			break;
		case '0':
			snprintf(cmd_buf, 127, "setenv %s", index==0? "pridis": "secdis");
			run_command(cmd_buf, 0);
			run_command("saveenv", 0);
			break;
		case 'q':
			break;
		default:
			goto start;
			break;
	}
}

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


static void menu_shell(void)
{
	char c;
	while(1)
	{
		printf("[p] primary displayer setting\n");
		printf("[s] secondary displayer setting\n");
		printf("[d] env default set\n");
		printf("[r] reboot the machine\n");
		//printf("[x] system firmware update !\n");
		printf("[q] quit to uboot command line\n");
		printf("Please select: ");

		c = getc();
		printf("%c\n", c);

		switch(c)
		{
			case 'p':
				display_set(0);
				break;

			case 's':
				display_set(1);
				break;

			case 'd':
				run_command("env default -f -a", 0);
				run_command("saveenv", 0);
				break;
			case 'r':
				run_command("reset", 0);
				break;

			case 'x':
				img_update();
				break;
				
			case 'q':
				return;

			default:
				break;	
		}
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

