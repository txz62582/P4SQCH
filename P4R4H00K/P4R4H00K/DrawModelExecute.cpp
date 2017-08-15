#include "Includes.h"
#include "Misc.h"

DrawModelExecute_t g_fnOriginalDrawModelExecute = nullptr;

void __fastcall Hookers::Hooked_DrawModelExecute(void* thisptr, void* edx, SSDK::IMatRenderContext* ctx, const SSDK::DrawModelState_t& state, const SSDK::ModelRenderInfo_t& pInfo, SSDK::matrix3x4_t* pCustomBoneToWorld) {
	if (pInfo.pModel) {
		if (SSDK::I::Engine()->isInGame() && SSDK::I::Engine()->isConnected())
		{
			C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();
			const char* cModelName = (char*)SSDK::I::ModelInfo()->GetModelName(pInfo.pModel);
			C_CSPlayer *pModelEntity = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(pInfo.entity_index);

			if (cModelName)
			{
				//CHAMS OR CHAMS WH
				if (MenuOptions.ESP.g_chamsOrGlow == 0 || MenuOptions.ESP.g_chamsOrGlow == 1) {
					if (strstr(cModelName, XorStr("models/player"))) {
						static float col[3];

						if (MenuOptions.ESP.g_deadbodiesChams)
						{
							col[0] = 1.f;
							col[1] = 1.f;
							col[2] = 1.f;
						}

						SSDK::studiohdr_t* hdr = SSDK::I::ModelInfo()->GetStudiomodel(pInfo.pModel);

						if (pModelEntity)
						{

							SSDK::IMaterial* PlayerMat;

							if (!MenuOptions.ESP.g_ChamsGlowOnlyEnemyTeam || (MenuOptions.ESP.g_ChamsGlowOnlyEnemyTeam && (pModelEntity->GetTeamNum() != pLocalPlayer->GetTeamNum())))
							{
								if (MenuOptions.ESP.g_chamsOrGlow == 1) //CHAMS WH
								{
									if (pModelEntity->IsAlive()) {
										if (pModelEntity->GetTeamNum() == 2)
										{
											col[0] = 1.f;
											col[1] = 1.f;
											col[2] = 0.f;
										}
										else
										{
											col[0] = 0.f;
											col[1] = 1.f;
											col[2] = 0.f;
										}
									}
									else if (MenuOptions.ESP.g_deadbodiesChams)
									{
										col[0] = 1.f;
										col[1] = 1.f;
										col[2] = 1.f;
									}

									PlayerMat = SSDK::I::MatSystem()->FindMaterial(XorStr("chams_z"), TEXTURE_GROUP_MODEL);

									SSDK::I::RenderView()->SetColorModulation(col);
									SSDK::I::ModelRender()->ForcedMaterialOverride(PlayerMat);

									//Return orig
									g_fnOriginalDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
								}

								//CHAMS NO WH IN ANY CASE
								if (MenuOptions.ESP.g_playerChams != 0)
									PlayerMat = MiscExt.GetChams(MenuOptions.ESP.g_playerChams + 1);
								else
									PlayerMat = SSDK::I::MatSystem()->FindMaterial(XorStr("chams"), TEXTURE_GROUP_MODEL);

								if (pModelEntity->IsAlive()) {
									if (pModelEntity->GetTeamNum() == 2)
									{
										col[0] = 1.f;
										col[1] = 0.f;
										col[2] = 0.f;
									}
									else
									{
										col[0] = 0.f;
										col[1] = 0.f;
										col[2] = 1.f;
									}
								}
								else if (MenuOptions.ESP.g_deadbodiesChams)
								{
									col[0] = 1.f;
									col[1] = 1.f;
									col[2] = 1.f;
								}

								SSDK::I::RenderView()->SetColorModulation(col);
								SSDK::I::ModelRender()->ForcedMaterialOverride(PlayerMat);
							}

						}
					}
				}

				//No hands
				if (MenuOptions.Misc.g_nohands) {
					if (strstr(cModelName, XorStr("arms"))) {
						return;
					}
				}

				//Weapons chams
				if (MenuOptions.ESP.g_weaponsChams != 0)
				{
					if (strstr(cModelName, XorStr("weapons/v")))
					{
						//Avoid arms
						if (!strstr(cModelName, "arms")) {
							SSDK::IMaterial *material = MiscExt.GetChams(MenuOptions.ESP.g_weaponsChams);
							SSDK::I::ModelRender()->ForcedMaterialOverride(material);
						}
					}
				}

				//Wireframe
				if (MenuOptions.ESP.g_armsChams != 0 || MenuOptions.Misc.g_wireframeArms)
				{
					if (strstr(cModelName, XorStr("arms")))
					{
						SSDK::IMaterial *chams;
						SSDK::IMaterial *wireframe;


						if (MenuOptions.ESP.g_armsChams != 0)
						{
							chams = MiscExt.GetChams(MenuOptions.ESP.g_armsChams);
							//SSDK::I::RenderView()->SetColorModulation(col);
							SSDK::I::ModelRender()->ForcedMaterialOverride(chams);
							if (MenuOptions.Misc.g_wireframeArms)
								g_fnOriginalDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}

						if (MenuOptions.Misc.g_wireframeArms)
						{
							wireframe = SSDK::I::MatSystem()->FindMaterial(cModelName, TEXTURE_GROUP_MODEL);
							wireframe->SetMaterialVarFlag(SSDK::MATERIAL_VAR_WIREFRAME, true);
							SSDK::I::ModelRender()->ForcedMaterialOverride(wireframe);
						}
						else
						{
							wireframe = SSDK::I::MatSystem()->FindMaterial(cModelName, TEXTURE_GROUP_MODEL);
							wireframe->SetMaterialVarFlag(SSDK::MATERIAL_VAR_WIREFRAME, false);
						}
					}
				}
			}
		}
	}
	g_fnOriginalDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	SSDK::I::ModelRender()->ForcedMaterialOverride(NULL);
}
