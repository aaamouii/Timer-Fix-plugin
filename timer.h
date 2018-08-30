#ifndef TIMER_H_
#define TIMER_H_

#include "precompilied.h"

class Timer
{
public:
	std::set<AMX *> interfaces;
	unsigned short gTimerID = 0;

	unsigned short Add(AMX *amx, const char *callback, int interval, bool repeat);
	unsigned short AddEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset);
	int Remove(int timerid);
	void RemoveAll(AMX *amx);
	int IsValid(int timerid);
	int SetInterval(int timerid, int interval);
	int GetInterval(int timerid);
	void Update();

private:
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
		int interval;
		bool repeat;
		bool is_destroyed;
		params_type params;
		boost::chrono::steady_clock::time_point entry_point;
	} timer_type;
	boost::unordered_map<int, timer_type *> timer_list;
};

#endif