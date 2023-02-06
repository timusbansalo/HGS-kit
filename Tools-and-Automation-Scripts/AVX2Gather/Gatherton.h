#pragma once

#if 0 //WIN32
#include <stdint.h>
#else
#include <inttypes.h>
#endif

typedef int PntrFixed;
typedef uint32_t Pixel;
#define SIZEpixel 4

/* vector proc brush message - the changeable (per call) stuff for vector procs */
struct _VectorBrushMessage
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
typedef _VectorBrushMessage VectorBrushMessage;

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

void grain_bandpass_subtractive_MT(VectorBrushMessage* vbm, BrushStateRec* bsr);
void grain_bandpass_subtractive_ST(VectorBrushMessage* vbm, BrushStateRec* bsr, int);
void process_depth_well_smoothing_MT(int stroke_id, Pixel* pp, int rows, int cols, int dy, int dx);
void process_depth_well_smoothing_ST(int stroke_id, Pixel* pp, int rows, int cols, int dy, int dx);

