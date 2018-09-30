#pragma once

#include "main.h"

namespace tf {

	void TimeToUpdate();
	unsigned int Supports();
	bool Load(void **ppData);
	void Unload();
	int AmxLoad(AMX *amx);
	int AmxUnload(AMX *amx);

} // namespace tf