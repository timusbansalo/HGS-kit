#include "UserMWait.h"


void runUserMWait(long duration) {

	bool runForever = (duration == -1);
	DWORD64 initialTicks = __rdtsc();
	long p1_freqL = (long)p1_freq;//for less assembely
	unsigned __int64 goal = ((unsigned __int64)duration) * p1_freqL;
	goal += initialTicks;

	unsigned __int64 mon = 0, tmp;
	while (runForever || __rdtsc() < goal) {
		tmp = goal;// only changed this line
		_umonitor(&mon);
		_umwait(0, tmp);
	}

}


//from hisham
/*
unsigned __int64 mon = 0, tmp;
std::cout << "User level mwait\n";
while (1) {
	tmp = _rdtsc() + WAIT_TIME;
	_umonitor(&mon);
	_umwait(0, tmp);
}
*/