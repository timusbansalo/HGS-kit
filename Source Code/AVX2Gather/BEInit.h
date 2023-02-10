#pragma once
//#include "BrushEngine.h"
//#include "MCBasicTypes.h"
#include "AIBrush.h"
//#include "rip.h"
//#include "Masks.h"
//#include "OpenCL/OpenCLKernelsTypes.h"

class picture;
class IPainterPicture;
class canvas;
class CBrushGlobals;
class mask_entry;
struct BrushStateRec;
class BrushDefinition;
struct BrushTrackingState;
class CBrush;
struct aa_pen;
class user_mask;
class cruiser;
class layer;
class water_color_layer;
struct FixRect;
class IPictureDatabase;
class ITile;
class picture_database;
class cruiser;
class undo_stack_entry;
class FlowParticle;
class OrbitParticle;
class SpringParticle;
class Spring;

struct PainterGlobalContext
{
	void * hWnd;
	void * hInst;
	
	double * pclone_src, * pclone_dest;	// points in picture space; x,y interleaved
	short * pclone_awaiting;
	short * pclone_delta_h;
	short * pclone_delta_v;
	short * pclone_org_h;
	short * pclone_org_v;
	short * pclone_dest_h;
	short * pclone_dest_v;
	short * pmaster_clone_delta_h;
	short * pmaster_clone_delta_v;
	short * pclone_cnt, * pclone_max;
	bool * pclone_following;

	short * pgrain_x_offset;
	short * pgrain_y_offset;
	short * pgrain_ncols;
	short * pgrain_nrows;
	short * pgrain_x_ncols;
	short * pgrain_row_org;
	short * pgrain_col_org;
	float * pgrain_rotation;

	unsigned char *pgGrainMin;
	unsigned char *pgGrainMax;
	unsigned char *pgGrainMedian;

	short * ppenpic_nrows;
	short * ppenpic_ncols;

	short * pn_rad_steps;
	short * pn_ang_steps;
	short * pn_squeeze_steps;

	short * pn_clone_buf_height;
	short * pn_clone_buf_width;
	Rect * pwellbuf_rect;
	
	Pixel * pfore_hsv_color;
	Pixel * pfore_rgb_color;
	Pixel * pback_rgb_color;

	short * pnvector;
	unsigned int * pdraw_extents_timestamp;
	short * pmain_rsrcid;
	short * papplication_rsrcid;

	//these two form prefs_record
	Boolean * ppreserve_transparency;
	Boolean * ppick_up_underlying_color;

	PntrFixed * pcur_radius;
	Pixel * plast_dab_color;
	PntrFixed * pstroke_direc;

	Pixel ** psample_ramp;
	color_modify_proc * pstandard_colormodifyproc; 
	color_modify_proc * pcolormodifyproc;
	notify_proc colormodifyproc_changed;
	store_proc * pstoreproc;

	aa_pen *** ppen_vector;
	unsigned char *** pgrain_body;

	picture ** pclone_source;
	picture ** ptexture_source;
	picture ** ppattern_source;
	picture **pmain_drawing_picture;
	canvas **pmain_drawing_buffer;
	canvas **pmain_buffer;
	mask_entry **pmain_drawing_mask;
	bool *pmain_drawing_undo;
	BrushStateRec * pBSR;
	BrushDefinition * pbdef;
	BrushDefinition * pbuilt_bdef; // a top-level copy of the one that's currently built

	bool * preplaying_stroke;

	PntrFixed * pvpower;
	PntrFixed * pvscale;

	bool * pmulticore_enabled;

	// opencl support
	Boolean         *pOpencl_enabled;
	bool			*p_cl_frisket_needs_update;
};

#if WINVER || __INTEL_COMPILER
#include "immintrin.h"
#endif

#ifndef SIMDH_ENABLED
#define SIMDH_ENABLED MACVER && !__INTEL_COMPILER
#endif
#if SIMDH_ENABLED
#include <simd/simd.h>
#endif

	struct PainterGlobalFunctions {

		MACPASCAL FXErr (*TouchCanvas) (void);
		MACPASCAL FXErr (*TouchRect) (Rect *r, PicObj pic, short layr);
		MACPASCAL FXErr (*GetPictureSize) (PicObj pic, short *pnrows, short *pncols);
		MACPASCAL FXErr (*GetPicturePixel) (PicObj pic, PictureLayerType layr, Pixel *pixel, short x, short y);
		MACPASCAL FXErr (*GetPicturePixelFast) (PicObj pic, PictureLayerType layr, Pixel* pixel, short x, short y, std::vector<ITile*>& tiles);
		MACPASCAL FXErr (*PreloadPictureTiles) (PicObj pic, PictureLayerType layr, const Point& topleft, const Rect& CPR, std::vector<ITile*>& tiles);
		MACPASCAL FXErr (*GetPicturePixelInterp) (PicObj pic, PictureLayerType layr, Pixel *pixel, PntrFixed x, PntrFixed y, std::vector<ITile*>* pTiles);
		MACPASCAL FXErr (*GetPicturePixelInterpTransColorReplace) (PicObj pic, PictureLayerType layr, Pixel *pixel, PntrFixed x, PntrFixed y, Pixel tranparentColorReplacement, std::vector<ITile*>* pTiles);
		MACPASCAL FXErr (*GetPictureRow) (PicObj pic, PictureLayerType layr, short row, short col, short width, Pixel *buffer);
		MACPASCAL FXErr (*PutPictureRow) (PicObj pic, PictureLayerType layr, short row, short col, short width, Pixel *buffer);
		void (*FcMaxMaskFlat)( Pixel *dst, Pixel *src, short width );
		void (*FcMaxMaskNorm)( Pixel *dst, Pixel *src, short width );
		void (*FcMaxMaskInv) ( Pixel *dst, Pixel *src, short width );
		void (*FcOpacityMaskFlat)( Pixel *dst, Pixel *src, short width );
		void (*FcOpacityMaskNorm)( Pixel *dst, Pixel *src, short width );
		void (*FcOpacityMaskInv)( Pixel *dst, Pixel *src, short width );

		Pixel (*LoadPixelLayer)(picture *p, PictureLayerType type, short x, short y);
		Pixel (*LoadPixel)(picture *p, short x, short y);
		Pixel (*LoadAveragedPixel)(picture *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelAlpha)(picture *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelAverageAlpha)(picture *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelWithMaxAlpha)(picture *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelAlphaPickupMinAlpha)(picture *p, short x, short y, PntrFixed radius, float& minAlpha);
		Pixel (*LoadAveragedPixelDepth)(picture *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelComposited)(canvas *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelCompositedAverageAlpha)(canvas *p, short x, short y, PntrFixed radius);
		Pixel (*LoadAveragedPixelCompositedWithMaxAlpha)(canvas *p, short x, short y, PntrFixed radius);
		Pixel (*LoadPixelMask)(picture *p, user_mask *um, const mask_type& type, short x, short y);
		Pixel (*LoadPixelComposited)(canvas *p, short x, short y, bool onlySelectedLayers /*false*/);
		Pixel (*LoadPixelCompositedBelowFloater)(canvas *p, cruiser *top, short x, short y);
		Pixel (*LoadPixelCompositedBelowFloaterIncluded)(canvas *p, cruiser *top, short x, short y);
		Pixel (*LoadPixelCompositedBelowFloaterAlpha)(canvas *p, cruiser *top, short x, short y);
		Pixel (*LoadMedianDyePixel)(picture *p, PictureLayerType type, short x, short y, PntrFixed radius);
		void (*PixelCombine)(layer *p, Pixel *dst, Pixel *src, bool bIgnoreViewmask /*false*/);
		void (*PixelCombineAlpha)(layer *p, Pixel *dst, Pixel *src, bool bIgnoreViewmask /*false*/);
		void (*SavePixelLayer)(picture *p, PictureLayerType type, short x, short y, Pixel color);
		
		Pixel (*PbGetTmap)(picture *p, PntrFixed dx, PntrFixed dy);
		Pixel (*PbGetTmapAlpha)(picture *p, PntrFixed dx, PntrFixed dy);
		Pixel (*PbGetTmapComposited)(canvas *p, PntrFixed x, PntrFixed y, bool alpha);
		Pixel (*PbGetTmapCompositedBelowFloater)(canvas *p, cruiser *top, PntrFixed x, PntrFixed y);
		Pixel (*PbGetTmapCompositedBelowFloaterIncluded)(canvas *p, cruiser *top, PntrFixed x, PntrFixed y, std::vector<ITile*>* pTiles);
		Pixel (*PbGetTmapDd)(picture *p, PntrFixed x, PntrFixed y);
		Pixel (*PbGetCubicTmapAlpha)(picture *p, PntrFixed dx, PntrFixed dy);
		void (*PbGetCompositedBelowFloaterForPUUC)(canvas *p, cruiser *top, short row, short col, short length, Pixel *buffer);
		void (*PbGetTmapNeighborhood)(picture *p, PictureLayerType type, PntrFixed dx, PntrFixed dy,
															Pixel scanline1[2], Pixel scanline2[2], unsigned short *pxfrac, unsigned short *pyfrac,
															std::vector<ITile*>* pTiles,
															FixRect *bounds, PntrFixed half_drop_offset);
		void (*PbGetComposited)(canvas *p, short row, short col, short length, Pixel *buffer, bool printing, bool ignoreCanvas, bool ignoreSelection);
		short (*GetOrgX)(cruiser* pCr);
		short (*GetOrgY)(cruiser* pCr);
		void (*SetArrowCursor)();
		void (*SetWaitCursor)();
		void (*SetFlowParticlesGhost)(unsigned int particlesCount, const FlowParticle* particles);
		void (*SetOrbitParticlesGhost)(unsigned int particlesCount, const OrbitParticle* particles);
		void (*SetSpringParticlesGhost)(const std::vector<SpringParticle*>* particles, const std::vector<Spring*>* springs);

		void (*InvalPictureRect)(canvas *p, Rect *r);
		void (*ConvertToGlobal)(canvas *p, Point *pt);
		void (*SaveForUndo)(canvas *p, Rect *r, short opcode);
		void (*SaveFloatingForUndo)(canvas *p, Rect *r, short opcode);
		void (*DoneWithUndo)();
		void (*RecDry)();

		void (*AddCloneDestCp)(Point loc);
		void (*CompositeStart)();
		bool (*SetDrawingBuffer)();
		void (*TestUpdateRamp)();
		short (*GetMethodsCategory)();
		Pixel (*InterpolateNeighborhood)(Pixel *src1, Pixel *src2, unsigned short xfrac, unsigned short yfrac);
		bool (*WkFraction)(float frac, bool abortable /*= true*/);
		void (*WkStart)(const std::wstring& str);
		void (*WkDone)();
		void (*TryReuseDabFromCurrentVariant)();
		void (*FloatCombineSetup)(layer *p, bool b /*=false*/, bool bIgnoreViewmask /*false*/);
		void (*StartUndoStrokeStart)();
		short (*StopAlert)(short message_number);
		void (*GrainGet)(short row, short col, short width, unsigned char *buffer, bool highQuality);
		void (*FlowMapGet)(short row, short col, short width, unsigned char *buffer, bool highQuality);
		void (*TextureGet)(short row, short col, short width, unsigned char *buffer, bool highQuality);
		bool (*DirectionalGrain)();
		bool (*TouchTile)(IPictureDatabase* pdb, short x_index, short y_index, 
								   bool operation_undo, IPictureDatabase* updb);
		void (*Error)(const char *str);
		bool (*PainterDrawPicture)(canvas *p, Rect *r);
		void (*PrintableColorCheck)(Pixel *ppix);
		bool (*IsSegmentedStrokes)();

		void (*InvalidateMixerWells)();																// Invalidates the mixer's wells (both the flat and 2D wells)
		bool (*GetMixerBrushWells)(uint32*, uint32*, bool&);					// Load the "artist's oils" wells from the mixer flat wells
		void (*CopyToMixerBrushWells)(uint32*, bool);									// Transfer "artist's oils" wells over to mixer flat wells
		void (*CopyTo2DMixerBrushWells)(uint32*);
		bool (*ChargeWellFromMixer)();																// Check if we can/should load 2D wells from mixer (used by non artists' oils)
 		Pixel (*MixerWellColor)(const real32 rUX, const real32 rUY);	// Retrieves the color in the mixer's 2D wells at specified position
						
		bool (*RipMemoryPrep)(unsigned long long amount);
		void (*FinishBatchDraw)();
		void (*OutputPointUpdate)(Rect *r, short ntimes, bool bForceUpdate);
		double (*GetMappedTiltHelper)( double dTilt );

		const float& (*ValueForMaxBrushRadius)();

		// Liquid Ink helpers
		float* (*getLIRowVal1)();
		float* (*getLIRowVal2)();
		Pixel* (*getLIScanBuf)();
		int (*getLIRowSize)();
		float* (*getLISoftenBuf)();
		int (*getLISoftenBufSize)();

		// ThickPaint helpers
		float* (*getTPRowVal1)();
		float* (*getTPRowVal2)();
		Pixel* (*getTPScanBuf)();
		int(*getTPRowSize)();
		char* (*getTPGrainBuf)();
		float* (*getTPDepthAreaBuf)();

		undo_stack_entry* (*get_latest_undo)(canvas *p);
		undo_stack_entry* (*get_latest_redo)(canvas *p);

		bool(*getRandomColorFromCurrentColorSet)(Pixel& outPixel, PntrFixed fixedNoise);
		bool (*isDoingStrokePreview)();

		bool (*UseJitter)();

		bool (*UsingTemporalPUUCLayer)();
		void (*SelectTemporalPUUCLayer)();
		void (*SelectTemporalBuddyLayer)();

		bool (*IsPuucAllowedForPluginBrush)(unsigned char *brushIdentifier);
		bool (*DoesCloneSourceContainsAlphaChannel)();
		void (*PurgePainterBrushCacheIfNeeded)(uint64_t neededSpace);
		bool (*IsBrushInPainterBrushCache)(aa_pen** pen_vector);
		PntrFixed (*GetScaleFactor)();
		cl_context (*GetOpenCLContext)();
		cl_command_queue (*GetOpenCLCommandQueue)();
		bool (*IsAMDPlatform)();
		const Corel::Painter::OpenCL::BrushKernelsData& (*GetOpenCLBrushKernels)();
		bool (*OpenCLBrushKernelsReadyForUse)();
		bool (*GetMultiPointCloningEnabled)();
		bool (*GetTintCloningEnabled)();
		bool (*IsCurrentBrushThickPaintCompatible)();
		bool (*IsCurrentBrushWaterColorCompatible)();
		bool (*IsCurrentPluginBrushOfType)(unsigned char* brushIdentifier);
		void (*SetLBPluginsProcs)(bool avx2);
		bool (*IsUsingAlphaWells)();

#if (_WIN64 || __INTEL_COMPILER)
		MACPASCAL FXErr(*GetPicturePixelFast_AVX2) (PicObj pic, IPictureDatabase* pd, __m256i& vpixel, __m256i vx, __m256i vy, std::vector<ITile*>& tiles);
		MACPASCAL FXErr(*GetPicturePixelInterp_picture_AVX2) (PicObj pic, IPictureDatabase* pd, __m256i& vpixel, __m256i vxd, __m256i vyd, std::vector<ITile*>* pTiles);
		MACPASCAL FXErr(*GetPicturePixelInterpTransColorReplace_picture_AVX2) (PicObj pic, IPictureDatabase* pd, __m256i& vpixel, __m256i vxd, __m256i vyd, Pixel transparentReplacement, std::vector<ITile*>* pTiles);
		MACPASCAL FXErr(*GetPicturePixelInterp_depth_AVX2) (PicObj pic, __m256i& vpixel, __m256i vxd, __m256i vyd, std::vector<ITile*>* pTiles);
		__m256i (*PbGetTmapCompositedBelowFloaterIncluded_AVX2)(canvas* p, cruiser* top, __m256i vx, __m256i vy, std::vector<ITile*>* pTiles);
		__m256i (*LoadPixelCompositedBelowFloaterIncluded_AVX2)(canvas* p, cruiser* top, __m256i vx, __m256i vy);
#else
		MACPASCAL FXErr(*GetPicturePixelFast_AVX2) (/*dummy*/);
		MACPASCAL FXErr(*GetPicturePixelInterp_picture_AVX2) (/*dummy*/);
		MACPASCAL FXErr(*GetPicturePixelInterpTransColorReplace_picture_AVX2) (/*dummy*/);
		MACPASCAL FXErr(*GetPicturePixelInterp_depth_AVX2) (/*dummy*/);
		void (*PbGetTmapCompositedBelowFloaterIncluded_AVX2)(/*dummy*/);
		void (*LoadPixelCompositedBelowFloaterIncluded_AVX2)(/*dummy*/);
#endif

#if SIMDH_ENABLED
		MACPASCAL FXErr(*GetPicturePixelFast_SIMDH) (PicObj pic, IPictureDatabase* pd, simd_int8& vpixel, simd_int8 vx, simd_int8 vy, std::vector<ITile*>& tiles);
		MACPASCAL FXErr(*GetPicturePixelInterp_picture_SIMDH) (PicObj pic, IPictureDatabase* pd, simd_int8& vpixel, simd_int8 vxd, simd_int8 vyd, std::vector<ITile*>* pTiles);
		MACPASCAL FXErr(*GetPicturePixelInterpTransColorReplace_picture_SIMDH) (PicObj pic, IPictureDatabase* pd, simd_int8& vpixel, simd_int8 vxd, simd_int8 vyd, Pixel transparentReplacement, std::vector<ITile*>* pTiles);
		simd_int8 (*PbGetTmapCompositedBelowFloaterIncluded_SIMDH)(canvas* p, cruiser* top, simd_int8 vx, simd_int8 vy, std::vector<ITile*>* pTiles);
		simd_int8 (*LoadPixelCompositedBelowFloaterIncluded_SIMDH)(canvas* p, cruiser* top, simd_int8 vx, simd_int8 vy);
#else
		MACPASCAL FXErr(*GetPicturePixelFast_SIMDH) (/*dummy*/);
		MACPASCAL FXErr(*GetPicturePixelInterp_picture_SIMDH) (/*dummy*/);
		MACPASCAL FXErr(*GetPicturePixelInterpTransColorReplace_picture_SIMDH) (/*dummy*/);
		void (*PbGetTmapCompositedBelowFloaterIncluded_SIMDH)(/*dummy*/);
		void (*LoadPixelCompositedBelowFloaterIncluded_SIMDH)(/*dummy*/);
#endif

#if UNIT_TESTS == 1
		bool (*GetRunRegressionTests)();
		bool (*GetRunSpeedTests)();
		bool (*GetRunOriginalTests)();
#endif
	};

BE_ENGINE_SIDE void BEInit(PainterGlobalContext & pPainterGlobals, PainterGlobalFunctions & pFunc);
BE_ENGINE_SIDE void BEOpenCLContextCreated();
BE_ENGINE_SIDE void BEOpenCLContextAboutToDestroy();
BE_ENGINE_SIDE bool &BEOpenCLShouldRun();
BE_ENGINE_SIDE void BESetVelocityThresholds(float gpuAreaLayer, float gpuAreaCanvas, float cpuArea);
