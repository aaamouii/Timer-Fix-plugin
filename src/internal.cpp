/*
	MIT License

	Copyright (c) 2018 Kash Cherry

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
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