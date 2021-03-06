#include <linux/linkage.h>
#include <linux/errno.h>
#include <asm/dwarf2.h>
#include <asm/asm.h>
#include <asm/msr.h>

#ifdef CONFIG_X86_64
/*
<<<<<<< HEAD
 * int native_{rdmsr,wrmsr}_safe_regs(u32 gprs[8]);
=======
 * int {rdmsr,wrmsr}_safe_regs(u32 gprs[8]);
>>>>>>> refs/remotes/origin/master
 *
 * reg layout: u32 gprs[eax, ecx, edx, ebx, esp, ebp, esi, edi]
 *
 */
.macro op_safe_regs op
<<<<<<< HEAD
ENTRY(native_\op\()_safe_regs)
=======
ENTRY(\op\()_safe_regs)
>>>>>>> refs/remotes/origin/master
	CFI_STARTPROC
	pushq_cfi %rbx
	pushq_cfi %rbp
	movq	%rdi, %r10	/* Save pointer */
	xorl	%r11d, %r11d	/* Return value */
	movl    (%rdi), %eax
	movl    4(%rdi), %ecx
	movl    8(%rdi), %edx
	movl    12(%rdi), %ebx
	movl    20(%rdi), %ebp
	movl    24(%rdi), %esi
	movl    28(%rdi), %edi
	CFI_REMEMBER_STATE
1:	\op
2:	movl    %eax, (%r10)
	movl	%r11d, %eax	/* Return value */
	movl    %ecx, 4(%r10)
	movl    %edx, 8(%r10)
	movl    %ebx, 12(%r10)
	movl    %ebp, 20(%r10)
	movl    %esi, 24(%r10)
	movl    %edi, 28(%r10)
	popq_cfi %rbp
	popq_cfi %rbx
	ret
3:
	CFI_RESTORE_STATE
	movl    $-EIO, %r11d
	jmp     2b

	_ASM_EXTABLE(1b, 3b)
	CFI_ENDPROC
<<<<<<< HEAD
ENDPROC(native_\op\()_safe_regs)
=======
ENDPROC(\op\()_safe_regs)
>>>>>>> refs/remotes/origin/master
.endm

#else /* X86_32 */

.macro op_safe_regs op
<<<<<<< HEAD
ENTRY(native_\op\()_safe_regs)
=======
ENTRY(\op\()_safe_regs)
>>>>>>> refs/remotes/origin/master
	CFI_STARTPROC
	pushl_cfi %ebx
	pushl_cfi %ebp
	pushl_cfi %esi
	pushl_cfi %edi
	pushl_cfi $0              /* Return value */
	pushl_cfi %eax
	movl    4(%eax), %ecx
	movl    8(%eax), %edx
	movl    12(%eax), %ebx
	movl    20(%eax), %ebp
	movl    24(%eax), %esi
	movl    28(%eax), %edi
	movl    (%eax), %eax
	CFI_REMEMBER_STATE
1:	\op
2:	pushl_cfi %eax
	movl    4(%esp), %eax
	popl_cfi (%eax)
	addl    $4, %esp
	CFI_ADJUST_CFA_OFFSET -4
	movl    %ecx, 4(%eax)
	movl    %edx, 8(%eax)
	movl    %ebx, 12(%eax)
	movl    %ebp, 20(%eax)
	movl    %esi, 24(%eax)
	movl    %edi, 28(%eax)
	popl_cfi %eax
	popl_cfi %edi
	popl_cfi %esi
	popl_cfi %ebp
	popl_cfi %ebx
	ret
3:
	CFI_RESTORE_STATE
	movl    $-EIO, 4(%esp)
	jmp     2b

	_ASM_EXTABLE(1b, 3b)
	CFI_ENDPROC
<<<<<<< HEAD
ENDPROC(native_\op\()_safe_regs)
=======
ENDPROC(\op\()_safe_regs)
>>>>>>> refs/remotes/origin/master
.endm

#endif

op_safe_regs rdmsr
op_safe_regs wrmsr

