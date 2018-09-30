#pragma once

#include "main.h"
#include <amx/amx2.h>

#include <assert.h>

namespace tf {

	class amxhooks {
	public:
		void install(AMX *amx);

	private:
		void AMXAPI amxred(AMX *amx, const char *from, ucell to, AMX_NATIVE *store) {
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
	};

} // namespace tf