#include "main.h"

CCallback::callback_t callback;

bool CCallback::Init(AMX *amx)
{
	callbacks_t callbacks;

	int num_publics;
	amx_NumPublics(amx, &num_publics);

	for (int i = 0; i < num_publics; i++)
	{
		char public_name[MAX_PUBLIC_NAME];
		amx_GetPublic(amx, i, public_name);
		callbacks.emplace(move(public_name), i);
	}

	callback.emplace(amx, callbacks);
	return true;
}

bool CCallback::UnInit(AMX *amx)
{
	callback_t::iterator iter = callback.find(amx);
	if (iter != callback.end())
	{
		iter->second.clear();
	}
	callback.erase(iter);
	return true;
}

int CCallback::FindPublic(AMX *amx, const char *public_name)
{
	callback_t::iterator iter = callback.find(amx);
	if (iter != callback.end())
	{
		callbacks_t::iterator public_iter = iter->second.find(public_name);
		if (public_iter != iter->second.end())
		{
			return public_iter->second;
		}
	}
	return -1;
}