//#include "AIBrush.h"
//#include "BEInit.h"
#include <stdio.h> //for printf
#pragma once

#define P_PRAGMA(x) __pragma(x)

#if defined(__INTEL_COMPILER) && defined(P_PRAGMA)
#define P_PRAGMA_IVDEP P_PRAGMA(ivdep)
#else
#define P_PRAGMA_IVDEP
#endif

#if defined(__INTEL_COMPILER) && defined(P_PRAGMA)
#define P_PRAGMA_VECTOR(a) P_PRAGMA(vector(a))
#else
#define P_PRAGMA_VECTOR(a)
#endif

#if defined(__INTEL_COMPILER) && defined(P_PRAGMA)
#define P_FORCE_VECTORIZE_LOOP P_PRAGMA_VECTOR(always) \
P_PRAGMA_IVDEP
#else
#define P_FORCE_VECTORIZE_LOOP
#endif

#if __INTEL_COMPILER || WINVER
#ifndef _FORCE_INLINE
#define _FORCE_INLINE __forceinline
#endif
#else
#define _FORCE_INLINE inline
#endif

//--> AIBrush.h
#if 1
#include "Gatherton.h"
#else
#if WIN32 
#include <stdint.h>
#else
#include <inttypes.h>
#endif

typedef int PntrFixed;
typedef uint32_t Pixel;
#define SIZEpixel 4

// 'null' color, to be used only where 'Pixel' is treated as RGB value (w/o alpha)
// Eli - to compile ok const Pixel nullColor = 0xFFFFFFFF;

/* pixel load & store */
#define ld_alpha(pix,ptr) ((pix) >> 24)
#define ld_red(pix,ptr) (((pix) >> 16) & 0xFF)
#define ld_green(pix,ptr) (((pix) >> 8) & 0xFF)
#define ld_blue(pix,ptr) ((pix) & 0xFF)

#define get_alpha(pix) ((pix) >> 24)
#define get_red(pix) (((pix) >> 16) & 0xFF)
#define get_green(pix) (((pix) >> 8) & 0xFF)
#define get_blue(pix) ((pix) & 0xFF)

#define mulu_shr8(a,b) (((unsigned int)(a) * (unsigned int)(b)) >> 8)
#define mulu_shr16(a,b) (((unsigned int)(a) * (unsigned int)(b)) >> 16)
#define mulu_shr13(a,b) (((unsigned int)(a) * (unsigned int)(b)) >> 13)
#define mulu_shr16r(a,b) (((unsigned int)(a) * (unsigned int)(b) + 0x00008000L) >> 16)
#define muls_shr15(a,b) (((int)(a) * (int)(b) + 0x00004000) >> 15)

#define st_pixel(p,a,r,g,b) *p = ((((((unsigned int)(a) << 8) | (unsigned int)(r)) << 8) | \
	(unsigned int)(g)) << 8) | (unsigned int)(b)
#define st_rgbpixel(p,r,g,b) *p = (*p & 0xFF000000UL) | ((((unsigned int)(r) << 8) | \
	(unsigned int)(g)) << 8) | (unsigned int)(b)
#define st_alpha(p,a) *p = (*p & 0x00FFFFFFL) | ((unsigned int)(a) << 24)

/* vector brush message flag bits and masks */
#define vbm_grainmask_valid		0x8000
#define vbm_clone_valid			0x4000
#define vbm_dry_valid			0x2000
#define vbm_type_mask			0x000F

/* transparent mask color decode */
inline bool trans_mask_decode(Pixel pix, int& alpha, int& red, int& green, int& blue)
{
	int f, dmask;

	// get alpha
	alpha = get_alpha(pix);

	// quickly handle opaque mask case
	if (alpha == 0)
	{
		red = get_red(pix);
		green = get_green(pix);
		blue = get_blue(pix);
		return false;
	}

	dmask = 255 - alpha;
	// get color assuming composite over white
	f = dmask << 8;
	f += dmask;		// f = dmask * 257
	red = 255 - mulu_shr16r(f, 255 - get_red(pix));
	green = 255 - mulu_shr16r(f, 255 - get_green(pix));
	blue = 255 - mulu_shr16r(f, 255 - get_blue(pix));
	return true;
}
// <--AIBrush.h
#endif

class CRecipArrayData
{
public:
	int recip_array[256][256];

	CRecipArrayData()
	{
		for (int alpha = 1; alpha < 255; alpha++)
		{
			int recip = (255 << 15) / (255 - alpha);

			for (int rgb = 0; rgb < 256; rgb++)
			{
				recip_array[alpha][rgb] = (recip * (rgb - alpha) + 0x4000) >> 15;
			}
		}

		for (int rgb = 0; rgb < 256; rgb++)
		{
			recip_array[0][rgb] = rgb;
			recip_array[255][rgb] = rgb;
		}
	}
};

static CRecipArrayData mRecipArrayData;

class CRecipArray
{
public:
	_FORCE_INLINE Pixel trans_mask_encode_recip_array(int alpha, int red, int green, int blue)
	{
		int mincomp;

		if (alpha != 0 && alpha != 255)
		{
			mincomp = red;
			if (mincomp > green)
				mincomp = green;
			if (mincomp > blue)
				mincomp = blue;
			// increase alpha so color is correct
			if (mincomp < alpha)
				alpha = mincomp;
			// adjust for composite with white
			red = mRecipArrayData.recip_array[alpha][red];
			green = mRecipArrayData.recip_array[alpha][green];
			blue = mRecipArrayData.recip_array[alpha][blue];
		}
		return (((Pixel)alpha << 24) | ((Pixel)red << 16) | ((Pixel)green << 8) | (Pixel)blue);
	}

	_FORCE_INLINE void trans_mask_store_recip_array(Pixel* pixelptr, int alpha, int red, int green, int blue)
	{
		*pixelptr = trans_mask_encode_recip_array(alpha, red, green, blue);
	}
};

static CRecipArray mRecipArray;

inline void memcpy_simd(void* dst, const void* src, size_t size)
{
	char* dst_char = (char*)dst;
	const char* src_char = (char*)src;
	P_FORCE_VECTORIZE_LOOP
		for (int i = 0; i < size; ++i)
			dst_char[i] = src_char[i];
}
#if MACVER
//Suppress warning section
#pragma clang diagnostic pop
#endif

#if 0
/* vector proc brush message - the changeable (per call) stuff for vector procs */
struct VectorBrushMessage
{
	short flags;
	int vectorlength;
	Pixel* pixelptr;
	unsigned char* penmask;
	/* optional quantites in order of likeliness */
	unsigned char* grainmask;	/* for grainy brushes */
	Pixel* cloneptr;			/* for cloning brushes */
	Pixel* impastoptr;			/* for painting with depth */
	Pixel* dryptr;				/* for wet layer brushes */
	Pixel* frisketptr;			/* for drawing on layers */
	/* XY location of start of span in picture space */
	int x;
	int y;
};
//
struct BrushStateRec
{
	Pixel cur_color;
	Pixel bkg_color;
	Pixel paper_color;				// from main_buffer
	PntrFixed grain_fixed;
	unsigned short grain;
	unsigned short grain_mask;
	unsigned short grain_felt;
	unsigned short grain_frac;
	unsigned short grain_257;
	unsigned short opacity;
	unsigned short opacity_mask;
	unsigned short opacity_felt;
	unsigned short opacity_frac;
	unsigned short opacity_257;
	PntrFixed opacity_fixed;
	// Impasto stuff
	PntrFixed depth_fixed;
	PntrFixed smoothing_fixed;
	PntrFixed plow_fixed;
	unsigned short depth_char;
	unsigned short smoothing_char;
	// Eli - to compile ok ZDrawingTypes z_drawing_type;
	// Eli - to compile ok ZDrawingMasks z_drawing_mask;
	// end Impasto
	short* absorption_table;
	unsigned char* density_table;
	int* absorption_table_simd;
	int* density_table_simd;
	Pixel* mask_table;
	// Eli - top compile ok BrushVectorProcType brush_mode;		// vpc_normal, vpc_frisket, or vpc_frisket_inverted
	// destination: indicates whether we are drawing in the image, active floater, or active mask
	// Eli - top compile  ok BrushDestination destination;
	unsigned char wet_difbit;
	bool bleed_composited;
	bool clone_sample;				// from pbdef
	bool random_cloning;				// from pbdef
	bool trans_mask_draw;
	bool need_load_frisket;
	PntrFixed pnt[2];						// accurate dab location
	unsigned short pixel_width;			// width of pixel in bytes
	unsigned short component_offset;	// offset to desired component of a packed pixel
	float glazing_opacity_value;
	float cloneCurrentColorMix;
	float textureCurrentColorMix;
	float textureBlurAmount;
	float stroke_opacity;
	float dab_texture_strength;
};
//
#endif

inline bool trans_mask_load_AUTO_SIMD(Pixel pix, int smask, int& alpha, int& red, int& green, int& blue)
{
	int dmask;
	unsigned int l;
	//printf("Value of smask: %d\n", smask);
	// get color and old alpha, return alpha 0 if destination mask 100%
	if (!trans_mask_decode(pix, alpha, red, green, blue))
		return true;

	dmask = 255 - alpha;
	/* source mask 100% or destination mask 0, store source mask */
	if (smask == 255 || dmask == 0)
	{
		alpha = 255 - smask;
		return true;
	}

	/* properly pro-rate the mask */
	if (dmask == 255)
		alpha = 0;
	else
	{
		/* perform mixdown now in a way that minimizes effect of accuracy bottlenecks */
		/* compute (255 - smask) * dmask * 257 */
		l = (int)(255 - smask) * (int)dmask;
		l += l << 8;
		alpha = 255 - (smask + ((l + 0x00008000) >> 16));
	}
	return true;
}

/* beer's law mixture (felt pen) with grain */
/* vector proc for grain pencil model */
void grain_bandpass_subtractive_MT(VectorBrushMessage* vbm, BrushStateRec* bsr)
{
	int alpha, sred, sgreen, sblue;
	int ii, jj, blue, green, red;
	unsigned int kk, pm, mask;
	Pixel pix;
	Pixel* pixelptr;

	const Pixel* frisketptr = vbm->frisketptr;
	const unsigned char* penmask = vbm->penmask;
	const unsigned char* grainmask = vbm->grainmask;
	const int vectorlength = vbm->vectorlength;
	const int* const absorb = bsr->absorption_table_simd;
	const int* const dens = bsr->density_table_simd;
	/* set up current color absorption spectrum */
	const int cc_red = absorb[ld_red(bsr->cur_color, &bsr->cur_color)];
	const int cc_green = absorb[ld_green(bsr->cur_color, &bsr->cur_color)];
	const int cc_blue = absorb[ld_blue(bsr->cur_color, &bsr->cur_color)];

	int bsr_op257, bsr_grain, bsr_gr257;
	pixelptr = vbm->pixelptr;
	switch (vbm->flags & vbm_type_mask)
	{
	case 2: //vpc_normal:
		if (1) //(glbPainter.pBSR->trans_mask_draw)
		{
			bsr_grain = bsr->grain_frac;
			bsr_op257 = bsr->opacity_257;
			P_FORCE_VECTORIZE_LOOP
				for (ii = 0; ii < vectorlength; ii++, pixelptr++)
				{
					/* traverse all bytes in span */
					kk = *grainmask++;
					pm = *penmask++;
					if (pm == 0)
						continue;
					jj = (pm * bsr_grain) >> 16;
					/* scale grain down and difference */
					jj -= kk >> 3;
					if (jj <= 0)
						continue;
					jj <<= 3;
					if (jj > 255)
						jj = 255;
					kk = jj * bsr_op257;		/* form 16-bit fraction */
					pix = *pixelptr;
					int smask = kk >> 8;

					if (smask == 0)
						continue;

					trans_mask_load_AUTO_SIMD(pix, smask, alpha, sred, sgreen, sblue);
					/* perform blue mix */
					blue = absorb[sblue];
					blue = dens[blue + mulu_shr16(cc_blue, kk)];
					/* perform green mix */
					green = absorb[sgreen];
					green = dens[green + mulu_shr16(cc_green, kk)];
					/* perform red mix */
					red = absorb[sred];
					red = dens[red + mulu_shr16(cc_red, kk)];
					mRecipArray.trans_mask_store_recip_array(pixelptr, alpha, red, green, blue);
				}
			break;
		}
		bsr_grain = bsr->grain_frac;
		bsr_op257 = bsr->opacity_257;
		P_FORCE_VECTORIZE_LOOP
			for (ii = 0; ii < vectorlength; ii++, pixelptr++)
			{
				/* traverse all bytes in span */
				kk = *grainmask++;
				pm = *penmask++;
				if (pm == 0)
					continue;
				jj = (pm * bsr_grain) >> 16;
				/* scale grain down and difference */
				jj -= kk >> 3;
				if (jj <= 0)
					continue;
				jj <<= 3;
				if (jj > 255)
					jj = 255;
				kk = jj * bsr_op257;		/* form 16-bit fraction */
				pix = *pixelptr;
				/* perform blue mix */
				blue = absorb[ld_blue(pix, pixelptr)];
				blue = dens[blue + mulu_shr16(cc_blue, kk)];
				/* perform green mix */
				green = absorb[ld_green(pix, pixelptr)];
				green = dens[green + mulu_shr16(cc_green, kk)];
				/* perform red mix */
				red = absorb[ld_red(pix, pixelptr)];
				red = dens[red + mulu_shr16(cc_red, kk)];
				st_rgbpixel(pixelptr, red, green, blue);
			}
		break;
	case 1: //vpc_frisket:
		// friskets on layers
		if (1) //((*glbPainter.pmain_drawing_picture) != (*glbPainter.pmain_drawing_buffer))
		{
			if (1) //(glbPainter.pBSR->trans_mask_draw)
			{
				bsr_gr257 = bsr->grain_257;
				bsr_op257 = bsr->opacity_257;
				// transparent layer
				P_FORCE_VECTORIZE_LOOP
					for (ii = 0; ii < vectorlength; ii++, pixelptr++, frisketptr++)
					{
						kk = *grainmask++;
						pm = *penmask++;
						if (pm == 0)
							continue;
						mask = get_alpha(*frisketptr);
						jj = (pm * mask * bsr_gr257) >> 16;
						/* scale grain down and difference */
						jj -= kk >> 3;
						if (jj <= 0)
							continue;
						jj <<= 3;
						if (jj > 255)
							jj = 255;
						kk = jj * bsr_op257;		/* form 16-bit fraction */
						pix = *pixelptr;
						int smask = kk >> 8;

						if (smask == 0)
							continue;

						trans_mask_load_AUTO_SIMD(pix, smask, alpha, sred, sgreen, sblue);
						/* perform blue mix */
						blue = absorb[sblue];
						blue = dens[blue + mulu_shr16(cc_blue, kk)];
						/* perform green mix */
						green = absorb[sgreen];
						green = dens[green + mulu_shr16(cc_green, kk)];
						/* perform red mix */
						red = absorb[sred];
						red = dens[red + mulu_shr16(cc_red, kk)];
						mRecipArray.trans_mask_store_recip_array(pixelptr, alpha, red, green, blue);
					}
			}
			else
			{
				bsr_gr257 = bsr->grain_257;
				bsr_op257 = bsr->opacity_257;
				// "Preserve Transparency" layer
				P_FORCE_VECTORIZE_LOOP
					for (ii = 0; ii < vectorlength; ii++, pixelptr++, frisketptr++)
					{
						kk = *grainmask++;
						pm = *penmask++;
						if (pm == 0)
							continue;
						mask = get_alpha(*frisketptr);
						jj = (pm * mask * bsr_gr257) >> 16;
						/* scale grain down and difference */
						jj -= kk >> 3;
						if (jj <= 0)
							continue;
						jj <<= 3;
						if (jj > 255)
							jj = 255;
						kk = jj * bsr_op257;		/* form 16-bit fraction */
						pix = *pixelptr;
						/* perform blue mix */
						blue = absorb[ld_blue(pix, pixelptr)];
						blue = dens[blue + mulu_shr16(cc_blue, kk)];
						/* perform green mix */
						green = absorb[ld_green(pix, pixelptr)];
						green = dens[green + mulu_shr16(cc_green, kk)];
						/* perform red mix */
						red = absorb[ld_red(pix, pixelptr)];
						red = dens[red + mulu_shr16(cc_red, kk)];
						st_rgbpixel(pixelptr, red, green, blue);
					}
			}
			break;
		}
		bsr_gr257 = bsr->grain_257;
		bsr_op257 = bsr->opacity_257;
		P_FORCE_VECTORIZE_LOOP
			for (ii = 0; ii < vectorlength; ii++, pixelptr++)
			{
				/* traverse all bytes in span */
				kk = *grainmask++;
				pm = *penmask++;
				if (pm == 0)
					continue;
				pix = *pixelptr;
				mask = ld_alpha(pix, pixelptr);
				jj = (pm * mask * bsr_gr257) >> 16;
				/* scale grain down and difference */
				jj -= kk >> 3;
				if (jj <= 0)
					continue;
				jj <<= 3;
				if (jj > 255)
					jj = 255;
				kk = jj * bsr_op257;		/* form 16-bit fraction */
											/* perform blue mix */
				blue = absorb[ld_blue(pix, pixelptr)];
				blue = dens[blue + mulu_shr16(cc_blue, kk)];
				/* perform green mix */
				green = absorb[ld_green(pix, pixelptr)];
				green = dens[green + mulu_shr16(cc_green, kk)];
				/* perform red mix */
				red = absorb[ld_red(pix, pixelptr)];
				red = dens[red + mulu_shr16(cc_red, kk)];
				st_rgbpixel(pixelptr, red, green, blue);
			}
		break;
	case 3: //vpc_inverted_frisket:
		// friskets on layers
		if (1) //((*glbPainter.pmain_drawing_picture) != (*glbPainter.pmain_drawing_buffer))
		{
			if (1) //(glbPainter.pBSR->trans_mask_draw)
			{
				bsr_gr257 = bsr->grain_257;
				bsr_op257 = bsr->opacity_257;
				/*
				int pmCnt = 0;
				int jjCnt = 0;
				int smCnt = 0;
				int luCnt = 0;
				*/
				// transparent layer
				P_FORCE_VECTORIZE_LOOP
					for (ii = 0; ii < vectorlength; ii++, pixelptr++, frisketptr++)
					{
						kk = *grainmask++;
						pm = *penmask++;
						if (pm == 0)
						{
							//pmCnt += 1;
							continue;
						}//	continue;
						mask = 255 - get_alpha(*frisketptr);
						jj = (pm * mask * bsr_gr257) >> 16;
						/* scale grain down and difference */
						jj -= kk >> 3;
						if (jj <= 0)
						{
							//jjCnt += 1;
							continue;
						}//	continue;
						jj <<= 3;
						if (jj > 255)
							jj = 255;
						kk = jj * bsr_op257;		/* form 16-bit fraction */
						pix = *pixelptr;
						int smask = kk >> 8;

						if (smask == 0)
						{
							//smCnt += 1;
							continue;
						}//	continue;
						//luCnt += 1;

						trans_mask_load_AUTO_SIMD(pix, smask, alpha, sred, sgreen, sblue);
						/* perform blue mix */
						blue = absorb[sblue];
						//int densIdx = blue + mulu_shr16(cc_blue, kk);
						blue = dens[blue + mulu_shr16(cc_blue, kk)];
						/* perform green mix */
						green = absorb[sgreen];
						green = dens[green + mulu_shr16(cc_green, kk)];
						/* perform red mix */
						red = absorb[sred];
						red = dens[red + mulu_shr16(cc_red, kk)];
						mRecipArray.trans_mask_store_recip_array(pixelptr, alpha, red, green, blue);
					}
				/*
				printf("penmask=0, count = %d\n", pmCnt);
				printf("jj=0, count: %d\n", jjCnt);
				printf("smask=0, count: %d\n", smCnt);
				printf("lookUp count: %d\n", luCnt);
				printf("Inter count: %d\n", ii);
				*/
			}
			else
			{
				bsr_gr257 = bsr->grain_257;
				bsr_op257 = bsr->opacity_257;
				// "Preserve Transparency" layer
				P_FORCE_VECTORIZE_LOOP
					for (ii = 0; ii < vectorlength; ii++, pixelptr++, frisketptr++)
					{
						kk = *grainmask++;
						pm = *penmask++;
						if (pm == 0)
							continue;
						mask = 255 - get_alpha(*frisketptr);
						jj = (pm * mask * bsr_gr257) >> 16;
						/* scale grain down and difference */
						jj -= kk >> 3;
						if (jj <= 0)
							continue;
						jj <<= 3;
						if (jj > 255)
							jj = 255;
						kk = jj * bsr_op257;		/* form 16-bit fraction */
						pix = *pixelptr;
						/* perform blue mix */
						blue = absorb[ld_blue(pix, pixelptr)];
						blue = dens[blue + mulu_shr16(cc_blue, kk)];
						/* perform green mix */
						green = absorb[ld_green(pix, pixelptr)];
						green = dens[green + mulu_shr16(cc_green, kk)];
						/* perform red mix */
						red = absorb[ld_red(pix, pixelptr)];
						red = dens[red + mulu_shr16(cc_red, kk)];
						st_rgbpixel(pixelptr, red, green, blue);
					}
			}
			break;
		}
		bsr_gr257 = bsr->grain_257;
		bsr_op257 = bsr->opacity_257;
		P_FORCE_VECTORIZE_LOOP
			for (ii = 0; ii < vectorlength; ii++, pixelptr++)
			{
				/* traverse all bytes in span */
				kk = *grainmask++;
				pm = *penmask++;
				if (pm == 0)
					continue;
				pix = *pixelptr;
				mask = 255 - ld_alpha(pix, pixelptr);
				jj = (pm * mask * bsr_gr257) >> 16;
				/* scale grain down and difference */
				jj -= kk >> 3;
				if (jj <= 0)
					continue;
				jj <<= 3;
				if (jj > 255)
					jj = 255;
				kk = jj * bsr_op257;		/* form 16-bit fraction */
											/* perform blue mix */
				blue = absorb[ld_blue(pix, pixelptr)];
				blue = dens[blue + mulu_shr16(cc_blue, kk)];
				/* perform green mix */
				green = absorb[ld_green(pix, pixelptr)];
				green = dens[green + mulu_shr16(cc_green, kk)];
				/* perform red mix */
				red = absorb[ld_red(pix, pixelptr)];
				red = dens[red + mulu_shr16(cc_red, kk)];
				st_rgbpixel(pixelptr, red, green, blue);
			}
		break;
	}
}
