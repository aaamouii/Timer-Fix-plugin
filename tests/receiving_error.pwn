//	A test that shows what the timer delay is before it calls the callback.
//	I can't say that it is 100% accurate, but of course it's close to reality.
//
//	So here you can see the default timers VS timerfix.
//	repo: https://github.com/KashCherry/Timer-Fix-plugin
//
//	Description:
//	We save the tickcount and set the timer.
//	After the callback is called, we compare the saved tickcount with the current.
#include <a_samp>
//	Not necessary. Plugin hooks default natives.
#include <timerfix>
//	Note: To switch between default timers and timerfix, you must disable(or enable) timerfix plugin.

new _gTimerTickCount = 0;

forward TimerCallback();
public TimerCallback()
{
	printf("Result: %dms", GetTickCount()-_gTimerTickCount);
	_gTimerTickCount = GetTickCount();
	return 1;
}

main()
{
	#if defined TEST_WITH_REPEAT
	  SetTimer("TimerCallback", 1000, true);
	#else
	  SetTimer("TimerCallback", 1000, false);
	#endif
	  _gTimerTickCount = GetTickCount();
}
