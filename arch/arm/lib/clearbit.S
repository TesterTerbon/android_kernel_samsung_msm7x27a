/*
 *  linux/arch/arm/lib/clearbit.S
 *
 *  Copyright (C) 1995-1996 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include "bitops.h"
                .text

<<<<<<< HEAD
<<<<<<< HEAD
ENTRY(_clear_bit)
	bitop	bic
ENDPROC(_clear_bit)
=======
bitop	_clear_bit, bic
>>>>>>> refs/remotes/origin/cm-10.0
=======
bitop	_clear_bit, bic
>>>>>>> refs/remotes/origin/master
