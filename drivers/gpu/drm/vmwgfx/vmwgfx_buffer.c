/**************************************************************************
 *
 * Copyright © 2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

#include "vmwgfx_drv.h"
<<<<<<< HEAD
#include "ttm/ttm_bo_driver.h"
#include "ttm/ttm_placement.h"
<<<<<<< HEAD
=======
#include "ttm/ttm_page_alloc.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_page_alloc.h>
>>>>>>> refs/remotes/origin/master

static uint32_t vram_placement_flags = TTM_PL_FLAG_VRAM |
	TTM_PL_FLAG_CACHED;

static uint32_t vram_ne_placement_flags = TTM_PL_FLAG_VRAM |
	TTM_PL_FLAG_CACHED |
	TTM_PL_FLAG_NO_EVICT;

static uint32_t sys_placement_flags = TTM_PL_FLAG_SYSTEM |
	TTM_PL_FLAG_CACHED;

static uint32_t gmr_placement_flags = VMW_PL_FLAG_GMR |
	TTM_PL_FLAG_CACHED;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static uint32_t gmr_ne_placement_flags = VMW_PL_FLAG_GMR |
	TTM_PL_FLAG_CACHED |
	TTM_PL_FLAG_NO_EVICT;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct ttm_placement vmw_vram_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 1,
	.placement = &vram_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &vram_placement_flags
};

static uint32_t vram_gmr_placement_flags[] = {
	TTM_PL_FLAG_VRAM | TTM_PL_FLAG_CACHED,
	VMW_PL_FLAG_GMR | TTM_PL_FLAG_CACHED
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static uint32_t gmr_vram_placement_flags[] = {
	VMW_PL_FLAG_GMR | TTM_PL_FLAG_CACHED,
	TTM_PL_FLAG_VRAM | TTM_PL_FLAG_CACHED
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct ttm_placement vmw_vram_gmr_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 2,
	.placement = vram_gmr_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &gmr_placement_flags
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static uint32_t vram_gmr_ne_placement_flags[] = {
	TTM_PL_FLAG_VRAM | TTM_PL_FLAG_CACHED | TTM_PL_FLAG_NO_EVICT,
	VMW_PL_FLAG_GMR | TTM_PL_FLAG_CACHED | TTM_PL_FLAG_NO_EVICT
};

struct ttm_placement vmw_vram_gmr_ne_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 2,
	.placement = vram_gmr_ne_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &gmr_ne_placement_flags
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct ttm_placement vmw_vram_sys_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 1,
	.placement = &vram_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &sys_placement_flags
};

struct ttm_placement vmw_vram_ne_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 1,
	.placement = &vram_ne_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &vram_ne_placement_flags
};

struct ttm_placement vmw_sys_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 1,
	.placement = &sys_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &sys_placement_flags
};

<<<<<<< HEAD
<<<<<<< HEAD
struct vmw_ttm_backend {
	struct ttm_backend backend;
	struct page **pages;
	unsigned long num_pages;
	struct vmw_private *dev_priv;
	int gmr_id;
};

static int vmw_ttm_populate(struct ttm_backend *backend,
			    unsigned long num_pages, struct page **pages,
			    struct page *dummy_read_page,
			    dma_addr_t *dma_addrs)
{
	struct vmw_ttm_backend *vmw_be =
	    container_of(backend, struct vmw_ttm_backend, backend);

	vmw_be->pages = pages;
	vmw_be->num_pages = num_pages;

	return 0;
}

static int vmw_ttm_bind(struct ttm_backend *backend, struct ttm_mem_reg *bo_mem)
{
	struct vmw_ttm_backend *vmw_be =
	    container_of(backend, struct vmw_ttm_backend, backend);

	vmw_be->gmr_id = bo_mem->start;

	return vmw_gmr_bind(vmw_be->dev_priv, vmw_be->pages,
			    vmw_be->num_pages, vmw_be->gmr_id);
}

static int vmw_ttm_unbind(struct ttm_backend *backend)
{
	struct vmw_ttm_backend *vmw_be =
	    container_of(backend, struct vmw_ttm_backend, backend);
=======
=======
>>>>>>> refs/remotes/origin/master
static uint32_t evictable_placement_flags[] = {
	TTM_PL_FLAG_SYSTEM | TTM_PL_FLAG_CACHED,
	TTM_PL_FLAG_VRAM | TTM_PL_FLAG_CACHED,
	VMW_PL_FLAG_GMR | TTM_PL_FLAG_CACHED
};

struct ttm_placement vmw_evictable_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 3,
	.placement = evictable_placement_flags,
	.num_busy_placement = 1,
	.busy_placement = &sys_placement_flags
};

struct ttm_placement vmw_srf_placement = {
	.fpfn = 0,
	.lpfn = 0,
	.num_placement = 1,
	.num_busy_placement = 2,
	.placement = &gmr_placement_flags,
	.busy_placement = gmr_vram_placement_flags
};

struct vmw_ttm_tt {
<<<<<<< HEAD
	struct ttm_tt ttm;
	struct vmw_private *dev_priv;
	int gmr_id;
};

static int vmw_ttm_bind(struct ttm_tt *ttm, struct ttm_mem_reg *bo_mem)
{
	struct vmw_ttm_tt *vmw_be = container_of(ttm, struct vmw_ttm_tt, ttm);

	vmw_be->gmr_id = bo_mem->start;

	return vmw_gmr_bind(vmw_be->dev_priv, ttm->pages,
=======
	struct ttm_dma_tt dma_ttm;
	struct vmw_private *dev_priv;
	int gmr_id;
	struct sg_table sgt;
	struct vmw_sg_table vsgt;
	uint64_t sg_alloc_size;
	bool mapped;
};

const size_t vmw_tt_size = sizeof(struct vmw_ttm_tt);

/**
 * Helper functions to advance a struct vmw_piter iterator.
 *
 * @viter: Pointer to the iterator.
 *
 * These functions return false if past the end of the list,
 * true otherwise. Functions are selected depending on the current
 * DMA mapping mode.
 */
static bool __vmw_piter_non_sg_next(struct vmw_piter *viter)
{
	return ++(viter->i) < viter->num_pages;
}

static bool __vmw_piter_sg_next(struct vmw_piter *viter)
{
	return __sg_page_iter_next(&viter->iter);
}


/**
 * Helper functions to return a pointer to the current page.
 *
 * @viter: Pointer to the iterator
 *
 * These functions return a pointer to the page currently
 * pointed to by @viter. Functions are selected depending on the
 * current mapping mode.
 */
static struct page *__vmw_piter_non_sg_page(struct vmw_piter *viter)
{
	return viter->pages[viter->i];
}

static struct page *__vmw_piter_sg_page(struct vmw_piter *viter)
{
	return sg_page_iter_page(&viter->iter);
}


/**
 * Helper functions to return the DMA address of the current page.
 *
 * @viter: Pointer to the iterator
 *
 * These functions return the DMA address of the page currently
 * pointed to by @viter. Functions are selected depending on the
 * current mapping mode.
 */
static dma_addr_t __vmw_piter_phys_addr(struct vmw_piter *viter)
{
	return page_to_phys(viter->pages[viter->i]);
}

static dma_addr_t __vmw_piter_dma_addr(struct vmw_piter *viter)
{
	return viter->addrs[viter->i];
}

static dma_addr_t __vmw_piter_sg_addr(struct vmw_piter *viter)
{
	return sg_page_iter_dma_address(&viter->iter);
}


/**
 * vmw_piter_start - Initialize a struct vmw_piter.
 *
 * @viter: Pointer to the iterator to initialize
 * @vsgt: Pointer to a struct vmw_sg_table to initialize from
 *
 * Note that we're following the convention of __sg_page_iter_start, so that
 * the iterator doesn't point to a valid page after initialization; it has
 * to be advanced one step first.
 */
void vmw_piter_start(struct vmw_piter *viter, const struct vmw_sg_table *vsgt,
		     unsigned long p_offset)
{
	viter->i = p_offset - 1;
	viter->num_pages = vsgt->num_pages;
	switch (vsgt->mode) {
	case vmw_dma_phys:
		viter->next = &__vmw_piter_non_sg_next;
		viter->dma_address = &__vmw_piter_phys_addr;
		viter->page = &__vmw_piter_non_sg_page;
		viter->pages = vsgt->pages;
		break;
	case vmw_dma_alloc_coherent:
		viter->next = &__vmw_piter_non_sg_next;
		viter->dma_address = &__vmw_piter_dma_addr;
		viter->page = &__vmw_piter_non_sg_page;
		viter->addrs = vsgt->addrs;
		break;
	case vmw_dma_map_populate:
	case vmw_dma_map_bind:
		viter->next = &__vmw_piter_sg_next;
		viter->dma_address = &__vmw_piter_sg_addr;
		viter->page = &__vmw_piter_sg_page;
		__sg_page_iter_start(&viter->iter, vsgt->sgt->sgl,
				     vsgt->sgt->orig_nents, p_offset);
		break;
	default:
		BUG();
	}
}

/**
 * vmw_ttm_unmap_from_dma - unmap  device addresses previsouly mapped for
 * TTM pages
 *
 * @vmw_tt: Pointer to a struct vmw_ttm_backend
 *
 * Used to free dma mappings previously mapped by vmw_ttm_map_for_dma.
 */
static void vmw_ttm_unmap_from_dma(struct vmw_ttm_tt *vmw_tt)
{
	struct device *dev = vmw_tt->dev_priv->dev->dev;

	dma_unmap_sg(dev, vmw_tt->sgt.sgl, vmw_tt->sgt.nents,
		DMA_BIDIRECTIONAL);
	vmw_tt->sgt.nents = vmw_tt->sgt.orig_nents;
}

/**
 * vmw_ttm_map_for_dma - map TTM pages to get device addresses
 *
 * @vmw_tt: Pointer to a struct vmw_ttm_backend
 *
 * This function is used to get device addresses from the kernel DMA layer.
 * However, it's violating the DMA API in that when this operation has been
 * performed, it's illegal for the CPU to write to the pages without first
 * unmapping the DMA mappings, or calling dma_sync_sg_for_cpu(). It is
 * therefore only legal to call this function if we know that the function
 * dma_sync_sg_for_cpu() is a NOP, and dma_sync_sg_for_device() is at most
 * a CPU write buffer flush.
 */
static int vmw_ttm_map_for_dma(struct vmw_ttm_tt *vmw_tt)
{
	struct device *dev = vmw_tt->dev_priv->dev->dev;
	int ret;

	ret = dma_map_sg(dev, vmw_tt->sgt.sgl, vmw_tt->sgt.orig_nents,
			 DMA_BIDIRECTIONAL);
	if (unlikely(ret == 0))
		return -ENOMEM;

	vmw_tt->sgt.nents = ret;

	return 0;
}

/**
 * vmw_ttm_map_dma - Make sure TTM pages are visible to the device
 *
 * @vmw_tt: Pointer to a struct vmw_ttm_tt
 *
 * Select the correct function for and make sure the TTM pages are
 * visible to the device. Allocate storage for the device mappings.
 * If a mapping has already been performed, indicated by the storage
 * pointer being non NULL, the function returns success.
 */
static int vmw_ttm_map_dma(struct vmw_ttm_tt *vmw_tt)
{
	struct vmw_private *dev_priv = vmw_tt->dev_priv;
	struct ttm_mem_global *glob = vmw_mem_glob(dev_priv);
	struct vmw_sg_table *vsgt = &vmw_tt->vsgt;
	struct vmw_piter iter;
	dma_addr_t old;
	int ret = 0;
	static size_t sgl_size;
	static size_t sgt_size;

	if (vmw_tt->mapped)
		return 0;

	vsgt->mode = dev_priv->map_mode;
	vsgt->pages = vmw_tt->dma_ttm.ttm.pages;
	vsgt->num_pages = vmw_tt->dma_ttm.ttm.num_pages;
	vsgt->addrs = vmw_tt->dma_ttm.dma_address;
	vsgt->sgt = &vmw_tt->sgt;

	switch (dev_priv->map_mode) {
	case vmw_dma_map_bind:
	case vmw_dma_map_populate:
		if (unlikely(!sgl_size)) {
			sgl_size = ttm_round_pot(sizeof(struct scatterlist));
			sgt_size = ttm_round_pot(sizeof(struct sg_table));
		}
		vmw_tt->sg_alloc_size = sgt_size + sgl_size * vsgt->num_pages;
		ret = ttm_mem_global_alloc(glob, vmw_tt->sg_alloc_size, false,
					   true);
		if (unlikely(ret != 0))
			return ret;

		ret = sg_alloc_table_from_pages(&vmw_tt->sgt, vsgt->pages,
						vsgt->num_pages, 0,
						(unsigned long)
						vsgt->num_pages << PAGE_SHIFT,
						GFP_KERNEL);
		if (unlikely(ret != 0))
			goto out_sg_alloc_fail;

		if (vsgt->num_pages > vmw_tt->sgt.nents) {
			uint64_t over_alloc =
				sgl_size * (vsgt->num_pages -
					    vmw_tt->sgt.nents);

			ttm_mem_global_free(glob, over_alloc);
			vmw_tt->sg_alloc_size -= over_alloc;
		}

		ret = vmw_ttm_map_for_dma(vmw_tt);
		if (unlikely(ret != 0))
			goto out_map_fail;

		break;
	default:
		break;
	}

	old = ~((dma_addr_t) 0);
	vmw_tt->vsgt.num_regions = 0;
	for (vmw_piter_start(&iter, vsgt, 0); vmw_piter_next(&iter);) {
		dma_addr_t cur = vmw_piter_dma_addr(&iter);

		if (cur != old + PAGE_SIZE)
			vmw_tt->vsgt.num_regions++;
		old = cur;
	}

	vmw_tt->mapped = true;
	return 0;

out_map_fail:
	sg_free_table(vmw_tt->vsgt.sgt);
	vmw_tt->vsgt.sgt = NULL;
out_sg_alloc_fail:
	ttm_mem_global_free(glob, vmw_tt->sg_alloc_size);
	return ret;
}

/**
 * vmw_ttm_unmap_dma - Tear down any TTM page device mappings
 *
 * @vmw_tt: Pointer to a struct vmw_ttm_tt
 *
 * Tear down any previously set up device DMA mappings and free
 * any storage space allocated for them. If there are no mappings set up,
 * this function is a NOP.
 */
static void vmw_ttm_unmap_dma(struct vmw_ttm_tt *vmw_tt)
{
	struct vmw_private *dev_priv = vmw_tt->dev_priv;

	if (!vmw_tt->vsgt.sgt)
		return;

	switch (dev_priv->map_mode) {
	case vmw_dma_map_bind:
	case vmw_dma_map_populate:
		vmw_ttm_unmap_from_dma(vmw_tt);
		sg_free_table(vmw_tt->vsgt.sgt);
		vmw_tt->vsgt.sgt = NULL;
		ttm_mem_global_free(vmw_mem_glob(dev_priv),
				    vmw_tt->sg_alloc_size);
		break;
	default:
		break;
	}
	vmw_tt->mapped = false;
}

static int vmw_ttm_bind(struct ttm_tt *ttm, struct ttm_mem_reg *bo_mem)
{
	struct vmw_ttm_tt *vmw_be =
		container_of(ttm, struct vmw_ttm_tt, dma_ttm.ttm);
	int ret;

	ret = vmw_ttm_map_dma(vmw_be);
	if (unlikely(ret != 0))
		return ret;

	vmw_be->gmr_id = bo_mem->start;

	return vmw_gmr_bind(vmw_be->dev_priv, &vmw_be->vsgt,
>>>>>>> refs/remotes/origin/master
			    ttm->num_pages, vmw_be->gmr_id);
}

static int vmw_ttm_unbind(struct ttm_tt *ttm)
{
<<<<<<< HEAD
	struct vmw_ttm_tt *vmw_be = container_of(ttm, struct vmw_ttm_tt, ttm);
>>>>>>> refs/remotes/origin/cm-10.0

	vmw_gmr_unbind(vmw_be->dev_priv, vmw_be->gmr_id);
	return 0;
}

<<<<<<< HEAD
static void vmw_ttm_clear(struct ttm_backend *backend)
{
	struct vmw_ttm_backend *vmw_be =
		container_of(backend, struct vmw_ttm_backend, backend);

	vmw_be->pages = NULL;
	vmw_be->num_pages = 0;
}

static void vmw_ttm_destroy(struct ttm_backend *backend)
{
	struct vmw_ttm_backend *vmw_be =
	    container_of(backend, struct vmw_ttm_backend, backend);

=======
static void vmw_ttm_destroy(struct ttm_tt *ttm)
{
	struct vmw_ttm_tt *vmw_be = container_of(ttm, struct vmw_ttm_tt, ttm);

	ttm_tt_fini(ttm);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(vmw_be);
}

static struct ttm_backend_func vmw_ttm_func = {
<<<<<<< HEAD
	.populate = vmw_ttm_populate,
	.clear = vmw_ttm_clear,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct vmw_ttm_tt *vmw_be =
		container_of(ttm, struct vmw_ttm_tt, dma_ttm.ttm);

	vmw_gmr_unbind(vmw_be->dev_priv, vmw_be->gmr_id);

	if (vmw_be->dev_priv->map_mode == vmw_dma_map_bind)
		vmw_ttm_unmap_dma(vmw_be);

	return 0;
}

static void vmw_ttm_destroy(struct ttm_tt *ttm)
{
	struct vmw_ttm_tt *vmw_be =
		container_of(ttm, struct vmw_ttm_tt, dma_ttm.ttm);

	vmw_ttm_unmap_dma(vmw_be);
	if (vmw_be->dev_priv->map_mode == vmw_dma_alloc_coherent)
		ttm_dma_tt_fini(&vmw_be->dma_ttm);
	else
		ttm_tt_fini(ttm);
	kfree(vmw_be);
}

static int vmw_ttm_populate(struct ttm_tt *ttm)
{
	struct vmw_ttm_tt *vmw_tt =
		container_of(ttm, struct vmw_ttm_tt, dma_ttm.ttm);
	struct vmw_private *dev_priv = vmw_tt->dev_priv;
	struct ttm_mem_global *glob = vmw_mem_glob(dev_priv);
	int ret;

	if (ttm->state != tt_unpopulated)
		return 0;

	if (dev_priv->map_mode == vmw_dma_alloc_coherent) {
		size_t size =
			ttm_round_pot(ttm->num_pages * sizeof(dma_addr_t));
		ret = ttm_mem_global_alloc(glob, size, false, true);
		if (unlikely(ret != 0))
			return ret;

		ret = ttm_dma_populate(&vmw_tt->dma_ttm, dev_priv->dev->dev);
		if (unlikely(ret != 0))
			ttm_mem_global_free(glob, size);
	} else
		ret = ttm_pool_populate(ttm);

	return ret;
}

static void vmw_ttm_unpopulate(struct ttm_tt *ttm)
{
	struct vmw_ttm_tt *vmw_tt = container_of(ttm, struct vmw_ttm_tt,
						 dma_ttm.ttm);
	struct vmw_private *dev_priv = vmw_tt->dev_priv;
	struct ttm_mem_global *glob = vmw_mem_glob(dev_priv);

	vmw_ttm_unmap_dma(vmw_tt);
	if (dev_priv->map_mode == vmw_dma_alloc_coherent) {
		size_t size =
			ttm_round_pot(ttm->num_pages * sizeof(dma_addr_t));

		ttm_dma_unpopulate(&vmw_tt->dma_ttm, dev_priv->dev->dev);
		ttm_mem_global_free(glob, size);
	} else
		ttm_pool_unpopulate(ttm);
}

static struct ttm_backend_func vmw_ttm_func = {
>>>>>>> refs/remotes/origin/master
	.bind = vmw_ttm_bind,
	.unbind = vmw_ttm_unbind,
	.destroy = vmw_ttm_destroy,
};

<<<<<<< HEAD
<<<<<<< HEAD
struct ttm_backend *vmw_ttm_backend_init(struct ttm_bo_device *bdev)
{
	struct vmw_ttm_backend *vmw_be;
=======
=======
>>>>>>> refs/remotes/origin/master
struct ttm_tt *vmw_ttm_tt_create(struct ttm_bo_device *bdev,
				 unsigned long size, uint32_t page_flags,
				 struct page *dummy_read_page)
{
	struct vmw_ttm_tt *vmw_be;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	vmw_be = kmalloc(sizeof(*vmw_be), GFP_KERNEL);
	if (!vmw_be)
		return NULL;

<<<<<<< HEAD
	vmw_be->backend.func = &vmw_ttm_func;
	vmw_be->dev_priv = container_of(bdev, struct vmw_private, bdev);

	return &vmw_be->backend;
=======
	vmw_be->ttm.func = &vmw_ttm_func;
	vmw_be->dev_priv = container_of(bdev, struct vmw_private, bdev);

	if (ttm_tt_init(&vmw_be->ttm, bdev, size, page_flags, dummy_read_page)) {
		kfree(vmw_be);
		return NULL;
	}

	return &vmw_be->ttm;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int ret;

	vmw_be = kzalloc(sizeof(*vmw_be), GFP_KERNEL);
	if (!vmw_be)
		return NULL;

	vmw_be->dma_ttm.ttm.func = &vmw_ttm_func;
	vmw_be->dev_priv = container_of(bdev, struct vmw_private, bdev);

	if (vmw_be->dev_priv->map_mode == vmw_dma_alloc_coherent)
		ret = ttm_dma_tt_init(&vmw_be->dma_ttm, bdev, size, page_flags,
				      dummy_read_page);
	else
		ret = ttm_tt_init(&vmw_be->dma_ttm.ttm, bdev, size, page_flags,
				  dummy_read_page);
	if (unlikely(ret != 0))
		goto out_no_init;

	return &vmw_be->dma_ttm.ttm;
out_no_init:
	kfree(vmw_be);
	return NULL;
>>>>>>> refs/remotes/origin/master
}

int vmw_invalidate_caches(struct ttm_bo_device *bdev, uint32_t flags)
{
	return 0;
}

int vmw_init_mem_type(struct ttm_bo_device *bdev, uint32_t type,
		      struct ttm_mem_type_manager *man)
{
	switch (type) {
	case TTM_PL_SYSTEM:
		/* System memory */

		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_CACHED;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	case TTM_PL_VRAM:
		/* "On-card" video ram */
		man->func = &ttm_bo_manager_func;
		man->gpu_offset = 0;
		man->flags = TTM_MEMTYPE_FLAG_FIXED | TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_CACHED;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	case VMW_PL_GMR:
		/*
		 * "Guest Memory Regions" is an aperture like feature with
		 *  one slot per bo. There is an upper limit of the number of
		 *  slots as well as the bo size.
		 */
		man->func = &vmw_gmrid_manager_func;
		man->gpu_offset = 0;
		man->flags = TTM_MEMTYPE_FLAG_CMA | TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_CACHED;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	default:
		DRM_ERROR("Unsupported memory type %u\n", (unsigned)type);
		return -EINVAL;
	}
	return 0;
}

void vmw_evict_flags(struct ttm_buffer_object *bo,
		     struct ttm_placement *placement)
{
	*placement = vmw_sys_placement;
}

<<<<<<< HEAD
/**
 * FIXME: Proper access checks on buffers.
 */

static int vmw_verify_access(struct ttm_buffer_object *bo, struct file *filp)
{
	return 0;
=======
static int vmw_verify_access(struct ttm_buffer_object *bo, struct file *filp)
{
	struct ttm_object_file *tfile =
		vmw_fpriv((struct drm_file *)filp->private_data)->tfile;

	return vmw_user_dmabuf_verify_access(bo, tfile);
>>>>>>> refs/remotes/origin/master
}

static int vmw_ttm_io_mem_reserve(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	struct vmw_private *dev_priv = container_of(bdev, struct vmw_private, bdev);

	mem->bus.addr = NULL;
	mem->bus.is_iomem = false;
	mem->bus.offset = 0;
	mem->bus.size = mem->num_pages << PAGE_SHIFT;
	mem->bus.base = 0;
	if (!(man->flags & TTM_MEMTYPE_FLAG_MAPPABLE))
		return -EINVAL;
	switch (mem->mem_type) {
	case TTM_PL_SYSTEM:
	case VMW_PL_GMR:
		return 0;
	case TTM_PL_VRAM:
		mem->bus.offset = mem->start << PAGE_SHIFT;
		mem->bus.base = dev_priv->vram_start;
		mem->bus.is_iomem = true;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void vmw_ttm_io_mem_free(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
}

static int vmw_ttm_fault_reserve_notify(struct ttm_buffer_object *bo)
{
	return 0;
}

/**
 * FIXME: We're using the old vmware polling method to sync.
 * Do this with fences instead.
 */

static void *vmw_sync_obj_ref(void *sync_obj)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return sync_obj;
=======

	return (void *)
		vmw_fence_obj_reference((struct vmw_fence_obj *) sync_obj);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	return (void *)
		vmw_fence_obj_reference((struct vmw_fence_obj *) sync_obj);
>>>>>>> refs/remotes/origin/master
}

static void vmw_sync_obj_unref(void **sync_obj)
{
<<<<<<< HEAD
<<<<<<< HEAD
	*sync_obj = NULL;
=======
	vmw_fence_obj_unreference((struct vmw_fence_obj **) sync_obj);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int vmw_sync_obj_flush(void *sync_obj, void *sync_arg)
{
<<<<<<< HEAD
	struct vmw_private *dev_priv = (struct vmw_private *)sync_arg;

	mutex_lock(&dev_priv->hw_mutex);
	vmw_write(dev_priv, SVGA_REG_SYNC, SVGA_SYNC_GENERIC);
	mutex_unlock(&dev_priv->hw_mutex);
=======
	vmw_fence_obj_flush((struct vmw_fence_obj *) sync_obj);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static bool vmw_sync_obj_signaled(void *sync_obj, void *sync_arg)
{
<<<<<<< HEAD
	struct vmw_private *dev_priv = (struct vmw_private *)sync_arg;
	uint32_t sequence = (unsigned long) sync_obj;

	return vmw_fence_signaled(dev_priv, sequence);
=======
	unsigned long flags = (unsigned long) sync_arg;
	return	vmw_fence_obj_signaled((struct vmw_fence_obj *) sync_obj,
				       (uint32_t) flags);

>>>>>>> refs/remotes/origin/cm-10.0
}

static int vmw_sync_obj_wait(void *sync_obj, void *sync_arg,
			     bool lazy, bool interruptible)
{
<<<<<<< HEAD
	struct vmw_private *dev_priv = (struct vmw_private *)sync_arg;
	uint32_t sequence = (unsigned long) sync_obj;

	return vmw_wait_fence(dev_priv, false, sequence, false, 3*HZ);
}

struct ttm_bo_driver vmw_bo_driver = {
	.create_ttm_backend_entry = vmw_ttm_backend_init,
=======
	unsigned long flags = (unsigned long) sync_arg;

	return vmw_fence_obj_wait((struct vmw_fence_obj *) sync_obj,
				  (uint32_t) flags,
=======
	vmw_fence_obj_unreference((struct vmw_fence_obj **) sync_obj);
}

static int vmw_sync_obj_flush(void *sync_obj)
{
	vmw_fence_obj_flush((struct vmw_fence_obj *) sync_obj);
	return 0;
}

static bool vmw_sync_obj_signaled(void *sync_obj)
{
	return	vmw_fence_obj_signaled((struct vmw_fence_obj *) sync_obj,
				       DRM_VMW_FENCE_FLAG_EXEC);

}

static int vmw_sync_obj_wait(void *sync_obj, bool lazy, bool interruptible)
{
	return vmw_fence_obj_wait((struct vmw_fence_obj *) sync_obj,
				  DRM_VMW_FENCE_FLAG_EXEC,
>>>>>>> refs/remotes/origin/master
				  lazy, interruptible,
				  VMW_FENCE_WAIT_TIMEOUT);
}

struct ttm_bo_driver vmw_bo_driver = {
	.ttm_tt_create = &vmw_ttm_tt_create,
<<<<<<< HEAD
	.ttm_tt_populate = &ttm_pool_populate,
	.ttm_tt_unpopulate = &ttm_pool_unpopulate,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.ttm_tt_populate = &vmw_ttm_populate,
	.ttm_tt_unpopulate = &vmw_ttm_unpopulate,
>>>>>>> refs/remotes/origin/master
	.invalidate_caches = vmw_invalidate_caches,
	.init_mem_type = vmw_init_mem_type,
	.evict_flags = vmw_evict_flags,
	.move = NULL,
	.verify_access = vmw_verify_access,
	.sync_obj_signaled = vmw_sync_obj_signaled,
	.sync_obj_wait = vmw_sync_obj_wait,
	.sync_obj_flush = vmw_sync_obj_flush,
	.sync_obj_unref = vmw_sync_obj_unref,
	.sync_obj_ref = vmw_sync_obj_ref,
	.move_notify = NULL,
	.swap_notify = NULL,
	.fault_reserve_notify = &vmw_ttm_fault_reserve_notify,
	.io_mem_reserve = &vmw_ttm_io_mem_reserve,
	.io_mem_free = &vmw_ttm_io_mem_free,
};
