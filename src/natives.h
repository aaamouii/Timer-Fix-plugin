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
#ifndef __NATIVES_H_
#define __NATIVES_H_

#include "Definitions.h"
#include "common.h"

class CNatives
{
public:
	static void Initialize();
	static void Destroy();
	static LWM::local_ptr<CNatives> Get();

	static cell AMX_NATIVE_CALL n_SetTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_SetTimerEx(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_KillTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_KillAllTimers(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_IsValidTimer(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_GetTimerInterval(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_SetTimerInterval(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL n_GetTimerRemainingTime(AMX *amx, cell *params);

	void Register(AMX *amx);
};

#endif // __NATIVES_H_