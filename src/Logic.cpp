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

#include "Logic.h"
#include "System.h"
#include "Natives.h"

extern void* pAMXFunctions;
logprintf_t logprintf;
std::unique_ptr<System> g_pSystem;

void InitializePluginLogic(void** data)
{
	pAMXFunctions = data[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(data[PLUGIN_DATA_LOGPRINTF]);
	g_pSystem = std::make_unique<System>();
	logprintf("  Timer Fix plugin v1.1.0 by KashCherry loaded.");
}

void ResetPluginLogic()
{
	g_pSystem.reset();
	logprintf("Timer Fix plugin v1.1.0 unloaded.");
}

void SetupNativeHooks(AMX* amx)
{
	amx_Redirect(amx, "SetTimer", reinterpret_cast<ucell>(Natives::SetTimer), 0);
	amx_Redirect(amx, "SetTimerEx", reinterpret_cast<ucell>(Natives::SetTimerEx), 0);
	amx_Redirect(amx, "KillTimer", reinterpret_cast<ucell>(Natives::KillTimer), 0);
}