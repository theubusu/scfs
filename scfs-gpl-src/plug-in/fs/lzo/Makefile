#
#  Copyright 2007 Sony Corporation.
#
#  This program is free software; you can redistribute  it and/or modify it
#  under  the terms of  the GNU General  Public License as published by the
#  Free Software Foundation;  either version 2 of the  License.
#
#  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
#  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
#  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
#  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  You should have received a copy of the  GNU General Public License along
#  with this program; if not, write  to the Free Software Foundation, Inc.,
#  675 Mass Ave, Cambridge, MA 02139, USA.
#

#------------------------------------------------
# Makefile for scfs_uncompression_lzo.
# This is a scfs uncompression plug-in.
#------------------------------------------------

SCFS_GPL_TOP ?= $(shell pwd)/../../..
export SCFS_GPL_TOP
include $(SCFS_GPL_TOP)/.config
include $(SCFS_GPL_TOP)/rules.make

# ==========================================================
#   common
# ==========================================================
EXTRA_CFLAGS	:= -I$(SCFS_GPL_TOP)/plug-in/include -I$(shell pwd)
EXTRA_CFLAGS	+= -I$(SCFS_GPL_TOP)/../plug-in/include
BUILD_TOP	 = $(SCFS_GPL_TOP)/$(BUILD_DIR_TARGET)
BUILD_DIR	 = $(BUILD_TOP)/module
export EXTRA_CFLAGS

NAME	= scfs_uncompression_lzo
TARGET	= $(NAME).ko
OBJS	= scfs_uncompression_lzo.o

all: $(TARGET)
	@install -d $(BUILD_DIR)
	install -m 644 $(TARGET) $(BUILD_DIR)

ifneq ($(KBUILD),)
# ==========================================================
#   kbuild
# ==========================================================
obj-m := $(subst .ko,.o,$(TARGET))
INSTDIR = kernel/drivers/snsc

$(TARGET): FORCE
	$(MAKE) -C $(KBUILD) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KBUILD) M=$(shell pwd) clean
	@rm -f $(BUILD_DIR)/$(TARGET)

install-only:
	$(MAKE) -C $(KBUILD) M=$(shell pwd)				\
		INSTALL_MOD_PATH=$(INSTROOT) INSTALL_MOD_DIR=$(INSTDIR)	\
		modules_install

install: all install-only

else # ($(KBUILD),)
# ==========================================================
#   modpost
# ==========================================================
EXTRA_CFLAGS += $(MODPOST_CFLAGS)
EXTRA_CFLAGS += -I$(KERN_HEADER_PATH)

KERNELRELEASE = $(shell egrep UTS_RELEASE $(KERN_HEADER_PATH)/linux/version.h \
		| cut -d '"' -f 2)
INSTALL_DIR   = $(INSTROOT)/lib/modules/$(KERNELRELEASE)/kernel/drivers/snsc

CC = $(CROSS)gcc
LD = $(CROSS)ld
AR = $(CROSS)ar

%.ko: %.o %.mod.o
	$(LD) -r -o $*.ko $*.o $*.mod.o

%.mod.o: %.mod.c
	$(CC) -Wp,-MD,.$*.mod.o.d $(EXTRA_CFLAGS)		\
		-DKBUILD_BASENAME=$* -DKBUILD_MODNAME=$*	\
		-c -o $@ $<

%.mod.c: %.o
	$(MODPOST) $<

.c.o:
	@for i in $(OBJS); do			\
		if [ $$i = $@ ]; then		\
			MOD_NAME=$(NAME);	\
		fi ;				\
	done
	$(CC) -Wp,-MD,.$$MOD_NAME.o.d $(EXTRA_CFLAGS)			\
		-DKBUILD_BASENAME=$(@F:.o=) -DKBUILD_MODNAME=$$MOD_NAME	\
		-c -o $@ $<

clean:
	@rm -fr .*.cmd .*.o.d *.mod.c *.o *.ko .tmp*
	@rm -f $(BUILD_DIR)/$(TARGET)


install-only:
		@install -d $(INSTALL_DIR)
		install -m 644 $(TARGET) $(INSTALL_DIR)

install: all install-only

endif # ($(KBUILD),)

.PHONY: FORCE
FORCE:
