#include "SDK.h"

namespace SSDK {

	uint64_t I::FindSignature(const char* szModule, const char* szSignature) {
		//CREDITS: learn_more
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;    //one ?
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	IVEngineClient *I::m_pEngine = nullptr;
	IClientEntityList *I::m_pEntityList = nullptr;
	ICvar *I::m_pCvar = nullptr;
	IBaseClientDLL *I::m_pClient = nullptr;
	IVModelRender *I::m_pModelRender = nullptr;
	IVModelInfo *I::m_pModelInfo = nullptr;
	IMaterialSystem *I::m_pMatSystem = nullptr;
	IPanel *I::m_pVGuiPanel = nullptr;
	ISurface *I::m_pVGuiSurface = nullptr;
	IVDebugOverlay *I::m_pDebugOverlay = nullptr;
	IClientMode *I::m_pClientMode = nullptr;
	IEngineTrace *I::m_pEngineTrace = nullptr;
	CGlowObjectManager *I::m_pGlowPointer = nullptr;
	IVRenderView *I::m_pRenderView = nullptr;
	IMDLCache *I::m_pMDLCache = nullptr;
	IPhysicsSurfaceProps *I::m_pPhysicsProps = nullptr;
	IGlobalVarsBase *I::m_pGlobalVars = nullptr;
	//************Appel de la classe ou il y a la méthode a hook
	IGameEventManager2 *I::m_gameEventManager2 = nullptr;
	IGameMovement *I::m_pGameMovement = nullptr;
	CPrediction *I::m_pPrediction = nullptr;
	IMoveHelper *I::m_pMoveHelper = nullptr;
	I::IInput *I::m_pInput = nullptr;

	IVEngineClient* I::Engine() {
		if (!m_pEngine) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pEngine = CaptureInterface<IVEngineClient>(pfnFactory, XorStr("VEngineClient014"));
		}
		return m_pEngine;
	}

	IClientEntityList* I::EntityList() {
		if (!m_pEntityList) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
			m_pEntityList = CaptureInterface<IClientEntityList>(pfnFactory, XorStr("VClientEntityList003"));
		}
		return m_pEntityList;
	}

	ICvar* I::CVar() {
		if (!m_pCvar) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vstdlib.dll")));
			m_pCvar = CaptureInterface<ICvar>(pfnFactory, XorStr("VEngineCvar007"));
		}
		return m_pCvar;
	}

	IBaseClientDLL* I::Client() {
		if (!m_pClient) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
			m_pClient = CaptureInterface<IBaseClientDLL>(pfnFactory, XorStr("VClient018"));
		}
		return m_pClient;
	}

	IVModelRender* I::ModelRender() {
		if (!m_pModelRender) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pModelRender = CaptureInterface<IVModelRender>(pfnFactory, XorStr("VEngineModel016"));
		}
		return m_pModelRender;
	}

	IVModelInfo* I::ModelInfo() {
		if (!m_pModelInfo) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pModelInfo = CaptureInterface<IVModelInfo>(pfnFactory, XorStr("VModelInfoClient004"));
		}
		return m_pModelInfo;
	}

	IMaterialSystem* I::MatSystem() {
		if (!m_pMatSystem) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("materialsystem.dll")));
			m_pMatSystem = CaptureInterface<IMaterialSystem>(pfnFactory, XorStr("VMaterialSystem080"));
		}
		return m_pMatSystem;
	}

	IPanel* I::VGUIPanel() {
		if (!m_pVGuiPanel) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vgui2.dll")));
			m_pVGuiPanel = CaptureInterface<IPanel>(pfnFactory, XorStr("VGUI_Panel009"));
		}
		return m_pVGuiPanel;
	}

	ISurface* I::MatSurface() {
		if (!m_pVGuiSurface) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vguimatsurface.dll")));
			m_pVGuiSurface = CaptureInterface<ISurface>(pfnFactory, XorStr("VGUI_Surface031"));
		}
		return m_pVGuiSurface;
	}

	IVDebugOverlay* I::DebugOverlay() {
		if (!m_pDebugOverlay) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pDebugOverlay = CaptureInterface<IVDebugOverlay>(pfnFactory, XorStr("VDebugOverlay004"));
		}
		return m_pDebugOverlay;
	}

	IClientMode* I::ClientMode() {
		if (!m_pClientMode) {
			m_pClientMode = **(IClientMode***)((*(DWORD**)I::Client())[10] + 0x5);
		}
		return m_pClientMode;
	}

	IEngineTrace* I::EngineTrace() {
		if (!m_pEngineTrace) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pEngineTrace = CaptureInterface<IEngineTrace>(pfnFactory, XorStr("EngineTraceClient004"));
		}
		return m_pEngineTrace;
	}

	CGlowObjectManager* I::GlowManager() {
		if (!m_pGlowPointer) {
			//m_pGlowPointer = (CGlowObjectManager*)(*(DWORD*)(FindSignature("client.dll", "0F 57 C0 C7 05 ? ? ? ? 00 00 00 00 0F 11 05 ? ? ? ? 68 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? 00 00 00 00 C7 05 ? ? ? ? 00 00 00 00") + 16));
			m_pGlowPointer = (CGlowObjectManager*)(*(DWORD*)(FindSignature("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 0x3));
		}
		return m_pGlowPointer;
	}

	IVRenderView *I::RenderView() {
		if (!m_pRenderView) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_pRenderView = CaptureInterface<IVRenderView>(pfnFactory, XorStr("VEngineRenderView014"));
		}
		return m_pRenderView;
	}

	IMDLCache* I::MDLCache() {
		if (!m_pMDLCache) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("datacache.dll")));
			m_pMDLCache = CaptureInterface<IMDLCache>(pfnFactory, XorStr("MDLCache004"));
		}
		return m_pMDLCache;
	}

	IPhysicsSurfaceProps* I::PhysicsProps() {
		if (!m_pPhysicsProps) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vphysics.dll")));
			m_pPhysicsProps = CaptureInterface<IPhysicsSurfaceProps>(pfnFactory, XorStr("VPhysicsSurfaceProps001"));
		}
		return m_pPhysicsProps;
	}

	IGlobalVarsBase* I::GlobalVars() {
		if (!m_pGlobalVars) {
			m_pGlobalVars = **(IGlobalVarsBase***)((*(DWORD**)I::Client())[0] + 0x1B);
		}
		return m_pGlobalVars;
	}

	//*********CREATION DU HOOK DE LA CLASSE POUR AVOIR LES METHODE A HOOK A LINTERIEUR
	IGameEventManager2* I::GameEventManager2()
	{
		if (!m_gameEventManager2) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
			m_gameEventManager2 = CaptureInterface<IGameEventManager2>(pfnFactory, XorStr("GAMEEVENTSMANAGER002"));
		};
		return m_gameEventManager2;
	}

	IGameMovement* I::GameMovement() {
		if (!m_pGameMovement) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
			m_pGameMovement = CaptureInterface<IGameMovement>(pfnFactory, XorStr("GameMovement001"));
		}
		return m_pGameMovement;
	}

	CPrediction* I::Prediction() {
		if (!m_pPrediction) {
			CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
			m_pPrediction = CaptureInterface<CPrediction>(pfnFactory, XorStr("VClientPrediction001"));
		}
		return m_pPrediction;
	}

	IMoveHelper* I::MoveHelper() {
		if (!m_pMoveHelper) {
			m_pMoveHelper = **(IMoveHelper***)(FindSignature("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2);
		}
		return m_pMoveHelper;
	}

	I::IInput* I::Input() {
		if (!m_pInput) {
			m_pInput = *(IInput**)((*(DWORD**)Client())[15] + 0x1);
		}
		return m_pInput;
	}
}