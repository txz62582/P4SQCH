#pragma once

#include <iostream>
#include "Utils.h"
#include "Color.h"
#include "ConVar.h"

namespace SSDK {
	typedef int CVarDLLIdentifier_t;

	class ICvar
	{
	public:
		void RegisterConCommand(ConVar *pCommandBase) {
			typedef void(__thiscall *OrigFn)(void*, ConVar *);
			CallVFunction<OrigFn>(this, 10)(this, pCommandBase);
		}

		void UnregisterConCommand(ConVar *pCommandBase) {
			typedef void(__thiscall *OrigFn)(void*, ConVar *);
			CallVFunction<OrigFn>(this, 11)(this, pCommandBase);
		}

		void UnregisterConCommands(CVarDLLIdentifier_t id) {
			typedef void(__thiscall *OrigFn)(void*, CVarDLLIdentifier_t);
			CallVFunction<OrigFn>(this, 12)(this, id);
		}

		ConVar* FindVar(const char *var_name) {
			typedef ConVar*(__thiscall *OrigFn)(void*, const char *);
			return CallVFunction<OrigFn>(this, 16)(this, var_name);
		}

		void ConsoleColorPrintf(const Color& clr, const char *pFormat, ...)
		{
			char buffer[1024];
			va_list list;
			va_start(list, pFormat);
			vsprintf_s(buffer, pFormat, list);
			va_end(list);
			typedef void(__cdecl *OrigFn)(void*, const Color&, const char *, va_list);
			CallVFunction<OrigFn>(this, 25)(this, clr, buffer, list);
		}

		void ConsolePrintf(const char *pFormat, ...)
		{
			char buffer[1024];
			va_list list;
			va_start(list, pFormat);
			vsprintf_s(buffer, pFormat, list);
			va_end(list);
			typedef void(__cdecl *OrigFn)(void*, const char *, va_list);
			CallVFunction<OrigFn>(this, 26)(this, buffer, list);
		}
	};
}