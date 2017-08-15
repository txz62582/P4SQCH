#include "CAutoWall.h"
#include "P4Menu\Config.h"
#include "CLagCompensation2.h"


//1
void CAutowall::UTIL_TraceLine(const SSDK::Vector& vecAbsStart, const SSDK::Vector& vecAbsEnd, unsigned int mask, SSDK::CTraceFilter* filter, SSDK::Trace_t* ptr)
{
	SSDK::Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);

	SSDK::I::EngineTrace()->TraceRay(ray, mask, filter, ptr);
}

void CAutowall::UTIL_ClipTraceToPlayers(SSDK::Vector& vecAbsStart, SSDK::Vector& vecAbsEnd, unsigned int mask, SSDK::ITraceFilter* filter, SSDK::Trace_t* tr)
{
	//CallFromGameMtfk
	typedef void(__thiscall* UTIL_ClipTraceToPlayersFn)(const SSDK::Vector&, const SSDK::Vector&, unsigned int, SSDK::ITraceFilter*, SSDK::Trace_t*);
	static UTIL_ClipTraceToPlayersFn clptrtp = reinterpret_cast<UTIL_ClipTraceToPlayersFn>(SSDK::I::FindSignature(XorStr("client.dll"), XorStr("53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10")));
	__asm {
		push tr
		push filter
		push mask
		lea edx, vecAbsEnd
		lea ecx, vecAbsStart
		call clptrtp
		add esp, 0xC
	}
}

float CAutowall::GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
		return 1.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	default:
		return 1.0f;
	}
}

void CAutowall::ScaleDamage(int hitgroup, C_CSPlayer *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);

	if (enemy->GetArmor() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= weapon_armor_ratio * 0.5f;
		}
		else
			current_damage *= weapon_armor_ratio * 0.5f;
	}
}

bool CAutowall::IsArmored(C_CSPlayer* pEntity, int iArmorValue, int iHitgroup)
{
	bool result = false;

	if (iArmorValue)
	{
		switch (iHitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			result = true;
			break;
		case HITGROUP_HEAD:
			result = pEntity->HasHelmet();
			break;
		}
	}

	return result;
}

float CAutowall::ScaleDamageReversed(C_CSPlayer* pEntity, float flDamage, float flWeaponArmorRatio, int iHitgroup)
{
	auto ArmorValue = pEntity->GetArmor();

	switch (iHitgroup)
	{
	case HITGROUP_HEAD:
		flDamage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		flDamage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		flDamage *= 0.75f;
		break;
	}

	if (IsArmored(pEntity, ArmorValue, iHitgroup))
	{
		float ArmorBonusRatio = 0.5f, ArmorRatio = flWeaponArmorRatio * 0.5f;

		auto NewDamage = flDamage * ArmorRatio;

		if (((flDamage - (flDamage * ArmorRatio)) * ArmorBonusRatio) > ArmorValue)
			NewDamage = flDamage - (ArmorValue / ArmorBonusRatio);

		flDamage = NewDamage;
	}

	return flDamage;
}

bool CAutowall::TraceToExit(SSDK::Vector& end, SSDK::Trace_t& tr, float x, float y, float z, float x2, float y2, float z2, SSDK::Trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(SSDK::Vector&, SSDK::Trace_t&, float, float, float, float, float, float, SSDK::Trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)SSDK::I::FindSignature("client.dll", "55 8B EC 83 EC 30 F3 0F 10 75");

	if (!TraceToExit)
	{
		return false;
	}
	_asm
	{
		push trace
		push z2
		push y2
		push x2
		push z
		push y
		push x
		mov edx, tr
		mov ecx, end
		call TraceToExit
		add esp, 0x1C
	}
}

bool CAutowall::TraceToExit1(SSDK::Vector &end, SSDK::Trace_t *enter_trace, SSDK::Vector start, SSDK::Vector dir, SSDK::Trace_t *exit_trace)
{
	float distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = SSDK::I::EngineTrace()->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		auto new_end = end - (dir * 4.0f);

		SSDK::Ray_t ray;
		ray.Init(end, new_end);
		SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			ray.Init(end, start);

			SSDK::CTraceFilter filter;
			filter.pSkip = exit_trace->m_pEnt;

			SSDK::I::EngineTrace()->TraceRay(ray, 0x600400B, &filter, exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}

			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
		{
			if (exit_trace->m_pEnt)
			{
				if (enter_trace->m_pEnt && enter_trace->m_pEnt == SSDK::I::EntityList()->GetClientEntity(0))
					return true;
			}

			continue;
		}

		if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}

	return false;
}


bool CAutowall::HandleBulletPenetration(SSDK::WeaponInfo_t *wpn_data, FireBulletData &data)
{
	SSDK::surfacedata_t *enter_surface_data = SSDK::I::PhysicsProps()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = *(float*)((DWORD)enter_surface_data + 76);

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow((wpn_data->flRangeModifier), (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	SSDK::Vector dummy;
	SSDK::Trace_t trace_exit;
	if (!TraceToExit1(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	SSDK::surfacedata_t *exit_surface_data = SSDK::I::PhysicsProps()->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = *(float*)((DWORD)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->flPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

//OLD AUTOWALL SA MERE, ca pue grave.
/*
bool CAutowall::SimulateFireBullet(C_BaseCombatWeapon* pWeapon, FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	SSDK::WeaponInfo_t* weaponData = C_CSPlayer::GetLocalPlayer()->GetActiveWeapon()->GetCSWpnData();

	if (weaponData == NULL)
		return false;

	data.current_damage = (float)weaponData->iDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weaponData->flRange - data.trace_length;

		SSDK::Vector end = data.src + data.direction * data.trace_length_remaining;

		//data.enter_trace
		UTIL_TraceLine(data.src, end, MASK_SHOT, C_CSPlayer::GetLocalPlayer(), &data.enter_trace);

		SSDK::Ray_t ray;
		ray.Init(data.src, end + data.direction*40.f);

		SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, &data.filter, &data.enter_trace);

		UTIL_TraceLine(data.src, end + data.direction*40.f, MASK_SHOT, C_CSPlayer::GetLocalPlayer(), &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(weaponData->flRangeModifier, data.trace_length * 0.002);

			C_CSPlayer* player = (C_CSPlayer*)data.enter_trace.m_pEnt;

			if (player->GetTeamNum() == C_CSPlayer::GetLocalPlayer()->GetTeamNum())
				return false;

			ScaleDamage(data.enter_trace.hitgroup, (C_CSPlayer*)data.enter_trace.m_pEnt, weaponData->flArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponData, data))
			break;
	}

	return false;
}

float CAutowall::GetDamage(const SSDK::Vector& point)
{
	float damage = 0.f;
	SSDK::Vector dst = point;
	FireBulletData *data = new FireBulletData(C_CSPlayer::GetLocalPlayer()->GetEyePos());
	data->filter.pSkip = C_CSPlayer::GetLocalPlayer();

	SSDK::QAngle angles = Utils::CalcAngle(data->src, dst);

	Utils::AngleVectors(angles, &data->direction);
	SSDK::Vector dataNormalized;

	Utils::Normalize(data->direction);

	C_BaseCombatWeapon* pWeap = C_CSPlayer::GetLocalPlayer()->GetActiveWeapon();
	if (SimulateFireBullet(pWeap, *data))
	{
		damage = data->current_damage;
	}

	return damage;
}*/


float CAutowall::BestHitPoint(C_CSPlayer* pEntity, int iPrioritized, float flMinDmg, SSDK::mstudiohitboxset_t* hitset, SSDK::matrix3x4a_t matrix[], SSDK::Vector& vecOut) {
	SSDK::mstudiobbox_t *hitbox = hitset->pHitbox(iPrioritized);

	if (!hitbox)
		return 0.f;

	SSDK::Vector vecArray[5];
	float flHigherDamage = 0.f;

	SSDK::Vector max;
	SSDK::Vector min;
	Utils::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], max);
	Utils::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	SSDK::QAngle CurrentAngles = Utils::CalcAngle(center, C_CSPlayer::GetLocalPlayer()->GetEyePos());

	SSDK::Vector Forward;
	Utils::AngleVectors(CurrentAngles, &Forward);

	SSDK::Vector Right = Forward.Cross(SSDK::Vector(0, 0, 1));
	SSDK::Vector Left = SSDK::Vector(-Right.x, -Right.y, Right.z);

	SSDK::Vector Top = SSDK::Vector(0, 0, 1);
	SSDK::Vector Bot = SSDK::Vector(0, 0, -1);

	//pSetts
	//Head : 0.8 -> Non. C'est 0.783
	//Arms : 0.7 

	SSDK::Vector RightPoint = Right * (hitbox->m_flRadius * (iPrioritized == SSDK::HitboxEnum_t::HITBOX_HEAD ? MenuOptions.Aimbot.g_headPointScale : MenuOptions.Aimbot.g_armsPointScale)) + center;
	SSDK::Vector LeftPoint = Left * (hitbox->m_flRadius * (iPrioritized == SSDK::HitboxEnum_t::HITBOX_HEAD ? MenuOptions.Aimbot.g_headPointScale : MenuOptions.Aimbot.g_armsPointScale)) + center;
	SSDK::Vector TopPoint = Top * (hitbox->m_flRadius * (iPrioritized == SSDK::HitboxEnum_t::HITBOX_HEAD ? MenuOptions.Aimbot.g_headPointScale : MenuOptions.Aimbot.g_armsPointScale)) + center;
	SSDK::Vector BotPoint = Bot * (hitbox->m_flRadius * (iPrioritized == SSDK::HitboxEnum_t::HITBOX_HEAD ? MenuOptions.Aimbot.g_headPointScale : MenuOptions.Aimbot.g_armsPointScale)) + center;

	switch (iPrioritized)
	{
	case SSDK::HitboxEnum_t::HITBOX_HEAD:
		vecArray[0] = TopPoint;
		vecArray[1] = RightPoint;
		vecArray[2] = LeftPoint;
		vecArray[3] = BotPoint;
		break;
	case SSDK::HitboxEnum_t::HITBOX_NECK:
		vecArray[0] = TopPoint;
		vecArray[1] = RightPoint;
		vecArray[2] = LeftPoint;
		vecArray[3] = BotPoint;
		break;
	case SSDK::HitboxEnum_t::HITBOX_LOWER_NECK:
		vecArray[0] = TopPoint;
		vecArray[1] = RightPoint;
		vecArray[2] = LeftPoint;
		vecArray[3] = BotPoint;
		break;
	case SSDK::HitboxEnum_t::HITBOX_PELVIS:
		vecArray[0] = TopPoint;
		vecArray[1] = RightPoint;
		vecArray[2] = LeftPoint;
		vecArray[3] = BotPoint;
		break;
	default:
		vecArray[0] = TopPoint;
		vecArray[1] = RightPoint;
		vecArray[2] = LeftPoint;
		break;
	}

	for (SSDK::Vector cur : vecArray) {
		if (!cur.IsValid())
			continue;

		float flCurDamage;

		if (!CanAutowall(pEntity, cur, &flCurDamage))
			continue;

		if (!(flCurDamage > 0.f))
			continue;

		if ((flCurDamage > flHigherDamage) && (flCurDamage > flMinDmg)) {
			flHigherDamage = flCurDamage;
			vecOut = cur;
		}
	}
	return flHigherDamage;
}

bool CAutowall::Hitscan(C_CSPlayer* pEntity, int iPrioritized, bool bAutomatic, float flMinDmg, float flMinDmgPrio, SSDK::Vector& vecOut) {
	SSDK::matrix3x4a_t matrix[128];

	if (!pEntity->SetupBones(matrix, 128, 256, SSDK::I::Engine()->GetLastTimeStamp()))
		return false;

	SSDK::studiohdr_t* hdr = SSDK::I::ModelInfo()->GetStudiomodel(pEntity->GetModel());
	SSDK::mstudiohitboxset_t* set = hdr->pHitboxSet(0);

	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();
	C_BaseCombatWeapon *activeWeapon = pLocalPlayer->GetActiveWeapon();

	if (pLocalPlayer && activeWeapon)
	{
		//Options menu
		MenuOptions.SetDefaultAwallValues();
		if (activeWeapon)
		{
			MenuOptions.CheckWeapons(activeWeapon);
		}

		if (MenuOptions.Overwritten.g_adaptiveMinDmg) 
		{
			if (!pEntity->isDucking() && pEntity->isMoving())
			{
				flMinDmg = MenuOptions.Overwritten.g_minDmgPeeking;
				//flMinDmgPrio = MenuOptions.Aimbot.g_minDmgPeeking;
			}

			if (pLocalPlayer->isMoving())
			{
				flMinDmg = MenuOptions.Overwritten.g_minDmgAdapt;
				//flMinDmgPrio = MenuOptions.Aimbot.g_minDmgAdapt;

				//Current fix for peeking ppl
				//Tu peux faire une option jsute pour ca si t'as envie. (OK)
			}
		}


		float flHigherDamage = 0.f;
		float flCurDamage = 0.f;

		SSDK::Vector vecBestHitPos;

		//Auto Hitscan
		//Si on trouve pas de tete bah on hitbox toute la hitbox

		if (!bAutomatic) {
			if (BestHitPoint(pEntity, iPrioritized, flMinDmgPrio, set, matrix, vecOut) > 0.f)
				return true;
		}

		for (int i = 0; i < SSDK::HitboxEnum_t::HITBOX_MAX; ++i) {
			flCurDamage = BestHitPoint(pEntity, i, flMinDmg, set, matrix, vecBestHitPos);

			if (flCurDamage > flHigherDamage) {
				flHigherDamage = flCurDamage;
				vecOut = vecBestHitPos;
			}
		}

		if (flHigherDamage > 0.f)
			return true;

		return false;
	}
}

SSDK::Trace_t CAutowall::trace;
SSDK::Trace_t CAutowall::trace2;

bool CAutowall::bTraceExit(SSDK::Vector &vStartyPosy, SSDK::Vector &vDir)
{
	float multiply = 0;

	while (multiply < 90.1f)
	{
		multiply += 4.0f;

		auto vDaStart = (vDir * multiply) + vStartyPosy;
		auto vDaEnd = vDaStart - (vDir * 4.0f);

		auto point_contents = (SSDK::I::EngineTrace()->GetPointContents(vDaStart, 0x4600400B));

		SSDK::Ray_t ray2;

		if ((point_contents & 0x600400B) && (!(point_contents & 0x40000000)))
		{
			continue;
		}

		ray2.Init(vDaStart, vDaEnd);
		SSDK::I::EngineTrace()->TraceRay(ray2, 0x4600400B, nullptr, &trace2);

		if (trace2.startsolid && (trace2.surface.flags & 0x8000))
		{
			auto pNewTraceEnt = trace2.m_pEnt;

			SSDK::CSimpleTraceFilter tracefilter_new(pNewTraceEnt);

			SSDK::Ray_t ray3;
			ray3.Init(vDaStart, vStartyPosy);
			SSDK::I::EngineTrace()->TraceRay(ray3, 0x600400B, reinterpret_cast<SSDK::CTraceFilter*>(&tracefilter_new), &trace2);

			if ((trace2.fraction < 1.0f || trace2.allsolid || trace2.startsolid) && !trace2.startsolid)
			{
				vDaStart = trace2.endpos;
				return true;
			}

			continue;
		}

		if (!(trace2.fraction < 1.0f || trace2.allsolid || trace2.startsolid) || trace2.startsolid)
		{
			auto pBreakEnt = trace.m_pEnt;

			if (pBreakEnt)
			{
				auto pWorldEnt = SSDK::I::EntityList()->GetClientEntity(0);

				if (pBreakEnt != pWorldEnt)
				{
					static DWORD dwEntBreakable = NULL;

					if (dwEntBreakable == NULL)
					{
						dwEntBreakable = SSDK::I::FindPattern(XorStr("client.dll"), reinterpret_cast<PBYTE>(XorStr("\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68")), XorStr("xxxxxxxxxxx"));
					}
					bool bRet;

					__asm
					{
						MOV ECX, pBreakEnt
						CALL dwEntBreakable
						MOV bRet, AL
					}
					if (bRet)
						return true;
				}
			}

			continue;
		}

		if (((trace2.surface.flags >> 7) & 1) && !((trace.surface.flags >> 7) & 1))
			continue;

		auto fWanker = ((trace2.plane.normal.x * vDir.x) + (trace2.plane.normal.y * vDir.y) + (trace2.plane.normal.z * vDir.z));

		if (fWanker < 1.1f)
		{
			auto fMody = trace2.fraction * 4.0f;

			auto vGody = vDaStart - (vDir * fMody);

			vDaStart = vGody;
			return true;
		}
	}
	return false;
}

float CAutowall::Invasive(SSDK::Vector vEnd)
{
	auto pWeapon = C_CSPlayer::GetLocalPlayer()->GetActiveWeapon();
	if (!pWeapon) return 0.f;
	auto wiWeaponInfo = pWeapon->GetCSWpnData();
	auto vStart = C_CSPlayer::GetLocalPlayer()->GetEyePos();

	static SSDK::Trace_t traceEmpty;

	trace = traceEmpty;
	static SSDK::CTraceFilterWorldAndPropsOnly tfNoPlayers;

	static auto pTraceFilter = &tfNoPlayers;

	SSDK::Vector vSource = vStart, vDir = (vEnd - vStart), vClip;
	vDir.NormalizeInPlace();

	float fTmpLength, fSumDist, fPow;

	float iCurrentDamage = (float)wiWeaponInfo->iDamage;


	auto fDamageCheck = 1.0f;

	auto iPenetration = 4;

	static DWORD dwCliptracetoplayers = NULL;

	if (dwCliptracetoplayers == NULL)
	{
		dwCliptracetoplayers = SSDK::I::FindPattern(XorStr("client.dll"), reinterpret_cast<PBYTE>(XorStr("\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10")), XorStr("xxxxxxxxxxxxxxxxxxxxxxxx????xxx"));

	}


	auto fRange = SSDK::Vector(vEnd - vStart).Length();


	while (iCurrentDamage > 0.0f)
	{
		SSDK::Ray_t ray;
		ray.Init(vSource, vEnd);

		SSDK::I::EngineTrace()->TraceRay(ray, 0x4600400B, pTraceFilter, &trace);

		vClip = (vDir * 40.0f) + vEnd;


		_asm
		{
			MOV EAX, pTraceFilter
			LEA ECX, trace
			PUSH ECX
			PUSH EAX
			PUSH 0x4600400B
			LEA EDX, vClip
			LEA ECX, vSource
			CALL dwCliptracetoplayers
			ADD ESP, 0x0C
		}


		if (trace.fraction != 1.0f)
		{

			auto pSurfaceData = SSDK::I::PhysicsProps()->GetSurfaceData(trace.surface.surfaceProps);

			auto material = *reinterpret_cast<USHORT*>(reinterpret_cast<DWORD>(pSurfaceData) + 84);
			auto fPenetrationPowerModifier = *reinterpret_cast<PFLOAT>(reinterpret_cast<DWORD>(pSurfaceData) + 76);

			if (iPenetration == 4)
				fTmpLength = fRange * trace.fraction;
			else
				fTmpLength = (fSumDist + ((fRange - fSumDist) * trace.fraction));

			fPow = (pow(wiWeaponInfo->flRangeModifier, (fTmpLength * 0.002f)));

			iCurrentDamage = iCurrentDamage * fPow;
			bool a5 = (trace.contents >> 3) & 1;

			bool v104 = (trace.surface.flags >> 7) & 1;

			auto v17 = false;

			if (iPenetration == 0 && !a5 && !v104 && material != 71 && material != 89)
				v17 = true;

			if (wiWeaponInfo->flPenetration < 0.0000001f || iPenetration < 1)
				v17 = true;

			trace2 = traceEmpty;

			auto vStartyPosy = trace.endpos;

			if (!bTraceExit(vStartyPosy, vDir)
				&& !(SSDK::I::EngineTrace()->GetPointContents(vStartyPosy, 0x600400B) & 0x600400B))
			{
				break;
			}

			if (v17)
			{
				break;
			}

			auto pSurfaceData2 = SSDK::I::PhysicsProps()->GetSurfaceData(trace2.surface.surfaceProps);

			auto material2 = *reinterpret_cast<USHORT*>(reinterpret_cast<DWORD>(pSurfaceData2) + 84);
			auto fPenetrationPowerModifer2 = *reinterpret_cast<PFLOAT>(reinterpret_cast<DWORD>(pSurfaceData2) + 76);

			auto v70 = 0.16f;
			float v31;

			if (a5 || v104)
			{
				if (material != 89 && material != 71)
					v31 = 1.0f;
				else
				{
					v31 = 3.0f;
					v70 = 0.05f;
				}
			}
			else
			{
				if (material != 89)
				{
					if (material != 71)
					{
						if (material == 70)
						{
							v31 = wiWeaponInfo->flPenetration;
						}
						else
						{

							v31 = (fPenetrationPowerModifer2 + fPenetrationPowerModifier) * 0.5f;
							v70 = 0.16f;
						}
					}
					else
					{
						v31 = 3.0f;
						v70 = 0.05f;
					}
				}
				else
				{
					v31 = 3.0f;
					v70 = 0.05f;
				}
			}

			if (material == material2)
			{
				if (material2 == 87 || material2 == 85)
					v31 = 3.0f;
				else
				{
					if (material2 == 76)
						v31 = 2.0f;
				}
			}

			auto v34 = 1.0f;
			v34 /= v31;

			if (v34 < 0.0f)
				v34 = 0.0f;

			auto fNewTmpLength = SSDK::Vector(trace2.endpos - trace.endpos).Length();

			auto fMine = 3.0f;
			fMine /= wiWeaponInfo->flPenetration;
			fMine *= 1.25f;

			auto v55 = 0.0f;

			if (fMine > 0.0f)
				v55 = fMine;

			auto v35 = v34;
			v35 *= 3.0f;
			v35 *= v55;

			v35 += (iCurrentDamage * v70);

			auto fShiz = fNewTmpLength;
			fShiz *= fNewTmpLength;
			fShiz *= v34;
			fShiz /= 24.0f;

			auto fBisc = (v35 + fShiz);

			auto v69 = 0.0f;

			if (fBisc > 0.0f)
				v69 = fBisc;

			auto fNewy = (iCurrentDamage - v69);

			iCurrentDamage = fNewy;

			if (fNewy < fDamageCheck)
				break;

			vSource = trace2.endpos;

			fSumDist = fTmpLength + fNewTmpLength;

			--iPenetration;
		}
		else
			return iCurrentDamage;
	}
	return 0.f;
}

bool CAutowall::CanAutowall(C_CSPlayer *pEntity, SSDK::Vector vecEnd, float *damage) {
	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();
	C_BaseCombatWeapon *pActiveWeapon = pLocalPlayer->GetActiveWeapon();

	if (!pEntity || !pLocalPlayer || !pActiveWeapon)
		return false;

	static bool(__thiscall *BulletHandler)(C_CSPlayer *me, float &penetration, int &SurfaceMat, int *usestaticvalues, SSDK::Trace_t *ray, SSDK::Vector *normaldelta,
		float _setto0f_8, float surfacepenetration, float damagemultiplier, int unknown, int _setto0x1002_12, float penetration2, int *hitsleft,
		SSDK::Vector *ResultPos, float hitx, float hity, float *damage) = 0;

	if (!BulletHandler)
		BulletHandler = (decltype(BulletHandler))(SSDK::I::FindSignature(XorStr("client.dll"), XorStr("59 74 14")) - 0x4C);

	SSDK::Trace_t enter_tr;

	SSDK::WeaponInfo_t *awInfo = pActiveWeapon->GetCSWpnData();

	if (!awInfo)
		return false;

	*damage = (float)awInfo->iDamage;

	SSDK::Vector vecSrc = pLocalPlayer->GetEyePos();

	SSDK::Vector vecDir = vecEnd - vecSrc;
	vecDir.NormalizeInPlace();

	int usestaticvalues = 0;
	int hitsleft = 4;
	SSDK::Vector result(vecSrc);

	MenuOptions.Menu.isAwalling = true;

	do {
		SSDK::CTraceFilter filter;
		filter.pSkip = pLocalPlayer;

		UTIL_TraceLine(result, vecEnd, MASK_SHOT, &filter, &enter_tr);
		{
			// Check for player hitboxes extending outside their collision bounds
			UTIL_ClipTraceToPlayers(result, vecEnd + vecDir * 40.f, MASK_SHOT, &filter, &enter_tr);
		}

		if (enter_tr.hitgroup != 0 || enter_tr.fraction == 1.f)
			break;

		SSDK::surfacedata_t *entersurf = SSDK::I::PhysicsProps()->GetSurfaceData(enter_tr.surface.surfaceProps);
		int material = entersurf->game.material;

		if (BulletHandler(pLocalPlayer, awInfo->flPenetration, material, &usestaticvalues, &enter_tr, &vecDir, 0.f, entersurf->game.flPenetrationModifier, entersurf->game.flDamageModifier, 0, 0x1002, awInfo->flPenetration, &hitsleft, &result, 0.f, 0.f, damage)) {
			break;
		}

	} while (hitsleft > 0);

	MenuOptions.Menu.isAwalling = false;

	if (enter_tr.hitgroup != 0 || enter_tr.fraction == 1.f) {
		*damage *= pow(awInfo->flRangeModifier, vecSrc.DistTo(result) * 0.002);
		*damage = ScaleDamageReversed(pEntity, *damage, awInfo->flArmorRatio, enter_tr.hitgroup);

		return (enter_tr.m_pEnt && enter_tr.m_pEnt == pEntity) || !enter_tr.m_pEnt;
	}


	return false;
}

C_CSPlayer* CAutowall::FindLowestHPTarget(SSDK::Vector& vecOut,bool atTargets) {
	C_CSPlayer *pLowestHPTarget = 0x0;
	for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
		C_CSPlayer *pEntity = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);
		C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();
		C_BaseCombatWeapon *activeWeapon = pLocalPlayer->GetActiveWeapon();

		//Is it a valid target?
		if (!Utils::isValidPlayer(pEntity))
			continue;

		if (!(pLocalPlayer->GetTeamNum() != pEntity->GetTeamNum()))
			continue;

		SSDK::Vector vecHitPos;
		bool hitscan;

		//Options menu
		MenuOptions.SetDefaultAwallValues();
		if (activeWeapon)
		{
			MenuOptions.CheckWeapons(activeWeapon);
		}
		MenuOptions.CheckTargetList(i);

		//CHECK SI ADAPTIVE MINDMG ET METS HITCHANCE A 0
		if (MenuOptions.Overwritten.g_adaptiveMinDmg)
		{
			if (pLocalPlayer->isMoving())
			{
				MenuOptions.Overwritten.g_hitChance = MenuOptions.Overwritten.g_hitchanceAdapt;
			}
		}

		//AUTOBAIM
		if (MenuOptions.Overwritten.g_autoBaim && ((MenuOptions.Overwritten.g_shotsAutoBaim && (MenuOptions.Menu.shotsFired > MenuOptions.Overwritten.shotsFiredMin)) || (MenuOptions.Overwritten.g_healthAutoBaim && (pLocalPlayer->GetHealth() <= MenuOptions.Overwritten.baimMinHealth))))
		{
			hitscan = CAutowall::Hitscan(pEntity, MenuOptions.Overwritten.hitboxbaim, false, MenuOptions.Overwritten.g_minDamage, MenuOptions.Overwritten.g_minDamage, vecHitPos);
		}
		//Noautobaim
		else
		{
			hitscan = CAutowall::Hitscan(pEntity, MenuOptions.Overwritten.hitbox, MenuOptions.Overwritten.g_autoHitbox, MenuOptions.Overwritten.g_minDamage, MenuOptions.Overwritten.g_minDamagePrio, vecHitPos);
		}
		
		if (!hitscan)
			continue;

		if (!pLowestHPTarget) {
			pLowestHPTarget = pEntity;
			vecOut = vecHitPos;
		}

		if (pLowestHPTarget->GetHealth() > pEntity->GetHealth()) {
			pLowestHPTarget = pEntity;
			vecOut = vecHitPos;
		}
	}
	return pLowestHPTarget;
}