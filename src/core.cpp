#include "core.h"

local_ptr<Core> core;

Core::Core(void *logprintf)
{
	p_internal.reset(new Internal(logprintf));
	p_interface.reset(new Interface);
	p_natives.reset(new Natives);
	p_hook.reset(new Hook);
}

Core::~Core()
{
	p_internal.reset();
	p_interface.reset();
	p_natives.reset();
	p_hook.reset();
}

local_ptr<Internal> Core::getInternal()
{
	return p_internal;
}

local_ptr<Interface> Core::getInterface()
{
	return p_interface;
}

local_ptr<Natives> Core::getNatives()
{
	return p_natives;
}

local_ptr<Hook> Core::getHook()
{
	return p_hook;
}