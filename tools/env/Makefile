#
# (C) Copyright 2002-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

# fw_printenv is supposed to run on the target system, which means it should be
# built with cross tools. Although it may look weird, we only replace "HOSTCC"
# with "CC" here for the maximum code reuse of scripts/Makefile.host.
HOSTCC = $(CC)
HOSTCC +="-mfloat-abi=hard"
# Compile for a hosted environment on the target
HOST_EXTRACFLAGS  = $(patsubst -I%,-idirafter%, $(UBOOTINCLUDE)) \
		-idirafter $(srctree)/tools/env \
		-DUSE_HOSTCC \
		-DTEXT_BASE=$(TEXT_BASE)

ifeq ($(MTD_VERSION),old)
HOST_EXTRACFLAGS += -DMTD_OLD
endif

always := fw_printenv
hostprogs-y := fw_printenv_unstripped

fw_printenv_unstripped-objs := fw_env.o fw_env_main.o \
	crc32.o ctype.o linux_string.o \
	env_attr.o env_flags.o

quiet_cmd_strip = STRIP   $@
      cmd_strip = $(STRIP) -o $@ $<

$(obj)/fw_printenv: $(obj)/fw_printenv_unstripped FORCE
	$(call if_changed,strip)
