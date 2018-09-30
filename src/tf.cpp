#include "tf.h"
#include "kernel.h"
#include "timer.h"
#include "amxhooks.h"
#include "natives.h"

extern void *pAMXFunctions;
tf::Kernel *kernel;
tf::Timer *timer;
tf::amxhooks ah;

void tf::TimeToUpdate()
{
	return timer->Update();
}

unsigned int tf::Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

bool tf::Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	kernel = new tf::Kernel(ppData);
	timer = new tf::Timer;
	return true;
}

void tf::Unload()
{
	kernel->send("unloaded.");
	delete timer;
	delete kernel;
}

int tf::AmxLoad(AMX *amx)
{
	ah.install(amx);
	tf::Natives::Register(amx);
	return AMX_ERR_NONE;
}

int tf::AmxUnload(AMX *amx)
{
	timer->KillAll(amx);
	return AMX_ERR_NONE;
}