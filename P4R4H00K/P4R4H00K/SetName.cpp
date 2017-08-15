#include "Includes.h"


__declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm {
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

void DECLSPEC_NOINLINE Hookers::NET_SetConVar(const char* value, const char* cvar)
{
	void* pvSetConVar = (char*)SSDK::I::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	Invoke_NET_SetConVar(pvSetConVar, cvar, value);
}