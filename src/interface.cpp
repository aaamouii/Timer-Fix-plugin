/*
	MIT License

	Copyright (c) 2018 Kash Cherry

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
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