<<<<<<< HEAD
#ifndef _ASM_IA64_MMAN_H
#define _ASM_IA64_MMAN_H

=======
>>>>>>> refs/remotes/origin/master
/*
 * Based on <asm-i386/mman.h>.
 *
 * Modified 1998-2000, 2002
 *	David Mosberger-Tang <davidm@hpl.hp.com>, Hewlett-Packard Co
 */
<<<<<<< HEAD

#include <asm-generic/mman.h>

#define MAP_GROWSUP	0x0200		/* register stack-like segment */

#ifdef __KERNEL__
=======
#ifndef _ASM_IA64_MMAN_H
#define _ASM_IA64_MMAN_H

#include <uapi/asm/mman.h>

>>>>>>> refs/remotes/origin/master
#ifndef __ASSEMBLY__
#define arch_mmap_check	ia64_mmap_check
int ia64_mmap_check(unsigned long addr, unsigned long len,
		unsigned long flags);
#endif
<<<<<<< HEAD
#endif

=======
>>>>>>> refs/remotes/origin/master
#endif /* _ASM_IA64_MMAN_H */
