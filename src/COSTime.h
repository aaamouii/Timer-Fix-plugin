// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

#include "CSingleton.h"
#include <cstdint>

class COSTime : public CSingleton<COSTime>
{
public:
	uint64_t Current();
};