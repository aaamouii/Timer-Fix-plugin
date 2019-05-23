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

#pragma once

#include "Logic.h"

enum E_PARAM_TYPE
{
	E_PARAM_ARRAY = 1,
	E_PARAM_STRING,
	E_PARAM_INTEGER
};

class Timer
{
private:
	struct
	{
		std::vector<E_PARAM_TYPE> m_vecParamType;
		std::vector<std::pair<cell*, cell>> m_vecArrayStack;
		std::vector<cell> m_vecIntegerStack;
		std::vector<std::string> m_vecStringStack;
		cell tmp_on_stack;
	} param_stack;

	void PushArray(AMX* amx, cell param, cell next_param);
	void PushInteger(AMX* amx, cell param);
	void PushString(AMX* amx, cell param);
	void PushID(int32_t id);

	struct
	{
		int64_t m_i64FinishTime;
		int64_t m_i64LastReferenced;
		int64_t m_i64SafeCounter;
		int32_t m_i32Interval;
	} error_counter;

public:
	AMX* m_pAMX;
	std::string m_sName;
	bool m_bRepeat;

	Timer(AMX *amx, const char* name, int32_t interval, bool repeat);
	Timer(AMX* amx, const char* name, int32_t interval, bool repeat, const char* format, cell* params, int32_t timerid);
	~Timer();

	bool Process();
	bool IsRepeat();

	int64_t CurrentTime();
	bool CheckForProcessing();
	void Reset();
	int32_t GetInterval();
	void SetInterval(int32_t interval);
	int32_t GetRemainingTime();
};