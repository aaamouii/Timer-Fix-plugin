#include "kernel.h"
#include "main.h"

#include <cstdarg>
#include <iostream>

using namespace tf;
logprintf_t logprintf;

Kernel::Kernel(void **ppData)
{
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("  %s plugin v%s by %s loaded.", PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
}

void Kernel::send(const char *format, ...)
{
	va_list va_args;
	char buffer[256];
	va_start(va_args, format);
	vsnprintf(buffer, sizeof(buffer), format, va_args);
	va_end(va_args);
	return logprintf("[plugin.timerfix] %s", buffer);
}