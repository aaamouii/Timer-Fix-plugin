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
