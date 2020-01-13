// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "plugin.h"
#include "natives.h"
#include "CServerHooks.h"
#include "CTimerManager.h"
#include "CStorage.h"
#include "CFreeSlotManager.h"
#include "COSTime.h"
// -----------------------------------------------------------------------------------
logprintf_t logprintf;
extern void* pAMXFunctions;
bool bLoadedCorrectly;
// -----------------------------------------------------------------------------------

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	logprintf("  Timer Fix plugin v1.1.1 by KashCherry loaded.");
	bLoadedCorrectly = false;
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("Timer Fix v1.1.1 unloaded.");
	bLoadedCorrectly = false;
	
	CServerHooks::Destroy();
	CTimerManager::Destroy();
	CStorage::Destroy();
	CFreeSlotManager::Destroy();
	COSTime::Destroy();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	CServerHooks::Get()->Apply(amx);
	// The plugin won't be able to find natives that aren't used in the gamemode.
	/*if (CServerHooks::Get()->Checks() == false)
	{
		logprintf("[timerfix.plugin] Plugin won't work until problems are fixed.");
	}*/
	bLoadedCorrectly = true;

	const AMX_NATIVE_INFO native_list[] = {
		{ "SetCustomTimer", n_SetCustomTimer},
		{ "SetCustomTimerEx", n_SetCustomTimerEx },
		{ "KillAllTimers", n_KillAllTimers },
		{ "IsValidTimer", n_IsValidTimer },
		{ "GetTimerInterval", n_GetTimerInterval },
		{ "SetTimerInterval", n_SetTimerInterval },
		{ "PauseTimer", n_PauseTimer },
		{ "ContinueTimer", n_ContinueTimer },
		{ "AddTimerHandler", n_AddTimerHandler },
		{ "RemoveTimerHandler", n_RemoveTimerHandler },
		{ NULL, NULL }
	};

	return amx_Register(amx, native_list, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	CServerHooks::Get()->Restore(amx);
	bLoadedCorrectly = false;
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	CTimerManager::Get()->Process();
}