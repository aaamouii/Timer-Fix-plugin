#include "plugin.h"
#include "main.h"
#include "precompilied.h"

logprintf_t logprintf;

void Plugin::Present(void **ppData)
{
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("  %s plugin v%s by %s loaded.", PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
}

void Plugin::Printf(const char *format, ...)
{
	char buffer[256];
	va_list va_args;
	va_start(va_args, format);
	vsnprintf(buffer, sizeof(buffer), format, va_args);
	va_end(va_args);

	return logprintf("[timerfix.plugin] %s", buffer);
}