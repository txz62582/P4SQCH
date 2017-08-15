#pragma once
#include <fstream>

#include "SqchSDK\SDK.h"
#include "EntityManager.h"
#include "P4Menu\Config.h"

#define M_PI		3.14159265358979323846f
#define M_PI_F		((float)(M_PI))
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )


class Utils {
public:

	static bool W2S(const SSDK::Vector& v3D, SSDK::Vector& v2D) {
		return (SSDK::I::DebugOverlay()->ScreenPosition(v3D, v2D) != 1);
	}

	static void InitChams() {
		std::ifstream file("csgo\\materials\\chams.vmt");

		if (!file.good()) {
			std::string str =
				R"###("vertexlitgeneric"
            {
                "$basetexture" "VGUI/white_additive"
                "$ignorez"     "0"
                "$nofog"       "1"
                "$model"       "1"
                "$nocull"      "1"
                "$halflambert" "1"
            })###";

			std::ofstream("csgo\\materials\\chams.vmt") << str;
		}
	}

	static void InitChamsXQZ() {
		std::ifstream file("csgo\\materials\\chams_z.vmt");

		if (!file.good()) {
			std::string str =
				R"###("vertexlitgeneric"
            {
                "$basetexture" "VGUI/white_additive"
                "$ignorez"     "1"
                "$nofog"       "1"
                "$model"       "1"
                "$nocull"      "1"
                "$halflambert" "1"
            })###";

			std::ofstream("csgo\\materials\\chams_z.vmt") << str;
		}
	}

	static void initVMTFiles() {
		InitChams();
		InitChamsXQZ();
	}

	static const char* PadStringRight(std::string text, size_t value)
	{
		text.insert(text.length(), value - text.length(), ' ');

		return text.c_str();
	}

	static float CSGO_Armor(float flDamage, int ArmorValue)
	{
		float flArmorRatio = 0.5f;
		float flArmorBonus = 0.5f;
		if (ArmorValue > 0) {
			float flNew = flDamage * flArmorRatio;
			float flArmor = (flDamage - flNew) * flArmorBonus;

			if (flArmor > static_cast<float>(ArmorValue)) {
				flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
				flNew = flDamage - flArmor;
			}

			flDamage = flNew;
		}
		return flDamage;
	}
	
	static bool HitChance(C_CSPlayer* pCSLocalPlayer, C_CSPlayer* pCSTarget, C_BaseCombatWeapon* pCSWeapon, SSDK::QAngle qAngle, int chance) {
		if (!chance)
			return true;

		int iHit = 0;
		int iHitsNeed = (int)((float)MenuOptions.Aimbot.g_hitchanceSeed * ((float)chance / 100.f));
		bool bHitchance = false;

		SSDK::Vector forward, right, up;
		AngleVectors(qAngle, &forward, &right, &up);

		pCSWeapon->UpdateAccuracy();

		for (auto i = 0; i < MenuOptions.Aimbot.g_hitchanceSeed; ++i) {
			//Utils::RandomSeed((seed & 0xFF) + 1);

			float RandomA = RandomFloat(0.0f, 1.0f);
			float RandomB = 1.0f - RandomA * RandomA;

			RandomA = RandomFloat(0.0f, M_PI_F * 2.0f);
			RandomB *= pCSWeapon->GetCone();

			float SpreadX1 = (cos(RandomA) * RandomB);
			float SpreadY1 = (sin(RandomA) * RandomB);

			float RandomC = RandomFloat(0.0f, 1.0f);
			float RandomF = RandomF = 1.0f - RandomC * RandomC;

			RandomC = RandomFloat(0.0f, M_PI_F * 2.0f);
			RandomF *= pCSWeapon->GetSpread();

			float SpreadX2 = (cos(RandomC) * RandomF);
			float SpreadY2 = (sin(RandomC) * RandomF);

			float fSpreadX = SpreadX1 + SpreadX2;
			float fSpreadY = SpreadY1 + SpreadY2;

			SSDK::Vector vSpreadForward;
			vSpreadForward[0] = forward[0] + (fSpreadX * right[0]) + (fSpreadY * up[0]);
			vSpreadForward[1] = forward[1] + (fSpreadX * right[1]) + (fSpreadY * up[1]);
			vSpreadForward[2] = forward[2] + (fSpreadX * right[2]) + (fSpreadY * up[2]);
			vSpreadForward.NormalizeInPlace();

			SSDK::QAngle qaNewAngle;
			Utils::VectorAngles1(vSpreadForward, qaNewAngle);
			Utils::Normalize(qaNewAngle);

			//DEBUG
			//SSDK::I::Engine()->SetViewAngles(qaNewAngle);

			SSDK::QAngle vEnd;
			Utils::AngleVectors(qaNewAngle, &vEnd);

			vEnd = pCSLocalPlayer->GetEyePos() + (vEnd * 8192.f);

			if (C_CSPlayer::GetLocalPlayer()->canHit(vEnd, pCSTarget)) {
				iHit++;
			}

			if ((int)(((float)iHit / MenuOptions.Aimbot.g_hitchanceSeed) * 100.f) >= chance) {
				bHitchance = true;
				break;
			}

			if ((MenuOptions.Aimbot.g_hitchanceSeed - 1 - i + iHit) < iHitsNeed)
				break;
		}

		return bHitchance;
	}

	static void RandomSeed(int iSeed)
	{
		typedef void(*RandomSeed_t)(int);
		static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomSeed"));
		return m_RandomSeed(iSeed);
	}

	static void VectorAngles(const SSDK::Vector& forward, SSDK::QAngle &angles) {
		if (forward[1] == 0.0f && forward[0] == 0.0f) {
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles[1] = 0.0f;
		}
		else {
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;
	}

	static void VectorAngles1(const SSDK::Vector& forward, SSDK::QAngle &angles) {
		float    tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	static void AngleVectors(const SSDK::QAngle &angles, SSDK::Vector *forward)
	{
		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	static void AngleVectors(const SSDK::QAngle &angles, SSDK::Vector *forward, SSDK::Vector *right, SSDK::Vector *up) {
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[SSDK::MathThings::YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[SSDK::MathThings::PITCH]), &sp, &cp);
		SinCos(DEG2RAD(angles[SSDK::MathThings::ROLL]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}

	static void SinCos(float radians, float *sine, float *cosine) {
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	static SSDK::Vector GetEntityHitbox(SSDK::IClientEntity* pEntity, int iHitBox) {
		SSDK::matrix3x4_t boneMatrix[128];

		if (!pEntity->SetupBones(boneMatrix, 128, 256, SSDK::I::Engine()->GetLastTimeStamp()))
			return SSDK::Vector();

		SSDK::studiohdr_t* hdr = SSDK::I::ModelInfo()->GetStudiomodel(pEntity->GetModel());
		SSDK::mstudiohitboxset_t* set = hdr->pHitboxSet(0);

		SSDK::matrix3x4_t hitbox = boneMatrix[set->pHitbox(iHitBox)->bone];

		return SSDK::Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}

	static SSDK::Vector GetEntityBone(SSDK::IClientEntity* pEntity, int iBone) {
		SSDK::matrix3x4_t boneMatrix[128];

		if (!pEntity->SetupBones(boneMatrix, 128, 256, SSDK::I::Engine()->GetLastTimeStamp()))
			return SSDK::Vector();

		SSDK::matrix3x4_t hitbox = boneMatrix[iBone];

		return SSDK::Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}

	static bool isValidPlayer(C_CSPlayer* pEntity, bool isLocalPlayer = false) {
		if (!pEntity)
			return false;
		if (pEntity->GetClientClass()->m_ClassID != SSDK::EClassIds::CCSPlayer)
			return false;
		if (pEntity->IsDormant())
			return false;
		if (!isLocalPlayer) {
			if (pEntity->EntIndex() == C_CSPlayer::GetLocalPlayer()->EntIndex())
				return false;
		}
		if (!pEntity->IsAlive())
			return false;
		return true;
	}

	static float GetClosestEnnemyDistance(C_CSPlayer* pFromPlayer) {
		float Closest = 1024.f;

		for (int i = 0; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
			C_CSPlayer *pSelectedPlayer = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);

			if (!isValidPlayer(pSelectedPlayer))
				continue;

			if (pFromPlayer->GetTeamNum() == pSelectedPlayer->GetTeamNum())
				continue;

			float fCurDist = (*pFromPlayer->GetOrigin()).DistTo(*pSelectedPlayer->GetOrigin());

			if ((fCurDist < Closest) & (fCurDist > 0)) {
				Closest = fCurDist;
			}
		}

		return Closest;
	}

	static bool isValidWeapon(C_CSPlayer* pEntity) {
		if (!pEntity)
			return false;
		if (strstr(pEntity->GetClientClass()->m_pNetworkName, XorStr("CWeapon")) ||
			strstr(pEntity->GetClientClass()->m_pNetworkName, XorStr("CAK47")) ||
			strstr(pEntity->GetClientClass()->m_pNetworkName, XorStr("CDEagle")))
			return true;
		return false;
	}

	static bool ApplyCustomSkin(C_CSPlayer* pLocal, DWORD hWeapon) {
		static int KnifeDefinitionArray[] = { 42,500,505,506,507,508,509,512,514,515,516 };

		// Get the weapon entity from the provided handle.
		CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)SSDK::I::EntityList()->GetClientEntityFromHandle(hWeapon);

		if (!pWeapon)
			return false;

		// Get the weapons item definition index.
		int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();


		if ((nWeaponIndex == SSDK::EItemDefinitionIndex::weapon_knife) ||
			(nWeaponIndex == SSDK::EItemDefinitionIndex::weapon_knife_t) ||
			((nWeaponIndex == KnifeDefinitionArray[MenuOptions.Skins.iSelectedKnife]) && (MenuOptions.Skins.iSelectedKnife > 0))) {

			//Change model
			CBaseViewModel* pViewModel = pLocal->GetViewModel();

			if (pViewModel) {

				DWORD hViewModelWeapon = pViewModel->GetWeapon();
				CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)SSDK::I::EntityList()->GetClientEntityFromHandle(hViewModelWeapon);

				if (pViewModelWeapon == pWeapon) {
					switch (MenuOptions.Skins.iSelectedKnife) {
					case 1:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_bayonet.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_bayonet.mdl"));
						break;
					case 2:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_flip.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_flip.mdl"));
						break;
					case 3:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_gut.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_gut.mdl"));
						break;
					case 4:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_karam.mdl"), pWeapon);
						//*pViewModelWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_karam.mdl"));
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_karam.mdl"));
						break;
					case 5:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_m9_bay.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_m9_bay.mdl"));
						break;
					case 6:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_tactical.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_tactical.mdl"));
						break;
					case 7:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_falchion_advanced.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_falchion_advanced.mdl"));
						break;
					case 8:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_survival_bowie.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_survival_bowie.mdl"));
						break;
					case 9:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_butterfly.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_butterfly.mdl"));
						break;
					case 10:
						//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_push.mdl"), pWeapon);
						*pViewModel->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_push.mdl"));
						break;
					}
				}
			}

			//Change skin
			//*pWeapon->GetFallbackPaintKit() = 415; //Ruby
			//*pWeapon->GetFallbackPaintKit() = 568; //Emerald
			//*pWeapon->GetFallbackPaintKit() = 413; //Marble fade
			*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.iCurPaintKit;
			*pWeapon->GetFallbackSeed() = 444;
			*pWeapon->GetEntityQuality() = 1;
			*pWeapon->GetFallbackStatTrak() = -1;
			*pWeapon->GetFallbackWear() = 0.0000001f;
			*pWeapon->GetItemDefinitionIndex() = KnifeDefinitionArray[MenuOptions.Skins.iSelectedKnife];

			*pWeapon->GetItemIDHigh() = -1;

			return true;
		}

		{
			if (nWeaponIndex == 1)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.deagle;
			}
			else if (nWeaponIndex == 63)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.cz75;
			}
			else if (nWeaponIndex == 32)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.p2000;
			}
			else if (nWeaponIndex == 2)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.elites;
			}
			else if (nWeaponIndex == 36)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.p250;
			}
			else if (nWeaponIndex == 3)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.fiveseven;
			}
			else if (nWeaponIndex == 30)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.tec9;
			}
			else if (nWeaponIndex == 4)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.glock;
			}
			else if (nWeaponIndex == 8)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.aug;
			}
			else if (nWeaponIndex == 13)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.galil;
			}
			else if (nWeaponIndex == 61)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.usps;
			}
			else if (nWeaponIndex == 7)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.ak47;
			}
			else if (nWeaponIndex == 10)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.famas;
			}
			else if (nWeaponIndex == 16)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.m4a4;
			}
			else if (nWeaponIndex == 60)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.m4a1s;
			}
			else if (nWeaponIndex == 26)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.bizon;
			}
			else if (nWeaponIndex == 17)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.mac10;
			}
			else if (nWeaponIndex == 33)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.mp7;
			}
			else if (nWeaponIndex == 34)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.mp9;
			}
			else if (nWeaponIndex == 19)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.p90;
			}
			else if (nWeaponIndex == 14)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.m249;
			}
			else if (nWeaponIndex == 24)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.ump45;
			}
			else if (nWeaponIndex == 28)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.negev;
			}
			else if (nWeaponIndex == 27)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.mag7;
			}
			else if (nWeaponIndex == 35)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.nova;
			}
			else if (nWeaponIndex == 29)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.sawedoff;
			}
			else if (nWeaponIndex == 25)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.xm1014;
			}
			else if (nWeaponIndex == 38)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.scar20;
			}
			else if (nWeaponIndex == 9)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.awp;
			}
			else if (nWeaponIndex == 40)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.ssg08;
			}
			else if (nWeaponIndex == 11)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.g3sg1;
			}
			else if (nWeaponIndex == 64)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.magnum;
			}
			else if (nWeaponIndex == 39)
			{
				*pWeapon->GetFallbackPaintKit() = MenuOptions.Skins.sg556;
			}

			*pWeapon->GetEntityQuality() = 1;
			*pWeapon->GetFallbackSeed() = 300;
			*pWeapon->GetFallbackStatTrak() = -1;
			*pWeapon->GetFallbackWear() = 0.000001f;

			*pWeapon->GetItemIDHigh() = -1;
			return true;
		}
		return false;
	}

	static SSDK::QAngle CalcAngle(SSDK::Vector src, SSDK::Vector dst) {
		SSDK::QAngle angles;
		SSDK::Vector delta = dst - src;
		delta.NormalizeInPlace();
		VectorAngles1(delta, angles);
		return angles;
	}

	static float AngleDifference(SSDK::Vector ViewAngles, SSDK::Vector TargetAngles, float Distance)
	{
		auto pitch = sin(DEG2RAD(ViewAngles.x - TargetAngles.x)) * Distance;
		auto yaw = sin(DEG2RAD(ViewAngles.y - TargetAngles.y)) * Distance;

		return sqrt(powf(pitch, 2.0) + powf(yaw, 2.0));
	}

	static SSDK::QAngle SmoothAngle(SSDK::Vector From, SSDK::Vector To, float flFactor)
	{
		SSDK::QAngle qStep;
		SSDK::QAngle qDelta = To - From;

		Normalize(qDelta);

		qStep.x = qDelta.x / flFactor;
		qStep.y = qDelta.y / flFactor;

		Normalize(qStep);

		return qStep;
	}

	//Method to get the infos of the player for the chatbot
	static SSDK::PlayerInfo GetInfo(int Index)
	{
		SSDK::PlayerInfo Info;
		SSDK::I::Engine()->GetPlayerInfo(Index, &Info);
		return Info;
	}


	static bool Clamp(SSDK::QAngle &angles) {
		SSDK::QAngle a = angles;
		Normalize(a);
		ClampAngles(a);

		if (isnan(a.x) || isinf(a.x) ||
			isnan(a.y) || isinf(a.y) ||
			isnan(a.z) || isinf(a.z)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}

	static void Normalize(SSDK::QAngle& angle) {
		while (angle.x > 89.0f) {
			angle.x -= 180.f;
		}
		while (angle.x < -89.0f) {
			angle.x += 180.f;
		}
		while (angle.y > 180.f) {
			angle.y -= 360.f;
		}
		while (angle.y < -180.f) {
			angle.y += 360.f;
		}
	}

	static void NormalizeFloat(float& angle)
	{
		while (angle > 180.f) {
			angle -= 360.f;
		}
		while (angle < -180.f) {
			angle += 360.f;
		}
	}

	static void ClampAngles(SSDK::QAngle &angles) {
		if (angles.y > 180.0f)
			angles.y = 180.0f;
		else if (angles.y < -180.0f)
			angles.y = -180.0f;

		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		angles.z = 0;
	}

	static SSDK::Vector extrapolateTick(SSDK::Vector p0, C_CSPlayer* pEntity) {
		if ((!p0.IsZero()) && (!pEntity->GetVelocity().IsZero())) {
			return p0 + (pEntity->GetVelocity() * SSDK::I::GlobalVars()->interval_per_tick);
		} else {
			return p0;
		}
	}

	static void VectorTransform(SSDK::Vector& in1, SSDK::matrix3x4a_t& in2, SSDK::Vector &out) {
		out.x = in1.Dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3];
		out.y = in1.Dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3];
		out.z = in1.Dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3];
	}

	static float RandomFloat(float min, float max)
	{
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomFloat"));
		return m_RandomFloat(min, max);
	}


	template< class T, class Y >
	static T ClampValue(T const &val, Y const &minVal, Y const &maxVal)
	{
		if (val < minVal)
			return minVal;
		else if (val > maxVal)
			return maxVal;
		else
			return val;
	}

	static float GetLerpTime() {
		float updaterate = SSDK::I::CVar()->FindVar(XorStr("cl_updaterate"))->GetFloat();
		float ratio = SSDK::I::CVar()->FindVar(XorStr("cl_interp_ratio"))->GetFloat();
		float lerp = SSDK::I::CVar()->FindVar(XorStr("cl_interp"))->GetFloat();

		//SSDK::I::CVar()->ConsolePrintf("%f\n", updaterate);

		return max(lerp, ratio / updaterate);
	}

	static char *substr(char *buf, const char *s, size_t beg, size_t len)
	{
		memcpy(buf, s + beg, len);
		buf[len] = '\0';
		return (buf);
	}
};