<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ARCH_ARM_MACH_MSM_RPM_RESOURCES_H
#define __ARCH_ARM_MACH_MSM_RPM_RESOURCES_H

#include <mach/rpm.h>
#include "pm.h"

<<<<<<< HEAD
<<<<<<< HEAD

struct msm_rpmrs_limits {
	uint32_t pxo;
	uint32_t l2_cache;
	uint32_t vdd_mem_upper_bound;
	uint32_t vdd_mem;
	uint32_t vdd_dig_upper_bound;
	uint32_t vdd_dig;

	uint32_t latency_us[NR_CPUS];
	uint32_t power[NR_CPUS];
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
enum {
	MSM_RPMRS_ID_PXO_CLK = 0,
	MSM_RPMRS_ID_L2_CACHE_CTL = 1,
	MSM_RPMRS_ID_VDD_DIG_0 = 2,
	MSM_RPMRS_ID_VDD_DIG_1 = 3,
	MSM_RPMRS_ID_VDD_MEM_0 = 4,
	MSM_RPMRS_ID_VDD_MEM_1 = 5,
	MSM_RPMRS_ID_RPM_CTL = 6,
	MSM_RPMRS_ID_LAST,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

enum {
	MSM_RPMRS_PXO_OFF = 0,
	MSM_RPMRS_PXO_ON = 1,
};

enum {
	MSM_RPMRS_L2_CACHE_HSFS_OPEN = 0,
	MSM_RPMRS_L2_CACHE_GDHS = 1,
	MSM_RPMRS_L2_CACHE_RETENTION = 2,
	MSM_RPMRS_L2_CACHE_ACTIVE = 3,
};

enum {
	MSM_RPMRS_MASK_RPM_CTL_CPU_HALT = 1,
	MSM_RPMRS_MASK_RPM_CTL_MULTI_TIER = 2,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
enum {
	MSM_RPMRS_VDD_MEM_RET_LOW = 0,
	MSM_RPMRS_VDD_MEM_RET_HIGH = 1,
	MSM_RPMRS_VDD_MEM_ACTIVE = 2,
	MSM_RPMRS_VDD_MEM_MAX = 3,
	MSM_RPMRS_VDD_MEM_LAST,
};

enum {
	MSM_RPMRS_VDD_DIG_RET_LOW = 0,
	MSM_RPMRS_VDD_DIG_RET_HIGH = 1,
	MSM_RPMRS_VDD_DIG_ACTIVE = 2,
	MSM_RPMRS_VDD_DIG_MAX = 3,
	MSM_RPMRS_VDD_DIG_LAST,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define MSM_RPMRS_LIMITS(_pxo, _l2, _vdd_upper_b, _vdd) { \
	MSM_RPMRS_PXO_##_pxo, \
	MSM_RPMRS_L2_CACHE_##_l2, \
	MSM_RPMRS_VDD_MEM_##_vdd_upper_b, \
	MSM_RPMRS_VDD_MEM_##_vdd, \
	MSM_RPMRS_VDD_DIG_##_vdd_upper_b, \
	MSM_RPMRS_VDD_DIG_##_vdd, \
	{0}, {0}, \
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct msm_rpmrs_limits {
	uint32_t pxo;
	uint32_t l2_cache;
	uint32_t vdd_mem_upper_bound;
	uint32_t vdd_mem;
	uint32_t vdd_dig_upper_bound;
	uint32_t vdd_dig;

	uint32_t latency_us[NR_CPUS];
	uint32_t power[NR_CPUS];
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct msm_rpmrs_level {
	enum msm_pm_sleep_mode sleep_mode;
	struct msm_rpmrs_limits rs_limits;
	bool available;
	uint32_t latency_us;
	uint32_t steady_state_power;
	uint32_t energy_overhead;
	uint32_t time_overhead_us;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct msm_rpmrs_platform_data {
	struct msm_rpmrs_level *levels;
	unsigned int num_levels;
	unsigned int vdd_mem_levels[MSM_RPMRS_VDD_MEM_LAST];
	unsigned int vdd_dig_levels[MSM_RPMRS_VDD_DIG_LAST];
	unsigned int vdd_mask;
	unsigned int rpmrs_target_id[MSM_RPMRS_ID_LAST];
};

#if defined(CONFIG_MSM_RPM)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
int msm_rpmrs_set(int ctx, struct msm_rpm_iv_pair *req, int count);
int msm_rpmrs_set_noirq(int ctx, struct msm_rpm_iv_pair *req, int count);
int msm_rpmrs_set_bits_noirq(int ctx, struct msm_rpm_iv_pair *req, int count,
			int *mask);

static inline int msm_rpmrs_set_nosleep(
	int ctx, struct msm_rpm_iv_pair *req, int count)
{
	unsigned long flags;
	int rc;

	local_irq_save(flags);
	rc = msm_rpmrs_set_noirq(ctx, req, count);
	local_irq_restore(flags);

	return rc;
}

int msm_rpmrs_clear(int ctx, struct msm_rpm_iv_pair *req, int count);
int msm_rpmrs_clear_noirq(int ctx, struct msm_rpm_iv_pair *req, int count);

static inline int msm_rpmrs_clear_nosleep(
	int ctx, struct msm_rpm_iv_pair *req, int count)
{
	unsigned long flags;
	int rc;

	local_irq_save(flags);
	rc = msm_rpmrs_clear_noirq(ctx, req, count);
	local_irq_restore(flags);

	return rc;
}

void msm_rpmrs_show_resources(void);
<<<<<<< HEAD
<<<<<<< HEAD

struct msm_rpmrs_limits *msm_rpmrs_lowest_limits(
	bool from_idle, enum msm_pm_sleep_mode sleep_mode, uint32_t latency_us,
	uint32_t sleep_us);

int msm_rpmrs_enter_sleep(uint32_t sclk_count, struct msm_rpmrs_limits *limits,
		bool from_idle, bool notify_rpm);
void msm_rpmrs_exit_sleep(struct msm_rpmrs_limits *limits, bool from_idle,
		bool notify_rpm, bool collapsed);

int msm_rpmrs_levels_init(struct msm_rpmrs_level *levels, int size);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
int msm_rpmrs_levels_init(struct msm_rpmrs_platform_data *data);

#else

static inline int msm_rpmrs_set(int ctx, struct msm_rpm_iv_pair *req,
				int count)
{
	return -ENODEV;
}

static inline int msm_rpmrs_set_noirq(int ctx, struct msm_rpm_iv_pair *req,
					int count)
{
	return -ENODEV;
}

static inline int msm_rpmrs_set_bits_noirq(int ctx, struct msm_rpm_iv_pair *req,
			int count, int *mask)
{
	return -ENODEV;
}

static inline int msm_rpmrs_set_nosleep(
	int ctx, struct msm_rpm_iv_pair *req, int count)
{
	return -ENODEV;
}

static inline int msm_rpmrs_clear(int ctx, struct msm_rpm_iv_pair *req,
					int count)
{
	return -ENODEV;
}

static inline int msm_rpmrs_clear_noirq(int ctx, struct msm_rpm_iv_pair *req,
						int count)
{
	return -ENODEV;
}

static inline int msm_rpmrs_clear_nosleep(
	int ctx, struct msm_rpm_iv_pair *req, int count)
{
	return -ENODEV;
}

static inline struct msm_rpmrs_limits *msm_rpmrs_lowest_limits(
	bool from_idle, enum msm_pm_sleep_mode sleep_mode, uint32_t latency_us,
	uint32_t sleep_us)
{
	return NULL;
}

static inline int msm_rpmrs_enter_sleep(uint32_t sclk_count,
	struct msm_rpmrs_limits *limits, bool from_idle, bool notify_rpm)
{
	return -ENODEV;
}

static inline void msm_rpmrs_exit_sleep(struct msm_rpmrs_limits *limits,
		bool from_idle, bool notify_rpm, bool collapsed)
{
	return;
}

static inline int msm_rpmrs_levels_init(struct msm_rpmrs_platform_data *data)
{
	return -ENODEV;
}

#endif /* CONFIG_MSM_RPM */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

#endif /* __ARCH_ARM_MACH_MSM_RPM_RESOURCES_H */
