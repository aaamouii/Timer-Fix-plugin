/// Internal includes
/// -----------------------

#include "internal.h"

/// -----------------------

/// External includes
/// -----------------------

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

/// -----------------------

Internal::Internal(void *logprintf)
{
	this->logprintf = (logprintf_t)logprintf;
	assert(this->logprintf != nullptr);
}

void Internal::Output(const char *format, ...)
{
	char buffer[256];
	va_list va_args;
	va_start(va_args, format);
	vsnprintf(buffer, sizeof(buffer), format, va_args);
	va_end(va_args);
	return logprintf(buffer);
}

void Internal::Log(const char *format, ...)
{
	char buffer[256];
	va_list va_args;
	va_start(va_args, format);
	vsnprintf(buffer, sizeof(buffer), format, va_args);
	va_end(va_args);
	return logprintf("[plugin.timerfix] %s", buffer);
}