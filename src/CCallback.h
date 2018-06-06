#pragma once

class CCallback
{
public:
	typedef unordered_map<string, int> callbacks_t;
	typedef unordered_map<AMX *, callbacks_t> callback_t;

	bool Init(AMX *amx);
	bool UnInit(AMX *amx);
	int FindPublic(AMX *amx, const char *public_name);
};