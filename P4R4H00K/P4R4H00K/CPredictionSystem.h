#pragma once
#include "SqchSDK\SDK.h"

#include "EntityManager.h"

class CPredictionSystem {
public:
	void StartPrediction(C_CSPlayer* pLocalPlayer, SSDK::CUserCmd *cmd);
};
