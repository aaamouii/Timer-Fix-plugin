#if defined _timerfix_included
	#endinput
#endif
#define _timerfix_included

#define INVALID_TIMER_ID		(0)

native KillAllTimers();
native IsValidTimer(timerid);
native GetTimerInterval(timerid);
native SetTimerInterval(timerid, interval);
native GetTimerRemainingTime(timerid);