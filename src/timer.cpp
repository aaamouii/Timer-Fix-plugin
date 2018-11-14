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
#include "Timer.h"
#include "Console.h"
#include "Definitions.h"
#include "Time.h"
#include "Pawn.h"

LWM::local_ptr<CTimer> TimerInstance;

void CTimer::Initialize()
{
	TimerInstance.reset(new CTimer);
}

void CTimer::Destroy()
{
	TimerInstance.reset();
}

LWM::local_ptr<CTimer> CTimer::Get()
{
	return TimerInstance;
}

TimerID CTimer::New(AMX *amx, const char *callback, int interval, Flag repeat)
{
	RemoteTimerStruct *remoteTimer = new RemoteTimerStruct;
	if (!remoteTimer) {
		CConsole::Get()->Log("cannot allocate memory");
		return INVALID_TIMER_ID;
	}

	if (currentId >= 0xFFFF) {
		delete remoteTimer;
		CConsole::Get()->Log("limit reached");
		return INVALID_TIMER_ID;
	}

	if (CPawn::Get()->Find(amx, callback) == false) {
		delete remoteTimer;
		CConsole::Get()->Log("cannot find public with name \"%s\"", callback);
		return INVALID_TIMER_ID;
	}

	currentId++;

	remoteTimer->amx = amx;
	remoteTimer->timerId = currentId;
	remoteTimer->callback = callback;
	
	if (interval <= 0)
		remoteTimer->interval = 1;
	else
		remoteTimer->interval = interval;

	remoteTimer->destroyed = false;
	remoteTimer->repeat = repeat;

	remoteTimer->entryTime = CTime::Get()->GetTime();
	remoteTimerList.insert(std::pair<TimerID, RemoteTimerStruct *>(currentId, remoteTimer));
	return currentId;
}

TimerID CTimer::NewEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset)
{
	RemoteTimerStruct *remoteTimer = new RemoteTimerStruct;
	if (!remoteTimer) {
		CConsole::Get()->Log("cannot allocate memory");
		return INVALID_TIMER_ID;
	}

	if (currentId >= 0xFFFF) {
		delete remoteTimer;
		CConsole::Get()->Log("limit reached");
		return INVALID_TIMER_ID;
	}

	if (CPawn::Get()->Find(amx, callback) == false) {
		delete remoteTimer;
		CConsole::Get()->Log("cannot find public with name \"%s\"", callback);
		return INVALID_TIMER_ID;
	}

	currentId++;

	remoteTimer->amx = amx;
	remoteTimer->timerId = currentId;
	remoteTimer->callback = callback;

	if (interval <= 0)
		remoteTimer->interval = 1;
	else
		remoteTimer->interval = interval;

	remoteTimer->destroyed = false;
	remoteTimer->repeat = repeat;

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
				remoteTimer->params.arrays.push_back(std::pair<cell *, cell>(arr, *ptr_len));
			}
			break;
		case 's':
		case 'S':
			char *amx_string;
			amx_StrParam(amx, params[j + offset], amx_string);
			remoteTimer->params.strings.push_back(amx_string);
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
			remoteTimer->params.integers.push_back(*amx_integer);
			break;
		default:
			CConsole::Get()->Log("SetTimerEx: unknown format specifer");
			break;
		}
	}

	remoteTimer->entryTime = CTime::Get()->GetTime();
	remoteTimerList.insert(std::pair<TimerID, RemoteTimerStruct *>(currentId, remoteTimer));
	return currentId;
}

int CTimer::Kill(TimerID timerId)
{
	auto p = remoteTimerList.find(timerId);
	if (p != remoteTimerList.end()) {
		p->second->destroyed = true;
		return 1;
	}
	return 0;
}

void CTimer::KillAll(AMX *amx)
{
	for (auto p = remoteTimerList.begin(); p != remoteTimerList.end();) {
		if (p->second->amx == amx) {
			for (auto arrays : p->second->params.arrays) free(arrays.first);
			p->second->params.arrays.clear();
			p->second->params.strings.clear();
			p->second->params.integers.clear();
			delete p->second;
			remoteTimerList.erase(p++);
			continue;
		}
		p++;
	}
}

int CTimer::IsValid(int timerId)
{
	return (remoteTimerList.find(timerId) != remoteTimerList.end());
}

int CTimer::GetInterval(int timerId)
{
	auto p = remoteTimerList.find(timerId);
	if (p != remoteTimerList.end()) {
		return p->second->interval;
	}
	return -1;
}

int CTimer::SetInterval(int timerId, int interval)
{
	auto p = remoteTimerList.find(timerId);
	if (p != remoteTimerList.end()) {
		p->second->interval = (interval <= 0) ? 1 : interval;
		return 1;
	}
	return 0;
}

int CTimer::GetRemaining(int timerId)
{
	auto p = remoteTimerList.find(timerId);
	if (p != remoteTimerList.end()) {
		return (p->second->interval - (CTime::Get()->GetTime() - p->second->entryTime));
	}
	return -1;
}

void CTimer::Process()
{
	for (auto p = remoteTimerList.begin(); p != remoteTimerList.end();) {
		if (p->second->destroyed) {
			for (auto arrays : p->second->params.arrays) free(arrays.first);
			p->second->params.arrays.clear();
			p->second->params.strings.clear();
			p->second->params.integers.clear();
			delete p->second;
			remoteTimerList.erase(p++);
			continue;
		}
		if ((CTime::Get()->GetTime() - p->second->entryTime) >= p->second->interval) {
			if (CPawn::Get()->Execute(p->second->amx, p->second->callback.c_str(), p->second->params) == false)
				CConsole::Get()->Log("cannot execute callback with name \"%s\"", p->second->callback.c_str());

			if (p->second->repeat) {
				p->second->entryTime = CTime::Get()->GetTime();
				p++;
				continue;
			} else {
				for (auto arrays : p->second->params.arrays) free(arrays.first);
				p->second->params.arrays.clear();
				p->second->params.strings.clear();
				p->second->params.integers.clear();
				delete p->second;
				remoteTimerList.erase(p++);
				continue;
			}
		}
		p++;
	}
}