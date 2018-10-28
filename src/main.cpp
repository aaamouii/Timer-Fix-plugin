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
/// Internal includes
/// ----------------------------
#include "main.h"
#include "core.h"
/// ----------------------------

extern void *pAMXFunctions;
extern local_ptr<Core> core;

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	core->getInterface()->startUpdateForAll();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	core->getInternal()->Output("%s plugin v%s unloaded.", PLUGIN_NAME, PLUGIN_VERSION);
	core.reset();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	core.reset(new Core(ppData[PLUGIN_DATA_LOGPRINTF]));
	core->getInternal()->Output("  %s plugin v%s by %s loaded.", PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	return true;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	core->getInterface()->Add(amx);
	core->getNatives()->RegisterNatives(amx);
	core->getHook()->Apply(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	core->getInterface()->KillAll(amx);
	core->getInterface()->Remove(amx);
	return AMX_ERR_NONE;
}