/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004, 2005 Ralf Baechle
 * Copyright (C) 2005 MIPS Technologies, Inc.
 */
#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/oprofile.h>
#include <linux/smp.h>
#include <asm/cpu-info.h>
<<<<<<< HEAD
=======
#include <asm/cpu-type.h>
>>>>>>> refs/remotes/origin/master

#include "op_impl.h"

extern struct op_mips_model op_model_mipsxx_ops __weak;
<<<<<<< HEAD
extern struct op_mips_model op_model_rm9000_ops __weak;
=======
>>>>>>> refs/remotes/origin/master
extern struct op_mips_model op_model_loongson2_ops __weak;

static struct op_mips_model *model;

static struct op_counter_config ctr[20];

static int op_mips_setup(void)
{
	/* Pre-compute the values to stuff in the hardware registers.  */
	model->reg_setup(ctr);

<<<<<<< HEAD
	/* Configure the registers on all cpus.  */
	on_each_cpu(model->cpu_setup, NULL, 1);

        return 0;
}

static int op_mips_create_files(struct super_block *sb, struct dentry *root)
=======
	/* Configure the registers on all cpus.	 */
	on_each_cpu(model->cpu_setup, NULL, 1);

	return 0;
}

static int op_mips_create_files(struct dentry *root)
>>>>>>> refs/remotes/origin/master
{
	int i;

	for (i = 0; i < model->num_counters; ++i) {
		struct dentry *dir;
		char buf[4];

		snprintf(buf, sizeof buf, "%d", i);
<<<<<<< HEAD
		dir = oprofilefs_mkdir(sb, root, buf);

		oprofilefs_create_ulong(sb, dir, "enabled", &ctr[i].enabled);
		oprofilefs_create_ulong(sb, dir, "event", &ctr[i].event);
		oprofilefs_create_ulong(sb, dir, "count", &ctr[i].count);
		oprofilefs_create_ulong(sb, dir, "kernel", &ctr[i].kernel);
		oprofilefs_create_ulong(sb, dir, "user", &ctr[i].user);
		oprofilefs_create_ulong(sb, dir, "exl", &ctr[i].exl);
		/* Dummy.  */
		oprofilefs_create_ulong(sb, dir, "unit_mask", &ctr[i].unit_mask);
=======
		dir = oprofilefs_mkdir(root, buf);

		oprofilefs_create_ulong(dir, "enabled", &ctr[i].enabled);
		oprofilefs_create_ulong(dir, "event", &ctr[i].event);
		oprofilefs_create_ulong(dir, "count", &ctr[i].count);
		oprofilefs_create_ulong(dir, "kernel", &ctr[i].kernel);
		oprofilefs_create_ulong(dir, "user", &ctr[i].user);
		oprofilefs_create_ulong(dir, "exl", &ctr[i].exl);
		/* Dummy.  */
		oprofilefs_create_ulong(dir, "unit_mask", &ctr[i].unit_mask);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

static int op_mips_start(void)
{
	on_each_cpu(model->cpu_start, NULL, 1);

	return 0;
}

static void op_mips_stop(void)
{
	/* Disable performance monitoring for all counters.  */
	on_each_cpu(model->cpu_stop, NULL, 1);
}

int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	struct op_mips_model *lmodel = NULL;
	int res;

	switch (current_cpu_type()) {
	case CPU_5KC:
<<<<<<< HEAD
=======
	case CPU_M14KC:
	case CPU_M14KEC:
>>>>>>> refs/remotes/origin/master
	case CPU_20KC:
	case CPU_24K:
	case CPU_25KF:
	case CPU_34K:
	case CPU_1004K:
	case CPU_74K:
<<<<<<< HEAD
=======
	case CPU_LOONGSON1:
>>>>>>> refs/remotes/origin/master
	case CPU_SB1:
	case CPU_SB1A:
	case CPU_R10000:
	case CPU_R12000:
	case CPU_R14000:
<<<<<<< HEAD
		lmodel = &op_model_mipsxx_ops;
		break;

	case CPU_RM9000:
		lmodel = &op_model_rm9000_ops;
		break;
=======
	case CPU_XLR:
		lmodel = &op_model_mipsxx_ops;
		break;

>>>>>>> refs/remotes/origin/master
	case CPU_LOONGSON2:
		lmodel = &op_model_loongson2_ops;
		break;
	};

	if (!lmodel)
		return -ENODEV;

	res = lmodel->init();
	if (res)
		return res;

	model = lmodel;

	ops->create_files	= op_mips_create_files;
	ops->setup		= op_mips_setup;
<<<<<<< HEAD
	//ops->shutdown         = op_mips_shutdown;
	ops->start		= op_mips_start;
	ops->stop		= op_mips_stop;
	ops->cpu_type		= lmodel->cpu_type;
<<<<<<< HEAD
=======
	ops->backtrace		= op_mips_backtrace;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	//ops->shutdown		= op_mips_shutdown;
	ops->start		= op_mips_start;
	ops->stop		= op_mips_stop;
	ops->cpu_type		= lmodel->cpu_type;
	ops->backtrace		= op_mips_backtrace;
>>>>>>> refs/remotes/origin/master

	printk(KERN_INFO "oprofile: using %s performance monitoring.\n",
	       lmodel->cpu_type);

	return 0;
}

void oprofile_arch_exit(void)
{
	if (model)
		model->exit();
}
