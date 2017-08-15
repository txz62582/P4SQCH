#include "Includes.h"
#include "CPredictionSystem.h"
#include "CAutoWall.h"
#include "Antiaim\Antiaim.h"
#include "CFixMove.h"
#include "DrawManager.h"
#include "EntityManager.h"
#include "SqchMenu\Menu.h"
#include "P4DrawManager.h"
#include "P4Menu\P4Menu.h"
#include "P4Menu\Config.h"
#include "P4Menu\Options.h"
#include "Misc.h"
#include "CLagCompensation2.h"
#include "Antiaim\Antiaim.h"

CreateMove_t       g_fnOriginalCreateMove = nullptr;

int counter = 0;
static float timer;
bool __fastcall Hookers::Hooked_CreateMove(void* thisptr, void* edx, float sample_input_frametime, SSDK::CUserCmd* pCmd)
{
	bool ret = g_fnOriginalCreateMove(SSDK::I::ClientMode(), sample_input_frametime, pCmd);
	if (pCmd->command_number == 0)
		return ret;

	if (SSDK::I::Engine()->isConnected())
	{
		static SSDK::Stack *_bp; __asm mov _bp, ebp;
		static bool *bSendPacket = ((bool *)_bp->next - 0x1c);

		C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();
		C_BaseCombatWeapon *activeWeapon = pLocalPlayer->GetActiveWeapon();
		CBaseAttributableItem *pAttribWeapon;

		static CFixMove *FixMoveManager = new CFixMove();
		static CPredictionSystem *Pred = new CPredictionSystem();

		if(activeWeapon)
		{
			pAttribWeapon = (CBaseAttributableItem*)activeWeapon;
		}


		if (MenuOptions.Misc.g_namespam)
		{
			float flServerTime = *pLocalPlayer->GetTickBase() * SSDK::I::GlobalVars()->interval_per_tick;
			static bool change = true;

			if (flServerTime >= MenuOptions.Menu.nextTime)
			{
				MenuOptions.Menu.nextTime = flServerTime + 0.1f;

				if (counter < 100)
				{
					if (change)
						Hookers::NET_SetConVar(MenuOptions.Menu.nameToSpam, "name");
					else
						Hookers::NET_SetConVar(MenuOptions.Menu.nameToSpam, "name");

					change = !change;
					counter++;
				}
				else
				{
					MenuOptions.Misc.g_namespam = false;
					MenuOptions.Menu.nameToSpam = "P4R4H00K";
					counter = 0;
				}
			}
			if ((MenuOptions.Menu.nextTime - flServerTime) > (0.1f + 1))
				MenuOptions.Menu.nextTime = 0.1f;
		}

		//BHOP Routine
		if (MenuOptions.Misc.g_bhopON) {
			MiscExt.BunnyHop(pCmd, pLocalPlayer);
		}

		//Spoof sv_cheats -> si tu spoof en mid game, tu vas te taper un joli untrusted
		if ((MenuOptions.Misc.g_noscope || MenuOptions.Misc.g_thirdperson))
		{
			MiscExt.SpoofConvars();
		}

		//Thirdperson
		MiscExt.ForceThirdPerson(pCmd, pLocalPlayer);

		//RCS Routine
		if ((MenuOptions.Aimbot.legitOrRage == 0 || MenuOptions.Aimbot.legitOrRage == 2) && MenuOptions.Aimbot.g_rcs)
		{
			//Check if weapon is good to enable rcs
			if (activeWeapon->WeaponIsRCSOK())
			{
				static SSDK::Vector OldAimPunch;

				if (pCmd->buttons & IN_ATTACK) {
					SSDK::Vector aimPunch = *pLocalPlayer->AimPunch() * 2.f;
					SSDK::QAngle qaNewAngle = pCmd->viewangles + (OldAimPunch - aimPunch);
					if (Utils::Clamp(qaNewAngle)) {
						pCmd->viewangles = qaNewAngle;
					}
					OldAimPunch = aimPunch;
				}
				else {
					OldAimPunch.Zero();
				}
			}
		}


		//Silent Start
		if (MenuOptions.Aimbot.legitOrRage == 1 && MenuOptions.Aimbot.g_silentaim)
		{
			FixMoveManager->Start(pCmd);
			AntiAim::PostCreateMoveAA(pLocalPlayer, activeWeapon, pCmd, *bSendPacket, pAttribWeapon);
		}

		float flCurrentTime = *pLocalPlayer->GetTickBase() * SSDK::I::GlobalVars()->interval_per_tick;

		if (activeWeapon)
		{
			if (*pAttribWeapon->GetItemDefinitionIndex() == SSDK::EItemDefinitionIndex::weapon_revolver)
			{
				if (MenuOptions.Aimbot.g_autoRevolver)
				{
					//SPAM LE BTN
					pCmd->buttons |= IN_ATTACK;

					float flPostponeFireReady = activeWeapon->GetFireReadyTime();
					if (flPostponeFireReady > 0 && flPostponeFireReady < flCurrentTime)
					{
						pCmd->buttons &= ~IN_ATTACK;
					}
				}
			}
		}

		//AIMBOT LEGIT
		if (MenuOptions.Aimbot.legitOrRage == 0)
		{
			if (MenuOptions.Misc.g_autozeus || pCmd->buttons & IN_ATTACK || (GetAsyncKeyState(MenuOptions.Keys.g_mouseAutoshoot) && MenuOptions.Aimbot.g_mouseAuto))
			{
				for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i)
				{
					C_CSPlayer *pClient = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);
					if (Utils::isValidPlayer(pClient)) {
						if (pClient->isVisible()) {
							if (pLocalPlayer->GetTeamNum() != pClient->GetTeamNum())
							{
								MenuOptions.SetDefaultLegitValues();
								if (activeWeapon)
								{
									MenuOptions.CheckWeapons(activeWeapon);
								}
								MenuOptions.CheckLegit(i);

								SSDK::Vector vecLocalHead = Utils::GetEntityHitbox(C_CSPlayer::GetLocalPlayer(), MenuOptions.Overwritten.hitbox);
								SSDK::Vector vecClientHead = Utils::extrapolateTick(Utils::GetEntityHitbox(pClient, MenuOptions.Overwritten.hitbox), pClient);

								SSDK::QAngle qaCurAngle = pCmd->viewangles;
								SSDK::QAngle qaNewViewAngle;

								qaNewViewAngle = Utils::CalcAngle(pLocalPlayer->GetEyePos(), vecClientHead);

								SSDK::Vector aimPunch = *pLocalPlayer->AimPunch() * 2.f;
								qaNewViewAngle -= aimPunch;

								auto Difference = Utils::AngleDifference(qaCurAngle, qaNewViewAngle, vecLocalHead.DistTo(vecClientHead));

								if (Difference < MenuOptions.Overwritten.fov)
								{
									qaCurAngle += Utils::SmoothAngle(qaCurAngle, qaNewViewAngle, MenuOptions.Overwritten.smooth);

									if (activeWeapon && activeWeapon->HasBullets() && activeWeapon->GetWeaponTypewitham())
									{
										if (Utils::Clamp(qaCurAngle))
										{
											if (activeWeapon->NextPrimaryAttack() <= flCurrentTime)
											{
												bool shoot = true;

												//AUTO ZEUS DES BARRES MAMEN
												if (MenuOptions.Misc.g_autozeus)
												{
													if (*((CBaseAttributableItem*)activeWeapon)->GetItemDefinitionIndex() == SSDK::EItemDefinitionIndex::weapon_taser) {
														SSDK::WeaponInfo_t *wInfo = activeWeapon->GetCSWpnData();

														if (wInfo) {
															if (pLocalPlayer->GetEyePos().DistTo(vecClientHead) > wInfo->flRange)
																shoot = false;
														}
													}
												}
												if (shoot || !MenuOptions.Misc.g_autozeus)
												{
													pCmd->viewangles = qaCurAngle;
													if (MenuOptions.Misc.g_autozeus || (MenuOptions.Aimbot.g_mouseAuto && GetAsyncKeyState(MenuOptions.Keys.g_mouseAutoshoot)))
													{

														pCmd->tick_count = TIME_TO_TICKS(*pClient->GetSimulationTime() + Utils::GetLerpTime());
														pCmd->buttons |= IN_ATTACK;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}


		Pred->StartPrediction(pLocalPlayer, pCmd);

#pragma region RAGEAIMBOT
		//AIMBOT With autowall
		if (MenuOptions.Aimbot.legitOrRage == 1)
		{
			if ((MenuOptions.Aimbot.g_autoshootON || pCmd->buttons & IN_ATTACK))
			{
				if (activeWeapon && !(MiscExt.IsKnife(*pAttribWeapon->GetItemDefinitionIndex())))
				{
					SSDK::Vector vecClientTarget;
					C_CSPlayer *pClient = CAutowall::FindLowestHPTarget(vecClientTarget);

					if (pClient)
					{
						MenuOptions.Menu.currentTarget = pClient;

						if (MenuOptions.Misc.g_autocrouch)
							pCmd->buttons |= IN_DUCK;
						if (MenuOptions.Misc.g_autostop)
						{
							pCmd->forwardmove = 0;
							pCmd->sidemove = 0;
						}
						SSDK::QAngle qaNewViewAngle = Utils::CalcAngle(pLocalPlayer->GetEyePos(), vecClientTarget) - *pLocalPlayer->AimPunch() * 2.f;

						if (activeWeapon->GetWeaponTypewitham() && activeWeapon->HasBullets())
						{
							if ((Utils::Clamp(qaNewViewAngle)))
							{
								if ((activeWeapon->NextPrimaryAttack() <= flCurrentTime))
								{
									if (!(pCmd->buttons & IN_ATTACK) && (!pLocalPlayer->isScoped() && activeWeapon->WeaponHasScope()) && MenuOptions.Misc.g_autoscope)
									{
										pCmd->buttons |= IN_ATTACK2;
									}
									else if (Utils::HitChance(pLocalPlayer, pClient, activeWeapon, qaNewViewAngle, MenuOptions.Overwritten.g_hitChance))
									{
									
										if (MenuOptions.Misc.g_autozeus || MenuOptions.Aimbot.g_autoshootON || pCmd->buttons & IN_ATTACK)
										{
											bool shoot = true;
											//AUTO ZEUS DES BARRES MAMEN
											if (MenuOptions.Misc.g_autozeus)
											{
												if (*((CBaseAttributableItem*)activeWeapon)->GetItemDefinitionIndex() == SSDK::EItemDefinitionIndex::weapon_taser) {
													SSDK::WeaponInfo_t *wInfo = activeWeapon->GetCSWpnData();

													if (wInfo) {
														if (pLocalPlayer->GetEyePos().DistTo(vecClientTarget) > wInfo->flRange)
															shoot = false;
													}
												}
											}

											if (shoot || !MenuOptions.Misc.g_autozeus)
											{
												pCmd->viewangles = qaNewViewAngle;

												//Calculation secret
												//On met le tick du bon record ouai
												LagCompensation2.SetValidTickCount(pClient, pCmd);
												pCmd->buttons |= IN_ATTACK;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		#pragma endregion RAGEAIMBOT
	
			//SHOTS FIRED
			if (activeWeapon)
			{
				if (!(pCmd->buttons & IN_ATTACK) && activeWeapon->NextPrimaryAttack() <= flCurrentTime) {
					MenuOptions.Menu.shotsFired = 0;
				}
				else {
					MenuOptions.Menu.shotsFired += pLocalPlayer->ShotsFired();
					MenuOptions.Menu.totalShotFired += pLocalPlayer->ShotsFired();
				}
			}


			//TRIGGERBOT
			if ((MenuOptions.Aimbot.legitOrRage == 0 || MenuOptions.Aimbot.legitOrRage == 2) && MenuOptions.Aimbot.g_triggerbotON) {
				if (GetAsyncKeyState(MenuOptions.Keys.g_triggerbot)) {
					SSDK::QAngle viewAngle = pCmd->viewangles;

					SSDK::Vector vEnd;
					Utils::AngleVectors(viewAngle, &vEnd);

					SSDK::Trace_t tTrace;
					SSDK::Ray_t tRay;
					SSDK::CTraceFilter tFilter;

					tFilter.pSkip = pLocalPlayer;

					vEnd = vEnd * 8192.0f + pLocalPlayer->GetEyePos();

					tRay.Init(pLocalPlayer->GetEyePos(), vEnd);

					SSDK::I::EngineTrace()->TraceRay(tRay, CONTENTS_SOLID | CONTENTS_GRATE | CONTENTS_HITBOX, &tFilter, &tTrace);

					C_CSPlayer *target = (C_CSPlayer *)tTrace.m_pEnt;

					if (Utils::isValidPlayer(target)) {
						if (target->GetTeamNum() != pLocalPlayer->GetTeamNum()) {
							if (activeWeapon && activeWeapon->GetWeaponTypewitham() && activeWeapon->HasBullets())
							{
								if (activeWeapon->NextPrimaryAttack() <= flCurrentTime)
									pCmd->tick_count = TIME_TO_TICKS(*target->GetSimulationTime() + Utils::GetLerpTime());
								pCmd->buttons |= IN_ATTACK;
							}
						}
					}
				}
			}
			if (MenuOptions.Aimbot.legitOrRage == 1 && MenuOptions.Aimbot.g_silentaim)
			{
				FixMoveManager->Stop(pCmd);
			}

			//OTHER SHIT

			//RADAR
			if (MenuOptions.Misc.g_radar) {
				for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i)
				{
					C_CSPlayer *pClient = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);
					if (Utils::isValidPlayer(pClient))
					{
						*pClient->GetSpotted() = true;
					}
				}
			}

			//REVEAL ALL RANK
			if (pCmd->buttons & IN_SCORE)
			{
				Hookers::RevealAllRanks();
			}


			//SPAM CHAT
			if (MenuOptions.Misc.g_spamchat)
			{
				MiscExt.Chatspam(MenuOptions.Misc.g_spamtype);
			}


			//Sonarr routine
			if (MenuOptions.Misc.g_sonarON)
			{
				float fCurDist = Utils::GetClosestEnnemyDistance(pLocalPlayer);

				if (fCurDist < MenuOptions.Misc.g_sonarDist) {
					MenuOptions.Menu.speedSonar = fCurDist;
					MenuOptions.Menu.activeSonar = true;
				}
				else {
					MenuOptions.Menu.activeSonar = false;
				}
			}
			else
			{
				MenuOptions.Menu.activeSonar = false;
			}


			//AIRSTUCK
			if (MenuOptions.Misc.g_airstuckON) {
				if (GetAsyncKeyState(MenuOptions.Keys.g_airstuck))
				{
					MiscExt.AirStuck(pCmd);
				}
			}

			//ClanTag -> Ca je touche pas, je vais tout break, faudra que tu me donnes ta logique d'ab ou alors je change de methode
			if (MenuOptions.Misc.g_animeClanTag && SSDK::I::Engine()->isConnected())
			{
				float flCurTime = SSDK::I::GlobalVars()->curtime;
				static float flNextTimeUpdate = flCurTime + MenuOptions.Misc.g_clantagspeed;

				if (flCurTime >= flNextTimeUpdate) {
					static int tagState = 0;
					//static const char tag[] = "\\|/P\\|/4\\|/R\\|/4\\|/H\\|/0\\|/0\\|/K";
					static const char tag[] = "_-_-P_-_-4_-_-R_-_-4_-_-|_-_-H_-_-0_-_-0_-_-K";
					static std::string tagWritten = "";
					std::string textToShow = "";

					for (tagState; tagState <= ARRAYSIZE(tag);)
					{
						if (tagState == ARRAYSIZE(tag))
						{
							tagWritten = "";
							tagState = 0;
							Hookers::SetClanTag("\n", "");
							break;
						}

						//if (tag[tagState] != '\\' && tag[tagState] != '|' && tag[tagState] != '/')
						if (tag[tagState] != '_' && tag[tagState] != '-')
						{
							tagWritten += tag[tagState];
							Hookers::SetClanTag(tagWritten.c_str(), "");
						}
						else if (tag[tagState] == '_')
						{
							textToShow = tagWritten;
							textToShow += tag[tagState];

							Hookers::SetClanTag(textToShow.c_str(), "");
						}
						else if (tag[tagState] == '-')
						{
							textToShow = tagWritten;
							textToShow += ' ';

							Hookers::SetClanTag(textToShow.c_str(), "");
						}

						tagState++;
						break;
					}
				}
				if ((flNextTimeUpdate < flCurTime) || ((flNextTimeUpdate - flCurTime) >(MenuOptions.Misc.g_clantagspeed + 1)))
				{
					flNextTimeUpdate = flCurTime + MenuOptions.Misc.g_clantagspeed;
				}
			}

			//SilentAim
			if (MenuOptions.Aimbot.legitOrRage == 1 && MenuOptions.Aimbot.g_silentaim)
				return false;
	}
	return true;
}
