#if defined _timerfix_included
	#endinput
#endif
#define _timerfix_included

native SetFixTimer(funcname[], interval, bool:repeat);
native SetFixTimerEx(funcname[], interval, bool:repeat, const format[], {Float,_}:...);
native KillFixTimer(const id);
native KillAllTimers();