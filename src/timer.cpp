/*
	MIT License
	Copyright (c) 2018-2019 Kash Cherry
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
#include "PawnString.h"

extern logprintf_t logprintf;

Timer::Timer(AMX *amx, const char* name, int32_t interval, bool repeat)
{
	m_pAMX = amx;
	m_sName = name;
	m_bRepeat = repeat;

	error_counter.m_i32Interval = interval;
	error_counter.m_i64FinishTime = (interval + CurrentTime());
	error_counter.m_i64LastReferenced = CurrentTime();
	error_counter.m_i64SafeCounter = CurrentTime();
}

Timer::Timer(AMX* amx, const char* name, int32_t interval, bool repeat, const char* format, cell* params, int32_t timerid)
{
	m_pAMX = amx;
	m_sName = name;
	m_bRepeat = repeat;

	for(int i = 0, len = strlen(format), j = 0; i != len; ++i, ++j)
	{
		switch (format[i])
		{
		case 'a':
		case 'A':
			PushArray(amx, params[j], params[j + 1]);
			break;
		case 's':
		case 'S':
			PushString(amx, params[j]);
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
		case 'x':
		case 'X':
			PushInteger(amx, params[j]);
			break;
		case 'p':
		case 'P':
		case 't':
		case 'T':
			--j;
			PushID(timerid);
			break;
		default:
			logprintf("[timerfix.plugin] SetTimerEx: unknown format specifier has been passed.");
		}
	}

	error_counter.m_i32Interval = interval;
	error_counter.m_i64FinishTime = (interval + CurrentTime());
	error_counter.m_i64LastReferenced = CurrentTime();
	error_counter.m_i64SafeCounter = CurrentTime();
}

Timer::~Timer()
{
	//m_pTimeLogic.reset();
	m_sName.clear();
	for (auto array_iter : param_stack.m_vecArrayStack)
		free(array_iter.first);
	param_stack.m_vecArrayStack.clear();
	param_stack.m_vecIntegerStack.clear();
	param_stack.m_vecParamType.clear();
	param_stack.m_vecStringStack.clear();
	param_stack.tmp_on_stack = -1;
}

bool Timer::Process()
{
	if (CheckForProcessing())
	{
		int index;
		if (amx_FindPublic(m_pAMX, m_sName.c_str(), &index) == AMX_ERR_NONE)
		{
			param_stack.tmp_on_stack = -1;
			if (param_stack.m_vecParamType.empty() == false)
			{
				size_t array_iter = param_stack.m_vecArrayStack.size(), string_iter = param_stack.m_vecStringStack.size(), integer_iter = param_stack.m_vecIntegerStack.size(), type_iter = param_stack.m_vecParamType.size();
				cell tmp;

				while (--type_iter != -1)
				{
					switch (param_stack.m_vecParamType[type_iter])
					{
					case E_PARAM_ARRAY:
						array_iter--;
						if (amx_PushArray(m_pAMX, &tmp, 0, param_stack.m_vecArrayStack[array_iter].first, param_stack.m_vecArrayStack[array_iter].second) != AMX_ERR_NONE)
						{
							logprintf("error while processing timer: cannot push array");
							return false;
						}

						if (param_stack.tmp_on_stack == -1)
							param_stack.tmp_on_stack = tmp;
						break;
					case E_PARAM_INTEGER:
						if (amx_Push(m_pAMX, param_stack.m_vecIntegerStack[--integer_iter]) != AMX_ERR_NONE)
						{
							logprintf("error while processing timer: cannot push integer");
							return false;
						}
						break;
					case E_PARAM_STRING:
						if (amx_PushString(m_pAMX, &tmp, 0, param_stack.m_vecStringStack[--string_iter].c_str(), 0, 0) != AMX_ERR_NONE)
						{
							logprintf("error while procesing timer: cannot push string");
							return false;
						}

						if (param_stack.tmp_on_stack == -1)
							param_stack.tmp_on_stack = tmp;
						break;
					default:
						// WTF UNKNOWN PARAM HAS BEEN PASSED?
						return false;
					}
				}
			}

			cell retval;
			if (amx_Exec(m_pAMX, &retval, index) != AMX_ERR_NONE)
			{
				logprintf("error while processing timer: cannot execute public '%s'", m_sName.c_str());
				return false;
			}

			if (param_stack.tmp_on_stack != -1)
				amx_Release(m_pAMX, param_stack.tmp_on_stack);

			if (m_bRepeat)
			{
				Reset();
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void Timer::PushArray(AMX* amx, cell param, cell next_param)
{
	cell* ptr_arr, * ptr_len, * arr;
	amx_GetAddr(amx, param, &ptr_arr);
	amx_GetAddr(amx, next_param, &ptr_len);
	arr = (cell*)malloc((*ptr_len * sizeof(cell)));
	if (arr)
	{
		memcpy(arr, ptr_arr, (*ptr_len * sizeof(cell)));
		param_stack.m_vecArrayStack.push_back(std::make_pair(arr, *ptr_len));
		param_stack.m_vecParamType.push_back(E_PARAM_ARRAY);
	}
	else
		logprintf("[timerfix.plugin] %s: cannot pass array argument", __FUNCTION__);
}

void Timer::PushInteger(AMX* amx, cell param)
{
	cell* amx_integer;
	amx_GetAddr(amx, param, &amx_integer);
	param_stack.m_vecIntegerStack.push_back(*amx_integer);
	param_stack.m_vecParamType.push_back(E_PARAM_INTEGER);
}

void Timer::PushID(int32_t id)
{
	param_stack.m_vecIntegerStack.push_back(id);
	param_stack.m_vecParamType.push_back(E_PARAM_INTEGER);
}

void Timer::PushString(AMX* amx, cell param)
{
	PawnString amx_string(amx, param);
	if (amx_string.get_instance().empty() == false)
	{
		param_stack.m_vecStringStack.push_back(amx_string.get_instance().c_str());
		param_stack.m_vecParamType.push_back(E_PARAM_STRING);
	}
	else
		logprintf("[timerfix.plugin] %s: cannot pass string argument", __FUNCTION__);
}

int64_t Timer::CurrentTime()
{
	int64_t curTime{};
#ifdef _WIN32
	LARGE_INTEGER liCount;
	LARGE_INTEGER liFrec;

	QueryPerformanceFrequency(&liFrec);
	QueryPerformanceCounter(&liCount);

	__int64 quotient, remainder;
	quotient = ((liCount.QuadPart) / liFrec.QuadPart);
	remainder = ((liCount.QuadPart) % liFrec.QuadPart);
	curTime = quotient * 1000000 + (remainder * 1000000 / liFrec.QuadPart);
#else
	timeval timeVal;
	gettimeofday(&timeVal, 0);

	curTime = static_cast<int64_t>((timeVal.tv_sec) * 1000000 + (timeVal.tv_usec));
#endif
	return (curTime / 1000);
}

bool Timer::CheckForProcessing()
{
	error_counter.m_i64LastReferenced += (CurrentTime() - error_counter.m_i64SafeCounter);
	error_counter.m_i64SafeCounter = CurrentTime();
	return (error_counter.m_i64LastReferenced >= error_counter.m_i64FinishTime);
}

void Timer::Reset()
{
	error_counter.m_i64FinishTime = (error_counter.m_i32Interval + CurrentTime());
	error_counter.m_i64LastReferenced = CurrentTime();
	error_counter.m_i64SafeCounter = CurrentTime();
}

int32_t Timer::GetInterval()
{
	return error_counter.m_i32Interval;
}

void Timer::SetInterval(int32_t interval)
{
	error_counter.m_i32Interval = interval;
}

int32_t Timer::GetRemainingTime()
{
	return static_cast<int32_t>(error_counter.m_i64FinishTime - error_counter.m_i64LastReferenced);
}