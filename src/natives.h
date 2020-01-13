// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "plugin.h"

cell AMX_NATIVE_CALL n_SetTimer(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_SetTimerEx(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_SetCustomTimer(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_SetCustomTimerEx(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_KillTimer(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_KillAllTimers(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_IsValidTimer(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_SetTimerInterval(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_GetTimerInterval(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_GetTimerRemainingTime(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_PauseTimer(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_ContinueTimer(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_AddTimerHandler(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_RemoveTimerHandler(AMX* amx, cell* params);