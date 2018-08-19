#include "natives.h"
#include "core.h"

extern boost::scoped_ptr<Core> core;

cell AMX_NATIVE_CALL Natives::n_SetTimer(AMX *amx, cell *params)
{
	char *callback;
	amx_StrParam(amx, params[1], callback);
	return core->getTimer()->Add(callback, params[2], !!params[3]);
}

cell AMX_NATIVE_CALL Natives::n_SetTimerEx(AMX *amx, cell *params)
{
	char *callback, *format;
	amx_StrParam(amx, params[1], callback);
	amx_StrParam(amx, params[4], format);
	return core->getTimer()->AddEx(amx, callback, params[2], !!params[3], params, format, 5);
}

cell AMX_NATIVE_CALL Natives::n_KillTimer(AMX *amx, cell *params)
{
	return core->getTimer()->Remove(params[1]);
}

cell AMX_NATIVE_CALL Natives::n_IsValid(AMX *amx, cell *params)
{
	return core->getTimer()->IsValid(params[1]);
}

cell AMX_NATIVE_CALL Natives::n_KillAllTimers(AMX *amx, cell *params)
{
	core->getTimer()->RemoveAll();
	return 1;
}

cell AMX_NATIVE_CALL Natives::n_SetTimerInterval(AMX *amx, cell *params)
{
	return core->getTimer()->SetInterval(params[1], params[2]);
}

cell AMX_NATIVE_CALL Natives::n_GetTimerInterval(AMX *amx, cell *params)
{
	return core->getTimer()->GetInterval(params[1]);
}

void AMXAPI amxred(AMX *amx, const char *from, ucell to, AMX_NATIVE *store) {
	AMX_HEADER *hdr = (AMX_HEADER*)amx->base;
	AMX_FUNCSTUB *func;
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

const AMX_NATIVE_INFO natives[] = {
	{ "IsValidTimer", Natives::n_IsValid },
	{ "KillAllTimers", Natives::n_KillAllTimers },
	{ "SetTimerInterval", Natives::n_SetTimerInterval },
	{ "GetTimerInterval", Natives::n_GetTimerInterval },
	{ NULL, NULL }
};

void Natives::Register(AMX *amx)
{
	amxred(amx, "SetTimer", (ucell)n_SetTimer, NULL);
	amxred(amx, "SetTimerEx", (ucell)n_SetTimerEx, NULL);
	amxred(amx, "KillTimer", (ucell)n_KillTimer, NULL);
	amx_Register(amx, natives, -1);
}