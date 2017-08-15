#pragma once
#include <Windows.h>

#include "NetvarManager.h"
#include "VTableFuncHooks.h"
#include "Utils.h"

#include "SqchMenu\Menu.h"
#include "SqchSDK\SDK.h"

#include "EntityManager.h"
#include "DrawManager.h"

#include <iostream>
#include <chrono>
#include <ctime>

#include <algorithm>
#include <random>
#include "P4Menu\Config.h"

extern std::vector<std::string> jokesSentences;
extern std::vector<std::string> flameSentences;

class Misc
{
public:
	int number;
	int jokeArrayCounter = 0;
	int flameArrayCounter = 0;
	std::random_device random;
	float GetTraceFractionWorldProps(SSDK::Vector startpos, SSDK::Vector endpos);
	void CircleStrafer(SSDK::CUserCmd* pCmd, bool& active);
	void ForceThirdPerson(SSDK::CUserCmd* pCmd, C_CSPlayer* pLocalPlayer);
	template<class T, class U>
	T Clamp(T in, U low, U high);
	void SpoofConvars();
	void Chatspam(int spamType);
	void BunnyHop(SSDK::CUserCmd* pCmd, C_CSPlayer* pLocalPlayer);
	void AirStuck(SSDK::CUserCmd* pCmd);
	bool IsKnife(int nWeaponIndex);
	bool IsGrenade(int classID);
	void SetNightMode();
	SSDK::IMaterial* GetChams(int chams);
	bool FindBestSide(C_CSPlayer* pLocalPlayer, float flDistance, SSDK::CUserCmd* pCmd, SSDK::QAngle& qAngle);
};

extern Misc MiscExt;