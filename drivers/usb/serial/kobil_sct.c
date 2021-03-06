/*
 *  KOBIL USB Smart Card Terminal Driver
 *
 *  Copyright (C) 2002  KOBIL Systems GmbH
 *  Author: Thomas Wahrenbruch
 *
 *  Contact: linuxusb@kobil.de
 *
 *  This program is largely derived from work by the linux-usb group
 *  and associated source files.  Please see the usb/serial files for
 *  individual credits and copyrights.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Thanks to Greg Kroah-Hartman (greg@kroah.com) for his help and
 *  patience.
 *
 * Supported readers: USB TWIN, KAAN Standard Plus and SecOVID Reader Plus
 * (Adapter K), B1 Professional and KAAN Professional (Adapter B)
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * (21/05/2004) tw
 *      Fix bug with P'n'P readers
 *
 * (28/05/2003) tw
 *      Add support for KAAN SIM
 *
 * (12/09/2002) tw
 *      Adapted to 2.5.
 *
 * (11/08/2002) tw
 *      Initial version.
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */


#include <linux/kernel.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#include <linux/ioctl.h>
#include "kobil_sct.h"

<<<<<<< HEAD
<<<<<<< HEAD
static int debug;
=======
static bool debug;
>>>>>>> refs/remotes/origin/cm-10.0

/* Version Information */
#define DRIVER_VERSION "21/05/2004"
=======
>>>>>>> refs/remotes/origin/master
#define DRIVER_AUTHOR "KOBIL Systems GmbH - http://www.kobil.com"
#define DRIVER_DESC "KOBIL USB Smart Card Terminal Driver (experimental)"

#define KOBIL_VENDOR_ID			0x0D46
#define KOBIL_ADAPTER_B_PRODUCT_ID	0x2011
#define KOBIL_ADAPTER_K_PRODUCT_ID	0x2012
#define KOBIL_USBTWIN_PRODUCT_ID	0x0078
#define KOBIL_KAAN_SIM_PRODUCT_ID       0x0081

#define KOBIL_TIMEOUT		500
#define KOBIL_BUF_LENGTH	300


/* Function prototypes */
<<<<<<< HEAD
static int  kobil_startup(struct usb_serial *serial);
static void kobil_release(struct usb_serial *serial);
=======
static int kobil_port_probe(struct usb_serial_port *probe);
static int kobil_port_remove(struct usb_serial_port *probe);
>>>>>>> refs/remotes/origin/master
static int  kobil_open(struct tty_struct *tty, struct usb_serial_port *port);
static void kobil_close(struct usb_serial_port *port);
static int  kobil_write(struct tty_struct *tty, struct usb_serial_port *port,
			 const unsigned char *buf, int count);
static int  kobil_write_room(struct tty_struct *tty);
static int  kobil_ioctl(struct tty_struct *tty,
			unsigned int cmd, unsigned long arg);
static int  kobil_tiocmget(struct tty_struct *tty);
static int  kobil_tiocmset(struct tty_struct *tty,
			   unsigned int set, unsigned int clear);
static void kobil_read_int_callback(struct urb *urb);
<<<<<<< HEAD
static void kobil_write_callback(struct urb *purb);
=======
static void kobil_write_int_callback(struct urb *urb);
>>>>>>> refs/remotes/origin/master
static void kobil_set_termios(struct tty_struct *tty,
			struct usb_serial_port *port, struct ktermios *old);
static void kobil_init_termios(struct tty_struct *tty);

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(KOBIL_VENDOR_ID, KOBIL_ADAPTER_B_PRODUCT_ID) },
	{ USB_DEVICE(KOBIL_VENDOR_ID, KOBIL_ADAPTER_K_PRODUCT_ID) },
	{ USB_DEVICE(KOBIL_VENDOR_ID, KOBIL_USBTWIN_PRODUCT_ID) },
	{ USB_DEVICE(KOBIL_VENDOR_ID, KOBIL_KAAN_SIM_PRODUCT_ID) },
	{ }			/* Terminating entry */
};
<<<<<<< HEAD


MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver kobil_driver = {
	.name =		"kobil",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
<<<<<<< HEAD
	.no_dynamic_id = 	1,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};


=======
MODULE_DEVICE_TABLE(usb, id_table);

>>>>>>> refs/remotes/origin/master
static struct usb_serial_driver kobil_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"kobil",
	},
	.description =		"KOBIL USB smart card terminal",
<<<<<<< HEAD
<<<<<<< HEAD
	.usb_driver = 		&kobil_driver,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.id_table =		id_table,
	.num_ports =		1,
	.attach =		kobil_startup,
	.release =		kobil_release,
=======
	.id_table =		id_table,
	.num_ports =		1,
	.port_probe =		kobil_port_probe,
	.port_remove =		kobil_port_remove,
>>>>>>> refs/remotes/origin/master
	.ioctl =		kobil_ioctl,
	.set_termios =		kobil_set_termios,
	.init_termios =		kobil_init_termios,
	.tiocmget =		kobil_tiocmget,
	.tiocmset =		kobil_tiocmset,
	.open =			kobil_open,
	.close =		kobil_close,
	.write =		kobil_write,
	.write_room =		kobil_write_room,
	.read_int_callback =	kobil_read_int_callback,
<<<<<<< HEAD
};

<<<<<<< HEAD
=======
static struct usb_serial_driver * const serial_drivers[] = {
	&kobil_device, NULL
};
>>>>>>> refs/remotes/origin/cm-10.0

struct kobil_private {
	int write_int_endpoint_address;
	int read_int_endpoint_address;
=======
	.write_int_callback =	kobil_write_int_callback,
};

static struct usb_serial_driver * const serial_drivers[] = {
	&kobil_device, NULL
};

struct kobil_private {
>>>>>>> refs/remotes/origin/master
	unsigned char buf[KOBIL_BUF_LENGTH]; /* buffer for the APDU to send */
	int filled;  /* index of the last char in buf */
	int cur_pos; /* index of the next char to send in buf */
	__u16 device_type;
};


<<<<<<< HEAD
static int kobil_startup(struct usb_serial *serial)
{
	int i;
	struct kobil_private *priv;
	struct usb_device *pdev;
	struct usb_host_config *actconfig;
	struct usb_interface *interface;
	struct usb_host_interface *altsetting;
	struct usb_host_endpoint *endpoint;
=======
static int kobil_port_probe(struct usb_serial_port *port)
{
	struct usb_serial *serial = port->serial;
	struct kobil_private *priv;
>>>>>>> refs/remotes/origin/master

	priv = kmalloc(sizeof(struct kobil_private), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->filled = 0;
	priv->cur_pos = 0;
	priv->device_type = le16_to_cpu(serial->dev->descriptor.idProduct);

	switch (priv->device_type) {
	case KOBIL_ADAPTER_B_PRODUCT_ID:
<<<<<<< HEAD
		printk(KERN_DEBUG "KOBIL B1 PRO / KAAN PRO detected\n");
		break;
	case KOBIL_ADAPTER_K_PRODUCT_ID:
		printk(KERN_DEBUG
		  "KOBIL KAAN Standard Plus / SecOVID Reader Plus detected\n");
		break;
	case KOBIL_USBTWIN_PRODUCT_ID:
		printk(KERN_DEBUG "KOBIL USBTWIN detected\n");
		break;
	case KOBIL_KAAN_SIM_PRODUCT_ID:
		printk(KERN_DEBUG "KOBIL KAAN SIM detected\n");
		break;
	}
	usb_set_serial_port_data(serial->port[0], priv);

	/* search for the necessary endpoints */
	pdev = serial->dev;
	actconfig = pdev->actconfig;
	interface = actconfig->interface[0];
	altsetting = interface->cur_altsetting;
	endpoint = altsetting->endpoint;

	for (i = 0; i < altsetting->desc.bNumEndpoints; i++) {
		endpoint = &altsetting->endpoint[i];
		if (usb_endpoint_is_int_out(&endpoint->desc)) {
			dbg("%s Found interrupt out endpoint. Address: %d",
				__func__, endpoint->desc.bEndpointAddress);
			priv->write_int_endpoint_address =
				endpoint->desc.bEndpointAddress;
		}
		if (usb_endpoint_is_int_in(&endpoint->desc)) {
			dbg("%s Found interrupt in  endpoint. Address: %d",
				__func__, endpoint->desc.bEndpointAddress);
			priv->read_int_endpoint_address =
				endpoint->desc.bEndpointAddress;
		}
	}
=======
		dev_dbg(&serial->dev->dev, "KOBIL B1 PRO / KAAN PRO detected\n");
		break;
	case KOBIL_ADAPTER_K_PRODUCT_ID:
		dev_dbg(&serial->dev->dev, "KOBIL KAAN Standard Plus / SecOVID Reader Plus detected\n");
		break;
	case KOBIL_USBTWIN_PRODUCT_ID:
		dev_dbg(&serial->dev->dev, "KOBIL USBTWIN detected\n");
		break;
	case KOBIL_KAAN_SIM_PRODUCT_ID:
		dev_dbg(&serial->dev->dev, "KOBIL KAAN SIM detected\n");
		break;
	}
	usb_set_serial_port_data(port, priv);

>>>>>>> refs/remotes/origin/master
	return 0;
}


<<<<<<< HEAD
static void kobil_release(struct usb_serial *serial)
{
	int i;
	dbg("%s - port %d", __func__, serial->port[0]->number);

	for (i = 0; i < serial->num_ports; ++i)
		kfree(usb_get_serial_port_data(serial->port[i]));
=======
static int kobil_port_remove(struct usb_serial_port *port)
{
	struct kobil_private *priv;

	priv = usb_get_serial_port_data(port);
	kfree(priv);

	return 0;
>>>>>>> refs/remotes/origin/master
}

static void kobil_init_termios(struct tty_struct *tty)
{
	/* Default to echo off and other sane device settings */
<<<<<<< HEAD
	tty->termios->c_lflag = 0;
	tty->termios->c_lflag &= ~(ISIG | ICANON | ECHO | IEXTEN | XCASE);
	tty->termios->c_iflag = IGNBRK | IGNPAR | IXOFF;
	/* do NOT translate CR to CR-NL (0x0A -> 0x0A 0x0D) */
	tty->termios->c_oflag &= ~ONLCR;
=======
	tty->termios.c_lflag = 0;
	tty->termios.c_iflag &= ~(ISIG | ICANON | ECHO | IEXTEN | XCASE);
	tty->termios.c_iflag |= IGNBRK | IGNPAR | IXOFF;
	/* do NOT translate CR to CR-NL (0x0A -> 0x0A 0x0D) */
	tty->termios.c_oflag &= ~ONLCR;
>>>>>>> refs/remotes/origin/master
}

static int kobil_open(struct tty_struct *tty, struct usb_serial_port *port)
{
<<<<<<< HEAD
=======
	struct device *dev = &port->dev;
>>>>>>> refs/remotes/origin/master
	int result = 0;
	struct kobil_private *priv;
	unsigned char *transfer_buffer;
	int transfer_buffer_length = 8;
<<<<<<< HEAD
	int write_urb_transfer_buffer_length = 8;

	dbg("%s - port %d", __func__, port->number);
	priv = usb_get_serial_port_data(port);

<<<<<<< HEAD
	/* someone sets the dev to 0 if the close method has been called */
	port->interrupt_in_urb->dev = port->serial->dev;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

	priv = usb_get_serial_port_data(port);

>>>>>>> refs/remotes/origin/master
	/* allocate memory for transfer buffer */
	transfer_buffer = kzalloc(transfer_buffer_length, GFP_KERNEL);
	if (!transfer_buffer)
		return -ENOMEM;

<<<<<<< HEAD
	/* allocate write_urb */
	if (!port->write_urb) {
		dbg("%s - port %d  Allocating port->write_urb",
						__func__, port->number);
		port->write_urb = usb_alloc_urb(0, GFP_KERNEL);
		if (!port->write_urb) {
			dbg("%s - port %d usb_alloc_urb failed",
						__func__, port->number);
			kfree(transfer_buffer);
			return -ENOMEM;
		}
	}

	/* allocate memory for write_urb transfer buffer */
	port->write_urb->transfer_buffer =
			kmalloc(write_urb_transfer_buffer_length, GFP_KERNEL);
	if (!port->write_urb->transfer_buffer) {
		kfree(transfer_buffer);
		usb_free_urb(port->write_urb);
		port->write_urb = NULL;
		return -ENOMEM;
	}

=======
>>>>>>> refs/remotes/origin/master
	/* get hardware version */
	result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_GetMisc,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_IN,
			  SUSBCR_MSC_GetHWVersion,
			  0,
			  transfer_buffer,
			  transfer_buffer_length,
			  KOBIL_TIMEOUT
	);
<<<<<<< HEAD
	dbg("%s - port %d Send get_HW_version URB returns: %i",
		__func__, port->number, result);
	dbg("Harware version: %i.%i.%i",
		transfer_buffer[0], transfer_buffer[1], transfer_buffer[2]);
=======
	dev_dbg(dev, "%s - Send get_HW_version URB returns: %i\n", __func__, result);
	dev_dbg(dev, "Harware version: %i.%i.%i\n", transfer_buffer[0],
		transfer_buffer[1], transfer_buffer[2]);
>>>>>>> refs/remotes/origin/master

	/* get firmware version */
	result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_GetMisc,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_IN,
			  SUSBCR_MSC_GetFWVersion,
			  0,
			  transfer_buffer,
			  transfer_buffer_length,
			  KOBIL_TIMEOUT
	);
<<<<<<< HEAD
	dbg("%s - port %d Send get_FW_version URB returns: %i",
					__func__, port->number, result);
	dbg("Firmware version: %i.%i.%i",
		transfer_buffer[0], transfer_buffer[1], transfer_buffer[2]);
=======
	dev_dbg(dev, "%s - Send get_FW_version URB returns: %i\n", __func__, result);
	dev_dbg(dev, "Firmware version: %i.%i.%i\n", transfer_buffer[0],
		transfer_buffer[1], transfer_buffer[2]);
>>>>>>> refs/remotes/origin/master

	if (priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID ||
			priv->device_type == KOBIL_ADAPTER_K_PRODUCT_ID) {
		/* Setting Baudrate, Parity and Stopbits */
		result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_SetBaudRateParityAndStopBits,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_OUT,
			  SUSBCR_SBR_9600 | SUSBCR_SPASB_EvenParity |
							SUSBCR_SPASB_1StopBit,
			  0,
			  transfer_buffer,
			  0,
			  KOBIL_TIMEOUT
		);
<<<<<<< HEAD
		dbg("%s - port %d Send set_baudrate URB returns: %i",
					__func__, port->number, result);
=======
		dev_dbg(dev, "%s - Send set_baudrate URB returns: %i\n", __func__, result);
>>>>>>> refs/remotes/origin/master

		/* reset all queues */
		result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_Misc,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_OUT,
			  SUSBCR_MSC_ResetAllQueues,
			  0,
			  transfer_buffer,
			  0,
			  KOBIL_TIMEOUT
		);
<<<<<<< HEAD
		dbg("%s - port %d Send reset_all_queues URB returns: %i",
					__func__, port->number, result);
=======
		dev_dbg(dev, "%s - Send reset_all_queues URB returns: %i\n", __func__, result);
>>>>>>> refs/remotes/origin/master
	}
	if (priv->device_type == KOBIL_USBTWIN_PRODUCT_ID ||
	    priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID ||
	    priv->device_type == KOBIL_KAAN_SIM_PRODUCT_ID) {
		/* start reading (Adapter B 'cause PNP string) */
		result = usb_submit_urb(port->interrupt_in_urb, GFP_ATOMIC);
<<<<<<< HEAD
		dbg("%s - port %d Send read URB returns: %i",
					__func__, port->number, result);
=======
		dev_dbg(dev, "%s - Send read URB returns: %i\n", __func__, result);
>>>>>>> refs/remotes/origin/master
	}

	kfree(transfer_buffer);
	return 0;
}


static void kobil_close(struct usb_serial_port *port)
{
<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	/* FIXME: Add rts/dtr methods */
	if (port->write_urb) {
		usb_poison_urb(port->write_urb);
		kfree(port->write_urb->transfer_buffer);
		usb_free_urb(port->write_urb);
		port->write_urb = NULL;
	}
=======
	/* FIXME: Add rts/dtr methods */
	usb_kill_urb(port->interrupt_out_urb);
>>>>>>> refs/remotes/origin/master
	usb_kill_urb(port->interrupt_in_urb);
}


static void kobil_read_int_callback(struct urb *urb)
{
	int result;
	struct usb_serial_port *port = urb->context;
<<<<<<< HEAD
	struct tty_struct *tty;
	unsigned char *data = urb->transfer_buffer;
	int status = urb->status;
/*	char *dbg_data; */

	dbg("%s - port %d", __func__, port->number);

	if (status) {
		dbg("%s - port %d Read int status not zero: %d",
		    __func__, port->number, status);
		return;
	}

	tty = tty_port_tty_get(&port->port);
	if (tty && urb->actual_length) {

		/* BEGIN DEBUG */
		/*
		  dbg_data = kzalloc((3 *  purb->actual_length + 10)
						* sizeof(char), GFP_KERNEL);
		  if (! dbg_data) {
			  return;
		  }
		  for (i = 0; i < purb->actual_length; i++) {
			  sprintf(dbg_data +3*i, "%02X ", data[i]);
		  }
		  dbg(" <-- %s", dbg_data);
		  kfree(dbg_data);
		*/
		/* END DEBUG */

		tty_insert_flip_string(tty, data, urb->actual_length);
		tty_flip_buffer_push(tty);
	}
	tty_kref_put(tty);
<<<<<<< HEAD
	/* someone sets the dev to 0 if the close method has been called */
	port->interrupt_in_urb->dev = port->serial->dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	result = usb_submit_urb(port->interrupt_in_urb, GFP_ATOMIC);
	dbg("%s - port %d Send read URB returns: %i",
			__func__, port->number, result);
}


static void kobil_write_callback(struct urb *purb)
=======
	unsigned char *data = urb->transfer_buffer;
	int status = urb->status;

	if (status) {
		dev_dbg(&port->dev, "%s - Read int status not zero: %d\n", __func__, status);
		return;
	}

	if (urb->actual_length) {
		usb_serial_debug_data(&port->dev, __func__, urb->actual_length,
									data);
		tty_insert_flip_string(&port->port, data, urb->actual_length);
		tty_flip_buffer_push(&port->port);
	}

	result = usb_submit_urb(port->interrupt_in_urb, GFP_ATOMIC);
	dev_dbg(&port->dev, "%s - Send read URB returns: %i\n", __func__, result);
}


static void kobil_write_int_callback(struct urb *urb)
>>>>>>> refs/remotes/origin/master
{
}


static int kobil_write(struct tty_struct *tty, struct usb_serial_port *port,
			const unsigned char *buf, int count)
{
	int length = 0;
	int result = 0;
	int todo = 0;
	struct kobil_private *priv;

	if (count == 0) {
<<<<<<< HEAD
		dbg("%s - port %d write request of 0 bytes",
						__func__, port->number);
=======
		dev_dbg(&port->dev, "%s - write request of 0 bytes\n", __func__);
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	priv = usb_get_serial_port_data(port);

	if (count > (KOBIL_BUF_LENGTH - priv->filled)) {
<<<<<<< HEAD
		dbg("%s - port %d Error: write request bigger than buffer size", __func__, port->number);
=======
		dev_dbg(&port->dev, "%s - Error: write request bigger than buffer size\n", __func__);
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}

	/* Copy data to buffer */
	memcpy(priv->buf + priv->filled, buf, count);
<<<<<<< HEAD
	usb_serial_debug_data(debug, &port->dev, __func__, count,
						priv->buf + priv->filled);
=======
	usb_serial_debug_data(&port->dev, __func__, count, priv->buf + priv->filled);
>>>>>>> refs/remotes/origin/master
	priv->filled = priv->filled + count;

	/* only send complete block. TWIN, KAAN SIM and adapter K
	   use the same protocol. */
	if (((priv->device_type != KOBIL_ADAPTER_B_PRODUCT_ID) && (priv->filled > 2) && (priv->filled >= (priv->buf[1] + 3))) ||
	     ((priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID) && (priv->filled > 3) && (priv->filled >= (priv->buf[2] + 4)))) {
		/* stop reading (except TWIN and KAAN SIM) */
		if ((priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID)
			|| (priv->device_type == KOBIL_ADAPTER_K_PRODUCT_ID))
			usb_kill_urb(port->interrupt_in_urb);

		todo = priv->filled - priv->cur_pos;

		while (todo > 0) {
			/* max 8 byte in one urb (endpoint size) */
<<<<<<< HEAD
			length = (todo < 8) ? todo : 8;
			/* copy data to transfer buffer */
			memcpy(port->write_urb->transfer_buffer,
					priv->buf + priv->cur_pos, length);
			usb_fill_int_urb(port->write_urb,
				  port->serial->dev,
				  usb_sndintpipe(port->serial->dev,
					priv->write_int_endpoint_address),
				  port->write_urb->transfer_buffer,
				  length,
				  kobil_write_callback,
				  port,
				  8
			);

			priv->cur_pos = priv->cur_pos + length;
			result = usb_submit_urb(port->write_urb, GFP_NOIO);
			dbg("%s - port %d Send write URB returns: %i",
					__func__, port->number, result);
=======
			length = min(todo, port->interrupt_out_size);
			/* copy data to transfer buffer */
			memcpy(port->interrupt_out_buffer,
					priv->buf + priv->cur_pos, length);
			port->interrupt_out_urb->transfer_buffer_length = length;

			priv->cur_pos = priv->cur_pos + length;
			result = usb_submit_urb(port->interrupt_out_urb, GFP_NOIO);
			dev_dbg(&port->dev, "%s - Send write URB returns: %i\n", __func__, result);
>>>>>>> refs/remotes/origin/master
			todo = priv->filled - priv->cur_pos;

			if (todo > 0)
				msleep(24);
		}

		priv->filled = 0;
		priv->cur_pos = 0;

<<<<<<< HEAD
<<<<<<< HEAD
		/* someone sets the dev to 0 if the close method
		   has been called */
		port->interrupt_in_urb->dev = port->serial->dev;

		/* start reading (except TWIN and KAAN SIM) */
		if (priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID ||
			priv->device_type == KOBIL_ADAPTER_K_PRODUCT_ID) {
			/* someone sets the dev to 0 if the close method has
			   been called */
			port->interrupt_in_urb->dev = port->serial->dev;

=======
		/* start reading (except TWIN and KAAN SIM) */
		if (priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID ||
			priv->device_type == KOBIL_ADAPTER_K_PRODUCT_ID) {
>>>>>>> refs/remotes/origin/cm-10.0
			result = usb_submit_urb(port->interrupt_in_urb,
								GFP_NOIO);
			dbg("%s - port %d Send read URB returns: %i",
					__func__, port->number, result);
=======
		/* start reading (except TWIN and KAAN SIM) */
		if (priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID ||
			priv->device_type == KOBIL_ADAPTER_K_PRODUCT_ID) {
			result = usb_submit_urb(port->interrupt_in_urb,
								GFP_NOIO);
			dev_dbg(&port->dev, "%s - Send read URB returns: %i\n", __func__, result);
>>>>>>> refs/remotes/origin/master
		}
	}
	return count;
}


static int kobil_write_room(struct tty_struct *tty)
{
<<<<<<< HEAD
	/* dbg("%s - port %d", __func__, port->number); */
=======
>>>>>>> refs/remotes/origin/master
	/* FIXME */
	return 8;
}


static int kobil_tiocmget(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct kobil_private *priv;
	int result;
	unsigned char *transfer_buffer;
	int transfer_buffer_length = 8;

	priv = usb_get_serial_port_data(port);
	if (priv->device_type == KOBIL_USBTWIN_PRODUCT_ID
			|| priv->device_type == KOBIL_KAAN_SIM_PRODUCT_ID) {
		/* This device doesn't support ioctl calls */
		return -EINVAL;
	}

	/* allocate memory for transfer buffer */
	transfer_buffer = kzalloc(transfer_buffer_length, GFP_KERNEL);
	if (!transfer_buffer)
		return -ENOMEM;

	result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_GetStatusLineState,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_IN,
			  0,
			  0,
			  transfer_buffer,
			  transfer_buffer_length,
			  KOBIL_TIMEOUT);

<<<<<<< HEAD
	dbg("%s - port %d Send get_status_line_state URB returns: %i. Statusline: %02x",
	    __func__, port->number, result, transfer_buffer[0]);
=======
	dev_dbg(&port->dev, "%s - Send get_status_line_state URB returns: %i. Statusline: %02x\n",
		__func__, result, transfer_buffer[0]);
>>>>>>> refs/remotes/origin/master

	result = 0;
	if ((transfer_buffer[0] & SUSBCR_GSL_DSR) != 0)
		result = TIOCM_DSR;
	kfree(transfer_buffer);
	return result;
}

static int kobil_tiocmset(struct tty_struct *tty,
			   unsigned int set, unsigned int clear)
{
	struct usb_serial_port *port = tty->driver_data;
<<<<<<< HEAD
=======
	struct device *dev = &port->dev;
>>>>>>> refs/remotes/origin/master
	struct kobil_private *priv;
	int result;
	int dtr = 0;
	int rts = 0;
	unsigned char *transfer_buffer;
	int transfer_buffer_length = 8;

	/* FIXME: locking ? */
	priv = usb_get_serial_port_data(port);
	if (priv->device_type == KOBIL_USBTWIN_PRODUCT_ID
		|| priv->device_type == KOBIL_KAAN_SIM_PRODUCT_ID) {
		/* This device doesn't support ioctl calls */
		return -EINVAL;
	}

	/* allocate memory for transfer buffer */
	transfer_buffer = kzalloc(transfer_buffer_length, GFP_KERNEL);
	if (!transfer_buffer)
		return -ENOMEM;

	if (set & TIOCM_RTS)
		rts = 1;
	if (set & TIOCM_DTR)
		dtr = 1;
	if (clear & TIOCM_RTS)
		rts = 0;
	if (clear & TIOCM_DTR)
		dtr = 0;

	if (priv->device_type == KOBIL_ADAPTER_B_PRODUCT_ID) {
		if (dtr != 0)
<<<<<<< HEAD
			dbg("%s - port %d Setting DTR",
						__func__, port->number);
		else
			dbg("%s - port %d Clearing DTR",
						__func__, port->number);
=======
			dev_dbg(dev, "%s - Setting DTR\n", __func__);
		else
			dev_dbg(dev, "%s - Clearing DTR\n", __func__);
>>>>>>> refs/remotes/origin/master
		result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_SetStatusLinesOrQueues,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_OUT,
			  ((dtr != 0) ? SUSBCR_SSL_SETDTR : SUSBCR_SSL_CLRDTR),
			  0,
			  transfer_buffer,
			  0,
			  KOBIL_TIMEOUT);
	} else {
		if (rts != 0)
<<<<<<< HEAD
			dbg("%s - port %d Setting RTS",
						__func__, port->number);
		else
			dbg("%s - port %d Clearing RTS",
						__func__, port->number);
=======
			dev_dbg(dev, "%s - Setting RTS\n", __func__);
		else
			dev_dbg(dev, "%s - Clearing RTS\n", __func__);
>>>>>>> refs/remotes/origin/master
		result = usb_control_msg(port->serial->dev,
			usb_rcvctrlpipe(port->serial->dev, 0),
			SUSBCRequest_SetStatusLinesOrQueues,
			USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_OUT,
			((rts != 0) ? SUSBCR_SSL_SETRTS : SUSBCR_SSL_CLRRTS),
			0,
			transfer_buffer,
			0,
			KOBIL_TIMEOUT);
	}
<<<<<<< HEAD
	dbg("%s - port %d Send set_status_line URB returns: %i",
					__func__, port->number, result);
=======
	dev_dbg(dev, "%s - Send set_status_line URB returns: %i\n", __func__, result);
>>>>>>> refs/remotes/origin/master
	kfree(transfer_buffer);
	return (result < 0) ? result : 0;
}

static void kobil_set_termios(struct tty_struct *tty,
			struct usb_serial_port *port, struct ktermios *old)
{
	struct kobil_private *priv;
	int result;
	unsigned short urb_val = 0;
<<<<<<< HEAD
	int c_cflag = tty->termios->c_cflag;
=======
	int c_cflag = tty->termios.c_cflag;
>>>>>>> refs/remotes/origin/master
	speed_t speed;

	priv = usb_get_serial_port_data(port);
	if (priv->device_type == KOBIL_USBTWIN_PRODUCT_ID ||
			priv->device_type == KOBIL_KAAN_SIM_PRODUCT_ID) {
		/* This device doesn't support ioctl calls */
<<<<<<< HEAD
		*tty->termios = *old;
=======
		tty_termios_copy_hw(&tty->termios, old);
>>>>>>> refs/remotes/origin/master
		return;
	}

	speed = tty_get_baud_rate(tty);
	switch (speed) {
	case 1200:
		urb_val = SUSBCR_SBR_1200;
		break;
	default:
		speed = 9600;
	case 9600:
		urb_val = SUSBCR_SBR_9600;
		break;
	}
	urb_val |= (c_cflag & CSTOPB) ? SUSBCR_SPASB_2StopBits :
							SUSBCR_SPASB_1StopBit;
	if (c_cflag & PARENB) {
		if  (c_cflag & PARODD)
			urb_val |= SUSBCR_SPASB_OddParity;
		else
			urb_val |= SUSBCR_SPASB_EvenParity;
	} else
		urb_val |= SUSBCR_SPASB_NoParity;
<<<<<<< HEAD
	tty->termios->c_cflag &= ~CMSPAR;
=======
	tty->termios.c_cflag &= ~CMSPAR;
>>>>>>> refs/remotes/origin/master
	tty_encode_baud_rate(tty, speed, speed);

	result = usb_control_msg(port->serial->dev,
		  usb_rcvctrlpipe(port->serial->dev, 0),
		  SUSBCRequest_SetBaudRateParityAndStopBits,
		  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_OUT,
		  urb_val,
		  0,
		  NULL,
		  0,
		  KOBIL_TIMEOUT
		);
}

static int kobil_ioctl(struct tty_struct *tty,
					unsigned int cmd, unsigned long arg)
{
	struct usb_serial_port *port = tty->driver_data;
	struct kobil_private *priv = usb_get_serial_port_data(port);
	unsigned char *transfer_buffer;
	int transfer_buffer_length = 8;
	int result;

	if (priv->device_type == KOBIL_USBTWIN_PRODUCT_ID ||
			priv->device_type == KOBIL_KAAN_SIM_PRODUCT_ID)
		/* This device doesn't support ioctl calls */
		return -ENOIOCTLCMD;

	switch (cmd) {
	case TCFLSH:
		transfer_buffer = kmalloc(transfer_buffer_length, GFP_KERNEL);
		if (!transfer_buffer)
			return -ENOBUFS;

		result = usb_control_msg(port->serial->dev,
			  usb_rcvctrlpipe(port->serial->dev, 0),
			  SUSBCRequest_Misc,
			  USB_TYPE_VENDOR | USB_RECIP_ENDPOINT | USB_DIR_OUT,
			  SUSBCR_MSC_ResetAllQueues,
			  0,
			  NULL, /* transfer_buffer, */
			  0,
			  KOBIL_TIMEOUT
			);

<<<<<<< HEAD
		dbg("%s - port %d Send reset_all_queues (FLUSH) URB returns: %i", __func__, port->number, result);
=======
		dev_dbg(&port->dev,
			"%s - Send reset_all_queues (FLUSH) URB returns: %i", __func__, result);
>>>>>>> refs/remotes/origin/master
		kfree(transfer_buffer);
		return (result < 0) ? -EIO: 0;
	default:
		return -ENOIOCTLCMD;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __init kobil_init(void)
{
	int retval;
	retval = usb_serial_register(&kobil_device);
	if (retval)
		goto failed_usb_serial_register;
	retval = usb_register(&kobil_driver);
	if (retval)
		goto failed_usb_register;

	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
	       DRIVER_DESC "\n");

	return 0;
failed_usb_register:
	usb_serial_deregister(&kobil_device);
failed_usb_serial_register:
	return retval;
}


static void __exit kobil_exit(void)
{
	usb_deregister(&kobil_driver);
	usb_serial_deregister(&kobil_device);
}

module_init(kobil_init);
module_exit(kobil_exit);
=======
module_usb_serial_driver(kobil_driver, serial_drivers);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_usb_serial_driver(serial_drivers, id_table);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
<<<<<<< HEAD

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");
=======
>>>>>>> refs/remotes/origin/master
