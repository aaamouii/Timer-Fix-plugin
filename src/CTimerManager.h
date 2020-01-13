// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "CSingleton.h"

class CTimerManager : public CSingleton<CTimerManager>
{
public:
	void Process();
};