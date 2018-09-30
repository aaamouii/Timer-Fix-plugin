#pragma once

typedef void(*logprintf_t)(const char *format, ...);

namespace tf {
	
	class Kernel {
	public:
		Kernel(void **ppData);
		void send(const char *format, ...);
	};

} // namespace tf