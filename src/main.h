#pragma once

/*
//	Include files
*/
#include <iostream>
#include <deque>
#include <map>
#include <unordered_map>
#include <chrono>
#include <string.h>

using namespace std;

/*
//	Plugin macroses
*/
#define DELETE_SAFE(n)				if(n) delete n
#define ALLOCATE_SAFE(f, n, c)		n = new c(); if(!n) logprintf("[Timer Fix plugin] %s: cannot allocate memory", f)
#define TIMERID						unsigned short
#define INVALID_TIMER_ID			0xFFFF

/*
//	Plugin SDK
*/
#include "amx/amx.h"
#include "plugincommon.h"

/*
//	Other libraries
*/
#include "cool-amx.h"

/*
//
*/
#include "CCallback.h"
#include "CTimer.h"
#include "CNatives.h"
#include "CCore.h"

typedef void(*logprintf_t)(char* format, ...);