#include <linux/linkage.h>
#include <linux/threads.h>
#include <asm/asm-offsets.h>
#include <asm/assembler.h>
#include <asm/glue-cache.h>
#include <asm/glue-proc.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
	.text

/*
 * Save CPU state for a suspend
 *  r1 = v:p offset
 *  r3 = virtual return function
 * Note: sp is decremented to allocate space for CPU state on stack
 * r0-r3,r9,r10,lr corrupted
 */
ENTRY(cpu_suspend)
	mov	r9, lr
#ifdef MULTI_CPU
	ldr	r10, =processor
	mov	r2, sp			@ current virtual SP
	ldr	r0, [r10, #CPU_SLEEP_SIZE] @ size of CPU sleep state
	ldr	ip, [r10, #CPU_DO_RESUME] @ virtual resume function
	sub	sp, sp, r0		@ allocate CPU state on stack
	mov	r0, sp			@ save pointer
	add	ip, ip, r1		@ convert resume fn to phys
	stmfd	sp!, {r1, r2, r3, ip}	@ save v:p, virt SP, retfn, phys resume fn
	ldr	r3, =sleep_save_sp
	add	r2, sp, r1		@ convert SP to phys
#ifdef CONFIG_SMP
	ALT_SMP(mrc p15, 0, lr, c0, c0, 5)
	ALT_UP(mov lr, #0)
	and	lr, lr, #15
	str	r2, [r3, lr, lsl #2]	@ save phys SP
#else
	str	r2, [r3]		@ save phys SP
#endif
	mov	lr, pc
	ldr	pc, [r10, #CPU_DO_SUSPEND] @ save CPU state
#else
	mov	r2, sp			@ current virtual SP
	ldr	r0, =cpu_suspend_size
	sub	sp, sp, r0		@ allocate CPU state on stack
	mov	r0, sp			@ save pointer
	stmfd	sp!, {r1, r2, r3}	@ save v:p, virt SP, return fn
	ldr	r3, =sleep_save_sp
	add	r2, sp, r1		@ convert SP to phys
=======
	.text
=======
	.text

/*
 * Implementation of MPIDR hash algorithm through shifting
 * and OR'ing.
 *
 * @dst: register containing hash result
 * @rs0: register containing affinity level 0 bit shift
 * @rs1: register containing affinity level 1 bit shift
 * @rs2: register containing affinity level 2 bit shift
 * @mpidr: register containing MPIDR value
 * @mask: register containing MPIDR mask
 *
 * Pseudo C-code:
 *
 *u32 dst;
 *
 *compute_mpidr_hash(u32 rs0, u32 rs1, u32 rs2, u32 mpidr, u32 mask) {
 *	u32 aff0, aff1, aff2;
 *	u32 mpidr_masked = mpidr & mask;
 *	aff0 = mpidr_masked & 0xff;
 *	aff1 = mpidr_masked & 0xff00;
 *	aff2 = mpidr_masked & 0xff0000;
 *	dst = (aff0 >> rs0 | aff1 >> rs1 | aff2 >> rs2);
 *}
 * Input registers: rs0, rs1, rs2, mpidr, mask
 * Output register: dst
 * Note: input and output registers must be disjoint register sets
         (eg: a macro instance with mpidr = r1 and dst = r1 is invalid)
 */
	.macro compute_mpidr_hash dst, rs0, rs1, rs2, mpidr, mask
	and	\mpidr, \mpidr, \mask			@ mask out MPIDR bits
	and	\dst, \mpidr, #0xff			@ mask=aff0
 ARM(	mov	\dst, \dst, lsr \rs0		)	@ dst=aff0>>rs0
 THUMB(	lsr	\dst, \dst, \rs0		)
	and	\mask, \mpidr, #0xff00			@ mask = aff1
 ARM(	orr	\dst, \dst, \mask, lsr \rs1	)	@ dst|=(aff1>>rs1)
 THUMB(	lsr	\mask, \mask, \rs1		)
 THUMB(	orr	\dst, \dst, \mask		)
	and	\mask, \mpidr, #0xff0000		@ mask = aff2
 ARM(	orr	\dst, \dst, \mask, lsr \rs2	)	@ dst|=(aff2>>rs2)
 THUMB(	lsr	\mask, \mask, \rs2		)
 THUMB(	orr	\dst, \dst, \mask		)
	.endm
>>>>>>> refs/remotes/origin/master

/*
 * Save CPU state for a suspend.  This saves the CPU general purpose
 * registers, and allocates space on the kernel stack to save the CPU
 * specific registers and some other data for resume.
 *  r0 = suspend function arg0
 *  r1 = suspend function
<<<<<<< HEAD
=======
 *  r2 = MPIDR value the resuming CPU will use
>>>>>>> refs/remotes/origin/master
 */
ENTRY(__cpu_suspend)
	stmfd	sp!, {r4 - r11, lr}
#ifdef MULTI_CPU
	ldr	r10, =processor
	ldr	r4, [r10, #CPU_SLEEP_SIZE] @ size of CPU sleep state
#else
	ldr	r4, =cpu_suspend_size
#endif
	mov	r5, sp			@ current virtual SP
	add	r4, r4, #12		@ Space for pgd, virt sp, phys resume fn
	sub	sp, sp, r4		@ allocate CPU state on stack
<<<<<<< HEAD
	stmfd	sp!, {r0, r1}		@ save suspend func arg and pointer
	add	r0, sp, #8		@ save pointer to save block
	mov	r1, r4			@ size of save block
	mov	r2, r5			@ virtual SP
	ldr	r3, =sleep_save_sp
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_SMP
	ALT_SMP(mrc p15, 0, lr, c0, c0, 5)
	ALT_UP(mov lr, #0)
	and	lr, lr, #15
<<<<<<< HEAD
	str	r2, [r3, lr, lsl #2]	@ save phys SP
#else
	str	r2, [r3]		@ save phys SP
#endif
	bl	cpu_do_suspend
#endif

	@ flush data cache
#ifdef MULTI_CACHE
	ldr	r10, =cpu_cache
	mov	lr, r9
	ldr	pc, [r10, #CACHE_FLUSH_KERN_ALL]
#else
	mov	lr, r9
	b	__cpuc_flush_kern_all
#endif
ENDPROC(cpu_suspend)
	.ltorg

/*
 * r0 = control register value
 * r1 = v:p offset (preserved by cpu_do_resume)
 * r2 = phys page table base
 * r3 = L1 section flags
 */
ENTRY(cpu_resume_mmu)
	adr	r4, cpu_resume_turn_mmu_on
	mov	r4, r4, lsr #20
	orr	r3, r3, r4, lsl #20
	ldr	r5, [r2, r4, lsl #2]	@ save old mapping
	str	r3, [r2, r4, lsl #2]	@ setup 1:1 mapping for mmu code
	sub	r2, r2, r1
	ldr	r3, =cpu_resume_after_mmu
	bic	r1, r0, #CR_C		@ ensure D-cache is disabled
	b	cpu_resume_turn_mmu_on
ENDPROC(cpu_resume_mmu)
	.ltorg
	.align	5
cpu_resume_turn_mmu_on:
	mcr	p15, 0, r1, c1, c0, 0	@ turn on MMU, I-cache, etc
	mrc	p15, 0, r1, c0, c0, 0	@ read id reg
	mov	r1, r1
	mov	r1, r1
	mov	pc, r3			@ jump to virtual address
ENDPROC(cpu_resume_turn_mmu_on)
cpu_resume_after_mmu:
	str	r5, [r2, r4, lsl #2]	@ restore old mapping
	mcr	p15, 0, r0, c1, c0, 0	@ turn on D-cache
	mov	pc, lr
=======
	add	r3, r3, lr, lsl #2
#endif
=======
	ldr	r3, =sleep_save_sp
	stmfd	sp!, {r0, r1}		@ save suspend func arg and pointer
	ldr	r3, [r3, #SLEEP_SAVE_SP_VIRT]
	ALT_SMP(ldr r0, =mpidr_hash)
	ALT_UP_B(1f)
	/* This ldmia relies on the memory layout of the mpidr_hash struct */
	ldmia	r0, {r1, r6-r8}	@ r1 = mpidr mask (r6,r7,r8) = l[0,1,2] shifts
	compute_mpidr_hash	r0, r6, r7, r8, r2, r1
	add	r3, r3, r0, lsl #2
1:	mov	r2, r5			@ virtual SP
	mov	r1, r4			@ size of save block
	add	r0, sp, #8		@ pointer to save block
>>>>>>> refs/remotes/origin/master
	bl	__cpu_suspend_save
	adr	lr, BSYM(cpu_suspend_abort)
	ldmfd	sp!, {r0, pc}		@ call suspend fn
ENDPROC(__cpu_suspend)
	.ltorg

cpu_suspend_abort:
	ldmia	sp!, {r1 - r3}		@ pop phys pgd, virt SP, phys resume fn
	teq	r0, #0
	moveq	r0, #1			@ force non-zero value
	mov	sp, r2
	ldmfd	sp!, {r4 - r11, pc}
ENDPROC(cpu_suspend_abort)

/*
 * r0 = control register value
 */
	.align	5
	.pushsection	.idmap.text,"ax"
ENTRY(cpu_resume_mmu)
	ldr	r3, =cpu_resume_after_mmu
	instr_sync
	mcr	p15, 0, r0, c1, c0, 0	@ turn on MMU, I-cache, etc
	mrc	p15, 0, r0, c0, c0, 0	@ read id reg
	instr_sync
	mov	r0, r0
	mov	r0, r0
	mov	pc, r3			@ jump to virtual address
ENDPROC(cpu_resume_mmu)
	.popsection
cpu_resume_after_mmu:
	bl	cpu_init		@ restore the und/abt/irq banked regs
	mov	r0, #0			@ return zero on success
	ldmfd	sp!, {r4 - r11, pc}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
ENDPROC(cpu_resume_after_mmu)

/*
 * Note: Yes, part of the following code is located into the .data section.
 *       This is to allow sleep_save_sp to be accessed with a relative load
 *       while we can't rely on any MMU translation.  We could have put
 *       sleep_save_sp in the .text section as well, but some setups might
 *       insist on it to be truly read-only.
 */
	.data
	.align
ENTRY(cpu_resume)
<<<<<<< HEAD
#ifdef CONFIG_SMP
	adr	r0, sleep_save_sp
	ALT_SMP(mrc p15, 0, r1, c0, c0, 5)
	ALT_UP(mov r1, #0)
	and	r1, r1, #15
	ldr	r0, [r0, r1, lsl #2]	@ stack phys addr
#else
	ldr	r0, sleep_save_sp	@ stack phys addr
#endif
	setmode	PSR_I_BIT | PSR_F_BIT | SVC_MODE, r1  @ set SVC, irqs off
<<<<<<< HEAD
#ifdef MULTI_CPU
	@ load v:p, stack, return fn, resume fn
  ARM(	ldmia	r0!, {r1, sp, lr, pc}	)
THUMB(	ldmia	r0!, {r1, r2, r3, r4}	)
THUMB(	mov	sp, r2			)
THUMB(	mov	lr, r3			)
THUMB(	bx	r4			)
#else
	@ load v:p, stack, return fn
  ARM(	ldmia	r0!, {r1, sp, lr}	)
THUMB(	ldmia	r0!, {r1, r2, lr}	)
THUMB(	mov	sp, r2			)
	b	cpu_do_resume
#endif
=======
=======
ARM_BE8(setend be)			@ ensure we are in BE mode
	mov	r1, #0
	ALT_SMP(mrc p15, 0, r0, c0, c0, 5)
	ALT_UP_B(1f)
	adr	r2, mpidr_hash_ptr
	ldr	r3, [r2]
	add	r2, r2, r3		@ r2 = struct mpidr_hash phys address
	/*
	 * This ldmia relies on the memory layout of the mpidr_hash
	 * struct mpidr_hash.
	 */
	ldmia	r2, { r3-r6 }	@ r3 = mpidr mask (r4,r5,r6) = l[0,1,2] shifts
	compute_mpidr_hash	r1, r4, r5, r6, r0, r3
1:
	adr	r0, _sleep_save_sp
	ldr	r0, [r0, #SLEEP_SAVE_SP_PHYS]
	ldr	r0, [r0, r1, lsl #2]

	setmode	PSR_I_BIT | PSR_F_BIT | SVC_MODE, r1  @ set SVC, irqs off
>>>>>>> refs/remotes/origin/master
	@ load phys pgd, stack, resume fn
  ARM(	ldmia	r0!, {r1, sp, pc}	)
THUMB(	ldmia	r0!, {r1, r2, r3}	)
THUMB(	mov	sp, r2			)
THUMB(	bx	r3			)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
ENDPROC(cpu_resume)

sleep_save_sp:
	.rept	CONFIG_NR_CPUS
	.long	0				@ preserve stack phys ptr here
	.endr
=======
ENDPROC(cpu_resume)

	.align 2
mpidr_hash_ptr:
	.long	mpidr_hash - .			@ mpidr_hash struct offset

	.type	sleep_save_sp, #object
ENTRY(sleep_save_sp)
_sleep_save_sp:
	.space	SLEEP_SAVE_SP_SZ		@ struct sleep_save_sp
>>>>>>> refs/remotes/origin/master
