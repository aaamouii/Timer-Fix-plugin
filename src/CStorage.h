// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "CSingleton.h"
#include "CFreeSlotManager.h"
#include "CTimer.h"
#include <map>

class CStorage : public CSingleton<CStorage>
{
public:
	std::map<cell, CTimer*>  m_pTimerDataStorage;

public:
	CStorage();
	~CStorage();

	cell Add(AMX* amx, const char* szFunction, uint32_t interval, bool repeat);
	cell Add(AMX* amx, const char* szFunction, uint32_t interval, bool repeat, const char *szFormat, cell *params);
	bool Remove(cell timerid);

	CTimer* GetById(cell timerid);

	void Clear();
};