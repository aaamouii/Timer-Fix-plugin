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