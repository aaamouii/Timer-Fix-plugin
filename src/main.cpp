#include "main.h"
#include "tf.h"

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	return tf::TimeToUpdate();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return tf::Supports();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	return tf::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	return tf::Unload();
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	return tf::AmxLoad(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return tf::AmxUnload(amx);
}