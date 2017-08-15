#include "Includes.h"

do_post_screen_space_effects_t g_fnOriginalOnPostScreenEffects = nullptr;


bool _fastcall Hookers::do_post_screen_space_effects(void* ecx, void* edx, SSDK::CViewSetup* pSetup)
{
	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();


	//GlowESP
	if (MenuOptions.ESP.g_chamsOrGlow == 2 || MenuOptions.ESP.g_glowDroppedWeapons)
	{
		for (int i = 0; i < SSDK::I::GlowManager()->size; ++i)
		{
			SSDK::CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &SSDK::I::GlowManager()->m_GlowObjectDefinitions[i];


			C_CSPlayer* Entity = (C_CSPlayer*)glowEntity->getEntity();

			if (glowEntity->IsEmpty() || !Entity)
				continue;

			if (MenuOptions.ESP.g_glowGrenades)
			{
				if (strstr(Entity->GetClientClass()->m_pNetworkName, XorStr("Projectile")) || strstr(Entity->GetClientClass()->m_pNetworkName, XorStr("CFlashbang")))
				{
					SSDK::Color colGrenade(WHITE_COLOR);
					glowEntity->set(colGrenade);
				}
			}

			if (MenuOptions.ESP.g_glowDroppedWeapons)
			{
				if (Utils::isValidWeapon(Entity)) {
					SSDK::Color colWeapon(WHITE_COLOR);
					glowEntity->set(colWeapon);
				}
			}

			if (MenuOptions.ESP.g_chamsOrGlow == 2)
			{
				if (Utils::isValidPlayer(Entity))
				{
					//LE GROS IF PERMET DE CHECKER SI IL VEUT QUE LES ENNEMIS OU AUSSI SON EQUIPE
					if (!MenuOptions.ESP.g_ChamsGlowOnlyEnemyTeam || (MenuOptions.ESP.g_ChamsGlowOnlyEnemyTeam && (Entity->GetTeamNum() != pLocalPlayer->GetTeamNum())))
					{
						//T
						if (Entity->GetTeamNum() == 2)
						{
							if (Entity->isVisible()) {
								glowEntity->set(SSDK::Color(254, 221, 0));
							}
							else {
								glowEntity->set(SSDK::Color(214, 0, 28));
							}
						}
						else
						{
							//CT
							if (Entity->isVisible()) {
								glowEntity->set(SSDK::Color(0, 153, 89));
							}
							else {
								glowEntity->set(SSDK::Color(0, 102, 153));
							}
						}
					}
				}
			}
		}
	}

	return g_fnOriginalOnPostScreenEffects(ecx,pSetup);

}
