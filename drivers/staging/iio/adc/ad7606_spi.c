/*
 * AD7606 SPI ADC driver
 *
 * Copyright 2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2.
 */

#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/types.h>
#include <linux/err.h>

<<<<<<< HEAD
#include "../iio.h"
=======
#include <linux/iio/iio.h>
>>>>>>> refs/remotes/origin/master
#include "ad7606.h"

#define MAX_SPI_FREQ_HZ		23500000	/* VDRIVE above 4.75 V */

static int ad7606_spi_read_block(struct device *dev,
				 int count, void *buf)
{
	struct spi_device *spi = to_spi_device(dev);
	int i, ret;
	unsigned short *data = buf;

	ret = spi_read(spi, (u8 *)buf, count * 2);
	if (ret < 0) {
		dev_err(&spi->dev, "SPI read error\n");
		return ret;
	}

	for (i = 0; i < count; i++)
		data[i] = be16_to_cpu(data[i]);

	return 0;
}

static const struct ad7606_bus_ops ad7606_spi_bops = {
	.read_block	= ad7606_spi_read_block,
};

<<<<<<< HEAD
static int __devinit ad7606_spi_probe(struct spi_device *spi)
=======
static int ad7606_spi_probe(struct spi_device *spi)
>>>>>>> refs/remotes/origin/master
{
	struct iio_dev *indio_dev;

	indio_dev = ad7606_probe(&spi->dev, spi->irq, NULL,
			   spi_get_device_id(spi)->driver_data,
			   &ad7606_spi_bops);

	if (IS_ERR(indio_dev))
		return PTR_ERR(indio_dev);

	spi_set_drvdata(spi, indio_dev);

	return 0;
}

<<<<<<< HEAD
static int __devexit ad7606_spi_remove(struct spi_device *spi)
{
	struct iio_dev *indio_dev = dev_get_drvdata(&spi->dev);

<<<<<<< HEAD
	return ad7606_remove(indio_dev);
=======
	return ad7606_remove(indio_dev, spi->irq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int ad7606_spi_remove(struct spi_device *spi)
{
	struct iio_dev *indio_dev = dev_get_drvdata(&spi->dev);

	return ad7606_remove(indio_dev, spi->irq);
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_PM
static int ad7606_spi_suspend(struct device *dev)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);

	ad7606_suspend(indio_dev);

	return 0;
}

static int ad7606_spi_resume(struct device *dev)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);

	ad7606_resume(indio_dev);

	return 0;
}

static const struct dev_pm_ops ad7606_pm_ops = {
	.suspend = ad7606_spi_suspend,
	.resume  = ad7606_spi_resume,
};
#define AD7606_SPI_PM_OPS (&ad7606_pm_ops)

#else
#define AD7606_SPI_PM_OPS NULL
#endif

static const struct spi_device_id ad7606_id[] = {
	{"ad7606-8", ID_AD7606_8},
	{"ad7606-6", ID_AD7606_6},
	{"ad7606-4", ID_AD7606_4},
	{}
};
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(spi, ad7606_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_DEVICE_TABLE(spi, ad7606_id);
>>>>>>> refs/remotes/origin/master

static struct spi_driver ad7606_driver = {
	.driver = {
		.name = "ad7606",
<<<<<<< HEAD
<<<<<<< HEAD
		.bus = &spi_bus_type,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.owner = THIS_MODULE,
		.pm    = AD7606_SPI_PM_OPS,
	},
	.probe = ad7606_spi_probe,
<<<<<<< HEAD
	.remove = __devexit_p(ad7606_spi_remove),
	.id_table = ad7606_id,
};
<<<<<<< HEAD

static int __init ad7606_spi_init(void)
{
	return spi_register_driver(&ad7606_driver);
}
module_init(ad7606_spi_init);

static void __exit ad7606_spi_exit(void)
{
	spi_unregister_driver(&ad7606_driver);
}
module_exit(ad7606_spi_exit);
=======
module_spi_driver(ad7606_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = ad7606_spi_remove,
	.id_table = ad7606_id,
};
module_spi_driver(ad7606_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
MODULE_DESCRIPTION("Analog Devices AD7606 ADC");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
<<<<<<< HEAD
MODULE_ALIAS("spi:ad7606_spi");
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
