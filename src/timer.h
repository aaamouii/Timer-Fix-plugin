#pragma once

#include "main.h"
#include "ftime.h"

#include <iostream>
#include <unordered_map>
#include <deque>

namespace tf {

	typedef struct
	{
		std::deque<std::pair<cell *, cell>> arrays;
		std::deque<std::string> strings;
		std::deque<int> integers;
	} params_type;
	typedef struct
	{
		AMX *amx;
		std::string callback_name;
		CORETYPE interval;
		bool repeat;
		bool is_destroyed;
		params_type params;
		CORETYPE entry_point;
	} timer_type;

	class Timer {
	public:
		unsigned short New(AMX *amx, const char *callback, CORETYPE interval, bool repeat);
		unsigned short NewEx(AMX *amx, const char *callback, CORETYPE interval, bool repeat, cell *params, const char *format, int offset);
		int Kill(unsigned short id);
		void KillAll(AMX *amx);
		int IsValid(unsigned short timerid);
		int SetInterval(unsigned short timerid, CORETYPE interval);
		CORETYPE GetInterval(unsigned short timerid);
		CORETYPE GetRemaining(unsigned short timerid);
		void Update();
	};

} // namespace tf