#
# arch/arm/boot/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2002 Russell King
#

<<<<<<< HEAD
<<<<<<< HEAD
MKIMAGE         := $(srctree)/scripts/mkuboot.sh

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
ifneq ($(MACHINE),)
include $(srctree)/$(MACHINE)/Makefile.boot
endif

# Note: the following conditions must always be true:
#   ZRELADDR == virt_to_phys(PAGE_OFFSET + TEXT_OFFSET)
#   PARAMS_PHYS must be within 4MB of ZRELADDR
#   INITRD_PHYS must be in RAM
ZRELADDR    := $(zreladdr-y)
PARAMS_PHYS := $(params_phys-y)
INITRD_PHYS := $(initrd_phys-y)

export ZRELADDR INITRD_PHYS PARAMS_PHYS

targets := Image zImage xipImage bootpImage uImage

ifeq ($(CONFIG_XIP_KERNEL),y)

$(obj)/xipImage: vmlinux FORCE
	$(call if_changed,objcopy)
<<<<<<< HEAD
	@echo '  Kernel: $@ is ready (physical address: $(CONFIG_XIP_PHYS_ADDR))'
=======
	@$(kecho) '  Kernel: $@ is ready (physical address: $(CONFIG_XIP_PHYS_ADDR))'
>>>>>>> refs/remotes/origin/master

$(obj)/Image $(obj)/zImage: FORCE
	@echo 'Kernel configured for XIP (CONFIG_XIP_KERNEL=y)'
	@echo 'Only the xipImage target is available in this case'
	@false

else

$(obj)/xipImage: FORCE
	@echo 'Kernel not configured for XIP (CONFIG_XIP_KERNEL!=y)'
	@false

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)
<<<<<<< HEAD
	@echo '  Kernel: $@ is ready'
=======
	@$(kecho) '  Kernel: $@ is ready'
>>>>>>> refs/remotes/origin/master

$(obj)/compressed/vmlinux: $(obj)/Image FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@

$(obj)/zImage:	$(obj)/compressed/vmlinux FORCE
	$(call if_changed,objcopy)
<<<<<<< HEAD
	@echo '  Kernel: $@ is ready'

endif

<<<<<<< HEAD
quiet_cmd_uimage = UIMAGE  $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A arm -O linux -T kernel \
		   -C none -a $(LOADADDR) -e $(STARTADDR) \
		   -n 'Linux-$(KERNELRELEASE)' -d $< $@

ifeq ($(CONFIG_ZBOOT_ROM),y)
$(obj)/uImage: LOADADDR=$(CONFIG_ZBOOT_ROM_TEXT)
else
$(obj)/uImage: LOADADDR=$(ZRELADDR)
endif

$(obj)/uImage: STARTADDR=$(LOADADDR)

$(obj)/uImage:	$(obj)/zImage FORCE
=======
targets += $(dtb-y)

# Rule to build device tree blobs
$(obj)/%.dtb: $(src)/dts/%.dts FORCE
	$(call if_changed_dep,dtc)

$(obj)/dtbs: $(addprefix $(obj)/, $(dtb-y))

clean-files := *.dtb

=======
	@$(kecho) '  Kernel: $@ is ready'

endif

>>>>>>> refs/remotes/origin/master
ifneq ($(LOADADDR),)
  UIMAGE_LOADADDR=$(LOADADDR)
else
  ifeq ($(CONFIG_ZBOOT_ROM),y)
    UIMAGE_LOADADDR=$(CONFIG_ZBOOT_ROM_TEXT)
  else
    UIMAGE_LOADADDR=$(ZRELADDR)
  endif
endif

check_for_multiple_loadaddr = \
<<<<<<< HEAD
if [ $(words $(UIMAGE_LOADADDR)) -gt 1 ]; then \
	echo 'multiple load addresses: $(UIMAGE_LOADADDR)'; \
=======
if [ $(words $(UIMAGE_LOADADDR)) -ne 1 ]; then \
	echo 'multiple (or no) load addresses: $(UIMAGE_LOADADDR)'; \
>>>>>>> refs/remotes/origin/master
	echo 'This is incompatible with uImages'; \
	echo 'Specify LOADADDR on the commandline to build an uImage'; \
	false; \
fi

$(obj)/uImage:	$(obj)/zImage FORCE
	@$(check_for_multiple_loadaddr)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	$(call if_changed,uimage)
	@echo '  Image $@ is ready'
=======
	$(call if_changed,uimage)
	@$(kecho) '  Image $@ is ready'
>>>>>>> refs/remotes/origin/master

$(obj)/bootp/bootp: $(obj)/zImage initrd FORCE
	$(Q)$(MAKE) $(build)=$(obj)/bootp $@
	@:

$(obj)/bootpImage: $(obj)/bootp/bootp FORCE
	$(call if_changed,objcopy)
<<<<<<< HEAD
	@echo '  Kernel: $@ is ready'
=======
	@$(kecho) '  Kernel: $@ is ready'
>>>>>>> refs/remotes/origin/master

PHONY += initrd FORCE
initrd:
	@test "$(INITRD_PHYS)" != "" || \
	(echo This machine does not support INITRD; exit -1)
	@test "$(INITRD)" != "" || \
	(echo You must specify INITRD; exit -1)

<<<<<<< HEAD
install: $(obj)/Image
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image System.map "$(INSTALL_PATH)"

zinstall: $(obj)/zImage
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/zImage System.map "$(INSTALL_PATH)"

uinstall: $(obj)/uImage
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/uImage System.map "$(INSTALL_PATH)"

zi:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/zImage System.map "$(INSTALL_PATH)"

i:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image System.map "$(INSTALL_PATH)"

subdir-	    := bootp compressed
=======
install:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh "$(KERNELRELEASE)" \
	$(obj)/Image System.map "$(INSTALL_PATH)"

zinstall:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh "$(KERNELRELEASE)" \
	$(obj)/zImage System.map "$(INSTALL_PATH)"

uinstall:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh "$(KERNELRELEASE)" \
	$(obj)/uImage System.map "$(INSTALL_PATH)"

zi:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh "$(KERNELRELEASE)" \
	$(obj)/zImage System.map "$(INSTALL_PATH)"

i:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh "$(KERNELRELEASE)" \
	$(obj)/Image System.map "$(INSTALL_PATH)"

subdir-	    := bootp compressed dts
>>>>>>> refs/remotes/origin/master
