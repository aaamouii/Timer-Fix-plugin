#pragma once

class CCore
{
private:
	CTimer *m_pTimer;
	CNatives *m_pNatives;
	CCallback *m_pCallback;

public:
	CCore();
	~CCore();

	CTimer *getTimer();
	CNatives *getNatives();
	CCallback *getCallback();
};