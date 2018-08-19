#include "core.h"

boost::scoped_ptr<Core> core;

Core::Core()
{
	plugin.reset(new Plugin);
	timer.reset(new Timer);
	natives.reset(new Natives);
}

Core::~Core()
{
	plugin.reset();
	timer.reset();
	natives.reset();
}