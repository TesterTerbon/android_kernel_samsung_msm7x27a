/*
	This is part of the rtl8180-sa2400 driver
	released under the GPL (See file COPYING for details).
<<<<<<< HEAD
	Copyright (c) 2005 Andrea Merello <andreamrl@tiscali.it>
=======
	Copyright (c) 2005 Andrea Merello <andrea.merello@gmail.com>
>>>>>>> refs/remotes/origin/master

	This files contains programming code for the rtl8225
	radio frontend.

	*Many* thanks to Realtek Corp. for their great support!

*/

#include "r8180.h"

#define RTL8225_ANAPARAM_ON  0xa0000b59
#define RTL8225_ANAPARAM_OFF 0xa00beb59
#define RTL8225_ANAPARAM2_OFF 0x840dec11
#define RTL8225_ANAPARAM2_ON  0x860dec11
#define RTL8225_ANAPARAM_SLEEP 0xa00bab59
#define RTL8225_ANAPARAM2_SLEEP 0x840dec11

void rtl8225z2_rf_init(struct net_device *dev);
void rtl8225z2_rf_set_chan(struct net_device *dev, short ch);
void rtl8225z2_rf_close(struct net_device *dev);

<<<<<<< HEAD
void RF_WriteReg(struct net_device *dev, u8 offset, u32	data);
u32 RF_ReadReg(struct net_device *dev, u8 offset);

void rtl8180_set_mode(struct net_device *dev, int mode);
void rtl8180_set_mode(struct net_device *dev, int mode);
bool SetZebraRFPowerState8185(struct net_device *dev, RT_RF_POWER_STATE  eRFPowerState);
=======
void RF_WriteReg(struct net_device *dev, u8 offset, u16 data);
u16 RF_ReadReg(struct net_device *dev, u8 offset);

void rtl8180_set_mode(struct net_device *dev, int mode);
void rtl8180_set_mode(struct net_device *dev, int mode);
bool SetZebraRFPowerState8185(struct net_device *dev,
			      RT_RF_POWER_STATE eRFPowerState);
>>>>>>> refs/remotes/origin/master
void rtl8225z4_rf_sleep(struct net_device *dev);
void rtl8225z4_rf_wakeup(struct net_device *dev);

