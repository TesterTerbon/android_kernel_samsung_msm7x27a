<<<<<<< HEAD
<<<<<<< HEAD
# arch/arm/plat-s3c64xx/Makefile
=======
# arch/arm/plat-samsung/Makefile
>>>>>>> refs/remotes/origin/cm-10.0
=======
# arch/arm/plat-samsung/Makefile
>>>>>>> refs/remotes/origin/master
#
# Copyright 2009 Simtec Electronics
#
# Licensed under GPLv2

obj-y				:=
obj-m				:=
obj-n				:= dummy.o
obj-				:=

# Objects we always build independent of SoC choice

<<<<<<< HEAD
<<<<<<< HEAD
obj-y				+= init.o
obj-$(CONFIG_ARCH_USES_GETTIMEOFFSET)   += time.o
obj-y				+= clock.o
obj-y				+= pwm-clock.o
obj-y				+= gpio.o
obj-y				+= gpio-config.o
obj-y				+= dev-asocdma.o

obj-$(CONFIG_SAMSUNG_CLKSRC)	+= clock-clksrc.o

obj-$(CONFIG_SAMSUNG_IRQ_UART)	+= irq-uart.o
=======
obj-y				+= init.o cpu.o
obj-$(CONFIG_ARCH_USES_GETTIMEOFFSET)   += time.o
obj-y				+= clock.o
obj-y				+= pwm-clock.o

obj-$(CONFIG_SAMSUNG_CLKSRC)	+= clock-clksrc.o

>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_SAMSUNG_IRQ_VIC_TIMER) += irq-vic-timer.o
=======
obj-y				+= init.o cpu.o

obj-$(CONFIG_SAMSUNG_CLOCK)	+= clock.o

obj-$(CONFIG_SAMSUNG_CLKSRC)	+= clock-clksrc.o
obj-$(CONFIG_S5P_CLOCK)		+= s5p-clock.o

obj-$(CONFIG_S5P_IRQ)		+= s5p-irq.o
obj-$(CONFIG_S5P_EXT_INT)	+= s5p-irq-eint.o
obj-$(CONFIG_S5P_GPIO_INT)	+= s5p-irq-gpioint.o
>>>>>>> refs/remotes/origin/master

# ADC

obj-$(CONFIG_S3C_ADC)	+= adc.o

# devices

<<<<<<< HEAD
obj-y				+= platformdata.o

<<<<<<< HEAD
obj-$(CONFIG_S3C_DEV_HSMMC)	+= dev-hsmmc.o
obj-$(CONFIG_S3C_DEV_HSMMC1)	+= dev-hsmmc1.o
obj-$(CONFIG_S3C_DEV_HSMMC2)	+= dev-hsmmc2.o
obj-$(CONFIG_S3C_DEV_HSMMC3)	+= dev-hsmmc3.o
obj-$(CONFIG_S3C_DEV_HWMON)	+= dev-hwmon.o
obj-y				+= dev-i2c0.o
obj-$(CONFIG_S3C_DEV_I2C1)	+= dev-i2c1.o
obj-$(CONFIG_S3C_DEV_I2C2)	+= dev-i2c2.o
obj-$(CONFIG_S3C_DEV_I2C3)	+= dev-i2c3.o
obj-$(CONFIG_S3C_DEV_I2C4)	+= dev-i2c4.o
obj-$(CONFIG_S3C_DEV_I2C5)	+= dev-i2c5.o
obj-$(CONFIG_S3C_DEV_I2C6)	+= dev-i2c6.o
obj-$(CONFIG_S3C_DEV_I2C7)	+= dev-i2c7.o
obj-$(CONFIG_S3C_DEV_FB)	+= dev-fb.o
obj-y				+= dev-uart.o
obj-$(CONFIG_S3C_DEV_USB_HOST)	+= dev-usb.o
obj-$(CONFIG_S3C_DEV_USB_HSOTG)	+= dev-usb-hsotg.o
obj-$(CONFIG_S3C_DEV_WDT)	+= dev-wdt.o
obj-$(CONFIG_S3C_DEV_NAND)	+= dev-nand.o
obj-$(CONFIG_S3C_DEV_ONENAND)	+= dev-onenand.o
obj-$(CONFIG_S3C_DEV_RTC)	+= dev-rtc.o

obj-$(CONFIG_SAMSUNG_DEV_ADC)	+= dev-adc.o
obj-$(CONFIG_SAMSUNG_DEV_IDE)	+= dev-ide.o
obj-$(CONFIG_SAMSUNG_DEV_TS)	+= dev-ts.o
obj-$(CONFIG_SAMSUNG_DEV_KEYPAD)	+= dev-keypad.o
obj-$(CONFIG_SAMSUNG_DEV_PWM)	+= dev-pwm.o

# DMA support

obj-$(CONFIG_S3C_DMA)		+= dma.o

obj-$(CONFIG_S3C_PL330_DMA)	+= s3c-pl330.o
=======
obj-y				+= devs.o
obj-y				+= dev-uart.o

obj-$(CONFIG_SAMSUNG_DEV_BACKLIGHT)	+= dev-backlight.o

=======
obj-$(CONFIG_SAMSUNG_ATAGS)	+= platformdata.o

obj-$(CONFIG_SAMSUNG_ATAGS)	+= devs.o
obj-$(CONFIG_SAMSUNG_ATAGS)	+= dev-uart.o
obj-$(CONFIG_S5P_DEV_MFC)	+= s5p-dev-mfc.o
obj-$(CONFIG_S5P_DEV_UART)	+= s5p-dev-uart.o

obj-$(CONFIG_SAMSUNG_DEV_BACKLIGHT)	+= dev-backlight.o

obj-$(CONFIG_S3C_SETUP_CAMIF)	+= setup-camif.o

>>>>>>> refs/remotes/origin/master
# DMA support

obj-$(CONFIG_S3C_DMA)		+= dma.o s3c-dma-ops.o

obj-$(CONFIG_SAMSUNG_DMADEV)	+= dma-ops.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

# PM support

obj-$(CONFIG_PM)		+= pm.o
obj-$(CONFIG_PM)		+= pm-gpio.o
obj-$(CONFIG_SAMSUNG_PM_CHECK)	+= pm-check.o

obj-$(CONFIG_SAMSUNG_WAKEMASK)	+= wakeup-mask.o

# PD support

obj-$(CONFIG_SAMSUNG_PD)	+= pd.o

# PWM support

obj-$(CONFIG_HAVE_PWM)		+= pwm.o
=======

# PM support

obj-$(CONFIG_SAMSUNG_PM)	+= pm.o
obj-$(CONFIG_SAMSUNG_PM_GPIO)	+= pm-gpio.o
obj-$(CONFIG_SAMSUNG_PM_CHECK)	+= pm-check.o

obj-$(CONFIG_SAMSUNG_WAKEMASK)	+= wakeup-mask.o
obj-$(CONFIG_SAMSUNG_WDT_RESET)	+= watchdog-reset.o

obj-$(CONFIG_S5P_PM)		+= s5p-pm.o
obj-$(CONFIG_S5P_IRQ_PM)	+= s5p-irq-pm.o
obj-$(CONFIG_S5P_SLEEP)		+= s5p-sleep.o
>>>>>>> refs/remotes/origin/master
