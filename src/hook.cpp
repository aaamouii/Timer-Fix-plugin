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
#include "hook.h"
#include "core.h"

#include "sdk/amx/amx2.h"
#include <assert.h>

extern local_ptr<Core> core;

void AMXAPI localhook(AMX *amx, const char *from, ucell to, AMX_NATIVE *store) {
	AMX_HEADER *hdr = (AMX_HEADER*)amx->base;
	AMX_FUNCSTUB *func;

	assert(hdr != NULL);
	assert(hdr->magic == AMX_MAGIC);

	for (int idx = 0, num = NUMENTRIES(hdr, natives, libraries); idx != num; ++idx) {
		func = GETENTRY(hdr, natives, idx);
		if (!strcmp(from, GETENTRYNAME(hdr, func))) {
			if (store) {
				*store = (AMX_NATIVE)func->address;
			}
			func->address = to;
			return;
		}
	}
}

void Hook::Apply(AMX *amx)
{
	localhook(amx, "SetTimer", (ucell)core->getNatives()->n_SetTimer, NULL);
	localhook(amx, "SetTimerEx", (ucell)core->getNatives()->n_SetTimerEx, NULL);
	localhook(amx, "KillTimer", (ucell)core->getNatives()->n_KillTimer, NULL);
}