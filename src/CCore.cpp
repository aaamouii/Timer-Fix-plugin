#include "main.h"

CCore::CCore()
{
	m_pTimer = new CTimer();
	m_pNatives = new CNatives();
	m_pCallback = new CCallback();
}

CCore::~CCore()
{
	DELETE_SAFE(m_pTimer);
	DELETE_SAFE(m_pNatives);
	DELETE_SAFE(m_pCallback);
}

CTimer *CCore::getTimer()
{
	return m_pTimer;
}

CNatives *CCore::getNatives()
{
	return m_pNatives;
}

CCallback *CCore::getCallback()
{
	return m_pCallback;
}