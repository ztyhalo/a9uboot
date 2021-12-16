#!/bin/bash
export ARCH=arm
export CROSS_COMPILE=/opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-

build_uboot(){
    make distclean
    rm $OBJ -f
    make $CONFIG
    make -j8 && cp u-boot.imx $OBJ
    cp $OBJ .. -f
}

if [[ $1 == "clean" ]]; then
     make distclean
     rm u-boot-M6708U-512LI.imx u-boot-M6708U-1GLI.imx u-boot-M6708Q-1GLI.imx -f
     exit 0 
else
	echo "build M6708U-512LI-T u-boot.imx"
	sed -i "s/MX6DL,DEFAULT_FDT_FILE=\"imx6dl-sabresd.dtb\",DDR_MB=1024,SYS_USE_SPINOR /MX6DL,DEFAULT_FDT_FILE=\"imx6dl-sabresd.dtb\",DDR_MB=512,SYS_USE_SPINOR /g" boards.cfg
	CONFIG=mx6dlsabresd_config
	OBJ=u-boot-M6708U-512LI.imx
	build_uboot

	echo "build M6708U-1GLI-T u-boot.imx";
	sed -i "s/MX6DL,DEFAULT_FDT_FILE=\"imx6dl-sabresd.dtb\",DDR_MB=512,SYS_USE_SPINOR /MX6DL,DEFAULT_FDT_FILE=\"imx6dl-sabresd.dtb\",DDR_MB=1024,SYS_USE_SPINOR /g" boards.cfg
	CONFIG=mx6dlsabresd_config
	OBJ=u-boot-M6708U-1GLI.imx
	build_uboot

	echo "build M6708Q-1GLI-T/M6708Q-1GLC-T u-boot.imx"
	CONFIG=mx6qsabresd_config
	OBJ=u-boot-M6708Q-1GLC.imx
	build_uboot

    mv ../u-boot-M6708U-512LI.imx ../u-boot-M6708U-1GLI.imx ../u-boot-M6708Q-1GLC.imx . -f
fi

