#
# Makefile for the drm device driver.  This driver provides support for the

ccflags-y := -Iinclude/drm
ttm-y := ttm_agp_backend.o ttm_memory.o ttm_tt.o ttm_bo.o \
	ttm_bo_util.o ttm_bo_vm.o ttm_module.o \
	ttm_object.o ttm_lock.o ttm_execbuf_util.o ttm_page_alloc.o \
<<<<<<< HEAD
	ttm_bo_manager.o

<<<<<<< HEAD
=======
ifeq ($(CONFIG_SWIOTLB),y)
ttm-y += ttm_page_alloc_dma.o
endif

>>>>>>> refs/remotes/origin/cm-10.0
=======
	ttm_bo_manager.o ttm_page_alloc_dma.o

>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_DRM_TTM) += ttm.o
