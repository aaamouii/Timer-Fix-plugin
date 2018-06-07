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
#include "cool-amx.h"
/*
//	save gamemode amx
*/
coolamx::coolamx_t cool_amx;
/*
//	gamemode amx (for natives and etc)
*/
AMX *gamemode_amx = NULL;

typedef void(*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;

// ----------------------------------
// class :coolamx

/*
//	global functions
*/
bool coolamx::isgamemode(AMX *amx)
{
	if (!amx)
	{
		return false;
	}

	int idx;
	if (!amx_FindPublic(amx, "OnFilterScriptInit", &idx)) return false;
	return true;
}
AMX *coolamx::get_gamemode_amx()
{
	return gamemode_amx;
}
// ----------------------------------
// natives
bool coolamx::parse_natives(AMX *amx)
{
	if (!amx)
	{
		return false;
	}

	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter != cool_amx.end())
	{
		int num_natives = 0;
		amx_NumNatives(amx, &num_natives);
		iter->second.num_natives = num_natives;
		for (int i = 0; i < num_natives; i++)
		{
			char native_name[MAX_NATIVE_NAME];
			amx_GetNative(amx, i, native_name);
			iter->second.coolamx_natives.emplace(std::move(native_name), i);
		}
		return true;
	}
	return false;
}
unsigned long coolamx::get_native_address(AMX *amx, int native_index)
{
	if (!native_index)
	{
		return NULL;
	}
	AMX_HEADER *hdr = (AMX_HEADER*)amx->base;
	if (!hdr || hdr->magic != AMX_MAGIC) return -1;
	return (unsigned long)((AMX_FUNCSTUB*)((char*)hdr + hdr->natives + hdr->defsize * native_index))->address;
}
int coolamx::find_native(AMX *amx, const char *native_name)
{
	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter != cool_amx.end())
	{
		coolamx_natives_t::iterator native_iter = iter->second.coolamx_natives.find(native_name);
		if (native_iter != iter->second.coolamx_natives.end())
		{
			return native_iter->second;
		}
	}
	return -1;
}
bool coolamx::hook_native(AMX *amx, const char *src, void *dst)
{
	if (!src)
	{
		return false;
	}
	AMX_HEADER *hdr = (AMX_HEADER*)amx->base;
	if (!hdr || hdr->magic != AMX_MAGIC)
	{
		return false;
	}
	
	int native_index = find_native(amx, src);
	if (native_index == -1)
	{
		return false;
	}

	AMX_FUNCSTUB *funcstub = (AMX_FUNCSTUB*)((char*)hdr + hdr->natives + hdr->defsize * native_index);
	funcstub->address = (ucell)dst;
	return true;
}
cell coolamx::call_native(AMX *amx, const char *native_name, cell *params)
{
	int native_index = find_native(amx, native_name);
	if (native_index != NULL)
	{
		unsigned long native_address = get_native_address(amx, native_index);
		if (native_address)
		{
			return reinterpret_cast<cell(AMX_NATIVE_CALL*)(AMX*, cell*)>(native_address)(amx, params);
		}
	}
	return NULL;
}
int coolamx::get_natives_count(AMX *amx)
{
	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter != cool_amx.end())
	{
		return iter->second.num_natives;
	}
	return NULL;
}
// ----------------------------------

// ----------------------------------
// publics
bool coolamx::parse_publics(AMX *amx)
{
	if (!amx)
	{
		return false;
	}
	
	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter != cool_amx.end())
	{
		int num_publics = 0;
		amx_NumPublics(iter->first, &iter->second.num_publics);
		if (!iter->second.num_publics)
		{
			return false;
		}
		for (int i = 0; i < iter->second.num_publics; i++)
		{
			char public_name[MAX_PUBLIC_NAME];
			amx_GetPublic(amx, i, public_name);
			iter->second.coolamx_publics.emplace(std::move(public_name), i);
		}
		return true;
	}

	return false;
}
unsigned long coolamx::get_public_address(AMX *amx, int public_index)
{
	AMX_HEADER *hdr = (AMX_HEADER*)amx->base;
	if (!hdr || hdr->magic != AMX_MAGIC) return -1;
	return (unsigned long)((AMX_FUNCSTUB*)((char*)hdr + hdr->publics + hdr->defsize * public_index))->address;
}
int coolamx::find_public(AMX *amx, const char *public_name)
{
	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter != cool_amx.end())
	{
		coolamx_publics_t::iterator public_iter = iter->second.coolamx_publics.find(public_name);
		if (public_iter != iter->second.coolamx_publics.end())
		{
			return public_iter->second;
		}
	}
	return -1;
}
int coolamx::get_publics_count(AMX *amx)
{
	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter != cool_amx.end())
	{
		return iter->second.num_publics;
	}
	return NULL;
}
// ----------------------------------
bool coolamx::store_amx(AMX *amx)
{
	coolamx_t::iterator iter = cool_amx.find(amx);
	if (iter == cool_amx.end())
	{
		coolamx_struct stcoolamx;
		stcoolamx.isgamemode = isgamemode(amx);
		if (stcoolamx.isgamemode)
		{
			gamemode_amx = amx;
		}
		cool_amx.insert(std::pair<AMX*, coolamx_struct>(amx, stcoolamx));
		return true;
	}
	return false;
}
COOL_AMX_ERRORS coolamx::init(AMX *amx)
{
	/*
	//	Safety first!
	*/
	if (!amx)
	{
		return COOL_AMX_ERROR_SCRIPT_OVERFLOW;
	}

	/*
	//	store amx
	*/
	if (!store_amx(amx))
	{
		return COOL_AMX_ERROR_STORE_AMX_FAILED;
	}

	/*
	//	saving natives instead of using amx_FindNative
	*/
	if (!parse_natives(amx))
	{
		return COOL_AMX_ERROR_PARSE_NATIVES_FAILED;
	}

	if (!parse_publics(amx))
	{
		return COOL_AMX_ERROR_PARSE_PUBLICS_FAILED;
	}

	return COOL_AMX_ERROR_OK;
}
bool coolamx::uninit(AMX *amx)
{
	coolamx_t::iterator iter = cool_amx.find(amx);
	if(iter != cool_amx.end())
	{
		cool_amx.erase(amx);
		return true;
	}
	return false;
}
// ----------------------------------