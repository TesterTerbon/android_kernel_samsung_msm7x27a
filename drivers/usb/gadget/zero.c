/*
 * zero.c -- Gadget Zero, for USB development
 *
 * Copyright (C) 2003-2008 David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> refs/remotes/origin/cm-10.0
 */


=======
 */

>>>>>>> refs/remotes/origin/master
/*
 * Gadget Zero only needs two bulk endpoints, and is an example of how you
 * can write a hardware-agnostic gadget driver running inside a USB device.
 * Some hardware details are visible, but don't affect most of the driver.
 *
 * Use it with the Linux host/master side "usbtest" driver to get a basic
 * functional test of your device-side usb stack, or with "usb-skeleton".
 *
 * It supports two similar configurations.  One sinks whatever the usb host
 * writes, and in return sources zeroes.  The other loops whatever the host
 * writes back, so the host can read it.
 *
 * Many drivers will only have one configuration, letting them be much
 * simpler if they also don't support high speed operation (like this
 * driver does).
 *
 * Why is *this* driver using two configurations, rather than setting up
 * two interfaces with different functions?  To help verify that multiple
 * configuration infrastucture is working correctly; also, so that it can
 * work with low capability USB controllers without four bulk endpoints.
 */

/*
 * driver assumes self-powered hardware, and
 * has no way for users to trigger remote wakeup.
 */

/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/utsname.h>
#include <linux/device.h>

#include "g_zero.h"
#include "gadget_chips.h"


/*-------------------------------------------------------------------------*/

/*
 * Kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 */
#include "composite.c"
#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"

#include "f_sourcesink.c"
#include "f_loopback.c"

/*-------------------------------------------------------------------------*/
=======
#include <linux/device.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/usb/composite.h>

#include "g_zero.h"
/*-------------------------------------------------------------------------*/
USB_GADGET_COMPOSITE_OPTIONS();
>>>>>>> refs/remotes/origin/master

#define DRIVER_VERSION		"Cinco de Mayo 2008"

static const char longname[] = "Gadget Zero";

<<<<<<< HEAD
unsigned buflen = 4096;
module_param(buflen, uint, 0);

=======
>>>>>>> refs/remotes/origin/master
/*
 * Normally the "loopback" configuration is second (index 1) so
 * it's not the default.  Here's where to change that order, to
 * work better with hosts where config changes are problematic or
 * controllers (like original superh) that only support one config.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int loopdefault = 0;
=======
static bool loopdefault = 0;
>>>>>>> refs/remotes/origin/cm-10.0
module_param(loopdefault, bool, S_IRUGO|S_IWUSR);

=======
static bool loopdefault = 0;
module_param(loopdefault, bool, S_IRUGO|S_IWUSR);

static struct usb_zero_options gzero_options = {
	.isoc_interval = GZERO_ISOC_INTERVAL,
	.isoc_maxpacket = GZERO_ISOC_MAXPACKET,
	.bulk_buflen = GZERO_BULK_BUFLEN,
	.qlen = GZERO_QLEN,
};

>>>>>>> refs/remotes/origin/master
/*-------------------------------------------------------------------------*/

/* Thanks to NetChip Technologies for donating this product ID.
 *
 * DO NOT REUSE THESE IDs with a protocol-incompatible driver!!  Ever!!
 * Instead:  allocate your own, using normal USB-IF procedures.
 */
#ifndef	CONFIG_USB_ZERO_HNPTEST
#define DRIVER_VENDOR_NUM	0x0525		/* NetChip */
#define DRIVER_PRODUCT_NUM	0xa4a0		/* Linux-USB "Gadget Zero" */
#define DEFAULT_AUTORESUME	0
#else
#define DRIVER_VENDOR_NUM	0x1a0a		/* OTG test device IDs */
#define DRIVER_PRODUCT_NUM	0xbadd
#define DEFAULT_AUTORESUME	5
#endif

/* If the optional "autoresume" mode is enabled, it provides good
 * functional coverage for the "USBCV" test harness from USB-IF.
 * It's always set if OTG mode is enabled.
 */
<<<<<<< HEAD
unsigned autoresume = DEFAULT_AUTORESUME;
module_param(autoresume, uint, S_IRUGO);
MODULE_PARM_DESC(autoresume, "zero, or seconds before remote wakeup");

=======
static unsigned autoresume = DEFAULT_AUTORESUME;
module_param(autoresume, uint, S_IRUGO);
MODULE_PARM_DESC(autoresume, "zero, or seconds before remote wakeup");

/* Maximum Autoresume time */
static unsigned max_autoresume;
module_param(max_autoresume, uint, S_IRUGO);
MODULE_PARM_DESC(max_autoresume, "maximum seconds before remote wakeup");

/* Interval between two remote wakeups */
static unsigned autoresume_interval_ms;
module_param(autoresume_interval_ms, uint, S_IRUGO);
MODULE_PARM_DESC(autoresume_interval_ms,
		"milliseconds to increase successive wakeup delays");

static unsigned autoresume_step_ms;
>>>>>>> refs/remotes/origin/master
/*-------------------------------------------------------------------------*/

static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		cpu_to_le16(0x0200),
	.bDeviceClass =		USB_CLASS_VENDOR_SPEC,

	.idVendor =		cpu_to_le16(DRIVER_VENDOR_NUM),
	.idProduct =		cpu_to_le16(DRIVER_PRODUCT_NUM),
	.bNumConfigurations =	2,
};

#ifdef CONFIG_USB_OTG
static struct usb_otg_descriptor otg_descriptor = {
	.bLength =		sizeof otg_descriptor,
	.bDescriptorType =	USB_DT_OTG,

	/* REVISIT SRP-only hardware is possible, although
	 * it would not be called "OTG" ...
	 */
	.bmAttributes =		USB_OTG_SRP | USB_OTG_HNP,
};

<<<<<<< HEAD
const struct usb_descriptor_header *otg_desc[] = {
	(struct usb_descriptor_header *) &otg_descriptor,
	NULL,
};
#endif

/* string IDs are assigned dynamically */

#define STRING_MANUFACTURER_IDX		0
#define STRING_PRODUCT_IDX		1
#define STRING_SERIAL_IDX		2

static char manufacturer[50];

/* default serial number takes at least two packets */
static char serial[] = "0123456789.0123456789.0123456789";

static struct usb_string strings_dev[] = {
	[STRING_MANUFACTURER_IDX].s = manufacturer,
	[STRING_PRODUCT_IDX].s = longname,
	[STRING_SERIAL_IDX].s = serial,
=======
static const struct usb_descriptor_header *otg_desc[] = {
	(struct usb_descriptor_header *) &otg_descriptor,
	NULL,
};
#else
#define otg_desc	NULL
#endif

/* string IDs are assigned dynamically */
/* default serial number takes at least two packets */
static char serial[] = "0123456789.0123456789.0123456789";

#define USB_GZERO_SS_DESC	(USB_GADGET_FIRST_AVAIL_IDX + 0)
#define USB_GZERO_LB_DESC	(USB_GADGET_FIRST_AVAIL_IDX + 1)

static struct usb_string strings_dev[] = {
	[USB_GADGET_MANUFACTURER_IDX].s = "",
	[USB_GADGET_PRODUCT_IDX].s = longname,
	[USB_GADGET_SERIAL_IDX].s = serial,
	[USB_GZERO_SS_DESC].s	= "source and sink data",
	[USB_GZERO_LB_DESC].s	= "loop input to output",
>>>>>>> refs/remotes/origin/master
	{  }			/* end of list */
};

static struct usb_gadget_strings stringtab_dev = {
	.language	= 0x0409,	/* en-us */
	.strings	= strings_dev,
};

static struct usb_gadget_strings *dev_strings[] = {
	&stringtab_dev,
	NULL,
};

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
struct usb_request *alloc_ep_req(struct usb_ep *ep)
{
	struct usb_request	*req;

	req = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (req) {
		req->length = buflen;
		req->buf = kmalloc(buflen, GFP_ATOMIC);
		if (!req->buf) {
			usb_ep_free_request(ep, req);
			req = NULL;
		}
	}
	return req;
}

void free_ep_req(struct usb_ep *ep, struct usb_request *req)
{
	kfree(req->buf);
	usb_ep_free_request(ep, req);
}

static void disable_ep(struct usb_composite_dev *cdev, struct usb_ep *ep)
{
	int			value;

	if (ep->driver_data) {
		value = usb_ep_disable(ep);
		if (value < 0)
			DBG(cdev, "disable %s --> %d\n",
					ep->name, value);
		ep->driver_data = NULL;
	}
}

void disable_endpoints(struct usb_composite_dev *cdev,
		struct usb_ep *in, struct usb_ep *out)
{
	disable_ep(cdev, in);
	disable_ep(cdev, out);
}

/*-------------------------------------------------------------------------*/

=======
>>>>>>> refs/remotes/origin/master
static struct timer_list	autoresume_timer;

static void zero_autoresume(unsigned long _c)
{
	struct usb_composite_dev	*cdev = (void *)_c;
	struct usb_gadget		*g = cdev->gadget;

	/* unconfigured devices can't issue wakeups */
	if (!cdev->config)
		return;

	/* Normally the host would be woken up for something
	 * more significant than just a timer firing; likely
	 * because of some direct user request.
	 */
	if (g->speed != USB_SPEED_UNKNOWN) {
		int status = usb_gadget_wakeup(g);
		INFO(cdev, "%s --> %d\n", __func__, status);
	}
}

static void zero_suspend(struct usb_composite_dev *cdev)
{
	if (cdev->gadget->speed == USB_SPEED_UNKNOWN)
		return;

	if (autoresume) {
<<<<<<< HEAD
		mod_timer(&autoresume_timer, jiffies + (HZ * autoresume));
		DBG(cdev, "suspend, wakeup in %d seconds\n", autoresume);
=======
		if (max_autoresume &&
			(autoresume_step_ms > max_autoresume * 1000))
				autoresume_step_ms = autoresume * 1000;

		mod_timer(&autoresume_timer, jiffies +
			msecs_to_jiffies(autoresume_step_ms));
		DBG(cdev, "suspend, wakeup in %d milliseconds\n",
			autoresume_step_ms);

		autoresume_step_ms += autoresume_interval_ms;
>>>>>>> refs/remotes/origin/master
	} else
		DBG(cdev, "%s\n", __func__);
}

static void zero_resume(struct usb_composite_dev *cdev)
{
	DBG(cdev, "%s\n", __func__);
	del_timer(&autoresume_timer);
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static int __init zero_bind(struct usb_composite_dev *cdev)
{
	int			gcnum;
	struct usb_gadget	*gadget = cdev->gadget;
	int			id;
=======
static struct usb_configuration loopback_driver = {
	.label          = "loopback",
	.bConfigurationValue = 2,
	.bmAttributes   = USB_CONFIG_ATT_SELFPOWER,
	/* .iConfiguration = DYNAMIC */
};

static struct usb_function *func_ss;
static struct usb_function_instance *func_inst_ss;

static int ss_config_setup(struct usb_configuration *c,
		const struct usb_ctrlrequest *ctrl)
{
	switch (ctrl->bRequest) {
	case 0x5b:
	case 0x5c:
		return func_ss->setup(func_ss, ctrl);
	default:
		return -EOPNOTSUPP;
	}
}

static struct usb_configuration sourcesink_driver = {
	.label                  = "source/sink",
	.setup                  = ss_config_setup,
	.bConfigurationValue    = 3,
	.bmAttributes           = USB_CONFIG_ATT_SELFPOWER,
	/* .iConfiguration      = DYNAMIC */
};

module_param_named(buflen, gzero_options.bulk_buflen, uint, 0);
module_param_named(pattern, gzero_options.pattern, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(pattern, "0 = all zeroes, 1 = mod63, 2 = none");

module_param_named(isoc_interval, gzero_options.isoc_interval, uint,
		S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(isoc_interval, "1 - 16");

module_param_named(isoc_maxpacket, gzero_options.isoc_maxpacket, uint,
		S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(isoc_maxpacket, "0 - 1023 (fs), 0 - 1024 (hs/ss)");

module_param_named(isoc_mult, gzero_options.isoc_mult, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(isoc_mult, "0 - 2 (hs/ss only)");

module_param_named(isoc_maxburst, gzero_options.isoc_maxburst, uint,
		S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(isoc_maxburst, "0 - 15 (ss only)");

static struct usb_function *func_lb;
static struct usb_function_instance *func_inst_lb;

module_param_named(qlen, gzero_options.qlen, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(qlen, "depth of loopback queue");

static int __init zero_bind(struct usb_composite_dev *cdev)
{
	struct f_ss_opts	*ss_opts;
	struct f_lb_opts	*lb_opts;
	int			status;
>>>>>>> refs/remotes/origin/master

	/* Allocate string descriptor numbers ... note that string
	 * contents can be overridden by the composite_dev glue.
	 */
<<<<<<< HEAD
	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_dev[STRING_MANUFACTURER_IDX].id = id;
	device_desc.iManufacturer = id;

	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_dev[STRING_PRODUCT_IDX].id = id;
	device_desc.iProduct = id;

	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_dev[STRING_SERIAL_IDX].id = id;
	device_desc.iSerialNumber = id;

	setup_timer(&autoresume_timer, zero_autoresume, (unsigned long) cdev);

=======
	status = usb_string_ids_tab(cdev, strings_dev);
	if (status < 0)
		return status;

	device_desc.iManufacturer = strings_dev[USB_GADGET_MANUFACTURER_IDX].id;
	device_desc.iProduct = strings_dev[USB_GADGET_PRODUCT_IDX].id;
	device_desc.iSerialNumber = strings_dev[USB_GADGET_SERIAL_IDX].id;

	setup_timer(&autoresume_timer, zero_autoresume, (unsigned long) cdev);

	func_inst_ss = usb_get_function_instance("SourceSink");
	if (IS_ERR(func_inst_ss))
		return PTR_ERR(func_inst_ss);

	ss_opts =  container_of(func_inst_ss, struct f_ss_opts, func_inst);
	ss_opts->pattern = gzero_options.pattern;
	ss_opts->isoc_interval = gzero_options.isoc_interval;
	ss_opts->isoc_maxpacket = gzero_options.isoc_maxpacket;
	ss_opts->isoc_mult = gzero_options.isoc_mult;
	ss_opts->isoc_maxburst = gzero_options.isoc_maxpacket;
	ss_opts->bulk_buflen = gzero_options.bulk_buflen;

	func_ss = usb_get_function(func_inst_ss);
	if (IS_ERR(func_ss)) {
		status = PTR_ERR(func_ss);
		goto err_put_func_inst_ss;
	}

	func_inst_lb = usb_get_function_instance("Loopback");
	if (IS_ERR(func_inst_lb)) {
		status = PTR_ERR(func_inst_lb);
		goto err_put_func_ss;
	}

	lb_opts = container_of(func_inst_lb, struct f_lb_opts, func_inst);
	lb_opts->bulk_buflen = gzero_options.bulk_buflen;
	lb_opts->qlen = gzero_options.qlen;

	func_lb = usb_get_function(func_inst_lb);
	if (IS_ERR(func_lb)) {
		status = PTR_ERR(func_lb);
		goto err_put_func_inst_lb;
	}

	sourcesink_driver.iConfiguration = strings_dev[USB_GZERO_SS_DESC].id;
	loopback_driver.iConfiguration = strings_dev[USB_GZERO_LB_DESC].id;

	/* support autoresume for remote wakeup testing */
	sourcesink_driver.bmAttributes &= ~USB_CONFIG_ATT_WAKEUP;
	loopback_driver.bmAttributes &= ~USB_CONFIG_ATT_WAKEUP;
	sourcesink_driver.descriptors = NULL;
	loopback_driver.descriptors = NULL;
	if (autoresume) {
		sourcesink_driver.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
		loopback_driver.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
		autoresume_step_ms = autoresume * 1000;
	}

	/* support OTG systems */
	if (gadget_is_otg(cdev->gadget)) {
		sourcesink_driver.descriptors = otg_desc;
		sourcesink_driver.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
		loopback_driver.descriptors = otg_desc;
		loopback_driver.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

>>>>>>> refs/remotes/origin/master
	/* Register primary, then secondary configuration.  Note that
	 * SH3 only allows one config...
	 */
	if (loopdefault) {
<<<<<<< HEAD
		loopback_add(cdev, autoresume != 0);
		sourcesink_add(cdev, autoresume != 0);
	} else {
		sourcesink_add(cdev, autoresume != 0);
		loopback_add(cdev, autoresume != 0);
	}

	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(0x0200 + gcnum);
	else {
		/* gadget zero is so simple (for now, no altsettings) that
		 * it SHOULD NOT have problems with bulk-capable hardware.
		 * so just warn about unrcognized controllers -- don't panic.
		 *
		 * things like configuration and altsetting numbering
		 * can need hardware-specific attention though.
		 */
		pr_warning("%s: controller '%s' not recognized\n",
			longname, gadget->name);
		device_desc.bcdDevice = cpu_to_le16(0x9999);
	}


	INFO(cdev, "%s, version: " DRIVER_VERSION "\n", longname);

	snprintf(manufacturer, sizeof manufacturer, "%s %s with %s",
		init_utsname()->sysname, init_utsname()->release,
		gadget->name);

	return 0;
=======
		usb_add_config_only(cdev, &loopback_driver);
		usb_add_config_only(cdev, &sourcesink_driver);
	} else {
		usb_add_config_only(cdev, &sourcesink_driver);
		usb_add_config_only(cdev, &loopback_driver);
	}
	status = usb_add_function(&sourcesink_driver, func_ss);
	if (status)
		goto err_conf_flb;

	usb_ep_autoconfig_reset(cdev->gadget);
	status = usb_add_function(&loopback_driver, func_lb);
	if (status)
		goto err_conf_flb;

	usb_ep_autoconfig_reset(cdev->gadget);
	usb_composite_overwrite_options(cdev, &coverwrite);

	INFO(cdev, "%s, version: " DRIVER_VERSION "\n", longname);

	return 0;

err_conf_flb:
	usb_put_function(func_lb);
	func_lb = NULL;
err_put_func_inst_lb:
	usb_put_function_instance(func_inst_lb);
	func_inst_lb = NULL;
err_put_func_ss:
	usb_put_function(func_ss);
	func_ss = NULL;
err_put_func_inst_ss:
	usb_put_function_instance(func_inst_ss);
	func_inst_ss = NULL;
	return status;
>>>>>>> refs/remotes/origin/master
}

static int zero_unbind(struct usb_composite_dev *cdev)
{
	del_timer_sync(&autoresume_timer);
<<<<<<< HEAD
	return 0;
}

static struct usb_composite_driver zero_driver = {
	.name		= "zero",
	.dev		= &device_desc,
	.strings	= dev_strings,
<<<<<<< HEAD
=======
	.max_speed	= USB_SPEED_SUPER,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!IS_ERR_OR_NULL(func_ss))
		usb_put_function(func_ss);
	usb_put_function_instance(func_inst_ss);
	if (!IS_ERR_OR_NULL(func_lb))
		usb_put_function(func_lb);
	usb_put_function_instance(func_inst_lb);
	return 0;
}

static __refdata struct usb_composite_driver zero_driver = {
	.name		= "zero",
	.dev		= &device_desc,
	.strings	= dev_strings,
	.max_speed	= USB_SPEED_SUPER,
	.bind		= zero_bind,
>>>>>>> refs/remotes/origin/master
	.unbind		= zero_unbind,
	.suspend	= zero_suspend,
	.resume		= zero_resume,
};

MODULE_AUTHOR("David Brownell");
MODULE_LICENSE("GPL");

static int __init init(void)
{
<<<<<<< HEAD
	return usb_composite_probe(&zero_driver, zero_bind);
=======
	return usb_composite_probe(&zero_driver);
>>>>>>> refs/remotes/origin/master
}
module_init(init);

static void __exit cleanup(void)
{
	usb_composite_unregister(&zero_driver);
}
module_exit(cleanup);
