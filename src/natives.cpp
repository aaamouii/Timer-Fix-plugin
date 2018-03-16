#include "main.hpp"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

cell AMX_NATIVE_CALL Natives::SetTimer(AMX *amx, cell *params)
{
	if (params[0] < 3 * sizeof(cell)) {
		logprintf("[Timer Fix] SetTimer: number of arguments does not match 3");
		return 0;
	}

	char *function;
	amx_StrParam(amx, params[1], function);

	if (!function) {
		logprintf("[Timer Fix] SetTimer: cannot get callback name");
		return 0;
	}

	int function_id;
	if (amx_FindPublic(amx, function, &function_id) == AMX_ERR_NONE) {
		return Timer::CreateTimer(function_id, params[2], params[3]);
	}

	logprintf("[Timer Fix] SetTimer: cannot find callback");
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetTimerEx(AMX *amx, cell *params)
{
	if (params[0] < 4 * sizeof(cell)) {
		logprintf("[Timer Fix] SetTimerEx: number of arguments must be at least 4");
		return 0;
	}

	char
		*function,
		*format;

	amx_StrParam(amx, params[1], function);
	amx_StrParam(amx, params[4], format);

	if (!function || !format) {
		logprintf("[Timer Fix] SetTimerEx: cannot get string, check params you entered");
		return 0;
	}

	int function_id;
	if (amx_FindPublic(amx, function, &function_id) == AMX_ERR_NONE) {
		return Timer::CreateTimerEx(amx, function_id, params[2], params[3], format, params);
	}

	logprintf("[Timer Fix] SetTimerEx: cannot find callback");
	return 0;
}

cell AMX_NATIVE_CALL Natives::KillTimer(AMX *amx, cell *params)
{
	if (params[0] < 1 * sizeof(cell)) {
		logprintf("[Timer Fix] KillTimer: number of arguments does not match 1");
		return 0;
	}

	Timer::DeleteTimer(params[1]);
	return 1;
}

cell AMX_NATIVE_CALL Natives::KillAllTimers(AMX *amx, cell *params)
{
	Timer::DeleteAllTimers();
	return 1;
}

cell AMX_NATIVE_CALL Natives::IsValidTimer(AMX *amx, cell *params)
{
	if (Timer::IsValidTimer(params[1])) {
		return 1;
	}
	return 0;
}

vector<AMX_NATIVE_INFO> natives{
	{"KillAllTimers",Natives::KillAllTimers},
	{"IsValidTimer",Natives::IsValidTimer}
};

void Natives::RegisterNatives(AMX *amx)
{
	amx_Register(amx, natives.data(), natives.size());
}