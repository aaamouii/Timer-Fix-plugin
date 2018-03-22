#include "main.hpp"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

Timer::AmxList amx_list;
int gTimerID = 0;

void Timer::Init(AMX *amx)
{
	if (isGamemode(amx)) {
		amx_list.push_front(AmxListItem{ amx });
	} else {
		amx_list.push_back(AmxListItem{ amx });
	}
	Natives::RegisterNatives(amx);
}

int Timer::SetTimer(AMX *amx, int funcidx, int interval, bool repeat)
{
	gTimerID++;
	const auto amx_iter = find_if(amx_list.begin(), amx_list.end(), [amx](const AmxListItem &item) {
		return item.amx == amx;
	});

	if (amx_iter != amx_list.end()) {
		try {
			amx_iter->timer_map.emplace(gTimerID, TimerMapItem{ funcidx, interval, chrono::steady_clock::now(), repeat, false });
		} catch (exception &e) {
			logprintf("[Timer Fix] SetTimer: %s [EL: 1]", e.what());
		}
	} else {
		logprintf("[Timer Fix] SetTimer: cannot find AMX [EL: 1]");
	}
	return gTimerID;
}

int Timer::SetTimerEx(AMX *amx, int funcidx, int interval, bool repeat, char *format, cell *params, int offset)
{
	gTimerID++;
	const auto amx_iter = find_if(amx_list.begin(), amx_list.end(), [amx](const AmxListItem &item) {
		return item.amx == amx;
	});

	if (amx_iter != amx_list.end()) {
		deque<cell> _timer_params;

		for (int i = strlen(format); --i != -1;) {
			switch (format[i]) {
			case 'i':
			case 'I':
			case 'd':
			case 'D':
			case 'c':
			case 'C':
			case 'b':
			case 'B':
			case 'f':
			case 'F':
				_timer_params.push_back(*getAddr(amx, params[offset + i]));
				break;
			case 's':
			case 'S':
				_timer_params.push_back(params[offset + i]);
				break;
			default:
				logprintf("[Timer Fix] SetTimerEx: unknown format specifer '%c'", format[i]);
			}
		}
		try {
			amx_iter->timer_map.emplace(gTimerID, TimerMapItem{ funcidx, interval, chrono::steady_clock::now(), repeat, false, _timer_params });
		} catch (exception &e) {
			logprintf("[Timer Fix] SetTimerEx: %s [EL: 1]", e.what());
		}
	} else {
		logprintf("[Timer Fix] SetTimerEx: cannot find AMX [EL: 1]");
	}
	return gTimerID;
}

void Timer::KillTimer(AMX *amx, int timerid)
{
	const auto amx_iter = find_if(amx_list.begin(), amx_list.end(), [amx](const AmxListItem &item) {
		return item.amx == amx;
	});

	if (amx_iter != amx_list.end()) {
		const auto iter = amx_iter->timer_map.find(timerid);
		if (iter != amx_iter->timer_map.end()) {
			iter->second.isdestroyed = true;
		} else {
			logprintf("[Timer Fix] KillTimer: cannot find timer with id %d", timerid);
		}
	} else {
		logprintf("[Timer Fix] KillTimer: cannot find AMX [EL: 1]");
	}
}

void Timer::KillAllTimers(AMX *amx)
{
	const auto amx_iter = find_if(amx_list.begin(), amx_list.end(), [amx](const AmxListItem &item) {
		return item.amx == amx;
	});

	if (amx_iter != amx_list.end()) {
		try {
			amx_iter->timer_map.clear();
		} catch (exception &e) {
			logprintf("[Timer Fix] KillAllTimers: %s [EL: 1]", e.what());
		}
	} else {
		logprintf("[Timer Fix] KillAllTimers: cannot find AMX [EL: 1]");
	}
}

bool Timer::IsValidTimer(AMX *amx, int timerid)
{
	const auto amx_iter = find_if(amx_list.begin(), amx_list.end(), [amx](const AmxListItem &item) {
		return item.amx == amx;
	});

	if (amx_iter != amx_list.end()) {
		if (amx_iter->timer_map.find(timerid) != amx_iter->timer_map.end()) return true;
	} else {
		logprintf("[Timer Fix] IsValidTimer: cannot find AMX [EL: 1]");
	}
	return false;
}

void Timer::Process()
{
	for (auto &amx_iter : amx_list) {
		for (auto iter = amx_iter.timer_map.begin(); iter != amx_iter.timer_map.end(); iter++) {
			if (iter->second.isdestroyed) {
				try {
					amx_iter.timer_map.erase(iter);
				} catch (exception &e) {
					logprintf("[Timer Fix] %s [EL: 1]", e.what());
				}
				continue;
			}
			if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - iter->second.start_time).count() >= iter->second.interval) {
				for (const auto params : iter->second.params) amx_Push(amx_iter.amx, params);
				if (amx_Exec(amx_iter.amx, nullptr, iter->second.funcidx) != AMX_ERR_NONE) {
					logprintf("[Timer Fix] Cannot call function [EL: 1]");
				}
				if (iter->second.repeat) {
					iter->second.start_time = chrono::steady_clock::now();
				} else {
					try {
						amx_iter.timer_map.erase(iter);
					} catch (exception &e) {
						logprintf("[Timer Fix] %s [EL: 1]", e.what());
					}
				}
			}
		}
	}
}