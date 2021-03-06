/*
 * linux/arch/arm/mach-at91/board-rm9200ek.c
 *
 *  Copyright (C) 2005 SAN People
 *
 *  Epson S1D framebuffer glue code is:
 *     Copyright (C) 2005 Thibaut VARENE <varenet@parisc-linux.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/mtd/physmap.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
<<<<<<< HEAD
#include <mach/board.h>
<<<<<<< HEAD
#include <mach/gpio.h>
#include <mach/at91rm9200_mc.h>
=======
#include <mach/at91rm9200_mc.h>
#include <mach/at91_ramc.h>
>>>>>>> refs/remotes/origin/cm-10.0

=======
#include <mach/at91rm9200_mc.h>
#include <mach/at91_ramc.h>

#include "at91_aic.h"
#include "board.h"
>>>>>>> refs/remotes/origin/master
#include "generic.h"


static void __init ek_init_early(void)
{
	/* Initialize processor: 18.432 MHz crystal */
<<<<<<< HEAD
<<<<<<< HEAD
	at91rm9200_initialize(18432000);
=======
	at91_initialize(18432000);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Setup the LEDs */
	at91_init_leds(AT91_PIN_PB1, AT91_PIN_PB2);

	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* USART1 on ttyS1. (Rx, Tx, CTS, RTS, DTR, DSR, DCD, RI) */
	at91_register_uart(AT91RM9200_ID_US1, 1, ATMEL_UART_CTS | ATMEL_UART_RTS
			   | ATMEL_UART_DTR | ATMEL_UART_DSR | ATMEL_UART_DCD
			   | ATMEL_UART_RI);

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
}

<<<<<<< HEAD
static void __init ek_init_irq(void)
{
	at91rm9200_init_interrupts(NULL);
}

static struct at91_eth_data __initdata ek_eth_data = {
=======
static struct macb_platform_data __initdata ek_eth_data = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	at91_initialize(18432000);
}

static struct macb_platform_data __initdata ek_eth_data = {
>>>>>>> refs/remotes/origin/master
	.phy_irq_pin	= AT91_PIN_PC4,
	.is_rmii	= 1,
};

static struct at91_usbh_data __initdata ek_usbh_data = {
	.ports		= 2,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
>>>>>>> refs/remotes/origin/master
};

static struct at91_udc_data __initdata ek_udc_data = {
	.vbus_pin	= AT91_PIN_PD4,
	.pullup_pin	= AT91_PIN_PD5,
};

#ifndef CONFIG_MTD_AT91_DATAFLASH_CARD
<<<<<<< HEAD
static struct at91_mmc_data __initdata ek_mmc_data = {
	.det_pin	= AT91_PIN_PB27,
	.slot_b		= 0,
	.wire4		= 1,
	.wp_pin		= AT91_PIN_PA17,
<<<<<<< HEAD
=======
	.vcc_pin	= -EINVAL,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct mci_platform_data __initdata ek_mci0_data = {
	.slot[0] = {
		.bus_width	= 4,
		.detect_pin	= AT91_PIN_PB27,
		.wp_pin		= AT91_PIN_PA17,
	}
>>>>>>> refs/remotes/origin/master
};
#endif

static struct spi_board_info ek_spi_devices[] = {
	{	/* DataFlash chip */
		.modalias	= "mtd_dataflash",
		.chip_select	= 0,
		.max_speed_hz	= 15 * 1000 * 1000,
	},
#ifdef CONFIG_MTD_AT91_DATAFLASH_CARD
	{	/* DataFlash card */
		.modalias	= "mtd_dataflash",
		.chip_select	= 3,
		.max_speed_hz	= 15 * 1000 * 1000,
	},
#endif
};

static struct i2c_board_info __initdata ek_i2c_devices[] = {
	{
		I2C_BOARD_INFO("ics1523", 0x26),
	},
	{
		I2C_BOARD_INFO("dac3550", 0x4d),
	}
};

#define EK_FLASH_BASE	AT91_CHIPSELECT_0
<<<<<<< HEAD
<<<<<<< HEAD
#define EK_FLASH_SIZE	SZ_2M
=======
#define EK_FLASH_SIZE	SZ_8M
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define EK_FLASH_SIZE	SZ_8M
>>>>>>> refs/remotes/origin/master

static struct physmap_flash_data ek_flash_data = {
	.width		= 2,
};

static struct resource ek_flash_resource = {
	.start		= EK_FLASH_BASE,
	.end		= EK_FLASH_BASE + EK_FLASH_SIZE - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device ek_flash = {
	.name		= "physmap-flash",
	.id		= 0,
	.dev		= {
				.platform_data	= &ek_flash_data,
			},
	.resource	= &ek_flash_resource,
	.num_resources	= 1,
};

static struct gpio_led ek_leds[] = {
	{	/* "user led 1", DS2 */
		.name			= "green",
		.gpio			= AT91_PIN_PB0,
		.active_low		= 1,
		.default_trigger	= "mmc0",
	},
	{	/* "user led 2", DS4 */
		.name			= "yellow",
		.gpio			= AT91_PIN_PB1,
		.active_low		= 1,
		.default_trigger	= "heartbeat",
	},
	{	/* "user led 3", DS6 */
		.name			= "red",
		.gpio			= AT91_PIN_PB2,
		.active_low		= 1,
	}
};

static void __init ek_board_init(void)
{
	/* Serial */
<<<<<<< HEAD
=======
	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* USART1 on ttyS1. (Rx, Tx, CTS, RTS, DTR, DSR, DCD, RI) */
	at91_register_uart(AT91RM9200_ID_US1, 1, ATMEL_UART_CTS | ATMEL_UART_RTS
			   | ATMEL_UART_DTR | ATMEL_UART_DSR | ATMEL_UART_DCD
			   | ATMEL_UART_RI);
>>>>>>> refs/remotes/origin/master
	at91_add_device_serial();
	/* Ethernet */
	at91_add_device_eth(&ek_eth_data);
	/* USB Host */
	at91_add_device_usbh(&ek_usbh_data);
	/* USB Device */
	at91_add_device_udc(&ek_udc_data);
	at91_set_multi_drive(ek_udc_data.pullup_pin, 1);	/* pullup_pin is connected to reset */
	/* I2C */
	at91_add_device_i2c(ek_i2c_devices, ARRAY_SIZE(ek_i2c_devices));
	/* SPI */
	at91_add_device_spi(ek_spi_devices, ARRAY_SIZE(ek_spi_devices));
#ifdef CONFIG_MTD_AT91_DATAFLASH_CARD
	/* DataFlash card */
	at91_set_gpio_output(AT91_PIN_PB22, 0);
#else
	/* MMC */
	at91_set_gpio_output(AT91_PIN_PB22, 1);	/* this MMC card slot can optionally use SPI signaling (CS3). */
<<<<<<< HEAD
	at91_add_device_mmc(0, &ek_mmc_data);
=======
	at91_add_device_mci(0, &ek_mci0_data);
>>>>>>> refs/remotes/origin/master
#endif
	/* NOR Flash */
	platform_device_register(&ek_flash);
	/* LEDs */
	at91_gpio_leds(ek_leds, ARRAY_SIZE(ek_leds));
	/* VGA */
//	ek_add_device_video();
}

MACHINE_START(AT91RM9200EK, "Atmel AT91RM9200-EK")
	/* Maintainer: SAN People/Atmel */
<<<<<<< HEAD
	.timer		= &at91rm9200_timer,
<<<<<<< HEAD
	.map_io		= at91rm9200_map_io,
	.init_early	= ek_init_early,
	.init_irq	= ek_init_irq,
=======
	.map_io		= at91_map_io,
	.init_early	= ek_init_early,
	.init_irq	= at91_init_irq_default,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_time	= at91rm9200_timer_init,
	.map_io		= at91_map_io,
	.handle_irq	= at91_aic_handle_irq,
	.init_early	= ek_init_early,
	.init_irq	= at91_init_irq_default,
>>>>>>> refs/remotes/origin/master
	.init_machine	= ek_board_init,
MACHINE_END
