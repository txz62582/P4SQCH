#pragma once
#include "../SqchSDK/SDK.h"
#include "../P4Menu/Options.h"
#include "../Misc.h"
#include "../CFixMove.h"
#include "../CLagCompensation2.h"

namespace AntiAim {
	void PostCreateMoveAA(C_CSPlayer* pLocalPlayer, C_BaseCombatWeapon* pActiveWeapon, SSDK::CUserCmd* pCmd, bool& bSendPacket, CBaseAttributableItem* pAttribWeapon);
	void SilentAim(SSDK::CUserCmd* pCmd, bool& bSendPacket);
}