/*
<<<<<<< HEAD
 * Copyright IBM Corp. 2008,2009
=======
 * Copyright IBM Corp. 2008, 2009
>>>>>>> refs/remotes/origin/master
 *
 *   Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>,
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/linkage.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/asm-offsets.h>

	.section .kprobes.text, "ax"

<<<<<<< HEAD
	.globl ftrace_stub
ftrace_stub:
	br	%r14

	.globl _mcount
_mcount:
#ifdef CONFIG_DYNAMIC_FTRACE
	br	%r14

	.globl ftrace_caller
ftrace_caller:
=======
=======
#include <linux/linkage.h>
#include <asm/asm-offsets.h>
#include <asm/ftrace.h>

	.section .kprobes.text, "ax"

>>>>>>> refs/remotes/origin/master
ENTRY(ftrace_stub)
	br	%r14

ENTRY(_mcount)
#ifdef CONFIG_DYNAMIC_FTRACE
	br	%r14

ENTRY(ftrace_caller)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
	larl	%r1,function_trace_stop
	icm	%r1,0xf,0(%r1)
	bnzr	%r14
	stmg	%r2,%r5,32(%r15)
	stg	%r14,112(%r15)
	lgr	%r1,%r15
	aghi	%r15,-160
	stg	%r1,__SF_BACKCHAIN(%r15)
	lgr	%r2,%r14
	lg	%r3,168(%r15)
<<<<<<< HEAD
=======
	aghi	%r2,-MCOUNT_INSN_SIZE
>>>>>>> refs/remotes/origin/master
	larl	%r14,ftrace_trace_function
	lg	%r14,0(%r14)
	basr	%r14,%r14
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	lg	%r2,168(%r15)
	lg	%r3,272(%r15)
<<<<<<< HEAD
<<<<<<< HEAD
	.globl	ftrace_graph_caller
ftrace_graph_caller:
=======
ENTRY(ftrace_graph_caller)
>>>>>>> refs/remotes/origin/cm-10.0
=======
ENTRY(ftrace_graph_caller)
>>>>>>> refs/remotes/origin/master
# The bras instruction gets runtime patched to call prepare_ftrace_return.
# See ftrace_enable_ftrace_graph_caller. The patched instruction is:
#	bras	%r14,prepare_ftrace_return
	bras	%r14,0f
0:	stg	%r2,168(%r15)
#endif
	aghi	%r15,160
	lmg	%r2,%r5,32(%r15)
	lg	%r14,112(%r15)
	br	%r14

#ifdef CONFIG_FUNCTION_GRAPH_TRACER

<<<<<<< HEAD
<<<<<<< HEAD
	.globl	return_to_handler
return_to_handler:
=======
ENTRY(return_to_handler)
>>>>>>> refs/remotes/origin/cm-10.0
=======
ENTRY(return_to_handler)
>>>>>>> refs/remotes/origin/master
	stmg	%r2,%r5,32(%r15)
	lgr	%r1,%r15
	aghi	%r15,-160
	stg	%r1,__SF_BACKCHAIN(%r15)
	brasl	%r14,ftrace_return_to_handler
	aghi	%r15,160
	lgr	%r14,%r2
	lmg	%r2,%r5,32(%r15)
	br	%r14

#endif
