#ifndef PLUGIN_H_
#define PLUGIN_H_

class Plugin
{
public:
	void Present(void **ppData);
	void Printf(const char *format, ...);
};

#endif