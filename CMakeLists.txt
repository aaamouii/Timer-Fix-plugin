project(timerfix)
cmake_minimum_required (VERSION 2.8.4)

set(LIBRARY_GROUP "lib\\")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

	add_definitions (-DWIN32 -D_WIN32 -D__STDC__)

	file(GLOB TIMERFIX_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/supports.def
	)

	set (CMAKE_C_FLAGS "/Zp1 /W4 /MT")
	set (CMAKE_CXX_FLAGS "/Zp1 /W4 /EHsc /MT")
	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

	add_definitions (-DLINUX)
	set(LINKER_FLAGS "-m32 -shared -O3 -static-libstdc++")

	set(CMAKE_C_FLAGS "-m32 -O3 -w -Ilib")
	set(CMAKE_CXX_FLAGS "-m32 -O3 -w -Ilib -std=c++11")
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})	

	set(CMAKE_CXX_COMPILER "/usr/bin/g++")
	set(CMAKE_C_COMPILER "/usr/bin/gcc")
	set(CMAKE_STATIC_LIBRARY_PREFIX "")

endif ()

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
	${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/amx/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/amx/*.c
)
source_group(${LIBRARY_GROUP}SDK FILES ${SDK_SRC})

add_library (${PROJECT_NAME} SHARED ${TIMERFIX_SRC}
				    ${SDK_SRC})
