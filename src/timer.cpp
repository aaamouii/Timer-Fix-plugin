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
/// Internal includes
/// ----------------------------
#include "timer.h"
#include "core.h"
/// ----------------------------

/// External includes
/// ----------------------------
#include <unordered_map>
/// ----------------------------

extern local_ptr<Core> core;

std::unordered_map<int, timer_type *> timer_list;
int gTimerID;

Timer::Timer()
{
	gTimerID = 0;
}

Timer::~Timer()
{
	gTimerID = 0;
}

int Timer::New(const char *callback, int interval, bool repeat)
{
	timer_type *t = new timer_type;
	if (!t)
	{
		core->getInternal()->Log("SetTimer: cannot allocate memory.");
		return 0;
	}

	if (interval < 1) interval = 1;

	gTimerID++;
	t->callback_name = callback;
	t->interval = interval;
	t->repeat = repeat;
	t->is_destroyed = false;
	t->entry_point = std::chrono::high_resolution_clock::now();

	timer_list.insert(std::pair<int, timer_type *>(gTimerID, t));
	return gTimerID;
}

int Timer::NewEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset)
{
	timer_type *t = new timer_type;
	if (!t)
	{
		core->getInternal()->Log("SetTimer: cannot allocate memory.");
		return 0;
	}

	if (interval < 1) interval = 1;

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
			core->getInternal()->Log("SetTimerEx: unknown format specifer");
			break;
		}
	}

	t->entry_point = std::chrono::high_resolution_clock::now();
	timer_list.insert(std::pair<int, timer_type *>(gTimerID, t));
	return gTimerID;
}

int Timer::Kill(int timerid)
{
	std::unordered_map<int, timer_type *>::const_iterator t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		t->second->is_destroyed = true;
		return 1;
	}
	return 0;
}

void Timer::KillAll()
{
	for (std::unordered_map<int, timer_type *>::const_iterator t = timer_list.begin(); t != timer_list.end();)
	{
		for (auto arrays : t->second->params.arrays) free(arrays.first);
		t->second->params.arrays.clear();
		t->second->params.strings.clear();
		t->second->params.integers.clear();
		delete t->second;
		timer_list.erase(t++);
	}
}

int Timer::IsValid(int timerid)
{
	std::unordered_map<int, timer_type *>::const_iterator t = timer_list.find(timerid);
	return (t != timer_list.end());
}

int Timer::SetInterval(int timerid, int interval)
{
	std::unordered_map<int, timer_type *>::iterator t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		if (interval < 1) interval = 1;
		t->second->interval = interval;
		return 1;
	}
	return 0;
}

int Timer::GetInterval(int timerid)
{
	std::unordered_map<int, timer_type *>::iterator t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		return t->second->interval;
	}
	return -1;
}

int Timer::GetRemaining(int timerid)
{
	std::unordered_map<int, timer_type *>::iterator t = timer_list.find(timerid);
	if (t != timer_list.end())
	{
		return (t->second->interval - (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t->second->entry_point).count()));
	}
	return -1;
}

void Timer::Process(AMX *amx)
{
	for (std::unordered_map<int, timer_type *>::const_iterator t = timer_list.begin(); t != timer_list.end();)
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
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t->second->entry_point).count() >= t->second->interval)
		{
			cell tmp, retval;
			int idx;
			if (amx_FindPublic(amx, t->second->callback_name.c_str(), &idx) == AMX_ERR_NONE)
			{
				for (auto arrays : t->second->params.arrays) amx_PushArray(amx, &tmp, NULL, arrays.first, arrays.second);
				for (auto strings : t->second->params.strings) amx_PushString(amx, &tmp, NULL, strings.c_str(), NULL, NULL);
				for (auto integers : t->second->params.integers) amx_Push(amx, integers);
				if (amx_Exec(amx, &retval, idx) != AMX_ERR_NONE)
				{
					core->getInternal()->Log("error: cannot execute callback with name \"%s\"", t->second->callback_name.c_str());
				}
			}

			if (t->second->repeat)
			{
				t->second->entry_point = std::chrono::high_resolution_clock::now();
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