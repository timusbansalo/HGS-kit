#pragma once
#include <iostream>
#include <immintrin.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <winreg.h>
#include <winerror.h>

#include "Class0.h"
#include "Class1.h"
#include "VNNI.h"
#include "INT8.h"
#include "PAUSE.h"
#include "UserMWait.h"
#include "HarnessSleep.h"

#define CLASS_MAX 6 
#define CLASS_MIN 0

extern bool printRunningScenario;
extern unsigned int p1_freq;


enum Index { //index in functon pointers
	CLASS0 = 0, 
	CLASS1 = 1, // combination - AVX2 and ...
	CLASS2 = 2, // we disabled avx and using vnni instead
	CLASS3 = 3, // default: PAUSE

	VNNI = 2, // same as class2 
	INT8_FUNC = 5, // Not working right
	PAUSE = 3, 
	USERMWAIT = 4,

	HARNESSSLEEP = 6
};

void initProgram(int, char**);

