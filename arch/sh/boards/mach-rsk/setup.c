/*
 * Renesas Technology Europe RSK+ Support.
 *
 * Copyright (C) 2008 Paul Mundt
 * Copyright (C) 2008 Peter Griffin <pgriffin@mpc-data.co.uk>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MTD
#include <linux/mtd/map.h>
#endif
#include <asm/machvec.h>
#include <asm/io.h>

=======
#include <linux/mtd/map.h>
#include <asm/machvec.h>
#include <asm/io.h>

static const char *part_probes[] = { "cmdlinepart", NULL };

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/mtd/map.h>
#include <linux/regulator/fixed.h>
#include <linux/regulator/machine.h>
#include <asm/machvec.h>
#include <asm/io.h>

/* Dummy supplies, where voltage doesn't matter */
static struct regulator_consumer_supply dummy_supplies[] = {
	REGULATOR_SUPPLY("vddvario", "smsc911x"),
	REGULATOR_SUPPLY("vdd33a", "smsc911x"),
};

static const char *part_probes[] = { "cmdlinepart", NULL };

>>>>>>> refs/remotes/origin/master
static struct mtd_partition rsk_partitions[] = {
	{
		.name		= "Bootloader",
		.offset		= 0x00000000,
		.size		= 0x00040000,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "Kernel",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 0x001c0000,
	}, {
		.name		= "Flash_FS",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= MTDPART_SIZ_FULL,
	}
};

static struct physmap_flash_data flash_data = {
<<<<<<< HEAD
<<<<<<< HEAD
	.parts		= rsk_partitions,
	.nr_parts	= ARRAY_SIZE(rsk_partitions),
	.width		= 2,
=======
=======
>>>>>>> refs/remotes/origin/master
	.parts			= rsk_partitions,
	.nr_parts		= ARRAY_SIZE(rsk_partitions),
	.width			= 2,
	.part_probe_types	= part_probes,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static struct resource flash_resource = {
	.start		= 0x20000000,
	.end		= 0x20400000,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device flash_device = {
	.name		= "physmap-flash",
	.id		= -1,
	.resource	= &flash_resource,
	.num_resources	= 1,
	.dev		= {
		.platform_data = &flash_data,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MTD
static const char *probes[] = { "cmdlinepart", NULL };

static struct map_info rsk_flash_map = {
	.name		= "RSK+ Flash",
	.size		= 0x400000,
	.bankwidth	= 2,
};

static struct mtd_info *flash_mtd;

static struct mtd_partition *parsed_partitions;

static void __init set_mtd_partitions(void)
{
	int nr_parts = 0;

	simple_map_init(&rsk_flash_map);
	flash_mtd = do_map_probe("cfi_probe", &rsk_flash_map);
	nr_parts = parse_mtd_partitions(flash_mtd, probes,
					&parsed_partitions, 0);
	/* If there is no partition table, used the hard coded table */
	if (nr_parts > 0) {
		flash_data.nr_parts = nr_parts;
		flash_data.parts = parsed_partitions;
	}
}
#else
static inline void set_mtd_partitions(void) {}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct platform_device *rsk_devices[] __initdata = {
	&flash_device,
};

static int __init rsk_devices_setup(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	set_mtd_partitions();
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	regulator_register_fixed(0, dummy_supplies, ARRAY_SIZE(dummy_supplies));

>>>>>>> refs/remotes/origin/master
	return platform_add_devices(rsk_devices,
				    ARRAY_SIZE(rsk_devices));
}
device_initcall(rsk_devices_setup);

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_rsk __initmv = {
	.mv_name        = "RSK+",
};
