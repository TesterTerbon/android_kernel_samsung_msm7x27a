/*
 * Gamma level definitions.
 *
 * Copyright (c) 2011 Samsung Electronics
 * InKi Dae <inki.dae@samsung.com>
 * Donghwa Lee <dh09.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef _LD9040_BRIGHTNESS_H
#define _LD9040_BRIGHTNESS_H

#define MAX_GAMMA_LEVEL		25
#define GAMMA_TABLE_COUNT	21

/* gamma value: 2.2 */
static const unsigned int ld9040_22_300[] = {
	0x00, 0xa7, 0xb4, 0xae, 0xbf, 0x00, 0x91,
	0x00, 0xb2, 0xb4, 0xaa, 0xbb, 0x00, 0xac,
	0x00, 0xb3, 0xb1, 0xaa, 0xbc, 0x00, 0xb3
};

static const unsigned int ld9040_22_290[] = {
	0x00, 0xa9, 0xb7, 0xae, 0xbd, 0x00, 0x89,
	0x00, 0xb7, 0xb6, 0xa8, 0xba, 0x00, 0xa4,
	0x00, 0xb1, 0xb4, 0xaa, 0xbb, 0x00, 0xaa
};

static const unsigned int ld9040_22_280[] = {
	0x00, 0xa9, 0xb6, 0xad, 0xbf, 0x00, 0x86,
	0x00, 0xb8, 0xb5, 0xa8, 0xbc, 0x00, 0xa0,
	0x00, 0xb3, 0xb3, 0xa9, 0xbc, 0x00, 0xa7
};

static const unsigned int ld9040_22_270[] = {
	0x00, 0xa8, 0xb8, 0xae, 0xbe, 0x00, 0x84,
	0x00, 0xb9, 0xb7, 0xa8, 0xbc, 0x00, 0x9d,
	0x00, 0xb2, 0xb5, 0xaa, 0xbc, 0x00, 0xa4

};
static const unsigned int ld9040_22_260[] = {
	0x00, 0xa4, 0xb8, 0xb0, 0xbf, 0x00, 0x80,
	0x00, 0xb8, 0xb6, 0xaa, 0xbc, 0x00, 0x9a,
	0x00, 0xb0, 0xb5, 0xab, 0xbd, 0x00, 0xa0
};

static const unsigned int ld9040_22_250[] = {
	0x00, 0xa4, 0xb9, 0xaf, 0xc1, 0x00, 0x7d,
	0x00, 0xb9, 0xb6, 0xaa, 0xbb, 0x00, 0x97,
	0x00, 0xb1, 0xb5, 0xaa, 0xbf, 0x00, 0x9d
};

static const unsigned int ld9040_22_240[] = {
	0x00, 0xa2, 0xb9, 0xaf, 0xc2, 0x00, 0x7a,
	0x00, 0xb9, 0xb7, 0xaa, 0xbd, 0x00, 0x94,
	0x00, 0xb0, 0xb5, 0xab, 0xbf, 0x00, 0x9a
};

static const unsigned int ld9040_22_230[] = {
	0x00, 0xa0, 0xb9, 0xaf, 0xc3, 0x00, 0x77,
	0x00, 0xb9, 0xb7, 0xab, 0xbe, 0x00, 0x90,
	0x00, 0xb0, 0xb6, 0xab, 0xbf, 0x00, 0x97
};

static const unsigned int ld9040_22_220[] = {
	0x00, 0x9e, 0xba, 0xb0, 0xc2, 0x00, 0x75,
	0x00, 0xb9, 0xb8, 0xab, 0xbe, 0x00, 0x8e,
	0x00, 0xb0, 0xb6, 0xac, 0xbf, 0x00, 0x94
};

static const unsigned int ld9040_22_210[] = {
	0x00, 0x9c, 0xb9, 0xb0, 0xc4, 0x00, 0x72,
	0x00, 0xb8, 0xb8, 0xac, 0xbf, 0x00, 0x8a,
	0x00, 0xb0, 0xb6, 0xac, 0xc0, 0x00, 0x91
};

static const unsigned int ld9040_22_200[] = {
	0x00, 0x9a, 0xba, 0xb1, 0xc4, 0x00, 0x6f,
	0x00, 0xb8, 0xb8, 0xad, 0xc0, 0x00, 0x86,
	0x00, 0xb0, 0xb7, 0xad, 0xc0, 0x00, 0x8d
};

static const unsigned int ld9040_22_190[] = {
	0x00, 0x97, 0xba, 0xb2, 0xc5, 0x00, 0x6c,
	0x00, 0xb8, 0xb8, 0xae, 0xc1, 0x00, 0x82,
	0x00, 0xb0, 0xb6, 0xae, 0xc2, 0x00, 0x89
};

static const unsigned int ld9040_22_180[] = {
	0x00, 0x93, 0xba, 0xb3, 0xc5, 0x00, 0x69,
	0x00, 0xb8, 0xb9, 0xae, 0xc1, 0x00, 0x7f,
	0x00, 0xb0, 0xb6, 0xae, 0xc3, 0x00, 0x85
};

static const unsigned int ld9040_22_170[] = {
	0x00, 0x8b, 0xb9, 0xb3, 0xc7, 0x00, 0x65,
	0x00, 0xb7, 0xb8, 0xaf, 0xc3, 0x00, 0x7a,
	0x00, 0x80, 0xb6, 0xae, 0xc4, 0x00, 0x81
};

static const unsigned int ld9040_22_160[] = {
	0x00, 0x89, 0xba, 0xb3, 0xc8, 0x00, 0x62,
	0x00, 0xb6, 0xba, 0xaf, 0xc3, 0x00, 0x76,
	0x00, 0xaf, 0xb7, 0xae, 0xc4, 0x00, 0x7e
};

static const unsigned int ld9040_22_150[] = {
	0x00, 0x82, 0xba, 0xb4, 0xc7, 0x00, 0x5f,
	0x00, 0xb5, 0xba, 0xb0, 0xc3, 0x00, 0x72,
	0x00, 0xae, 0xb8, 0xb0, 0xc3, 0x00, 0x7a
};

static const unsigned int ld9040_22_140[] = {
	0x00, 0x7b, 0xbb, 0xb4, 0xc8, 0x00, 0x5b,
	0x00, 0xb5, 0xba, 0xb1, 0xc4, 0x00, 0x6e,
	0x00, 0xae, 0xb9, 0xb0, 0xc5, 0x00, 0x75
};

static const unsigned int ld9040_22_130[] = {
	0x00, 0x71, 0xbb, 0xb5, 0xc8, 0x00, 0x57,
	0x00, 0xb5, 0xbb, 0xb0, 0xc5, 0x00, 0x6a,
	0x00, 0xae, 0xb9, 0xb1, 0xc6, 0x00, 0x70
};

static const unsigned int ld9040_22_120[] = {
	0x00, 0x47, 0xba, 0xb6, 0xca, 0x00, 0x53,
	0x00, 0xb5, 0xbb, 0xb3, 0xc6, 0x00, 0x65,
	0x00, 0xae, 0xb8, 0xb3, 0xc7, 0x00, 0x6c
};

static const unsigned int ld9040_22_110[] = {
	0x00, 0x13, 0xbb, 0xb7, 0xca, 0x00, 0x4f,
	0x00, 0xb4, 0xbb, 0xb3, 0xc7, 0x00, 0x60,
	0x00, 0xad, 0xb8, 0xb4, 0xc7, 0x00, 0x67
};

static const unsigned int ld9040_22_100[] = {
	0x00, 0x13, 0xba, 0xb8, 0xcb, 0x00, 0x4b,
	0x00, 0xb3, 0xbc, 0xb4, 0xc7, 0x00, 0x5c,
	0x00, 0xac, 0xb8, 0xb4, 0xc8, 0x00, 0x62
};

static const unsigned int ld9040_22_90[] = {
	0x00, 0x13, 0xb9, 0xb8, 0xcd, 0x00, 0x46,
	0x00, 0xb1, 0xbc, 0xb5, 0xc8, 0x00, 0x56,
	0x00, 0xaa, 0xb8, 0xb4, 0xc9, 0x00, 0x5d
};

static const unsigned int ld9040_22_80[] = {
	0x00, 0x13, 0xba, 0xb9, 0xcd, 0x00, 0x41,
	0x00, 0xb0, 0xbe, 0xb5, 0xc9, 0x00, 0x51,
	0x00, 0xa9, 0xb9, 0xb5, 0xca, 0x00, 0x57
};

static const unsigned int ld9040_22_70[] = {
	0x00, 0x13, 0xb9, 0xb9, 0xd0, 0x00, 0x3c,
	0x00, 0xaf, 0xbf, 0xb6, 0xcb, 0x00, 0x4b,
	0x00, 0xa8, 0xb9, 0xb5, 0xcc, 0x00, 0x52
};

static const unsigned int ld9040_22_50[] = {
	0x00, 0x13, 0xb2, 0xba, 0xd2, 0x00, 0x30,
	0x00, 0xaf, 0xc0, 0xb8, 0xcd, 0x00, 0x3d,
	0x00, 0xa8, 0xb8, 0xb7, 0xcd, 0x00, 0x44
};

struct ld9040_gamma {
	unsigned int *gamma_22_table[MAX_GAMMA_LEVEL];
<<<<<<< HEAD
} gamma_table = {
=======
};

static struct ld9040_gamma gamma_table = {
>>>>>>> refs/remotes/origin/master
	.gamma_22_table[0] = (unsigned int *)&ld9040_22_50,
	.gamma_22_table[1] = (unsigned int *)&ld9040_22_70,
	.gamma_22_table[2] = (unsigned int *)&ld9040_22_80,
	.gamma_22_table[3] = (unsigned int *)&ld9040_22_90,
	.gamma_22_table[4] = (unsigned int *)&ld9040_22_100,
	.gamma_22_table[5] = (unsigned int *)&ld9040_22_110,
	.gamma_22_table[6] = (unsigned int *)&ld9040_22_120,
	.gamma_22_table[7] = (unsigned int *)&ld9040_22_130,
	.gamma_22_table[8] = (unsigned int *)&ld9040_22_140,
	.gamma_22_table[9] = (unsigned int *)&ld9040_22_150,
	.gamma_22_table[10] = (unsigned int *)&ld9040_22_160,
	.gamma_22_table[11] = (unsigned int *)&ld9040_22_170,
	.gamma_22_table[12] = (unsigned int *)&ld9040_22_180,
	.gamma_22_table[13] = (unsigned int *)&ld9040_22_190,
	.gamma_22_table[14] = (unsigned int *)&ld9040_22_200,
	.gamma_22_table[15] = (unsigned int *)&ld9040_22_210,
	.gamma_22_table[16] = (unsigned int *)&ld9040_22_220,
	.gamma_22_table[17] = (unsigned int *)&ld9040_22_230,
	.gamma_22_table[18] = (unsigned int *)&ld9040_22_240,
	.gamma_22_table[19] = (unsigned int *)&ld9040_22_250,
	.gamma_22_table[20] = (unsigned int *)&ld9040_22_260,
	.gamma_22_table[21] = (unsigned int *)&ld9040_22_270,
	.gamma_22_table[22] = (unsigned int *)&ld9040_22_280,
	.gamma_22_table[23] = (unsigned int *)&ld9040_22_290,
	.gamma_22_table[24] = (unsigned int *)&ld9040_22_300,
};

#endif
