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

#include "TimerManager.h"

extern logprintf_t logprintf;

TimerManager::TimerManager()
{
	m_pFreeSlotManager = std::make_unique<FreeSlotManager>();
}

TimerManager::~TimerManager()
{
	m_pFreeSlotManager.reset();
}

int32_t TimerManager::AddTimer(AMX* amx, const char* name, int32_t interval, bool repeat)
{
	int32_t id = m_pFreeSlotManager->Get();
	m_mapTimerStack.insert(std::make_pair(id, new Timer(amx, name, interval, repeat)));
	return id;
}

int32_t TimerManager::AddTimerEx(AMX* amx, const char* name, int32_t interval, bool repeat, const char* format, cell* params)
{
	int32_t id = m_pFreeSlotManager->Get();
	m_mapTimerStack.insert(std::make_pair(id, new Timer(amx, name, interval, repeat, format, params, id)));
	return id;
}

void TimerManager::Process()
{
	for (std::map<int32_t, Timer*>::iterator timer_stack_iterator = m_mapTimerStack.begin(); timer_stack_iterator != m_mapTimerStack.end();)
	{
		if (timer_stack_iterator->second->Process() == false)
		{
			m_pFreeSlotManager->Remove(timer_stack_iterator->first);
			delete timer_stack_iterator->second;
			m_mapTimerStack.erase(timer_stack_iterator);
		}
		timer_stack_iterator++;
	}
}

bool TimerManager::KillTimer(int32_t timerid)
{
	std::map<int32_t, Timer*>::iterator timer_stack_iterator = m_mapTimerStack.find(timerid);
	if (timer_stack_iterator != m_mapTimerStack.end())
	{
		timer_stack_iterator->second->m_bRepeat = false;
		return true;
	}
	return false;
}

void TimerManager::KillAll(AMX* amx)
{
	for (std::map<int32_t, Timer*>::iterator timer_stack_iterator = m_mapTimerStack.begin(); timer_stack_iterator != m_mapTimerStack.end();)
	{
		if (timer_stack_iterator->second->m_pAMX == amx)
		{
			delete timer_stack_iterator->second;
			m_mapTimerStack.erase(timer_stack_iterator++);
			continue;
		}
		timer_stack_iterator++;
	}
}

bool TimerManager::IsValidTimer(int32_t timerid)
{
	return m_mapTimerStack.find(timerid) != m_mapTimerStack.end();
}

int32_t TimerManager::GetTimerInterval(int32_t timerid)
{
	std::map<int32_t, Timer*>::iterator timer_stack_iterator = m_mapTimerStack.find(timerid);
	if (timer_stack_iterator != m_mapTimerStack.end())
		return timer_stack_iterator->second->GetInterval();
	return -1;
}

bool TimerManager::SetTimerInterval(int32_t timerid, int32_t interval)
{
	std::map<int32_t, Timer*>::iterator timer_stack_iterator = m_mapTimerStack.find(timerid);
	if (timer_stack_iterator != m_mapTimerStack.end())
	{
		timer_stack_iterator->second->SetInterval(interval);
		return true;
	}
	return false;
}

int32_t TimerManager::GetTimerRemainingTime(int32_t timerid)
{
	std::map<int32_t, Timer*>::iterator timer_stack_iterator = m_mapTimerStack.find(timerid);
	if (timer_stack_iterator != m_mapTimerStack.end())
		return timer_stack_iterator->second->GetRemainingTime();
	return -1;
}