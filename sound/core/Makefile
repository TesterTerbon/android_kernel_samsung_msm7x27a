#
# Makefile for ALSA
# Copyright (c) 1999,2001 by Jaroslav Kysela <perex@perex.cz>
#

snd-y     := sound.o init.o memory.o info.o control.o misc.o device.o
snd-$(CONFIG_ISA_DMA_API) += isadma.o
snd-$(CONFIG_SND_OSSEMUL) += sound_oss.o info_oss.o
snd-$(CONFIG_SND_VMASTER) += vmaster.o
<<<<<<< HEAD
<<<<<<< HEAD
=======
snd-$(CONFIG_SND_KCTL_JACK) += ctljack.o
>>>>>>> refs/remotes/origin/cm-10.0
snd-$(CONFIG_SND_JACK)	  += jack.o

snd-pcm-objs := pcm.o pcm_native.o pcm_lib.o pcm_timer.o pcm_misc.o \
		pcm_memory.o

snd-page-alloc-y := memalloc.o
snd-page-alloc-$(CONFIG_SND_DMA_SGBUF) += sgbuf.o
=======
snd-$(CONFIG_SND_KCTL_JACK) += ctljack.o
snd-$(CONFIG_SND_JACK)	  += jack.o

snd-pcm-y := pcm.o pcm_native.o pcm_lib.o pcm_timer.o pcm_misc.o \
		pcm_memory.o memalloc.o
snd-pcm-$(CONFIG_SND_DMA_SGBUF) += sgbuf.o

snd-pcm-dmaengine-objs := pcm_dmaengine.o
>>>>>>> refs/remotes/origin/master

snd-rawmidi-objs  := rawmidi.o
snd-timer-objs    := timer.o
snd-hrtimer-objs  := hrtimer.o
snd-rtctimer-objs := rtctimer.o
snd-hwdep-objs    := hwdep.o

<<<<<<< HEAD
<<<<<<< HEAD
=======
snd-compress-objs := compress_offload.o

>>>>>>> refs/remotes/origin/cm-10.0
=======
snd-compress-objs := compress_offload.o

>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SND) 		+= snd.o
obj-$(CONFIG_SND_HWDEP)		+= snd-hwdep.o
obj-$(CONFIG_SND_TIMER)		+= snd-timer.o
obj-$(CONFIG_SND_HRTIMER)	+= snd-hrtimer.o
obj-$(CONFIG_SND_RTCTIMER)	+= snd-rtctimer.o
<<<<<<< HEAD
obj-$(CONFIG_SND_PCM)		+= snd-pcm.o snd-page-alloc.o
=======
obj-$(CONFIG_SND_PCM)		+= snd-pcm.o
obj-$(CONFIG_SND_DMAENGINE_PCM)	+= snd-pcm-dmaengine.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SND_RAWMIDI)	+= snd-rawmidi.o

obj-$(CONFIG_SND_OSSEMUL)	+= oss/
obj-$(CONFIG_SND_SEQUENCER)	+= seq/
<<<<<<< HEAD
<<<<<<< HEAD
=======

obj-$(CONFIG_SND_COMPRESS_OFFLOAD)	+= snd-compress.o
>>>>>>> refs/remotes/origin/cm-10.0
=======

obj-$(CONFIG_SND_COMPRESS_OFFLOAD)	+= snd-compress.o
>>>>>>> refs/remotes/origin/master
