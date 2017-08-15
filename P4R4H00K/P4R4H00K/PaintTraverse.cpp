#include "Includes.h"
#include "DrawManager.h"
#include "EntityManager.h"
#include "CLagCompensation2.h"

PaintTraverse_t    g_fnOriginalPaintTraverse = nullptr;
int iScreenWidth, iScreenHeight;


char* GetGrenadeName(int id, C_CSPlayer* pEntity)
{
	const char* modelName;

	switch (id) {
	case SSDK::EClassIds::CBaseCSGrenadeProjectile:
		modelName = SSDK::I::ModelInfo()->GetModelName(pEntity->GetModel());

		if (modelName[21] == 'r') {
			return "HE";
		}
		else {
			return "FLASH";
		}
		break;

	case SSDK::EClassIds::CSmokeGrenadeProjectile:
		return "SMOKE";
		break;

	case SSDK::EClassIds::CMolotovProjectile:
		return "MOLOTOV";
		break;

	case SSDK::EClassIds::CDecoyProjectile:
		return "DECOY";
		break;

	case SSDK::EClassIds::CSensorGrenadeProjectile:
		return "SENSOR";
		break;

	default:
		return pEntity->GetClientClass()->m_pNetworkName;
		break;
	}
}

void DrawDamageIndicator(C_CSPlayer* pLocalPlayer)
{
	float CurrentTime = *pLocalPlayer->GetTickBase() * SSDK::I::GlobalVars()->interval_per_tick;

	for (int i = 0; i < MenuOptions.Menu.DamageIndicator.size(); i++)
	{
		if (MenuOptions.Menu.DamageIndicator[i].flEraseTime < CurrentTime)
		{
			MenuOptions.Menu.DamageIndicator.erase(MenuOptions.Menu.DamageIndicator.begin() + i);
			continue;
		}

		if (!MenuOptions.Menu.DamageIndicator[i].bInitialized)
		{
			MenuOptions.Menu.DamageIndicator[i].Position = Utils::GetEntityBone(MenuOptions.Menu.DamageIndicator[i].Player, SSDK::ECSPlayerBones::head_0);
			MenuOptions.Menu.DamageIndicator[i].bInitialized = true;
		}

		if (CurrentTime - MenuOptions.Menu.DamageIndicator[i].flLastUpdate > 0.00001f)
		{
			MenuOptions.Menu.DamageIndicator[i].Position.z -= (1.f * (CurrentTime - MenuOptions.Menu.DamageIndicator[i].flEraseTime));
			MenuOptions.Menu.DamageIndicator[i].flLastUpdate = CurrentTime;
		}

		SSDK::Vector ScreenPosition;

		if (Utils::W2S(MenuOptions.Menu.DamageIndicator[i].Position, ScreenPosition)) {
			DrawManager::DrawStringValue(SSDK::Color(0, 102, 204, 255), ScreenPosition.x, ScreenPosition.y, "-%i", MenuOptions.Menu.DamageIndicator[i].iDamage);
		}
	}
}

void __fastcall Hookers::Hooked_PaintTraverse(void* thisptr, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();

	//POSTPROCESS AND NO SCOPE
	static auto postprocess = SSDK::I::CVar()->FindVar(XorStr("mat_postprocess_enable"));

	if (MenuOptions.Misc.g_noscope)
	{
		if (Utils::isValidPlayer(pLocalPlayer, true)) {
			if (pLocalPlayer->isScoped())
			{
				if (!strcmp(XorStr("HudZoom"), SSDK::I::VGUIPanel()->GetName(vguiPanel)))
				{
					postprocess->SetValue(0);
					return;
				}
			}
		}
	}

	g_fnOriginalPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);

	if (MenuOptions.Misc.g_postprocess)
	{
		postprocess->SetValue(1);
	}
	else
	{
		postprocess->SetValue(0);
	}

	static unsigned int vSelectedVGui = 0;
	if (!vSelectedVGui)
	{
		if (strstr(SSDK::I::VGUIPanel()->GetName(vguiPanel), XorStr("MatSystemTopPanel")))
		{
			vSelectedVGui = vguiPanel;
		}
	}
	else if (vguiPanel == vSelectedVGui)
	{
		// On alloue 128 char en mémoire et ensuite on formate comme je t'ai appris, on gagne au moins 5x en vitesse
		char buf[128];
		if (MenuOptions.Misc.g_watermark)
		{
			sprintf(buf, "P4R4H00K 7.0 - %s's build", USERNAME);
		}
		else
		{
			sprintf(buf, " %s", " ");
		}
		DrawManager::DrawString(SSDK::Color(0, 110, 160), buf, 10, 20);

		if (SSDK::I::Engine()->isInGame())
		{
			C_BaseCombatWeapon *localPlayerWeapon = C_CSPlayer::GetLocalPlayer()->GetActiveWeapon();
			if (localPlayerWeapon)
			{
				if (MenuOptions.Misc.g_noscope)
				{
					if (localPlayerWeapon->WeaponHasZoom())
					{
						if (Utils::isValidPlayer(pLocalPlayer, true))
						{
							if (pLocalPlayer->isScoped())
							{
								DrawManager::DrawZoom();
							}
						}
					}
				}
				//Direction
				if (MenuOptions.Misc.g_lbyIndicator && MenuOptions.Aimbot.g_realYawAA == 6) {
					char *dir = "->";
					if (MenuOptions.Menu.unhittable_side)
						dir = "<-";

					float lby = *pLocalPlayer->GetLowerBodyYaw();
					Utils::NormalizeFloat(lby);
					float hit = MenuOptions.Menu.currenthittableyaw;
					Utils::NormalizeFloat(hit);

					bool bFaking = !pLocalPlayer->isMoving() && fabs(hit - lby) > 90.f;

					DrawManager::DrawLbyIndicator(bFaking ? SSDK::Color(0, 200, 0) : SSDK::Color(255, 255, 255), dir, iScreenWidth * .5f, 150, true);
				}
			}

			SSDK::I::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

			//Hitmarker
			if (MenuOptions.Misc.g_hitmarkerCrosshair)
			{
				if (MenuOptions.Menu._flHurtTime + 0.25f >= SSDK::I::GlobalVars()->curtime) {
					DrawManager::DrawHitmarker(iScreenWidth, iScreenHeight);
				}
			}

			//ESP
			for (int i = 1; i < SSDK::I::EntityList()->GetHighestEntityIndex(); ++i) {
				C_CSPlayer *pEntity = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);

				//Si entité pas valide
				if (!pEntity)
					continue;

				if (MenuOptions.ESP.g_damageIndicator)
				{
					DrawDamageIndicator(pLocalPlayer);
				}

				SSDK::Vector weapOrigin = *pEntity->GetOrigin();
				SSDK::Vector outOrigin;

				if (MenuOptions.ESP.g_espON)
				{
					if (Utils::isValidPlayer(pEntity))
					{
						//Draw ESP
						if (!MenuOptions.ESP.g_ESPonlyEnemyTeam || (MenuOptions.ESP.g_ESPonlyEnemyTeam && (pEntity->GetTeamNum() != pLocalPlayer->GetTeamNum())))
						{
							SSDK::Vector outHead;
							SSDK::Vector headPos = Utils::GetEntityBone(pEntity, SSDK::ECSPlayerBones::head_0);
							headPos.z += 10.0f;

							if (Utils::W2S((*pEntity->GetOrigin()), outOrigin) && Utils::W2S(headPos, outHead)) {

								float height = outOrigin.y - outHead.y;
								float width = height * 0.70f;
								//Box ESP
								if (MenuOptions.ESP.g_espBox) {

									//T
									if (pEntity->GetTeamNum() == 2)
									{
										if (pEntity->isVisible()) {
											DrawManager::DrawESPRect(EnemyVisibleEspColorT, outHead.x - width * .5f, outHead.y, outHead.x + width / 2, outHead.y + height);
										}
										else {
											DrawManager::DrawESPRect(EnemyEspColorT, outHead.x - width / 2, outHead.y, outHead.x + width / 2, outHead.y + height);
										}
									}
									else
									{
										//CT
										if (pEntity->isVisible()) {
											DrawManager::DrawESPRect(EnemyVisibleEspColor, outHead.x - width / 2, outHead.y, outHead.x + width / 2, outHead.y + height);
										}
										else {
											DrawManager::DrawESPRect(EnemyEspColor, outHead.x - width / 2, outHead.y, outHead.x + width / 2, outHead.y + height);
										}
									}
								}

								if (MenuOptions.ESP.g_espWeapon)
								{
									DrawManager::DrawWeapon(pEntity, outHead.x, outHead.y + height);
								}

								//Health ESP
								if (MenuOptions.ESP.g_espHealth) {
									DrawManager::DrawHealthBar(pEntity->GetHealth(), outHead.x - (width / 2) * 1.28f, outHead.y, outHead.x - (width / 2) * 1.07f, outHead.y + height);
								}
								//Armor ESP
								if (MenuOptions.ESP.g_espArmor)
								{
									DrawManager::DrawArmorBar(pEntity->GetHealth(), outHead.x + (width / 2) * 1.07f, outHead.y, outHead.x + (width / 2) * 1.28f, outHead.y + height);
								}
								//Name ESP
								if (MenuOptions.ESP.g_espName) {
									DrawManager::DrawPlayerName(pEntity->EntIndex(), outHead.x, outHead.y - 25.f);
								}
								//Bone ESP
								if (MenuOptions.ESP.g_espBone) {
									DrawManager::DrawBoneESP(WHITE_COLOR, pEntity);
								}
								//Dist line ESP
								if (MenuOptions.ESP.g_espDist) {
									DrawManager::DrawDistLine(EnemyEspColor, pEntity, iScreenWidth / 2, iScreenHeight);
								}
								//Draw head cross ESP
								if (MenuOptions.ESP.g_espHead) {
									DrawManager::DrawHeadCross(pEntity);
								}
								if (MenuOptions.ESP.g_lbyUpdate && pEntity->GetTeamNum() != pLocalPlayer->GetTeamNum())
								{
									SSDK::Color lbyColor;

									if (LagCompensation2.isLbyUpdate(pEntity))
									{
										lbyColor = SSDK::Color(0, 204, 0);
									}
									else
									{
										lbyColor = SSDK::Color(204, 0, 0);
									}
									//outHead.x + (width / 2) * 1.07f, outHead.y, outHead.x + (width / 2) * 1.28f, outHead.y + height);
									DrawManager::DrawString(lbyColor, "LBY", outHead.x + (width / 2) * 1.48f, outHead.y);
								}
								if (MenuOptions.ESP.g_ammoEsp)
								{
									DrawManager::DrawAmmo(pEntity, outHead.x + (width / 2) * 1.48f, outHead.y);
								}
							}
						}
					}
					else if (Utils::isValidWeapon(pEntity) || strstr(pEntity->GetClientClass()->m_pNetworkName, XorStr("CFlashbang"))) {
						if (MenuOptions.ESP.g_espDroppedWeapons)
						{
							if (!weapOrigin.IsZero()) {
								if (Utils::W2S(weapOrigin, outOrigin)) {
									C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)pEntity;

									//Draw weapons names
									DrawManager::DrawString(MainColor, pWeapon->GetNameWeap(), outOrigin.x, outOrigin.y);
								}
							}
						}
					}
					if (MenuOptions.ESP.g_GrenadesEsp)
					{
						if (strstr(pEntity->GetClientClass()->m_pNetworkName, XorStr("Projectile")))
						{
							if (!weapOrigin.IsZero()) {
								if (Utils::W2S(weapOrigin, outOrigin)) {
									/*	if (strstr(pEntity->GetClientClass()->m_pNetworkName, XorStr("CFlashbang")))
									{
									DrawManager::DrawString(WHITE_COLOR, "FLASH", outOrigin.x, outOrigin.y);
									}
									else
									{*/
									DrawManager::DrawString(WHITE_COLOR, GetGrenadeName(pEntity->GetClientClass()->m_ClassID, pEntity), outOrigin.x, outOrigin.y);
									//}
								}
							}
						}
					}
					if (MenuOptions.ESP.g_bombEsp)
					{

						if (pEntity->GetClientClass()->m_ClassID == SSDK::EClassIds::CPlantedC4)
						{
							//T'use des calc outdated
							//Ok, j'avais vu sur le topic, ca fonctionne mais ya 1 voire 2 hp de marge d'erreur avec mon pasta de la
							//1ere page.
							/*
							CBaseEntity *pLocalPlayer = CBaseEntity::GetLocalPlayer();

							if (!pLocalPlayer)
							return 0.f;

							int Damage = 500; //TODO: Get this dynamicly if possible
							float BombRadius = Damage * 3.5f;

							auto vecToTarget = pEntity->GetEyePos() - pLocalPlayer->GetEyePos();
							float BombScaledDamage = Damage * exp(-((vecToTarget.Length() * vecToTarget.Length()) / (((BombRadius * 0.33333334) * 2.f) * (BombRadius * 0.33333334))));

							ScaleDamage(HITGROUP_GENERIC, pLocalPlayer, 1.f, BombScaledDamage);

							return roundf(BombScaledDamage);
							*/

							float flDistance = (*pLocalPlayer->GetOrigin()).DistTo(*pEntity->GetOrigin());
							float a = 450.7f;
							float b = 75.68f;
							float c = 789.2f;
							float d = ((flDistance - b) / c);
							float flDamage = a*exp(-d * d);
							float damage = (std::max)(((int)ceilf(Utils::CSGO_Armor(flDamage, pLocalPlayer->GetArmor()))), 0);

							if (!weapOrigin.IsZero()) {
								SSDK::Vector outBomb;
								SSDK::Vector BombUpper = SSDK::Vector(weapOrigin.x, weapOrigin.y, weapOrigin.z + 10.f);

								if (Utils::W2S(*pEntity->GetOrigin(), outOrigin) && Utils::W2S(BombUpper, outBomb)) {
									float height = outOrigin.y - outBomb.y;
									float width = height * 0.70f;

									DrawManager::DrawStringValue(WHITE_COLOR, outOrigin.x, outOrigin.y, "%.1f", pEntity->GetBombTimer());
									//DrawManager::DrawStringValue(WHITE_COLOR, outBomb.x, outBomb.y, "%.1f", damage);
								}
							}
						}
						if (pEntity->GetClientClass()->m_ClassID == SSDK::EClassIds::CC4)
						{
							if (!weapOrigin.IsZero()) {
								if (Utils::W2S(weapOrigin, outOrigin)) {
									DrawManager::DrawString(WHITE_COLOR, "BOMB", outOrigin.x, outOrigin.y);
								}
							}
						}
					}
				}
			}

			//Draw crosshair
			if (MenuOptions.Misc.g_crosshair) {
				DrawManager::DrawCross(MainColor, iScreenWidth / 2, iScreenHeight / 2, 40);
			}
		}
	}
}