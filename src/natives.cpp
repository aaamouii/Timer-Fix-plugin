#include "main.hpp"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

AMX *gl_amx;

cell AMX_NATIVE_CALL Natives::SetFixTimer(AMX *amx, cell *params)
{
	char *function;

	amx_StrParam(amx, params[1], function);

	int function_id;

	if (amx_FindPublic(gl_amx, function, &function_id) == AMX_ERR_NONE) {
		return Timer::CreateTimer(function_id, params[2], params[3]);
	}
	return -1;
}

cell AMX_NATIVE_CALL Natives::SetFixTimerEx(AMX *amx, cell *params)
{
	char
		*function,
		*format;

	amx_StrParam(amx, params[1], function);
	amx_StrParam(amx, params[4], format);

	int function_id;

	if (amx_FindPublic(gl_amx, function, &function_id) == AMX_ERR_NONE) {
		return Timer::CreateTimerEx(amx, function_id, params[2], params[3], format, params);
	}
	return -1;
}

cell AMX_NATIVE_CALL Natives::KillFixTimer(AMX *amx, cell *params)
{
	Timer::DeleteTimer(params[1]);
	return 1;
}

cell AMX_NATIVE_CALL Natives::KillAllTimers(AMX *amx, cell *params)
{
	Timer::DeleteAllTimers();
	return 1;
}

vector<AMX_NATIVE_INFO> natives{
	{"SetFixTimer",Natives::SetFixTimer},
	{"SetFixTimerEx",Natives::SetFixTimerEx},
	{"KillFixTimer",Natives::KillFixTimer},
	{"KillAllTimers",Natives::KillAllTimers}
};

void Natives::RegisterNatives(AMX *amx)
{
	gl_amx = amx;
	amx_Register(amx, natives.data(), natives.size());
}