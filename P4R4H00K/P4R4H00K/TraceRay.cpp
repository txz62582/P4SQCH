#include "Includes.h"

TraceRay pOriginalTraceRay;

void __fastcall Hookers::Hooked_TraceRay(void* thisptr, void* edx, const SSDK::Ray_t &ray, unsigned int fMask, SSDK::ITraceFilter *pTraceFilter, SSDK::CGameTrace *pTrace) {
	pOriginalTraceRay(thisptr, ray, fMask, pTraceFilter, pTrace);
	if (MenuOptions.Menu.isAwalling)
	{
		pTrace->surface.flags |= SURF_SKY;
	}
}