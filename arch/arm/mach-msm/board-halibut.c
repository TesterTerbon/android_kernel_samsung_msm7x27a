/* linux/arch/arm/mach-msm/board-halibut.c
 *
 * Copyright (C) 2007 Google, Inc.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/delay.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
#include <asm/setup.h>

#include <mach/irqs.h>
<<<<<<< HEAD
#include <mach/board.h>
=======
>>>>>>> refs/remotes/origin/master
#include <mach/msm_iomap.h>

#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#include "devices.h"
<<<<<<< HEAD
=======
#include "common.h"
>>>>>>> refs/remotes/origin/master

static struct resource smc91x_resources[] = {
	[0] = {
		.start	= 0x9C004300,
		.end	= 0x9C004400,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= MSM_GPIO_TO_INT(49),
		.end	= MSM_GPIO_TO_INT(49),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.resource	= smc91x_resources,
};

static struct platform_device *devices[] __initdata = {
<<<<<<< HEAD
=======
	&msm_clock_7x01a,
	&msm_device_gpio_7201,
>>>>>>> refs/remotes/origin/master
	&msm_device_uart3,
	&msm_device_smd,
	&msm_device_nand,
	&msm_device_hsusb,
	&msm_device_i2c,
	&smc91x_device,
};

<<<<<<< HEAD
extern struct sys_timer msm_timer;

<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void __init halibut_init_early(void)
{
	arch_ioremap_caller = __msm_ioremap_caller;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void __init halibut_init_irq(void)
{
	msm_init_irq();
}

static void __init halibut_init(void)
{
	platform_add_devices(devices, ARRAY_SIZE(devices));
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __init halibut_fixup(struct machine_desc *desc, struct tag *tags,
				 char **cmdline, struct meminfo *mi)
{
	mi->nr_banks=1;
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].size = (101*1024*1024);
=======
static void __init halibut_fixup(struct tag *tags, char **cmdline,
				 struct meminfo *mi)
{
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __init halibut_fixup(struct tag *tags, char **cmdline,
				 struct meminfo *mi)
{
>>>>>>> refs/remotes/origin/master
}

static void __init halibut_map_io(void)
{
	msm_map_common_io();
<<<<<<< HEAD
	msm_clock_init(msm_clocks_7x01a, msm_num_clocks_7x01a);
}

MACHINE_START(HALIBUT, "Halibut Board (QCT SURF7200A)")
<<<<<<< HEAD
	.boot_params	= 0x10000100,
	.fixup		= halibut_fixup,
	.map_io		= halibut_map_io,
=======
=======
}

static void __init halibut_init_late(void)
{
	smd_debugfs_init();
}

MACHINE_START(HALIBUT, "Halibut Board (QCT SURF7200A)")
>>>>>>> refs/remotes/origin/master
	.atag_offset	= 0x100,
	.fixup		= halibut_fixup,
	.map_io		= halibut_map_io,
	.init_early	= halibut_init_early,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	.init_irq	= halibut_init_irq,
	.init_machine	= halibut_init,
	.timer		= &msm_timer,
=======
	.init_irq	= halibut_init_irq,
	.init_machine	= halibut_init,
	.init_late	= halibut_init_late,
	.init_time	= msm7x01_timer_init,
>>>>>>> refs/remotes/origin/master
MACHINE_END
