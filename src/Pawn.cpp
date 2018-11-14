/*
	MIT License

	Copyright (c) 2018 Kash Cherry

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
#include "Pawn.h"

LWM::local_ptr<CPawn> PawnInstance;

void CPawn::Initialize()
{
	PawnInstance.reset(new CPawn);
}

void CPawn::Destroy()
{
	PawnInstance.reset();
}

LWM::local_ptr<CPawn> CPawn::Get()
{
	return PawnInstance;
}

bool CPawn::Find(AMX *amx, const char *name)
{
	int index;
	return (amx_FindPublic(amx, name, &index) == AMX_ERR_NONE);
}

bool CPawn::Execute(AMX *amx, const char *name, LocalParams params)
{
	int index;
	if (!amx_FindPublic(amx, name, &index)) {
		cell tmp, retval;
		for (auto arrays : params.arrays) amx_PushArray(amx, &tmp, NULL, arrays.first, arrays.second);
		for (auto strings : params.strings) amx_PushString(amx, &tmp, NULL, strings.c_str(), NULL, NULL);
		for (auto integers : params.integers) amx_Push(amx, integers);
		if (amx_Exec(amx, &retval, index) != AMX_ERR_NONE)
			return false;
		return true;
	}
	return false;
}