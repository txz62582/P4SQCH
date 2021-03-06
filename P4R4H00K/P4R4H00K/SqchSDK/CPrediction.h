#pragma once

#include "IMoveHelper.h"
#include "Utils.h"
#include "IGameMovement.h"

namespace SSDK {
	class CPrediction{
	public:
		void SetupMove(IClientEntity *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move) {
			typedef void(__thiscall *OrigFn)(void*, IClientEntity*, CUserCmd*, IMoveHelper*, CMoveData*);
			CallVFunction<OrigFn>(this, 20)(this, player, ucmd, pHelper, move);
		}

		void FinishMove(IClientEntity *player, CUserCmd *ucmd, CMoveData *move) {
			typedef void(__thiscall *OrigFn)(void*, IClientEntity*, CUserCmd*, CMoveData*);
			CallVFunction<OrigFn>(this, 21)(this, player, ucmd, move);
		}
	};
}