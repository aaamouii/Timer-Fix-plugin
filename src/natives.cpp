#include "natives.h"
#include "core.h"

extern local_ptr<Core> core;

#define CHECK_PARAMS(c) if(params[0] != (c * 4)) core->getInternal()->Log("bad parameter count (count is %d, should be %d)", params[0] / 4, c)

cell AMX_NATIVE_CALL Natives::n_SetTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	char *callback;
	amx_StrParam(amx, params[1], callback);
	return (cell)core->getInterface()->Create(amx, callback, params[2], !!params[3]);
}

cell AMX_NATIVE_CALL Natives::n_SetTimerEx(AMX *amx, cell *params)
{
	if (params[0] < (5 * 4)) core->getInternal()->Log("bad parameter count (count is %d, should be more %d)", params[0] / 4, 4);
	char *callback, *format;
	amx_StrParam(amx, params[1], callback);
	amx_StrParam(amx, params[4], format);
	return (cell)core->getInterface()->CreateEx(amx, callback, params[2], !!params[3], params, format, 5);
}

cell AMX_NATIVE_CALL Natives::n_KillTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return core->getInterface()->Kill(amx, params[1]);
}

cell AMX_NATIVE_CALL Natives::n_KillAllTimers(AMX *amx, cell *params)
{
	core->getInterface()->KillAll(amx);
	return 1;
}

cell AMX_NATIVE_CALL Natives::n_IsValidTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return core->getInterface()->IsValid(amx, params[1]);
}

cell AMX_NATIVE_CALL Natives::n_GetTimerInterval(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return core->getInterface()->GetInterval(amx, params[1]);
}

cell AMX_NATIVE_CALL Natives::n_SetTimerInterval(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	return core->getInterface()->SetInterval(amx, params[1], params[2]);
}

cell AMX_NATIVE_CALL Natives::n_GetTimerRemainingTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return core->getInterface()->GetRemaining(amx, params[1]);
}

const AMX_NATIVE_INFO natives[] = {
	{ "IsValidTimer", Natives::n_IsValidTimer },
	{ "GetTimerInterval", Natives::n_GetTimerInterval },
	{ "SetTimerInterval", Natives::n_SetTimerInterval },
	{ "KillAllTimers", Natives::n_KillAllTimers },
	{ "GetTimerRemainingTime", Natives::n_GetTimerRemainingTime },
	{ NULL, NULL }
};

void Natives::RegisterNatives(AMX *amx)
{
	amx_Register(amx, natives, -1);
}