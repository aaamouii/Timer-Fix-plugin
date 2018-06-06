/*
	Copyright 2018 RakLabs

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
/*
//	cool-amx is
//	cross-platform C++ amx library for simple working with natives and publics
*/
#pragma once
#include <iostream>
#include <unordered_map>
#ifndef AMX_H_INCLUDED
/*
//	include amx headers
//	for working with it
*/
#include "amx/amx.h"
#include "plugincommon.h"
#endif

/*
//	cool-amx limits
*/
#define MAX_NATIVE_NAME		32
#define MAX_PUBLIC_NAME		32

/*
//	cool-amx errors
*/
enum COOL_AMX_ERRORS
{
	COOL_AMX_ERROR_OK,
	COOL_AMX_ERROR_SCRIPT_OVERFLOW,
	COOL_AMX_ERROR_INVALID_NATIVE_ADDRESS,
	COOL_AMX_ERROR_PARSE_NATIVES_FAILED,
	COOL_AMX_ERROR_PARSE_PUBLICS_FAILED,
	COOL_AMX_ERROR_STORE_AMX_FAILED
};

class coolamx
{
public:
	/*
	//	cool-amx types
	*/
	typedef std::unordered_map<std::string, int> coolamx_natives_t;
	typedef std::unordered_map<std::string, int> coolamx_publics_t;
	typedef struct
	{
		coolamx_natives_t coolamx_natives;
		coolamx_publics_t coolamx_publics;
		bool isgamemode = false;
		int num_publics = 0;
		int num_natives = 0;
	} coolamx_struct;
	typedef std::unordered_map<AMX *, coolamx_struct> coolamx_t;

	/*
	//	save natives, instead of using amx_FindNative,
	//	and getting native addresses
	*/
	static bool isgamemode(AMX *amx);
	static AMX *get_gamemode_amx();

	/*
	//	natives
	*/
	static bool parse_natives(AMX *amx);
	static unsigned long get_native_address(AMX *amx, int native_index);
	static int find_native(AMX *amx, const char *native_name);
	static bool hook_native(AMX *amx, const char *src, void *dst);
	static cell call_native(AMX *amx, const char *native_name, cell *params);
	static int get_natives_count(AMX *amx);

	/*
	//	publics
	*/
	static bool parse_publics(AMX *amx);
	static unsigned long get_public_address(AMX *amx, int public_index);
	static int find_public(AMX *amx, const char *public_name);
	static int get_publics_count(AMX *amx);

	/*
	//	cool-amx initializating
	*/
	static COOL_AMX_ERRORS init(AMX *amx);
	static void uninit(AMX *amx);
	static bool store_amx(AMX *amx);
};