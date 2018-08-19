#include "main.h"
#include "precompilied.h"
#include "core.h"

extern void *pAMXFunctions;
extern boost::scoped_ptr<Core> core;

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	core->getTimer()->Update();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	core.reset(new Core);
	core->getPlugin()->Present(ppData);
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	core->getPlugin()->Printf("unloaded.");
	core.reset();
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	core->getTimer()->interfaces.insert(amx);
	core->getNatives()->Register(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	core->getTimer()->interfaces.erase(amx);
	core->getTimer()->RemoveAll();
	return AMX_ERR_NONE;
}