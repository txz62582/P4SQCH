#include "Misc.h"

float Misc::GetTraceFractionWorldProps(SSDK::Vector startpos, SSDK::Vector endpos) {
	SSDK::Ray_t ray;
	SSDK::Trace_t tr;
	SSDK::CTraceFilterWorldOnly filter;

	ray.Init(startpos, endpos);

	SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

	return tr.fraction;
}

void Misc::CircleStrafer(SSDK::CUserCmd* pCmd, bool& active)
{
	static float StrafeAngle;

	C_CSPlayer *pLocal = C_CSPlayer::GetLocalPlayer();
	SSDK::Vector Velocity = pLocal->GetVelocity();
	Velocity.z = 0;

	float Speed = Velocity.Length();
	if (Speed < 45) Speed = 45;
	if (Speed > 750) Speed = 750;


	float FinalPath = GetTraceFractionWorldProps((*pLocal->GetOrigin()) + SSDK::Vector(0, 0, 10), (*pLocal->GetOrigin()) + SSDK::Vector(0, 0, 10) + Velocity / 2.0f);
	float DeltaAngle = fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / SSDK::I::GlobalVars()->interval_per_tick)) * 4.f, 2.0f);
	StrafeAngle += DeltaAngle;

	if (fabs(StrafeAngle) >= 360.0f) {
		StrafeAngle = 0.0f;
		active = false;
	}
	else {
		pCmd->forwardmove = cos((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
		pCmd->sidemove = sin((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
	}
}

void Misc::ForceThirdPerson(SSDK::CUserCmd* pCmd, C_CSPlayer* pLocalPlayer)
{
	static bool ThirdPerson = false;

	if (SSDK::I::Engine()->isConnected())
	{
		if (pLocalPlayer)
		{
			if (MenuOptions.Misc.g_thirdperson)
			{
				if (pLocalPlayer->IsAlive())
					SSDK::I::Engine()->ExecuteClientCmd("thirdperson");
				else
					SSDK::I::Engine()->ExecuteClientCmd("firstperson");

				ThirdPerson = true;
			}
		}
	}

	if (!MenuOptions.Misc.g_thirdperson && ThirdPerson)
	{
		ThirdPerson = false;
		SSDK::I::Engine()->ExecuteClientCmd("firstperson");
	}
}

template<class T, class U>
T Misc::Clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void Misc::SpoofConvars()
{
	auto svcheats = SSDK::I::CVar()->FindVar("sv_cheats");
	auto svcheatsspoof = new SSDK::SpoofedConvar(svcheats);
	svcheatsspoof->SetInt(1);
}

//Chat spammer
//Spamtype: 
//0 = spamchat
//1 = spamjokes
void Misc::Chatspam(int spamType)
{
	C_CSPlayer *pLocalPlayer = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetLocalPlayer());
	if (pLocalPlayer)
	{
		float flServerTime = *pLocalPlayer->GetTickBase() * SSDK::I::GlobalVars()->interval_per_tick;

		if (flServerTime >= MenuOptions.Menu.nextTime)
		{
			MenuOptions.Menu.nextTime = flServerTime + MenuOptions.Misc.g_spamspeed;
			if (spamType == 0)
			{
				SSDK::I::Engine()->ClientCmd_Unrestricted("say You got rekt by P4R4H00K.XYZ ! ");
			}
			else if (spamType == 1)
			{
				if (!(std::find(std::begin(MenuOptions.Menu.playedJokeSentences), std::end(MenuOptions.Menu.playedJokeSentences), -1) != std::end(MenuOptions.Menu.playedJokeSentences)))
				{
					std::fill(std::begin(MenuOptions.Menu.playedJokeSentences), std::end(MenuOptions.Menu.playedJokeSentences), -1);
					jokeArrayCounter = 0;
				}

				//Get a random number
				do
				{
					std::default_random_engine rndEngine(random());
					std::uniform_int_distribution<int> uniform_dist(0, jokesSentences.size() - 1);
					number = uniform_dist(rndEngine);
				} while (std::find(std::begin(MenuOptions.Menu.playedJokeSentences), std::end(MenuOptions.Menu.playedJokeSentences), number) != std::end(MenuOptions.Menu.playedJokeSentences));

				SSDK::I::Engine()->ClientCmd_Unrestricted(("say " + jokesSentences[number]).c_str());

				MenuOptions.Menu.playedJokeSentences[jokeArrayCounter] = number;
				jokeArrayCounter++;
			}
			else if (spamType == 2)
			{
				if (!(std::find(std::begin(MenuOptions.Menu.playedFlameSentences), std::end(MenuOptions.Menu.playedFlameSentences), -1) != std::end(MenuOptions.Menu.playedFlameSentences)))
				{
					std::fill(std::begin(MenuOptions.Menu.playedFlameSentences), std::end(MenuOptions.Menu.playedFlameSentences), -1);
					flameArrayCounter = 0;
				}

				//Get a random number
				do
				{
					std::default_random_engine rndEngine(random());
					std::uniform_int_distribution<int> uniform_dist(0, flameSentences.size() - 1);
					number = uniform_dist(rndEngine);
				} while (std::find(std::begin(MenuOptions.Menu.playedFlameSentences), std::end(MenuOptions.Menu.playedFlameSentences), number) != std::end(MenuOptions.Menu.playedFlameSentences));

				SSDK::I::Engine()->ClientCmd_Unrestricted(("say " + flameSentences[number]).c_str());

				MenuOptions.Menu.playedFlameSentences[flameArrayCounter] = number;
				flameArrayCounter++;
			}
		}
		if ((MenuOptions.Menu.nextTime - flServerTime) > (MenuOptions.Misc.g_spamspeed + 1))
			MenuOptions.Menu.nextTime = 0;
	}
}

void Misc::BunnyHop(SSDK::CUserCmd* pCmd, C_CSPlayer* pLocalPlayer)
{
	if (pCmd->buttons & IN_JUMP && !(*pLocalPlayer->GetFlags() & (int)SSDK::EntityFlags::FL_ONGROUND))
	{
		pCmd->buttons &= ~IN_JUMP;

		static bool isActive = false;

		if (MenuOptions.Misc.g_circleStrafe && (GetAsyncKeyState(MenuOptions.Keys.g_circleStrafe) || isActive)) {
			if (!isActive)
				isActive = true;
			CircleStrafer(pCmd, isActive);
		}
		else {
			if (MenuOptions.Misc.g_autostrafe) {
				if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
					pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
				}
				else {
					pCmd->forwardmove = (10000.f) / (pLocalPlayer->GetVelocity().Length2D() + 1);
					pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
				}
			}
		}
	}
}

void Misc::AirStuck(SSDK::CUserCmd* pCmd)
{
	pCmd->tick_count = INT_MAX;
}

bool Misc::IsKnife(int nWeaponIndex)
{
	switch (nWeaponIndex)
	{
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
			return true;
			break;
		default:
			return false;
	}
}

bool Misc::IsGrenade(int classID)
{
	switch (classID)
	{
		case SSDK::EItemDefinitionIndex::weapon_hegrenade:
		case SSDK::EItemDefinitionIndex::weapon_incgrenade:
		case SSDK::EItemDefinitionIndex::weapon_smokegrenade:
		case SSDK::EItemDefinitionIndex::weapon_flashbang:
		case SSDK::EItemDefinitionIndex::weapon_decoy:
			return true;
			break;
		default:
			return false;
	}
}

void LoadNamedSky(const char* skybox) {
	auto LoadNamedSky = reinterpret_cast< void(__fastcall*)(const char*) >(SSDK::I::FindSignature(XorStr("engine.dll"), XorStr("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));
	LoadNamedSky(skybox);
}

void Misc::SetNightMode() {
	if (SSDK::I::Engine()->isInGame())
	{
		SSDK::I::CVar()->FindVar("r_DrawSpecificStaticProp")->SetValue(0);

		for (SSDK::MaterialHandle_t i = SSDK::I::MatSystem()->FirstMaterial(); i != SSDK::I::MatSystem()->InvalidMaterial(); i = SSDK::I::MatSystem()->NextMaterial(i))
		{
			SSDK::IMaterial *pMaterial = SSDK::I::MatSystem()->GetMaterial(i);

			if (!pMaterial)
				continue;

			pMaterial->Refresh();

			if (strstr(pMaterial->GetTextureGroupName(), TEXTURE_GROUP_WORLD) || /*strstr(pMaterial->GetTextureGroupName(), TEXTURE_GROUP_SKYBOX) ||*/ strstr(pMaterial->GetTextureGroupName(), TEXTURE_GROUP_PARTICLE) || strstr(pMaterial->GetTextureGroupName(), TEXTURE_GROUP_DECAL) || strstr(pMaterial->GetTextureGroupName(), "StaticProp textures")) {
				float r, g, b;
				pMaterial->GetColorModulation(&r, &g, &b);
				pMaterial->ColorModulate(r * .3f, g * .3f, b * .3f);
			}
		}
		LoadNamedSky("sky_csgo_night02b");
	}
}

bool Misc::FindBestSide(C_CSPlayer* pLocalPlayer, float flDistance, SSDK::CUserCmd* pCmd, SSDK::QAngle& qAngle)
{
	float closest_distance = flDistance;

	SSDK::QAngle startAngle = qAngle;

	startAngle.x = 0.f;

	SSDK::Vector eyePos = pLocalPlayer->GetEyePos();

	bool closestfound = false;

	SSDK::Vector forward;

	float sideArray[2] = { -45.f, 45.f };

	for (int i = 0; i < ARRAYSIZE(sideArray); ++i) {
		SSDK::QAngle rotateAngle = startAngle;

		SSDK::Ray_t ray;
		SSDK::Trace_t trace;
		SSDK::CTraceFilterWorldAndPropsOnly filter;

		rotateAngle.y += sideArray[i];

		Utils::AngleVectors(rotateAngle, &forward);

		SSDK::Vector vEnd = forward * (flDistance * cos(sideArray[i])) + pLocalPlayer->GetEyePos();

		ray.Init(eyePos, vEnd);

		SSDK::I::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &trace);

		float distance = eyePos.DistTo(trace.endpos);
		if (distance < closest_distance) {
			qAngle.y = startAngle.y + sideArray[i] * 2;
			closest_distance = distance;
			closestfound = true;
		}
	}

	return closestfound;
}

SSDK::IMaterial* Misc::GetChams(int chams)
{
	SSDK::IMaterial *mat = SSDK::I::MatSystem()->FindMaterial("chams", TEXTURE_GROUP_MODEL);

	switch (chams)
	{
		case 2:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
			break;
		case 3:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER);
			break;
		case 4:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
			break;
		case 5:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/music_kit/darude_01/mp3_detail", TEXTURE_GROUP_OTHER);
			break;
		case 6:
			mat = SSDK::I::MatSystem()->FindMaterial("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER);
			break;
		case 7:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
			break;
		case 8:
			mat = SSDK::I::MatSystem()->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_OTHER);
			break;
		case 9:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
			break;
		case 10:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER);
			break;
		case 11:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/dogtags/dogtags_lightray", TEXTURE_GROUP_OTHER);
			break;
		case 12:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_OTHER);
			break;
		case 13:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/dreamhack_trophies/dreamhack_star_blur", TEXTURE_GROUP_OTHER);
			break;
		case 14:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
			break;
		case 15:
			mat = SSDK::I::MatSystem()->FindMaterial("models/props_shacks/fishing_net01", TEXTURE_GROUP_OTHER);
			break;
		case 16:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/contributor_map_tokens/contributor_charset_color", TEXTURE_GROUP_OTHER);
			break;
		case 17:
			mat = SSDK::I::MatSystem()->FindMaterial("models/props_foliage/urban_tree03_branches", TEXTURE_GROUP_OTHER);
			break;
		case 18:
			mat = SSDK::I::MatSystem()->FindMaterial("models/extras/speech_info", TEXTURE_GROUP_OTHER);
			break;
		case 19:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER);
			break;
		case 20:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/vanguard_gold/vanguard_gold_detail", TEXTURE_GROUP_OTHER);
			break;
		case 21:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/trophy_majors/gold_dust", TEXTURE_GROUP_OTHER);
			break;
		case 22:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/phoenix_gold_01/phoenix_gold_01", TEXTURE_GROUP_OTHER);
			break;
		case 23:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/payback_gold_01/payback_gold_01", TEXTURE_GROUP_OTHER);
			break;
		case 24:
			mat = SSDK::I::MatSystem()->FindMaterial("models/inventory_items/sticker_inspect/sticker_backing", TEXTURE_GROUP_OTHER);
			break;


	}

	return mat;
}

Misc MiscExt;