#include "VNNI.h"



//this code is for adl, tested , working alright
void runVNNI(long duration) {
	bool runForever = (duration == -1);
	DWORD64 initialTicks = __rdtsc();
	long p1_freqL = (long)p1_freq;//for less assembely
	unsigned __int64 goal = ((unsigned __int64)duration) * p1_freqL;
	goal += initialTicks;
	while (runForever || __rdtsc() < goal) {
		__asm {
			_emit 0xc4
			_emit 0xe2
			_emit 0x7d
			_emit 0x50
			_emit 0xd1
			//vpdpbusd ...
			_emit 0xc4
			_emit 0xe2
			_emit 0x7d
			_emit 0x50
			_emit 0xd9

			_emit 0xc4
			_emit 0xe2
			_emit 0x7d
			_emit 0x50
			_emit 0xe1

			_emit 0xc4
			_emit 0xe2
			_emit 0x7d
			_emit 0x50
			_emit 0xe9


			_emit 0xc4
			_emit 0xe2
			_emit 0x7d
			_emit 0x50
			_emit 0xf1

			_emit 0xc4
			_emit 0xe2
			_emit 0x7d
			_emit 0x50
			_emit 0xf9

			_emit 0xc4
			_emit 0x62
			_emit 0x7d
			_emit 0x50
			_emit 0xc1

			_emit 0xc4
			_emit 0x62
			_emit 0x7d
			_emit 0x50
			_emit 0xc9


			_emit 0xc4
			_emit 0x62
			_emit 0x7d
			_emit 0x50
			_emit 0xd1


			_emit 0xc4
			_emit 0x62
			_emit 0x7d
			_emit 0x50
			_emit 0xd9


			_emit 0xc4
			_emit 0x62
			_emit 0x7d
			_emit 0x50
			_emit 0xe1

			_emit 0xc4
			_emit 0x62
			_emit 0x7d
			_emit 0x50
			_emit 0xe9
		}
	}
}





void runClass2AVX(long duration) {
	bool runForever = (duration == -1);
	DWORD64 initialTicks = __rdtsc();
	long p1_freqL = (long)p1_freq;//for less assembely
	unsigned __int64 goal = ((unsigned __int64)duration) * p1_freqL;
	goal += initialTicks;


	while (runForever || __rdtsc() < goal) {
		asm(
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm2;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm3;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm4;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm5;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm6;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm7;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm8;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm9;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm10;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm2;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm3;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm4;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm5;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm6;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm7;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm8;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm9;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm10;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm2;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm3;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm4;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm5;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm6;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm7;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm8;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm9;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm5;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm6;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm7;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm8;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm9;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm5;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm6;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm7;"
			"vfmaddsub132ps %ymm0  , %ymm1  , %ymm8;"
			"vfmaddsub213ps %ymm0  , %ymm1  , %ymm9;"
			"vfmaddsub231ps %ymm0  , %ymm1  , %ymm10;"
		);

	}


}





//VNNI in opcpde: got it from hisham
/*__asm {
	//l1:

	_emit 0xc4
	_emit 0xe2
	_emit 0x7d
	_emit 0x50
	_emit 0xd1

	_emit 0xc4
	_emit 0xe2
	_emit 0x7d
	_emit 0x50
	_emit 0xd9

	_emit 0xc4
	_emit 0xe2
	_emit 0x7d
	_emit 0x50
	_emit 0xe1

	_emit 0xc4
	_emit 0xe2
	_emit 0x7d
	_emit 0x50
	_emit 0xe9


	_emit 0xc4
	_emit 0xe2
	_emit 0x7d
	_emit 0x50
	_emit 0xf1

	_emit 0xc4
	_emit 0xe2
	_emit 0x7d
	_emit 0x50
	_emit 0xf9

	_emit 0xc4
	_emit 0x62
	_emit 0x7d
	_emit 0x50
	_emit 0xc1

	_emit 0xc4
	_emit 0x62
	_emit 0x7d
	_emit 0x50
	_emit 0xc9


	_emit 0xc4
	_emit 0x62
	_emit 0x7d
	_emit 0x50
	_emit 0xd1


	_emit 0xc4
	_emit 0x62
	_emit 0x7d
	_emit 0x50
	_emit 0xd9


	_emit 0xc4
	_emit 0x62
	_emit 0x7d
	_emit 0x50
	_emit 0xe1

	_emit 0xc4
	_emit 0x62
	_emit 0x7d
	_emit 0x50
	_emit 0xe9
	//jmp l1
}*/



//VNNI512 
/*
void runVNNI(long duration) {
		bool runForever = (duration == -1);
		DWORD64 initialTicks = __rdtsc();
		long p1_freqL = (long)p1_freq;//for less assembely
		unsigned __int64 goal = ((unsigned __int64)duration) * p1_freqL;
		goal += initialTicks;

		__m512i a = _mm512_set_epi32(0x3456789A, 0x56789ABC, 0x789ABCDE, 0x9ABCDEF0, 0xABCDEFAB, 0xCDEFABCD, 0xEFABCDEF, 0x12345678, 0x3456789A, 0x56789ABC, 0x789ABCDE, 0x9ABCDEF0, 0xABCDEFAB, 0xCDEFABCD, 0xEFABCDEF, 0x12345678);
		__m512i b = _mm512_set_epi32(0xABCDEFAB, 0xCDEFABCD, 0xEFABCDEF, 0x12345678, 0x3456789A, 0x56789ABC, 0x789ABCDE, 0x9ABCDEF0, 0xABCDEFAB, 0xCDEFABCD, 0xEFABCDEF, 0x12345678, 0x3456789A, 0x56789ABC, 0x789ABCDE, 0x9ABCDEF0);
		__m512i src = _mm512_set_epi32(0x9ABCDEF0, 0xABCDEFAB, 0xCDEFABCD, 0xEFABCDEF, 0x12345678, 0x3456789A, 0x56789ABC, 0x789ABCDE, 0x9ABCDEF0, 0xABCDEFAB, 0xCDEFABCD, 0xEFABCDEF, 0x12345678, 0x3456789A, 0x56789ABC, 0x789ABCDE);
		__m512i dst1, dst2, dst3;


		while (runForever || __rdtsc() < goal) {
			//VNNI 512 register:
			  dst1 = _mm512_dpbusd_epi32(src, a, b);
			//dst2 = _mm512_dpbusd_epi32(src, a, a);
			//dst3 = _mm512_dpbusd_epi32(src, b, b);
		}

		std::cout	  << _mm256_extract_epi32(_mm512_extracti32x8_epi32(dst1, 0), 0);
					//<< _mm256_extract_epi32(_mm512_extracti32x8_epi32(dst2, 0), 0)
					//<< _mm256_extract_epi32(_mm512_extracti32x8_epi32(dst3, 0), 0);
}
*/
