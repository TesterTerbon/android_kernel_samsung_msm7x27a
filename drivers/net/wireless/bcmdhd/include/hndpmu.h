/*
 * HND SiliconBackplane PMU support.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 1999-2011, Broadcom Corporation
 * 
 *         Unless you and Broadcom execute a separate written software license
=======
 * Copyright (C) 1999-2012, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 1999-2012, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
>>>>>>> refs/remotes/origin/cm-11.0
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * $Id: hndpmu.h 335486 2012-05-28 09:47:55Z $
=======
 * $Id: hndpmu.h 241182 2011-02-17 21:50:03Z $
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * $Id: hndpmu.h 241182 2011-02-17 21:50:03Z $
>>>>>>> refs/remotes/origin/cm-11.0
 */

#ifndef _hndpmu_h_
#define _hndpmu_h_


extern void si_pmu_otp_power(si_t *sih, osl_t *osh, bool on);
extern void si_sdiod_drive_strength_init(si_t *sih, osl_t *osh, uint32 drivestrength);

<<<<<<< HEAD
<<<<<<< HEAD
extern void si_pmu_set_otp_wr_volts(si_t *sih);
extern void si_pmu_set_otp_rd_volts(si_t *sih);
=======
extern void si_pmu_minresmask_htavail_set(si_t *sih, osl_t *osh, bool set_clear);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void si_pmu_minresmask_htavail_set(si_t *sih, osl_t *osh, bool set_clear);
>>>>>>> refs/remotes/origin/cm-11.0

#endif /* _hndpmu_h_ */
