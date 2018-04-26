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
#include "Timer.hpp"

extern void *pAMXFunctions;

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	Timer::Manager::Timer::Process();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	Timer::Utility::SetFilePath("timerfix_log.txt");
	Timer::Utility::Printf("by KashCherry loaded. Version: 1.0");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	Timer::Utility::Printf("Unloaded.");
}

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx)
{
	amx_Redirect(amx, "SetTimer", reinterpret_cast<ucell>(Timer::Natives::n_SetTimer), NULL);
	amx_Redirect(amx, "SetTimerEx", reinterpret_cast<ucell>(Timer::Natives::n_SetTimerEx), NULL);
	amx_Redirect(amx, "KillTimer", reinterpret_cast<ucell>(Timer::Natives::n_KillTimer), NULL);
	Timer::Manager::Callback::Initialize(amx);
	Timer::Natives::Register(amx);
}