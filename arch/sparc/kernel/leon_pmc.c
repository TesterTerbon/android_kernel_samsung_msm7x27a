/* leon_pmc.c: LEON Power-down cpu_idle() handler
 *
 * Copyright (C) 2011 Daniel Hellstrom (daniel@gaisler.com) Aeroflex Gaisler AB
 */

#include <linux/init.h>
#include <linux/pm.h>

#include <asm/leon_amba.h>
<<<<<<< HEAD
#include <asm/leon.h>
=======
#include <asm/cpu_type.h>
#include <asm/leon.h>
#include <asm/processor.h>
>>>>>>> refs/remotes/origin/master

/* List of Systems that need fixup instructions around power-down instruction */
unsigned int pmc_leon_fixup_ids[] = {
	AEROFLEX_UT699,
	GAISLER_GR712RC,
	LEON4_NEXTREME1,
	0
};

int pmc_leon_need_fixup(void)
{
	unsigned int systemid = amba_system_id >> 16;
	unsigned int *id;

	id = &pmc_leon_fixup_ids[0];
	while (*id != 0) {
		if (*id == systemid)
			return 1;
		id++;
	}

	return 0;
}

/*
 * CPU idle callback function for systems that need some extra handling
 * See .../arch/sparc/kernel/process.c
 */
void pmc_leon_idle_fixup(void)
{
	/* Prepare an address to a non-cachable region. APB is always
	 * none-cachable. One instruction is executed after the Sleep
	 * instruction, we make sure to read the bus and throw away the
	 * value by accessing a non-cachable area, also we make sure the
	 * MMU does not get a TLB miss here by using the MMU BYPASS ASI.
	 */
	register unsigned int address = (unsigned int)leon3_irqctrl_regs;
<<<<<<< HEAD
	__asm__ __volatile__ (
		"mov	%%g0, %%asr19\n"
=======

	/* Interrupts need to be enabled to not hang the CPU */
	local_irq_enable();

	__asm__ __volatile__ (
		"wr	%%g0, %%asr19\n"
>>>>>>> refs/remotes/origin/master
		"lda	[%0] %1, %%g0\n"
		:
		: "r"(address), "i"(ASI_LEON_BYPASS));
}

/*
 * CPU idle callback function
 * See .../arch/sparc/kernel/process.c
 */
void pmc_leon_idle(void)
{
<<<<<<< HEAD
	/* For systems without power-down, this will be no-op */
	__asm__ __volatile__ ("mov	%g0, %asr19\n\t");
=======
	/* Interrupts need to be enabled to not hang the CPU */
	local_irq_enable();

	/* For systems without power-down, this will be no-op */
	__asm__ __volatile__ ("wr	%g0, %asr19\n\t");
>>>>>>> refs/remotes/origin/master
}

/* Install LEON Power Down function */
static int __init leon_pmc_install(void)
{
<<<<<<< HEAD
	/* Assign power management IDLE handler */
	if (pmc_leon_need_fixup())
		pm_idle = pmc_leon_idle_fixup;
	else
		pm_idle = pmc_leon_idle;

	printk(KERN_INFO "leon: power management initialized\n");
=======
	if (sparc_cpu_model == sparc_leon) {
		/* Assign power management IDLE handler */
		if (pmc_leon_need_fixup())
			sparc_idle = pmc_leon_idle_fixup;
		else
			sparc_idle = pmc_leon_idle;

		printk(KERN_INFO "leon: power management initialized\n");
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

/* This driver is not critical to the boot process, don't care
 * if initialized late.
 */
late_initcall(leon_pmc_install);
