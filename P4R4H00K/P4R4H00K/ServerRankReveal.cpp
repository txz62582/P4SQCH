#include "Includes.h"

ServerRankRevealAllFn ServerRankRevealAllEx;

void Hookers::RevealAllRanks()
{
	using tServerRankRevealAllFn = bool(__cdecl*)(int*);
	static tServerRankRevealAllFn ServerRankRevealAll = (tServerRankRevealAllFn)(SSDK::I::FindSignature(XorStr("client.dll"), XorStr("55 8B EC 8B 0D ? ? ? ? 68")));

	int fArray[3] = { 0,0,0 };

	ServerRankRevealAll(fArray);
}