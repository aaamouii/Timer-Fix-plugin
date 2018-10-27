#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "main.h"
#include "timer.h"

#include <map>

class Interface
{
private:
	std::map<AMX *, Timer> local_timer_map;

public:
	void Add(AMX *amx);
	void Remove(AMX *amx);

	void startUpdateForAll();

	int Create(AMX *amx, const char *callback, int interval, bool repeat);
	int CreateEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset);
	int Kill(AMX *amx, int timerid);
	void KillAll(AMX *amx);
	int IsValid(AMX *amx, int timerid);
	int SetInterval(AMX *amx, int timerid, int interval);
	int GetInterval(AMX *amx, int timerid);
	int GetRemaining(AMX *amx, int timerid);

};

#endif