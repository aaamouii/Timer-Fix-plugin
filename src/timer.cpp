#include "timer.h"
#include "kernel.h"

using namespace tf;

extern Kernel *kernel;

std::unordered_map<unsigned short, timer_type *> timer_list;
unsigned short gTimerID = 0;

unsigned short Timer::New(AMX *amx, const char *callback, CORETYPE interval, bool repeat)
{
	timer_type *t = new timer_type;
	if (t == NULL)
	{
		kernel->send("SetTimer: cannot allocate memory.");
		return 0;
	}
	gTimerID++;
	t->amx = amx;
	t->callback_name = callback;
	t->interval = interval;
	t->repeat = repeat;
	t->is_destroyed = false;
	t->entry_point = time::milliseconds::get();
	timer_list.insert(std::pair<int, timer_type *>(gTimerID, t));
	return gTimerID;
}

unsigned short Timer::NewEx(AMX *amx, const char *callback, CORETYPE interval, bool repeat, cell *params, const char *format, int offset)
{
	timer_type *t = new timer_type;
	if (t == NULL)
	{
		kernel->send("SetTimerEx: cannot allocate memory.");
		return 0;
	}
	gTimerID++;
	t->amx = amx;
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
			kernel->send("SetTimerEx: unknown format specifer");
			break;
		}
	}
	t->entry_point = time::milliseconds::get();
	timer_list.insert(std::pair<int, timer_type *>(gTimerID, t));
	return gTimerID;
}

int Timer::Kill(unsigned short timerid)
{
	std::unordered_map<unsigned short, timer_type *>::iterator t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		t->second->is_destroyed = true;
		return 1;
	}
	return 0;
}

int Timer::IsValid(unsigned short timerid)
{
	return (timer_list.find(timerid) != timer_list.end());
}

int Timer::SetInterval(unsigned short timerid, CORETYPE interval)
{
	auto t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		t->second->interval = interval;
		return 1;
	}
	return 0;
}

CORETYPE Timer::GetInterval(unsigned short timerid)
{
	auto t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		return t->second->interval;
	}
	return 0;
}

void Timer::KillAll(AMX *amx)
{
	for (std::unordered_map<unsigned short, timer_type *>::iterator t = timer_list.begin(); t != timer_list.end();)
	{
		if (t->second->amx == amx)
		{
			for (auto arrays : t->second->params.arrays) free(arrays.first);
			t->second->params.arrays.clear();
			t->second->params.strings.clear();
			t->second->params.integers.clear();
			delete t->second;
			timer_list.erase(t++);
			continue;
		}
		t++;
	}
}

CORETYPE Timer::GetRemaining(unsigned short timerid)
{
	std::unordered_map<unsigned short, timer_type *>::iterator t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		return t->second->interval - (time::milliseconds::get() - t->second->entry_point);
	}
	return 0;
}

void Timer::Update()
{
	for (std::unordered_map<unsigned short, timer_type *>::iterator t = timer_list.begin(); t != timer_list.end();)
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
		if ((time::milliseconds::get() - t->second->entry_point) >= t->second->interval)
		{
			cell tmp, retval;
			int idx;
			if (amx_FindPublic(t->second->amx, t->second->callback_name.c_str(), &idx) == AMX_ERR_NONE)
			{
				for (auto arrays : t->second->params.arrays) amx_PushArray(t->second->amx, &tmp, NULL, arrays.first, arrays.second);
				for (auto strings : t->second->params.strings) amx_PushString(t->second->amx, &tmp, NULL, strings.c_str(), NULL, NULL);
				for (auto integers : t->second->params.integers) amx_Push(t->second->amx, integers);
				if (amx_Exec(t->second->amx, &retval, idx) != AMX_ERR_NONE)
				{
					kernel->send("error: cannot execute callback with name \"%s\"", t->second->callback_name.c_str());
				}
			}
			if (t->second->repeat)
			{
				t->second->entry_point = time::milliseconds::get();
				t++;
				continue;
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