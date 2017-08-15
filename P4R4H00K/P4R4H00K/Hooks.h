#pragma once
#include <d3d9.h>
#include <d3dx9.h>

using SetClanTagFn = void(__fastcall*) (const char *tag, const char *name);
extern SetClanTagFn SetClanTagEx;

using ServerRankRevealAllFn = bool(__cdecl*)(float*);
extern ServerRankRevealAllFn ServerRankRevealAllEx;

using FireEvent = bool(__thiscall*)(void*, SSDK::IGameEvent*);
extern FireEvent g_fnOriginalFireEvent;

using OverrideViewFn = void(__thiscall*)(void* pThis, SSDK::CViewSetup* pSetup);
extern OverrideViewFn g_fnOriginalOverrideView;

using FrameStageNotify_t = void(__thiscall*)(void*, SSDK::ClientFrameStage_t);
extern FrameStageNotify_t g_fnOriginalFrameStageNotify;

using DrawModelExecute_t = void(__thiscall*)(void*, SSDK::IMatRenderContext*, const SSDK::DrawModelState_t&, const SSDK::ModelRenderInfo_t&, SSDK::matrix3x4_t*);
extern DrawModelExecute_t g_fnOriginalDrawModelExecute;

using PaintTraverse_t = void* (__thiscall*)(void*, unsigned int, bool, bool);
extern PaintTraverse_t g_fnOriginalPaintTraverse;

using CreateMove_t =  bool(__thiscall*)(SSDK::IClientMode*, float, SSDK::CUserCmd*);
extern CreateMove_t	g_fnOriginalCreateMove;

using TraceRay = void(__thiscall* )(void*, const SSDK::Ray_t &ray, unsigned int fMask, SSDK::ITraceFilter *pTraceFilter, SSDK::CGameTrace *pTrace);
extern TraceRay pOriginalTraceRay;

using EndScene_t = long(__stdcall* )(IDirect3DDevice9* device);
extern EndScene_t g_fnOriginalEndScene;

using Reset_t = long(__stdcall* )(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
extern Reset_t g_fnOriginalReset;

using OnScreenSizeChanged_t = void* (__thiscall*)(SSDK::ISurface*, int, int);
extern OnScreenSizeChanged_t  g_fnOriginalOnScreenSizeChanged;

using do_post_screen_space_effects_t = bool(__thiscall*)(void*, SSDK::CViewSetup*);
extern do_post_screen_space_effects_t g_fnOriginalOnPostScreenEffects;


extern WNDPROC	g_pOldWindowProc;
extern HWND	g_hWindow;



namespace Hookers {
	extern bool __fastcall Hooked_FireEvent(void* thisptr, void* edx, SSDK::IGameEvent* event);
	extern void __fastcall Hooked_OverrideView(void* pThis, DWORD _EDX, SSDK::CViewSetup* pSetup);
	extern void DidSmokeEffect(const SSDK::CRecvProxyData *pData, void *pStruct, void *pOut);
	extern void __fastcall Hooked_FrameStageNotify(void* thisptr, void* edx, SSDK::ClientFrameStage_t curStage);
	extern void __fastcall Hooked_DrawModelExecute(void* thisptr, void* edx, SSDK::IMatRenderContext* ctx, const SSDK::DrawModelState_t& state, const SSDK::ModelRenderInfo_t& pInfo, SSDK::matrix3x4_t* pCustomBoneToWorld);
	extern void __fastcall Hooked_PaintTraverse(void* thisptr, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	extern bool __fastcall Hooked_CreateMove(void* thisptr, void* edx, float sample_input_frametime, SSDK::CUserCmd* pCmd);
	extern void __stdcall Hooked_OnScreenSizeChanged(int oldWidth, int oldHeight);
	extern void SetClanTag(const char* tag, const char* name);
	extern void RevealAllRanks();
	extern void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar);
	extern void __fastcall Hooked_TraceRay(void* thisptr, void* edx, const SSDK::Ray_t &ray, unsigned int fMask, SSDK::ITraceFilter *pTraceFilter, SSDK::CGameTrace *pTrace);
	extern HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
	extern HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern bool _fastcall do_post_screen_space_effects(void* ecx, void* edx, SSDK::CViewSetup* pSetup);
}