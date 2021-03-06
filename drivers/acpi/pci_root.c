/*
 *  pci_root.c - ACPI PCI Root Bridge Driver ($Revision: 40 $)
 *
 *  Copyright (C) 2001, 2002 Andy Grover <andrew.grover@intel.com>
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/spinlock.h>
=======
#include <linux/mutex.h>
>>>>>>> refs/remotes/origin/master
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/pci.h>
#include <linux/pci-acpi.h>
#include <linux/pci-aspm.h>
#include <linux/acpi.h>
#include <linux/slab.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>
#include <acpi/apei.h>

<<<<<<< HEAD
=======
#include "internal.h"

>>>>>>> refs/remotes/origin/master
#define PREFIX "ACPI: "

#define _COMPONENT		ACPI_PCI_COMPONENT
ACPI_MODULE_NAME("pci_root");
#define ACPI_PCI_ROOT_CLASS		"pci_bridge"
#define ACPI_PCI_ROOT_DEVICE_NAME	"PCI Root Bridge"
<<<<<<< HEAD
static int acpi_pci_root_add(struct acpi_device *device);
static int acpi_pci_root_remove(struct acpi_device *device, int type);
static int acpi_pci_root_start(struct acpi_device *device);

#define ACPI_PCIE_REQ_SUPPORT (OSC_EXT_PCI_CONFIG_SUPPORT \
				| OSC_ACTIVE_STATE_PWR_SUPPORT \
				| OSC_CLOCK_PWR_CAPABILITY_SUPPORT \
				| OSC_MSI_SUPPORT)
=======
static int acpi_pci_root_add(struct acpi_device *device,
			     const struct acpi_device_id *not_used);
static void acpi_pci_root_remove(struct acpi_device *device);

#define ACPI_PCIE_REQ_SUPPORT (OSC_PCI_EXT_CONFIG_SUPPORT \
				| OSC_PCI_ASPM_SUPPORT \
				| OSC_PCI_CLOCK_PM_SUPPORT \
				| OSC_PCI_MSI_SUPPORT)
>>>>>>> refs/remotes/origin/master

static const struct acpi_device_id root_device_ids[] = {
	{"PNP0A03", 0},
	{"", 0},
};
<<<<<<< HEAD
MODULE_DEVICE_TABLE(acpi, root_device_ids);

static struct acpi_driver acpi_pci_root_driver = {
	.name = "pci_root",
	.class = ACPI_PCI_ROOT_CLASS,
	.ids = root_device_ids,
	.ops = {
		.add = acpi_pci_root_add,
		.remove = acpi_pci_root_remove,
		.start = acpi_pci_root_start,
		},
};

static LIST_HEAD(acpi_pci_roots);

static struct acpi_pci_driver *sub_driver;
static DEFINE_MUTEX(osc_lock);

int acpi_pci_register_driver(struct acpi_pci_driver *driver)
{
	int n = 0;
	struct acpi_pci_root *root;

	struct acpi_pci_driver **pptr = &sub_driver;
	while (*pptr)
		pptr = &(*pptr)->next;
	*pptr = driver;

	if (!driver->add)
		return 0;

	list_for_each_entry(root, &acpi_pci_roots, node) {
		driver->add(root->device->handle);
		n++;
	}

	return n;
}

EXPORT_SYMBOL(acpi_pci_register_driver);

void acpi_pci_unregister_driver(struct acpi_pci_driver *driver)
{
	struct acpi_pci_root *root;

	struct acpi_pci_driver **pptr = &sub_driver;
	while (*pptr) {
		if (*pptr == driver)
			break;
		pptr = &(*pptr)->next;
	}
	BUG_ON(!*pptr);
	*pptr = (*pptr)->next;

	if (!driver->remove)
		return;

	list_for_each_entry(root, &acpi_pci_roots, node)
		driver->remove(root->device->handle);
}

EXPORT_SYMBOL(acpi_pci_unregister_driver);

acpi_handle acpi_get_pci_rootbridge_handle(unsigned int seg, unsigned int bus)
{
	struct acpi_pci_root *root;
	
	list_for_each_entry(root, &acpi_pci_roots, node)
		if ((root->segment == (u16) seg) &&
		    (root->secondary.start == (u16) bus))
			return root->device->handle;
	return NULL;		
}

EXPORT_SYMBOL_GPL(acpi_get_pci_rootbridge_handle);

=======

static struct acpi_scan_handler pci_root_handler = {
	.ids = root_device_ids,
	.attach = acpi_pci_root_add,
	.detach = acpi_pci_root_remove,
	.hotplug = {
		.ignore = true,
	},
};

static DEFINE_MUTEX(osc_lock);

>>>>>>> refs/remotes/origin/master
/**
 * acpi_is_root_bridge - determine whether an ACPI CA node is a PCI root bridge
 * @handle - the ACPI CA node in question.
 *
 * Note: we could make this API take a struct acpi_device * instead, but
 * for now, it's more convenient to operate on an acpi_handle.
 */
int acpi_is_root_bridge(acpi_handle handle)
{
	int ret;
	struct acpi_device *device;

	ret = acpi_bus_get_device(handle, &device);
	if (ret)
		return 0;

	ret = acpi_match_device_ids(device, root_device_ids);
	if (ret)
		return 0;
	else
		return 1;
}
EXPORT_SYMBOL_GPL(acpi_is_root_bridge);

static acpi_status
get_root_bridge_busnr_callback(struct acpi_resource *resource, void *data)
{
	struct resource *res = data;
	struct acpi_resource_address64 address;
<<<<<<< HEAD

	if (resource->type != ACPI_RESOURCE_TYPE_ADDRESS16 &&
	    resource->type != ACPI_RESOURCE_TYPE_ADDRESS32 &&
	    resource->type != ACPI_RESOURCE_TYPE_ADDRESS64)
		return AE_OK;

	acpi_resource_to_address64(resource, &address);
=======
	acpi_status status;

	status = acpi_resource_to_address64(resource, &address);
	if (ACPI_FAILURE(status))
		return AE_OK;

>>>>>>> refs/remotes/origin/master
	if ((address.address_length > 0) &&
	    (address.resource_type == ACPI_BUS_NUMBER_RANGE)) {
		res->start = address.minimum;
		res->end = address.minimum + address.address_length - 1;
	}

	return AE_OK;
}

static acpi_status try_get_root_bridge_busnr(acpi_handle handle,
					     struct resource *res)
{
	acpi_status status;

	res->start = -1;
	status =
	    acpi_walk_resources(handle, METHOD_NAME__CRS,
				get_root_bridge_busnr_callback, res);
	if (ACPI_FAILURE(status))
		return status;
	if (res->start == -1)
		return AE_ERROR;
	return AE_OK;
}

<<<<<<< HEAD
static void acpi_pci_bridge_scan(struct acpi_device *device)
{
	int status;
	struct acpi_device *child = NULL;

	if (device->flags.bus_address)
		if (device->parent && device->parent->ops.bind) {
			status = device->parent->ops.bind(device);
			if (!status) {
				list_for_each_entry(child, &device->children, node)
					acpi_pci_bridge_scan(child);
			}
		}
=======
struct pci_osc_bit_struct {
	u32 bit;
	char *desc;
};

static struct pci_osc_bit_struct pci_osc_support_bit[] = {
	{ OSC_PCI_EXT_CONFIG_SUPPORT, "ExtendedConfig" },
	{ OSC_PCI_ASPM_SUPPORT, "ASPM" },
	{ OSC_PCI_CLOCK_PM_SUPPORT, "ClockPM" },
	{ OSC_PCI_SEGMENT_GROUPS_SUPPORT, "Segments" },
	{ OSC_PCI_MSI_SUPPORT, "MSI" },
};

static struct pci_osc_bit_struct pci_osc_control_bit[] = {
	{ OSC_PCI_EXPRESS_NATIVE_HP_CONTROL, "PCIeHotplug" },
	{ OSC_PCI_SHPC_NATIVE_HP_CONTROL, "SHPCHotplug" },
	{ OSC_PCI_EXPRESS_PME_CONTROL, "PME" },
	{ OSC_PCI_EXPRESS_AER_CONTROL, "AER" },
	{ OSC_PCI_EXPRESS_CAPABILITY_CONTROL, "PCIeCapability" },
};

static void decode_osc_bits(struct acpi_pci_root *root, char *msg, u32 word,
			    struct pci_osc_bit_struct *table, int size)
{
	char buf[80];
	int i, len = 0;
	struct pci_osc_bit_struct *entry;

	buf[0] = '\0';
	for (i = 0, entry = table; i < size; i++, entry++)
		if (word & entry->bit)
			len += snprintf(buf + len, sizeof(buf) - len, "%s%s",
					len ? " " : "", entry->desc);

	dev_info(&root->device->dev, "_OSC: %s [%s]\n", msg, buf);
}

static void decode_osc_support(struct acpi_pci_root *root, char *msg, u32 word)
{
	decode_osc_bits(root, msg, word, pci_osc_support_bit,
			ARRAY_SIZE(pci_osc_support_bit));
}

static void decode_osc_control(struct acpi_pci_root *root, char *msg, u32 word)
{
	decode_osc_bits(root, msg, word, pci_osc_control_bit,
			ARRAY_SIZE(pci_osc_control_bit));
>>>>>>> refs/remotes/origin/master
}

static u8 pci_osc_uuid_str[] = "33DB4D5B-1FF7-401C-9657-7441C03DD766";

static acpi_status acpi_pci_run_osc(acpi_handle handle,
				    const u32 *capbuf, u32 *retval)
{
	struct acpi_osc_context context = {
		.uuid_str = pci_osc_uuid_str,
		.rev = 1,
		.cap.length = 12,
		.cap.pointer = (void *)capbuf,
	};
	acpi_status status;

	status = acpi_run_osc(handle, &context);
	if (ACPI_SUCCESS(status)) {
		*retval = *((u32 *)(context.ret.pointer + 8));
		kfree(context.ret.pointer);
	}
	return status;
}

static acpi_status acpi_pci_query_osc(struct acpi_pci_root *root,
					u32 support,
					u32 *control)
{
	acpi_status status;
	u32 result, capbuf[3];

	support &= OSC_PCI_SUPPORT_MASKS;
	support |= root->osc_support_set;

<<<<<<< HEAD
	capbuf[OSC_QUERY_TYPE] = OSC_QUERY_ENABLE;
	capbuf[OSC_SUPPORT_TYPE] = support;
	if (control) {
		*control &= OSC_PCI_CONTROL_MASKS;
		capbuf[OSC_CONTROL_TYPE] = *control | root->osc_control_set;
	} else {
		/* Run _OSC query only with existing controls. */
		capbuf[OSC_CONTROL_TYPE] = root->osc_control_set;
<<<<<<< HEAD
=======
	capbuf[OSC_QUERY_DWORD] = OSC_QUERY_ENABLE;
	capbuf[OSC_SUPPORT_DWORD] = support;
	if (control) {
		*control &= OSC_PCI_CONTROL_MASKS;
		capbuf[OSC_CONTROL_DWORD] = *control | root->osc_control_set;
	} else {
		/* Run _OSC query only with existing controls. */
		capbuf[OSC_CONTROL_DWORD] = root->osc_control_set;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	status = acpi_pci_run_osc(root->device->handle, capbuf, &result);
	if (ACPI_SUCCESS(status)) {
		root->osc_support_set = support;
		if (control)
			*control = result;
	}
	return status;
}

static acpi_status acpi_pci_osc_support(struct acpi_pci_root *root, u32 flags)
{
	acpi_status status;
<<<<<<< HEAD
	acpi_handle tmp;

	status = acpi_get_handle(root->device->handle, "_OSC", &tmp);
	if (ACPI_FAILURE(status))
		return status;
=======

>>>>>>> refs/remotes/origin/master
	mutex_lock(&osc_lock);
	status = acpi_pci_query_osc(root, flags, NULL);
	mutex_unlock(&osc_lock);
	return status;
}

struct acpi_pci_root *acpi_pci_find_root(acpi_handle handle)
{
	struct acpi_pci_root *root;
<<<<<<< HEAD

	list_for_each_entry(root, &acpi_pci_roots, node) {
		if (root->device->handle == handle)
			return root;
	}
	return NULL;
=======
	struct acpi_device *device;

	if (acpi_bus_get_device(handle, &device) ||
	    acpi_match_device_ids(device, root_device_ids))
		return NULL;

	root = acpi_driver_data(device);

	return root;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(acpi_pci_find_root);

struct acpi_handle_node {
	struct list_head node;
	acpi_handle handle;
};

/**
 * acpi_get_pci_dev - convert ACPI CA handle to struct pci_dev
 * @handle: the handle in question
 *
 * Given an ACPI CA handle, the desired PCI device is located in the
 * list of PCI devices.
 *
 * If the device is found, its reference count is increased and this
 * function returns a pointer to its data structure.  The caller must
 * decrement the reference count by calling pci_dev_put().
 * If no device is found, %NULL is returned.
 */
struct pci_dev *acpi_get_pci_dev(acpi_handle handle)
{
	int dev, fn;
	unsigned long long adr;
	acpi_status status;
	acpi_handle phandle;
	struct pci_bus *pbus;
	struct pci_dev *pdev = NULL;
	struct acpi_handle_node *node, *tmp;
	struct acpi_pci_root *root;
	LIST_HEAD(device_list);

	/*
	 * Walk up the ACPI CA namespace until we reach a PCI root bridge.
	 */
	phandle = handle;
	while (!acpi_is_root_bridge(phandle)) {
		node = kzalloc(sizeof(struct acpi_handle_node), GFP_KERNEL);
		if (!node)
			goto out;

		INIT_LIST_HEAD(&node->node);
		node->handle = phandle;
		list_add(&node->node, &device_list);

		status = acpi_get_parent(phandle, &phandle);
		if (ACPI_FAILURE(status))
			goto out;
	}

	root = acpi_pci_find_root(phandle);
	if (!root)
		goto out;

	pbus = root->bus;

	/*
	 * Now, walk back down the PCI device tree until we return to our
	 * original handle. Assumes that everything between the PCI root
	 * bridge and the device we're looking for must be a P2P bridge.
	 */
	list_for_each_entry(node, &device_list, node) {
		acpi_handle hnd = node->handle;
		status = acpi_evaluate_integer(hnd, "_ADR", NULL, &adr);
		if (ACPI_FAILURE(status))
			goto out;
		dev = (adr >> 16) & 0xffff;
		fn  = adr & 0xffff;

		pdev = pci_get_slot(pbus, PCI_DEVFN(dev, fn));
		if (!pdev || hnd == handle)
			break;

		pbus = pdev->subordinate;
		pci_dev_put(pdev);

		/*
		 * This function may be called for a non-PCI device that has a
		 * PCI parent (eg. a disk under a PCI SATA controller).  In that
		 * case pdev->subordinate will be NULL for the parent.
		 */
		if (!pbus) {
			dev_dbg(&pdev->dev, "Not a PCI-to-PCI bridge\n");
			pdev = NULL;
			break;
		}
	}
out:
	list_for_each_entry_safe(node, tmp, &device_list, node)
		kfree(node);

	return pdev;
}
EXPORT_SYMBOL_GPL(acpi_get_pci_dev);

/**
 * acpi_pci_osc_control_set - Request control of PCI root _OSC features.
 * @handle: ACPI handle of a PCI root bridge (or PCIe Root Complex).
 * @mask: Mask of _OSC bits to request control of, place to store control mask.
 * @req: Mask of _OSC bits the control of is essential to the caller.
 *
 * Run _OSC query for @mask and if that is successful, compare the returned
 * mask of control bits with @req.  If all of the @req bits are set in the
 * returned mask, run _OSC request for it.
 *
 * The variable at the @mask address may be modified regardless of whether or
 * not the function returns success.  On success it will contain the mask of
 * _OSC bits the BIOS has granted control of, but its contents are meaningless
 * on failure.
 **/
acpi_status acpi_pci_osc_control_set(acpi_handle handle, u32 *mask, u32 req)
{
	struct acpi_pci_root *root;
<<<<<<< HEAD
	acpi_status status;
	u32 ctrl, capbuf[3];
	acpi_handle tmp;
=======
	acpi_status status = AE_OK;
	u32 ctrl, capbuf[3];
>>>>>>> refs/remotes/origin/master

	if (!mask)
		return AE_BAD_PARAMETER;

	ctrl = *mask & OSC_PCI_CONTROL_MASKS;
	if ((ctrl & req) != req)
		return AE_TYPE;

	root = acpi_pci_find_root(handle);
	if (!root)
		return AE_NOT_EXIST;

<<<<<<< HEAD
	status = acpi_get_handle(handle, "_OSC", &tmp);
	if (ACPI_FAILURE(status))
		return status;

=======
>>>>>>> refs/remotes/origin/master
	mutex_lock(&osc_lock);

	*mask = ctrl | root->osc_control_set;
	/* No need to evaluate _OSC if the control was already granted. */
	if ((root->osc_control_set & ctrl) == ctrl)
		goto out;

	/* Need to check the available controls bits before requesting them. */
	while (*mask) {
		status = acpi_pci_query_osc(root, root->osc_support_set, mask);
		if (ACPI_FAILURE(status))
			goto out;
		if (ctrl == *mask)
			break;
<<<<<<< HEAD
=======
		decode_osc_control(root, "platform does not support",
				   ctrl & ~(*mask));
>>>>>>> refs/remotes/origin/master
		ctrl = *mask;
	}

	if ((ctrl & req) != req) {
<<<<<<< HEAD
=======
		decode_osc_control(root, "not requesting control; platform does not support",
				   req & ~(ctrl));
>>>>>>> refs/remotes/origin/master
		status = AE_SUPPORT;
		goto out;
	}

<<<<<<< HEAD
	capbuf[OSC_QUERY_TYPE] = 0;
	capbuf[OSC_SUPPORT_TYPE] = root->osc_support_set;
	capbuf[OSC_CONTROL_TYPE] = ctrl;
=======
	capbuf[OSC_QUERY_DWORD] = 0;
	capbuf[OSC_SUPPORT_DWORD] = root->osc_support_set;
	capbuf[OSC_CONTROL_DWORD] = ctrl;
>>>>>>> refs/remotes/origin/master
	status = acpi_pci_run_osc(handle, capbuf, mask);
	if (ACPI_SUCCESS(status))
		root->osc_control_set = *mask;
out:
	mutex_unlock(&osc_lock);
	return status;
}
EXPORT_SYMBOL(acpi_pci_osc_control_set);

<<<<<<< HEAD
static int __devinit acpi_pci_root_add(struct acpi_device *device)
=======
static void negotiate_os_control(struct acpi_pci_root *root, int *no_aspm,
				 int *clear_aspm)
{
	u32 support, control, requested;
	acpi_status status;
	struct acpi_device *device = root->device;
	acpi_handle handle = device->handle;

	/*
	 * All supported architectures that use ACPI have support for
	 * PCI domains, so we indicate this in _OSC support capabilities.
	 */
	support = OSC_PCI_SEGMENT_GROUPS_SUPPORT;
	if (pci_ext_cfg_avail())
		support |= OSC_PCI_EXT_CONFIG_SUPPORT;
	if (pcie_aspm_support_enabled())
		support |= OSC_PCI_ASPM_SUPPORT | OSC_PCI_CLOCK_PM_SUPPORT;
	if (pci_msi_enabled())
		support |= OSC_PCI_MSI_SUPPORT;

	decode_osc_support(root, "OS supports", support);
	status = acpi_pci_osc_support(root, support);
	if (ACPI_FAILURE(status)) {
		dev_info(&device->dev, "_OSC failed (%s); disabling ASPM\n",
			 acpi_format_exception(status));
		*no_aspm = 1;
		return;
	}

	if (pcie_ports_disabled) {
		dev_info(&device->dev, "PCIe port services disabled; not requesting _OSC control\n");
		return;
	}

	if ((support & ACPI_PCIE_REQ_SUPPORT) != ACPI_PCIE_REQ_SUPPORT) {
		decode_osc_support(root, "not requesting OS control; OS requires",
				   ACPI_PCIE_REQ_SUPPORT);
		return;
	}

	control = OSC_PCI_EXPRESS_CAPABILITY_CONTROL
		| OSC_PCI_EXPRESS_NATIVE_HP_CONTROL
		| OSC_PCI_EXPRESS_PME_CONTROL;

	if (pci_aer_available()) {
		if (aer_acpi_firmware_first())
			dev_info(&device->dev,
				 "PCIe AER handled by firmware\n");
		else
			control |= OSC_PCI_EXPRESS_AER_CONTROL;
	}

	requested = control;
	status = acpi_pci_osc_control_set(handle, &control,
					  OSC_PCI_EXPRESS_CAPABILITY_CONTROL);
	if (ACPI_SUCCESS(status)) {
		decode_osc_control(root, "OS now controls", control);
		if (acpi_gbl_FADT.boot_flags & ACPI_FADT_NO_ASPM) {
			/*
			 * We have ASPM control, but the FADT indicates
			 * that it's unsupported. Clear it.
			 */
			*clear_aspm = 1;
		}
	} else {
		decode_osc_control(root, "OS requested", requested);
		decode_osc_control(root, "platform willing to grant", control);
		dev_info(&device->dev, "_OSC failed (%s); disabling ASPM\n",
			acpi_format_exception(status));

		/*
		 * We want to disable ASPM here, but aspm_disabled
		 * needs to remain in its state from boot so that we
		 * properly handle PCIe 1.1 devices.  So we set this
		 * flag here, to defer the action until after the ACPI
		 * root scan.
		 */
		*no_aspm = 1;
	}
}

static int acpi_pci_root_add(struct acpi_device *device,
			     const struct acpi_device_id *not_used)
>>>>>>> refs/remotes/origin/master
{
	unsigned long long segment, bus;
	acpi_status status;
	int result;
	struct acpi_pci_root *root;
<<<<<<< HEAD
	acpi_handle handle;
	struct acpi_device *child;
	u32 flags, base_flags;
=======
	acpi_handle handle = device->handle;
	int no_aspm = 0, clear_aspm = 0;
>>>>>>> refs/remotes/origin/master

	root = kzalloc(sizeof(struct acpi_pci_root), GFP_KERNEL);
	if (!root)
		return -ENOMEM;

	segment = 0;
<<<<<<< HEAD
	status = acpi_evaluate_integer(device->handle, METHOD_NAME__SEG, NULL,
				       &segment);
	if (ACPI_FAILURE(status) && status != AE_NOT_FOUND) {
		printk(KERN_ERR PREFIX "can't evaluate _SEG\n");
=======
	status = acpi_evaluate_integer(handle, METHOD_NAME__SEG, NULL,
				       &segment);
	if (ACPI_FAILURE(status) && status != AE_NOT_FOUND) {
		dev_err(&device->dev,  "can't evaluate _SEG\n");
>>>>>>> refs/remotes/origin/master
		result = -ENODEV;
		goto end;
	}

	/* Check _CRS first, then _BBN.  If no _BBN, default to zero. */
	root->secondary.flags = IORESOURCE_BUS;
<<<<<<< HEAD
	status = try_get_root_bridge_busnr(device->handle, &root->secondary);
=======
	status = try_get_root_bridge_busnr(handle, &root->secondary);
>>>>>>> refs/remotes/origin/master
	if (ACPI_FAILURE(status)) {
		/*
		 * We need both the start and end of the downstream bus range
		 * to interpret _CBA (MMCONFIG base address), so it really is
		 * supposed to be in _CRS.  If we don't find it there, all we
		 * can do is assume [_BBN-0xFF] or [0-0xFF].
		 */
		root->secondary.end = 0xFF;
<<<<<<< HEAD
		printk(KERN_WARNING FW_BUG PREFIX
		       "no secondary bus range in _CRS\n");
<<<<<<< HEAD
		status = acpi_evaluate_integer(device->handle, METHOD_NAME__BBN,					       NULL, &bus);
=======
		status = acpi_evaluate_integer(device->handle, METHOD_NAME__BBN,
					       NULL, &bus);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_warn(&device->dev,
			 FW_BUG "no secondary bus range in _CRS\n");
		status = acpi_evaluate_integer(handle, METHOD_NAME__BBN,
					       NULL, &bus);
>>>>>>> refs/remotes/origin/master
		if (ACPI_SUCCESS(status))
			root->secondary.start = bus;
		else if (status == AE_NOT_FOUND)
			root->secondary.start = 0;
		else {
<<<<<<< HEAD
			printk(KERN_ERR PREFIX "can't evaluate _BBN\n");
=======
			dev_err(&device->dev, "can't evaluate _BBN\n");
>>>>>>> refs/remotes/origin/master
			result = -ENODEV;
			goto end;
		}
	}

<<<<<<< HEAD
	INIT_LIST_HEAD(&root->node);
=======
>>>>>>> refs/remotes/origin/master
	root->device = device;
	root->segment = segment & 0xFFFF;
	strcpy(acpi_device_name(device), ACPI_PCI_ROOT_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_PCI_ROOT_CLASS);
	device->driver_data = root;

<<<<<<< HEAD
	/*
	 * All supported architectures that use ACPI have support for
	 * PCI domains, so we indicate this in _OSC support capabilities.
	 */
	flags = base_flags = OSC_PCI_SEGMENT_GROUPS_SUPPORT;
	acpi_pci_osc_support(root, flags);
=======
	pr_info(PREFIX "%s [%s] (domain %04x %pR)\n",
	       acpi_device_name(device), acpi_device_bid(device),
	       root->segment, &root->secondary);

	root->mcfg_addr = acpi_pci_root_get_mcfg_addr(handle);

	negotiate_os_control(root, &no_aspm, &clear_aspm);
>>>>>>> refs/remotes/origin/master

	/*
	 * TBD: Need PCI interface for enumeration/configuration of roots.
	 */

<<<<<<< HEAD
	/* TBD: Locking */
	list_add_tail(&root->node, &acpi_pci_roots);

	printk(KERN_INFO PREFIX "%s [%s] (domain %04x %pR)\n",
	       acpi_device_name(device), acpi_device_bid(device),
	       root->segment, &root->secondary);

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Scan the Root Bridge
	 * --------------------
	 * Must do this prior to any attempt to bind the root device, as the
<<<<<<< HEAD
	 * PCI namespace does not get created until this call is made (and 
=======
	 * PCI namespace does not get created until this call is made (and
>>>>>>> refs/remotes/origin/master
	 * thus the root bridge's pci_dev does not exist).
	 */
	root->bus = pci_acpi_scan_root(root);
	if (!root->bus) {
<<<<<<< HEAD
		printk(KERN_ERR PREFIX
			    "Bus %04x:%02x not present in PCI namespace\n",
			    root->segment, (unsigned int)root->secondary.start);
=======
		dev_err(&device->dev,
			"Bus %04x:%02x not present in PCI namespace\n",
			root->segment, (unsigned int)root->secondary.start);
		device->driver_data = NULL;
>>>>>>> refs/remotes/origin/master
		result = -ENODEV;
		goto end;
	}

<<<<<<< HEAD
	/*
	 * Attach ACPI-PCI Context
	 * -----------------------
	 * Thus binding the ACPI and PCI devices.
	 */
	result = acpi_pci_bind_root(device);
	if (result)
		goto end;

	/*
	 * PCI Routing Table
	 * -----------------
	 * Evaluate and parse _PRT, if exists.
	 */
	status = acpi_get_handle(device->handle, METHOD_NAME__PRT, &handle);
	if (ACPI_SUCCESS(status))
		result = acpi_pci_irq_add_prt(device->handle, root->bus);

	/*
	 * Scan and bind all _ADR-Based Devices
	 */
	list_for_each_entry(child, &device->children, node)
		acpi_pci_bridge_scan(child);

	/* Indicate support for various _OSC capabilities. */
	if (pci_ext_cfg_avail(root->bus->self))
		flags |= OSC_EXT_PCI_CONFIG_SUPPORT;
	if (pcie_aspm_support_enabled())
		flags |= OSC_ACTIVE_STATE_PWR_SUPPORT |
			OSC_CLOCK_PWR_CAPABILITY_SUPPORT;
	if (pci_msi_enabled())
		flags |= OSC_MSI_SUPPORT;
	if (flags != base_flags)
		acpi_pci_osc_support(root, flags);

	if (!pcie_ports_disabled
	    && (flags & ACPI_PCIE_REQ_SUPPORT) == ACPI_PCIE_REQ_SUPPORT) {
		flags = OSC_PCI_EXPRESS_CAP_STRUCTURE_CONTROL
			| OSC_PCI_EXPRESS_NATIVE_HP_CONTROL
			| OSC_PCI_EXPRESS_PME_CONTROL;

		if (pci_aer_available()) {
			if (aer_acpi_firmware_first())
				dev_dbg(root->bus->bridge,
					"PCIe errors handled by BIOS.\n");
			else
				flags |= OSC_PCI_EXPRESS_AER_CONTROL;
		}

		dev_info(root->bus->bridge,
			"Requesting ACPI _OSC control (0x%02x)\n", flags);

		status = acpi_pci_osc_control_set(device->handle, &flags,
					OSC_PCI_EXPRESS_CAP_STRUCTURE_CONTROL);
		if (ACPI_SUCCESS(status)) {
			dev_info(root->bus->bridge,
				"ACPI _OSC control (0x%02x) granted\n", flags);
			if (acpi_gbl_FADT.boot_flags & ACPI_FADT_NO_ASPM) {
				/*
				 * We have ASPM control, but the FADT indicates
				 * that it's unsupported. Clear it.
				 */
				pcie_clear_aspm(root->bus);
			}
		} else {
			dev_info(root->bus->bridge,
				"ACPI _OSC request failed (%s), "
				"returned control mask: 0x%02x\n",
				acpi_format_exception(status), flags);
			pr_info("ACPI _OSC control for PCIe not granted, "
				"disabling ASPM\n");
			pcie_no_aspm();
		}
	} else {
		dev_info(root->bus->bridge,
			 "Unable to request _OSC control "
			 "(_OSC support mask: 0x%02x)\n", flags);
	}
=======
	if (clear_aspm) {
		dev_info(&device->dev, "Disabling ASPM (FADT indicates it is unsupported)\n");
		pcie_clear_aspm(root->bus);
	}
	if (no_aspm)
		pcie_no_aspm();
>>>>>>> refs/remotes/origin/master

	pci_acpi_add_bus_pm_notifier(device, root->bus);
	if (device->wakeup.flags.run_wake)
		device_set_run_wake(root->bus->bridge, true);

<<<<<<< HEAD
	return 0;

end:
	if (!list_empty(&root->node))
		list_del(&root->node);
=======
	if (system_state != SYSTEM_BOOTING) {
		pcibios_resource_survey_bus(root->bus);
		pci_assign_unassigned_root_bus_resources(root->bus);
	}

	pci_bus_add_devices(root->bus);
	return 1;

end:
>>>>>>> refs/remotes/origin/master
	kfree(root);
	return result;
}

<<<<<<< HEAD
static int acpi_pci_root_start(struct acpi_device *device)
{
	struct acpi_pci_root *root = acpi_driver_data(device);

	pci_bus_add_devices(root->bus);
	return 0;
}

static int acpi_pci_root_remove(struct acpi_device *device, int type)
{
	struct acpi_pci_root *root = acpi_driver_data(device);
=======
static void acpi_pci_root_remove(struct acpi_device *device)
{
	struct acpi_pci_root *root = acpi_driver_data(device);

	pci_stop_root_bus(root->bus);
>>>>>>> refs/remotes/origin/master

	device_set_run_wake(root->bus->bridge, false);
	pci_acpi_remove_bus_pm_notifier(device);

<<<<<<< HEAD
	kfree(root);
	return 0;
}

static int __init acpi_pci_root_init(void)
{
	acpi_hest_init();

	if (acpi_pci_disabled)
		return 0;

	pci_acpi_crs_quirks();
	if (acpi_bus_register_driver(&acpi_pci_root_driver) < 0)
		return -ENODEV;

	return 0;
}

subsys_initcall(acpi_pci_root_init);
=======
	pci_remove_root_bus(root->bus);

	kfree(root);
}

void __init acpi_pci_root_init(void)
{
	acpi_hest_init();

	if (!acpi_pci_disabled) {
		pci_acpi_crs_quirks();
		acpi_scan_add_handler(&pci_root_handler);
	}
}
/* Support root bridge hotplug */

static void handle_root_bridge_insertion(acpi_handle handle)
{
	struct acpi_device *device;

	if (!acpi_bus_get_device(handle, &device)) {
		dev_printk(KERN_DEBUG, &device->dev,
			   "acpi device already exists; ignoring notify\n");
		return;
	}

	if (acpi_bus_scan(handle))
		acpi_handle_err(handle, "cannot add bridge to acpi list\n");
}

static void hotplug_event_root(void *data, u32 type)
{
	acpi_handle handle = data;
	struct acpi_pci_root *root;

	acpi_scan_lock_acquire();

	root = acpi_pci_find_root(handle);

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		/* bus enumerate */
		acpi_handle_printk(KERN_DEBUG, handle,
				   "Bus check notify on %s\n", __func__);
		if (root)
			acpiphp_check_host_bridge(handle);
		else
			handle_root_bridge_insertion(handle);

		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		/* device check */
		acpi_handle_printk(KERN_DEBUG, handle,
				   "Device check notify on %s\n", __func__);
		if (!root)
			handle_root_bridge_insertion(handle);
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* request device eject */
		acpi_handle_printk(KERN_DEBUG, handle,
				   "Device eject notify on %s\n", __func__);
		if (!root)
			break;

		get_device(&root->device->dev);

		acpi_scan_lock_release();

		acpi_bus_device_eject(root->device, ACPI_NOTIFY_EJECT_REQUEST);
		return;
	default:
		acpi_handle_warn(handle,
				 "notify_handler: unknown event type 0x%x\n",
				 type);
		break;
	}

	acpi_scan_lock_release();
}

static void handle_hotplug_event_root(acpi_handle handle, u32 type,
					void *context)
{
	acpi_hotplug_execute(hotplug_event_root, handle, type);
}

static acpi_status __init
find_root_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	acpi_status status;
	int *count = (int *)context;

	if (!acpi_is_root_bridge(handle))
		return AE_OK;

	(*count)++;

	status = acpi_install_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
					handle_hotplug_event_root, NULL);
	if (ACPI_FAILURE(status))
		acpi_handle_printk(KERN_DEBUG, handle,
			"notify handler is not installed, exit status: %u\n",
			 (unsigned int)status);
	else
		acpi_handle_printk(KERN_DEBUG, handle,
				   "notify handler is installed\n");

	return AE_OK;
}

void __init acpi_pci_root_hp_init(void)
{
	int num = 0;

	acpi_walk_namespace(ACPI_TYPE_DEVICE, ACPI_ROOT_OBJECT,
		ACPI_UINT32_MAX, find_root_bridges, NULL, &num, NULL);

	printk(KERN_DEBUG "Found %d acpi root devices\n", num);
}
>>>>>>> refs/remotes/origin/master
