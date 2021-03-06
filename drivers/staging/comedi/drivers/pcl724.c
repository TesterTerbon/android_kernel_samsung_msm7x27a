/*
<<<<<<< HEAD
    comedi/drivers/pcl724.c

    Michal Dobes <dobes@tesnet.cz>

    hardware driver for Advantech cards:
     card:   PCL-724, PCL-722, PCL-731
     driver: pcl724,  pcl722,  pcl731
    and ADLink cards:
     card:   ACL-7122, ACL-7124, PET-48DIO
     driver: acl7122,  acl7124,  pet48dio

    Options for PCL-724, PCL-731, ACL-7124 and PET-48DIO:
     [0] - IO Base

    Options for PCL-722 and ACL-7122:
     [0] - IO Base
     [1] - IRQ (0=disable IRQ) IRQ isn't supported at this time!
     [2] -number of DIO:
	      0, 144: 144 DIO configuration
	      1,  96:  96 DIO configuration
*/
/*
Driver: pcl724
Description: Advantech PCL-724, PCL-722, PCL-731 ADLink ACL-7122, ACL-7124,
  PET-48DIO
Author: Michal Dobes <dobes@tesnet.cz>
Devices: [Advantech] PCL-724 (pcl724), PCL-722 (pcl722), PCL-731 (pcl731),
  [ADLink] ACL-7122 (acl7122), ACL-7124 (acl7124), PET-48DIO (pet48dio)
Status: untested

This is driver for digital I/O boards PCL-722/724/731 with 144/24/48 DIO
and for digital I/O boards ACL-7122/7124/PET-48DIO with 144/24/48 DIO.
It need 8255.o for operations and only immediate mode is supported.
See the source for configuration details.
*/
/*
 * check_driver overrides:
 *   struct comedi_insn
 */

#include "../comedidev.h"

#include <linux/ioport.h>
#include <linux/delay.h>

#include "8255.h"

#define PCL722_SIZE    32
#define PCL722_96_SIZE 16
#define PCL724_SIZE     4
#define PCL731_SIZE     8
#define PET48_SIZE      2

#define SIZE_8255	4

/* #define PCL724_IRQ   1  no IRQ support now */

static int pcl724_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int pcl724_detach(struct comedi_device *dev);

struct pcl724_board {

	const char *name;	/*  board name */
	int dio;		/*  num of DIO */
	int numofports;		/*  num of 8255 subdevices */
	unsigned int IRQbits;	/*  allowed interrupts */
	unsigned int io_range;	/*  len of IO space */
	char can_have96;
	char is_pet48;
};

static const struct pcl724_board boardtypes[] = {
	{"pcl724", 24, 1, 0x00fc, PCL724_SIZE, 0, 0,},
	{"pcl722", 144, 6, 0x00fc, PCL722_SIZE, 1, 0,},
	{"pcl731", 48, 2, 0x9cfc, PCL731_SIZE, 0, 0,},
	{"acl7122", 144, 6, 0x9ee8, PCL722_SIZE, 1, 0,},
	{"acl7124", 24, 1, 0x00fc, PCL724_SIZE, 0, 0,},
	{"pet48dio", 48, 2, 0x9eb8, PET48_SIZE, 0, 1,},
};

#define n_boardtypes (sizeof(boardtypes)/sizeof(struct pcl724_board))
#define this_board ((const struct pcl724_board *)dev->board_ptr)

static struct comedi_driver driver_pcl724 = {
	.driver_name = "pcl724",
	.module = THIS_MODULE,
	.attach = pcl724_attach,
	.detach = pcl724_detach,
	.board_name = &boardtypes[0].name,
	.num_names = n_boardtypes,
	.offset = sizeof(struct pcl724_board),
};

static int __init driver_pcl724_init_module(void)
{
	return comedi_driver_register(&driver_pcl724);
}

static void __exit driver_pcl724_cleanup_module(void)
{
	comedi_driver_unregister(&driver_pcl724);
}

module_init(driver_pcl724_init_module);
module_exit(driver_pcl724_cleanup_module);

static int subdev_8255_cb(int dir, int port, int data, unsigned long arg)
{
	unsigned long iobase = arg;

	if (dir) {
		outb(data, iobase + port);
		return 0;
	} else {
		return inb(iobase + port);
	}
}

static int subdev_8255mapped_cb(int dir, int port, int data,
=======
 * pcl724.c
 * Comedi driver for 8255 based ISA and PC/104 DIO boards
 *
 * Michal Dobes <dobes@tesnet.cz>
 */

/*
 * Driver: pcl724
 * Description: Comedi driver for 8255 based ISA DIO boards
 * Devices: (Advantech) PCL-724 [pcl724]
 *	    (Advantech) PCL-722 [pcl722]
 *	    (Advantech) PCL-731 [pcl731]
 *	    (ADLink) ACL-7122 [acl7122]
 *	    (ADLink) ACL-7124 [acl7124]
 *	    (ADLink) PET-48DIO [pet48dio]
 *	    (WinSystems) PCM-IO48 [pcmio48]
 * Author: Michal Dobes <dobes@tesnet.cz>
 * Status: untested
 *
 * Configuration options:
 *   [0] - IO Base
 *   [1] - IRQ (not supported)
 *   [2] - number of DIO (pcl722 and acl7122 boards)
 *	   0, 144: 144 DIO configuration
 *	   1,  96:  96 DIO configuration
 */

#include <linux/module.h>
#include "../comedidev.h"

#include "8255.h"

#define SIZE_8255	4

struct pcl724_board {
	const char *name;
	unsigned int io_range;
	unsigned int can_have96:1;
	unsigned int is_pet48:1;
	int numofports;
};

static const struct pcl724_board boardtypes[] = {
	{
		.name		= "pcl724",
		.io_range	= 0x04,
		.numofports	= 1,	/* 24 DIO channels */
	}, {
		.name		= "pcl722",
		.io_range	= 0x20,
		.can_have96	= 1,
		.numofports	= 6,	/* 144 (or 96) DIO channels */
	}, {
		.name		= "pcl731",
		.io_range	= 0x08,
		.numofports	= 2,	/* 48 DIO channels */
	}, {
		.name		= "acl7122",
		.io_range	= 0x20,
		.can_have96	= 1,
		.numofports	= 6,	/* 144 (or 96) DIO channels */
	}, {
		.name		= "acl7124",
		.io_range	= 0x04,
		.numofports	= 1,	/* 24 DIO channels */
	}, {
		.name		= "pet48dio",
		.io_range	= 0x02,
		.is_pet48	= 1,
		.numofports	= 2,	/* 48 DIO channels */
	}, {
		.name		= "pcmio48",
		.io_range	= 0x08,
		.numofports	= 2,	/* 48 DIO channels */
	},
};

static int pcl724_8255mapped_io(int dir, int port, int data,
>>>>>>> refs/remotes/origin/master
				unsigned long iobase)
{
	int movport = SIZE_8255 * (iobase >> 12);

	iobase &= 0x0fff;

	if (dir) {
		outb(port + movport, iobase);
		outb(data, iobase + 1);
		return 0;
	} else {
		outb(port + movport, iobase);
		return inb(iobase + 1);
	}
}

<<<<<<< HEAD
static int pcl724_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	unsigned long iobase;
	unsigned int iorange;
	int ret, i, n_subdevices;
#ifdef PCL724_IRQ
	unsigned int irq;
#endif

	iobase = it->options[0];
	iorange = this_board->io_range;
	if ((this_board->can_have96) && ((it->options[1] == 1)
					 || (it->options[1] == 96)))
		iorange = PCL722_96_SIZE; /* PCL-724 in 96 DIO configuration */
	printk(KERN_INFO "comedi%d: pcl724: board=%s, 0x%03lx ", dev->minor,
	       this_board->name, iobase);
	if (!request_region(iobase, iorange, "pcl724")) {
		printk("I/O port conflict\n");
		return -EIO;
	}

	dev->iobase = iobase;

	dev->board_name = this_board->name;

#ifdef PCL724_IRQ
	irq = 0;
	if (this_board->IRQbits != 0) {	/* board support IRQ */
		irq = it->options[1];
		if (irq) {	/* we want to use IRQ */
			if (((1 << irq) & this_board->IRQbits) == 0) {
				printk(KERN_WARNING
				       ", IRQ %u is out of allowed range, "
				       "DISABLING IT", irq);
				irq = 0;	/* Bad IRQ */
			} else {
				if (request_irq
				    (irq, interrupt_pcl724, 0, "pcl724", dev)) {
					printk(KERN_WARNING
					       ", unable to allocate IRQ %u, "
					       "DISABLING IT", irq);
					irq = 0;	/* Can't use IRQ */
				} else {
					printk(", irq=%u", irq);
				}
			}
		}
	}

	dev->irq = irq;
#endif

	printk("\n");

	n_subdevices = this_board->numofports;
	if ((this_board->can_have96) && ((it->options[1] == 1)
					 || (it->options[1] == 96)))
		n_subdevices = 4;	/*  PCL-724 in 96 DIO configuration */

	ret = alloc_subdevices(dev, n_subdevices);
	if (ret < 0)
		return ret;

	for (i = 0; i < dev->n_subdevices; i++) {
		if (this_board->is_pet48) {
			subdev_8255_init(dev, dev->subdevices + i,
					 subdev_8255mapped_cb,
					 (unsigned long)(dev->iobase +
							 i * 0x1000));
		} else
			subdev_8255_init(dev, dev->subdevices + i,
					 subdev_8255_cb,
					 (unsigned long)(dev->iobase +
							 SIZE_8255 * i));
=======
static int pcl724_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it)
{
	const struct pcl724_board *board = comedi_board(dev);
	struct comedi_subdevice *s;
	unsigned long iobase;
	unsigned int iorange;
	int n_subdevices;
	int ret;
	int i;

	iorange = board->io_range;
	n_subdevices = board->numofports;

	/* Handle PCL-724 in 96 DIO configuration */
	if (board->can_have96 &&
	    (it->options[2] == 1 || it->options[2] == 96)) {
		iorange = 0x10;
		n_subdevices = 4;
	}

	ret = comedi_request_region(dev, it->options[0], iorange);
	if (ret)
		return ret;

	ret = comedi_alloc_subdevices(dev, n_subdevices);
	if (ret)
		return ret;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
		if (board->is_pet48) {
			iobase = dev->iobase + (i * 0x1000);
			ret = subdev_8255_init(dev, s, pcl724_8255mapped_io,
					       iobase);
		} else {
			iobase = dev->iobase + (i * SIZE_8255);
			ret = subdev_8255_init(dev, s, NULL, iobase);
		}
		if (ret)
			return ret;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
static int pcl724_detach(struct comedi_device *dev)
{
	int i;

	/* printk("comedi%d: pcl724: remove\n",dev->minor); */

	for (i = 0; i < dev->n_subdevices; i++)
		subdev_8255_cleanup(dev, dev->subdevices + i);

#ifdef PCL724_IRQ
	if (dev->irq)
		free_irq(dev->irq, dev);
#endif

	release_region(dev->iobase, this_board->io_range);

	return 0;
}

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
=======
static struct comedi_driver pcl724_driver = {
	.driver_name	= "pcl724",
	.module		= THIS_MODULE,
	.attach		= pcl724_attach,
	.detach		= comedi_legacy_detach,
	.board_name	= &boardtypes[0].name,
	.num_names	= ARRAY_SIZE(boardtypes),
	.offset		= sizeof(struct pcl724_board),
};
module_comedi_driver(pcl724_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi driver for 8255 based ISA and PC/104 DIO boards");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");
