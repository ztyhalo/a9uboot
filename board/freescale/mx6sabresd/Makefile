#
# Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
#
# (C) Copyright 2011 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y  := mx6sabresd.o

ifneq ($(wildcard board/freescale/mx6sabresd/aw_encrypt/ds2460.c),)
obj-y	+= aw_encrypt/ds2460.o
obj-y	+= aw_encrypt/my_serial.o
obj-y   += aw_encrypt/anywhere_uart.o
obj-y   += aw_encrypt/ds2460_encrypt.o

else
obj-y   += aw_encrypt/ds2460.zy
obj-y   += aw_encrypt/my_serial.zy
obj-y   += aw_encrypt/anywhere_uart.zy
obj-y   += aw_encrypt/ds2460_encrypt.zy
endif


extra-$(CONFIG_USE_PLUGIN) :=  plugin.bin
$(obj)/plugin.bin: $(obj)/plugin.o
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@
