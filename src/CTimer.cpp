#include "main.h"

extern logprintf_t logprintf;
extern CCore *m_pCore;
/*
//	List of all timers
*/
CTimer::amx_t timer_list;
int gTimerID = 0;

bool CTimer::Init(AMX *amx)
{
	timer_list.insert(pair<AMX *, timers_t>(amx, timers_t{}));
	return true;
}

bool CTimer::UnInit(AMX *amx)
{
	KillAllTimers();
	timer_list.clear();
	return true;
}

TIMERID CTimer::New(AMX *amx, const char *callback, int interval, bool repeat)
{
	amx_t::iterator iter = timer_list.find(amx);
	if (iter != timer_list.end())
	{
		gTimerID++;

		timer_t *timer;
		ALLOCATE_SAFE("SetTimer", timer, timer_t);
		timer->callback_name = callback;
		timer->repeat = repeat;
		timer->interval = interval;
		timer->is_destroyed = false;
		timer->start_time_point = chrono::steady_clock::now();

		iter->second.insert(pair<int, timer_t*>(gTimerID, timer));
		return gTimerID;
	}
	return INVALID_TIMER_ID;
}

TIMERID CTimer::NewEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset)
{
	amx_t::iterator iter = timer_list.find(amx);
	if (iter != timer_list.end())
	{
		gTimerID++;

		timer_t *timer;
		ALLOCATE_SAFE("SetTimerEx", timer, timer_t);
		timer->callback_name = callback;
		timer->repeat = repeat;
		timer->interval = interval;
		timer->is_destroyed = false;
		timer->start_time_point = chrono::steady_clock::now();
		
		int j = strlen(format);
		while (j)
		{
			j--;
			switch (format[j])
			{
			case 'a':
			case 'A':
				cell *ptr_arr, *ptr_len, *arr;
				amx_GetAddr(amx, params[j + offset], &ptr_arr);
				amx_GetAddr(amx, params[j + offset + 1], &ptr_len);
				arr = (cell*)malloc((*ptr_len * sizeof(cell)));
				if (arr != NULL)
				{
					memcpy(arr, ptr_arr, (*ptr_len * sizeof(cell)));
					timer->parameters.arrays.push_back(pair<cell *, cell>(arr, *ptr_len));
				}
				break;
			case 's':
			case 'S':
				char *amx_string;
				amx_StrParam(amx, params[j + offset], amx_string);
				timer->parameters.strings.push_back(amx_string);
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
				cell *amx_integer; // not only integer, but... :/
				amx_GetAddr(amx, params[j + offset], &amx_integer);
				timer->parameters.integers.push_back(*amx_integer);
				break;
			default:
				logprintf("[Timer Fix plugin] SetTimerEx: unknown format specifer");
			}
		}

		iter->second.insert(pair<int, timer_t*>(gTimerID, timer));
		return gTimerID;
	}
	return INVALID_TIMER_ID;
}

void CTimer::KillAllTimers()
{
	for (amx_t::iterator iter = timer_list.begin(); iter != timer_list.end(); iter++)
	{
		for (timers_t::iterator timer_iter = iter->second.begin(); timer_iter != iter->second.end(); timer_iter++)
		{
			for (auto arrays : timer_iter->second->parameters.arrays)
			{
				free(arrays.first);
			}
			DELETE_SAFE(timer_iter->second);
			iter->second.erase(timer_iter);
		}
		timer_list.erase(iter);
	}
}

bool CTimer::Kill(AMX *amx, int timerid)
{
	amx_t::iterator iter = timer_list.find(amx);
	if (iter != timer_list.end())
	{
		timers_t::iterator timer_iter = iter->second.find(timerid);
		if (timer_iter != iter->second.end())
		{
			timer_iter->second->is_destroyed = true;
			return true;
		}
	}
	return false;
}

bool CTimer::SetInterval(AMX *amx, int timerid, int interval)
{
	amx_t::iterator iter = timer_list.find(amx);
	if (iter != timer_list.end())
	{
		timers_t::iterator timer_iter = iter->second.find(timerid);
		if (timer_iter != iter->second.end())
		{
			timer_iter->second->interval = interval;
			return true;
		}
	}
	return false;
}

int CTimer::GetInterval(AMX *amx, int timerid)
{
	amx_t::iterator iter = timer_list.find(amx);
	if (iter != timer_list.end())
	{
		timers_t::iterator timer_iter = iter->second.find(timerid);
		if (timer_iter != iter->second.end())
		{
			return timer_iter->second->interval;
		}
	}
	return -1;
}

bool CTimer::IsValid(AMX *amx, int timerid)
{
	amx_t::iterator iter = timer_list.find(amx);
	if (iter != timer_list.end())
	{
		timers_t::iterator timer_iter = iter->second.find(timerid);
		if (timer_iter != iter->second.end())
		{
			return true;
		}
	}
	return false;
}

TIMERID CTimer::GetIdLastTimer()
{
	return gTimerID;
}

void CTimer::Process()
{
	/*
	//	damn it
	*/
	cell tmp;
	cell retval;

	for (amx_t::iterator iter = timer_list.begin(); iter != timer_list.end(); iter++)
	{
		for (timers_t::iterator timer_iter = iter->second.begin(); timer_iter != iter->second.end();)
		{
			if (timer_iter->second->is_destroyed)
			{
				for (auto arrays : timer_iter->second->parameters.arrays)
				{
					free(arrays.first);
				}
				DELETE_SAFE(timer_iter->second);
				iter->second.erase(timer_iter++);
				continue;
			}
			if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - timer_iter->second->start_time_point).count() >= timer_iter->second->interval)
			{
				/*
				//	Push parameters
				*/
				for (auto arrays : timer_iter->second->parameters.arrays)
				{
					if (amx_PushArray(iter->first, &tmp, NULL, arrays.first, arrays.second) != AMX_ERR_NONE)
					{
						logprintf("[Timer Fix plugin] cannot push array");
					}
				}
				for (auto strings : timer_iter->second->parameters.strings)
				{
					if (amx_PushString(iter->first, &tmp, NULL, strings, NULL, NULL) != AMX_ERR_NONE)
					{
						logprintf("[Timer Fix plugin] cannot push string");
					}
				}
				for (auto integers : timer_iter->second->parameters.integers)
				{
					if (amx_Push(iter->first, integers) != AMX_ERR_NONE)
					{
						logprintf("[Timer Fix plugin] cannot push integer");
					}
				}

				/*
				//	execute callback
				*/
				int callback_index = m_pCore->getCallback()->FindPublic(iter->first, timer_iter->second->callback_name.c_str());
				if (callback_index != -1)
				{
					if (amx_Exec(iter->first, &retval, callback_index) != AMX_ERR_NONE)
					{
						logprintf("[Timer Fix plugin] %s: cannot execute callback", timer_iter->second->callback_name);
					}
				}
				else
				{
					logprintf("[Timer Fix plugin] %s: cannot find callback", timer_iter->second->callback_name);
				}

				/*
				//	check repeat
				*/
				if (timer_iter->second->repeat)
				{
					timer_iter->second->start_time_point = chrono::steady_clock::now();
				}
				else
				{
					for (auto arrays : timer_iter->second->parameters.arrays)
					{
						free(arrays.first);
					}
					DELETE_SAFE(timer_iter->second);
					iter->second.erase(timer_iter++);
					continue;
				}
			}
			timer_iter++;
		}
	}
}