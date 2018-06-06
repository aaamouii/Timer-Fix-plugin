#pragma once

class CNatives
{
public:
	static cell AMX_NATIVE_CALL n_SetTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_SetTimerEx(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_KillTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_KillAllTimers(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_SetTimerInterval(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_GetTimerInterval(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_IsValidTimer(AMX *amx, cell *params);

	void Register(AMX *amx);
};