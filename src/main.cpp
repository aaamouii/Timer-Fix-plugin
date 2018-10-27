/// Internal includes
/// ----------------------------
#include "main.h"
#include "core.h"
/// ----------------------------

extern void *pAMXFunctions;
extern local_ptr<Core> core;

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	core->getInterface()->startUpdateForAll();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	core->getInternal()->Output("%s plugin v%s unloaded.", PLUGIN_NAME, PLUGIN_VERSION);
	core.reset();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	core.reset(new Core(ppData[PLUGIN_DATA_LOGPRINTF]));
	core->getInternal()->Output("  %s plugin v%s by %s loaded.", PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	return true;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	core->getInterface()->Add(amx);
	core->getNatives()->RegisterNatives(amx);
	core->getHook()->Apply(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	core->getInterface()->KillAll(amx);
	core->getInterface()->Remove(amx);
	return AMX_ERR_NONE;
}