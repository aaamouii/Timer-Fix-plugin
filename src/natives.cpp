/*
	MIT License
	Copyright (c) 2018-2019 Kash Cherry
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "Natives.h"
#include "System.h"
#include "PawnString.h"

#define BYTES_TO_CELLS(bytes)				(cell)(bytes/sizeof(cell))
#define CELLS_TO_BYTES(cells)				(bytes*sizeof(cell))
#define MAKE_OFFSET(cells,offset)			(cells + offset * sizeof(cell))

#define INVALID_TIMER_ID					(0)

extern logprintf_t logprintf;
extern std::unique_ptr<System> g_pSystem;

bool CheckNativeParamsForDefinedCount(cell count_of_params, cell waited_count, const char* func)
{
	if (BYTES_TO_CELLS(count_of_params) == waited_count) return true;
	logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be %d)", func, BYTES_TO_CELLS(count_of_params), waited_count);
	return false;
}

bool CheckNativeParamsForUndefinedCount(cell count_of_params, cell waited_count, const char* func)
{
	if (BYTES_TO_CELLS(count_of_params) >= waited_count) return true;
	logprintf("[timerfix.plugin] %s: bad parameter count (count is %d, should be %d)", func, BYTES_TO_CELLS(count_of_params), waited_count);
	return false;
}

cell AMX_NATIVE_CALL Natives::SetTimer(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForDefinedCount(params[0], 3, __FUNCTION__)) return INVALID_TIMER_ID;
	PawnString fname(amx, params[1]);
	if (fname.get_instance().empty())
	{
		logprintf("[timerfix.plugin] %s: cannot get string", __FUNCTION__);
		return INVALID_TIMER_ID;
	}
	return g_pSystem->GetTimerManager()->AddTimer(amx, fname.get_instance().c_str(), params[2], !!params[3]);
}

cell AMX_NATIVE_CALL Natives::SetTimerEx(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForUndefinedCount(params[0], 5, __FUNCTION__)) return INVALID_TIMER_ID;
	PawnString fname(amx, params[1]);
	if (fname.get_instance().empty())
	{
		logprintf("[timerfix.plugin] %s: cannot get string", __FUNCTION__);
		return INVALID_TIMER_ID;
	}
	PawnString format(amx, params[4]);
	if (format.get_instance().empty())
	{
		logprintf("[timerfix.plugin] %s: cannot get string", __FUNCTION__);
		return INVALID_TIMER_ID;
	}
	return g_pSystem->GetTimerManager()->AddTimerEx(amx, fname.get_instance().c_str(), params[2], !!params[3], format.get_instance().c_str(), &params[5]);
}

cell AMX_NATIVE_CALL Natives::KillTimer(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForDefinedCount(params[0], 1, __FUNCTION__)) return INVALID_TIMER_ID;
	return g_pSystem->GetTimerManager()->KillTimer(params[1]);
}

cell AMX_NATIVE_CALL Natives::KillAllTimers(AMX* amx, cell* params)
{
	g_pSystem->GetTimerManager()->KillAll(amx);
	return 1;
}

cell AMX_NATIVE_CALL Natives::IsValidTimer(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForDefinedCount(params[0], 1, __FUNCTION__)) return INVALID_TIMER_ID;
	return g_pSystem->GetTimerManager()->IsValidTimer(params[1]);
}

cell AMX_NATIVE_CALL Natives::GetTimerInterval(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForDefinedCount(params[0], 1, __FUNCTION__)) return INVALID_TIMER_ID;
	return g_pSystem->GetTimerManager()->GetTimerInterval(params[1]);
}

cell AMX_NATIVE_CALL Natives::SetTimerInterval(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForDefinedCount(params[0], 2, __FUNCTION__)) return INVALID_TIMER_ID;
	return g_pSystem->GetTimerManager()->SetTimerInterval(params[1], params[2]);
}

cell AMX_NATIVE_CALL Natives::GetTimerRemainingTime(AMX* amx, cell* params)
{
	if (!CheckNativeParamsForDefinedCount(params[0], 1, __FUNCTION__)) return INVALID_TIMER_ID;
	return g_pSystem->GetTimerManager()->GetTimerRemainingTime(params[1]);
}

int Natives::Register(AMX* amx)
{
	const AMX_NATIVE_INFO natives[] = {
		{"KillAllTimers", Natives::KillAllTimers},
		{"IsValidTimer", Natives::IsValidTimer},
		{"GetTimerInterval", Natives::GetTimerInterval},
		{"SetTimerInterval", Natives::SetTimerInterval},
		{"GetTimerRemainingTime", Natives::GetTimerRemainingTime},
		{NULL, NULL}
	};
	return amx_Register(amx, natives, -1);
}