#include "main.h"

extern CCore *m_pCore;
extern logprintf_t logprintf;

cell AMX_NATIVE_CALL CNatives::n_SetTimer(AMX *amx, cell *params)
{
	char *callback_name;
	amx_StrParam(amx, params[1], callback_name);
	return m_pCore->getTimer()->New(amx, callback_name, params[2], !!params[3]);
}

cell AMX_NATIVE_CALL CNatives::n_SetTimerEx(AMX *amx, cell *params)
{
	char *callback_name;
	char *format;
	amx_StrParam(amx, params[1], callback_name);
	amx_StrParam(amx, params[4], format);
	return m_pCore->getTimer()->NewEx(amx, callback_name, params[2], !!params[3], params, format, 5);
}

cell AMX_NATIVE_CALL CNatives::n_KillTimer(AMX *amx, cell *params)
{
	return m_pCore->getTimer()->Kill(amx, params[1]);
}

cell AMX_NATIVE_CALL CNatives::n_KillAllTimers(AMX *amx, cell *params)
{
	m_pCore->getTimer()->KillAllTimers();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::n_SetTimerInterval(AMX *amx, cell *params)
{
	return m_pCore->getTimer()->SetInterval(amx, params[1], params[2]);
}

cell AMX_NATIVE_CALL CNatives::n_GetTimerInterval(AMX *amx, cell *params)
{
	return m_pCore->getTimer()->GetInterval(amx, params[1]);
}

cell AMX_NATIVE_CALL CNatives::n_IsValidTimer(AMX *amx, cell *params)
{
	return m_pCore->getTimer()->IsValid(amx, params[1]);
}

const AMX_NATIVE_INFO PluginNatives[] = {
	{"KillAllTimers", CNatives::n_KillAllTimers},
	{"SetTimerInterval", CNatives::n_SetTimerInterval},
	{"GetTimerInterval", CNatives::n_GetTimerInterval},
	{"IsValidTimer", CNatives::n_IsValidTimer},
	{0,0}
};

void CNatives::Register(AMX *amx)
{
	amx_Register(amx, PluginNatives, -1);
}