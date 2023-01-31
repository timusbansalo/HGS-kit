#include "Class1.h"

void runClass1(long duration) {
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


/*
void runClass1(long duration) {
												//israel: radio is not caltulated this way(must consider op time),
												//hisham said its okay to keep it like this for now
	if (duration == -1) {//run forever
		asm(
			"1:vfmaddsub132ps %ymm0  , %ymm0  , %ymm0;"
			"vfmaddsub213ps %ymm0  , %ymm0  , %ymm0;"
			"vfmaddsub231ps %ymm0  , %ymm0  , %ymm0;"
			"xor %rax,%rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"inc %rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"add $5,%rax;"
			"inc %rax;"
			"inc %rax;"
			"inc %rax;"
			"mov %rax,%rax;"
			"inc %rax;"
			"mov %rax,%rax;"
			"inc %rax;"
			"mov %rax,%rax;"
			"jmp 1b;"
		); //1b means go to label named 1, in backwad direction
	}
	else {//timed
		DWORD64 initialTicks = __rdtsc();
		long tmp = (long)p1_freq;//for less assembely
		while (((__rdtsc() - initialTicks) / tmp) < duration) {
			asm(
				"vfmaddsub132ps %ymm0  , %ymm0  , %ymm0;"
				"vfmaddsub213ps %ymm0  , %ymm0  , %ymm0;"
				"vfmaddsub231ps %ymm0  , %ymm0  , %ymm0;"
				"xor %rax,%rax;"
				"add $5,%rax;"
				"inc %rax;"
				"add $5,%rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"add $5,%rax;"
				"inc %rax;"
				"add $5,%rax;"
				"inc %rax;"
				"inc %rax;"
				"inc %rax;"
				"mov %rax,%rax;"
				"inc %rax;"
				"mov %rax,%rax;"
				"inc %rax;"
				"mov %rax,%rax;"
			);
		}
	}
}

*/