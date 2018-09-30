#pragma once

#include "main.h"

namespace tf {

	class Natives {
	public:

		// hooked natives
		static cell AMX_NATIVE_CALL n_SetTimer(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL n_SetTimerEx(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL n_KillTimer(AMX *amx, cell *params);

		// timerfix natives
		static cell AMX_NATIVE_CALL n_IsValidTimer(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL n_GetTimerInterval(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL n_SetTimerInterval(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL n_GetTimerRemainingTime(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL n_KillAllTimers(AMX *amx, cell *params);

		// timerfix register natives
		static void Register(AMX *amx);
	};

}