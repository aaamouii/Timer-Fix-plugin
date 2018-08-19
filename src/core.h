#ifndef CORE_H_
#define CORE_H_

#include "plugin.h"
#include "timer.h"
#include "natives.h"
#include "precompilied.h"

class Core
{
public:
	Core();
	~Core();

	inline Plugin *getPlugin()
	{
		return plugin.get();
	}

	inline Timer *getTimer()
	{
		return timer.get();
	}

	inline Natives *getNatives()
	{
		return natives.get();
	}
private:
	boost::scoped_ptr<Plugin> plugin;
	boost::scoped_ptr<Timer> timer;
	boost::scoped_ptr<Natives> natives;
};

#endif