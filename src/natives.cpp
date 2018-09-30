#include "natives.h"
#include "kernel.h"
#include "timer.h"

extern tf::Kernel *kernel;
extern tf::Timer *timer;

#define CHECK_PARAMS(c) if(params[0] != (c * 4)) kernel->send("bad parameter count (count is %d, should be %d)", params[0] / 4, c)

cell AMX_NATIVE_CALL tf::Natives::n_SetTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	char *callback;
	amx_StrParam(amx, params[1], callback);
	return (cell)timer->New(amx, callback, (CORETYPE)params[2], !!params[3]);
}

cell AMX_NATIVE_CALL tf::Natives::n_SetTimerEx(AMX *amx, cell *params)
{
	if (params[0] < (5 * 4)) kernel->send("bad parameter count (count is %d, should be more %d)", params[0] / 4, 4);
	char *callback, *format;
	amx_StrParam(amx, params[1], callback);
	amx_StrParam(amx, params[4], format);
	return (cell)timer->NewEx(amx, callback, (CORETYPE)params[2], !!params[3], params, format, 5);
}

cell AMX_NATIVE_CALL tf::Natives::n_KillTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return timer->Kill((unsigned short)params[1]);
}

cell AMX_NATIVE_CALL tf::Natives::n_KillAllTimers(AMX *amx, cell *params)
{
	timer->KillAll(amx);
	return 1;
}

cell AMX_NATIVE_CALL tf::Natives::n_IsValidTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return timer->IsValid((unsigned short)params[1]);
}

cell AMX_NATIVE_CALL tf::Natives::n_GetTimerInterval(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return (cell)timer->GetInterval((unsigned short)params[1]);
}

cell AMX_NATIVE_CALL tf::Natives::n_SetTimerInterval(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	return timer->SetInterval((unsigned short)params[1], (CORETYPE)params[2]);
}

cell AMX_NATIVE_CALL tf::Natives::n_GetTimerRemainingTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return (cell)timer->GetRemaining((unsigned short)params[1]);
}

const AMX_NATIVE_INFO natives[] = {
	{ "IsValidTimer", tf::Natives::n_IsValidTimer },
	{ "GetTimerInterval", tf::Natives::n_GetTimerInterval },
	{ "SetTimerInterval", tf::Natives::n_SetTimerInterval },
	{ "KillAllTimers", tf::Natives::n_KillAllTimers },
	{ "GetTimerRemainingTime", tf::Natives::n_GetTimerRemainingTime },
	{ NULL, NULL }
};

void tf::Natives::Register(AMX *amx)
{
	amx_Register(amx, natives, -1);
}