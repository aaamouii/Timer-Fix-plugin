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
#include "Console.h"

logprintf_t _logprintf;
LWM::local_ptr<CConsole> ConsoleInstance;

void CConsole::Initialize(logprintf_t logprintf)
{
	ConsoleInstance.reset(new CConsole);
	_logprintf = logprintf;
}

void CConsole::Destroy()
{
	ConsoleInstance.reset();
}

LWM::local_ptr<CConsole> CConsole::Get()
{
	return ConsoleInstance;
}

void CConsole::Output(const char *format, ...)
{
	char buffer[512];
	va_list va_args;
	va_start(va_args, format);
	vsnprintf(buffer, sizeof(buffer), format, va_args);
	va_end(va_args);
	return _logprintf(buffer);
}

void CConsole::Log(const char *format, ...)
{
	char buffer[512];
	va_list va_args;
	va_start(va_args, format);
	vsnprintf(buffer, sizeof(buffer), format, va_args);
	va_end(va_args);
	return _logprintf("[timerfix.plugin] %s", buffer);
}