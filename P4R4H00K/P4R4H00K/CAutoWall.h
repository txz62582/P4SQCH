#pragma once

#include "SqchSDK\SDK.h"

#include "EntityManager.h"

#include "Utils.h"

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

struct FireBulletData
{
	FireBulletData(const SSDK::Vector &eye_pos)
		: src(eye_pos)
	{
	}

	SSDK::Vector           src;
	SSDK::Trace_t          enter_trace;
	SSDK::Vector           direction;
	SSDK::CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};


class CAutowall
{
public:
	//1
	static void UTIL_TraceLine(const SSDK::Vector& vecAbsStart, const SSDK::Vector& vecAbsEnd, unsigned int mask, SSDK::CTraceFilter* filter, SSDK::Trace_t* ptr);
	static void UTIL_ClipTraceToPlayers(SSDK::Vector& vecAbsStart, SSDK::Vector& vecAbsEnd, unsigned int mask, SSDK::ITraceFilter* filter, SSDK::Trace_t* tr);
	static void ScaleDamage(int hitgroup, C_CSPlayer *enemy, float weapon_armor_ratio, float &current_damage);
	static bool TraceToExit(SSDK::Vector& end, SSDK::Trace_t& tr, float x, float y, float z, float x2, float y2, float z2, SSDK::Trace_t* trace);
	static bool TraceToExit1(SSDK::Vector &end, SSDK::Trace_t *enter_trace, SSDK::Vector start, SSDK::Vector dir, SSDK::Trace_t *exit_trace);
	static float GetHitgroupDamageMultiplier(int iHitGroup);
	static bool HandleBulletPenetration(SSDK::WeaponInfo_t* weaponInfo, FireBulletData &data);
	static bool IsArmored(C_CSPlayer* pEntity, int iArmorValue, int iHitgroup);
	static float ScaleDamageReversed(C_CSPlayer* pEntity, float flDamage, float flWeaponArmorRatio, int iHitgroup);
	//OLD
	/*
	static bool SimulateFireBullet(C_BaseCombatWeapon* pWeapon, FireBulletData &data);
	static float GetDamage(const SSDK::Vector& point);*/

	//2
	static bool bTraceExit(SSDK::Vector &vStartyPosy, SSDK::Vector &vDir);
	static float Invasive(SSDK::Vector vEnd);

	//3 pAutoWall
	static bool CAutowall::CanAutowall(C_CSPlayer *other, SSDK::Vector endpos, float *damage);

	static float BestHitPoint(C_CSPlayer* pEntity, int prioritized, float minDmg, SSDK::mstudiohitboxset_t* hitset, SSDK::matrix3x4a_t matrix[], SSDK::Vector& vecOut);
	static bool Hitscan(C_CSPlayer* pEntity, int iPrioritized, bool bAutomatic, float flMinDmg, float flMinDmgPrio, SSDK::Vector& vecOut);
	static C_CSPlayer* FindLowestHPTarget(SSDK::Vector& vecOut, bool atTargets = false);
private:
	static SSDK::Trace_t trace;
	static SSDK::Trace_t trace2;
};