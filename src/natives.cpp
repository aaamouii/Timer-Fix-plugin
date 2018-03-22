#include "main.hpp"

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

#define GetArgs() (params[0] / sizeof(cell))

cell AMX_NATIVE_CALL Natives::SetTimer(AMX *amx, cell *params)
{
	if (GetArgs() < 3) {
		logprintf("[Timer Fix] SetTimer: number of arguments must be 3");
	}
	char *funcname;
	amx_StrParam(amx, params[1], funcname);
	
	int idx;
	if (amx_FindPublic(amx, funcname, &idx) == AMX_ERR_NONE) {
		return Timer::SetTimer(amx, idx, params[2], params[3]);
	}
	logprintf("[Timer Fix] SetTimer: cannot find public function \"%s\"", funcname);
	return 0;
}

cell AMX_NATIVE_CALL Natives::SetTimerEx(AMX *amx, cell *params)
{
	if (GetArgs() < 5) {
		logprintf("[Timer Fix] SetTimerEx: number of arguments must be at least 5");
	}
	char *funcname;
	char *format;
	amx_StrParam(amx, params[1], funcname);
	amx_StrParam(amx, params[4], format);

	int idx;
	if (amx_FindPublic(amx, funcname, &idx) == AMX_ERR_NONE) {
		return Timer::SetTimerEx(amx, idx, params[2], params[3], format, params, 5);
	}
	logprintf("[Timer Fix] SetTimerEx: cannot find public function \"%s\"", funcname);
	return 0;
}

cell AMX_NATIVE_CALL Natives::KillTimer(AMX *amx, cell *params)
{
	Timer::KillTimer(amx, params[1]);
	return 1;
}

cell AMX_NATIVE_CALL Natives::KillAllTimers(AMX *amx, cell *params)
{
	Timer::KillAllTimers(amx);
	return 1;
}

cell AMX_NATIVE_CALL Natives::IsValidTimer(AMX *amx, cell *params)
{
	if (Timer::IsValidTimer(amx, params[1])) {
		return 1;
	}
	return 0;
}

void Natives::RegisterNatives(AMX *amx)
{
	amx_Redirect(amx, "SetTimer", reinterpret_cast<ucell>(Natives::SetTimer), NULL);
	amx_Redirect(amx, "SetTimerEx", reinterpret_cast<ucell>(Natives::SetTimerEx), NULL);
	amx_Redirect(amx, "KillTimer", reinterpret_cast<ucell>(Natives::KillTimer), NULL);
	vector<AMX_NATIVE_INFO> natives{
		{"KillAllTimers",Natives::KillAllTimers},
		{"IsValidTimer",Natives::IsValidTimer}
	};
	amx_Register(amx, natives.data(), natives.size());
}