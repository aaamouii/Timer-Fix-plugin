#include <map>
#include <chrono>
#include <deque>

#include "amx/amx.h"
#include "plugincommon.h"

using namespace std;

class Timer
{
public:

	struct TimerMapItem {
		int
			function_id,
			end_time;
		chrono::steady_clock::time_point start_time;
		bool repeat;
		deque<cell> params;
	};

	using TimerMap = map<int, TimerMapItem>;

	static int CreateTimer(int function_id, int end_time, bool repeat);
	static int CreateTimerEx(AMX *amx, int function_id, int end_time, bool repeat, const char *format, cell *params);
	static void DeleteTimer(const int id);
	static void DeleteAllTimers();
	static void ProcessTimer(AMX *amx);

};