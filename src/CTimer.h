#pragma once

class CTimer
{
public:
	typedef struct
	{
		deque<pair<cell*, cell>> arrays;
		deque<const char *> strings;
		deque<cell> integers;
	} params_t;

	typedef struct
	{
		string callback_name;
		params_t parameters;
		int interval;
		bool repeat;
		bool is_destroyed;
		chrono::steady_clock::time_point start_time_point;
	} timer_t;

	typedef map<int, timer_t *> timers_t;
	typedef map<AMX *, timers_t> amx_t;

	bool Init(AMX *amx);
	bool UnInit(AMX *amx);
	TIMERID New(AMX *amx, const char *callback, int interval, bool repeat);
	TIMERID NewEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset);
	bool Kill(AMX *amx, int timerid);
	void KillAllTimers();
	bool SetInterval(AMX *amx, int timerid, int interval);
	int GetInterval(AMX *amx, int timerid);
	bool IsValid(AMX *amx, int timerid);

	void Process();
};