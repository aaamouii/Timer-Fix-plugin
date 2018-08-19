#include "timer.h"
#include "core.h"

extern boost::scoped_ptr<Core> core;

unsigned short Timer::Add(const char *callback, int interval, bool repeat)
{
	timer_type *t = new timer_type;
	if (t == NULL)
	{
		core->getPlugin()->Printf("SetTimer: cannot allocate memory.");
		return 0;
	}
	gTimerID++;
	t->callback_name = callback;
	t->interval = interval;
	t->repeat = repeat;
	t->is_destroyed = false;
	t->entry_point = boost::chrono::steady_clock::now();
	timer_list.insert(std::pair<int, timer_type *>(gTimerID, t));
	return gTimerID;
}

unsigned short Timer::AddEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset)
{
	timer_type *t = new timer_type;
	if (t == NULL)
	{
		core->getPlugin()->Printf("SetTimerEx: cannot allocate memory.");
		return 0;
	}
	gTimerID++;
	t->callback_name = callback;
	t->interval = interval;
	t->repeat = repeat;
	t->is_destroyed = false;

	int j = strlen(format);
	while (j)
	{
		j--;
		switch (format[j])
		{
		case 'a':
		case 'A':
			cell * ptr_arr, *ptr_len, *arr;
			amx_GetAddr(amx, params[j + offset], &ptr_arr);
			amx_GetAddr(amx, params[j + offset + 1], &ptr_len);
			arr = (cell*)malloc((*ptr_len * sizeof(cell)));
			if (arr != NULL)
			{
				memcpy(arr, ptr_arr, (*ptr_len * sizeof(cell)));
				t->params.arrays.push_back(std::pair<cell *, cell>(arr, *ptr_len));
			}
			break;
		case 's':
		case 'S':
			char *amx_string;
			amx_StrParam(amx, params[j + offset], amx_string);
			t->params.strings.push_back(amx_string);
			break;
		case 'f':
		case 'F':
		case 'c':
		case 'C':
		case 'd':
		case 'D':
		case 'i':
		case 'I':
		case 'b':
		case 'B':
			cell * amx_integer;
			amx_GetAddr(amx, params[j + offset], &amx_integer);
			t->params.integers.push_back(*amx_integer);
			break;
		default:
			core->getPlugin()->Printf("SetTimerEx: unknown format specifer");
		}
	}
	t->entry_point = boost::chrono::steady_clock::now();
	timer_list.insert(std::pair<int, timer_type *>(gTimerID, t));
	return gTimerID;
}

int Timer::Remove(int timerid)
{
	auto t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		t->second->is_destroyed = true;
		return 1;
	}
	return 0;
}

void Timer::RemoveAll()
{
	for (auto t = timer_list.begin(); t != timer_list.end();)
	{
		for (auto arrays : t->second->params.arrays) free(arrays.first);
		t->second->params.arrays.clear();
		t->second->params.strings.clear();
		t->second->params.integers.clear();
		delete t->second;
		timer_list.erase(t);
		t++;
	}
}

int Timer::IsValid(int timerid)
{
	return (timer_list.find(timerid) != timer_list.end());
}

int Timer::SetInterval(int timerid, int interval)
{
	auto t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		t->second->interval = interval;
		return 1;
	}
	return 0;
}

int Timer::GetInterval(int timerid)
{
	auto t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		return t->second->interval;
	}
	return 0;
}

void Timer::Update()
{
	for (auto t = timer_list.begin(); t != timer_list.end();)
	{
		if (t->second->is_destroyed)
		{
			for (auto arrays : t->second->params.arrays) free(arrays.first);
			t->second->params.arrays.clear();
			t->second->params.strings.clear();
			t->second->params.integers.clear();
			delete t->second;
			timer_list.erase(t++);
			continue;
		}
		if (boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::steady_clock::now() - t->second->entry_point).count() >= t->second->interval)
		{
			cell tmp, retval;
			int idx;
			for (auto m = interfaces.begin(); m != interfaces.end(); m++)
			{
				if (amx_FindPublic(*m, t->second->callback_name.c_str(), &idx) == AMX_ERR_NONE)
				{
					for (auto arrays : t->second->params.arrays) amx_PushArray(*m, &tmp, NULL, arrays.first, arrays.second);
					for (auto strings : t->second->params.strings) amx_PushString(*m, &tmp, NULL, strings.c_str(), NULL, NULL);
					for (auto integers : t->second->params.integers) amx_Push(*m, integers);
					if (amx_Exec(*m, &retval, idx) != AMX_ERR_NONE)
					{
						core->getPlugin()->Printf("error: cannot execute callback with name \"%s\"", t->second->callback_name.c_str());
					}
				}
			}
			if (t->second->repeat)
			{
				t->second->entry_point = boost::chrono::steady_clock::now();
			}
			else
			{
				for (auto arrays : t->second->params.arrays) free(arrays.first);
				t->second->params.arrays.clear();
				t->second->params.strings.clear();
				t->second->params.integers.clear();
				delete t->second;
				timer_list.erase(t++);
				continue;
			}
		}
		t++;
	}
}