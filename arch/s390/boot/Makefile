#
# Makefile for the linux s390-specific parts of the memory manager.
#

COMPILE_VERSION := __linux_compile_version_id__`hostname |  \
			tr -c '[0-9A-Za-z]' '_'`__`date | \
			tr -c '[0-9A-Za-z]' '_'`_t

ccflags-y  := -DCOMPILE_VERSION=$(COMPILE_VERSION) -gstabs -I.

targets := image
targets += bzImage
subdir- := compressed

$(obj)/image: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/bzImage: $(obj)/compressed/vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/compressed/vmlinux: FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@

<<<<<<< HEAD
install: $(CONFIGURE) $(obj)/image
	sh -x  $(srctree)/$(obj)/install.sh $(KERNELRELEASE) $(obj)/image \
<<<<<<< HEAD
	      System.map Kerntypes "$(INSTALL_PATH)"
=======
	      System.map "$(INSTALL_PATH)"
>>>>>>> refs/remotes/origin/cm-10.0
=======
install: $(CONFIGURE) $(obj)/bzImage
	sh -x  $(srctree)/$(obj)/install.sh $(KERNELRELEASE) $(obj)/bzImage \
	      System.map "$(INSTALL_PATH)"
>>>>>>> refs/remotes/origin/master
