/**
mod 08-22-02 jeffp - added dir_param argument to GetAnimationFraction for v8 directions
mod 12 18 00 tsh - use bool
MOD 12-11-00 MAZ - FINALLY opacity => grain, conc => opacity terminology in BrushStateRec
MOD 11-01-00 MAZ - add a "max" for some enum types
MOD 10-31-00 MAZ - change WellPreproc resat parameter from unsigned short to float
mod 6 28 00 tsh - add bleed_composited, optimize transparent drawing if opaque

Copyright (c) 1994-2000 Fractal Design Corporation, Aptos, CA. All Rights Reserved.
**/

#ifndef __AIBRUSH__
#define __AIBRUSH__

/*******************************************************************************
**
**	Imports
**
**/

//#include "AITypes.h"
//#include "AIEvent.h"
//#include "AIPlugin.h"
//#include "PntrFixed.h"
//#include "DrawEngMAZlib.h"
#pragma once

#if WIN32 
#include <stdint.h>
#else
#include <inttypes.h>
#endif

typedef uint32_t Pixel;
#define SIZEpixel 4

// 'null' color, to be used only where 'Pixel' is treated as RGB value (w/o alpha)
const Pixel nullColor = 0xFFFFFFFF;

struct BrushTrackingState;

/* Pragma to byte align structures 
(make sure to have an even number of bytes in structs) */
#if defined(__BORLANDC__)
#pragma option -a1
#endif

#if WIN32
#pragma pack(push,1)
#endif

#if __powerc
#pragma pack(push, 2) // replace mac68k
#endif


/*******************************************************************************
**
** Constants
**
**/

#define kAIBrushSuite			"FDC Go Plugin Brush"
#define kAIBrushVersion			2

#if __MACH__
	#include <Renderer/Pixel.h>
#else
	//#include "Pixel.h"
#endif

/* These are the options available to a brush. */
#define kBrushMethod		0x00000001		// brush appears on "Plug-in" menu (method category)
#define kBrushStroke		0x00000002		// brush appears on stroke type menu
#define kBrushHasOptions	0x00000004  	// When this option is passed into AddBrush() the host 
// will call the plugin with the kSelectorBrushOptions
// selector when the user has requested to see the brush options
#define kBrushPackVariant	0x00000008		// Brush packs private data in variant

#define kBrushEnableSize	0x00000100 
#define kBrushEnableAngle	0x00000200 
#define kBrushEnableSpacing	0x00000400 
#define kBrushEnableRandom	0x00000800 
#define kBrushEnableBristle	0x00001000 
#define kBrushEnableCloning	0x00002000 
#define kBrushEnableRake	0x00004000 
#define kBrushEnableWater	0x00008000 
#define kBrushEnableWell	0x00010000 
#define kBrushEnableSliders	0x00020000 

#ifndef PI_IDENTIFIER_SIZE
#define PI_IDENTIFIER_SIZE		32
#endif

/*******************************************************************************
**
** Selectors
**
**/

#define kSelectorAIGetBrushParameters	"AI Get File Format Parameters"

// kSelectorBrushOptions is an opportunity for the brush to display an options dialog. 
// The options are invoked when the user chooses plug-in options from the Brush Controls menu. 
#define kSelectorBrushOptions			"Plugin Brush Options"	

// kSelectorBrushPack is used when the host wants to save the brush's current custom parameters,
// so the user can restore to this state later. The message will be of type PBrushPackMessage. The
// plug-in has the option of encoding its parameters into 44 bytes of private data (passed back in
// private_data field), and/or storing its parameters in a custom resource of type res_type. The
// plug-in must pack the resource data into a handle and pass it back in the resH field.
// must set kBrushPackVariant in options when brush is added.
#define kSelectorBrushPack				"Plugin Brush Pack"	

// kSelectorBrushUnpack selector is used when the host wants to restore the plug-in brush
// state.  The message will be of type PBrushUnpackMessage. The plug-in will receive any
// data saved in the private_data field and any resource data saved in the handle.
// must set kBrushPackVariant in options when brush is added.
#define kSelectorBrushUnpack			"Plugin Brush Unpack"	


/*******************************************************************************
**
** Plugin Brush Types
**
**/

/* Global Brush State Record Definition */

typedef void *PBrushObj;  

#ifndef PicObj_defined
typedef void *PicObj;			// opaque reference to a Picture
#define PicObj_defined
#endif

enum PictureLayerType
{
	picture_layer = 0,
	wet_layer,
	mask_layer,
	temp_layer,
	depth_layer,
	frisket_layer,
	thickpaint_layer,
	all_layers,
	num_layers
};  


/* Values for destination field in BrushStateRec */
enum BrushDestination
{
	kCanvasDest = 1,
	kFloaterDest,
	kMaskDest
};

/* vector proc types */
enum BrushVectorProcType
{
	vpc_illegal = 0,
	vpc_normal,
	vpc_frisket,
	vpc_inverted_frisket
};

// impasto defines
// z drawing types
enum ZDrawingTypes
{
	zd_default = 0,
	zd_pen,
	zd_pen_limit,
	zd_pen_grain,
	zd_pen_original_lum,
	zd_pen_weaving,
	zd_pen_texture,
	zd_max
};

// z drawing mask bits
enum ZDrawingMasks
{
	zd_subtract_bit		=	0x08,
	zd_invert_bit		=	0x04,	
	zd_draw_depth		=	0x02,
	zd_draw_color		=	0x01
};


struct BrushStateRec
{
	Pixel cur_color;
	Pixel bkg_color;
	Pixel paper_color;				/* from main_buffer */
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
	ZDrawingTypes z_drawing_type;
	ZDrawingMasks z_drawing_mask;
	// end Impasto
	short *absorption_table;
	unsigned char *density_table;
	int *absorption_table_simd;
	int *density_table_simd;
	Pixel *mask_table;
	BrushVectorProcType brush_mode;		/* vpc_normal, vpc_frisket, or vpc_frisket_inverted */
	/* destination: indicates whether we are drawing in the image, active floater, or active mask */
	BrushDestination destination;
	unsigned char wet_difbit;
	bool bleed_composited;
	bool clone_sample;				/* from pbdef */
	bool random_cloning;				/* from pbdef */
	bool trans_mask_draw;
	bool need_load_frisket;
	PntrFixed pnt[2];						/* accurate dab location */
	unsigned short pixel_width;			/* width of pixel in bytes */
	unsigned short component_offset;	/* offset to desired component of a packed pixel */
	float glazing_opacity_value;
	float cloneCurrentColorMix;
	float textureCurrentColorMix;
	float textureBlurAmount;
	float stroke_opacity;
	float dab_texture_strength;
};

/* animator controllers for GetAnimationFraction */
typedef enum
{
	rt_none = 1,
	rt_velocity,
	rt_angle,
	rt_pressure,
	rt_wheel,
	rt_tilt,
	rt_bearing,
	rt_rotation,
	rt_clone,
	rt_random,
	rt_sequential,
	rt_max
} BrushAnimators;

// List of brush parameters that can be controlled in real-time
typedef enum
{
	eNoneControl = 1,
	eRadiusControl,
	eAngleControl,
	eFeatureSizeControl,
	eFlowControl,
	eJitterControl,
	eOpacityControl,
	eGrainControl,
	eGraduationControl,
	eBleedControl,
	eResatControl,
	eDepthControl,
	eRank1Control,
	eRank2Control,
	eRank3Control,
	eVolumeControl,
	ePickupControl,
	ePlowControl,
	eParticlesForceControl,
	eParticlesDirectionControl,
	eParticlesGlobalChaosControl,
	eParticlesLocalChaosControl,
	eFlowParticlesPositionControl,
	eGravityParticlesSpinRateControl,
	eSpringParticlesStiffnessControl,
	eDynamicSpeckleThicknessControl,
	eColorVariabilityControl,
	eGlazingOpacityControl,
	eDabTextureStrengthControl,
	eCloneColorControl,
	eTextureColor,
	eTextureBlur,
	eTPThicknessControl,
	eMaxControl
} BrushController;

struct BrushParams
{
	/* from brush state record */
	Pixel cur_color;
	Pixel bkg_color;
	Pixel paper_color;				/* from main_buffer */
	PntrFixed grain_fixed;
	short brush_mode;				/* vpc_normal, vpc_frisket, or vpc_frisket_inverted */
	PntrFixed opacity_fixed;
	unsigned char wet_difbit;
	bool clone_sample;				/* from pbdef */
	bool random_cloning;			/* from pbdef */
	/* pen def */
	PntrFixed radius_log;
	PntrFixed base_angle;
	PntrFixed thinness;	
	short profile;
	unsigned char clone_type;
	unsigned char clone_flags;
	PntrFixed delta_angle;
	PntrFixed min_angle;
	PntrFixed factor;
	PntrFixed rand_rad;
	PntrFixed speckle_scale;
	PntrFixed scale_size;
	short flags;  /* use the following defines */
#define BCR_grain				0x0001
#define BCR_bristle_model		0x0002
#define BCR_random_grain_lookup	0x0004
#define BCR_random_clone_lookup	0x0010
#define BCR_full_edge			0x0020
	PntrFixed advance;
	PntrFixed min_advance;
	PntrFixed jitter;
	PntrFixed h_jitter;
	PntrFixed s_jitter;
	PntrFixed v_jitter;
	BrushAnimators radius_animator;
	BrushAnimators jitter_animator;
	BrushAnimators grain_animator;
	BrushAnimators graduation_animator;
	BrushAnimators opacity_animator;
	BrushAnimators angle_animator;
	BrushAnimators bleed_animator;
	BrushAnimators resat_animator;
	BrushAnimators rank1_animator;
	BrushAnimators rank2_animator;
	BrushAnimators rank3_animator;
	char bp_pad;
	short clone_jitter_amount;
	short clone_jitter_time_threshold;
	short njitter; /* number of bristles around the ring or in Van Gogh stroke */
	unsigned short color_resat_frac;
	unsigned short color_bleed_frac;
	bool pressure_radius; /* 1: bristles spread with pressure, 0: contact angle grows */
	bool wet;
	PntrFixed wet_fringe;
	short post_diffuse;
	short tapering_resat;
	PntrFixed contact_angle; /* half the contact angle between ring and perp line to stroke */
	PntrFixed bristle_scale; /* how big the brush ring is compared to pen radius */
	PntrFixed turn; /* how much the brush turns around curves (fix_one is normal) */
	short stroke_type; /* stroke types */
};

struct BrushSavePnt
{
	PntrFixed loc2[2];
	int time;
	PntrFixed pressure2;
	PntrFixed tilt;
	PntrFixed bearing;
	PntrFixed wheel;
	PntrFixed rotation;

	struct Audio
	{
		explicit Audio() : level(-1) {}
		/// @return true If Audio Expression is enabled.
		bool isOn() const { return level >= 0; }
		float level;
	} audio;
};

struct AddBrushData
{
	wchar_t brushName[128];
	unsigned char brushId[PI_IDENTIFIER_SIZE];  // unique identifier for this plugin (do not localize)
	Pixel *brushIcon;	
	Rect *brush_size;
	int options;			// changed from method_or_stroke to be more generic
};

/* vector proc brush message - the changeable (per call) stuff for vector procs */
struct VectorBrushMessage
{
	short flags;
	int vectorlength;
	Pixel *pixelptr;
	unsigned char *penmask;
	/* optional quantites in order of likeliness */
	unsigned char *grainmask;	/* for grainy brushes */
	Pixel *cloneptr;			/* for cloning brushes */
	Pixel *impastoptr;			/* for painting with depth */
	Pixel *dryptr;				/* for wet layer brushes */
	Pixel *frisketptr;			/* for drawing on layers */
	/* XY location of start of span in picture space */
	int x;
	int y;
};

/* vector brush message flag bits and masks */
#define vbm_grainmask_valid		0x8000
#define vbm_clone_valid			0x4000
#define vbm_dry_valid			0x2000
#define vbm_type_mask			0x000F

/* types of storeproc/tileprocs */
typedef enum
{
	st_regular = 1,			/* pen_store and regular_tile */
	st_grain,				/* pen_store and grain_tile */
	st_clone,				/* clone_store and regular_clone_tile */
	st_grain_clone,			/* clone_store and grain_clone_tile */
	st_wet_grain,			/* wet_store and grain_wet_tile */
	st_mask,				/* pen_mask_store and regular_mask_tile */
	st_grain_mask,			/* pen_mask_store and grain_mask_tile */
	st_vector_only			/* only set vector proc */
} BrushOtherProcTypes;

/* encapsulated brush tracking state - generally passed to animator evaluation procedures */
struct BrushTrackingState
{
	BrushSavePnt *p;		/* -> location, pressure, angle, bearing, time from spooler */
	PntrFixed velocity;		/* higher-level computed velocity of brush */
	PntrFixed dx;			/* x component of unit direction vector */
	PntrFixed dy;			/* y component of unit direction vector */
};

/* messages for the color modify proc */
typedef enum
{
	cmp_expects_hsv = 1,
	cmp_will_modify_color,
	cmp_modify_color,
	cmp_modify_color_stable
} BrushCMPMessage;

/* angle-treatment constants for GetAnimationFraction callback*/
typedef enum
{
	angle_standard,
	angle_180,			/* for impressionist brush, etc. */
	angle_360,			/* for angle animation in dab brushes */
	angle_absolute		/* 360 allowing for rotated page */
} BrushAngleTreatment;

/* here are the prototypes for various procs used in brushes */

/* update location where brush has drawn */
typedef void (*picture_update_proc)(PicObj pic, Rect *r, RgnHandle abortregion);  // -->RgnHandle a prob for windows ??
/* store proc type definition */
typedef void (*store_proc)(void *pen, Point loc, PntrFixed pt[2], PicObj pic, BrushStateRec *bsr);
/* vector proc prototype type definition */
typedef void (*vector_proc)(VectorBrushMessage *vbm, BrushStateRec *bsr);
/* brush dab location modify proc type definition */
typedef void (*loc_modify_proc)(PntrFixed pt[2], BrushTrackingState *bts);
// Anti aliased line procs
typedef void(*WC_AALineProc)(uint32 brush_color, uint32& suspension_dye, uint32& water, pntr_int32 frac);

/*
when message = cmp_expects_hsv, the proc returns true if it expects the
pixel to be passed in HSVA format, false if the caller should pass ARGB
pixel format - it ignores the parameter p

when message = cmp_will_modify_color, the proc returns true if
the proc will actually modify the color, false if no
modification will occur - it ignores the parameter p

when message = cmp_modify color, the proc returns true if
it actually modified the color, false if no
modification occured - the color parameter in *p is changed

the parameter bts is provided in case color modification in a plug-in should
be controlled by the result of an animator like pressure or bearing
*/

/* brush dab color modify proc type definition */
typedef bool (*color_modify_proc)(BrushCMPMessage message, BrushTrackingState *bts, Pixel *p);
/* first point proc type definition */
typedef void (*first_point_proc)(PicObj pic, BrushTrackingState *bts);
/* next point proc type definition */
typedef void (*next_point_proc)(BrushTrackingState *bts);
/* end stroke proc type definition */
typedef void (*end_proc)(void);
/* notify proc*/
typedef void (*notify_proc)(void);

/* The contents of a brush pack message (passed in with the kSelectorBrushPack.) */
struct PBrushPackMessage
{
	AIPluginData d;
	unsigned char private_data[44]; // plug-in brush's private data in ascii format
	int res_type;		// plug-in brush's custom resource type (optional)
	Handle resH;		// handle to contents of the custom resource
};

/* The contents of a brush unpack message (passed in with the kSelectorBrushUnpack.) */
struct PBrushUnpackMessage
{
	AIPluginData d;
	unsigned char private_data[44]; // plug-in brush's private data in ascii format
	Handle resH;		// handle to contents of the custom resource
};


/*******************************************************************************
**
**	Suite
**
**/

typedef struct
{
	MACPASCAL FXErr (*AddBrush)(AIPluginHandle self, AddBrushData *data);

	/* note: these proc pointers are native architecture (PPC, 68K, Intel) not UniversalProcPtrs */
	/* note also: assume C calling conventions for the procs themselves, not Pascal */
	MACPASCAL FXErr (*GetVectorProc)(vector_proc *proc);
	MACPASCAL FXErr (*SetVectorProc)(vector_proc proc, BrushOtherProcTypes other_procs);
	MACPASCAL FXErr (*GetStoreProc)(store_proc *proc);
	MACPASCAL FXErr (*SetStoreProc)(store_proc proc);
	MACPASCAL FXErr (*GetColorModifyProc)(color_modify_proc *proc);
	MACPASCAL FXErr (*SetColorModifyProc)(color_modify_proc proc);
	MACPASCAL FXErr (*GetLocModifyProc)(loc_modify_proc *proc);
	MACPASCAL FXErr (*SetLocModifyProc)(loc_modify_proc proc);
	MACPASCAL FXErr (*GetGeneralProcs)(first_point_proc *proc1, next_point_proc *proc2, end_proc *proc3);
	MACPASCAL FXErr (*SetGeneralProcs)(first_point_proc proc1, next_point_proc proc2, end_proc proc3);

	/* get the pen rectangle origined at loc */
	MACPASCAL FXErr (*GetPenRect)(void *pen, Point loc, Rect *r);
	/* set up undo context to be marked for wet layer swap */
	MACPASCAL FXErr (*SetWetUndoContext)(PicObj pic);
	/* get current cloning info */
	MACPASCAL FXErr (*GetCloneInfo)(PicObj *pclone_source, short *pclone_delta_h,
		short *pclone_delta_v, bool *pclone_wrap);
	/* get buffers allocated for current store-level brush */
	MACPASCAL FXErr (*GetBrushBuffers)(Pixel **pclone_buf, short *pcb_height, short *pcb_width,
		Pixel **pclone_composite_buf, short *pccb_length, short thread_num);
	/* get a pointer to a grain buffer for the brush span */
	MACPASCAL unsigned char *(*GetBrushGrain)(short x, short y, short len);
	/* get the current radius for store-level brush */
	MACPASCAL PntrFixed (*GetBrushRadius)(void);
	/* get the current stroke direction for store-level brush */
	MACPASCAL PntrFixed (*GetStrokeDirection)(void);
	/* update grain offsets per dab for store-level brush */
	MACPASCAL FXErr (*UpdateGrainOffsets)(void);
	/* update cloning offsets per dab for store-level brush (usually when pending) */
	MACPASCAL FXErr (*UpdateCloneOffsets)(Point loc, bool true_cloner);
	/* generalized pen span enumerator. note: loc is origin for dab, "r" is returned */
	MACPASCAL FXErr (*ForAllPenSpans)(void *pen, Point loc, Rect *r, void *data,
		void (*rectproc)(Rect *ir, Rect *pr, Point topleft, short xorg, short yorg, void *data),
		void (*rowproc)(short y, void *data),
		void (*spanproc)(unsigned char *sp, short len, short x, short y, void *data),
		void* (*getdataproc)(void *data));

	/* called by high-level brushes at the start of the stroke */
	MACPASCAL PntrFixed (*AnimationStart)(void);
	/* called for getting animation fractions (used in stroke real-time parameter-changing) */
	MACPASCAL PntrFixed (*GetAnimationFraction)(BrushAnimators type, bool invert, float dir_param, BrushAngleTreatment angle_treatment, BrushTrackingState *bts, BrushController brushControl );

	/* this is called at brush first point to set up last_dab_color */
	MACPASCAL Pixel (*WellInit)(BrushSavePnt *p, Pixel initial_color, bool do_clone, bool offset_clone);
	/* this routine is called before dab store to handle bleed pickup and resaturation */
	MACPASCAL FXErr (*WellPreproc)(Point loc, PntrFixed cur_radius, Pixel induction_color,
		float resat, float resat_alpha, BrushTrackingState *bts);
	/* this call is made after store to handle color bleeding into cur color */
	MACPASCAL Pixel (*WellPostproc)(BrushTrackingState *bts, float& resat_alpha);

	/* used for setting the current color from the clone at a location */
	MACPASCAL FXErr (*CloneSampleColor)(PntrFixed pt[2]);

	/* used for enumerating pattern symmetries (in strokes) */
	MACPASCAL FXErr (*EnumeratePatternSymmetries)(PicObj pic, Point loc, PntrFixed pt[2], void *data,
		void (*ptproc)(PicObj pic, Point loc, PntrFixed pt[2], void *data));
	/* used for enumerating pattern symmetries - PntrFixed-point version */
	MACPASCAL FXErr (*EnumeratePatternSymmetriesFixed)(PicObj pic, PntrFixed pt[2], void *data,
		void (*ptproc)(PicObj pic, PntrFixed pt[2], void *data));

	/* used in general brushes and some plugin-floaters with brush tools for spooling input points */
	MACPASCAL FXErr (*Track)(EventRecord *e, PicObj pic, void (*first_point)(BrushSavePnt *p, short modifiers),
		void (*next_point)(BrushSavePnt *p), void (*end)(void));
	/* returns the current brush parameters as determined by the host UI */
	MACPASCAL FXErr (*GetBrushParams)(BrushParams *params);
	/* enable or disable brush control menu items */
	MACPASCAL FXErr (*SetBrushEnables)(PBrushObj brush, int options);
	/* get proc to update area brush has drawn */
	MACPASCAL FXErr (*GetPictureUpdateProc)(picture_update_proc *proc);
	/* set proc to update area brush has drawn */
	MACPASCAL FXErr (*SetPictureUpdateProc)(picture_update_proc proc);

} AIBrushSuite;


/*********************************************************************************/
/* turn off the byte align pragma back to its original state */
#if WIN32
#pragma pack(pop)
#endif

#if defined(__BORLANDC__)
#pragma option -a.
#endif

#if __powerc
#pragma pack(pop) // options align=reset
#endif

/* define old names for ease of conversion [#define NEWBRUSHINTERFACE to disable]  */
#if __BUILD_PLUGIN__ && defined(_ASIPORT_) && !defined(NEWBRUSHINTERFACE)
// typedef struct's
#define brush_state_rec			BrushStateRec
#define save_pnt				BrushSavePnt
#define brush_tracking_state	BrushTrackingState

// function names
#define add_brush				AddBrush
#define get_vector_proc			GetVectorProc
#define set_vector_proc			SetVectorProc
#define get_store_proc			GetStoreProc
#define set_store_proc			SetStoreProc
#define	get_color_modify_proc	GetColorModifyProc
#define	set_color_modify_proc	SetColorModifyProc
#define	get_loc_modify_proc		GetLocModifyProc
#define	get_general_procs		GetGeneralProcs
#define	set_general_procs		SetGeneralProcs
#define	get_pen_rect			GetPenRect
#define	set_wet_undo_context	SetWetUndoContext
#define	get_clone_info			GetCloneInfo
#define	get_brush_buffers		GetBrushBuffers
#define	brush_get_grain			GetBrushGrain
#define	get_brush_radius		GetBrushRadius
#define	get_stroke_direction	GetStrokeDirection
#define	update_grain_offsets	UpdateGrainOffsets
#define	update_clone_offsets	UpdateCloneOffsets
#define	for_all_pen_spans		ForAllPenSpans
#define	animation_start			AnimationStart
#define	get_animation_fraction	GetAnimationFraction
#define	well_init				WellInit
#define	well_preproc			WellPreproc
#define	well_postproc			WellPostproc
#define	clone_sample_color		CloneSampleColor
#define	enumerate_pattern_symmetries	EnumeratePatternSymmetries
#define	enumerate_pattern_symmetries_fixed	EnumeratePatternSymmetriesFixed
#define	track					Track
#endif

/* pixel load & store */
#define ld_alpha(pix,ptr) ((pix) >> 24)
#define ld_red(pix,ptr) (((pix) >> 16) & 0xFF)
#define ld_green(pix,ptr) (((pix) >> 8) & 0xFF)
#define ld_blue(pix,ptr) ((pix) & 0xFF)

#define get_alpha(pix) ((pix) >> 24)
#define get_red(pix) (((pix) >> 16) & 0xFF)
#define get_green(pix) (((pix) >> 8) & 0xFF)
#define get_blue(pix) ((pix) & 0xFF)

#define st_pixel(p,a,r,g,b) *p = ((((((unsigned int)(a) << 8) | (unsigned int)(r)) << 8) | \
	(unsigned int)(g)) << 8) | (unsigned int)(b)
#define st_rgbpixel(p,r,g,b) *p = (*p & 0xFF000000UL) | ((((unsigned int)(r) << 8) | \
	(unsigned int)(g)) << 8) | (unsigned int)(b)
#define st_alpha(p,a) *p = (*p & 0x00FFFFFFL) | ((unsigned int)(a) << 24)

#define BUILD_RGB(r, g, b)		(((unsigned int)(r)<<16) | ((unsigned int)(g)<<8) | (b))
#define BUILD_ARGB(a, r, g, b)	(((unsigned int)(a)<<24) | ((unsigned int)(r)<<16) | ((unsigned int)(g)<<8) | (b))

#define BUILD_HSV(h, s, v)	(((int)(h)<<24) | ((int)(s)<<16) | ((int)(v)<<8))
#ifndef BUILD_HSVA
#define BUILD_HSVA(h, s, v, a)	(((int)(h)<<24) | ((int)(s)<<16) | ((int)(v)<<8) | (a & 0x000000FFL))
#endif

#define mulu_shr8(a,b) (((unsigned int)(a) * (unsigned int)(b)) >> 8)
#define mulu_shr16(a,b) (((unsigned int)(a) * (unsigned int)(b)) >> 16)
#define mulu_shr13(a,b) (((unsigned int)(a) * (unsigned int)(b)) >> 13)
#define mulu_shr16r(a,b) (((unsigned int)(a) * (unsigned int)(b) + 0x00008000L) >> 16)
#define muls_shr15(a,b) (((int)(a) * (int)(b) + 0x00004000) >> 15)

#define RGB2LUMA(r,g,b) ((0.299f*(r))+(0.587f*(g))+(0.114f*(b)))
#define RGB2CHROMA_B(r,g,b) ((-0.168736f*(r))-(0.331264f*(g))+(0.5f*(b)))
#define RGB2CHROMA_R(r,g,b) ((0.5f*(r))-(0.418686f*(g))-(0.081312f*(b)))

#define CHROMA2R(CR) (0.5f+(1.402f*(CR)))
#define CHROMA2G(CB,CR) (0.5f-(0.344136f*(CB))-(0.714136f*(CR)))
#define CHROMA2B(CB) (0.5f+(1.772f*(CB)))

#define LUMACHROMA2R(Y,CR) ((Y)+(1.402f*(CR)))
#define LUMACHROMA2G(Y,CB,CR) ((Y)-(0.344136f*(CB))-(0.714136f*(CR)))
#define LUMACHROMA2B(Y,CB) ((Y)+(1.772f*(CB)))

/* transparent mask color decode */
inline bool trans_mask_decode(Pixel pix, short &alpha, short &red, short &green, short &blue)
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

/* transparent mask load and mask compute */
inline bool trans_mask_load(Pixel pix, int smask, short &alpha, short &red, short &green, short &blue)
{
	int dmask;
	unsigned int l;

	/* handle some cases quickly */
	if (smask == 0)
		return false;		/* destination stays the same if source mask is 0 */

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

/* transparent mask color decode */
inline bool trans_mask_decode(Pixel pix, int &alpha, int &red, int &green, int &blue)
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

/* transparent mask load and mask compute */
inline bool trans_mask_load(Pixel pix, int smask, int &alpha, int &red, int &green, int &blue)
{
	int dmask;
	unsigned int l;

	/* handle some cases quickly */
	if (smask == 0)
		return false;		/* destination stays the same if source mask is 0 */

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

/* transparent mask encode */
inline Pixel trans_mask_encode(int alpha, int red, int green, int blue)
{
	int mincomp, recip;
	Pixel pixResult = BUILD_ARGB(alpha, red, green, blue);
	if (alpha != 0 && alpha != 255)
	{
		// solve: comp = ((255 - alpha) * x + 255 * alpha) / 255
		// x = 255 * (comp - alpha) / (255 - alpha)
		mincomp = red;
		if (mincomp > green)
			mincomp = green;
		if (mincomp > blue)
			mincomp = blue;
		// increase alpha so color is correct
		if (mincomp < alpha)
			alpha = mincomp;
		if (alpha != 255)
		{
			// adjust for composite with white
			recip = (255 << 15) / (255 - alpha);
			red = (recip * (red - alpha) + 0x4000) >> 15;
			green = (recip * (green - alpha) + 0x4000) >> 15;
			blue = (recip * (blue - alpha) + 0x4000) >> 15;
			pixResult = BUILD_ARGB(alpha, red, green, blue);
		}
	}
	//	if (alpha < 0 || alpha > 255 || red < 0 || red > 255 || green < 0 || green > 255 ||
	//		blue < 0 || blue > 255)
	//		Debugger();
	return pixResult;
}

/* transparent mask store */
inline void trans_mask_store(Pixel *pixelptr, int alpha, int red, int green, int blue)
{
	*pixelptr = trans_mask_encode(alpha, red, green, blue);
}

Pixel TransMix(Pixel b, Pixel a, PntrFixed frac);

#endif  /* __AIBRUSH__ */
