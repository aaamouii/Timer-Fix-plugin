cmake_minimum_required (VERSION 2.8.4)
project(timerfix)

if (CMAKE_VERSION VERSION_GREATER 3.1.3)
	cmake_policy(SET CMP0054 NEW)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	if(NOT CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
		message(FATAL_ERROR "Building for Windows requires -A Win32 flag. (cmake ./ -A Win32)")
    endif()

	add_definitions (-DWIN32 -D_WIN32 -D__STDC__)

	file(GLOB TIMERFIX_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/exports.def
	)

	set (CMAKE_C_FLAGS "/Zp1 /W4 /MT")
	set (CMAKE_CXX_FLAGS "/Zp1 /W4 /EHsc /MT")
	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

	add_definitions (-DLINUX)
	set(LINKER_FLAGS "-m32 -shared -O3 -static-libstdc++")

	set(CMAKE_C_FLAGS "-m32 -O3 -w -Ilib")
	set(CMAKE_CXX_FLAGS "-m32 -O3 -w -Ilib -std=c++14")
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})	

	set(CMAKE_CXX_COMPILER "/usr/bin/g++")
	set(CMAKE_C_COMPILER "/usr/bin/gcc")
	set(CMAKE_SHARED_LIBRARY_PREFIX "")

else ()
	message(FATAL_ERROR "Sorry, but Timer Fix building supports only MSVC++ and GNU compilers")
endif()

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/lib
)

file(GLOB TIMERFIX_SRC
	${TIMERFIX_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)
source_group("" FILES ${FCNPC_SRC})

file(GLOB SDK_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/amx/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/amx/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/amx/*.c
)
source_group(${LIBRARY_GROUP}SDK FILES ${SDK_SRC})

add_library (${PROJECT_NAME} SHARED ${TIMERFIX_SRC}
				    ${SDK_SRC})