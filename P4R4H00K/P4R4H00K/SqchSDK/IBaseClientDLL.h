#pragma once

#include "ClientClass.h"

namespace SSDK {
	class IBaseClientDLL {
	public:
		ClientClass* GetAllClasses() {
			typedef ClientClass*(__thiscall *OrigFn)(void*);
			return CallVFunction<OrigFn>(this, 8)(this);
		}
	};
}