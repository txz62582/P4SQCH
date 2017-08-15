#include "Includes.h"
SetClanTagFn SetClanTagEx;

void Hookers::SetClanTag(const char* tag, const char* name)
{
	SetClanTagFn SetClanTagEx = reinterpret_cast<SetClanTagFn >(SSDK::I::FindSignature(XorStr("engine.dll"), XorStr("53 56 57 8B DA 8B F9 FF 15")));
	SetClanTagEx(tag, name);
}