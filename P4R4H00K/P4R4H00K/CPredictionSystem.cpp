#include "CPredictionSystem.h"

void CPredictionSystem::StartPrediction(C_CSPlayer* pLocalPlayer, SSDK::CUserCmd* cmd) {
	static DWORD dwRunCommandAddress = (*(PDWORD*)SSDK::I::Prediction())[19];
	static DWORD dwPredictionSeedAddress = *(DWORD*)(dwRunCommandAddress + 0x30); //works
	static DWORD dwPredictionPlayerAddress = *(DWORD*)(dwRunCommandAddress + 0x54); //works

	static WORD wPlayerCommand = *(WORD*)(dwRunCommandAddress + 0x1B); //works
	static WORD wCurrentCommand = *(WORD*)(dwRunCommandAddress + 0x21); //works

	float flOldCurTime = SSDK::I::GlobalVars()->curtime;
	float flOldFrameTime = SSDK::I::GlobalVars()->frametime;

	int iTickBase = *pLocalPlayer->GetTickBase();

	SSDK::I::GlobalVars()->curtime = iTickBase * SSDK::I::GlobalVars()->interval_per_tick;
	SSDK::I::GlobalVars()->frametime = SSDK::I::GlobalVars()->interval_per_tick;

	bool v3 = *(bool*)((uintptr_t)SSDK::I::Prediction() + 8);
	*(bool*)((uintptr_t)SSDK::I::Prediction() + 8) = true;

	if (*(bool*)((uintptr_t)SSDK::I::Prediction() + 10))
		SSDK::I::GlobalVars()->frametime = 0;

	*(DWORD*)((DWORD)pLocalPlayer + wCurrentCommand) = (DWORD)cmd;

	int iOldPredictionSeed = *(int*)dwPredictionSeedAddress;

	*(int*)dwPredictionSeedAddress = cmd->random_seed;
	SSDK::I::MoveHelper()->SetHost(pLocalPlayer);

	//static SSDK::CMoveData *moveData = *(SSDK::CMoveData**)(SSDK::O::FindSignature("client.dll", "FF 35 ? ? ? ? 53 8B 06") + 2);

	SSDK::CMoveData moveData;
	memset(&moveData, 0, sizeof(moveData));

	SSDK::I::Prediction()->SetupMove(pLocalPlayer, cmd, SSDK::I::MoveHelper(), &moveData);
	SSDK::I::GameMovement()->ProcessMovement(pLocalPlayer, &moveData);

	*pLocalPlayer->GetTickBase() = iTickBase;
	SSDK::I::Prediction()->FinishMove(pLocalPlayer, cmd, &moveData);

	*(bool*)((uintptr_t)SSDK::I::Prediction() + 8) = v3;
	*(int*)dwPredictionSeedAddress = iOldPredictionSeed;

	SSDK::I::GlobalVars()->curtime = flOldCurTime;
	SSDK::I::GlobalVars()->frametime = flOldFrameTime;
}