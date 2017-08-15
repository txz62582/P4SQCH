#include "Includes.h"
#include <vector>
#include "VTableFuncHooks.h"
#include "NetvarManager.h"
#include "DrawManager.h"
#include "Event.h"
#include <future>

#pragma region HOOKTABLES
std::unique_ptr<HookTable::VFTableHook> g_pClientHook = nullptr;
std::unique_ptr<HookTable::VFTableHook> g_pModelRenderHook = nullptr;
std::unique_ptr<HookTable::VFTableHook> g_pMatSurfaceHook = nullptr;
std::unique_ptr<HookTable::VFTableHook> g_pVGuiHook = nullptr;
std::unique_ptr<HookTable::VFTableHook> g_pClientModeHook = nullptr;
std::unique_ptr<HookTable::VFTableHook>       g_pD3DDevice9Hook = nullptr;
std::unique_ptr<HookTable::VFTableHook> g_pFireEventHook = nullptr;
HookTable::VFTableHook *hkEngineTrace = nullptr;
#pragma endregion HOOKTABLES

#pragma region MAIN

#pragma region SONAR
//Thread of the sonar
void BeepThread(HINSTANCE hinstDLL) {
	while (true) {
		if (MenuOptions.Menu.activeSonar) {
			Beep(MenuOptions.Misc.g_sonarHZ, MenuOptions.Menu.speedSonar);
		}
		Sleep(1);
	}
}

//Save config handler
void SaveLastConfig(HINSTANCE hinstDLL) {
	while (true) {
		if (MenuOptions.Menu.SaveLastConfig) {
			MenuOptions.Menu.SaveLastConfig = false;

			int tmpconf = MenuOptions.Menu.currentConfigID;

			//Save configs
			MenuOptions.Menu.currentConfigID = 1;
			Config->Save(0);
			Config->Save(1);
			MenuOptions.Menu.currentConfigID = tmpconf;
		}
		Sleep(1);
	}
}

#pragma endregion SONAR

#pragma region UNHOOK
/*void UnhookThread(HINSTANCE hinstDLL)
{
while (true)
{
if (GetAsyncKeyState(VK_HOME) & 1) { //Press HOME to detach DLL
FreeLibraryAndExitThread(hinstDLL, 0);
}
}
}*/
#pragma endregion UNHOOK

void FillChatspam() {
	//Init and set default values
	std::fill(std::begin(MenuOptions.Menu.playedJokeSentences), std::end(MenuOptions.Menu.playedJokeSentences), -1);
	std::fill(std::begin(MenuOptions.Menu.playedFlameSentences), std::end(MenuOptions.Menu.playedFlameSentences), -1);
}

void LoadConfig() {
	//Load configs
	Config->Setup();
	Config->SetupSkins();
	Config->Load(1);

	MenuOptions.Menu.currentConfigID = 1;
	Config->Load(0);
}

void Initialize() {
	//Init vars
	NetvarManager::Instance()->CreateDatabase();
	//NetvarManager::Instance()->Dump("dumplol.txt");//Dump netvars

	//Init drawmanager
	DrawManager::InitializeFont("Vintage Avalanche Black", 17);


	//Create chams vmt files
	Utils::initVMTFiles();

	std::async(FillChatspam);
	std::async (LoadConfig);
}

void InitializeHooks()
{
	//D3DX9
	//Finds the D3D9 Device pointer
	auto dwDevice = **(uint32_t**)(SSDK::I::FindSignature(XorStr("shaderapidx9.dll"), XorStr("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1);

	//Create the virtual table hooks
	g_pD3DDevice9Hook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)dwDevice, true);

	g_fnOriginalReset = g_pD3DDevice9Hook->Hook(16, Hookers::Hooked_Reset);                            //Hooks IDirect3DDevice9::EndScene
	g_fnOriginalEndScene = g_pD3DDevice9Hook->Hook(42, Hookers::Hooked_EndScene);                      //Hooks IDirect3DDevice9::Reset

																							  //Find CSGO main window
	while (!(g_hWindow = FindWindowA(XorStr("Valve001"), NULL))) Sleep(200);

	//Replace the WindowProc with our own to capture user input
	if (g_hWindow)
		g_pOldWindowProc = (WNDPROC)SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, (LONG_PTR)Hookers::Hooked_WndProc);

	//*************************

	g_pClientHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::Client(), true);
	g_pModelRenderHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::ModelRender(), true);
	g_pMatSurfaceHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::MatSurface(), true);
	g_pVGuiHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::VGUIPanel(), true);
	g_pClientModeHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::ClientMode(), true);
	g_pFireEventHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::GameEventManager2(), true);
	//g_pMDLCacheHook = std::make_unique<HookTable::VFTableHook>((HookTable::PPDWORD)SSDK::I::MDLCache(), true);
	hkEngineTrace = new HookTable::VFTableHook((HookTable::PPDWORD)SSDK::I::EngineTrace(), true);

	//Table de hook avec la variable/fonction faite dans sdk.h / sdk.cpp
	g_fnOriginalOnPostScreenEffects = g_pClientModeHook->Hook(44,(do_post_screen_space_effects_t)Hookers::do_post_screen_space_effects);
	g_fnOriginalFrameStageNotify = g_pClientHook->Hook(36, (FrameStageNotify_t)Hookers::Hooked_FrameStageNotify);
	g_fnOriginalOnScreenSizeChanged = g_pMatSurfaceHook->Hook(116, (OnScreenSizeChanged_t)Hookers::Hooked_OnScreenSizeChanged);
	g_fnOriginalDrawModelExecute = g_pModelRenderHook->Hook(21, (DrawModelExecute_t)Hookers::Hooked_DrawModelExecute);
	g_fnOriginalPaintTraverse = g_pVGuiHook->Hook(41, (PaintTraverse_t)Hookers::Hooked_PaintTraverse);
	g_fnOriginalCreateMove = g_pClientModeHook->Hook(24, (CreateMove_t)Hookers::Hooked_CreateMove);
	g_fnOriginalFireEvent = g_pFireEventHook->Hook(9, (FireEvent)Hookers::Hooked_FireEvent);
	g_fnOriginalOverrideView = g_pClientModeHook->Hook(18, (OverrideViewFn)Hookers::Hooked_OverrideView);
	//g_fnOriginalFindMDL = g_pMDLCacheHook->Hook(10, (FindMDL_t)Hooked_FindMDL); //Ability to change model skins
	pOriginalTraceRay = hkEngineTrace->Hook(5, (TraceRay)Hookers::Hooked_TraceRay);
	
	//ProxyHook
	SSDK::ClientClass *pClass = SSDK::I::Client()->GetAllClasses();
	while (pClass) {
		const char *pszName = pClass->m_pRecvTable->GetName();
		if (!strcmp(pszName, XorStr("DT_SmokeGrenadeProjectile"))) {
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; ++i) {
				SSDK::RecvProp *pProp = pClass->m_pRecvTable->GetTableProp(i);
				const char *name = pProp->GetName();
				if (!strcmp(name, XorStr("m_bDidSmokeEffect"))) {
					pProp->SetProxyFn(Hookers::DidSmokeEffect);
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}

void MainThread(HMODULE hInst)
{
	SSDK::I::Engine()->ExecuteClientCmd(XorStr("clear")); //clear console
	SSDK::I::CVar()->ConsolePrintf(XorStr("    ____  ____  __  ____ __    _____ ____ \n"
										  "   / __ \\/ __ \\/ / / / //_/   /__  // __ \\ \n"
										  "  / /_/ / /_/ / /_/ / ,<        / // / / / \n"
										  " / ____/ _, _/ __  / /| |      / // /_/ /  \n"
										  "/_/   /_/ |_/_/ /_/_/ |_|     /_(_)____/  \n"
										  " TOGGLE MENU - INSERT \n"));

	MenuOptions.Menu.hInst = hInst;
	Initialize();
	InitializeHooks();

	bullet::singleton()->initialize(hInst);
	bullet_hurt::singleton()->initialize(hInst);
}

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle,
	IN DWORD     nReason,
	IN LPVOID    Reserved)
{
	switch (nReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hDllHandle, 0, 0);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UnhookThread, hDllHandle, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)BeepThread, hDllHandle, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SaveLastConfig, hDllHandle, 0, 0);
		break;

	case DLL_PROCESS_DETACH:
		SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, (LONG_PTR)g_pOldWindowProc);
		g_pClientHook->RestoreTable();
		g_pModelRenderHook->RestoreTable();
		g_pMatSurfaceHook->RestoreTable();
		g_pVGuiHook->RestoreTable();
		g_pClientModeHook->RestoreTable();
		g_pD3DDevice9Hook->RestoreTable();
		g_pFireEventHook->RestoreTable();
		hkEngineTrace->RestoreTable();
		//g_pMDLCacheHook->RestoreTable();
		break;
	}
	return TRUE;
}
#pragma endregion MAIN

#pragma region BANANA
/*
//typedef SSDK::MDLHandle_t(__thiscall *FindMDL_t)(void*, const char*);
//1: Nom de la méthode à hook
//2: Paramètre nécéssaire à la méthode

//SSDK::MDLHandle_t __fastcall Hooked_FindMDL(void* thisptr, void* edx, const char* pMDLRelativePath);

//Paramètres à passer à la méthode

//std::unique_ptr<HookTable::VFTableHook> g_pMDLCacheHook = nullptr;
//FindMDL_t		   g_fnOriginalFindMDL = nullptr;




// Sqch -> Banana_joe													3 arg, const char ou char*
//SSDK::MDLHandle_t __fastcall Hooked_FindMDL(void* thisptr, void* edx, const char* pMDLRelativePath) {
//if (strstr(pMDLRelativePath, XorStr("models/player")))
//	return g_fnOriginalFindMDL(thisptr, XorStr("models/player/custom_player/kuristaja/banana_joe/banana_joe.mdl"));

//if (strstr(pMDLRelativePath, XorStr("arms")))
//	return g_fnOriginalFindMDL(thisptr, XorStr("models/player/custom_player/kuristaja/banana_joe/banana_joe_arms.mdl"));

//return g_fnOriginalFindMDL(thisptr, pMDLRelativePath);

//if (strstr(pMDLRelativePath, "v_glove"))
//	sprintf(pMDLRelativePath, "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl");
//else if (strstr(pMDLRelativePath, "w_glove"))
//	sprintf(pMDLRelativePath, "models/weapons/w_models/arms/w_glove_slick.mdl");
//}
*/
#pragma endregion BANANA