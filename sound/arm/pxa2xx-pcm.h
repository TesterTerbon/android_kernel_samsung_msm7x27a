/*
 * linux/sound/arm/pxa2xx-pcm.h -- ALSA PCM interface for the Intel PXA2xx chip
 *
 * Author:	Nicolas Pitre
 * Created:	Nov 30, 2004
 * Copyright:	MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <mach/dma.h>

struct pxa2xx_runtime_data {
	int dma_ch;
<<<<<<< HEAD
	struct pxa2xx_pcm_dma_params *params;
=======
	struct snd_dmaengine_dai_dma_data *params;
>>>>>>> refs/remotes/origin/master
	pxa_dma_desc *dma_desc_array;
	dma_addr_t dma_desc_array_phys;
};

struct pxa2xx_pcm_client {
<<<<<<< HEAD
	struct pxa2xx_pcm_dma_params *playback_params;
	struct pxa2xx_pcm_dma_params *capture_params;
=======
	struct snd_dmaengine_dai_dma_data *playback_params;
	struct snd_dmaengine_dai_dma_data *capture_params;
>>>>>>> refs/remotes/origin/master
	int (*startup)(struct snd_pcm_substream *);
	void (*shutdown)(struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *);
};

extern int pxa2xx_pcm_new(struct snd_card *, struct pxa2xx_pcm_client *, struct snd_pcm **);

