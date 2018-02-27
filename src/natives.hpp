#include <vector>

#include "amx/amx.h"
#include "plugincommon.h"

class Natives
{
public:

	static cell AMX_NATIVE_CALL SetFixTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetFixTimerEx(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL KillFixTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL KillAllTimers(AMX *amx, cell *params);

	static void RegisterNatives(AMX *amx);

};