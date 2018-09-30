#include "ftime.h"

#include <time.h>
#if defined WIN32 || defined _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#pragma comment (lib, "winmm.lib")

// thanks to kalcor
CORETYPE tf::time::milliseconds::get()
{
	CORETYPE value;

#if defined __WIN32__ || defined _WIN32 || defined WIN32
	value = timeGetTime();        /* this value is already in milliseconds */
#elif defined __linux || defined __linux__ || defined __LINUX__ || defined __APPLE__
	struct timeval tv;
	gettimeofday(&tv, 0);
	value = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#else
	value = clock();
#if CLOCKS_PER_SEC<1000
	/* convert to milliseconds */
	value = (cell)((1000L * value) / CLOCKS_PER_SEC);
#elif CLOCKS_PER_SEC>1000
	/* convert to milliseconds */
	value = (cell)(value / (CLOCKS_PER_SEC / 1000));
#endif
#endif
	return value;
}