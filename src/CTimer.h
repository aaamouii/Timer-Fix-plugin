// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "plugin.h"
#include <string>
#include <vector>
#include <list>

enum eParamType
{
	PARAM_TYPE_ARRAY = 1,
	PARAM_TYPE_STRING,
	PARAM_TYPE_DIGITAL,
};

class CTimer
{
private:
	AMX* m_pAMX;
	std::list<std::string> m_functionContext;

	/*
	 * 0x01 - repeat
	 * 0x02 - removed
	 * 0x04 - paused
	 * 0x08 - count set
	 * 0x10 - delayed
	 */
	unsigned char m_ucFlags;

	cell m_iInterval;

	cell m_iDelay;
	int m_iCountOfExecutions;
	int m_iCurrentCountOfExecutions;

	struct FunctionArgumentsStruct
	{
		std::vector<eParamType> vecParamTypes;
		std::vector<std::pair<cell*, cell>> vecArrays;
		std::vector<cell> vecDigitals;
		std::vector<std::string> vecStrings;

		cell trash;
	};
	FunctionArgumentsStruct m_pFunctionArguments;

	struct TimeCounterStruct
	{
		uint64_t m_iFinishTime;
		uint64_t m_iLastReferenced;
		uint64_t m_iSafeCounter;
		uint32_t m_i32Interval;
	};
	TimeCounterStruct m_pTimeCounter;

public:
	CTimer(AMX* amx, const char* szFunctionName, cell interval, bool repeat);
	CTimer(AMX* amx, const char* szFunctionName, cell interval, bool repeat, const char* szFormat, cell* params, cell timerid);
	~CTimer();

	void Process();

	unsigned char GetFlags();
	void Remove();

	void Pause();
	void Continue();

	void SetInterval(cell interval);
	cell GetInterval();

	void AddHandler(const char* handler);
	void RemoveHandler(const  char* handler);

	cell GetRemainingTime();
	void SetCount(int count);
	void SetDelay(cell delay);
};