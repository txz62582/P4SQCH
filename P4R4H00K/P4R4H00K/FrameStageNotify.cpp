#include "Includes.h"
#include "Misc.h"
#include "CLagCompensation2.h"


FrameStageNotify_t g_fnOriginalFrameStageNotify = nullptr;

void __fastcall Hookers::Hooked_FrameStageNotify(void* thisptr, void* edx, SSDK::ClientFrameStage_t curStage)
{
	//NOVISRECOIL
	SSDK::Vector vecAimPunch;
	SSDK::Vector vecViewPunch;

	SSDK::Vector* pAimPunch = nullptr;
	SSDK::Vector* pViewPunch = nullptr;

	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();


	if (SSDK::I::Engine()->isInGame())
	{
		if (pLocalPlayer)
		{
			if (curStage == SSDK::ClientFrameStage_t::FRAME_RENDER_START)
			{
				if (MenuOptions.Misc.g_nosmoke)
				{
					static DWORD iSmokeCount = SSDK::I::FindSignature(XorStr("client.dll"), XorStr("A3 ?? ?? ?? ?? 57 8B CB")) + 0x1;
					if (*reinterpret_cast<int*>(*reinterpret_cast<uint32_t*>(iSmokeCount)) > 0)
						*reinterpret_cast<int*>(*reinterpret_cast<uint32_t*>(iSmokeCount)) = 0;
				}

				if (*(bool*)((DWORD)SSDK::I::Input() + 0xA5))
					*(SSDK::QAngle*)((DWORD)pLocalPlayer + 0x031C8) = SSDK::QAngle(pLocalPlayer->GetEyeAngles()->x, *pLocalPlayer->GetLowerBodyYaw(), 0);//OptionsManager::qaLocalEyesAngle;

				for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
					C_CSPlayer *pClient = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);
					if (Utils::isValidPlayer(pClient)) {
						//PVSFIX V2
						*pClient->GetLastOcclusion() = SSDK::I::GlobalVars()->framecount; //we'll skip occlusion checks now // FTG AUSSI ? TU SKIP RIEN WSH
						*pClient->GetOcclusionFlag() = 0;//clear occlusion flags
					}
				}

				if (MenuOptions.Aimbot.legitOrRage == 1 && MenuOptions.Aimbot.g_novisrecoil)
				{
					if (Utils::isValidPlayer(pLocalPlayer, true))
					{
						pAimPunch = pLocalPlayer->AimPunch();
						pViewPunch = pLocalPlayer->ViewPunch();

						vecAimPunch = *pAimPunch;
						vecViewPunch = *pViewPunch;

						*pAimPunch = SSDK::Vector(0, 0, 0);
						*pViewPunch = SSDK::Vector(0, 0, 0);
					}
				}
			}

			if (curStage == SSDK::ClientFrameStage_t::FRAME_NET_UPDATE_END)
			{
				C_BaseCombatWeapon *activeWeapon = pLocalPlayer->GetActiveWeapon();

				if (Utils::isValidPlayer(pLocalPlayer, true)) {
					if (MenuOptions.Misc.g_noflash)
					{
						//NOFLASH -> opti la netvar est mise a jour ici
						if (*pLocalPlayer->flashDuration() > 0.f)
							*pLocalPlayer->flashDuration() = 0.f;
					}

					static float flOldLby = 0.f;
					float flCurLby = *pLocalPlayer->GetLowerBodyYaw();

					if (flCurLby != flOldLby) {
						flOldLby = flCurLby;
						MenuOptions.Menu.bLocalLowerbodyUpdate = true;
					}

					UINT* hWeapons = pLocalPlayer->GetWeapons();

					if (hWeapons)
					{
						for (int i = 0; hWeapons[i]; ++i) {

							CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)SSDK::I::EntityList()->GetClientEntityFromHandle(hWeapons[i]);

							if (!pWeapon)
								continue;

							int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();

							static int KnifeDefinitionArray[] = { 42,500,505,506,507,508,509,512,514,515,516 };

							if ((nWeaponIndex == SSDK::EItemDefinitionIndex::weapon_knife) ||
								(nWeaponIndex == SSDK::EItemDefinitionIndex::weapon_knife_t) ||
								((nWeaponIndex == KnifeDefinitionArray[MenuOptions.Skins.iSelectedKnife]) && (MenuOptions.Skins.iSelectedKnife > 0))) {

								switch (MenuOptions.Skins.iSelectedKnife) {
								case 1:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_bayonet.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_bayonet.mdl"));
									break;
								case 2:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_flip.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_flip.mdl"));
									break;
								case 3:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_gut.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_gut.mdl"));
									break;
								case 4:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_karam.mdl"), pWeapon);
									//*pViewModelWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_karam.mdl"));
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_karam.mdl"));
									break;
								case 5:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_m9_bay.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_m9_bay.mdl"));
									break;
								case 6:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_tactical.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_tactical.mdl"));
									break;
								case 7:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_falchion_advanced.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_falchion_advanced.mdl"));
									break;
								case 8:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_survival_bowie.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_survival_bowie.mdl"));
									break;
								case 9:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_butterfly.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_butterfly.mdl"));
									break;
								case 10:
									//pViewModel->SetWeaponModel(XorStr("models/weapons/v_knife_push.mdl"), pWeapon);
									*pWeapon->GetModelIndex() = SSDK::I::ModelInfo()->GetModelIndex(XorStr("models/weapons/v_knife_push.mdl"));
									break;
								}
							}

							Utils::ApplyCustomSkin(pLocalPlayer, hWeapons[i]);
						}
					}
				}

				if (activeWeapon)
				{
					if (MenuOptions.Aimbot.legitOrRage == 1)
					{
						//RESOLVER
						for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i)
						{
							C_CSPlayer *pClient = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);
							//CHECK IF PLAYER IS VALID
							if (Utils::isValidPlayer(pClient))
							{
								if (pClient->GetTeamNum() != pLocalPlayer->GetTeamNum())
								{
									//opti, les entrées sont ajoutées dans ce stage ci
									pClient->DisableInterp();

									float backup = *pClient->GetLowerBodyYaw();

									LagCompensation2.StoreDatas(pClient);
									LagCompensation2.RestorePlayer(pClient);

									//CHECK IF RESOLVER IS ENABLED AND RAGEMODE IS ENABLED
									if ((MenuOptions.Aimbot.g_resolver != 1 || (MenuOptions.Menu.TargetList[i].customResolve && MenuOptions.Menu.TargetList[i].resolver != 1)))
									{

										//SAVE THE RESOLVER TO USE IN A VAR AND THE ANGLE TO FORCE
										int resolver = MenuOptions.Aimbot.g_resolver;

										//CHECK IF THE ENTITY AS CUSTOM SETTINGS SET
										//CHECK IF THE ENTITY IN TARGETLIST, HAS A RESOLVER OTHER THAN "NONE"
										if (MenuOptions.Menu.TargetList[i].customResolve)
										{
											//SET THE VALUES TO THE VAR
											resolver = MenuOptions.Menu.TargetList[i].resolver;
										}


										//Simple Resolver
										if (resolver == 0)
										{
											pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw();
										}


										static bool shouldBrute[64] = { false };
										static int stateHit[64] = { 0 };

										if (!pClient->isMoving(150.f)) {
											
											if (MenuOptions.Menu.LocalPlayerShotdata[pClient->EntIndex()].hit && !(MenuOptions.Menu.LocalPlayerShotdata[pClient->EntIndex()].hurt)) {
												if (!shouldBrute[pClient->EntIndex()])
													shouldBrute[pClient->EntIndex()] = true;
												stateHit[pClient->EntIndex()]++;
											}

											if (shouldBrute[pClient->EntIndex()]) {
												//SSDK::I::CVar()->ConsolePrintf("FSN Brute %i\n", pClient->EntIndex());

												switch (stateHit[pClient->EntIndex()]) {
												case 1:
													pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw() + 180.f;
													break;
												case 2:
													pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw() + 160.f;
													break;
												case 3:
													pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw() - 100.f;
													break;
												case 4:
													pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw() - 160.f;
													break;
												case 5:
													pClient->GetEyeAngles()->y = *pClient->GetLowerBodyYaw() + 100.f;
													shouldBrute[pClient->EntIndex()] = false;
													stateHit[pClient->EntIndex()] = 0;
													break;
												}


												MenuOptions.Menu.LocalPlayerShotdata[pClient->EntIndex()].hit = false;
												MenuOptions.Menu.LocalPlayerShotdata[pClient->EntIndex()].hurt = false;
											}
										}
										else {
											//SSDK::I::CVar()->ConsolePrintf("!!! FSN NOT Brute !!! %i\n", pClient->EntIndex());

											shouldBrute[pClient->EntIndex()] = false;
											stateHit[pClient->EntIndex()] = 0;
										}
									}
									*pClient->GetLowerBodyYaw() = backup;
								}
							}
						}
					}
				}
			}
		}
	}

	//Call Origin func
	g_fnOriginalFrameStageNotify(thisptr, curStage);

	// Restore the original aim and view punches.
	if (MenuOptions.Aimbot.legitOrRage == 1 && MenuOptions.Aimbot.g_novisrecoil && pAimPunch && pViewPunch)
	{
		*pAimPunch = vecAimPunch;
		*pViewPunch = vecViewPunch;
	}
}
