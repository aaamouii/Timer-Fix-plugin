/*
	MIT License

	Copyright (c) 2018 Takagi with love <3

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
#include "main.h"
logprintf_t logprintf;
extern void *pAMXFunctions;

CCore *m_pCore;

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	m_pCore->getTimer()->Process();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	m_pCore = new CCore();
	logprintf("[Timer Fix plugin] by KashCherry loaded. Version: 1.1");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	DELETE_SAFE(m_pCore);
	logprintf("[Timer Fix plugin] unloaded.");
}

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx)
{
	if (coolamx::init(amx) == COOL_AMX_ERROR_OK)
	{
		coolamx::hook_native(amx, "SetTimer", reinterpret_cast<void*>(m_pCore->getNatives()->n_SetTimer));
		coolamx::hook_native(amx, "SetTimerEx", reinterpret_cast<void*>(m_pCore->getNatives()->n_SetTimerEx));
		coolamx::hook_native(amx, "KillTimer", reinterpret_cast<void*>(m_pCore->getNatives()->n_KillTimer));
	}
	m_pCore->getNatives()->Register(amx);
	m_pCore->getTimer()->Init(amx);
	m_pCore->getCallback()->Init(amx);
}

PLUGIN_EXPORT void PLUGIN_CALL AmxUnload(AMX *amx)
{
	coolamx::uninit(amx);
	m_pCore->getCallback()->UnInit(amx);
	m_pCore->getTimer()->UnInit(amx);
}