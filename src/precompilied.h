#ifndef PRECOMPILIED_H_
#define PRECOMPILIED_H_

#include <stdarg.h>
#include <set>
#include <deque>
#include <string.h>

#include "lib/amx/amx.h"
#include "lib/amx/amx2.h"
#include "lib/plugincommon.h"

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/chrono.hpp>

typedef void(*logprintf_t)(const char *format, ...);

#endif