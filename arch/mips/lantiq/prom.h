/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 */

#ifndef _LTQ_PROM_H__
#define _LTQ_PROM_H__

#define LTQ_SYS_TYPE_LEN	0x100
<<<<<<< HEAD
=======
#define LTQ_SYS_REV_LEN		0x10
>>>>>>> refs/remotes/origin/master

struct ltq_soc_info {
	unsigned char *name;
	unsigned int rev;
<<<<<<< HEAD
	unsigned int partnum;
	unsigned int type;
	unsigned char sys_type[LTQ_SYS_TYPE_LEN];
};

extern void ltq_soc_detect(struct ltq_soc_info *i);
extern void ltq_soc_setup(void);
=======
	unsigned char rev_type[LTQ_SYS_REV_LEN];
	unsigned int srev;
	unsigned int partnum;
	unsigned int type;
	unsigned char sys_type[LTQ_SYS_TYPE_LEN];
	unsigned char *compatible;
};

extern void ltq_soc_detect(struct ltq_soc_info *i);
extern void ltq_soc_init(void);

extern struct boot_param_header __dtb_start;
>>>>>>> refs/remotes/origin/master

#endif
