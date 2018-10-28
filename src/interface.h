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
#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "main.h"
#include "timer.h"

#include <map>

class Interface
{
private:
	std::map<AMX *, Timer> local_timer_map;

public:
	void Add(AMX *amx);
	void Remove(AMX *amx);

	void startUpdateForAll();

	int Create(AMX *amx, const char *callback, int interval, bool repeat);
	int CreateEx(AMX *amx, const char *callback, int interval, bool repeat, cell *params, const char *format, int offset);
	int Kill(AMX *amx, int timerid);
	void KillAll(AMX *amx);
	int IsValid(AMX *amx, int timerid);
	int SetInterval(AMX *amx, int timerid, int interval);
	int GetInterval(AMX *amx, int timerid);
	int GetRemaining(AMX *amx, int timerid);

};

#endif