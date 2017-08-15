#include "Includes.h"
#include "DrawManager.h"

OnScreenSizeChanged_t g_fnOriginalOnScreenSizeChanged = nullptr;


void __stdcall Hookers::Hooked_OnScreenSizeChanged(int oldWidth, int oldHeight)
{
	g_fnOriginalOnScreenSizeChanged(SSDK::I::MatSurface(), oldWidth, oldHeight);
	DrawManager::InitializeFont("Vintage Avalanche Black", 17);
}