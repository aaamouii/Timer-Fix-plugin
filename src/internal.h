#ifndef INTERNAL_H_
#define INTERNAL_H_

/// Internal includes
/// ---------------------------

#include "main.h"

/// ---------------------------

/// Local types
typedef void(*logprintf_t)(const char *format, ...);

class Internal
{
protected:
	logprintf_t logprintf;

public:
	Internal(void *logprintf);

	void Output(const char *format, ...);
	void Log(const char *format, ...);

};

#endif