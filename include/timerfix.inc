// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#if defined _timerfix_included
	#endinput
#endif
#define _timerfix_included
// -----------------------------------------------------------------------------------
#define INVALID_TIMER_ID		(0)
// -----------------------------------------------------------------------------------

/*
 * Replacement for a standart native function that includes additional parameters.
 * delay - the delay before the timer starts.
 * count - count of executions.
 */
native SetCustomTimer(funcname[], interval, delay, count);

/*
 * The same function but with argument passing.
 */
native SetCustomTimerEx(funcname[], delay, count, const format[], {Float,_}:...);
native KillAllTimers();
native IsValidTimer(timerid);
native GetTimerInterval(timerid);
native SetTimerInterval(timerid, interval);
native GetTimerRemainingTime(timerid);

/*
 * Stops the timer but doesn't delete it.
 * Use the ContinueTimer() to start it again.
 * Note: the callback will execute after the time remaining before the call.
 */
native PauseTimer(timerid);

/*
 * Continues the timer after pausing.
 */
native ContinueTimer(timerid);
/*
 * Adds the custom callback for timer.
 * For example:
 * new a = SetTimer("function_1");
 * AddTimerHandler(a, "function_2");
 * The timer will execute both callbacks.
 *
 * Note: arguments will also be passed to both functions.
 */
native AddTimerHandler(timerid, handler[]);
/*
 * Removes the added handler.
 */
native RemoveTimerHandler(timerid, handler[]);