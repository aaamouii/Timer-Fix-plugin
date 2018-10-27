//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2013 SA-MP Team, Dan
//
//----------------------------------------------------------

#pragma once

//----------------------------------------------------------

#include <string.h>

//----------------------------------------------------------

#include "amx.h"

//----------------------------------------------------------

#define USENAMETABLE(hdr) \
	((hdr)->defsize==sizeof(AMX_FUNCSTUBNT))

#define NUMENTRIES(hdr,field,nextfield) \
	(unsigned)(((hdr)->nextfield - (hdr)->field) / (hdr)->defsize)

#define GETENTRY(hdr,table,index) \
	(AMX_FUNCSTUB *)((unsigned char*)(hdr) + (unsigned)(hdr)->table + (unsigned)index*(hdr)->defsize)

#define GETENTRYNAME(hdr,entry) \
	(USENAMETABLE(hdr) ? \
		(char *)((unsigned char*)(hdr) + (unsigned)((AMX_FUNCSTUBNT*)(entry))->nameofs) : \
		((AMX_FUNCSTUB*)(entry))->name)

//----------------------------------------------------------