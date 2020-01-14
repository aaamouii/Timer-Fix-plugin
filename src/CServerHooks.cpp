// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#include "CServerHooks.h"
#include "natives.h"

#define GETNAME_SAFE(hdr,entry) \
		(((hdr)->defsize==sizeof(AMX_FUNCSTUBNT)) ? \
		(char *)((unsigned char*)(hdr) + (unsigned)((AMX_FUNCSTUBNT*)(entry))->nameofs) : \
		((AMX_FUNCSTUB*)(entry))->name)

#define NATIVESIZE_SAFE(hdr,field,nextfield) \
	(unsigned)(((hdr)->nextfield - (hdr)->field) / (hdr)->defsize)

#define GETNATIVE_SAFE(hdr,table,index) \
	(AMX_FUNCSTUB *)((unsigned char*)(hdr) + (unsigned)(hdr)->table + (unsigned)index*(hdr)->defsize)

extern logprintf_t logprintf;

CServerHooks::CServerHooks()
{
	m_pFunctionDataStorage.clear();
	m_pNativeDataStorage.clear();

	m_pNativeDataStorage.insert(std::make_pair("SetTimer", false));
	m_pNativeDataStorage.insert(std::make_pair("SetTimerEx", false));
	m_pNativeDataStorage.insert(std::make_pair("KillTimer", false));
}

CServerHooks::~CServerHooks()
{
	m_pFunctionDataStorage.clear();
	m_pNativeDataStorage.clear();
}

bool CServerHooks::Apply(AMX* amx)
{
	if (!amx && !m_pAMX)
		return false;

	if (amx)
		m_pAMX = amx;

	AMX_HEADER* pHeader = reinterpret_cast<AMX_HEADER*>(m_pAMX->base);
	AMX_FUNCSTUB* pFunc;
	if (pHeader)
	{
		for (int i = 0, num = NATIVESIZE_SAFE(pHeader, natives, libraries); i != num; ++i)
		{
			/*pFunc = reinterpret_cast<AMX_FUNCSTUB*>(((unsigned char*)pHeader + pHeader->natives + i * pHeader->defsize));*/
			pFunc = GETNATIVE_SAFE(pHeader, natives, i);
			if (pFunc)
			{
				//std::map<std::string, bool>::iterator iter = m_pNativeDataStorage.find(pFunc->name);
				std::map<std::string, bool>::iterator iter = m_pNativeDataStorage.find(GETNAME_SAFE(pHeader, pFunc));
				if (iter != m_pNativeDataStorage.end())
				{
					// Not used in current version.
					//m_pFunctionDataStorage.insert(std::make_pair(pFunc, pFunc->address));
					iter->second = true;
					if (iter->first == "SetTimer")
						pFunc->address = (ucell)n_SetTimer;
					else if (iter->first == "SetTimerEx")
						pFunc->address = (ucell)n_SetTimerEx;
					else if (iter->first == "KillTimer")
						pFunc->address = (ucell)n_KillTimer;
				}
			}
		}
	}

	return true;
}

bool CServerHooks::Restore(AMX* amx)
{
	if (!amx && !m_pAMX)
		return false;

	if (amx)
		m_pAMX = amx;
	
	for (std::map<AMX_FUNCSTUB*, ucell>::iterator iter = m_pFunctionDataStorage.begin(); iter != m_pFunctionDataStorage.end();)
	{
		std::map<std::string, bool>::iterator inviter = m_pNativeDataStorage.find(GETNAME_SAFE(reinterpret_cast<AMX_HEADER*>(m_pAMX->base), iter->first));
		if (inviter != m_pNativeDataStorage.end())
		{
			if (inviter->second)
			{
				iter->first->address = iter->second;
				inviter->second = false;
				m_pFunctionDataStorage.erase(iter++);
				continue;
			}
		}

		iter++;
	}

	return true;
}

bool CServerHooks::Checks()
{
	bool bResult = true;

	for (std::map<std::string, bool>::iterator iter = m_pNativeDataStorage.begin(); iter != m_pNativeDataStorage.end(); iter++)
	{
		if (iter->second == false/* && m_pFunctionDataStorage.find(iter->first.c_str()) == m_pFunctionDataStorage.end()*/)
		{
			logprintf("[timerfix.plugin] error: hook on native '%s' has not been installed.", iter->first.c_str());
			// Let user to know all problems
			bResult = false;
		}
	}

	return bResult;
}