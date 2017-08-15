#include "Includes.h"

OverrideViewFn g_fnOriginalOverrideView = nullptr;

void __fastcall Hookers::Hooked_OverrideView(void* pThis, DWORD _EDX, SSDK::CViewSetup* pSetup)
{
	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();
	if (Utils::isValidPlayer(pLocalPlayer, true)) {
		//Comment if u want to apply fov modif on scoped
		//if (!pLocalPlayer->isScoped())
		pSetup->fov += (MenuOptions.Misc.g_fovPlayer - 90.f);
	}
	g_fnOriginalOverrideView(pThis, pSetup);
}