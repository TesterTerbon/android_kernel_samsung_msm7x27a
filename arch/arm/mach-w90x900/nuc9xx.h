/*
 * arch/arm/mach-w90x900/nuc9xx.h
 *
 * Copied from nuc910.h, which had:
 *
 * Copyright (c) 2008 Nuvoton corporation
 *
 * Header file for NUC900 CPU support
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
<<<<<<< HEAD
struct map_desc;
struct sys_timer;
=======

#include <linux/reboot.h>

struct map_desc;
>>>>>>> refs/remotes/origin/master

/* core initialisation functions */

extern void nuc900_init_irq(void);
<<<<<<< HEAD
extern struct sys_timer nuc900_timer;
extern void nuc9xx_restart(char, const char *);
=======
extern void nuc900_timer_init(void);
extern void nuc9xx_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master
