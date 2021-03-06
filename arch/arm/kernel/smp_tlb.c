/*
 *  linux/arch/arm/kernel/smp_tlb.c
 *
 *  Copyright (C) 2002 ARM Limited, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/preempt.h>
#include <linux/smp.h>

#include <asm/smp_plat.h>
#include <asm/tlbflush.h>
<<<<<<< HEAD

<<<<<<< HEAD
static void on_each_cpu_mask(void (*func)(void *), void *info, int wait,
	const struct cpumask *mask)
{
	preempt_disable();

	smp_call_function_many(mask, func, info, wait);
	if (cpumask_test_cpu(smp_processor_id(), mask))
		func(info);

	preempt_enable();
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/mmu_context.h>

>>>>>>> refs/remotes/origin/master
/**********************************************************************/

/*
 * TLB operations
 */
struct tlb_args {
	struct vm_area_struct *ta_vma;
	unsigned long ta_start;
	unsigned long ta_end;
};

static inline void ipi_flush_tlb_all(void *ignored)
{
	local_flush_tlb_all();
}

static inline void ipi_flush_tlb_mm(void *arg)
{
	struct mm_struct *mm = (struct mm_struct *)arg;

	local_flush_tlb_mm(mm);
}

static inline void ipi_flush_tlb_page(void *arg)
{
	struct tlb_args *ta = (struct tlb_args *)arg;

	local_flush_tlb_page(ta->ta_vma, ta->ta_start);
}

static inline void ipi_flush_tlb_kernel_page(void *arg)
{
	struct tlb_args *ta = (struct tlb_args *)arg;

	local_flush_tlb_kernel_page(ta->ta_start);
}

static inline void ipi_flush_tlb_range(void *arg)
{
	struct tlb_args *ta = (struct tlb_args *)arg;

	local_flush_tlb_range(ta->ta_vma, ta->ta_start, ta->ta_end);
}

static inline void ipi_flush_tlb_kernel_range(void *arg)
{
	struct tlb_args *ta = (struct tlb_args *)arg;

	local_flush_tlb_kernel_range(ta->ta_start, ta->ta_end);
}

<<<<<<< HEAD
=======
static inline void ipi_flush_bp_all(void *ignored)
{
	local_flush_bp_all();
}

#ifdef CONFIG_ARM_ERRATA_798181
bool (*erratum_a15_798181_handler)(void);

static bool erratum_a15_798181_partial(void)
{
	asm("mcr p15, 0, %0, c8, c3, 1" : : "r" (0));
	dsb(ish);
	return false;
}

static bool erratum_a15_798181_broadcast(void)
{
	asm("mcr p15, 0, %0, c8, c3, 1" : : "r" (0));
	dsb(ish);
	return true;
}

void erratum_a15_798181_init(void)
{
	unsigned int midr = read_cpuid_id();
	unsigned int revidr = read_cpuid(CPUID_REVIDR);

	/* Cortex-A15 r0p0..r3p2 w/o ECO fix affected */
	if ((midr & 0xff0ffff0) != 0x410fc0f0 || midr > 0x413fc0f2 ||
	    (revidr & 0x210) == 0x210) {
		return;
	}
	if (revidr & 0x10)
		erratum_a15_798181_handler = erratum_a15_798181_partial;
	else
		erratum_a15_798181_handler = erratum_a15_798181_broadcast;
}
#endif

static void ipi_flush_tlb_a15_erratum(void *arg)
{
	dmb();
}

static void broadcast_tlb_a15_erratum(void)
{
	if (!erratum_a15_798181())
		return;

	smp_call_function(ipi_flush_tlb_a15_erratum, NULL, 1);
}

static void broadcast_tlb_mm_a15_erratum(struct mm_struct *mm)
{
	int this_cpu;
	cpumask_t mask = { CPU_BITS_NONE };

	if (!erratum_a15_798181())
		return;

	this_cpu = get_cpu();
	a15_erratum_get_cpumask(this_cpu, mm, &mask);
	smp_call_function_many(&mask, ipi_flush_tlb_a15_erratum, NULL, 1);
	put_cpu();
}

>>>>>>> refs/remotes/origin/master
void flush_tlb_all(void)
{
	if (tlb_ops_need_broadcast())
		on_each_cpu(ipi_flush_tlb_all, NULL, 1);
	else
<<<<<<< HEAD
		local_flush_tlb_all();
=======
		__flush_tlb_all();
	broadcast_tlb_a15_erratum();
>>>>>>> refs/remotes/origin/master
}

void flush_tlb_mm(struct mm_struct *mm)
{
	if (tlb_ops_need_broadcast())
<<<<<<< HEAD
<<<<<<< HEAD
		on_each_cpu_mask(ipi_flush_tlb_mm, mm, 1, mm_cpumask(mm));
=======
		on_each_cpu_mask(mm_cpumask(mm), ipi_flush_tlb_mm, mm, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	else
		local_flush_tlb_mm(mm);
=======
		on_each_cpu_mask(mm_cpumask(mm), ipi_flush_tlb_mm, mm, 1);
	else
		__flush_tlb_mm(mm);
	broadcast_tlb_mm_a15_erratum(mm);
>>>>>>> refs/remotes/origin/master
}

void flush_tlb_page(struct vm_area_struct *vma, unsigned long uaddr)
{
	if (tlb_ops_need_broadcast()) {
		struct tlb_args ta;
		ta.ta_vma = vma;
		ta.ta_start = uaddr;
<<<<<<< HEAD
<<<<<<< HEAD
		on_each_cpu_mask(ipi_flush_tlb_page, &ta, 1, mm_cpumask(vma->vm_mm));
=======
		on_each_cpu_mask(mm_cpumask(vma->vm_mm), ipi_flush_tlb_page,
					&ta, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	} else
		local_flush_tlb_page(vma, uaddr);
=======
		on_each_cpu_mask(mm_cpumask(vma->vm_mm), ipi_flush_tlb_page,
					&ta, 1);
	} else
		__flush_tlb_page(vma, uaddr);
	broadcast_tlb_mm_a15_erratum(vma->vm_mm);
>>>>>>> refs/remotes/origin/master
}

void flush_tlb_kernel_page(unsigned long kaddr)
{
	if (tlb_ops_need_broadcast()) {
		struct tlb_args ta;
		ta.ta_start = kaddr;
		on_each_cpu(ipi_flush_tlb_kernel_page, &ta, 1);
	} else
<<<<<<< HEAD
		local_flush_tlb_kernel_page(kaddr);
=======
		__flush_tlb_kernel_page(kaddr);
	broadcast_tlb_a15_erratum();
>>>>>>> refs/remotes/origin/master
}

void flush_tlb_range(struct vm_area_struct *vma,
                     unsigned long start, unsigned long end)
{
	if (tlb_ops_need_broadcast()) {
		struct tlb_args ta;
		ta.ta_vma = vma;
		ta.ta_start = start;
		ta.ta_end = end;
<<<<<<< HEAD
<<<<<<< HEAD
		on_each_cpu_mask(ipi_flush_tlb_range, &ta, 1, mm_cpumask(vma->vm_mm));
=======
		on_each_cpu_mask(mm_cpumask(vma->vm_mm), ipi_flush_tlb_range,
					&ta, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	} else
		local_flush_tlb_range(vma, start, end);
=======
		on_each_cpu_mask(mm_cpumask(vma->vm_mm), ipi_flush_tlb_range,
					&ta, 1);
	} else
		local_flush_tlb_range(vma, start, end);
	broadcast_tlb_mm_a15_erratum(vma->vm_mm);
>>>>>>> refs/remotes/origin/master
}

void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	if (tlb_ops_need_broadcast()) {
		struct tlb_args ta;
		ta.ta_start = start;
		ta.ta_end = end;
		on_each_cpu(ipi_flush_tlb_kernel_range, &ta, 1);
	} else
		local_flush_tlb_kernel_range(start, end);
<<<<<<< HEAD
}

=======
	broadcast_tlb_a15_erratum();
}

void flush_bp_all(void)
{
	if (tlb_ops_need_broadcast())
		on_each_cpu(ipi_flush_bp_all, NULL, 1);
	else
		__flush_bp_all();
}
>>>>>>> refs/remotes/origin/master
