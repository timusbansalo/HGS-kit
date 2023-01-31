#include "PAUSE.h"

void runClass3(long duration) {/////////////////////////ask about this
	bool runForever = (duration == -1);
	DWORD64 initialTicks = __rdtsc();
	long p1_freqL = (long)p1_freq;//for less assembely
	unsigned __int64 goal = ((unsigned __int64)duration) * p1_freqL;
	goal += initialTicks;

	while (runForever || __rdtsc() < goal) {
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
		asm("PAUSE;");//spinlock - hint
	}
}
