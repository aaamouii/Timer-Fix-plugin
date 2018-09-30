#pragma once

#if !defined TBIT_64 && !defined TBIT_32
  #define TBIT_32
#endif

#ifndef CORETYPE
  #ifdef TBIT_64
	#define CORETYPE unsigned long long
  #else
	#define CORETYPE unsigned long
  #endif
#endif

namespace tf {

	namespace time {

		namespace milliseconds {

			CORETYPE get();

		} // namespace milliseconds

	} // namespace time

} // namespace tf