#include "Includes.h"


void Hookers::DidSmokeEffect(const SSDK::CRecvProxyData *pData, void *pStruct, void *pOut)
{
	if (MenuOptions.Misc.g_nosmoke)
	{
		*(bool*)((DWORD)pOut + 0x1) = true;
	}
}