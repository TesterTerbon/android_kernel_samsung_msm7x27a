/*
 * Copyright 2003-2011 NetLogic Microsystems, Inc. (NetLogic). All rights
 * reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the NetLogic
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NETLOGIC ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL NETLOGIC OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/irq.h>

#include <asm/mmu_context.h>

#include <asm/netlogic/interrupt.h>
#include <asm/netlogic/mips-extns.h>
#include <asm/netlogic/haldefs.h>
#include <asm/netlogic/common.h>

#if defined(CONFIG_CPU_XLP)
#include <asm/netlogic/xlp-hal/iomap.h>
#include <asm/netlogic/xlp-hal/xlp.h>
#include <asm/netlogic/xlp-hal/pic.h>
#elif defined(CONFIG_CPU_XLR)
#include <asm/netlogic/xlr/iomap.h>
#include <asm/netlogic/xlr/pic.h>
#include <asm/netlogic/xlr/xlr.h>
#else
#error "Unknown CPU"
#endif

void nlm_send_ipi_single(int logical_cpu, unsigned int action)
{
<<<<<<< HEAD
	int cpu = cpu_logical_map(logical_cpu);

	if (action & SMP_CALL_FUNCTION)
		nlm_pic_send_ipi(nlm_pic_base, cpu, IRQ_IPI_SMP_FUNCTION, 0);
	if (action & SMP_RESCHEDULE_YOURSELF)
		nlm_pic_send_ipi(nlm_pic_base, cpu, IRQ_IPI_SMP_RESCHEDULE, 0);
=======
	int cpu, node;
	uint64_t picbase;

	cpu = cpu_logical_map(logical_cpu);
	node = cpu / NLM_CPUS_PER_NODE;
	picbase = nlm_get_node(node)->picbase;

	if (action & SMP_CALL_FUNCTION)
		nlm_pic_send_ipi(picbase, cpu, IRQ_IPI_SMP_FUNCTION, 0);
	if (action & SMP_RESCHEDULE_YOURSELF)
		nlm_pic_send_ipi(picbase, cpu, IRQ_IPI_SMP_RESCHEDULE, 0);
>>>>>>> refs/remotes/origin/master
}

void nlm_send_ipi_mask(const struct cpumask *mask, unsigned int action)
{
	int cpu;

	for_each_cpu(cpu, mask) {
		nlm_send_ipi_single(cpu, action);
	}
}

/* IRQ_IPI_SMP_FUNCTION Handler */
void nlm_smp_function_ipi_handler(unsigned int irq, struct irq_desc *desc)
{
<<<<<<< HEAD
	write_c0_eirr(1ull << irq);
	smp_call_function_interrupt();
=======
	clear_c0_eimr(irq);
	ack_c0_eirr(irq);
	smp_call_function_interrupt();
	set_c0_eimr(irq);
>>>>>>> refs/remotes/origin/master
}

/* IRQ_IPI_SMP_RESCHEDULE  handler */
void nlm_smp_resched_ipi_handler(unsigned int irq, struct irq_desc *desc)
{
<<<<<<< HEAD
	write_c0_eirr(1ull << irq);
	scheduler_ipi();
=======
	clear_c0_eimr(irq);
	ack_c0_eirr(irq);
	scheduler_ipi();
	set_c0_eimr(irq);
>>>>>>> refs/remotes/origin/master
}

/*
 * Called before going into mips code, early cpu init
 */
void nlm_early_init_secondary(int cpu)
{
	change_c0_config(CONF_CM_CMASK, 0x3);
<<<<<<< HEAD
	write_c0_ebase((uint32_t)nlm_common_ebase);
#ifdef CONFIG_CPU_XLP
	if (hard_smp_processor_id() % 4 == 0)
		xlp_mmu_init();
#endif
=======
#ifdef CONFIG_CPU_XLP
	xlp_mmu_init();
#endif
	write_c0_ebase(nlm_current_node()->ebase);
>>>>>>> refs/remotes/origin/master
}

/*
 * Code to run on secondary just after probing the CPU
 */
<<<<<<< HEAD
static void __cpuinit nlm_init_secondary(void)
{
	current_cpu_data.core = hard_smp_processor_id() / 4;
	nlm_smp_irq_init();
=======
static void nlm_init_secondary(void)
{
	int hwtid;

	hwtid = hard_smp_processor_id();
	current_cpu_data.core = hwtid / NLM_THREADS_PER_CORE;
	nlm_percpu_init(hwtid);
	nlm_smp_irq_init(hwtid);
>>>>>>> refs/remotes/origin/master
}

void nlm_prepare_cpus(unsigned int max_cpus)
{
	/* declare we are SMT capable */
	smp_num_siblings = nlm_threads_per_core;
}

void nlm_smp_finish(void)
{
<<<<<<< HEAD
#ifdef notyet
	nlm_common_msgring_cpu_init();
#endif
=======
>>>>>>> refs/remotes/origin/master
	local_irq_enable();
}

void nlm_cpus_done(void)
{
}

/*
 * Boot all other cpus in the system, initialize them, and bring them into
 * the boot function
 */
<<<<<<< HEAD
int nlm_cpu_ready[NR_CPUS];
unsigned long nlm_next_gp;
unsigned long nlm_next_sp;

cpumask_t phys_cpu_present_map;

void nlm_boot_secondary(int logical_cpu, struct task_struct *idle)
{
	unsigned long gp = (unsigned long)task_thread_info(idle);
	unsigned long sp = (unsigned long)__KSTK_TOS(idle);
	int cpu = cpu_logical_map(logical_cpu);

	nlm_next_sp = sp;
	nlm_next_gp = gp;

	/* barrier */
	__sync();
	nlm_pic_send_ipi(nlm_pic_base, cpu, 1, 1);
=======
unsigned long nlm_next_gp;
unsigned long nlm_next_sp;
static cpumask_t phys_cpu_present_mask;

void nlm_boot_secondary(int logical_cpu, struct task_struct *idle)
{
	int cpu, node;

	cpu = cpu_logical_map(logical_cpu);
	node = cpu / NLM_CPUS_PER_NODE;
	nlm_next_sp = (unsigned long)__KSTK_TOS(idle);
	nlm_next_gp = (unsigned long)task_thread_info(idle);

	/* barrier for sp/gp store above */
	__sync();
	nlm_pic_send_ipi(nlm_get_node(node)->picbase, cpu, 1, 1);  /* NMI */
>>>>>>> refs/remotes/origin/master
}

void __init nlm_smp_setup(void)
{
	unsigned int boot_cpu;
<<<<<<< HEAD
	int num_cpus, i;

	boot_cpu = hard_smp_processor_id();
	cpus_clear(phys_cpu_present_map);

	cpu_set(boot_cpu, phys_cpu_present_map);
=======
	int num_cpus, i, ncore;
	volatile u32 *cpu_ready = nlm_get_boot_data(BOOT_CPU_READY);
	char buf[64];

	boot_cpu = hard_smp_processor_id();
	cpumask_clear(&phys_cpu_present_mask);

	cpumask_set_cpu(boot_cpu, &phys_cpu_present_mask);
>>>>>>> refs/remotes/origin/master
	__cpu_number_map[boot_cpu] = 0;
	__cpu_logical_map[0] = boot_cpu;
	set_cpu_possible(0, true);

	num_cpus = 1;
	for (i = 0; i < NR_CPUS; i++) {
		/*
<<<<<<< HEAD
		 * nlm_cpu_ready array is not set for the boot_cpu,
		 * it is only set for ASPs (see smpboot.S)
		 */
		if (nlm_cpu_ready[i]) {
			cpu_set(i, phys_cpu_present_map);
=======
		 * cpu_ready array is not set for the boot_cpu,
		 * it is only set for ASPs (see smpboot.S)
		 */
		if (cpu_ready[i]) {
			cpumask_set_cpu(i, &phys_cpu_present_mask);
>>>>>>> refs/remotes/origin/master
			__cpu_number_map[i] = num_cpus;
			__cpu_logical_map[num_cpus] = i;
			set_cpu_possible(num_cpus, true);
			++num_cpus;
		}
	}

<<<<<<< HEAD
	pr_info("Phys CPU present map: %lx, possible map %lx\n",
		(unsigned long)phys_cpu_present_map.bits[0],
		(unsigned long)cpumask_bits(cpu_possible_mask)[0]);

	pr_info("Detected %i Slave CPU(s)\n", num_cpus);
	nlm_set_nmi_handler(nlm_boot_secondary_cpus);
}

static int nlm_parse_cpumask(u32 cpu_mask)
{
	uint32_t core0_thr_mask, core_thr_mask;
	int threadmode, i;

	core0_thr_mask = cpu_mask & 0xf;
=======
	cpumask_scnprintf(buf, ARRAY_SIZE(buf), &phys_cpu_present_mask);
	pr_info("Physical CPU mask: %s\n", buf);
	cpumask_scnprintf(buf, ARRAY_SIZE(buf), cpu_possible_mask);
	pr_info("Possible CPU mask: %s\n", buf);

	/* check with the cores we have worken up */
	for (ncore = 0, i = 0; i < NLM_NR_NODES; i++)
		ncore += hweight32(nlm_get_node(i)->coremask);

	pr_info("Detected (%dc%dt) %d Slave CPU(s)\n", ncore,
		nlm_threads_per_core, num_cpus);

	/* switch NMI handler to boot CPUs */
	nlm_set_nmi_handler(nlm_boot_secondary_cpus);
}

static int nlm_parse_cpumask(cpumask_t *wakeup_mask)
{
	uint32_t core0_thr_mask, core_thr_mask;
	int threadmode, i, j;

	core0_thr_mask = 0;
	for (i = 0; i < NLM_THREADS_PER_CORE; i++)
		if (cpumask_test_cpu(i, wakeup_mask))
			core0_thr_mask |= (1 << i);
>>>>>>> refs/remotes/origin/master
	switch (core0_thr_mask) {
	case 1:
		nlm_threads_per_core = 1;
		threadmode = 0;
		break;
	case 3:
		nlm_threads_per_core = 2;
		threadmode = 2;
		break;
	case 0xf:
		nlm_threads_per_core = 4;
		threadmode = 3;
		break;
	default:
		goto unsupp;
	}

	/* Verify other cores CPU masks */
<<<<<<< HEAD
	nlm_coremask = 1;
	nlm_cpumask = core0_thr_mask;
	for (i = 1; i < 8; i++) {
		core_thr_mask = (cpu_mask >> (i * 4)) & 0xf;
		if (core_thr_mask) {
			if (core_thr_mask != core0_thr_mask)
				goto unsupp;
			nlm_coremask |= 1 << i;
			nlm_cpumask |= core0_thr_mask << (4 * i);
		}
=======
	for (i = 0; i < NR_CPUS; i += NLM_THREADS_PER_CORE) {
		core_thr_mask = 0;
		for (j = 0; j < NLM_THREADS_PER_CORE; j++)
			if (cpumask_test_cpu(i + j, wakeup_mask))
				core_thr_mask |= (1 << j);
		if (core_thr_mask != 0 && core_thr_mask != core0_thr_mask)
				goto unsupp;
>>>>>>> refs/remotes/origin/master
	}
	return threadmode;

unsupp:
<<<<<<< HEAD
	panic("Unsupported CPU mask %x\n", cpu_mask);
	return 0;
}

int __cpuinit nlm_wakeup_secondary_cpus(u32 wakeup_mask)
{
	unsigned long reset_vec;
	char *reset_data;
	int threadmode;

	/* Update reset entry point with CPU init code */
	reset_vec = CKSEG1ADDR(RESET_VEC_PHYS);
	memcpy((void *)reset_vec, (void *)nlm_reset_entry,
			(nlm_reset_entry_end - nlm_reset_entry));

	/* verify the mask and setup core config variables */
	threadmode = nlm_parse_cpumask(wakeup_mask);

	/* Setup CPU init parameters */
	reset_data = (char *)CKSEG1ADDR(RESET_DATA_PHYS);
	*(int *)(reset_data + BOOT_THREAD_MODE) = threadmode;
=======
	panic("Unsupported CPU mask %lx",
		(unsigned long)cpumask_bits(wakeup_mask)[0]);
	return 0;
}

int nlm_wakeup_secondary_cpus(void)
{
	u32 *reset_data;
	int threadmode;

	/* verify the mask and setup core config variables */
	threadmode = nlm_parse_cpumask(&nlm_cpumask);

	/* Setup CPU init parameters */
	reset_data = nlm_get_boot_data(BOOT_THREAD_MODE);
	*reset_data = threadmode;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_CPU_XLP
	xlp_wakeup_secondary_cpus();
#else
	xlr_wakeup_secondary_cpus();
#endif
	return 0;
}

struct plat_smp_ops nlm_smp_ops = {
	.send_ipi_single	= nlm_send_ipi_single,
	.send_ipi_mask		= nlm_send_ipi_mask,
	.init_secondary		= nlm_init_secondary,
	.smp_finish		= nlm_smp_finish,
	.cpus_done		= nlm_cpus_done,
	.boot_secondary		= nlm_boot_secondary,
	.smp_setup		= nlm_smp_setup,
	.prepare_cpus		= nlm_prepare_cpus,
};
