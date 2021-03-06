<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2009-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
=======
/*
 * Copyright (c) 2011 Bosch Sensortec GmbH
 * Copyright (c) 2011 Unixphere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2009-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 */
#ifndef LINUX_BMA150_MODULE_H
#define LINUX_BMA150_MODULE_H

/**
 * struct bma150_platform_data - data to set up bma150 driver
 *
 * @setup: optional callback to activate the driver.
 * @teardown: optional callback to invalidate the driver.
 *
**/

struct bma150_platform_data {
	int (*setup)(struct device *);
	void (*teardown)(struct device *);
	int (*power_on)(void);
	void (*power_off)(void);
};

#endif /* LINUX_BMA150_MODULE_H */
=======
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> refs/remotes/origin/cm-11.0
 */
#ifndef LINUX_BMA150_MODULE_H
#define LINUX_BMA150_MODULE_H

<<<<<<< HEAD
#ifndef _BMA150_H_
#define _BMA150_H_

#define BMA150_DRIVER		"bma150"

#define BMA150_RANGE_2G		0
#define BMA150_RANGE_4G		1
#define BMA150_RANGE_8G		2

#define BMA150_BW_25HZ		0
#define BMA150_BW_50HZ		1
#define BMA150_BW_100HZ		2
#define BMA150_BW_190HZ		3
#define BMA150_BW_375HZ		4
#define BMA150_BW_750HZ		5
#define BMA150_BW_1500HZ	6

struct bma150_cfg {
	bool any_motion_int;		/* Set to enable any-motion interrupt */
	bool hg_int;			/* Set to enable high-G interrupt */
	bool lg_int;			/* Set to enable low-G interrupt */
	unsigned char any_motion_dur;	/* Any-motion duration */
	unsigned char any_motion_thres;	/* Any-motion threshold */
	unsigned char hg_hyst;		/* High-G hysterisis */
	unsigned char hg_dur;		/* High-G duration */
	unsigned char hg_thres;		/* High-G threshold */
	unsigned char lg_hyst;		/* Low-G hysterisis */
	unsigned char lg_dur;		/* Low-G duration */
	unsigned char lg_thres;		/* Low-G threshold */
	unsigned char range;		/* one of BMA0150_RANGE_xxx */
	unsigned char bandwidth;	/* one of BMA0150_BW_xxx */
};
=======
/**
 * struct bma150_platform_data - data to set up bma150 driver
 *
 * @setup: optional callback to activate the driver.
 * @teardown: optional callback to invalidate the driver.
 *
**/
>>>>>>> refs/remotes/origin/cm-11.0

struct bma150_platform_data {
	int (*setup)(struct device *);
	void (*teardown)(struct device *);
	int (*power_on)(void);
	void (*power_off)(void);
};

<<<<<<< HEAD
#endif /* _BMA150_H_ */
>>>>>>> refs/remotes/origin/master
=======
#endif /* LINUX_BMA150_MODULE_H */
>>>>>>> refs/remotes/origin/cm-11.0
