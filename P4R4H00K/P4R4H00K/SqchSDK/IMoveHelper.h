#pragma once

#include "IClientEntity.h"

namespace SSDK {
	typedef CBaseHandle EntityHandle_t;

	struct soundlevel_t;
	class PLAYER_ANIM;

	class IMoveHelper{
	public:
		void SetHost(IClientEntity* host) {
			typedef void(__thiscall *OrigFn)(void*, IClientEntity*);
			CallVFunction<OrigFn>(this, 1)(this, host);
		}
	};
}