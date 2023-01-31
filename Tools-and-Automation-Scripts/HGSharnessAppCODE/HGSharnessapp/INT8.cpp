#include "INT8.h"


//int8 opcode for vpdpbusd %xmm0, %xmm1, %xmm2 : c4 e2 71 50 d0
void runINT8(long duration) {
	bool runForever = (duration == -1);
	DWORD64 initialTicks = __rdtsc();
	long p1_freqL = (long)p1_freq;//for less assembely
	unsigned __int64 goal = ((unsigned __int64)duration) * p1_freqL;
	goal += initialTicks;


	while (runForever || __rdtsc() < goal) {

		__asm {
			_emit 0xc4
			_emit 0xe2
			_emit 0x71
			_emit 0x50
			_emit 0xd0
		}

	}
}





/*__m128i dest, src, a, b;
a = _mm_setr_epi32(1,2,3,4);
b = _mm_setr_epi32(5, 6, 7, 8);*/

/*_mm_dpbusd_epi32(src, a, b);
_mm_dpbusd_epi32(src, b, a);
_mm_dpbusd_epi32(a, src, b);
_mm_dpbusd_epi32(b, a, src);
_mm_dpbusd_epi32(a, src, b);
_mm_dpbusd_epi32(src, a, b);*/