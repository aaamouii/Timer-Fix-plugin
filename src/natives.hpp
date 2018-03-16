#include <vector>

#include "amx/amx.h"
#include "plugincommon.h"

class Natives
{
public:

	static cell AMX_NATIVE_CALL SetTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetTimerEx(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL KillTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL KillAllTimers(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL IsValidTimer(AMX *amx, cell *params);

	static void RegisterNatives(AMX *amx);

};