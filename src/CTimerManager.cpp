// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "CTimerManager.h"
#include "CStorage.h"
#include "plugin.h"

extern logprintf_t logprintf;

void CTimerManager::Process()
{
	unsigned char ucFlags;
	for (std::map<cell, CTimer*>::iterator iter = CStorage::Get()->m_pTimerDataStorage.begin(); iter != CStorage::Get()->m_pTimerDataStorage.end();)
	{
		ucFlags = iter->second->GetFlags();

		if (ucFlags & 0x04)
		{
			iter++;
			continue;
		}

		iter->second->Process();

		// Update the flags
		ucFlags = iter->second->GetFlags();

		if (/*!(ucFlags & 0x01) || */(ucFlags & 0x02))
		{
			CFreeSlotManager::Get()->Remove(iter->first);
			if (iter->second)
				delete iter->second;
			//CStorage::Get()->Remove(iter->first);
			iter = CStorage::Get()->m_pTimerDataStorage.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}