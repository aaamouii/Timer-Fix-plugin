#include "interface.h"
#include "core.h"

extern local_ptr<Core> core;

void Interface::Add(AMX *amx)
{
	static Timer single;
	local_timer_map.insert(std::pair<AMX *, Timer>(amx, single));
}

void Interface::Remove(AMX *amx)
{
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		local_timer_map.erase(t);
	}
}

void Interface::startUpdateForAll()
{
	for (std::map<AMX *, Timer>::iterator t = local_timer_map.begin(); t != local_timer_map.end(); t++)
	{
		t->second.Process(t->first);
	}
}

int Interface::Create(AMX *amx, const char *callback, int interval, bool repeat)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.New(callback, interval, repeat);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}

int Interface::CreateEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.NewEx(amx, callback, interval, repeat, params, format, offset);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}

int Interface::Kill(AMX *amx, int timerid)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.Kill(timerid);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}

void Interface::KillAll(AMX *amx)
{
	if (!amx) return;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.KillAll();
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return;
	}
}

int Interface::IsValid(AMX *amx, int timerid)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.IsValid(timerid);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}

int Interface::SetInterval(AMX *amx, int timerid, int interval)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.SetInterval(timerid, interval);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}

int Interface::GetInterval(AMX *amx, int timerid)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.GetInterval(timerid);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}

int Interface::GetRemaining(AMX *amx, int timerid)
{
	if (!amx) return 0;
	std::map<AMX *, Timer>::iterator t = local_timer_map.find(amx);
	if (t != local_timer_map.end())
	{
		return t->second.GetRemaining(timerid);
	}
	else
	{
		core->getInternal()->Log("cannot find machine pointer.");
		return 0;
	}
}