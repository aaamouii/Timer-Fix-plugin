/*
	MIT License

	Copyright (c) 2018 Kash Cherry

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#include "gettime.h"

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/time.h>
  #include <unistd.h>
#endif

Time GetTime()
{
#ifdef _WIN32
	Time curTime;
	LARGE_INTEGER PerfVal;
	LARGE_INTEGER yo1;

	QueryPerformanceFrequency(&yo1);
	QueryPerformanceCounter(&PerfVal);

	__int64 quotient, remainder;
	quotient = ((PerfVal.QuadPart) / yo1.QuadPart);
	remainder = ((PerfVal.QuadPart) % yo1.QuadPart);
	curTime = (Time) quotient*(Time)1000000 + (remainder*(Time)1000000 / yo1.QuadPart);
	return (curTime / 1000);
#else
	timeval tp;
	Time curTime;
	gettimeofday(&tp, 0);

	curTime = (tp.tv_sec) * (Time) 1000000 + (tp.tv_usec);
	return (curTime / 1000);
#endif
}