#include "Antiaim.h"

#include <chrono>

namespace AntiAim {
	float Pitch(int selected) {
		float tmp = 0.f;

		switch (selected) {
		case 1:
			tmp = 89.f; //Down
			break;
		case 2:
			tmp = 70.f; //Down test
			break;
		case 3:
			tmp = 0.f; //Mid
			break;
		case 4:
			tmp = -89.f; //Up
			break;
		}
		return tmp;
	}

	float Yaw(int selected, SSDK::CUserCmd* pCmd, bool stop = false) {
		//Forward jitter2
		static int fJitter1 = 1;
		static float fJitterAngles[] = { 10.f,-10.f,-30.f,30.f,0.f };

		float tmp;
		static bool flip = false;

		switch (selected) {
		case 1:
			tmp = flip ? 90.f : -90.f; //Jitter LBY Basic
			break;
		case 2:
			tmp = flip ? -90.f : 90.f; //Jitter LBY Reversed Basic
			break;
		case 3:
			tmp = flip ? 140.f : -140.f; //Back Jitter LBY Basic
			break;
		case 4:
			tmp = (float)(fmod(SSDK::I::GlobalVars()->curtime / MenuOptions.Aimbot.g_spinSpeed * 360.0f, 360.0f)); //Spin LBY Basic
			break;
		case 5:
			tmp = 180.f; //Backward Basic
			break;
		case 6:
			tmp = *C_CSPlayer::GetLocalPlayer()->GetLowerBodyYaw(); //Side Unhittable
			break;
		case 7:
			tmp = fJitterAngles[pCmd->tick_count % fJitter1];
			fJitter1++;

			if (fJitter1 > ARRAYSIZE(fJitterAngles))
				fJitter1 = 1;
			break;
		}

		if (MenuOptions.Menu.bLocalLowerbodyUpdate && !stop) {
			flip = !flip;
			MenuOptions.Menu.bLocalLowerbodyUpdate = false;
		}

		return tmp;
	}

	float FakeYawUnhittable(int selected, SSDK::CUserCmd* pCmd)
	{
		float tmp;

		switch (selected)
		{
			case 0:
				tmp = MenuOptions.Menu.unhittable_side ? -60.f : 60.f; //rand 30-140
				break;
			case 1:
				tmp = MenuOptions.Menu.unhittable_side ? -90.f : 90.f; //rand 30-140
				break;
			case 2:
				tmp = MenuOptions.Menu.unhittable_side ? Utils::RandomFloat(-30.f, -140.f) : Utils::RandomFloat(30.f, 140.f); //rand 30-140
				break;
			case 3:
				if (pCmd->tick_count % 2)
				{
					tmp = MenuOptions.Menu.unhittable_side ? -160.f : 160.f; // -160 - +160
				}
				else
				{
					tmp = MenuOptions.Menu.unhittable_side ? -80.f : 80.f; //
				}
				break;
			case 4:
				tmp = MenuOptions.Menu.unhittable_side ? -160.f : 160.f; // -160 +160
				break;
			case 5:
				tmp = MenuOptions.Menu.unhittable_side ? Utils::RandomFloat(-140.f, -160.f) : Utils::RandomFloat(140.f, 160.f);
				break;
			case 6:
				tmp = MenuOptions.Menu.unhittable_side ? -135.f : 135.f; // -160 +160
				break;
			case 7:
				tmp = MenuOptions.Menu.unhittable_side ? -70.f : 70.f; // -160 +160
				break;
		}

		return tmp;
	}



	int GetFakelagAmout() {
		C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();

		if (MenuOptions.Aimbot.g_jumpingFakeLag) {
			if (!(*pLocalPlayer->GetFlags() & (int)SSDK::EntityFlags::FL_ONGROUND))
				return MenuOptions.Aimbot.g_jumpingFakeLag;
		}
		if(MenuOptions.Aimbot.g_movingFakeLag)
		{
			if(pLocalPlayer->isMoving())
				return MenuOptions.Aimbot.g_movingFakeLag;
		}

		return MenuOptions.Aimbot.g_fakeLagAmount;
	}

	void FakeLag(SSDK::CUserCmd* pCmd, bool& bSendPacket) {
		static int iTick;
		int iFakeLagAmount = GetFakelagAmout();

		if (iTick < iFakeLagAmount && !(pCmd->buttons & IN_ATTACK)) {
			bSendPacket = false;
			iTick++;
		}
		else {
			bSendPacket = true;
			iTick = 0;
		}
	}


	float doSqchAA(bool& move, bool& move2, SSDK::QAngle newAngle, SSDK::CUserCmd* pCmd, C_BaseCombatWeapon* pActiveWeapon) {
		float flUpdateTime = move2 ? 0.22000001f : 1.1f; //+ interval cause fakelag
		static auto start = std::chrono::high_resolution_clock::now();


		if (move) {
			start = std::chrono::high_resolution_clock::now();
			move = false;
		}

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> delta = end - start;

		//Find bestside
		SSDK::QAngle tmpAngle = newAngle;
		if (MenuOptions.Aimbot.g_unhittableBestSide)
		{
			MiscExt.FindBestSide(C_CSPlayer::GetLocalPlayer(), 512.f, pCmd, tmpAngle);
		}

		if (delta.count() >= flUpdateTime) {
			move2 = false;
			start = std::chrono::high_resolution_clock::now();

			if (!MenuOptions.Aimbot.g_unhittableBestSide)
				 return MenuOptions.Menu.unhittable_side ? newAngle.y + 90.f : newAngle.y - 90.f; //90,-90

			return tmpAngle.y + 180;//180
		}

		if (move2)
			return newAngle.y + Yaw(MenuOptions.Aimbot.g_movingRealYawAA, pCmd, true);

		if (!MenuOptions.Aimbot.g_unhittableBestSide)
			return newAngle.y + FakeYawUnhittable(MenuOptions.Aimbot.g_fakeUnhittable, pCmd);

		return tmpAngle.y - 70;//tmpAngle.y
	}

	void PostCreateMoveAA(C_CSPlayer* pLocalPlayer, C_BaseCombatWeapon* pActiveWeapon, SSDK::CUserCmd* pCmd, bool& bSendPacket, CBaseAttributableItem* pAttribWeapon) {
		FakeLag(pCmd, bSendPacket);

		if (GetAsyncKeyState(MenuOptions.Keys.g_disableAA) & 0x8000)
			return;

		if (!Utils::isValidPlayer(pLocalPlayer, true))
			return;

		if (!pActiveWeapon)
			return;

		if (pLocalPlayer->GetMoveType() == MoveType_t::MOVETYPE_LADDER || pLocalPlayer->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP)
			return;

		if ((pCmd->buttons & IN_USE || (pCmd->buttons & IN_ATTACK) || ((pCmd->buttons & IN_ATTACK2) && MiscExt.IsKnife(*pAttribWeapon->GetItemDefinitionIndex()))) && !pActiveWeapon->IsGrenade()) {
			return;
		}

		//if (!(pCmd->buttons & IN_USE) && !(pCmd->buttons & IN_ATTACK) && !(MiscExt.IsKnife(*pAttribWeapon->GetItemDefinitionIndex()) && (pCmd->buttons & IN_ATTACK2))) {

			//Option Nade + Grenade en main
			if (MenuOptions.Aimbot.bNadeAA && pActiveWeapon->IsGrenade()) {
				if (pActiveWeapon->GetThrowTime() > 0.f)
					return;
			}
			//WEAPON AA
			else if (!pActiveWeapon->WeaponAA()) {
				return;
			}


			static SSDK::QAngle staticAngle = pCmd->viewangles;

			SSDK::QAngle newAngle;

			//Change our pitch
			newAngle.x = Pitch(MenuOptions.Aimbot.g_pitchAA);

			static bool move;
			static bool move2;
			static bool move3;

			if (pLocalPlayer->isMoving()) {
				staticAngle = pCmd->viewangles;
				newAngle.y = staticAngle.y;

				if (bSendPacket)
					newAngle.y += Yaw(MenuOptions.Aimbot.g_movingYawAA, pCmd);
				else {
					newAngle.y += Yaw(MenuOptions.Aimbot.g_movingRealYawAA, pCmd);
				}
				move = true;
				move2 = true;
				move3 = true;
			}
			else {
				newAngle.y = staticAngle.y;

				if (bSendPacket)
					switch (MenuOptions.Aimbot.g_fakeYawAA) {
					case 6:
						newAngle.y = Yaw(MenuOptions.Aimbot.g_fakeYawAA, pCmd);
						break;
					default:
						newAngle.y += Yaw(MenuOptions.Aimbot.g_fakeYawAA, pCmd);
						break;
					}
				else {
					switch (MenuOptions.Aimbot.g_realYawAA) {
					case 6:
						newAngle.y = doSqchAA(move, move2,newAngle,pCmd, pActiveWeapon);
						break;
					default:
						newAngle.y += Yaw(MenuOptions.Aimbot.g_realYawAA, pCmd);
						break;
					}
				}
				move3 = false;
			}

			if (!bSendPacket)
				MenuOptions.Menu.currenthittableyaw = newAngle.y;

			if (Utils::Clamp(newAngle))
				pCmd->viewangles = newAngle;
		//}
	}
	void SilentAim(SSDK::CUserCmd* pCmd, bool& bSendPacket) {
		if (!MenuOptions.Aimbot.g_silentaim)
			return;

		static bool state;
		if ((pCmd->buttons & IN_ATTACK) && !state) {
			state = true;
			bSendPacket = false;
		}
		else if (state) {
			state = false;
			bSendPacket = true;
		}
	}
}
