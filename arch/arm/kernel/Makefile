#
# Makefile for the linux kernel.
#

CPPFLAGS_vmlinux.lds := -DTEXT_OFFSET=$(TEXT_OFFSET)
AFLAGS_head.o        := -DTEXT_OFFSET=$(TEXT_OFFSET)

ifdef CONFIG_FUNCTION_TRACER
CFLAGS_REMOVE_ftrace.o = -pg
<<<<<<< HEAD
<<<<<<< HEAD
=======
CFLAGS_REMOVE_insn.o = -pg
CFLAGS_REMOVE_patch.o = -pg
>>>>>>> refs/remotes/origin/cm-10.0
=======
CFLAGS_REMOVE_insn.o = -pg
CFLAGS_REMOVE_patch.o = -pg
>>>>>>> refs/remotes/origin/master
endif

CFLAGS_REMOVE_return_address.o = -pg

# Object file lists.

<<<<<<< HEAD
<<<<<<< HEAD
obj-y		:= elf.o entry-armv.o entry-common.o irq.o \
		   process.o ptrace.o return_address.o setup.o signal.o \
		   sys_arm.o stacktrace.o time.o traps.o
=======
obj-y		:= elf.o entry-armv.o entry-common.o irq.o opcodes.o \
		   process.o ptrace.o return_address.o sched_clock.o \
		   setup.o signal.o stacktrace.o sys_arm.o time.o traps.o
>>>>>>> refs/remotes/origin/cm-10.0

obj-$(CONFIG_DEPRECATED_PARAM_STRUCT) += compat.o

obj-$(CONFIG_LEDS)		+= leds.o
obj-$(CONFIG_OC_ETM)		+= etm.o
<<<<<<< HEAD

obj-$(CONFIG_ISA_DMA_API)	+= dma.o
obj-$(CONFIG_ARCH_ACORN)	+= ecard.o 
=======
obj-$(CONFIG_CPU_IDLE)		+= cpuidle.o
obj-$(CONFIG_ISA_DMA_API)	+= dma.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-y		:= elf.o entry-common.o irq.o opcodes.o \
		   process.o ptrace.o return_address.o \
		   setup.o signal.o sigreturn_codes.o \
		   stacktrace.o sys_arm.o time.o traps.o

obj-$(CONFIG_ATAGS)		+= atags_parse.o
obj-$(CONFIG_ATAGS_PROC)	+= atags_proc.o
obj-$(CONFIG_DEPRECATED_PARAM_STRUCT) += atags_compat.o

ifeq ($(CONFIG_CPU_V7M),y)
obj-y		+= entry-v7m.o v7m.o
else
obj-y		+= entry-armv.o
endif

obj-$(CONFIG_OC_ETM)		+= etm.o
obj-$(CONFIG_CPU_IDLE)		+= cpuidle.o
obj-$(CONFIG_ISA_DMA_API)	+= dma.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_FIQ)		+= fiq.o fiqasm.o
obj-$(CONFIG_MODULES)		+= armksyms.o module.o
obj-$(CONFIG_ARTHUR)		+= arthur.o
obj-$(CONFIG_ISA_DMA)		+= dma-isa.o
obj-$(CONFIG_PCI)		+= bios32.o isa.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_PM_SLEEP)		+= sleep.o
obj-$(CONFIG_HAVE_SCHED_CLOCK)	+= sched_clock.o
=======
obj-$(CONFIG_ARM_CPU_SUSPEND)	+= sleep.o suspend.o
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_SMP)		+= smp.o smp_tlb.o
obj-$(CONFIG_HAVE_ARM_SCU)	+= smp_scu.o
obj-$(CONFIG_HAVE_ARM_TWD)	+= smp_twd.o
obj-$(CONFIG_ARM_ARCH_TIMER)	+= arch_timer.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_DYNAMIC_FTRACE)	+= ftrace.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER)	+= ftrace.o
obj-$(CONFIG_KEXEC)		+= machine_kexec.o relocate_kernel.o
obj-$(CONFIG_KPROBES)		+= kprobes.o kprobes-decode.o
=======
=======
obj-$(CONFIG_ARM_CPU_SUSPEND)	+= sleep.o suspend.o
obj-$(CONFIG_SMP)		+= smp.o
ifdef CONFIG_MMU
obj-$(CONFIG_SMP)		+= smp_tlb.o
endif
obj-$(CONFIG_HAVE_ARM_SCU)	+= smp_scu.o
obj-$(CONFIG_HAVE_ARM_TWD)	+= smp_twd.o
obj-$(CONFIG_ARM_ARCH_TIMER)	+= arch_timer.o
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
obj-$(CONFIG_DYNAMIC_FTRACE)	+= ftrace.o insn.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER)	+= ftrace.o insn.o
obj-$(CONFIG_JUMP_LABEL)	+= jump_label.o insn.o patch.o
obj-$(CONFIG_KEXEC)		+= machine_kexec.o relocate_kernel.o
obj-$(CONFIG_KPROBES)		+= kprobes.o kprobes-common.o patch.o
ifdef CONFIG_THUMB2_KERNEL
obj-$(CONFIG_KPROBES)		+= kprobes-thumb.o
else
obj-$(CONFIG_KPROBES)		+= kprobes-arm.o
endif
obj-$(CONFIG_ARM_KPROBES_TEST)	+= test-kprobes.o
test-kprobes-objs		:= kprobes-test.o
ifdef CONFIG_THUMB2_KERNEL
test-kprobes-objs		+= kprobes-test-thumb.o
else
test-kprobes-objs		+= kprobes-test-arm.o
endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_ATAGS_PROC)	+= atags.o
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_OABI_COMPAT)	+= sys_oabi-compat.o
obj-$(CONFIG_ARM_THUMBEE)	+= thumbee.o
obj-$(CONFIG_KGDB)		+= kgdb.o
obj-$(CONFIG_ARM_UNWIND)	+= unwind.o
obj-$(CONFIG_HAVE_TCM)		+= tcm.o
obj-$(CONFIG_OF)		+= devtree.o
obj-$(CONFIG_CRASH_DUMP)	+= crash_dump.o
obj-$(CONFIG_SWP_EMULATE)	+= swp_emulate.o
CFLAGS_swp_emulate.o		:= -Wa,-march=armv7-a
obj-$(CONFIG_HAVE_HW_BREAKPOINT)	+= hw_breakpoint.o

<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_CRUNCH)		+= crunch.o crunch-bits.o
AFLAGS_crunch-bits.o		:= -Wa,-mcpu=ep9312

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_CPU_XSCALE)	+= xscale-cp0.o
obj-$(CONFIG_CPU_XSC3)		+= xscale-cp0.o
obj-$(CONFIG_CPU_MOHAWK)	+= xscale-cp0.o
obj-$(CONFIG_CPU_PJ4)		+= pj4-cp0.o
obj-$(CONFIG_IWMMXT)		+= iwmmxt.o
<<<<<<< HEAD
obj-$(CONFIG_CPU_HAS_PMU)	+= pmu.o
obj-$(CONFIG_HW_PERF_EVENTS)	+= perf_event.o
AFLAGS_iwmmxt.o			:= -Wa,-mcpu=iwmmxt
<<<<<<< HEAD
=======
obj-$(CONFIG_ARM_CPU_TOPOLOGY)  += topology.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-$(CONFIG_PERF_EVENTS)	+= perf_regs.o
obj-$(CONFIG_HW_PERF_EVENTS)	+= perf_event.o perf_event_cpu.o
AFLAGS_iwmmxt.o			:= -Wa,-mcpu=iwmmxt
obj-$(CONFIG_ARM_CPU_TOPOLOGY)  += topology.o
>>>>>>> refs/remotes/origin/master

ifneq ($(CONFIG_ARCH_EBSA110),y)
  obj-y		+= io.o
endif

head-y			:= head$(MMUEXT).o
obj-$(CONFIG_DEBUG_LL)	+= debug.o
obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o

<<<<<<< HEAD
extra-y := $(head-y) init_task.o vmlinux.lds
=======
obj-$(CONFIG_ARM_VIRT_EXT)	+= hyp-stub.o
ifeq ($(CONFIG_ARM_PSCI),y)
obj-y				+= psci.o
obj-$(CONFIG_SMP)		+= psci_smp.o
endif

extra-y := $(head-y) vmlinux.lds
>>>>>>> refs/remotes/origin/master
