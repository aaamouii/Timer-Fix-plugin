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