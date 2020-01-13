// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "CSingleton.h"
#include "plugin.h"
#include <queue>
#include <cstdint>

class CFreeSlotManager : public CSingleton<CFreeSlotManager>
{
private:
	std::queue<cell> m_queueFreeSlots;
	cell m_iHighestId;

public:
	CFreeSlotManager()
	{
		m_iHighestId = 1;
	}

	~CFreeSlotManager()
	{
		while (!m_queueFreeSlots.empty()) m_queueFreeSlots.pop();
	}

	cell GetId()
	{
		cell id = m_iHighestId;
		if (!m_queueFreeSlots.empty())
		{
			id = m_queueFreeSlots.front();
			m_queueFreeSlots.pop();
		}
		else
		{
			m_iHighestId++;
		}
		return id;
	}

	void Remove(cell id)
	{
		m_queueFreeSlots.push(id);
	}
};