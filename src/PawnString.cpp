/*
	MIT License
	Copyright (c) 2018-2019 Kash Cherry
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "PawnString.h"

extern logprintf_t logprintf;

PawnString::PawnString(AMX* amx, cell param)
{
	cell* addr;
	if (amx_GetAddr(amx, param, &addr) == AMX_ERR_NONE)
	{
		int length;
		if (amx_StrLen(addr, &length) == AMX_ERR_NONE)
		{
			length++;
			char* str = new char[length];
			amx_GetString(str, addr, 0, length);
			m_sCharConversion = str;
		}
	}
}

PawnString::~PawnString()
{
	m_sCharConversion.clear();
}

std::string PawnString::get_instance()
{
	return m_sCharConversion;
}