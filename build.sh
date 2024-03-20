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

#	if [[ $1 == "k256host" ]]; then
#		cp mx6sabre_common_k256host.h ./include/configs/mx6sabre_common.h
#	elif [[ $1 == "k256monitor" ]]; then
#		cp mx6sabre_common_k256monitor.h ./include/configs/mx6sabre_common.h
#	elif [[ $1 == "ktc101max" ]]; then
#		cp mx6sabre_common_ktc101max.h ./include/configs/mx6sabre_common.h
#	elif [[ $1 == "tk200_ktc101" ]]; then
#		cp mx6sabre_common_tk200_ktc101.h ./include/configs/mx6sabre_common.h
#	elif [[ $1 == "k256zhongji" ]]; then
#		cp mx6sabre_common_k256zhongji.h ./include/configs/mx6sabre_common.h
#	else
#		cp mx6sabre_common.h ./include/configs/mx6sabre_common.h
#	fi


	echo "build M6708U-1GLI-T u-boot.imx";
	sed -i "s/MX6DL,DEFAULT_FDT_FILE=\"imx6dl-sabresd.dtb\",DDR_MB=512,SYS_USE_SPINOR /MX6DL,DEFAULT_FDT_FILE=\"imx6dl-sabresd.dtb\",DDR_MB=1024,SYS_USE_SPINOR /g" boards.cfg
	CONFIG=mx6dlsabresd_config
	OBJ=u-boot-M6708U-1GLI.imx
	build_uboot

fi

scp u-boot-M6708U-1GLI.imx root@169.254.1.218:/home/root
