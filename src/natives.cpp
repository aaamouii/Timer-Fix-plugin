/*
	MIT License

	Copyright (c) 2018 Kash Cherry

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
#include "Console.h"
#include "Timer.h"

LWM::local_ptr<CNatives> NativesInstance;

#define CHECK_PARAMS(c) if(params[0] != (c * 4)) CConsole::Get()->Log("bad parameter count (count is %d, should be %d)", params[0] / 4, c)

void CNatives::Initialize()
{
	NativesInstance.reset(new CNatives);
}

void CNatives::Destroy()
{
	NativesInstance.reset();
}

LWM::local_ptr<CNatives> CNatives::Get()
{
	return NativesInstance;
}

cell AMX_NATIVE_CALL CNatives::n_SetTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	char *callback;
	amx_StrParam(amx, params[1], callback);
	return CTimer::Get()->New(amx, callback, params[2], !!params[3]);
}

cell AMX_NATIVE_CALL CNatives::n_SetTimerEx(AMX *amx, cell *params)
{
	if (params[0] < (5 * 4)) CConsole::Get()->Log("bad parameter count (count is %d, should be more %d)", params[0] / 4, 4);
	char *callback, *format;
	amx_StrParam(amx, params[1], callback);
	amx_StrParam(amx, params[4], format);
	return CTimer::Get()->NewEx(amx, callback, params[2], !!params[3], params, format, 5);
}

cell AMX_NATIVE_CALL CNatives::n_KillTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return CTimer::Get()->Kill(params[1]);
}

cell AMX_NATIVE_CALL CNatives::n_KillAllTimers(AMX *amx, cell *params)
{
	CTimer::Get()->KillAll(amx);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::n_IsValidTimer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return CTimer::Get()->IsValid(params[1]);
}

cell AMX_NATIVE_CALL CNatives::n_GetTimerInterval(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return CTimer::Get()->GetInterval(params[1]);
}

cell AMX_NATIVE_CALL CNatives::n_SetTimerInterval(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	return CTimer::Get()->SetInterval(params[1], params[2]);
}

cell AMX_NATIVE_CALL CNatives::n_GetTimerRemainingTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	return CTimer::Get()->GetRemaining(params[1]);
}

constexpr AMX_NATIVE_INFO nativelist[] = {
	{ "KillAllTimers", CNatives::n_KillAllTimers },
	{ "IsValidTimer", CNatives::n_IsValidTimer },
	{ "GetTimerInterval", CNatives::n_GetTimerInterval },
	{ "SetTimerInterval", CNatives::n_SetTimerInterval },
	{ "GetTimerRemainingTime", CNatives::n_GetTimerRemainingTime },
	{NULL,NULL}
};

void CNatives::Register(AMX *amx)
{
	amx_Register(amx, nativelist, -1);
}