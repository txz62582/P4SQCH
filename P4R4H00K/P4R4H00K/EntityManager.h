#pragma once

#include <Windows.h>
#include "SqchSDK/SDK.h"
#include "NetvarManager.h"
#include "XorStr.h"

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

class CBaseCSGrenade;
class C_BaseCombatWeapon;


class CBaseViewModel {
	template<class T>
	inline T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}
	template<class T>
	T* GetFieldPointer(int offset) {
		return (T*)((DWORD)this + offset);
	}
public:
	int *GetModelIndex() {
		static int m_nModelIndex = GET_NETVAR(XorStr("DT_BaseViewModel"), XorStr("m_nModelIndex"));
		return GetFieldPointer<int>(m_nModelIndex);
	}

	DWORD GetWeapon() {
		static DWORD m_hWeapon = GET_NETVAR(XorStr("DT_BaseViewModel"), XorStr("m_hWeapon"));
		return GetFieldValue<DWORD>(m_hWeapon);
	}
};

class VarMapEntry_t
{


public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void				*data;
	void	*watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	CUtlVector< VarMapEntry_t >	m_Entries;
	int							m_nInterpolatedEntries;
	float						m_lastInterpolationTime;
};


class C_CSPlayer : public SSDK::IClientEntity {
	template<class T>
	inline T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}
	template<class T>
	T* GetFieldPointer(int offset) {
		return (T*)((DWORD)this + offset);
	}
public:
	static C_CSPlayer* GetLocalPlayer() {
		return (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetLocalPlayer());
	}

	SSDK::Vector *GetBaseVelocity() {
		static int m_vecBaseVelocity = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecBaseVelocity"));
		return GetFieldPointer<SSDK::Vector>(m_vecBaseVelocity);
	}

	bool canHit(SSDK::Vector end, C_CSPlayer* ent) {
		SSDK::Ray_t ray;

		SSDK::Trace_t tr;

		ray.Init(this->GetEyePos(), end);

		SSDK::I::EngineTrace()->ClipRayToEntity(ray, MASK_SHOT, (SSDK::IHandleEntity*)ent, &tr); // ignore grate

		if (tr.hitgroup != 0)
			return (tr.m_pEnt && tr.m_pEnt == ent) || !tr.m_pEnt;

		return false;
	}

	bool isDucking() {
		return (*this->GetFlags() & (int)SSDK::EntityFlags::FL_DUCKING);
	}

	float* fallVelocity() {
		static float m_flFallVelocity = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_flFallVelocity"));
		return GetFieldPointer<float>(m_flFallVelocity);
	}

	SSDK::Vector* baseVelocity() {
		static int m_vecBaseVelocity = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecBaseVelocity"));
		return GetFieldPointer<SSDK::Vector>(m_vecBaseVelocity);
	}

	float GetBombTimer()
	{
		static float m_flC4Blow = GET_NETVAR(XorStr("DT_PlantedC4"), XorStr("m_flC4Blow"));
		float bombTime = GetFieldValue<float>(m_flC4Blow);
		float returnValue = bombTime - SSDK::I::GlobalVars()->curtime;
		return (returnValue < 0) ? 0.f : returnValue;
	}

	int ShotsFired()
	{
		static int m_iShotsFired = GET_NETVAR(XorStr("DT_CSPlayer"),XorStr("cslocaldata") , XorStr("m_iShotsFired"));
		return GetFieldValue<int>(m_iShotsFired);
	}

	SSDK::Vector& GetAbsOrigin()
	{
		typedef SSDK::Vector&(__thiscall* oGetServrAngles)(void*);
		return SSDK::CallVFunction<oGetServrAngles>(this, 10)(this);
	}

	SSDK::Vector& GetAbsAngles()
	{
		typedef SSDK::Vector&(__thiscall* oGetServrAngles)(PVOID);
		return SSDK::CallVFunction<oGetServrAngles>(this, 11)(this);
	}

	int GetMoveType()
	{
		//****OFFSET2UPDATE*******
		return GetFieldValue<int>(/*m_nMoveType*/0x258);
	}

	SSDK::Vector GetEntityBoneFromMatrix(SSDK::matrix3x4_t mat[], int iBone) {
		SSDK::matrix3x4_t hitbox = mat[iBone];

		return SSDK::Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}

	int *GetModelIndex() {
		static int m_nModelIndex = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_nModelIndex"));
		return GetFieldPointer<int>(m_nModelIndex);
	}
	SSDK::Vector *GetMin() {
		static int m_vecMins = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_Collision"), XorStr("m_vecMins"));
		return GetFieldPointer<SSDK::Vector>(m_vecMins);
	}

	SSDK::Vector *GetMax() {
		static int m_vecMaxs = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_Collision"), XorStr("m_vecMaxs"));
		return GetFieldPointer<SSDK::Vector>(m_vecMaxs);
	}
	SSDK::Vector WorldSpaceCenter() {
		SSDK::Vector Max = *this->GetMax() + *this->GetOrigin();
		SSDK::Vector Min = *this->GetMin() + *this->GetOrigin();
		SSDK::Vector Size = Max - Min;
		Size /= 2;
		Size += Min;
		return Size;
	}

	C_BaseCombatWeapon* GetActiveWeapon() {
		static int m_hActiveWeapon = GET_NETVAR(XorStr("DT_BaseCombatCharacter"), XorStr("m_hActiveWeapon"));
		return (C_BaseCombatWeapon*)SSDK::I::EntityList()->GetClientEntityFromHandle(GetFieldValue<SSDK::CBaseHandle>(m_hActiveWeapon));
	}

     int GetHealth() {
          static int m_iHealth = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_iHealth"));
          return GetFieldValue<int>(m_iHealth);
     }

	 int GetArmor() {
		 static int m_ArmorValue = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_ArmorValue"));
		 return GetFieldValue<int>(m_ArmorValue);
	 }

	 bool HasHelmet() {
		 static int m_bHasHelmet = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_bHasHelmet"));
		 return GetFieldValue<bool>(m_bHasHelmet);
	 }

	 bool* GetSpotted() {
		 static int m_bSpotted = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_bSpotted"));
		 return GetFieldPointer<bool>(m_bSpotted);
	 }

	 int *GetTickBase() {
		 static int m_nTickBase = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_nTickBase"));
		 return GetFieldPointer<int>(m_nTickBase);
	 }

     bool IsAlive() {
          static int m_lifeState = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_lifeState"));
          return GetFieldValue<int>(m_lifeState) == 0;
     }

     int GetTeamNum() {
          static int m_iTeamNum = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_iTeamNum"));
          return GetFieldValue<int>(m_iTeamNum);
     }

     int *GetFlags() {
          static int m_fFlags = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_fFlags"));
          return GetFieldPointer<int>(m_fFlags);
     }

	 SSDK::ObserverMode_t GetObserverMode() {
		 static int m_iObserverMode = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_iObserverMode"));
		 return GetFieldValue<SSDK::ObserverMode_t>(m_iObserverMode);
	 }

	 C_CSPlayer* GetObserved() {
		 static int m_hObserverTarget = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_hObserverTarget"));
		 return (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntityFromHandle(GetFieldValue<SSDK::CBaseHandle>(m_hObserverTarget));
	 }

	 UINT* GetWeapons() {
		 static UINT m_hMyWeapons = GET_NETVAR(XorStr("DT_BaseCombatCharacter"), XorStr("m_hMyWeapons"));
		 return GetFieldPointer<UINT>(m_hMyWeapons);
	 }

	 CBaseViewModel* GetViewModel() {
		 static int m_hViewModel = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_hViewModel[0]"));
		 return (CBaseViewModel*)SSDK::I::EntityList()->GetClientEntityFromHandle(GetFieldValue<int>(m_hViewModel));
	 }

	 SSDK::Vector GetViewOffset() {
          static int m_vecViewOffset = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecViewOffset[0]"));
          return GetFieldValue<SSDK::Vector>(m_vecViewOffset);
     }

	 SSDK::Vector* GetOrigin() {
          static int m_vecOrigin = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_vecOrigin"));
          return GetFieldPointer<SSDK::Vector>(m_vecOrigin);
     }

	 int *GetSequence() {
		 static int m_nSequence = GET_NETVAR(XorStr("DT_BaseAnimating"), XorStr("m_nSequence"));
		 return GetFieldPointer<int>(m_nSequence);		 
	 }

	 float *GetPosePosition() {
		 static int m_flPoseParameter = GET_NETVAR(XorStr("DT_BaseAnimating"), XorStr("m_flPoseParameter"));
		 return GetFieldPointer<float>(m_flPoseParameter);
	 }

	 float *GetCycle() {
		 static float m_flCycle = GET_NETVAR(XorStr("DT_BaseAnimating"), XorStr("serveranimdata"), XorStr("m_flCycle"));
		 return GetFieldPointer<float>(m_flCycle);
	 }

	 SSDK::Vector GetEyePos() {
          return *GetOrigin() + GetViewOffset();
     }

	 SSDK::Vector* ViewPunch() {
          static int m_viewPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_viewPunchAngle"));
          return GetFieldPointer<SSDK::Vector>(m_viewPunchAngle);
     }

	 SSDK::Vector* AimPunch() {
          static int m_aimPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_aimPunchAngle"));
          return GetFieldPointer<SSDK::Vector>(m_aimPunchAngle);
     }

	 int* iFov() {
		 static int m_iFOV = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_iFOV"));
		 return GetFieldPointer<int>(m_iFOV);
	 }

	 float *flashDuration() {
		 static int m_flFlashDuration = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_flFlashDuration"));
		 return GetFieldPointer<float>(m_flFlashDuration);
	 }

	 bool isVisible() {
		 SSDK::Trace_t tr;
		 SSDK::Ray_t ray;
		 SSDK::CTraceFilter filter;
		 filter.pSkip = C_CSPlayer::GetLocalPlayer();

		 SSDK::matrix3x4_t boneMatrix[128];

		 if (!this->SetupBones(boneMatrix, 128, 256, SSDK::I::Engine()->GetLastTimeStamp()))
			 return false;

		 SSDK::matrix3x4_t hitbox = boneMatrix[SSDK::ECSPlayerBones::head_0];
		 //-------------------------

		 ray.Init(this->GetLocalPlayer()->GetEyePos(), SSDK::Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]));
		 SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

		 return tr.m_pEnt == this || tr.fraction > 0.97f;
	 }

	 bool isVectorVisible(SSDK::Vector vec) {
		 SSDK::Trace_t tr;
		 SSDK::Ray_t ray;
		 SSDK::CTraceFilter filter;
		 filter.pSkip = C_CSPlayer::GetLocalPlayer();

		 ray.Init(this->GetLocalPlayer()->GetEyePos(), vec);
		 SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

		 return tr.m_pEnt == this || tr.fraction > 0.97f;
	 }

	 bool isMoving(float value = .1f) {
		return GetVelocity().Length() > value;
	 }

	 DWORD glowIndex() {
		 static int m_glowIndex = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_flFlashDuration")) + 0x18;
		 return GetFieldValue<DWORD>(m_glowIndex);
	 }

	 SSDK::Vector GetVelocity() {
		 static int m_vecVelocity = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecVelocity[0]"));
		 return GetFieldValue<SSDK::Vector>(m_vecVelocity);
	 }

	 SSDK::Vector *GetVelocityPointer() {
		 static int m_vecVelocity = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecVelocity[0]"));
		 return GetFieldPointer<SSDK::Vector>(m_vecVelocity);
	 }

	 bool isScoped() {
		 static int m_bIsScoped = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_bIsScoped"));
		 return GetFieldValue<bool>(m_bIsScoped);
	 }

	 float *GetLowerBodyYaw() {
		 static int m_flLowerBodyYawTarget = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_flLowerBodyYawTarget"));
		 return GetFieldPointer<float>(m_flLowerBodyYawTarget);
	 }

	 SSDK::QAngle *GetEyeAngles() {
		 static int m_angEyeAngles = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_angEyeAngles[0]"));
		 return GetFieldPointer<SSDK::QAngle>(m_angEyeAngles);
	 }

	 float *GetSimulationTime() {
		 static int m_flSimulationTime = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_flSimulationTime"));
		 return GetFieldPointer<float>(m_flSimulationTime);
	 }

	 void DisableInterp() {
		 VarMapping_t *map = (VarMapping_t*)((DWORD)this + 0x24);
		 //Debug
		 //SSDK::I::CVar()->ConsolePrintf("%f\n", map->m_lastInterpolationTime);
		 //SSDK::I::CVar()->ConsolePrintf("%i\n", map->m_nInterpolatedEntries);
		 map->m_nInterpolatedEntries = 0;
		 
		 //PRQ TAS UNCOMMENT CA???????? ???? ??? ?? ?
		 /*for (int i = 0; i < map->m_nInterpolatedEntries; ++i)
		 {
			 VarMapEntry_t *e = &map->m_Entries[i];

			 e->m_bNeedsToInterpolate = false;
		 }*/
	 }

	 bool GetDucking() {
		 static int m_bDucking = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_bDucking"));
		 return GetFieldValue<bool>(m_bDucking);
	 }

	 int *GetLastOcclusion() {
		 static int iLastOcclusionOffset = 0xA30;
		 return GetFieldPointer<int>(iLastOcclusionOffset);
	 }

	 int *GetOcclusionFlag() {
		 static int iOcclusionFlag = 0xA28;
		 return GetFieldPointer<int>(iOcclusionFlag);
	 }
};

class CBaseAttributableItem : public C_CSPlayer {
	template<class T>
	inline T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}
	template<class T>
	T* GetFieldPointer(int offset) {
		return (T*)((DWORD)this + offset);
	}
public:
	int* GetItemDefinitionIndex() {
		static int m_iItemDefinitionIndex = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iItemDefinitionIndex"));
		return GetFieldPointer<int>(m_iItemDefinitionIndex);
	}

	int* GetItemIDHigh() {
		static int m_iItemIDHigh = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iItemIDHigh"));
		return GetFieldPointer<int>(m_iItemIDHigh);
	}

	int* GetEntityQuality() {
		static int m_iEntityQuality = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_iEntityQuality"));
		return GetFieldPointer<int>(m_iEntityQuality);
	}

	char* GetCustomName() {
		static int m_szCustomName = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_AttributeManager"), XorStr("m_Item"), XorStr("m_szCustomName"));
		return GetFieldPointer<char>(m_szCustomName);
	}

	int* GetFallbackPaintKit() {
		static int m_nFallbackPaintKit = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_nFallbackPaintKit"));
		return GetFieldPointer<int>(m_nFallbackPaintKit);
	}

	int* GetFallbackSeed() {
		static int m_nFallbackSeed = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_nFallbackSeed"));
		return GetFieldPointer<int>(m_nFallbackSeed);
	}

	float* GetFallbackWear() {
		static int m_flFallbackWear = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_flFallbackWear"));
		return GetFieldPointer<float>(m_flFallbackWear);
	}

	int* GetFallbackStatTrak() {
		static int m_bSpotted = GET_NETVAR(XorStr("DT_BaseAttributableItem"), XorStr("m_nFallbackStatTrak"));
		return GetFieldPointer<int>(m_bSpotted);
	}
};

class C_BaseCombatWeapon : public SSDK::IClientEntity {
	template<class T>
	inline T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}

public:

	//check if we got bullets
	bool HasBullets() {
		return GetClip() > 0 && !IsReloading();
	}

	CBaseCSGrenade *getGrenade() {
		return (CBaseCSGrenade*)this;
	}

	//Check if isReloading
	bool IsReloading()
	{
		//****OFFSET2UPDATE*******
		return GetFieldValue<bool>(/*m_bInReload*/0x3245);
	}

	//Check if we got ammos
	int GetClip()
	{
		static int m_iClip1 = GET_NETVAR(XorStr("DT_BaseCombatWeapon"), XorStr("m_iClip1"));
		return GetFieldValue<int>(m_iClip1);
	}

	const char* GetNameWeap() {
		CBaseAttributableItem *pAttribWeapon = (CBaseAttributableItem*)this;

		if (!pAttribWeapon)
			return "Unknown";

		int id = *pAttribWeapon->GetItemDefinitionIndex();

		switch (id) {
		case SSDK::EItemDefinitionIndex::weapon_deagle:         return "Desert Eagle";
		case SSDK::EItemDefinitionIndex::weapon_elite:          return "Dual-Elites";
		case SSDK::EItemDefinitionIndex::weapon_fiveseven:      return "Five-Seven";
		case SSDK::EItemDefinitionIndex::weapon_glock:          return "Glock";
		case SSDK::EItemDefinitionIndex::weapon_ak47:           return "AK-47";
		case SSDK::EItemDefinitionIndex::weapon_aug:            return "AUG";
		case SSDK::EItemDefinitionIndex::weapon_awp:            return "AWP";
		case SSDK::EItemDefinitionIndex::weapon_famas:          return "Famas";
		case SSDK::EItemDefinitionIndex::weapon_g3sg1:          return "G3SG1";
		case SSDK::EItemDefinitionIndex::weapon_galilar:        return "Galil";
		case SSDK::EItemDefinitionIndex::weapon_m249:           return "M249";
		case SSDK::EItemDefinitionIndex::weapon_m4a4:           return "M4A4";
		case SSDK::EItemDefinitionIndex::weapon_mac10:          return "MAC-10";
		case SSDK::EItemDefinitionIndex::weapon_p90:            return "P90";
		case SSDK::EItemDefinitionIndex::weapon_ump:            return "UMP";
		case SSDK::EItemDefinitionIndex::weapon_xm1014:         return "XM1014";
		case SSDK::EItemDefinitionIndex::weapon_bizon:          return "PP-Bizon";
		case SSDK::EItemDefinitionIndex::weapon_mag7:           return "MAG7";
		case SSDK::EItemDefinitionIndex::weapon_negev:          return "Negev";
		case SSDK::EItemDefinitionIndex::weapon_sawedoff:       return "Sawed-off";
		case SSDK::EItemDefinitionIndex::weapon_tec9:           return "TEC9";
		case SSDK::EItemDefinitionIndex::weapon_taser:          return "Zeus/Taser";
		case SSDK::EItemDefinitionIndex::weapon_hkp2000:        return "P2000";
		case SSDK::EItemDefinitionIndex::weapon_mp7:            return "MP7";
		case SSDK::EItemDefinitionIndex::weapon_mp9:            return "MP9";
		case SSDK::EItemDefinitionIndex::weapon_nova:           return "Nova";
		case SSDK::EItemDefinitionIndex::weapon_p250:           return "P250";
		case SSDK::EItemDefinitionIndex::weapon_scar20:         return "SCAR20";
		case SSDK::EItemDefinitionIndex::weapon_sg556:          return "SG556";
		case SSDK::EItemDefinitionIndex::weapon_ssg08:          return "SSG-08";
		case SSDK::EItemDefinitionIndex::weapon_flashbang:      return "Flashbang";
		case SSDK::EItemDefinitionIndex::weapon_hegrenade:      return "HE Granade";
		case SSDK::EItemDefinitionIndex::weapon_smokegrenade:   return "Smoke Grenade";
		case SSDK::EItemDefinitionIndex::weapon_molotov:        return "Molotov";
		case SSDK::EItemDefinitionIndex::weapon_decoy:          return "Decoy";
		case SSDK::EItemDefinitionIndex::weapon_incgrenade:     return "Incendiary";
		case SSDK::EItemDefinitionIndex::weapon_c4:             return "C4";
		case SSDK::EItemDefinitionIndex::weapon_m4a1_silencer:  return "M4A1-S";
		case SSDK::EItemDefinitionIndex::weapon_usp_silencer:   return "USP-S";
		case SSDK::EItemDefinitionIndex::weapon_cz75a:          return "CZ-75a";
		case SSDK::EItemDefinitionIndex::weapon_revolver:       return "Revolver";
		case SSDK::EItemDefinitionIndex::weapon_bayonet:
		case SSDK::EItemDefinitionIndex::weapon_knife:
		case SSDK::EItemDefinitionIndex::weapon_knife_t:
		case SSDK::EItemDefinitionIndex::weapon_knife_flip:
		case SSDK::EItemDefinitionIndex::weapon_knife_gut:
		case SSDK::EItemDefinitionIndex::weapon_knife_karambit:
		case SSDK::EItemDefinitionIndex::weapon_knife_m9_bayonet:
		case SSDK::EItemDefinitionIndex::weapon_knife_tactical:
		case SSDK::EItemDefinitionIndex::weapon_knife_falchion:
		case SSDK::EItemDefinitionIndex::weapon_knife_survival_bowie:
		case SSDK::EItemDefinitionIndex::weapon_knife_butterfly:
		case SSDK::EItemDefinitionIndex::weapon_knife_push:
			return "Knife";
		default:
			return "Unknown";
		}
	}

	///Check for good gun
	bool WeaponIsRCSOK()
	{
		if (!this)
			return false;

		int id = this->GetId();

		switch (id)
		{
		case SSDK::EItemDefinitionIndex::weapon_deagle:
		case SSDK::EItemDefinitionIndex::weapon_elite:
		case SSDK::EItemDefinitionIndex::weapon_hkp2000:
		case SSDK::EItemDefinitionIndex::weapon_fiveseven:
		case SSDK::EItemDefinitionIndex::weapon_glock:
		case SSDK::EItemDefinitionIndex::weapon_taser:
		case SSDK::EItemDefinitionIndex::weapon_knife:
		case SSDK::EItemDefinitionIndex::weapon_flashbang:
		case SSDK::EItemDefinitionIndex::weapon_hegrenade:
		case SSDK::EItemDefinitionIndex::weapon_smokegrenade:
		case SSDK::EItemDefinitionIndex::weapon_molotov:
		case SSDK::EItemDefinitionIndex::weapon_decoy:
		case SSDK::EItemDefinitionIndex::weapon_incgrenade:
		case SSDK::EItemDefinitionIndex::weapon_knife_t:
		case SSDK::EItemDefinitionIndex::weapon_usp_silencer:
		case SSDK::EItemDefinitionIndex::weapon_cz75a:
		case SSDK::EItemDefinitionIndex::weapon_revolver:
		case SSDK::EItemDefinitionIndex::weapon_aug:
		case SSDK::EItemDefinitionIndex::weapon_awp:
		case SSDK::EItemDefinitionIndex::weapon_xm1014:
		case SSDK::EItemDefinitionIndex::weapon_sawedoff:
		case SSDK::EItemDefinitionIndex::weapon_mag7:
		case SSDK::EItemDefinitionIndex::weapon_nova:
		case SSDK::EItemDefinitionIndex::weapon_p250:
		case SSDK::EItemDefinitionIndex::weapon_sg556:
		case SSDK::EItemDefinitionIndex::weapon_ssg08:
		case SSDK::EItemDefinitionIndex::weapon_tec9:
			return false;
		default:
			return true;
		}
	}

	bool IsGrenade()
	{
		return this->GetCSWpnData()->iWeaponType == SSDK::CSWeaponType::WEAPONTYPE_GRENADE;
	}

	///Check for good gun
	bool WeaponAA()
	{
		if (!this)
			return false;

		int id = this->GetId();

		switch (id)
		{
		case SSDK::EItemDefinitionIndex::weapon_deagle:
		case SSDK::EItemDefinitionIndex::weapon_elite:
		case SSDK::EItemDefinitionIndex::weapon_fiveseven:
		case SSDK::EItemDefinitionIndex::weapon_glock:
		case SSDK::EItemDefinitionIndex::weapon_ak47:
		case SSDK::EItemDefinitionIndex::weapon_aug:
		case SSDK::EItemDefinitionIndex::weapon_awp:
		case SSDK::EItemDefinitionIndex::weapon_famas:
		case SSDK::EItemDefinitionIndex::weapon_g3sg1:
		case SSDK::EItemDefinitionIndex::weapon_galilar:
		case SSDK::EItemDefinitionIndex::weapon_m249:
		case SSDK::EItemDefinitionIndex::weapon_m4a4:
		case SSDK::EItemDefinitionIndex::weapon_mac10:
		case SSDK::EItemDefinitionIndex::weapon_p90:
		case SSDK::EItemDefinitionIndex::weapon_ump:
		case SSDK::EItemDefinitionIndex::weapon_xm1014:
		case SSDK::EItemDefinitionIndex::weapon_bizon:
		case SSDK::EItemDefinitionIndex::weapon_mag7:
		case SSDK::EItemDefinitionIndex::weapon_negev:
		case SSDK::EItemDefinitionIndex::weapon_sawedoff:
		case SSDK::EItemDefinitionIndex::weapon_tec9:
		case SSDK::EItemDefinitionIndex::weapon_taser:
		case SSDK::EItemDefinitionIndex::weapon_hkp2000:
		case SSDK::EItemDefinitionIndex::weapon_mp7:
		case SSDK::EItemDefinitionIndex::weapon_mp9:
		case SSDK::EItemDefinitionIndex::weapon_nova:
		case SSDK::EItemDefinitionIndex::weapon_p250:
		case SSDK::EItemDefinitionIndex::weapon_scar20:
		case SSDK::EItemDefinitionIndex::weapon_sg556:
		case SSDK::EItemDefinitionIndex::weapon_ssg08:
		case SSDK::EItemDefinitionIndex::weapon_m4a1_silencer:
		case SSDK::EItemDefinitionIndex::weapon_usp_silencer:
		case SSDK::EItemDefinitionIndex::weapon_cz75a:
		case SSDK::EItemDefinitionIndex::weapon_revolver:
		case SSDK::EItemDefinitionIndex::weapon_knife:
		case SSDK::EItemDefinitionIndex::weapon_knife_t:
		case SSDK::EItemDefinitionIndex::weapon_knife_butterfly:
		case SSDK::EItemDefinitionIndex::weapon_knife_falchion:
		case SSDK::EItemDefinitionIndex::weapon_knife_flip:
		case SSDK::EItemDefinitionIndex::weapon_knife_gut:
		case SSDK::EItemDefinitionIndex::weapon_knife_karambit:
		case SSDK::EItemDefinitionIndex::weapon_knife_m9_bayonet:
			return true;
		case SSDK::EItemDefinitionIndex::weapon_c4:
		case SSDK::EItemDefinitionIndex::weapon_flashbang:
		case SSDK::EItemDefinitionIndex::weapon_hegrenade:
		case SSDK::EItemDefinitionIndex::weapon_smokegrenade:
		case SSDK::EItemDefinitionIndex::weapon_molotov:
		case SSDK::EItemDefinitionIndex::weapon_decoy:
		case SSDK::EItemDefinitionIndex::weapon_incgrenade:
			return false;
		default:
			return false;
		}
	}

	///Check for good gun
	bool GetWeaponTypewitham()
	{
		if (!this)
			return false;

		int id = this->GetId();

		switch (id)
		{
		case SSDK::EItemDefinitionIndex::weapon_deagle:
		case SSDK::EItemDefinitionIndex::weapon_elite:
		case SSDK::EItemDefinitionIndex::weapon_fiveseven:
		case SSDK::EItemDefinitionIndex::weapon_glock:
		case SSDK::EItemDefinitionIndex::weapon_ak47:
		case SSDK::EItemDefinitionIndex::weapon_aug:
		case SSDK::EItemDefinitionIndex::weapon_awp:
		case SSDK::EItemDefinitionIndex::weapon_famas:
		case SSDK::EItemDefinitionIndex::weapon_g3sg1:
		case SSDK::EItemDefinitionIndex::weapon_galilar:
		case SSDK::EItemDefinitionIndex::weapon_m249:
		case SSDK::EItemDefinitionIndex::weapon_m4a4:
		case SSDK::EItemDefinitionIndex::weapon_mac10:
		case SSDK::EItemDefinitionIndex::weapon_p90:
		case SSDK::EItemDefinitionIndex::weapon_ump:
		case SSDK::EItemDefinitionIndex::weapon_xm1014:
		case SSDK::EItemDefinitionIndex::weapon_bizon:
		case SSDK::EItemDefinitionIndex::weapon_mag7:
		case SSDK::EItemDefinitionIndex::weapon_negev:
		case SSDK::EItemDefinitionIndex::weapon_sawedoff:
		case SSDK::EItemDefinitionIndex::weapon_tec9:
		case SSDK::EItemDefinitionIndex::weapon_taser:
			return true;
		case SSDK::EItemDefinitionIndex::weapon_hkp2000:
		case SSDK::EItemDefinitionIndex::weapon_mp7:
		case SSDK::EItemDefinitionIndex::weapon_mp9:
		case SSDK::EItemDefinitionIndex::weapon_nova:
		case SSDK::EItemDefinitionIndex::weapon_p250:
		case SSDK::EItemDefinitionIndex::weapon_scar20:
		case SSDK::EItemDefinitionIndex::weapon_sg556:
		case SSDK::EItemDefinitionIndex::weapon_ssg08:
			return true;
		case SSDK::EItemDefinitionIndex::weapon_knife:
		case SSDK::EItemDefinitionIndex::weapon_flashbang:
		case SSDK::EItemDefinitionIndex::weapon_hegrenade:
		case SSDK::EItemDefinitionIndex::weapon_smokegrenade:
		case SSDK::EItemDefinitionIndex::weapon_molotov:
		case SSDK::EItemDefinitionIndex::weapon_decoy:
		case SSDK::EItemDefinitionIndex::weapon_incgrenade:
		case SSDK::EItemDefinitionIndex::weapon_c4:
		case SSDK::EItemDefinitionIndex::weapon_knife_t:
			return false;
		case SSDK::EItemDefinitionIndex::weapon_m4a1_silencer:
		case SSDK::EItemDefinitionIndex::weapon_usp_silencer:
		case SSDK::EItemDefinitionIndex::weapon_cz75a:
		case SSDK::EItemDefinitionIndex::weapon_revolver:
			return true;
		default:
			return false;
		}
	}

	///Weapons with scope
	bool WeaponHasScope()
	{
		if (!this)
			return false;

		int id = this->GetId();

		switch (id)
		{
		case SSDK::EItemDefinitionIndex::weapon_aug:
		case SSDK::EItemDefinitionIndex::weapon_awp:
		case SSDK::EItemDefinitionIndex::weapon_g3sg1:
		case SSDK::EItemDefinitionIndex::weapon_sg556:
		case SSDK::EItemDefinitionIndex::weapon_ssg08:
		case SSDK::EItemDefinitionIndex::weapon_scar20:
			return true;
		default:
			return false;
		}
	}

	///Weapons zoom
	bool WeaponHasZoom()
	{
		if (!this)
			return false;

		int id = this->GetId();

		switch (id)
		{
		case SSDK::EItemDefinitionIndex::weapon_awp:
		case SSDK::EItemDefinitionIndex::weapon_g3sg1:
		case SSDK::EItemDefinitionIndex::weapon_ssg08:
		case SSDK::EItemDefinitionIndex::weapon_scar20:
			return true;
		default:
			return false;
		}
	}

	float NextPrimaryAttack() {
		static int m_flNextPrimaryAttack = GET_NETVAR(XorStr("DT_BaseCombatWeapon"), XorStr("LocalActiveWeaponData"), XorStr("m_flNextPrimaryAttack"));
		return GetFieldValue<float>(m_flNextPrimaryAttack);
	}
	float GetAccuracy() {
		static int m_fAccuracyPenalty = GET_NETVAR(XorStr("DT_WeaponCSBase"), XorStr("m_fAccuracyPenalty"));
		return GetFieldValue<float>(m_fAccuracyPenalty);
	}

	float GetFireReadyTime()
	{
		static int m_flPostponeFireReadyTime = GET_NETVAR(XorStr("DT_WeaponCSBase"), XorStr("m_flPostponeFireReadyTime"));
		return GetFieldValue<float>(m_flPostponeFireReadyTime);
	}

	int GetId() {
		typedef int(__thiscall* tGetId)(void*);
		return SSDK::CallVFunction<tGetId>(this, 458)(this);
	}
	const char* GetName() {
		typedef const char* (__thiscall* tGetName)(void*);
		return SSDK::CallVFunction<tGetName>(this, 378)(this);
	}

	float GetCone() {
		typedef float(__thiscall* OriginalFn)(void*);
		return SSDK::CallVFunction<OriginalFn>(this, 483)(this);
	}
	float GetSpread() {
		typedef float(__thiscall* OriginalFn)(void*);
		return SSDK::CallVFunction<OriginalFn>(this, 484)(this);
	}
	void UpdateAccuracy() {
		typedef void(__thiscall* OriginalFn)(void*);
		SSDK::CallVFunction<OriginalFn>(this, 485)(this);
	}

	SSDK::WeaponInfo_t* GetCSWpnData()
	{
		typedef SSDK::WeaponInfo_t*(__thiscall* OriginalFn)(void*);
		return SSDK::CallVFunction<OriginalFn>(this, 456)(this);
	}

	float GetThrowTime()
	{
		static int m_fThrowTime = GET_NETVAR(XorStr("DT_BaseCSGrenade"), XorStr("m_fThrowTime"));
		return GetFieldValue<float>(m_fThrowTime);
	}
};