#include <unordered_map>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>
#include <string.h>

#include "amx/amx.h"
#include "plugincommon.h"

using namespace std;

class Timer
{
public:

	struct TimerMapItem;
	struct AmxListItem;

	using TimerMap = unordered_map<int, TimerMapItem>;
	using AmxList = list<AmxListItem>;

	struct TimerMapItem {
		int funcidx,
			interval;
		chrono::steady_clock::time_point start_time;
		bool repeat,
			isdestroyed;
		deque<cell> params;
	};

	struct AmxListItem {
		AMX *amx;
		TimerMap timer_map;
	};

	static void Init(AMX *amx);
	static int SetTimer(AMX *amx, int funcidx, int interval, bool repeat);
	static int SetTimerEx(AMX *amx, int funcidx, int interval, bool repeat, char *format, cell *params, int offset);
	static void KillTimer(AMX *amx, int timerid);
	static void KillAllTimers(AMX *amx);
	static bool IsValidTimer(AMX *amx, int timerid);
	static void Process();

private:

	static bool isGamemode(AMX *amx) {
		int idx;
		if (amx_FindPublic(amx, "OnFilterScriptInit", &idx) == AMX_ERR_NONE) return false;
		return true;
	}

	static cell *getAddr(AMX *amx, cell amx_addr)
	{
		cell *addr;
		if (amx_GetAddr(amx, amx_addr, &addr) == AMX_ERR_NONE) return addr;
		return nullptr;
	}
};