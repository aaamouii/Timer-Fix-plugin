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
#include "common.h"
#include "Config.h"
#include "Console.h"
#include "Types.h"
#include "Pawn.h"
#include "Time.h"
#include "Timer.h"
#include "Natives.h"
#include "Hook.h"
#include "ID.h"

extern void *pAMXFunctions;
logprintf_t logprintf;

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	/// Set AMX functions
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	///	Initialize plugin classes
	CPawn::Initialize();
	CConsole::Initialize(logprintf);
	CTimer::Initialize();
	CTime::Initialize();
	CNatives::Initialize();
	CHook::Initialize();
	CID::Initialize();

	///	Present
	CConsole::Get()->Output("  %s plugin v%s by %s loaded.", PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	CConsole::Get()->Output("%s plugin v%s unloaded.", PLUGIN_NAME, PLUGIN_VERSION);

	CPawn::Destroy();
	CConsole::Destroy();
	CTimer::Destroy();
	CTime::Destroy();
	CNatives::Destroy();
	CHook::Destroy();
	CID::Destroy();
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	CNatives::Get()->Register(amx);
	CHook::Get()->Apply(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	CTimer::Get()->KillAll(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	CTimer::Get()->Process();
}