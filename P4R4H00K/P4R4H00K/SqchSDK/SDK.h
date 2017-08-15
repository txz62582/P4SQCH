#pragma once
#include <Windows.h>
#include <Psapi.h>
#include "../xorstr.h"
//SDK
#include "AllDef.h"
#include "CInput.h"
#include "INetChannelInfo.h"
#include "IVEngineClient.h"
#include "IVModelRender.h"
#include "IVDebugOverlay.h"
#include "IVModelInfo.h"
#include "IVRenderView.h"
#include "ICvar.h"
#include "IClientEntityList.h"
#include "IBaseClientDLL.h"
#include "IClientMode.h"
#include "IMaterial.h"
#include "IMaterialSystem.h"
#include "IPanel.h"
#include "ISurface.h"
#include "ICollideable.h"
#include "IEngineTrace.h"
#include "IMDLCache.h"
#include "IPhysicsSurfaceProps.h"
#include "IGlobalVarsBase.h"
#include "PlayerInfo.h"
#include "Color.h"
#include "Recv.h"
#include "Vector.h"
#include "Studio.h"
#include "GlowOutlineEffect.h"
#include "CWeaponInfo.h"
#include "IGameEvents.h"
#include "CPrediction.h"
#include "IGameMovement.h"
#include "IMoveHelper.h"
#include "CViewSetup.h"
#include "CUtlVector.h"

namespace SSDK {
	class I {
	private:
		typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
		typedef void* (*InstantiateInterfaceFn)();

		static CreateInterfaceFn GetFactory(HMODULE hMod) {
			return (CreateInterfaceFn)GetProcAddress(hMod, XorStr("CreateInterface"));
		}

		template<typename T>
		static T* CaptureInterface(CreateInterfaceFn f, const char* szInterfaceVersion) {
			return (T*)f(szInterfaceVersion, NULL);
		}

		static bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
		{
			for (; *szMask; ++szMask, ++Mask, ++Data)
			{
				if (*szMask == 'x' && *Mask != *Data)
				{
					return false;
				}
			}
			return (*szMask) == 0;
		}

		class IInput;

		static IVEngineClient *m_pEngine;
		static IClientEntityList *m_pEntityList;
		static ICvar *m_pCvar;
		static IBaseClientDLL *m_pClient;
		static IVModelRender *m_pModelRender;
		static IVModelInfo *m_pModelInfo;
		static IMaterialSystem *m_pMatSystem;
		static IPanel *m_pVGuiPanel;
		static ISurface *m_pVGuiSurface;
		static IVDebugOverlay *m_pDebugOverlay;
		static IClientMode *m_pClientMode;
		static IEngineTrace *m_pEngineTrace;
		static CGlowObjectManager *I::m_pGlowPointer;
		static IVRenderView *I::m_pRenderView;
		static IMDLCache *I::m_pMDLCache;
		static IPhysicsSurfaceProps *I::m_pPhysicsProps;
		static IGlobalVarsBase *I::m_pGlobalVars;
		static IGameEventManager2 *I::m_gameEventManager2;
		static IGameMovement *I::m_pGameMovement;
		static CPrediction *I::m_pPrediction;
		static IMoveHelper *I::m_pMoveHelper;
		static IInput *I::m_pInput;
	public:
		static DWORD FindPattern(const char *moduleName, BYTE* Mask, char* szMask)
		{
			DWORD dwAddress = (DWORD)GetModuleHandleA(moduleName);

			MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)dwAddress, &miModInfo, sizeof(MODULEINFO));
			DWORD dwLength = miModInfo.SizeOfImage;
			for (DWORD i = 0; i < dwLength; i++)
			{
				if (bCompare((BYTE*)(dwAddress + i), Mask, szMask))
				{
					return (DWORD)(dwAddress + i);
				}
			}
			return 0;
		}
		static uint64_t FindSignature(const char* szModule, const char* szSignature);
		static IVEngineClient* I::Engine();
		static IClientEntityList* I::EntityList();
		static ICvar* I::CVar();
		static IBaseClientDLL* I::Client();
		static IVModelRender* I::ModelRender();
		static IVModelInfo* I::ModelInfo();
		static IMaterialSystem* I::MatSystem();
		static IPanel* I::VGUIPanel();
		static ISurface* I::MatSurface();
		static IVDebugOverlay* I::DebugOverlay();
		static IClientMode* I::ClientMode();
		static IEngineTrace* I::EngineTrace();
		static CGlowObjectManager *I::GlowManager();
		static IVRenderView *I::RenderView();
		static IMDLCache *I::MDLCache();
		static IPhysicsSurfaceProps *I::PhysicsProps();
		static IGlobalVarsBase *I::GlobalVars();
		static IGameEventManager2 *I::GameEventManager2();
		static IGameMovement *I::GameMovement();
		static CPrediction *I::Prediction();
		static IMoveHelper *I::MoveHelper();
		static IInput *I::Input();
	};
}