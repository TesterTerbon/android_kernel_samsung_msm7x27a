/*
 *  dma.h --
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  ALSA PCM interface for the Samsung S3C24xx CPU
=======
 *  ALSA PCM interface for the Samsung SoC
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  ALSA PCM interface for the Samsung SoC
>>>>>>> refs/remotes/origin/master
 */

#ifndef _S3C_AUDIO_H
#define _S3C_AUDIO_H

<<<<<<< HEAD
=======
#include <sound/dmaengine_pcm.h>

>>>>>>> refs/remotes/origin/master
struct s3c_dma_params {
	struct s3c2410_dma_client *client;	/* stream identifier */
	int channel;				/* Channel ID */
	dma_addr_t dma_addr;
	int dma_size;			/* Size of the DMA transfer */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned ch;
	struct samsung_dma_ops *ops;
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	unsigned ch;
	struct samsung_dma_ops *ops;
	char *ch_name;
	struct snd_dmaengine_dai_dma_data dma_data;
};

void samsung_asoc_init_dma_data(struct snd_soc_dai *dai,
				struct s3c_dma_params *playback,
				struct s3c_dma_params *capture);
int samsung_asoc_dma_platform_register(struct device *dev);
void samsung_asoc_dma_platform_unregister(struct device *dev);

>>>>>>> refs/remotes/origin/master
#endif
