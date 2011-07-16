extern "C"
{
#include "txc_dxtn.h"
};

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "s2tc_algorithm.h"
#include "s2tc_common.h"

void fetch_2d_texel_rgb_dxt1(GLint srcRowStride, const GLubyte *pixdata,
			     GLint i, GLint j, GLvoid *texel)
{
	// fetches a single texel (i,j) into pixdata (RGB)
	GLubyte *t = (GLubyte *) texel;
	const GLubyte *blksrc = (pixdata + (((srcRowStride + 3) >> 2) * (j >> 2) + (i >> 2)) * 8);
	unsigned int c  = blksrc[0] + 256*blksrc[1];
	unsigned int c1 = blksrc[2] + 256*blksrc[3];
	int b = (blksrc[4 + (j & 3)] >> (2 * (i & 3))) & 0x03;
	switch(b)
	{
		case 0:                        break;
		case 1:  c = c1;               break;
		case 3:  if(c1 > c) { c = 0;   break; }
		default: if((i^j) & 1) c = c1; break;
	}
	t[0] = ((c >> 11) & 0x1F); t[0] = (t[0] << 3) | (t[0] >> 2);
	t[1] = ((c >>  5) & 0x3F); t[1] = (t[1] << 2) | (t[1] >> 4);
	t[2] = ((c      ) & 0x1F); t[2] = (t[2] << 3) | (t[2] >> 2);
	t[3] = 255;
}

void fetch_2d_texel_rgba_dxt1(GLint srcRowStride, const GLubyte *pixdata,
			     GLint i, GLint j, GLvoid *texel)
{
	// fetches a single texel (i,j) into pixdata (RGBA)
	GLubyte *t = (GLubyte *) texel;
	const GLubyte *blksrc = (pixdata + (((srcRowStride + 3) >> 2) * (j >> 2) + (i >> 2)) * 8);
	unsigned int c  = blksrc[0] + 256*blksrc[1];
	unsigned int c1 = blksrc[2] + 256*blksrc[3];
	int b = (blksrc[4 + (j & 3)] >> (2 * (i & 3))) & 0x03;
	switch(b)
	{
		case 0:                        t[3] = 255; break;
		case 1:  c = c1;               t[3] = 255; break;
		case 3:  if(c1 > c) { c = 0;   t[3] =   0; break; }
		default: if((i^j) & 1) c = c1; t[3] = 255; break;
	}
	t[0] = ((c >> 11) & 0x1F); t[0] = (t[0] << 3) | (t[0] >> 2);
	t[1] = ((c >>  5) & 0x3F); t[1] = (t[1] << 2) | (t[1] >> 4);
	t[2] = ((c      ) & 0x1F); t[2] = (t[2] << 3) | (t[2] >> 2);
}

void fetch_2d_texel_rgba_dxt3(GLint srcRowStride, const GLubyte *pixdata,
			     GLint i, GLint j, GLvoid *texel)
{
	// fetches a single texel (i,j) into pixdata (RGBA)
	GLubyte *t = (GLubyte *) texel;
	const GLubyte *blksrc = (pixdata + (((srcRowStride + 3) >> 2) * (j >> 2) + (i >> 2)) * 16);
	unsigned int c  = blksrc[8] + 256*blksrc[9];
	unsigned int c1 = blksrc[10] + 256*blksrc[11];
	int b = (blksrc[12 + (j & 3)] >> (2 * (i & 3))) & 0x03;
	switch(b)
	{
		case 0:                        break;
		case 1:  c = c1;               break;
		default: if((i^j) & 1) c = c1; break;
	}
	t[0] = ((c >> 11) & 0x1F); t[0] = (t[0] << 3) | (t[0] >> 2);
	t[1] = ((c >>  5) & 0x3F); t[1] = (t[1] << 2) | (t[1] >> 4);
	t[2] = ((c      ) & 0x1F); t[2] = (t[2] << 3) | (t[2] >> 2);
	int a = (blksrc[(j & 3) * 2 + ((i & 3) >> 1)] >> (4 * (i & 1))) & 0x0F;
	t[3] = a | (a << 4);
}

void fetch_2d_texel_rgba_dxt5(GLint srcRowStride, const GLubyte *pixdata,
			     GLint i, GLint j, GLvoid *texel)
{
	// fetches a single texel (i,j) into pixdata (RGBA)
	GLubyte *t = (GLubyte *) texel;
	const GLubyte *blksrc = (pixdata + (((srcRowStride + 3) >> 2) * (j >> 2) + (i >> 2)) * 16);
	unsigned int c  = blksrc[8] + 256*blksrc[9];
	unsigned int c1 = blksrc[10] + 256*blksrc[11];
	int b = (blksrc[12 + (j & 3)] >> (2 * (i & 3))) & 0x03;
	switch(b)
	{
		case 0:                        break;
		case 1:  c = c1;               break;
		default: if((i^j) & 1) c = c1; break;
	}
	t[0] = ((c >> 11) & 0x1F); t[0] = (t[0] << 3) | (t[0] >> 2);
	t[1] = ((c >>  5) & 0x3F); t[1] = (t[1] << 2) | (t[1] >> 4);
	t[2] = ((c      ) & 0x1F); t[2] = (t[2] << 3) | (t[2] >> 2);

	unsigned int a  = blksrc[0];
	unsigned int a1 = blksrc[1];
	int abit = ((j & 3) * 4 + (i & 3)) * 3;
	int ab = 0;
	if(testbit(&blksrc[2], abit))
		ab |= 1;
	++abit;
	if(testbit(&blksrc[2], abit))
		ab |= 2;
	++abit;
	if(testbit(&blksrc[2], abit))
		ab |= 4;
	switch(ab)
	{
		case 0:                        break;
		case 1:  a = a1;               break;
		case 6:  a = 0;                break;
		case 7:  a = 255;              break;
		default: if((i^j) & 1) a = a1; break;
	}
	t[3] = a;
}

void tx_compress_dxtn(GLint srccomps, GLint width, GLint height,
		      const GLubyte *srcPixData, GLenum destformat,
		      GLubyte *dest, GLint dstRowStride)
{
	// compresses width*height pixels (RGB or RGBA depending on srccomps) at srcPixData (packed) to destformat (dest, dstRowStride)

	GLubyte *blkaddr = dest;
	GLint numxpixels, numypixels;
	GLint i, j;
	GLint dstRowDiff;
	unsigned char *rgba = (unsigned char *) malloc(width * height * 4);
	unsigned char *srcaddr;
	DxtMode dxt;

	switch (destformat) {
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			dxt = DXT1;
			rgb565_image(rgba, srcPixData, width, height, srccomps, 0, 1);
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			dxt = DXT3;
			rgb565_image(rgba, srcPixData, width, height, srccomps, 0, 4);
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			dxt = DXT5;
			rgb565_image(rgba, srcPixData, width, height, srccomps, 0, 8);
			break;
		default:
			free(rgba);
			fprintf(stderr, "libdxtn: Bad dstFormat %d in tx_compress_dxtn\n", destformat);
			return;
	}

	ColorDistMode cd = WAVG;
	int nrandom = -1;
	RefinementMode refine = REFINE_ALWAYS;
	{
		const char *v = getenv("S2TC_COLORDIST_MODE");
		if(v)
		{
			if(!strcasecmp(v, "RGB"))
				cd = RGB;
			else if(!strcasecmp(v, "YUV"))
				cd = YUV;
			else if(!strcasecmp(v, "SRGB"))
				cd = SRGB;
			else if(!strcasecmp(v, "SRGB_MIXED"))
				cd = SRGB_MIXED;
			else if(!strcasecmp(v, "LAB"))
				cd = LAB;
			else if(!strcasecmp(v, "AVG"))
				cd = AVG;
			else if(!strcasecmp(v, "WAVG"))
				cd = WAVG;
			else if(!strcasecmp(v, "NORMALMAP"))
				cd = NORMALMAP;
			else
				fprintf(stderr, "Invalid color dist mode: %s\n", v);
		}
	}
	{
		const char *v = getenv("S2TC_RANDOM_COLORS");
		if(v)
			nrandom = atoi(v);
	}
	{
		const char *v = getenv("S2TC_REFINE_COLORS");
		if(v)
		{
			if(!strcasecmp(v, "NEVER"))
				refine = REFINE_NEVER;
			else if(!strcasecmp(v, "ALWAYS"))
				refine = REFINE_ALWAYS;
			else if(!strcasecmp(v, "CHECK"))
				refine = REFINE_CHECK;
			else if(!strcasecmp(v, "LOOP"))
				refine = REFINE_LOOP;
			else
				fprintf(stderr, "Invalid refinement mode: %s\n", v);
		}
	}

	s2tc_encode_block_func_t encode_block = s2tc_encode_block_func(dxt, cd, nrandom, refine);
	switch (destformat) {
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			/* hmm we used to get called without dstRowStride... */
			dstRowDiff = dstRowStride >= (width * 2) ? dstRowStride - (((width + 3) & ~3) * 2) : 0;
			/*      fprintf(stderr, "dxt1 tex width %d tex height %d dstRowStride %d\n",
				width, height, dstRowStride); */
			for (j = 0; j < height; j += 4) {
				if (height > j + 3) numypixels = 4;
				else numypixels = height - j;
				srcaddr = rgba + j * width * 4;
				for (i = 0; i < width; i += 4) {
					if (width > i + 3) numxpixels = 4;
					else numxpixels = width - i;
					encode_block(blkaddr, srcaddr, width, numxpixels, numypixels, nrandom);
					srcaddr += 4 * numxpixels;
					blkaddr += 8;
				}
				blkaddr += dstRowDiff;
			}
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			dstRowDiff = dstRowStride >= (width * 4) ? dstRowStride - (((width + 3) & ~3) * 4) : 0;
			/*      fprintf(stderr, "dxt3 tex width %d tex height %d dstRowStride %d\n",
				width, height, dstRowStride); */
			for (j = 0; j < height; j += 4) {
				if (height > j + 3) numypixels = 4;
				else numypixels = height - j;
				srcaddr = rgba + j * width * 4;
				for (i = 0; i < width; i += 4) {
					if (width > i + 3) numxpixels = 4;
					else numxpixels = width - i;
					encode_block(blkaddr, srcaddr, width, numxpixels, numypixels, nrandom);
					srcaddr += 4 * numxpixels;
					blkaddr += 16;
				}
				blkaddr += dstRowDiff;
			}
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			dstRowDiff = dstRowStride >= (width * 4) ? dstRowStride - (((width + 3) & ~3) * 4) : 0;
			/*      fprintf(stderr, "dxt5 tex width %d tex height %d dstRowStride %d\n",
				width, height, dstRowStride); */
			for (j = 0; j < height; j += 4) {
				if (height > j + 3) numypixels = 4;
				else numypixels = height - j;
				srcaddr = rgba + j * width * 4;
				for (i = 0; i < width; i += 4) {
					if (width > i + 3) numxpixels = 4;
					else numxpixels = width - i;
					encode_block(blkaddr, srcaddr, width, numxpixels, numypixels, nrandom);
					srcaddr += 4 * numxpixels;
					blkaddr += 16;
				}
				blkaddr += dstRowDiff;
			}
			break;
	}

	free(rgba);
}
