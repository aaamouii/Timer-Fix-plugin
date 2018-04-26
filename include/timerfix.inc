#if defined _timerfix_included
	#endinput
#endif
#define _timerfix_included

native KillAllTimers();
native IsValidTimer(timerid);
native GetTimerInterval(timerid);
native SetTimerInterval(timerid, interval);