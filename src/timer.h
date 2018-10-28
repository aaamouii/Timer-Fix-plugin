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
#ifndef TIMER_H_
#define TIMER_H_

/// Internal includes
/// ----------------------------
#include "main.h"
/// ----------------------------

/// External includes
/// ----------------------------
#include <deque>
#include <chrono>
#include <iostream>
/// ----------------------------

typedef struct
{
	std::deque<std::pair<cell *, cell>> arrays;
	std::deque<std::string> strings;
	std::deque<int> integers;
} params_type;
typedef struct
{
	std::string callback_name;
	int interval;
	bool repeat;
	bool is_destroyed;
	params_type params;
	std::chrono::high_resolution_clock::time_point entry_point;
} timer_type;

class Timer
{
public:

	Timer();
	~Timer();

	int New(const char *callback, int interval, bool repeat);
	int NewEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset);
	int Kill(int timerid);
	void KillAll();
	int IsValid(int timerid);
	int SetInterval(int timerid, int interval);
	int GetInterval(int timerid);
	int GetRemaining(int timerid);

	void Process(AMX *amx);
};

#endif