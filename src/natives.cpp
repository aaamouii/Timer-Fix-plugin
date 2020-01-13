// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "natives.h"
#include "CStorage.h"

extern bool bLoadedCorrectly;
#define BYTES_TO_CELLS(bytes)		(cell)(bytes/sizeof(cell))
extern logprintf_t logprintf;

cell AMX_NATIVE_CALL n_SetTimer(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 3)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 3)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	char* funcname;
	amx_StrParam(amx, params[1], funcname);

	return CStorage::Get()->Add(amx, funcname, params[2], !!params[3]);
}

cell AMX_NATIVE_CALL n_SetTimerEx(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) < 5)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be greater than 5)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	char* funcname, * format;
	amx_StrParam(amx, params[1], funcname);
	amx_StrParam(amx, params[4], format);

	return CStorage::Get()->Add(amx, funcname, params[2], !!params[3], format, &params[5]);
}

cell AMX_NATIVE_CALL n_SetCustomTimer(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 4)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 4)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	};
	char* funcname;
	amx_StrParam(amx, params[1], funcname);

	cell id = CStorage::Get()->Add(amx, funcname, params[2], true);
	CTimer* pTimer = CStorage::Get()->GetById(id);
	if (pTimer)
	{
		pTimer->SetDelay(params[3]);
		// Just let it run forever.
		if(params[4] != -1) pTimer->SetCount(params[4]);
	}
	return id;
}

cell AMX_NATIVE_CALL n_SetCustomTimerEx(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) < 6)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be greater than 6)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	char* funcname, * format;
	amx_StrParam(amx, params[1], funcname);
	amx_StrParam(amx, params[5], format);

	cell id = CStorage::Get()->Add(amx, funcname, params[2], true, format, &params[6]);
	CTimer* pTimer = CStorage::Get()->GetById(id);
	if (pTimer)
	{
		pTimer->SetDelay(params[3]);
		if (params[4] != -1) pTimer->SetCount(params[4]);
	}
	return id;
}

cell AMX_NATIVE_CALL n_KillTimer(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 1)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		pTimer->Remove();
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL n_KillAllTimers(AMX* amx, cell* params)
{
	for (std::map<cell, CTimer*>::iterator iter = CStorage::Get()->m_pTimerDataStorage.begin(); iter != CStorage::Get()->m_pTimerDataStorage.end(); iter++)
	{
		if (iter->second)
		{
			// We can't just remove all timers from the storage while processing script...
			// So let the process function do it
			iter->second->Remove();
		}
	}
	return 1;
}

cell AMX_NATIVE_CALL n_IsValidTimer(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 1)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}
	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	return (pTimer) ? 1 : 0;
}

cell AMX_NATIVE_CALL n_SetTimerInterval(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 2)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}
	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		pTimer->SetInterval(params[2]);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL n_GetTimerInterval(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 1)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}
	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		return pTimer->GetInterval();
	}
	return -1;
}

cell AMX_NATIVE_CALL n_GetTimerRemainingTime(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 1)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}
	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		return pTimer->GetRemainingTime();
	}
	return -1;
}

cell AMX_NATIVE_CALL n_PauseTimer(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 1)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		pTimer->Pause();
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL n_ContinueTimer(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 1)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		pTimer->Continue();
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL n_AddTimerHandler(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 2)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		char* handler;
		amx_StrParam(amx, params[2], handler);
		pTimer->AddHandler(handler);
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL n_RemoveTimerHandler(AMX* amx, cell* params)
{
	if (BYTES_TO_CELLS(params[0]) != 2)
	{
		logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be 1)", __FUNCTION__, BYTES_TO_CELLS(params[0]));
		return 0;
	}

	CTimer* pTimer = CStorage::Get()->GetById(params[1]);
	if (pTimer)
	{
		char* handler;
		amx_StrParam(amx, params[2], handler);
		pTimer->RemoveHandler(handler);
		return 1;
	}
	return 0;
}