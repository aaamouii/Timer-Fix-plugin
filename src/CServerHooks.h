// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "CSingleton.h"
#include "plugin.h"
#include <string>
#include <map>

class CServerHooks : public CSingleton<CServerHooks>
{
public:
	CServerHooks();
	~CServerHooks();

	bool Apply(AMX* amx);
	bool Restore(AMX* amx);

	bool Checks();

private:
	AMX* m_pAMX;
	std::map<AMX_FUNCSTUB*, ucell> m_pFunctionDataStorage;
	std::map<std::string, bool> m_pNativeDataStorage;
};