/*
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __ARCH_H__
#define __ARCH_H__

<<<<<<< HEAD
#include "sysdep/ptrace.h"
=======
#include <sysdep/ptrace.h>
>>>>>>> refs/remotes/origin/master

extern void arch_check_bugs(void);
extern int arch_fixup(unsigned long address, struct uml_pt_regs *regs);
extern void arch_examine_signal(int sig, struct uml_pt_regs *regs);

#endif
