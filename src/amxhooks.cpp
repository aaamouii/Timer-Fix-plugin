#include "amxhooks.h"
#include "natives.h"

void tf::amxhooks::install(AMX *amx)
{
	amxred(amx, "SetTimer", (ucell)Natives::n_SetTimer, NULL);
	amxred(amx, "SetTimerEx", (ucell)Natives::n_SetTimerEx, NULL);
	amxred(amx, "KillTimer", (ucell)Natives::n_KillTimer, NULL);
}