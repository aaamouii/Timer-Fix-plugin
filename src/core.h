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
#ifndef CORE_H_
#define CORE_H_

/// Internal includes
/// ----------------------------
#include "internal.h"
#include "interface.h"
#include "natives.h"
#include "hook.h"
/// ----------------------------

/// External includes
/// ----------------------------
#include <lwm/pointer.h>
/// ----------------------------

using namespace LWM;

class Core
{
private:
	local_ptr<Internal> p_internal;
	local_ptr<Interface> p_interface;
	local_ptr<Natives> p_natives;
	local_ptr<Hook> p_hook;

public:

	Core(void *logprintf);
	~Core();

	local_ptr<Internal> getInternal();
	local_ptr<Interface> getInterface();
	local_ptr<Natives> getNatives();
	local_ptr<Hook> getHook();

};

#endif
