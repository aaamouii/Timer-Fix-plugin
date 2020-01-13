// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "COSTime.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

uint64_t COSTime::Current()
{
	uint64_t time = 0;
#ifdef _WIN32
	LARGE_INTEGER liCount;
	LARGE_INTEGER liFrec;

	QueryPerformanceCounter(&liCount);
	QueryPerformanceFrequency(&liFrec);

	uint64_t quotient, remainder;
	quotient = ((liCount.QuadPart) / liFrec.QuadPart);
	remainder = ((liCount.QuadPart) % liFrec.QuadPart);
	time = quotient * 1000000 + (remainder * 1000000 / liFrec.QuadPart);
#else
	timeval timeVal;
	gettimeofday(&timeVal, 0);
	time = static_cast<uint64_t>((timeVal.tv_sec) * 1000000 + (timeVal.tv_usec));
#endif
	return (time / 1000);
}