
#if 1 //(_WIN64 || __INTEL_COMPILER) && ARCH_X86_64

#pragma warning( push )
#pragma warning( disable : 4752 ) //found Intel(R) Advanced Vector Extensions; consider using /arch:AVX

namespace
{
	void determineMagnitudeAndSign(int& magnitude, int& sign, const int& direction, const int& width);
	inline void rangeIt(int& x, int max);
}

#include "Gatherton.h"
#include <immintrin.h>
#include <algorithm>

//extern PainterGlobalContext glbPainter;
//extern short clone_buf_depth_org_width;
//extern Pixel* wellbuf2_depth[max_strokes];
const float smoothing = 1.0f;
const short clone_buf_depth_org_width = 1004;
extern Pixel* wellbuf2_depth[24];

#define fixfactor			(65536.0)		/* 2.0**16 */
#define float2fixed(f)  ((int)((f) * fixfactor + 0.5))

void process_depth_well_smoothing_ST(int stroke_id, Pixel* pp, int rows, int cols, int dy, int dx)
{
	_mm256_zeroupper();

	int magx, magy, signx, signy;
	unsigned int factor, omfactor;
	const int ymax10 = (rows - 1) * clone_buf_depth_org_width;

	// do nothing if no motion or smppthing
	if ((dy == 0 && dx == 0) || (smoothing < 0.001f))
	{
		return;
	}
	// determines magnitudes and signs
	determineMagnitudeAndSign(magx, signx, dx, 1);
	determineMagnitudeAndSign(magy, signy, dy, clone_buf_depth_org_width);

	// motion blur each pixel in turn
	const int cnt = magx + magy;
	Pixel* qq = wellbuf2_depth[stroke_id];

	const int cols8 = cols & ~7L;
	const __m256i viZero = _mm256_setzero_si256();
	const __m256i viFFFF = _mm256_set1_epi32(0xFFFF);
	const __m256i vi8000 = _mm256_set1_epi32(0x8000);
	const __m256i viF4Z4 = _mm256_set1_epi32(0xFFFF0000);
	const __m256i viInit = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);

	const __m256i viCols = _mm256_set1_epi32(cols - 1);
	const __m256i viYmax10 = _mm256_set1_epi32(ymax10);
	const __m256 vfCountInv = _mm256_set1_ps(1.0f / cnt);
	const __m256i vStoreMask = _mm256_cmpgt_epi32(_mm256_set1_epi32(cols - cols8), viInit);

	const bool isXgeY = (magx >= magy);
	const __m256i viBrezInit = _mm256_set1_epi32(std::min(-((isXgeY ? magx : magy) >> 1), -1));
	const __m256i viMag = _mm256_set1_epi32(isXgeY ? magy : magx);
	const __m256i viMag_ = _mm256_set1_epi32(isXgeY ? -magx : -magy);
	const __m256i viSignX = _mm256_set1_epi32(isXgeY ? signx : 0);
	const __m256i viSignY = _mm256_set1_epi32(isXgeY ? signy : 0);
	const __m256i viSignX_ = _mm256_set1_epi32(isXgeY ? 0 : signx);
	const __m256i viSignY_ = _mm256_set1_epi32(isXgeY ? 0 : signy);

	// pass well again interpolating in blurred value
	factor = std::min(float2fixed(smoothing * 0.5f), 0x0000FFFF);
	omfactor = 0xFFFF - factor;
	const __m256i vfactor = _mm256_set1_epi32(factor);
	const __m256i vomfactor = _mm256_set1_epi32(omfactor);

//#pragma omp parallel
	{
//#pragma omp for
		for (int iy = 0; iy < rows; iy++)
		{
			Pixel* qq1 = qq + iy * cols;
			const __m256i viY0 = _mm256_set1_epi32(iy * clone_buf_depth_org_width);

			for (int ix = 0; ix < cols; ix += 8, qq1 += 8)
			{
				__m256i viZsum = viZero;
				__m256i viX = _mm256_add_epi32(_mm256_set1_epi32(ix), viInit);
				__m256i viY = viY0;
				__m256i viBrez = viBrezInit;
				//__m256i temp1, temp2;

				for (int j = 0; j < cnt; j++)
				{
					const __m256i viMask = _mm256_cmpgt_epi32(viZero, viBrez);
					viX = _mm256_add_epi32(viX, _mm256_blendv_epi8(viSignX_, viSignX, viMask));
					viY = _mm256_add_epi32(viY, _mm256_blendv_epi8(viSignY, viSignY_, viMask));
					viX = _mm256_max_epi32(_mm256_min_epi32(viX, viCols), viZero);

					viY = _mm256_max_epi32(_mm256_min_epi32(viY, viYmax10), viZero);
					viBrez = _mm256_add_epi32(viBrez, _mm256_blendv_epi8(viMag_, viMag, viMask));

					//temp1 = _mm256_add_epi32(viX, viY);
					//temp2 = _mm256_i32gather_epi32((const int*)pp, temp1, 4);
					//viZsum = _mm256_add_epi32(viZsum, _mm256_and_si256(temp2, viFFFF));
					viZsum = _mm256_add_epi32(viZsum, _mm256_and_si256(_mm256_i32gather_epi32((const int*)pp, _mm256_add_epi32(viX, viY), 4), viFFFF));
				}

				__m256i viResult = _mm256_cvtps_epi32(_mm256_mul_ps(_mm256_cvtepi32_ps(viZsum), vfCountInv));
				if (ix < cols8)
					_mm256_storeu_si256((__m256i*)qq1, viResult);
				else
					_mm256_maskstore_epi32((int*)qq1, vStoreMask, viResult);
			}
		}

		//ensure data *pp has been used up before update
//#pragma omp barrier
//#pragma omp for
		for (int iy = 0; iy < rows; iy++)
		{
			Pixel* pp1 = pp + iy * clone_buf_depth_org_width;
			Pixel* qq1 = qq + iy * cols;

			int ix = 0;
			for (; ix < (cols - 7); ix += 8, pp1 += 8, qq1 += 8)
			{
				// interpolate
				//int zsum1 = ((*pp1 & 0x0000FFFF) * omfactor + *qq * factor + 0x8000) >> 16;
				__m256i vpp1 = _mm256_loadu_si256((const __m256i*)pp1);
				__m256i vpp = _mm256_mullo_epi32(_mm256_and_si256(vpp1, viFFFF), vomfactor);
				__m256i vqq = _mm256_mullo_epi32(_mm256_loadu_si256((const __m256i*)qq1), vfactor);
				__m256i vzsum1 = _mm256_srli_epi32(_mm256_add_epi32(_mm256_add_epi32(vpp, vqq), vi8000), 16);

				// store interpolated result
				//*pp1 = (*pp1 & 0xFFFF0000) | zsum1;
				_mm256_storeu_si256((__m256i*)pp1, _mm256_or_si256(_mm256_and_si256(vpp1, viF4Z4), vzsum1));
			}

			for (; ix < cols; ix++, pp1++, qq1++)
			{
				// interpolate
				Pixel vpp1 = *pp1;
				int zsum1 = ((vpp1 & 0x0000FFFF) * omfactor + *qq1 * factor + 0x8000) >> 16;

				// store interpolated result
				*pp1 = (vpp1 & 0xFFFF0000) | zsum1;
			}
		}
	}

	_mm256_zeroupper();
}

namespace
{
	void determineMagnitudeAndSign(int& magnitude, int& sign, const int& direction, const int& width)
	{
		if (direction < 0)
		{
			magnitude = -direction;
			sign = -width;
		}
		else if (direction > 0)
		{
			magnitude = direction;
			sign = width;
		}
		else
		{
			magnitude = sign = 0;
		}
	}

	// smooth the 2-D depth well using simplistic motion blur, wellbuf2_depth is used as scratch buffer
	inline void rangeIt(int& x, int max)
	{
		if (x < 0)
		{
			x = 0;
		}
		else if (x > max)
		{
			x = max;
		}
	}
}
#else
// in case AVX is enabled and it's Mac, will be implemented later
void process_depth_well_smoothing_AVX(int stroke_id, Pixel* pp, int rows, int cols, int dy, int dx)
{ }
#endif
