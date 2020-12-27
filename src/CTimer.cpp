// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "plugin.h"
#include "CTimer.h"
#include "COSTime.h"
#include <algorithm>

extern logprintf_t logprintf;

CTimer::CTimer(AMX* amx, const char* szFunctionName, cell interval, bool repeat)
{
	m_functionContext.clear();
	m_pAMX = amx;

	m_functionContext.push_back(szFunctionName);
	m_iInterval = interval;
	m_ucFlags = 0;

	if (repeat)
		m_ucFlags |= 0x01;

	m_iCountOfExecutions = 0;
	m_iCurrentCountOfExecutions = 0;
	m_iDelay = 0;

	m_pFunctionArguments.vecArrays.clear();
	m_pFunctionArguments.vecDigitals.clear();
	m_pFunctionArguments.vecParamTypes.clear();
	m_pFunctionArguments.vecStrings.clear();

	m_pTimeCounter.m_i32Interval = interval;
	m_pTimeCounter.m_iFinishTime = (interval + COSTime::Get()->Current());
	m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current();
	m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
}

CTimer::CTimer(AMX* amx, const char* szFunctionName, cell interval, bool repeat, const char* szFormat, cell* params, cell timerid)
{
	m_functionContext.clear();
	m_pAMX = amx;

	m_functionContext.push_back(szFunctionName);
	m_iInterval = interval;
	m_ucFlags = 0;

	if (repeat)
		m_ucFlags |= 0x01;

	m_iCountOfExecutions = 0;
	m_iCurrentCountOfExecutions = 0;
	m_iDelay = 0;

	m_pTimeCounter.m_i32Interval = interval;
	m_pTimeCounter.m_iFinishTime = (interval + COSTime::Get()->Current());
	m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current();
	m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();

	m_pFunctionArguments.vecArrays.clear();
	m_pFunctionArguments.vecDigitals.clear();
	m_pFunctionArguments.vecParamTypes.clear();
	m_pFunctionArguments.vecStrings.clear();

	for (int i = 0, len = strlen(szFormat), j = 0; i != len; ++i, ++j)
	{
		switch (szFormat[i])
		{
		case 'a':
		case 'A':
			cell * ptr_arr, * ptr_len, * arr;
			amx_GetAddr(amx, params[j], &ptr_arr);
			amx_GetAddr(amx, params[j + 1], &ptr_len);
			arr = (cell*)malloc((*ptr_len * sizeof(cell)));
			if (arr)
			{
				memcpy(arr, ptr_arr, (*ptr_len * sizeof(cell)));
				m_pFunctionArguments.vecArrays.push_back(std::make_pair(arr, *ptr_len));
				m_pFunctionArguments.vecParamTypes.push_back(PARAM_TYPE_ARRAY);
			}
			else
			{
				logprintf("[timerfix.plugin] SetTimerEx: cannot pass array argument");
			}
			break;
		case 's':
		case 'S':
			char* str;
			amx_StrParam(amx, params[j], str);
			m_pFunctionArguments.vecStrings.push_back(str);
			m_pFunctionArguments.vecParamTypes.push_back(PARAM_TYPE_STRING);
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
			cell * amx_digital;
			amx_GetAddr(amx, params[j], &amx_digital);
			m_pFunctionArguments.vecDigitals.push_back(*amx_digital);
			m_pFunctionArguments.vecParamTypes.push_back(PARAM_TYPE_DIGITAL);
			break;
		case 'p':
		case 'P':
		case 't':
		case 'T':
			--j;
			m_pFunctionArguments.vecDigitals.push_back(timerid);
			m_pFunctionArguments.vecParamTypes.push_back(PARAM_TYPE_DIGITAL);
			break;
		default:
			logprintf("[timerfix.plugin] SetTimerEx: unknown format specifier has been passed ['%c']", szFormat[i]);
		}
	}
}

CTimer::~CTimer()
{
	m_functionContext.clear();

	m_pFunctionArguments.vecDigitals.clear();
	m_pFunctionArguments.vecParamTypes.clear();
	m_pFunctionArguments.vecStrings.clear();

	if (m_pFunctionArguments.vecArrays.empty() == false)
	{
		for (size_t i = 0; i < m_pFunctionArguments.vecArrays.size(); i++)
			free(m_pFunctionArguments.vecArrays.at(i).first);
	}
}

void CTimer::Process()
{
	if (m_ucFlags & 0x10)
	{
		m_pTimeCounter.m_iLastReferenced += (COSTime::Get()->Current() - m_pTimeCounter.m_iSafeCounter);
		m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();

		if (m_pTimeCounter.m_iLastReferenced >= m_pTimeCounter.m_iFinishTime)
		{
			m_ucFlags &= ~(0x10);
			m_pTimeCounter.m_iFinishTime = (m_pTimeCounter.m_i32Interval + COSTime::Get()->Current());
			m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current();
			m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
		}
	}
	else
	{
		m_pTimeCounter.m_iLastReferenced += (COSTime::Get()->Current() - m_pTimeCounter.m_iSafeCounter);
		m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
	}

	if (m_pTimeCounter.m_iLastReferenced >= m_pTimeCounter.m_iFinishTime)
	{
		int index;
		for (auto& function : m_functionContext)
		{
			if (amx_FindPublic(m_pAMX, function.c_str(), &index) == AMX_ERR_NONE)
			{
				m_pFunctionArguments.trash = -1;
				if (m_pFunctionArguments.vecParamTypes.empty() == false)
				{
					cell tmp = 0;

					size_t array_iter = m_pFunctionArguments.vecArrays.size();
					size_t string_iter = m_pFunctionArguments.vecStrings.size();
					size_t digitals_iter = m_pFunctionArguments.vecDigitals.size();
					size_t type_iter = m_pFunctionArguments.vecParamTypes.size();
					while (--type_iter != -1)
					{
						switch (m_pFunctionArguments.vecParamTypes[type_iter])
						{
						case PARAM_TYPE_ARRAY:
							array_iter--;
							if (amx_PushArray(m_pAMX, &tmp, 0, m_pFunctionArguments.vecArrays[array_iter].first, m_pFunctionArguments.vecArrays[array_iter].second) != AMX_ERR_NONE)
							{
								logprintf("[timerfix.plugin] Sorry, but I couldn't push array for function '%s'", function.c_str());
							}
							else
							{
								if (m_pFunctionArguments.trash == -1)
									m_pFunctionArguments.trash = tmp;
							}
							break;
						case PARAM_TYPE_DIGITAL:
							if (amx_Push(m_pAMX, m_pFunctionArguments.vecDigitals[--digitals_iter]) != AMX_ERR_NONE)
							{
								logprintf("[timerfix.plugin] Sorry, but I couldn't push integer for function '%s'", function.c_str());
							}
							break;
						case PARAM_TYPE_STRING:
							if (amx_PushString(m_pAMX, &tmp, 0, m_pFunctionArguments.vecStrings[--string_iter].c_str(), 0, 0) != AMX_ERR_NONE)
							{
								logprintf("[timerfix.plugin] Sorry, but I couldn't push string for function '%s'", function.c_str());
							}
							else
							{
								if (m_pFunctionArguments.trash == -1)
									m_pFunctionArguments.trash = tmp;
							}
							break;
						default:
							// It seems like sometimes there's some trash and because of it timers stop
							//return false;
							break;
						}
					}
				}

				cell retval;
				if (amx_Exec(m_pAMX, &retval, index) != AMX_ERR_NONE)
				{
					logprintf("[timerfix.plugin] Sorry, but I couldn't execute function '%s'", function.c_str());
				}

				if (m_pFunctionArguments.vecParamTypes.empty() == false)
				{
					if (m_pFunctionArguments.trash != -1)
						amx_Release(m_pAMX, m_pFunctionArguments.trash);
				}

				if (m_ucFlags & 0x08)
				{
					m_iCurrentCountOfExecutions++;
					if (m_iCurrentCountOfExecutions >= m_iCountOfExecutions)
						Remove();

					m_pTimeCounter.m_iFinishTime = (m_pTimeCounter.m_i32Interval + COSTime::Get()->Current());
					m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current();
					m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
				}
				else
				{
					if (m_ucFlags & 0x01)
					{
						m_pTimeCounter.m_iFinishTime = (m_pTimeCounter.m_i32Interval + COSTime::Get()->Current());
						m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current();
						m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
					}
					else
					{
						Remove();
					}
				}
			}
			else
			{
				logprintf("[timerfix.plugin] Sorry, but I couldn't find function '%s'", function.c_str());
			}
		}
	}
}

unsigned char CTimer::GetFlags()
{
	return m_ucFlags;
}

void CTimer::Remove()
{
	m_ucFlags |= 0x02;
}

void CTimer::Pause()
{
	m_ucFlags |= 0x04;
}

void CTimer::Continue()
{
	m_ucFlags &= ~(0x04);
	m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current() - (m_pTimeCounter.m_iFinishTime - m_pTimeCounter.m_iLastReferenced);
	m_pTimeCounter.m_iFinishTime = COSTime::Get()->Current() + m_pTimeCounter.m_i32Interval;
	m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
}

void CTimer::SetInterval(cell interval)
{
	m_iInterval = interval;
	m_pTimeCounter.m_i32Interval = interval;
}

cell CTimer::GetInterval()
{
	return m_iInterval;
}

void CTimer::AddHandler(const char* handler)
{
	std::list<std::string>::iterator iter = std::find(m_functionContext.begin(), m_functionContext.end(), handler);
	if (iter == m_functionContext.end())
	{
		m_functionContext.push_back(handler);
	}
}

void CTimer::RemoveHandler(const char* handler)
{
	std::list<std::string>::iterator iter = std::find(m_functionContext.begin(), m_functionContext.end(), handler);
	if (iter != m_functionContext.end())
	{
		m_functionContext.erase(iter);
	}
}

cell CTimer::GetRemainingTime()
{
	return (cell)(m_pTimeCounter.m_iFinishTime - m_pTimeCounter.m_iLastReferenced);
}

void CTimer::SetCount(int count)
{
	m_ucFlags |= 0x08;
	m_iCurrentCountOfExecutions = 0;
	m_iCountOfExecutions = count;
}

void CTimer::SetDelay(cell delay)
{
	m_ucFlags |= 0x10;
	m_iDelay = delay;

	m_pTimeCounter.m_iFinishTime = (delay + COSTime::Get()->Current());
	m_pTimeCounter.m_iLastReferenced = COSTime::Get()->Current();
	m_pTimeCounter.m_iSafeCounter = COSTime::Get()->Current();
}