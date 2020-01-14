// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "CStorage.h"

extern logprintf_t logprintf;

CStorage::CStorage()
{
	m_pTimerDataStorage.clear();
}

CStorage::~CStorage()
{
	if (m_pTimerDataStorage.empty() == false)
	{
		for (std::map<cell, CTimer*>::iterator iter = m_pTimerDataStorage.begin(); iter != m_pTimerDataStorage.end(); iter++)
		{
			if (iter->second)
				delete iter->second;
			iter->second = nullptr;
		}
	}
	m_pTimerDataStorage.clear();
}

cell CStorage::Add(AMX* amx, const char* szFunction, uint32_t interval, bool repeat)
{
	cell id = CFreeSlotManager::Get()->GetId();

	if (m_pTimerDataStorage.find(id) != m_pTimerDataStorage.end())
	{
		logprintf("[timerfix.plugin] Oops! Seems like ID already used :(");
		return -1;
	}

	CTimer* pTimer = new CTimer(amx, szFunction, interval, repeat);
	m_pTimerDataStorage.insert(std::make_pair(id, pTimer));
	return id;
}

cell CStorage::Add(AMX* amx, const char* szFunction, uint32_t interval, bool repeat, const char* szFormat, cell* params)
{
	cell id = CFreeSlotManager::Get()->GetId();

	if (m_pTimerDataStorage.find(id) != m_pTimerDataStorage.end())
	{
		logprintf("[timerfix.plugin] Oops! Seems like ID already used :(");
		return -1;
	}

	CTimer* pTimer = new CTimer(amx, szFunction, interval, repeat, szFormat, params, id);
	m_pTimerDataStorage.insert(std::make_pair(id, pTimer));
	return id;
}

bool CStorage::Remove(cell timerid)
{
	std::map<cell, CTimer*>::iterator iter = m_pTimerDataStorage.find(timerid);
	if (iter != m_pTimerDataStorage.end())
	{
		if (iter->second)
			delete iter->second;
		iter->second = nullptr;
		m_pTimerDataStorage.erase(iter);
		return true;
	}
	return false;
}

CTimer* CStorage::GetById(cell timerid)
{
	std::map<cell, CTimer*>::iterator iter = m_pTimerDataStorage.find(timerid);
	if (iter != m_pTimerDataStorage.end())
	{
		return iter->second;
	}
	return nullptr;
}

void CStorage::Clear()
{
	if (m_pTimerDataStorage.empty() == false)
	{
		for (std::map<cell, CTimer*>::iterator iter = m_pTimerDataStorage.begin(); iter != m_pTimerDataStorage.end(); iter++)
		{
			if (iter->second)
				delete iter->second;
			iter->second = nullptr;
		}
	}
	m_pTimerDataStorage.clear();
}