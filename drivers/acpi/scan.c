/*
 * scan.c - support for transforming the ACPI namespace into individual objects
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/acpi.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/dmi.h>
<<<<<<< HEAD
=======
#include <linux/nls.h>
>>>>>>> refs/remotes/origin/master

#include <acpi/acpi_drivers.h>

#include "internal.h"

#define _COMPONENT		ACPI_BUS_COMPONENT
ACPI_MODULE_NAME("scan");
#define STRUCT_TO_INT(s)	(*((int*)&s))
extern struct acpi_device *acpi_root;

#define ACPI_BUS_CLASS			"system_bus"
#define ACPI_BUS_HID			"LNXSYBUS"
#define ACPI_BUS_DEVICE_NAME		"System Bus"

#define ACPI_IS_ROOT_DEVICE(device)    (!(device)->parent)

<<<<<<< HEAD
static const char *dummy_hid = "device";

static LIST_HEAD(acpi_device_list);
static LIST_HEAD(acpi_bus_id_list);
=======
/*
 * If set, devices will be hot-removed even if they cannot be put offline
 * gracefully (from the kernel's standpoint).
 */
bool acpi_force_hot_remove;

static const char *dummy_hid = "device";

static LIST_HEAD(acpi_bus_id_list);
static DEFINE_MUTEX(acpi_scan_lock);
static LIST_HEAD(acpi_scan_handlers_list);
>>>>>>> refs/remotes/origin/master
DEFINE_MUTEX(acpi_device_lock);
LIST_HEAD(acpi_wakeup_device_list);

struct acpi_device_bus_id{
	char bus_id[15];
	unsigned int instance_no;
	struct list_head node;
};

<<<<<<< HEAD
=======
void acpi_scan_lock_acquire(void)
{
	mutex_lock(&acpi_scan_lock);
}
EXPORT_SYMBOL_GPL(acpi_scan_lock_acquire);

void acpi_scan_lock_release(void)
{
	mutex_unlock(&acpi_scan_lock);
}
EXPORT_SYMBOL_GPL(acpi_scan_lock_release);

int acpi_scan_add_handler(struct acpi_scan_handler *handler)
{
	if (!handler || !handler->attach)
		return -EINVAL;

	list_add_tail(&handler->list_node, &acpi_scan_handlers_list);
	return 0;
}

int acpi_scan_add_handler_with_hotplug(struct acpi_scan_handler *handler,
				       const char *hotplug_profile_name)
{
	int error;

	error = acpi_scan_add_handler(handler);
	if (error)
		return error;

	acpi_sysfs_add_hotplug_profile(&handler->hotplug, hotplug_profile_name);
	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * Creates hid/cid(s) string needed for modalias and uevent
 * e.g. on a device with hid:IBM0001 and cid:ACPI0001 you get:
 * char *modalias: "acpi:IBM0001:ACPI0001"
*/
static int create_modalias(struct acpi_device *acpi_dev, char *modalias,
			   int size)
{
	int len;
	int count;
	struct acpi_hardware_id *id;

	if (list_empty(&acpi_dev->pnp.ids))
		return 0;

	len = snprintf(modalias, size, "acpi:");
	size -= len;

	list_for_each_entry(id, &acpi_dev->pnp.ids, list) {
		count = snprintf(&modalias[len], size, "%s:", id->id);
		if (count < 0 || count >= size)
			return -EINVAL;
		len += count;
		size -= count;
	}

	modalias[len] = '\0';
	return len;
}

static ssize_t
acpi_device_modalias_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	int len;

	/* Device has no HID and no CID or string is >1024 */
	len = create_modalias(acpi_dev, buf, 1024);
	if (len <= 0)
		return 0;
	buf[len++] = '\n';
	return len;
}
static DEVICE_ATTR(modalias, 0444, acpi_device_modalias_show, NULL);

<<<<<<< HEAD
static void acpi_bus_hot_remove_device(void *context)
{
	struct acpi_device *device;
	acpi_handle handle = context;
	struct acpi_object_list arg_list;
	union acpi_object arg;
	acpi_status status = AE_OK;

	if (acpi_bus_get_device(handle, &device))
		return;

	if (!device)
		return;

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
		"Hot-removing device %s...\n", dev_name(&device->dev)));

	if (acpi_bus_trim(device, 1)) {
		printk(KERN_ERR PREFIX
				"Removing device failed\n");
		return;
	}

	/* power off device */
	status = acpi_evaluate_object(handle, "_PS3", NULL, NULL);
	if (ACPI_FAILURE(status) && status != AE_NOT_FOUND)
		printk(KERN_WARNING PREFIX
				"Power-off device failed\n");

	if (device->flags.lockable) {
		arg_list.count = 1;
		arg_list.pointer = &arg;
		arg.type = ACPI_TYPE_INTEGER;
		arg.integer.value = 0;
		acpi_evaluate_object(handle, "_LCK", &arg_list, NULL);
	}

	arg_list.count = 1;
	arg_list.pointer = &arg;
	arg.type = ACPI_TYPE_INTEGER;
	arg.integer.value = 1;

	/*
	 * TBD: _EJD support.
	 */
	status = acpi_evaluate_object(handle, "_EJ0", &arg_list, NULL);
	if (ACPI_FAILURE(status))
		printk(KERN_WARNING PREFIX
				"Eject device failed\n");

	return;
}

=======
static acpi_status acpi_bus_offline(acpi_handle handle, u32 lvl, void *data,
				    void **ret_p)
{
	struct acpi_device *device = NULL;
	struct acpi_device_physical_node *pn;
	bool second_pass = (bool)data;
	acpi_status status = AE_OK;

	if (acpi_bus_get_device(handle, &device))
		return AE_OK;

	if (device->handler && !device->handler->hotplug.enabled) {
		*ret_p = &device->dev;
		return AE_SUPPORT;
	}

	mutex_lock(&device->physical_node_lock);

	list_for_each_entry(pn, &device->physical_node_list, node) {
		int ret;

		if (second_pass) {
			/* Skip devices offlined by the first pass. */
			if (pn->put_online)
				continue;
		} else {
			pn->put_online = false;
		}
		ret = device_offline(pn->dev);
		if (acpi_force_hot_remove)
			continue;

		if (ret >= 0) {
			pn->put_online = !ret;
		} else {
			*ret_p = pn->dev;
			if (second_pass) {
				status = AE_ERROR;
				break;
			}
		}
	}

	mutex_unlock(&device->physical_node_lock);

	return status;
}

static acpi_status acpi_bus_online(acpi_handle handle, u32 lvl, void *data,
				   void **ret_p)
{
	struct acpi_device *device = NULL;
	struct acpi_device_physical_node *pn;

	if (acpi_bus_get_device(handle, &device))
		return AE_OK;

	mutex_lock(&device->physical_node_lock);

	list_for_each_entry(pn, &device->physical_node_list, node)
		if (pn->put_online) {
			device_online(pn->dev);
			pn->put_online = false;
		}

	mutex_unlock(&device->physical_node_lock);

	return AE_OK;
}

static int acpi_scan_hot_remove(struct acpi_device *device)
{
	acpi_handle handle = device->handle;
	struct device *errdev;
	acpi_status status;
	unsigned long long sta;

	/* If there is no handle, the device node has been unregistered. */
	if (!handle) {
		dev_dbg(&device->dev, "ACPI handle missing\n");
		put_device(&device->dev);
		return -EINVAL;
	}

	/*
	 * Carry out two passes here and ignore errors in the first pass,
	 * because if the devices in question are memory blocks and
	 * CONFIG_MEMCG is set, one of the blocks may hold data structures
	 * that the other blocks depend on, but it is not known in advance which
	 * block holds them.
	 *
	 * If the first pass is successful, the second one isn't needed, though.
	 */
	errdev = NULL;
	status = acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				     NULL, acpi_bus_offline, (void *)false,
				     (void **)&errdev);
	if (status == AE_SUPPORT) {
		dev_warn(errdev, "Offline disabled.\n");
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_online, NULL, NULL, NULL);
		put_device(&device->dev);
		return -EPERM;
	}
	acpi_bus_offline(handle, 0, (void *)false, (void **)&errdev);
	if (errdev) {
		errdev = NULL;
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    NULL, acpi_bus_offline, (void *)true,
				    (void **)&errdev);
		if (!errdev || acpi_force_hot_remove)
			acpi_bus_offline(handle, 0, (void *)true,
					 (void **)&errdev);

		if (errdev && !acpi_force_hot_remove) {
			dev_warn(errdev, "Offline failed.\n");
			acpi_bus_online(handle, 0, NULL, NULL);
			acpi_walk_namespace(ACPI_TYPE_ANY, handle,
					    ACPI_UINT32_MAX, acpi_bus_online,
					    NULL, NULL, NULL);
			put_device(&device->dev);
			return -EBUSY;
		}
	}

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
		"Hot-removing device %s...\n", dev_name(&device->dev)));

	acpi_bus_trim(device);

	/* Device node has been unregistered. */
	put_device(&device->dev);
	device = NULL;

	acpi_evaluate_lck(handle, 0);
	/*
	 * TBD: _EJD support.
	 */
	status = acpi_evaluate_ej0(handle);
	if (status == AE_NOT_FOUND)
		return -ENODEV;
	else if (ACPI_FAILURE(status))
		return -EIO;

	/*
	 * Verify if eject was indeed successful.  If not, log an error
	 * message.  No need to call _OST since _EJ0 call was made OK.
	 */
	status = acpi_evaluate_integer(handle, "_STA", NULL, &sta);
	if (ACPI_FAILURE(status)) {
		acpi_handle_warn(handle,
			"Status check after eject failed (0x%x)\n", status);
	} else if (sta & ACPI_STA_DEVICE_ENABLED) {
		acpi_handle_warn(handle,
			"Eject incomplete - status 0x%llx\n", sta);
	}

	return 0;
}

void acpi_bus_device_eject(void *data, u32 ost_src)
{
	struct acpi_device *device = data;
	acpi_handle handle = device->handle;
	u32 ost_code = ACPI_OST_SC_NON_SPECIFIC_FAILURE;
	int error;

	lock_device_hotplug();
	mutex_lock(&acpi_scan_lock);

	if (ost_src == ACPI_NOTIFY_EJECT_REQUEST)
		acpi_evaluate_hotplug_ost(handle, ACPI_NOTIFY_EJECT_REQUEST,
					  ACPI_OST_SC_EJECT_IN_PROGRESS, NULL);

	if (device->handler && device->handler->hotplug.mode == AHM_CONTAINER)
		kobject_uevent(&device->dev.kobj, KOBJ_OFFLINE);

	error = acpi_scan_hot_remove(device);
	if (error == -EPERM) {
		goto err_support;
	} else if (error) {
		goto err_out;
	}

 out:
	mutex_unlock(&acpi_scan_lock);
	unlock_device_hotplug();
	return;

 err_support:
	ost_code = ACPI_OST_SC_EJECT_NOT_SUPPORTED;
 err_out:
	acpi_evaluate_hotplug_ost(handle, ost_src, ost_code, NULL);
	goto out;
}

static void acpi_scan_bus_device_check(void *data, u32 ost_source)
{
	acpi_handle handle = data;
	struct acpi_device *device = NULL;
	u32 ost_code = ACPI_OST_SC_NON_SPECIFIC_FAILURE;
	int error;

	lock_device_hotplug();
	mutex_lock(&acpi_scan_lock);

	if (ost_source != ACPI_NOTIFY_BUS_CHECK) {
		acpi_bus_get_device(handle, &device);
		if (device) {
			dev_warn(&device->dev, "Attempt to re-insert\n");
			goto out;
		}
	}
	error = acpi_bus_scan(handle);
	if (error) {
		acpi_handle_warn(handle, "Namespace scan failure\n");
		goto out;
	}
	error = acpi_bus_get_device(handle, &device);
	if (error) {
		acpi_handle_warn(handle, "Missing device node object\n");
		goto out;
	}
	ost_code = ACPI_OST_SC_SUCCESS;
	if (device->handler && device->handler->hotplug.mode == AHM_CONTAINER)
		kobject_uevent(&device->dev.kobj, KOBJ_ONLINE);

 out:
	acpi_evaluate_hotplug_ost(handle, ost_source, ost_code, NULL);
	mutex_unlock(&acpi_scan_lock);
	unlock_device_hotplug();
}

static void acpi_hotplug_unsupported(acpi_handle handle, u32 type)
{
	u32 ost_status;

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		acpi_handle_debug(handle,
			"ACPI_NOTIFY_BUS_CHECK event: unsupported\n");
		ost_status = ACPI_OST_SC_INSERT_NOT_SUPPORTED;
		break;
	case ACPI_NOTIFY_DEVICE_CHECK:
		acpi_handle_debug(handle,
			"ACPI_NOTIFY_DEVICE_CHECK event: unsupported\n");
		ost_status = ACPI_OST_SC_INSERT_NOT_SUPPORTED;
		break;
	case ACPI_NOTIFY_EJECT_REQUEST:
		acpi_handle_debug(handle,
			"ACPI_NOTIFY_EJECT_REQUEST event: unsupported\n");
		ost_status = ACPI_OST_SC_EJECT_NOT_SUPPORTED;
		break;
	default:
		/* non-hotplug event; possibly handled by other handler */
		return;
	}

	acpi_evaluate_hotplug_ost(handle, type, ost_status, NULL);
}

static void acpi_hotplug_notify_cb(acpi_handle handle, u32 type, void *data)
{
	struct acpi_scan_handler *handler = data;
	struct acpi_device *adev;
	acpi_status status;

	if (!handler->hotplug.enabled)
		return acpi_hotplug_unsupported(handle, type);

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		acpi_handle_debug(handle, "ACPI_NOTIFY_BUS_CHECK event\n");
		break;
	case ACPI_NOTIFY_DEVICE_CHECK:
		acpi_handle_debug(handle, "ACPI_NOTIFY_DEVICE_CHECK event\n");
		break;
	case ACPI_NOTIFY_EJECT_REQUEST:
		acpi_handle_debug(handle, "ACPI_NOTIFY_EJECT_REQUEST event\n");
		if (acpi_bus_get_device(handle, &adev))
			goto err_out;

		get_device(&adev->dev);
		status = acpi_hotplug_execute(acpi_bus_device_eject, adev, type);
		if (ACPI_SUCCESS(status))
			return;

		put_device(&adev->dev);
		goto err_out;
	default:
		/* non-hotplug event; possibly handled by other handler */
		return;
	}
	status = acpi_hotplug_execute(acpi_scan_bus_device_check, handle, type);
	if (ACPI_SUCCESS(status))
		return;

 err_out:
	acpi_evaluate_hotplug_ost(handle, type,
				  ACPI_OST_SC_NON_SPECIFIC_FAILURE, NULL);
}

static ssize_t real_power_state_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	struct acpi_device *adev = to_acpi_device(dev);
	int state;
	int ret;

	ret = acpi_device_get_power(adev, &state);
	if (ret)
		return ret;

	return sprintf(buf, "%s\n", acpi_power_state_string(state));
}

static DEVICE_ATTR(real_power_state, 0444, real_power_state_show, NULL);

static ssize_t power_state_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct acpi_device *adev = to_acpi_device(dev);

	return sprintf(buf, "%s\n", acpi_power_state_string(adev->power.state));
}

static DEVICE_ATTR(power_state, 0444, power_state_show, NULL);

>>>>>>> refs/remotes/origin/master
static ssize_t
acpi_eject_store(struct device *d, struct device_attribute *attr,
		const char *buf, size_t count)
{
<<<<<<< HEAD
	int ret = count;
	acpi_status status;
	acpi_object_type type = 0;
	struct acpi_device *acpi_device = to_acpi_device(d);

	if ((!count) || (buf[0] != '1')) {
		return -EINVAL;
	}
#ifndef FORCE_EJECT
	if (acpi_device->driver == NULL) {
		ret = -ENODEV;
		goto err;
	}
#endif
	status = acpi_get_type(acpi_device->handle, &type);
	if (ACPI_FAILURE(status) || (!acpi_device->flags.ejectable)) {
		ret = -ENODEV;
		goto err;
	}

	acpi_os_hotplug_execute(acpi_bus_hot_remove_device, acpi_device->handle);
err:
	return ret;
=======
	struct acpi_device *acpi_device = to_acpi_device(d);
	acpi_object_type not_used;
	acpi_status status;

	if (!count || buf[0] != '1')
		return -EINVAL;

	if ((!acpi_device->handler || !acpi_device->handler->hotplug.enabled)
	    && !acpi_device->driver)
		return -ENODEV;

	status = acpi_get_type(acpi_device->handle, &not_used);
	if (ACPI_FAILURE(status) || !acpi_device->flags.ejectable)
		return -ENODEV;

	acpi_evaluate_hotplug_ost(acpi_device->handle, ACPI_OST_EC_OSPM_EJECT,
				  ACPI_OST_SC_EJECT_IN_PROGRESS, NULL);
	get_device(&acpi_device->dev);
	status = acpi_hotplug_execute(acpi_bus_device_eject, acpi_device,
				      ACPI_OST_EC_OSPM_EJECT);
	if (ACPI_SUCCESS(status))
		return count;

	put_device(&acpi_device->dev);
	acpi_evaluate_hotplug_ost(acpi_device->handle, ACPI_OST_EC_OSPM_EJECT,
				  ACPI_OST_SC_NON_SPECIFIC_FAILURE, NULL);
	return status == AE_NO_MEMORY ? -ENOMEM : -EAGAIN;
>>>>>>> refs/remotes/origin/master
}

static DEVICE_ATTR(eject, 0200, NULL, acpi_eject_store);

static ssize_t
acpi_device_hid_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	return sprintf(buf, "%s\n", acpi_device_hid(acpi_dev));
}
static DEVICE_ATTR(hid, 0444, acpi_device_hid_show, NULL);

<<<<<<< HEAD
=======
static ssize_t acpi_device_uid_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	return sprintf(buf, "%s\n", acpi_dev->pnp.unique_id);
}
static DEVICE_ATTR(uid, 0444, acpi_device_uid_show, NULL);

static ssize_t acpi_device_adr_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	return sprintf(buf, "0x%08x\n",
		       (unsigned int)(acpi_dev->pnp.bus_address));
}
static DEVICE_ATTR(adr, 0444, acpi_device_adr_show, NULL);

>>>>>>> refs/remotes/origin/master
static ssize_t
acpi_device_path_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_buffer path = {ACPI_ALLOCATE_BUFFER, NULL};
	int result;

	result = acpi_get_name(acpi_dev->handle, ACPI_FULL_PATHNAME, &path);
	if (result)
		goto end;

	result = sprintf(buf, "%s\n", (char*)path.pointer);
	kfree(path.pointer);
end:
	return result;
}
static DEVICE_ATTR(path, 0444, acpi_device_path_show, NULL);

<<<<<<< HEAD
static int acpi_device_setup_files(struct acpi_device *dev)
{
	acpi_status status;
	acpi_handle temp;
=======
/* sysfs file that shows description text from the ACPI _STR method */
static ssize_t description_show(struct device *dev,
				struct device_attribute *attr,
				char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	int result;

	if (acpi_dev->pnp.str_obj == NULL)
		return 0;

	/*
	 * The _STR object contains a Unicode identifier for a device.
	 * We need to convert to utf-8 so it can be displayed.
	 */
	result = utf16s_to_utf8s(
		(wchar_t *)acpi_dev->pnp.str_obj->buffer.pointer,
		acpi_dev->pnp.str_obj->buffer.length,
		UTF16_LITTLE_ENDIAN, buf,
		PAGE_SIZE);

	buf[result++] = '\n';

	return result;
}
static DEVICE_ATTR(description, 0444, description_show, NULL);

static ssize_t
acpi_device_sun_show(struct device *dev, struct device_attribute *attr,
		     char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	return sprintf(buf, "%lu\n", acpi_dev->pnp.sun);
}
static DEVICE_ATTR(sun, 0444, acpi_device_sun_show, NULL);

static int acpi_device_setup_files(struct acpi_device *dev)
{
	struct acpi_buffer buffer = {ACPI_ALLOCATE_BUFFER, NULL};
	acpi_status status;
	unsigned long long sun;
>>>>>>> refs/remotes/origin/master
	int result = 0;

	/*
	 * Devices gotten from FADT don't have a "path" attribute
	 */
	if (dev->handle) {
		result = device_create_file(&dev->dev, &dev_attr_path);
		if (result)
			goto end;
	}

	if (!list_empty(&dev->pnp.ids)) {
		result = device_create_file(&dev->dev, &dev_attr_hid);
		if (result)
			goto end;

		result = device_create_file(&dev->dev, &dev_attr_modalias);
		if (result)
			goto end;
	}

<<<<<<< HEAD
=======
	/*
	 * If device has _STR, 'description' file is created
	 */
	if (acpi_has_method(dev->handle, "_STR")) {
		status = acpi_evaluate_object(dev->handle, "_STR",
					NULL, &buffer);
		if (ACPI_FAILURE(status))
			buffer.pointer = NULL;
		dev->pnp.str_obj = buffer.pointer;
		result = device_create_file(&dev->dev, &dev_attr_description);
		if (result)
			goto end;
	}

	if (dev->pnp.type.bus_address)
		result = device_create_file(&dev->dev, &dev_attr_adr);
	if (dev->pnp.unique_id)
		result = device_create_file(&dev->dev, &dev_attr_uid);

	status = acpi_evaluate_integer(dev->handle, "_SUN", NULL, &sun);
	if (ACPI_SUCCESS(status)) {
		dev->pnp.sun = (unsigned long)sun;
		result = device_create_file(&dev->dev, &dev_attr_sun);
		if (result)
			goto end;
	} else {
		dev->pnp.sun = (unsigned long)-1;
	}

>>>>>>> refs/remotes/origin/master
        /*
         * If device has _EJ0, 'eject' file is created that is used to trigger
         * hot-removal function from userland.
         */
<<<<<<< HEAD
	status = acpi_get_handle(dev->handle, "_EJ0", &temp);
	if (ACPI_SUCCESS(status))
		result = device_create_file(&dev->dev, &dev_attr_eject);
=======
	if (acpi_has_method(dev->handle, "_EJ0")) {
		result = device_create_file(&dev->dev, &dev_attr_eject);
		if (result)
			return result;
	}

	if (dev->flags.power_manageable) {
		result = device_create_file(&dev->dev, &dev_attr_power_state);
		if (result)
			return result;

		if (dev->power.flags.power_resources)
			result = device_create_file(&dev->dev,
						    &dev_attr_real_power_state);
	}

>>>>>>> refs/remotes/origin/master
end:
	return result;
}

static void acpi_device_remove_files(struct acpi_device *dev)
{
<<<<<<< HEAD
	acpi_status status;
	acpi_handle temp;

	/*
	 * If device has _EJ0, 'eject' file is created that is used to trigger
	 * hot-removal function from userland.
	 */
	status = acpi_get_handle(dev->handle, "_EJ0", &temp);
	if (ACPI_SUCCESS(status))
		device_remove_file(&dev->dev, &dev_attr_eject);

=======
	if (dev->flags.power_manageable) {
		device_remove_file(&dev->dev, &dev_attr_power_state);
		if (dev->power.flags.power_resources)
			device_remove_file(&dev->dev,
					   &dev_attr_real_power_state);
	}

	/*
	 * If device has _STR, remove 'description' file
	 */
	if (acpi_has_method(dev->handle, "_STR")) {
		kfree(dev->pnp.str_obj);
		device_remove_file(&dev->dev, &dev_attr_description);
	}
	/*
	 * If device has _EJ0, remove 'eject' file.
	 */
	if (acpi_has_method(dev->handle, "_EJ0"))
		device_remove_file(&dev->dev, &dev_attr_eject);

	if (acpi_has_method(dev->handle, "_SUN"))
		device_remove_file(&dev->dev, &dev_attr_sun);

	if (dev->pnp.unique_id)
		device_remove_file(&dev->dev, &dev_attr_uid);
	if (dev->pnp.type.bus_address)
		device_remove_file(&dev->dev, &dev_attr_adr);
>>>>>>> refs/remotes/origin/master
	device_remove_file(&dev->dev, &dev_attr_modalias);
	device_remove_file(&dev->dev, &dev_attr_hid);
	if (dev->handle)
		device_remove_file(&dev->dev, &dev_attr_path);
}
/* --------------------------------------------------------------------------
			ACPI Bus operations
   -------------------------------------------------------------------------- */

<<<<<<< HEAD
int acpi_match_device_ids(struct acpi_device *device,
			  const struct acpi_device_id *ids)
=======
static const struct acpi_device_id *__acpi_match_device(
	struct acpi_device *device, const struct acpi_device_id *ids)
>>>>>>> refs/remotes/origin/master
{
	const struct acpi_device_id *id;
	struct acpi_hardware_id *hwid;

	/*
	 * If the device is not present, it is unnecessary to load device
	 * driver for it.
	 */
	if (!device->status.present)
<<<<<<< HEAD
		return -ENODEV;
=======
		return NULL;
>>>>>>> refs/remotes/origin/master

	for (id = ids; id->id[0]; id++)
		list_for_each_entry(hwid, &device->pnp.ids, list)
			if (!strcmp((char *) id->id, hwid->id))
<<<<<<< HEAD
				return 0;

	return -ENOENT;
}
EXPORT_SYMBOL(acpi_match_device_ids);

static void acpi_free_ids(struct acpi_device *device)
{
	struct acpi_hardware_id *id, *tmp;

	list_for_each_entry_safe(id, tmp, &device->pnp.ids, list) {
		kfree(id->id);
		kfree(id);
	}
}

static void acpi_device_release(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	acpi_free_ids(acpi_dev);
	kfree(acpi_dev);
}

static int acpi_device_suspend(struct device *dev, pm_message_t state)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = acpi_dev->driver;

	if (acpi_drv && acpi_drv->ops.suspend)
		return acpi_drv->ops.suspend(acpi_dev, state);
	return 0;
}

static int acpi_device_resume(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = acpi_dev->driver;

	if (acpi_drv && acpi_drv->ops.resume)
		return acpi_drv->ops.resume(acpi_dev);
	return 0;
=======
				return id;

	return NULL;
}

/**
 * acpi_match_device - Match a struct device against a given list of ACPI IDs
 * @ids: Array of struct acpi_device_id object to match against.
 * @dev: The device structure to match.
 *
 * Check if @dev has a valid ACPI handle and if there is a struct acpi_device
 * object for that handle and use that object to match against a given list of
 * device IDs.
 *
 * Return a pointer to the first matching ID on success or %NULL on failure.
 */
const struct acpi_device_id *acpi_match_device(const struct acpi_device_id *ids,
					       const struct device *dev)
{
	struct acpi_device *adev;
	acpi_handle handle = ACPI_HANDLE(dev);

	if (!ids || !handle || acpi_bus_get_device(handle, &adev))
		return NULL;

	return __acpi_match_device(adev, ids);
}
EXPORT_SYMBOL_GPL(acpi_match_device);

int acpi_match_device_ids(struct acpi_device *device,
			  const struct acpi_device_id *ids)
{
	return __acpi_match_device(device, ids) ? 0 : -ENOENT;
}
EXPORT_SYMBOL(acpi_match_device_ids);

static void acpi_free_power_resources_lists(struct acpi_device *device)
{
	int i;

	if (device->wakeup.flags.valid)
		acpi_power_resources_list_free(&device->wakeup.resources);

	if (!device->flags.power_manageable)
		return;

	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++) {
		struct acpi_device_power_state *ps = &device->power.states[i];
		acpi_power_resources_list_free(&ps->resources);
	}
}

static void acpi_device_release(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	acpi_free_pnp_ids(&acpi_dev->pnp);
	acpi_free_power_resources_lists(acpi_dev);
	kfree(acpi_dev);
>>>>>>> refs/remotes/origin/master
}

static int acpi_bus_match(struct device *dev, struct device_driver *drv)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(drv);

<<<<<<< HEAD
	return !acpi_match_device_ids(acpi_dev, acpi_drv->ids);
=======
	return acpi_dev->flags.match_driver
		&& !acpi_match_device_ids(acpi_dev, acpi_drv->ids);
>>>>>>> refs/remotes/origin/master
}

static int acpi_device_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	int len;

	if (list_empty(&acpi_dev->pnp.ids))
		return 0;

	if (add_uevent_var(env, "MODALIAS="))
		return -ENOMEM;
	len = create_modalias(acpi_dev, &env->buf[env->buflen - 1],
			      sizeof(env->buf) - env->buflen);
	if (len >= (sizeof(env->buf) - env->buflen))
		return -ENOMEM;
	env->buflen += len;
	return 0;
}

static void acpi_device_notify(acpi_handle handle, u32 event, void *data)
{
	struct acpi_device *device = data;

	device->driver->ops.notify(device, event);
}

static acpi_status acpi_device_notify_fixed(void *data)
{
	struct acpi_device *device = data;

	/* Fixed hardware devices have no handles */
	acpi_device_notify(NULL, ACPI_FIXED_HARDWARE_EVENT, device);
	return AE_OK;
}

static int acpi_device_install_notify_handler(struct acpi_device *device)
{
	acpi_status status;

	if (device->device_type == ACPI_BUS_TYPE_POWER_BUTTON)
		status =
		    acpi_install_fixed_event_handler(ACPI_EVENT_POWER_BUTTON,
						     acpi_device_notify_fixed,
						     device);
	else if (device->device_type == ACPI_BUS_TYPE_SLEEP_BUTTON)
		status =
		    acpi_install_fixed_event_handler(ACPI_EVENT_SLEEP_BUTTON,
						     acpi_device_notify_fixed,
						     device);
	else
		status = acpi_install_notify_handler(device->handle,
						     ACPI_DEVICE_NOTIFY,
						     acpi_device_notify,
						     device);

	if (ACPI_FAILURE(status))
		return -EINVAL;
	return 0;
}

static void acpi_device_remove_notify_handler(struct acpi_device *device)
{
	if (device->device_type == ACPI_BUS_TYPE_POWER_BUTTON)
		acpi_remove_fixed_event_handler(ACPI_EVENT_POWER_BUTTON,
						acpi_device_notify_fixed);
	else if (device->device_type == ACPI_BUS_TYPE_SLEEP_BUTTON)
		acpi_remove_fixed_event_handler(ACPI_EVENT_SLEEP_BUTTON,
						acpi_device_notify_fixed);
	else
		acpi_remove_notify_handler(device->handle, ACPI_DEVICE_NOTIFY,
					   acpi_device_notify);
}

<<<<<<< HEAD
static int acpi_bus_driver_init(struct acpi_device *, struct acpi_driver *);
static int acpi_start_single_object(struct acpi_device *);
static int acpi_device_probe(struct device * dev)
=======
static int acpi_device_probe(struct device *dev)
>>>>>>> refs/remotes/origin/master
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(dev->driver);
	int ret;

<<<<<<< HEAD
	ret = acpi_bus_driver_init(acpi_dev, acpi_drv);
	if (!ret) {
		if (acpi_dev->bus_ops.acpi_op_start)
			acpi_start_single_object(acpi_dev);

		if (acpi_drv->ops.notify) {
			ret = acpi_device_install_notify_handler(acpi_dev);
			if (ret) {
				if (acpi_drv->ops.remove)
					acpi_drv->ops.remove(acpi_dev,
						     acpi_dev->removal_type);
				return ret;
			}
		}

		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			"Found driver [%s] for device [%s]\n",
			acpi_drv->name, acpi_dev->pnp.bus_id));
		get_device(dev);
	}
	return ret;
=======
	if (acpi_dev->handler)
		return -EINVAL;

	if (!acpi_drv->ops.add)
		return -ENOSYS;

	ret = acpi_drv->ops.add(acpi_dev);
	if (ret)
		return ret;

	acpi_dev->driver = acpi_drv;
	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "Driver [%s] successfully bound to device [%s]\n",
			  acpi_drv->name, acpi_dev->pnp.bus_id));

	if (acpi_drv->ops.notify) {
		ret = acpi_device_install_notify_handler(acpi_dev);
		if (ret) {
			if (acpi_drv->ops.remove)
				acpi_drv->ops.remove(acpi_dev);

			acpi_dev->driver = NULL;
			acpi_dev->driver_data = NULL;
			return ret;
		}
	}

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Found driver [%s] for device [%s]\n",
			  acpi_drv->name, acpi_dev->pnp.bus_id));
	get_device(dev);
	return 0;
>>>>>>> refs/remotes/origin/master
}

static int acpi_device_remove(struct device * dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = acpi_dev->driver;

	if (acpi_drv) {
		if (acpi_drv->ops.notify)
			acpi_device_remove_notify_handler(acpi_dev);
		if (acpi_drv->ops.remove)
<<<<<<< HEAD
			acpi_drv->ops.remove(acpi_dev, acpi_dev->removal_type);
=======
			acpi_drv->ops.remove(acpi_dev);
>>>>>>> refs/remotes/origin/master
	}
	acpi_dev->driver = NULL;
	acpi_dev->driver_data = NULL;

	put_device(dev);
	return 0;
}

struct bus_type acpi_bus_type = {
	.name		= "acpi",
<<<<<<< HEAD
	.suspend	= acpi_device_suspend,
	.resume		= acpi_device_resume,
=======
>>>>>>> refs/remotes/origin/master
	.match		= acpi_bus_match,
	.probe		= acpi_device_probe,
	.remove		= acpi_device_remove,
	.uevent		= acpi_device_uevent,
};

<<<<<<< HEAD
static int acpi_device_register(struct acpi_device *device)
=======
static void acpi_bus_data_handler(acpi_handle handle, void *context)
{
	/* Intentionally empty. */
}

int acpi_bus_get_device(acpi_handle handle, struct acpi_device **device)
{
	acpi_status status;

	if (!device)
		return -EINVAL;

	status = acpi_get_data(handle, acpi_bus_data_handler, (void **)device);
	if (ACPI_FAILURE(status) || !*device) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "No context for object [%p]\n",
				  handle));
		return -ENODEV;
	}
	return 0;
}
EXPORT_SYMBOL(acpi_bus_get_device);

int acpi_device_add(struct acpi_device *device,
		    void (*release)(struct device *))
>>>>>>> refs/remotes/origin/master
{
	int result;
	struct acpi_device_bus_id *acpi_device_bus_id, *new_bus_id;
	int found = 0;

<<<<<<< HEAD
=======
	if (device->handle) {
		acpi_status status;

		status = acpi_attach_data(device->handle, acpi_bus_data_handler,
					  device);
		if (ACPI_FAILURE(status)) {
			acpi_handle_err(device->handle,
					"Unable to attach device data\n");
			return -ENODEV;
		}
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * Linkage
	 * -------
	 * Link this device to its parent and siblings.
	 */
	INIT_LIST_HEAD(&device->children);
	INIT_LIST_HEAD(&device->node);
	INIT_LIST_HEAD(&device->wakeup_list);
<<<<<<< HEAD

	new_bus_id = kzalloc(sizeof(struct acpi_device_bus_id), GFP_KERNEL);
	if (!new_bus_id) {
		printk(KERN_ERR PREFIX "Memory allocation error\n");
		return -ENOMEM;
=======
	INIT_LIST_HEAD(&device->physical_node_list);
	mutex_init(&device->physical_node_lock);

	new_bus_id = kzalloc(sizeof(struct acpi_device_bus_id), GFP_KERNEL);
	if (!new_bus_id) {
		pr_err(PREFIX "Memory allocation error\n");
		result = -ENOMEM;
		goto err_detach;
>>>>>>> refs/remotes/origin/master
	}

	mutex_lock(&acpi_device_lock);
	/*
	 * Find suitable bus_id and instance number in acpi_bus_id_list
	 * If failed, create one and link it into acpi_bus_id_list
	 */
	list_for_each_entry(acpi_device_bus_id, &acpi_bus_id_list, node) {
		if (!strcmp(acpi_device_bus_id->bus_id,
			    acpi_device_hid(device))) {
			acpi_device_bus_id->instance_no++;
			found = 1;
			kfree(new_bus_id);
			break;
		}
	}
	if (!found) {
		acpi_device_bus_id = new_bus_id;
		strcpy(acpi_device_bus_id->bus_id, acpi_device_hid(device));
		acpi_device_bus_id->instance_no = 0;
		list_add_tail(&acpi_device_bus_id->node, &acpi_bus_id_list);
	}
	dev_set_name(&device->dev, "%s:%02x", acpi_device_bus_id->bus_id, acpi_device_bus_id->instance_no);

	if (device->parent)
		list_add_tail(&device->node, &device->parent->children);

	if (device->wakeup.flags.valid)
		list_add_tail(&device->wakeup_list, &acpi_wakeup_device_list);
	mutex_unlock(&acpi_device_lock);

	if (device->parent)
		device->dev.parent = &device->parent->dev;
	device->dev.bus = &acpi_bus_type;
<<<<<<< HEAD
	device->dev.release = &acpi_device_release;
	result = device_register(&device->dev);
	if (result) {
		dev_err(&device->dev, "Error registering device\n");
		goto end;
=======
	device->dev.release = release;
	result = device_add(&device->dev);
	if (result) {
		dev_err(&device->dev, "Error registering device\n");
		goto err;
>>>>>>> refs/remotes/origin/master
	}

	result = acpi_device_setup_files(device);
	if (result)
		printk(KERN_ERR PREFIX "Error creating sysfs interface for device %s\n",
		       dev_name(&device->dev));

<<<<<<< HEAD
	device->removal_type = ACPI_BUS_REMOVAL_NORMAL;
	return 0;
end:
=======
	return 0;

 err:
>>>>>>> refs/remotes/origin/master
	mutex_lock(&acpi_device_lock);
	if (device->parent)
		list_del(&device->node);
	list_del(&device->wakeup_list);
	mutex_unlock(&acpi_device_lock);
<<<<<<< HEAD
	return result;
}

static void acpi_device_unregister(struct acpi_device *device, int type)
=======

 err_detach:
	acpi_detach_data(device->handle, acpi_bus_data_handler);
	return result;
}

static void acpi_device_unregister(struct acpi_device *device)
>>>>>>> refs/remotes/origin/master
{
	mutex_lock(&acpi_device_lock);
	if (device->parent)
		list_del(&device->node);

	list_del(&device->wakeup_list);
	mutex_unlock(&acpi_device_lock);

	acpi_detach_data(device->handle, acpi_bus_data_handler);

<<<<<<< HEAD
	acpi_device_remove_files(device);
	device_unregister(&device->dev);
}

/* --------------------------------------------------------------------------
                                 Driver Management
   -------------------------------------------------------------------------- */
/**
 * acpi_bus_driver_init - add a device to a driver
 * @device: the device to add and initialize
 * @driver: driver for the device
 *
 * Used to initialize a device via its device driver.  Called whenever a
 * driver is bound to a device.  Invokes the driver's add() ops.
 */
static int
acpi_bus_driver_init(struct acpi_device *device, struct acpi_driver *driver)
{
	int result = 0;

	if (!device || !driver)
		return -EINVAL;

	if (!driver->ops.add)
		return -ENOSYS;

	result = driver->ops.add(device);
	if (result) {
		device->driver = NULL;
		device->driver_data = NULL;
		return result;
	}

	device->driver = driver;

	/*
	 * TBD - Configuration Management: Assign resources to device based
	 * upon possible configuration and currently allocated resources.
	 */

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "Driver successfully bound to device\n"));
	return 0;
}

static int acpi_start_single_object(struct acpi_device *device)
{
	int result = 0;
	struct acpi_driver *driver;


	if (!(driver = device->driver))
		return 0;

	if (driver->ops.start) {
		result = driver->ops.start(device);
		if (result && driver->ops.remove)
			driver->ops.remove(device, ACPI_BUS_REMOVAL_NORMAL);
	}

	return result;
}

=======
	acpi_power_add_remove_device(device, false);
	acpi_device_remove_files(device);
	if (device->remove)
		device->remove(device);

	device_del(&device->dev);
	/*
	 * Transition the device to D3cold to drop the reference counts of all
	 * power resources the device depends on and turn off the ones that have
	 * no more references.
	 */
	acpi_device_set_power(device, ACPI_STATE_D3_COLD);
	device->handle = NULL;
	put_device(&device->dev);
}

/* --------------------------------------------------------------------------
                                 Driver Management
   -------------------------------------------------------------------------- */
>>>>>>> refs/remotes/origin/master
/**
 * acpi_bus_register_driver - register a driver with the ACPI bus
 * @driver: driver being registered
 *
 * Registers a driver with the ACPI bus.  Searches the namespace for all
 * devices that match the driver's criteria and binds.  Returns zero for
 * success or a negative error status for failure.
 */
int acpi_bus_register_driver(struct acpi_driver *driver)
{
	int ret;

	if (acpi_disabled)
		return -ENODEV;
	driver->drv.name = driver->name;
	driver->drv.bus = &acpi_bus_type;
	driver->drv.owner = driver->owner;

	ret = driver_register(&driver->drv);
	return ret;
}

EXPORT_SYMBOL(acpi_bus_register_driver);

/**
<<<<<<< HEAD
 * acpi_bus_unregister_driver - unregisters a driver with the APIC bus
=======
 * acpi_bus_unregister_driver - unregisters a driver with the ACPI bus
>>>>>>> refs/remotes/origin/master
 * @driver: driver to unregister
 *
 * Unregisters a driver with the ACPI bus.  Searches the namespace for all
 * devices that match the driver's criteria and unbinds.
 */
void acpi_bus_unregister_driver(struct acpi_driver *driver)
{
	driver_unregister(&driver->drv);
}

EXPORT_SYMBOL(acpi_bus_unregister_driver);

/* --------------------------------------------------------------------------
                                 Device Enumeration
   -------------------------------------------------------------------------- */
static struct acpi_device *acpi_bus_get_parent(acpi_handle handle)
{
<<<<<<< HEAD
	acpi_status status;
	int ret;
	struct acpi_device *device;
=======
	struct acpi_device *device = NULL;
	acpi_status status;
>>>>>>> refs/remotes/origin/master

	/*
	 * Fixed hardware devices do not appear in the namespace and do not
	 * have handles, but we fabricate acpi_devices for them, so we have
	 * to deal with them specially.
	 */
<<<<<<< HEAD
	if (handle == NULL)
=======
	if (!handle)
>>>>>>> refs/remotes/origin/master
		return acpi_root;

	do {
		status = acpi_get_parent(handle, &handle);
<<<<<<< HEAD
		if (status == AE_NULL_ENTRY)
			return NULL;
		if (ACPI_FAILURE(status))
			return acpi_root;

		ret = acpi_bus_get_device(handle, &device);
		if (ret == 0)
			return device;
	} while (1);
=======
		if (ACPI_FAILURE(status))
			return status == AE_NULL_ENTRY ? NULL : acpi_root;
	} while (acpi_bus_get_device(handle, &device));
	return device;
>>>>>>> refs/remotes/origin/master
}

acpi_status
acpi_bus_get_ejd(acpi_handle handle, acpi_handle *ejd)
{
	acpi_status status;
	acpi_handle tmp;
	struct acpi_buffer buffer = {ACPI_ALLOCATE_BUFFER, NULL};
	union acpi_object *obj;

	status = acpi_get_handle(handle, "_EJD", &tmp);
	if (ACPI_FAILURE(status))
		return status;

	status = acpi_evaluate_object(handle, "_EJD", NULL, &buffer);
	if (ACPI_SUCCESS(status)) {
		obj = buffer.pointer;
		status = acpi_get_handle(ACPI_ROOT_OBJECT, obj->string.pointer,
					 ejd);
		kfree(buffer.pointer);
	}
	return status;
}
EXPORT_SYMBOL_GPL(acpi_bus_get_ejd);

<<<<<<< HEAD
void acpi_bus_data_handler(acpi_handle handle, void *context)
{

	/* TBD */

	return;
}

static int acpi_bus_get_perf_flags(struct acpi_device *device)
{
	device->performance.state = ACPI_STATE_UNKNOWN;
	return 0;
}

static acpi_status
acpi_bus_extract_wakeup_device_power_package(acpi_handle handle,
					     struct acpi_device_wakeup *wakeup)
=======
static int acpi_bus_extract_wakeup_device_power_package(acpi_handle handle,
					struct acpi_device_wakeup *wakeup)
>>>>>>> refs/remotes/origin/master
{
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *package = NULL;
	union acpi_object *element = NULL;
	acpi_status status;
<<<<<<< HEAD
	int i = 0;

	if (!wakeup)
		return AE_BAD_PARAMETER;
=======
	int err = -ENODATA;

	if (!wakeup)
		return -EINVAL;

	INIT_LIST_HEAD(&wakeup->resources);
>>>>>>> refs/remotes/origin/master

	/* _PRW */
	status = acpi_evaluate_object(handle, "_PRW", NULL, &buffer);
	if (ACPI_FAILURE(status)) {
		ACPI_EXCEPTION((AE_INFO, status, "Evaluating _PRW"));
<<<<<<< HEAD
		return status;
=======
		return err;
>>>>>>> refs/remotes/origin/master
	}

	package = (union acpi_object *)buffer.pointer;

<<<<<<< HEAD
	if (!package || (package->package.count < 2)) {
		status = AE_BAD_DATA;
		goto out;
	}

	element = &(package->package.elements[0]);
	if (!element) {
		status = AE_BAD_DATA;
		goto out;
	}
=======
	if (!package || package->package.count < 2)
		goto out;

	element = &(package->package.elements[0]);
	if (!element)
		goto out;

>>>>>>> refs/remotes/origin/master
	if (element->type == ACPI_TYPE_PACKAGE) {
		if ((element->package.count < 2) ||
		    (element->package.elements[0].type !=
		     ACPI_TYPE_LOCAL_REFERENCE)
<<<<<<< HEAD
		    || (element->package.elements[1].type != ACPI_TYPE_INTEGER)) {
			status = AE_BAD_DATA;
			goto out;
		}
=======
		    || (element->package.elements[1].type != ACPI_TYPE_INTEGER))
			goto out;

>>>>>>> refs/remotes/origin/master
		wakeup->gpe_device =
		    element->package.elements[0].reference.handle;
		wakeup->gpe_number =
		    (u32) element->package.elements[1].integer.value;
	} else if (element->type == ACPI_TYPE_INTEGER) {
		wakeup->gpe_device = NULL;
		wakeup->gpe_number = element->integer.value;
	} else {
<<<<<<< HEAD
		status = AE_BAD_DATA;
=======
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	element = &(package->package.elements[1]);
<<<<<<< HEAD
	if (element->type != ACPI_TYPE_INTEGER) {
		status = AE_BAD_DATA;
		goto out;
	}
	wakeup->sleep_state = element->integer.value;

	if ((package->package.count - 2) > ACPI_MAX_HANDLES) {
		status = AE_NO_MEMORY;
		goto out;
	}
	wakeup->resources.count = package->package.count - 2;
	for (i = 0; i < wakeup->resources.count; i++) {
		element = &(package->package.elements[i + 2]);
		if (element->type != ACPI_TYPE_LOCAL_REFERENCE) {
			status = AE_BAD_DATA;
			goto out;
		}

		wakeup->resources.handles[i] = element->reference.handle;
	}

=======
	if (element->type != ACPI_TYPE_INTEGER)
		goto out;

	wakeup->sleep_state = element->integer.value;

	err = acpi_extract_power_resources(package, 2, &wakeup->resources);
	if (err)
		goto out;

	if (!list_empty(&wakeup->resources)) {
		int sleep_state;

		err = acpi_power_wakeup_list_init(&wakeup->resources,
						  &sleep_state);
		if (err) {
			acpi_handle_warn(handle, "Retrieving current states "
					 "of wakeup power resources failed\n");
			acpi_power_resources_list_free(&wakeup->resources);
			goto out;
		}
		if (sleep_state < wakeup->sleep_state) {
			acpi_handle_warn(handle, "Overriding _PRW sleep state "
					 "(S%d) by S%d from power resources\n",
					 (int)wakeup->sleep_state, sleep_state);
			wakeup->sleep_state = sleep_state;
		}
	}
>>>>>>> refs/remotes/origin/master
	acpi_setup_gpe_for_wake(handle, wakeup->gpe_device, wakeup->gpe_number);

 out:
	kfree(buffer.pointer);
<<<<<<< HEAD

	return status;
=======
	return err;
>>>>>>> refs/remotes/origin/master
}

static void acpi_bus_set_run_wake_flags(struct acpi_device *device)
{
	struct acpi_device_id button_device_ids[] = {
		{"PNP0C0C", 0},
		{"PNP0C0D", 0},
		{"PNP0C0E", 0},
		{"", 0},
	};
	acpi_status status;
	acpi_event_status event_status;

	device->wakeup.flags.notifier_present = 0;

	/* Power button, Lid switch always enable wakeup */
	if (!acpi_match_device_ids(device, button_device_ids)) {
		device->wakeup.flags.run_wake = 1;
		if (!acpi_match_device_ids(device, &button_device_ids[1])) {
			/* Do not use Lid/sleep button for S5 wakeup */
			if (device->wakeup.sleep_state == ACPI_STATE_S5)
				device->wakeup.sleep_state = ACPI_STATE_S4;
		}
		device_set_wakeup_capable(&device->dev, true);
		return;
	}

	status = acpi_get_gpe_status(device->wakeup.gpe_device,
					device->wakeup.gpe_number,
						&event_status);
	if (status == AE_OK)
		device->wakeup.flags.run_wake =
				!!(event_status & ACPI_EVENT_FLAG_HANDLE);
}

static void acpi_bus_get_wakeup_device_flags(struct acpi_device *device)
{
<<<<<<< HEAD
	acpi_handle temp;
	acpi_status status = 0;
	int psw_error;

	/* Presence of _PRW indicates wake capable */
	status = acpi_get_handle(device->handle, "_PRW", &temp);
	if (ACPI_FAILURE(status))
		return;

	status = acpi_bus_extract_wakeup_device_power_package(device->handle,
							      &device->wakeup);
	if (ACPI_FAILURE(status)) {
		ACPI_EXCEPTION((AE_INFO, status, "Extracting _PRW package"));
=======
	int err;

	/* Presence of _PRW indicates wake capable */
	if (!acpi_has_method(device->handle, "_PRW"))
		return;

	err = acpi_bus_extract_wakeup_device_power_package(device->handle,
							   &device->wakeup);
	if (err) {
		dev_err(&device->dev, "_PRW evaluation error: %d\n", err);
>>>>>>> refs/remotes/origin/master
		return;
	}

	device->wakeup.flags.valid = 1;
	device->wakeup.prepare_count = 0;
	acpi_bus_set_run_wake_flags(device);
	/* Call _PSW/_DSW object to disable its ability to wake the sleeping
	 * system for the ACPI device with the _PRW object.
	 * The _PSW object is depreciated in ACPI 3.0 and is replaced by _DSW.
	 * So it is necessary to call _DSW object first. Only when it is not
	 * present will the _PSW object used.
	 */
<<<<<<< HEAD
	psw_error = acpi_device_sleep_wake(device, 0, 0, 0);
	if (psw_error)
=======
	err = acpi_device_sleep_wake(device, 0, 0, 0);
	if (err)
>>>>>>> refs/remotes/origin/master
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				"error in _DSW or _PSW evaluation\n"));
}

<<<<<<< HEAD
static void acpi_bus_add_power_resource(acpi_handle handle);

static int acpi_bus_get_power_flags(struct acpi_device *device)
{
	acpi_status status = 0;
	acpi_handle handle = NULL;
	u32 i = 0;

=======
static void acpi_bus_init_power_state(struct acpi_device *device, int state)
{
	struct acpi_device_power_state *ps = &device->power.states[state];
	char pathname[5] = { '_', 'P', 'R', '0' + state, '\0' };
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	acpi_status status;

	INIT_LIST_HEAD(&ps->resources);

	/* Evaluate "_PRx" to get referenced power resources */
	status = acpi_evaluate_object(device->handle, pathname, NULL, &buffer);
	if (ACPI_SUCCESS(status)) {
		union acpi_object *package = buffer.pointer;

		if (buffer.length && package
		    && package->type == ACPI_TYPE_PACKAGE
		    && package->package.count) {
			int err = acpi_extract_power_resources(package, 0,
							       &ps->resources);
			if (!err)
				device->power.flags.power_resources = 1;
		}
		ACPI_FREE(buffer.pointer);
	}

	/* Evaluate "_PSx" to see if we can do explicit sets */
	pathname[2] = 'S';
	if (acpi_has_method(device->handle, pathname))
		ps->flags.explicit_set = 1;

	/*
	 * State is valid if there are means to put the device into it.
	 * D3hot is only valid if _PR3 present.
	 */
	if (!list_empty(&ps->resources)
	    || (ps->flags.explicit_set && state < ACPI_STATE_D3_HOT)) {
		ps->flags.valid = 1;
		ps->flags.os_accessible = 1;
	}

	ps->power = -1;		/* Unknown - driver assigned */
	ps->latency = -1;	/* Unknown - driver assigned */
}

static void acpi_bus_get_power_flags(struct acpi_device *device)
{
	u32 i;

	/* Presence of _PS0|_PR0 indicates 'power manageable' */
	if (!acpi_has_method(device->handle, "_PS0") &&
	    !acpi_has_method(device->handle, "_PR0"))
		return;

	device->flags.power_manageable = 1;
>>>>>>> refs/remotes/origin/master

	/*
	 * Power Management Flags
	 */
<<<<<<< HEAD
	status = acpi_get_handle(device->handle, "_PSC", &handle);
	if (ACPI_SUCCESS(status))
		device->power.flags.explicit_get = 1;
	status = acpi_get_handle(device->handle, "_IRC", &handle);
	if (ACPI_SUCCESS(status))
=======
	if (acpi_has_method(device->handle, "_PSC"))
		device->power.flags.explicit_get = 1;
	if (acpi_has_method(device->handle, "_IRC"))
>>>>>>> refs/remotes/origin/master
		device->power.flags.inrush_current = 1;

	/*
	 * Enumerate supported power management states
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3; i++) {
=======
	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++) {
>>>>>>> refs/remotes/origin/cm-10.0
		struct acpi_device_power_state *ps = &device->power.states[i];
		char object_name[5] = { '_', 'P', 'R', '0' + i, '\0' };

		/* Evaluate "_PRx" to se if power resources are referenced */
		acpi_evaluate_reference(device->handle, object_name, NULL,
					&ps->resources);
		if (ps->resources.count) {
			int j;

			device->power.flags.power_resources = 1;
<<<<<<< HEAD
			ps->flags.valid = 1;
=======
>>>>>>> refs/remotes/origin/cm-10.0
			for (j = 0; j < ps->resources.count; j++)
				acpi_bus_add_power_resource(ps->resources.handles[j]);
		}

		/* Evaluate "_PSx" to see if we can do explicit sets */
		object_name[2] = 'S';
		status = acpi_get_handle(device->handle, object_name, &handle);
<<<<<<< HEAD
		if (ACPI_SUCCESS(status)) {
			ps->flags.explicit_set = 1;
			ps->flags.valid = 1;
		}

		/* State is valid if we have some power control */
		if (ps->resources.count || ps->flags.explicit_set)
=======
		if (ACPI_SUCCESS(status))
			ps->flags.explicit_set = 1;

		/*
		 * State is valid if there are means to put the device into it.
		 * D3hot is only valid if _PR3 present.
		 */
		if (ps->resources.count ||
		    (ps->flags.explicit_set && i < ACPI_STATE_D3_HOT))
>>>>>>> refs/remotes/origin/cm-10.0
			ps->flags.valid = 1;

		ps->power = -1;	/* Unknown - driver assigned */
		ps->latency = -1;	/* Unknown - driver assigned */
	}
=======
	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++)
		acpi_bus_init_power_state(device, i);

	INIT_LIST_HEAD(&device->power.states[ACPI_STATE_D3_COLD].resources);
>>>>>>> refs/remotes/origin/master

	/* Set defaults for D0 and D3 states (always valid) */
	device->power.states[ACPI_STATE_D0].flags.valid = 1;
	device->power.states[ACPI_STATE_D0].power = 100;
<<<<<<< HEAD
	device->power.states[ACPI_STATE_D3].flags.valid = 1;
	device->power.states[ACPI_STATE_D3].power = 0;

<<<<<<< HEAD
=======
=======
	device->power.states[ACPI_STATE_D3_COLD].flags.valid = 1;
	device->power.states[ACPI_STATE_D3_COLD].power = 0;

>>>>>>> refs/remotes/origin/master
	/* Set D3cold's explicit_set flag if _PS3 exists. */
	if (device->power.states[ACPI_STATE_D3_HOT].flags.explicit_set)
		device->power.states[ACPI_STATE_D3_COLD].flags.explicit_set = 1;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	acpi_bus_init_power(device);

	return 0;
}

static int acpi_bus_get_flags(struct acpi_device *device)
{
	acpi_status status = AE_OK;
	acpi_handle temp = NULL;


	/* Presence of _STA indicates 'dynamic_status' */
	status = acpi_get_handle(device->handle, "_STA", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.dynamic_status = 1;

	/* Presence of _RMV indicates 'removable' */
	status = acpi_get_handle(device->handle, "_RMV", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.removable = 1;

	/* Presence of _EJD|_EJ0 indicates 'ejectable' */
	status = acpi_get_handle(device->handle, "_EJD", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.ejectable = 1;
	else {
		status = acpi_get_handle(device->handle, "_EJ0", &temp);
		if (ACPI_SUCCESS(status))
			device->flags.ejectable = 1;
	}

	/* Presence of _LCK indicates 'lockable' */
	status = acpi_get_handle(device->handle, "_LCK", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.lockable = 1;

	/* Power resources cannot be power manageable. */
	if (device->device_type == ACPI_BUS_TYPE_POWER)
		return 0;

	/* Presence of _PS0|_PR0 indicates 'power manageable' */
	status = acpi_get_handle(device->handle, "_PS0", &temp);
	if (ACPI_FAILURE(status))
		status = acpi_get_handle(device->handle, "_PR0", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.power_manageable = 1;

	/* TBD: Performance management */

	return 0;
=======
	/* Presence of _PS3 or _PRx means we can put the device into D3 cold */
	if (device->power.states[ACPI_STATE_D3_HOT].flags.explicit_set ||
			device->power.flags.power_resources)
		device->power.states[ACPI_STATE_D3_COLD].flags.os_accessible = 1;

	if (acpi_bus_init_power(device)) {
		acpi_free_power_resources_lists(device);
		device->flags.power_manageable = 0;
	}
}

static void acpi_bus_get_flags(struct acpi_device *device)
{
	/* Presence of _STA indicates 'dynamic_status' */
	if (acpi_has_method(device->handle, "_STA"))
		device->flags.dynamic_status = 1;

	/* Presence of _RMV indicates 'removable' */
	if (acpi_has_method(device->handle, "_RMV"))
		device->flags.removable = 1;

	/* Presence of _EJD|_EJ0 indicates 'ejectable' */
	if (acpi_has_method(device->handle, "_EJD") ||
	    acpi_has_method(device->handle, "_EJ0"))
		device->flags.ejectable = 1;
>>>>>>> refs/remotes/origin/master
}

static void acpi_device_get_busid(struct acpi_device *device)
{
	char bus_id[5] = { '?', 0 };
	struct acpi_buffer buffer = { sizeof(bus_id), bus_id };
	int i = 0;

	/*
	 * Bus ID
	 * ------
	 * The device's Bus ID is simply the object name.
	 * TBD: Shouldn't this value be unique (within the ACPI namespace)?
	 */
	if (ACPI_IS_ROOT_DEVICE(device)) {
		strcpy(device->pnp.bus_id, "ACPI");
		return;
	}

	switch (device->device_type) {
	case ACPI_BUS_TYPE_POWER_BUTTON:
		strcpy(device->pnp.bus_id, "PWRF");
		break;
	case ACPI_BUS_TYPE_SLEEP_BUTTON:
		strcpy(device->pnp.bus_id, "SLPF");
		break;
	default:
		acpi_get_name(device->handle, ACPI_SINGLE_NAME, &buffer);
		/* Clean up trailing underscores (if any) */
		for (i = 3; i > 1; i--) {
			if (bus_id[i] == '_')
				bus_id[i] = '\0';
			else
				break;
		}
		strcpy(device->pnp.bus_id, bus_id);
		break;
	}
}

/*
<<<<<<< HEAD
 * acpi_bay_match - see if a device is an ejectable driver bay
=======
 * acpi_ata_match - see if an acpi object is an ATA device
 *
 * If an acpi object has one of the ACPI ATA methods defined,
 * then we can safely call it an ATA device.
 */
bool acpi_ata_match(acpi_handle handle)
{
	return acpi_has_method(handle, "_GTF") ||
	       acpi_has_method(handle, "_GTM") ||
	       acpi_has_method(handle, "_STM") ||
	       acpi_has_method(handle, "_SDD");
}

/*
 * acpi_bay_match - see if an acpi object is an ejectable driver bay
>>>>>>> refs/remotes/origin/master
 *
 * If an acpi object is ejectable and has one of the ACPI ATA methods defined,
 * then we can safely call it an ejectable drive bay
 */
<<<<<<< HEAD
static int acpi_bay_match(struct acpi_device *device){
	acpi_status status;
	acpi_handle handle;
	acpi_handle tmp;
	acpi_handle phandle;

	handle = device->handle;

	status = acpi_get_handle(handle, "_EJ0", &tmp);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	if ((ACPI_SUCCESS(acpi_get_handle(handle, "_GTF", &tmp))) ||
		(ACPI_SUCCESS(acpi_get_handle(handle, "_GTM", &tmp))) ||
		(ACPI_SUCCESS(acpi_get_handle(handle, "_STM", &tmp))) ||
		(ACPI_SUCCESS(acpi_get_handle(handle, "_SDD", &tmp))))
		return 0;

	if (acpi_get_parent(handle, &phandle))
		return -ENODEV;

        if ((ACPI_SUCCESS(acpi_get_handle(phandle, "_GTF", &tmp))) ||
                (ACPI_SUCCESS(acpi_get_handle(phandle, "_GTM", &tmp))) ||
                (ACPI_SUCCESS(acpi_get_handle(phandle, "_STM", &tmp))) ||
                (ACPI_SUCCESS(acpi_get_handle(phandle, "_SDD", &tmp))))
                return 0;

	return -ENODEV;
}

/*
 * acpi_dock_match - see if a device has a _DCK method
 */
static int acpi_dock_match(struct acpi_device *device)
{
	acpi_handle tmp;
	return acpi_get_handle(device->handle, "_DCK", &tmp);
=======
bool acpi_bay_match(acpi_handle handle)
{
	acpi_handle phandle;

	if (!acpi_has_method(handle, "_EJ0"))
		return false;
	if (acpi_ata_match(handle))
		return true;
	if (ACPI_FAILURE(acpi_get_parent(handle, &phandle)))
		return false;

	return acpi_ata_match(phandle);
}

/*
 * acpi_dock_match - see if an acpi object has a _DCK method
 */
bool acpi_dock_match(acpi_handle handle)
{
	return acpi_has_method(handle, "_DCK");
>>>>>>> refs/remotes/origin/master
}

const char *acpi_device_hid(struct acpi_device *device)
{
	struct acpi_hardware_id *hid;

	if (list_empty(&device->pnp.ids))
		return dummy_hid;

	hid = list_first_entry(&device->pnp.ids, struct acpi_hardware_id, list);
	return hid->id;
}
EXPORT_SYMBOL(acpi_device_hid);

<<<<<<< HEAD
static void acpi_add_id(struct acpi_device *device, const char *dev_id)
=======
static void acpi_add_id(struct acpi_device_pnp *pnp, const char *dev_id)
>>>>>>> refs/remotes/origin/master
{
	struct acpi_hardware_id *id;

	id = kmalloc(sizeof(*id), GFP_KERNEL);
	if (!id)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	id->id = kmalloc(strlen(dev_id) + 1, GFP_KERNEL);
=======
	id->id = kstrdup(dev_id, GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	id->id = kstrdup(dev_id, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!id->id) {
		kfree(id);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	strcpy(id->id, dev_id);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	list_add_tail(&id->list, &device->pnp.ids);
=======
	list_add_tail(&id->list, &pnp->ids);
	pnp->type.hardware_id = 1;
>>>>>>> refs/remotes/origin/master
}

/*
 * Old IBM workstations have a DSDT bug wherein the SMBus object
 * lacks the SMBUS01 HID and the methods do not have the necessary "_"
 * prefix.  Work around this.
 */
<<<<<<< HEAD
static int acpi_ibm_smbus_match(struct acpi_device *device)
{
	acpi_handle h_dummy;
	struct acpi_buffer path = {ACPI_ALLOCATE_BUFFER, NULL};
	int result;

	if (!dmi_name_in_vendors("IBM"))
		return -ENODEV;

	/* Look for SMBS object */
	result = acpi_get_name(device->handle, ACPI_SINGLE_NAME, &path);
	if (result)
		return result;

	if (strcmp("SMBS", path.pointer)) {
		result = -ENODEV;
		goto out;
	}

	/* Does it have the necessary (but misnamed) methods? */
	result = -ENODEV;
	if (ACPI_SUCCESS(acpi_get_handle(device->handle, "SBI", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "SBR", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "SBW", &h_dummy)))
		result = 0;
out:
	kfree(path.pointer);
	return result;
}

static void acpi_device_set_id(struct acpi_device *device)
{
	acpi_status status;
	struct acpi_device_info *info;
	struct acpica_device_id_list *cid_list;
	int i;

	switch (device->device_type) {
	case ACPI_BUS_TYPE_DEVICE:
		if (ACPI_IS_ROOT_DEVICE(device)) {
			acpi_add_id(device, ACPI_SYSTEM_HID);
			break;
		}

		status = acpi_get_object_info(device->handle, &info);
		if (ACPI_FAILURE(status)) {
			printk(KERN_ERR PREFIX "%s: Error reading device info\n", __func__);
=======
static bool acpi_ibm_smbus_match(acpi_handle handle)
{
	char node_name[ACPI_PATH_SEGMENT_LENGTH];
	struct acpi_buffer path = { sizeof(node_name), node_name };

	if (!dmi_name_in_vendors("IBM"))
		return false;

	/* Look for SMBS object */
	if (ACPI_FAILURE(acpi_get_name(handle, ACPI_SINGLE_NAME, &path)) ||
	    strcmp("SMBS", path.pointer))
		return false;

	/* Does it have the necessary (but misnamed) methods? */
	if (acpi_has_method(handle, "SBI") &&
	    acpi_has_method(handle, "SBR") &&
	    acpi_has_method(handle, "SBW"))
		return true;

	return false;
}

static void acpi_set_pnp_ids(acpi_handle handle, struct acpi_device_pnp *pnp,
				int device_type)
{
	acpi_status status;
	struct acpi_device_info *info;
	struct acpi_pnp_device_id_list *cid_list;
	int i;

	switch (device_type) {
	case ACPI_BUS_TYPE_DEVICE:
		if (handle == ACPI_ROOT_OBJECT) {
			acpi_add_id(pnp, ACPI_SYSTEM_HID);
			break;
		}

		status = acpi_get_object_info(handle, &info);
		if (ACPI_FAILURE(status)) {
			pr_err(PREFIX "%s: Error reading device info\n",
					__func__);
>>>>>>> refs/remotes/origin/master
			return;
		}

		if (info->valid & ACPI_VALID_HID)
<<<<<<< HEAD
			acpi_add_id(device, info->hardware_id.string);
		if (info->valid & ACPI_VALID_CID) {
			cid_list = &info->compatible_id_list;
			for (i = 0; i < cid_list->count; i++)
				acpi_add_id(device, cid_list->ids[i].string);
		}
		if (info->valid & ACPI_VALID_ADR) {
			device->pnp.bus_address = info->address;
			device->flags.bus_address = 1;
		}
=======
			acpi_add_id(pnp, info->hardware_id.string);
		if (info->valid & ACPI_VALID_CID) {
			cid_list = &info->compatible_id_list;
			for (i = 0; i < cid_list->count; i++)
				acpi_add_id(pnp, cid_list->ids[i].string);
		}
		if (info->valid & ACPI_VALID_ADR) {
			pnp->bus_address = info->address;
			pnp->type.bus_address = 1;
		}
		if (info->valid & ACPI_VALID_UID)
			pnp->unique_id = kstrdup(info->unique_id.string,
							GFP_KERNEL);
>>>>>>> refs/remotes/origin/master

		kfree(info);

		/*
		 * Some devices don't reliably have _HIDs & _CIDs, so add
		 * synthetic HIDs to make sure drivers can find them.
		 */
<<<<<<< HEAD
		if (acpi_is_video_device(device))
			acpi_add_id(device, ACPI_VIDEO_HID);
		else if (ACPI_SUCCESS(acpi_bay_match(device)))
			acpi_add_id(device, ACPI_BAY_HID);
		else if (ACPI_SUCCESS(acpi_dock_match(device)))
			acpi_add_id(device, ACPI_DOCK_HID);
		else if (!acpi_ibm_smbus_match(device))
			acpi_add_id(device, ACPI_SMBUS_IBM_HID);
		else if (list_empty(&device->pnp.ids) &&
			 ACPI_IS_ROOT_DEVICE(device->parent)) {
			acpi_add_id(device, ACPI_BUS_HID); /* \_SB, LNXSYBUS */
			strcpy(device->pnp.device_name, ACPI_BUS_DEVICE_NAME);
			strcpy(device->pnp.device_class, ACPI_BUS_CLASS);
=======
		if (acpi_is_video_device(handle))
			acpi_add_id(pnp, ACPI_VIDEO_HID);
		else if (acpi_bay_match(handle))
			acpi_add_id(pnp, ACPI_BAY_HID);
		else if (acpi_dock_match(handle))
			acpi_add_id(pnp, ACPI_DOCK_HID);
		else if (acpi_ibm_smbus_match(handle))
			acpi_add_id(pnp, ACPI_SMBUS_IBM_HID);
		else if (list_empty(&pnp->ids) && handle == ACPI_ROOT_OBJECT) {
			acpi_add_id(pnp, ACPI_BUS_HID); /* \_SB, LNXSYBUS */
			strcpy(pnp->device_name, ACPI_BUS_DEVICE_NAME);
			strcpy(pnp->device_class, ACPI_BUS_CLASS);
>>>>>>> refs/remotes/origin/master
		}

		break;
	case ACPI_BUS_TYPE_POWER:
<<<<<<< HEAD
		acpi_add_id(device, ACPI_POWER_HID);
		break;
	case ACPI_BUS_TYPE_PROCESSOR:
		acpi_add_id(device, ACPI_PROCESSOR_OBJECT_HID);
		break;
	case ACPI_BUS_TYPE_THERMAL:
		acpi_add_id(device, ACPI_THERMAL_HID);
		break;
	case ACPI_BUS_TYPE_POWER_BUTTON:
		acpi_add_id(device, ACPI_BUTTON_HID_POWERF);
		break;
	case ACPI_BUS_TYPE_SLEEP_BUTTON:
		acpi_add_id(device, ACPI_BUTTON_HID_SLEEPF);
=======
		acpi_add_id(pnp, ACPI_POWER_HID);
		break;
	case ACPI_BUS_TYPE_PROCESSOR:
		acpi_add_id(pnp, ACPI_PROCESSOR_OBJECT_HID);
		break;
	case ACPI_BUS_TYPE_THERMAL:
		acpi_add_id(pnp, ACPI_THERMAL_HID);
		break;
	case ACPI_BUS_TYPE_POWER_BUTTON:
		acpi_add_id(pnp, ACPI_BUTTON_HID_POWERF);
		break;
	case ACPI_BUS_TYPE_SLEEP_BUTTON:
		acpi_add_id(pnp, ACPI_BUTTON_HID_SLEEPF);
>>>>>>> refs/remotes/origin/master
		break;
	}
}

<<<<<<< HEAD
static int acpi_device_set_context(struct acpi_device *device)
{
	acpi_status status;

	/*
	 * Context
	 * -------
	 * Attach this 'struct acpi_device' to the ACPI object.  This makes
	 * resolutions from handle->device very efficient.  Fixed hardware
	 * devices have no handles, so we skip them.
	 */
	if (!device->handle)
		return 0;

	status = acpi_attach_data(device->handle,
				  acpi_bus_data_handler, device);
	if (ACPI_SUCCESS(status))
		return 0;

	printk(KERN_ERR PREFIX "Error attaching device data\n");
	return -ENODEV;
}

static int acpi_bus_remove(struct acpi_device *dev, int rmdevice)
{
	if (!dev)
		return -EINVAL;

	dev->removal_type = ACPI_BUS_REMOVAL_EJECT;
	device_release_driver(&dev->dev);

	if (!rmdevice)
		return 0;

	/*
	 * unbind _ADR-Based Devices when hot removal
	 */
	if (dev->flags.bus_address) {
		if ((dev->parent) && (dev->parent->ops.unbind))
			dev->parent->ops.unbind(dev);
	}
	acpi_device_unregister(dev, ACPI_BUS_REMOVAL_EJECT);

	return 0;
=======
void acpi_free_pnp_ids(struct acpi_device_pnp *pnp)
{
	struct acpi_hardware_id *id, *tmp;

	list_for_each_entry_safe(id, tmp, &pnp->ids, list) {
		kfree(id->id);
		kfree(id);
	}
	kfree(pnp->unique_id);
}

void acpi_init_device_object(struct acpi_device *device, acpi_handle handle,
			     int type, unsigned long long sta)
{
	INIT_LIST_HEAD(&device->pnp.ids);
	device->device_type = type;
	device->handle = handle;
	device->parent = acpi_bus_get_parent(handle);
	STRUCT_TO_INT(device->status) = sta;
	acpi_device_get_busid(device);
	acpi_set_pnp_ids(handle, &device->pnp, type);
	acpi_bus_get_flags(device);
	device->flags.match_driver = false;
	device_initialize(&device->dev);
	dev_set_uevent_suppress(&device->dev, true);
}

void acpi_device_add_finalize(struct acpi_device *device)
{
	dev_set_uevent_suppress(&device->dev, false);
	kobject_uevent(&device->dev.kobj, KOBJ_ADD);
>>>>>>> refs/remotes/origin/master
}

static int acpi_add_single_object(struct acpi_device **child,
				  acpi_handle handle, int type,
<<<<<<< HEAD
				  unsigned long long sta,
				  struct acpi_bus_ops *ops)
=======
				  unsigned long long sta)
>>>>>>> refs/remotes/origin/master
{
	int result;
	struct acpi_device *device;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };

	device = kzalloc(sizeof(struct acpi_device), GFP_KERNEL);
	if (!device) {
		printk(KERN_ERR PREFIX "Memory allocation error\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	INIT_LIST_HEAD(&device->pnp.ids);
	device->device_type = type;
	device->handle = handle;
	device->parent = acpi_bus_get_parent(handle);
	device->bus_ops = *ops; /* workround for not call .start */
	STRUCT_TO_INT(device->status) = sta;

	acpi_device_get_busid(device);

	/*
	 * Flags
	 * -----
	 * Note that we only look for object handles -- cannot evaluate objects
	 * until we know the device is present and properly initialized.
	 */
	result = acpi_bus_get_flags(device);
	if (result)
		goto end;

	/*
	 * Initialize Device
	 * -----------------
	 * TBD: Synch with Core's enumeration/initialization process.
	 */
	acpi_device_set_id(device);

	/*
	 * Power Management
	 * ----------------
	 */
	if (device->flags.power_manageable) {
		result = acpi_bus_get_power_flags(device);
		if (result)
			goto end;
	}

	/*
	 * Wakeup device management
	 *-----------------------
	 */
	acpi_bus_get_wakeup_device_flags(device);

	/*
	 * Performance Management
	 * ----------------------
	 */
	if (device->flags.performance_manageable) {
		result = acpi_bus_get_perf_flags(device);
		if (result)
			goto end;
	}

	if ((result = acpi_device_set_context(device)))
		goto end;

	result = acpi_device_register(device);

	/*
	 * Bind _ADR-Based Devices when hot add
	 */
	if (device->flags.bus_address) {
		if (device->parent && device->parent->ops.bind)
			device->parent->ops.bind(device);
	}

end:
	if (!result) {
		acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			"Adding %s [%s] parent %s\n", dev_name(&device->dev),
			 (char *) buffer.pointer,
			 device->parent ? dev_name(&device->parent->dev) :
					  "(null)"));
		kfree(buffer.pointer);
		*child = device;
	} else
		acpi_device_release(&device->dev);

	return result;
}

#define ACPI_STA_DEFAULT (ACPI_STA_DEVICE_PRESENT | ACPI_STA_DEVICE_ENABLED | \
			  ACPI_STA_DEVICE_UI      | ACPI_STA_DEVICE_FUNCTIONING)

static void acpi_bus_add_power_resource(acpi_handle handle)
{
	struct acpi_bus_ops ops = {
		.acpi_op_add = 1,
		.acpi_op_start = 1,
	};
	struct acpi_device *device = NULL;

	acpi_bus_get_device(handle, &device);
	if (!device)
		acpi_add_single_object(&device, handle, ACPI_BUS_TYPE_POWER,
					ACPI_STA_DEFAULT, &ops);
=======
	acpi_init_device_object(device, handle, type, sta);
	acpi_bus_get_power_flags(device);
	acpi_bus_get_wakeup_device_flags(device);

	result = acpi_device_add(device, acpi_device_release);
	if (result) {
		acpi_device_release(&device->dev);
		return result;
	}

	acpi_power_add_remove_device(device, true);
	acpi_device_add_finalize(device);
	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Added %s [%s] parent %s\n",
		dev_name(&device->dev), (char *) buffer.pointer,
		device->parent ? dev_name(&device->parent->dev) : "(null)"));
	kfree(buffer.pointer);
	*child = device;
	return 0;
>>>>>>> refs/remotes/origin/master
}

static int acpi_bus_type_and_status(acpi_handle handle, int *type,
				    unsigned long long *sta)
{
	acpi_status status;
	acpi_object_type acpi_type;

	status = acpi_get_type(handle, &acpi_type);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	switch (acpi_type) {
	case ACPI_TYPE_ANY:		/* for ACPI_ROOT_OBJECT */
	case ACPI_TYPE_DEVICE:
		*type = ACPI_BUS_TYPE_DEVICE;
		status = acpi_bus_get_status_handle(handle, sta);
		if (ACPI_FAILURE(status))
			return -ENODEV;
		break;
	case ACPI_TYPE_PROCESSOR:
		*type = ACPI_BUS_TYPE_PROCESSOR;
		status = acpi_bus_get_status_handle(handle, sta);
		if (ACPI_FAILURE(status))
			return -ENODEV;
		break;
	case ACPI_TYPE_THERMAL:
		*type = ACPI_BUS_TYPE_THERMAL;
		*sta = ACPI_STA_DEFAULT;
		break;
	case ACPI_TYPE_POWER:
		*type = ACPI_BUS_TYPE_POWER;
		*sta = ACPI_STA_DEFAULT;
		break;
	default:
		return -ENODEV;
	}

	return 0;
}

<<<<<<< HEAD
static acpi_status acpi_bus_check_add(acpi_handle handle, u32 lvl,
				      void *context, void **return_value)
{
	struct acpi_bus_ops *ops = context;
	int type;
	unsigned long long sta;
	struct acpi_device *device;
	acpi_status status;
	int result;

=======
static bool acpi_scan_handler_matching(struct acpi_scan_handler *handler,
				       char *idstr,
				       const struct acpi_device_id **matchid)
{
	const struct acpi_device_id *devid;

	for (devid = handler->ids; devid->id[0]; devid++)
		if (!strcmp((char *)devid->id, idstr)) {
			if (matchid)
				*matchid = devid;

			return true;
		}

	return false;
}

static struct acpi_scan_handler *acpi_scan_match_handler(char *idstr,
					const struct acpi_device_id **matchid)
{
	struct acpi_scan_handler *handler;

	list_for_each_entry(handler, &acpi_scan_handlers_list, list_node)
		if (acpi_scan_handler_matching(handler, idstr, matchid))
			return handler;

	return NULL;
}

void acpi_scan_hotplug_enabled(struct acpi_hotplug_profile *hotplug, bool val)
{
	if (!!hotplug->enabled == !!val)
		return;

	mutex_lock(&acpi_scan_lock);

	hotplug->enabled = val;

	mutex_unlock(&acpi_scan_lock);
}

static void acpi_scan_init_hotplug(acpi_handle handle, int type)
{
	struct acpi_device_pnp pnp = {};
	struct acpi_hardware_id *hwid;
	struct acpi_scan_handler *handler;

	INIT_LIST_HEAD(&pnp.ids);
	acpi_set_pnp_ids(handle, &pnp, type);

	if (!pnp.type.hardware_id)
		goto out;

	/*
	 * This relies on the fact that acpi_install_notify_handler() will not
	 * install the same notify handler routine twice for the same handle.
	 */
	list_for_each_entry(hwid, &pnp.ids, list) {
		handler = acpi_scan_match_handler(hwid->id, NULL);
		if (handler && !handler->hotplug.ignore) {
			acpi_install_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
					acpi_hotplug_notify_cb, handler);
			break;
		}
	}

out:
	acpi_free_pnp_ids(&pnp);
}

static acpi_status acpi_bus_check_add(acpi_handle handle, u32 lvl_not_used,
				      void *not_used, void **return_value)
{
	struct acpi_device *device = NULL;
	int type;
	unsigned long long sta;
	int result;

	acpi_bus_get_device(handle, &device);
	if (device)
		goto out;

>>>>>>> refs/remotes/origin/master
	result = acpi_bus_type_and_status(handle, &type, &sta);
	if (result)
		return AE_OK;

<<<<<<< HEAD
	if (!(sta & ACPI_STA_DEVICE_PRESENT) &&
	    !(sta & ACPI_STA_DEVICE_FUNCTIONING)) {
		struct acpi_device_wakeup wakeup;
		acpi_handle temp;

		status = acpi_get_handle(handle, "_PRW", &temp);
		if (ACPI_SUCCESS(status))
			acpi_bus_extract_wakeup_device_power_package(handle,
								     &wakeup);
		return AE_CTRL_DEPTH;
	}

	/*
	 * We may already have an acpi_device from a previous enumeration.  If
	 * so, we needn't add it again, but we may still have to start it.
	 */
	device = NULL;
	acpi_bus_get_device(handle, &device);
	if (ops->acpi_op_add && !device)
		acpi_add_single_object(&device, handle, type, sta, ops);

	if (!device)
		return AE_CTRL_DEPTH;

	if (ops->acpi_op_start && !(ops->acpi_op_add)) {
		status = acpi_start_single_object(device);
		if (ACPI_FAILURE(status))
			return AE_CTRL_DEPTH;
	}

	if (!*return_value)
		*return_value = device;
	return AE_OK;
}

static int acpi_bus_scan(acpi_handle handle, struct acpi_bus_ops *ops,
			 struct acpi_device **child)
{
	acpi_status status;
	void *device = NULL;

	status = acpi_bus_check_add(handle, 0, ops, &device);
	if (ACPI_SUCCESS(status))
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_check_add, NULL, ops, &device);

	if (child)
		*child = device;

	if (device)
		return 0;
	else
		return -ENODEV;
}

/*
 * acpi_bus_add and acpi_bus_start
 *
 * scan a given ACPI tree and (probably recently hot-plugged)
 * create and add or starts found devices.
 *
 * If no devices were found -ENODEV is returned which does not
 * mean that this is a real error, there just have been no suitable
 * ACPI objects in the table trunk from which the kernel could create
 * a device and add/start an appropriate driver.
 */

int
acpi_bus_add(struct acpi_device **child,
	     struct acpi_device *parent, acpi_handle handle, int type)
{
	struct acpi_bus_ops ops;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_add = 1;

	return acpi_bus_scan(handle, &ops, child);
}
EXPORT_SYMBOL(acpi_bus_add);

int acpi_bus_start(struct acpi_device *device)
{
	struct acpi_bus_ops ops;
	int result;

	if (!device)
		return -EINVAL;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_start = 1;

	result = acpi_bus_scan(device->handle, &ops, NULL);

	acpi_update_all_gpes();

	return result;
}
EXPORT_SYMBOL(acpi_bus_start);

int acpi_bus_trim(struct acpi_device *start, int rmdevice)
{
	acpi_status status;
	struct acpi_device *parent, *child;
	acpi_handle phandle, chandle;
	acpi_object_type type;
	u32 level = 1;
	int err = 0;

	parent = start;
	phandle = start->handle;
	child = chandle = NULL;

	while ((level > 0) && parent && (!err)) {
		status = acpi_get_next_object(ACPI_TYPE_ANY, phandle,
					      chandle, &chandle);

		/*
		 * If this scope is exhausted then move our way back up.
		 */
		if (ACPI_FAILURE(status)) {
			level--;
			chandle = phandle;
			acpi_get_parent(phandle, &phandle);
			child = parent;
			parent = parent->parent;

			if (level == 0)
				err = acpi_bus_remove(child, rmdevice);
			else
				err = acpi_bus_remove(child, 1);

			continue;
		}

		status = acpi_get_type(chandle, &type);
		if (ACPI_FAILURE(status)) {
			continue;
		}
		/*
		 * If there is a device corresponding to chandle then
		 * parse it (depth-first).
		 */
		if (acpi_bus_get_device(chandle, &child) == 0) {
			level++;
			phandle = chandle;
			chandle = NULL;
			parent = child;
		}
		continue;
	}
	return err;
=======
	if (type == ACPI_BUS_TYPE_POWER) {
		acpi_add_power_resource(handle);
		return AE_OK;
	}

	acpi_scan_init_hotplug(handle, type);

	if (!(sta & ACPI_STA_DEVICE_PRESENT) &&
	    !(sta & ACPI_STA_DEVICE_FUNCTIONING)) {
		struct acpi_device_wakeup wakeup;

		if (acpi_has_method(handle, "_PRW")) {
			acpi_bus_extract_wakeup_device_power_package(handle,
								     &wakeup);
			acpi_power_resources_list_free(&wakeup.resources);
		}
		return AE_CTRL_DEPTH;
	}

	acpi_add_single_object(&device, handle, type, sta);
	if (!device)
		return AE_CTRL_DEPTH;

 out:
	if (!*return_value)
		*return_value = device;

	return AE_OK;
}

static int acpi_scan_attach_handler(struct acpi_device *device)
{
	struct acpi_hardware_id *hwid;
	int ret = 0;

	list_for_each_entry(hwid, &device->pnp.ids, list) {
		const struct acpi_device_id *devid;
		struct acpi_scan_handler *handler;

		handler = acpi_scan_match_handler(hwid->id, &devid);
		if (handler) {
			ret = handler->attach(device, devid);
			if (ret > 0) {
				device->handler = handler;
				break;
			} else if (ret < 0) {
				break;
			}
		}
	}
	return ret;
}

static acpi_status acpi_bus_device_attach(acpi_handle handle, u32 lvl_not_used,
					  void *not_used, void **ret_not_used)
{
	struct acpi_device *device;
	unsigned long long sta_not_used;
	int ret;

	/*
	 * Ignore errors ignored by acpi_bus_check_add() to avoid terminating
	 * namespace walks prematurely.
	 */
	if (acpi_bus_type_and_status(handle, &ret, &sta_not_used))
		return AE_OK;

	if (acpi_bus_get_device(handle, &device))
		return AE_CTRL_DEPTH;

	if (device->handler)
		return AE_OK;

	ret = acpi_scan_attach_handler(device);
	if (ret < 0)
		return AE_CTRL_DEPTH;

	device->flags.match_driver = true;
	if (ret > 0)
		return AE_OK;

	ret = device_attach(&device->dev);
	return ret >= 0 ? AE_OK : AE_CTRL_DEPTH;
}

/**
 * acpi_bus_scan - Add ACPI device node objects in a given namespace scope.
 * @handle: Root of the namespace scope to scan.
 *
 * Scan a given ACPI tree (probably recently hot-plugged) and create and add
 * found devices.
 *
 * If no devices were found, -ENODEV is returned, but it does not mean that
 * there has been a real error.  There just have been no suitable ACPI objects
 * in the table trunk from which the kernel could create a device and add an
 * appropriate driver.
 *
 * Must be called under acpi_scan_lock.
 */
int acpi_bus_scan(acpi_handle handle)
{
	void *device = NULL;
	int error = 0;

	if (ACPI_SUCCESS(acpi_bus_check_add(handle, 0, NULL, &device)))
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_check_add, NULL, NULL, &device);

	if (!device)
		error = -ENODEV;
	else if (ACPI_SUCCESS(acpi_bus_device_attach(handle, 0, NULL, NULL)))
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_device_attach, NULL, NULL, NULL);

	return error;
}
EXPORT_SYMBOL(acpi_bus_scan);

static acpi_status acpi_bus_device_detach(acpi_handle handle, u32 lvl_not_used,
					  void *not_used, void **ret_not_used)
{
	struct acpi_device *device = NULL;

	if (!acpi_bus_get_device(handle, &device)) {
		struct acpi_scan_handler *dev_handler = device->handler;

		if (dev_handler) {
			if (dev_handler->detach)
				dev_handler->detach(device);

			device->handler = NULL;
		} else {
			device_release_driver(&device->dev);
		}
	}
	return AE_OK;
}

static acpi_status acpi_bus_remove(acpi_handle handle, u32 lvl_not_used,
				   void *not_used, void **ret_not_used)
{
	struct acpi_device *device = NULL;

	if (!acpi_bus_get_device(handle, &device))
		acpi_device_unregister(device);

	return AE_OK;
}

/**
 * acpi_bus_trim - Remove ACPI device node and all of its descendants
 * @start: Root of the ACPI device nodes subtree to remove.
 *
 * Must be called under acpi_scan_lock.
 */
void acpi_bus_trim(struct acpi_device *start)
{
	/*
	 * Execute acpi_bus_device_detach() as a post-order callback to detach
	 * all ACPI drivers from the device nodes being removed.
	 */
	acpi_walk_namespace(ACPI_TYPE_ANY, start->handle, ACPI_UINT32_MAX, NULL,
			    acpi_bus_device_detach, NULL, NULL);
	acpi_bus_device_detach(start->handle, 0, NULL, NULL);
	/*
	 * Execute acpi_bus_remove() as a post-order callback to remove device
	 * nodes in the given namespace scope.
	 */
	acpi_walk_namespace(ACPI_TYPE_ANY, start->handle, ACPI_UINT32_MAX, NULL,
			    acpi_bus_remove, NULL, NULL);
	acpi_bus_remove(start->handle, 0, NULL, NULL);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(acpi_bus_trim);

static int acpi_bus_scan_fixed(void)
{
	int result = 0;
<<<<<<< HEAD
	struct acpi_device *device = NULL;
	struct acpi_bus_ops ops;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_add = 1;
	ops.acpi_op_start = 1;
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Enumerate all fixed-feature devices.
	 */
<<<<<<< HEAD
	if ((acpi_gbl_FADT.flags & ACPI_FADT_POWER_BUTTON) == 0) {
		result = acpi_add_single_object(&device, NULL,
						ACPI_BUS_TYPE_POWER_BUTTON,
						ACPI_STA_DEFAULT,
						&ops);
	}

	if ((acpi_gbl_FADT.flags & ACPI_FADT_SLEEP_BUTTON) == 0) {
		result = acpi_add_single_object(&device, NULL,
						ACPI_BUS_TYPE_SLEEP_BUTTON,
						ACPI_STA_DEFAULT,
						&ops);
	}

	return result;
=======
	if (!(acpi_gbl_FADT.flags & ACPI_FADT_POWER_BUTTON)) {
		struct acpi_device *device = NULL;

		result = acpi_add_single_object(&device, NULL,
						ACPI_BUS_TYPE_POWER_BUTTON,
						ACPI_STA_DEFAULT);
		if (result)
			return result;

		device->flags.match_driver = true;
		result = device_attach(&device->dev);
		if (result < 0)
			return result;

		device_init_wakeup(&device->dev, true);
	}

	if (!(acpi_gbl_FADT.flags & ACPI_FADT_SLEEP_BUTTON)) {
		struct acpi_device *device = NULL;

		result = acpi_add_single_object(&device, NULL,
						ACPI_BUS_TYPE_SLEEP_BUTTON,
						ACPI_STA_DEFAULT);
		if (result)
			return result;

		device->flags.match_driver = true;
		result = device_attach(&device->dev);
	}

	return result < 0 ? result : 0;
>>>>>>> refs/remotes/origin/master
}

int __init acpi_scan_init(void)
{
	int result;
<<<<<<< HEAD
	struct acpi_bus_ops ops;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_add = 1;
	ops.acpi_op_start = 1;
=======
>>>>>>> refs/remotes/origin/master

	result = bus_register(&acpi_bus_type);
	if (result) {
		/* We don't want to quit even if we failed to add suspend/resume */
		printk(KERN_ERR PREFIX "Could not register bus type\n");
	}

<<<<<<< HEAD
	acpi_power_init();

	/*
	 * Enumerate devices in the ACPI namespace.
	 */
	result = acpi_bus_scan(ACPI_ROOT_OBJECT, &ops, &acpi_root);

	if (!result)
		result = acpi_bus_scan_fixed();

	if (result)
		acpi_device_unregister(acpi_root, ACPI_BUS_REMOVAL_NORMAL);
	else
		acpi_update_all_gpes();

=======
	acpi_pci_root_init();
	acpi_pci_link_init();
	acpi_processor_init();
	acpi_platform_init();
	acpi_lpss_init();
	acpi_cmos_rtc_init();
	acpi_container_init();
	acpi_memory_hotplug_init();
	acpi_dock_init();

	mutex_lock(&acpi_scan_lock);
	/*
	 * Enumerate devices in the ACPI namespace.
	 */
	result = acpi_bus_scan(ACPI_ROOT_OBJECT);
	if (result)
		goto out;

	result = acpi_bus_get_device(ACPI_ROOT_OBJECT, &acpi_root);
	if (result)
		goto out;

	result = acpi_bus_scan_fixed();
	if (result) {
		acpi_device_unregister(acpi_root);
		goto out;
	}

	acpi_update_all_gpes();

	acpi_pci_root_hp_init();

 out:
	mutex_unlock(&acpi_scan_lock);
>>>>>>> refs/remotes/origin/master
	return result;
}
