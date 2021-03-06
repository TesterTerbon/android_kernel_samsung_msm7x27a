/*
 * Uniprocessor-only support functions.  The counterpart to kernel/smp.c
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/smp.h>

int smp_call_function_single(int cpu, void (*func) (void *info), void *info,
				int wait)
{
<<<<<<< HEAD
	WARN_ON(cpu != 0);

	local_irq_disable();
	(func)(info);
	local_irq_enable();
=======
	unsigned long flags;

	WARN_ON(cpu != 0);

	local_irq_save(flags);
	func(info);
	local_irq_restore(flags);
>>>>>>> refs/remotes/origin/master

	return 0;
}
EXPORT_SYMBOL(smp_call_function_single);
<<<<<<< HEAD
=======

void __smp_call_function_single(int cpu, struct call_single_data *csd,
				int wait)
{
	unsigned long flags;

	local_irq_save(flags);
	csd->func(csd->info);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(__smp_call_function_single);

int on_each_cpu(smp_call_func_t func, void *info, int wait)
{
	unsigned long flags;

	local_irq_save(flags);
	func(info);
	local_irq_restore(flags);
	return 0;
}
EXPORT_SYMBOL(on_each_cpu);

/*
 * Note we still need to test the mask even for UP
 * because we actually can get an empty mask from
 * code that on SMP might call us without the local
 * CPU in the mask.
 */
void on_each_cpu_mask(const struct cpumask *mask,
		      smp_call_func_t func, void *info, bool wait)
{
	unsigned long flags;

	if (cpumask_test_cpu(0, mask)) {
		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
	}
}
EXPORT_SYMBOL(on_each_cpu_mask);

/*
 * Preemption is disabled here to make sure the cond_func is called under the
 * same condtions in UP and SMP.
 */
void on_each_cpu_cond(bool (*cond_func)(int cpu, void *info),
		      smp_call_func_t func, void *info, bool wait,
		      gfp_t gfp_flags)
{
	unsigned long flags;

	preempt_disable();
	if (cond_func(0, info)) {
		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
	}
	preempt_enable();
}
EXPORT_SYMBOL(on_each_cpu_cond);
>>>>>>> refs/remotes/origin/master
