#pragma once

#pragma comment(lib, "Winmm.lib") 

#include <Windows.h>
#include "Includes.h"
#include "EntityManager.h"

#include "SqchSDK\SDK.h"

#include "resource.h"

#pragma comment(lib, "Winmm.lib") 

class bullet
{
	class bullet_listener
		: public SSDK::IGameEventListener2
	{
	public:
		void start()
		{
			if (!SSDK::I::GameEventManager2()->AddListener(this, XorStr("bullet_impact"), false)) {
				return;
			}
		}

		void stop()
		{
			SSDK::I::GameEventManager2()->RemoveListener(this);
		}

		void FireGameEvent(SSDK::IGameEvent *event) override
		{
			bullet::singleton()->on_fire_event(event);
		}

		int GetEventDebugID(void) override
		{
			return 0x2A;
		}
	};
public:
	static bullet* singleton()
	{
		static bullet* instance = new bullet;
		return instance;
	}

	void initialize(HMODULE hBaseModule)
	{
		hInst = hBaseModule;
		_listener.start();
	}

	void stopit() {
		_listener.stop();
	}

	void on_fire_event(SSDK::IGameEvent* event)
	{
		if (!strcmp(event->GetName(), XorStr("bullet_impact"))) {
			C_CSPlayer* pLocalPlayer = C_CSPlayer::GetLocalPlayer();
			C_CSPlayer* userId = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetPlayerForUserID(event->GetInt("userid")));

			if (!pLocalPlayer || !userId)
				return;

			//Si le tireur c'est nous
			if (userId == pLocalPlayer)
			{
				SSDK::Vector vecEnd = SSDK::Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

				SSDK::Vector vecDir = vecEnd - pLocalPlayer->GetEyePos();
				vecDir.NormalizeInPlace();

				if (!MenuOptions.Menu.currentTarget)
					return;


				if (pLocalPlayer->canHit(vecEnd + vecDir * 40.f, MenuOptions.Menu.currentTarget)) {
					//SSDK::I::CVar()->ConsolePrintf("Event HIT ! %i\n", MenuOptions.Menu.currentTarget->EntIndex());

					hit = true;
				}
				else {
					//SSDK::I::CVar()->ConsolePrintf("!!! Event NOT HIT !!! %i\n", MenuOptions.Menu.currentTarget->EntIndex());

					hit = false;
				}

				if (hit) {
					MenuOptions.Menu.LocalPlayerShotdata[MenuOptions.Menu.currentTarget->EntIndex()].flLastTimeShot = SSDK::I::GlobalVars()->curtime;
					MenuOptions.Menu.LocalPlayerShotdata[MenuOptions.Menu.currentTarget->EntIndex()].hit = hit;
				}
			}
		}
	}

private:
	bullet_listener    _listener;
	HMODULE hInst;
	bool hit;
};


class bullet_hurt
{
	class bullet_listener
		: public SSDK::IGameEventListener2
	{
	public:
		void start()
		{
			if (!SSDK::I::GameEventManager2()->AddListener(this, XorStr("player_hurt"), false)) {
				return;
			}
		}

		void stop()
		{
			SSDK::I::GameEventManager2()->RemoveListener(this);
		}

		void FireGameEvent(SSDK::IGameEvent *event) override
		{
			bullet_hurt::singleton()->on_fire_event(event);
		}

		int GetEventDebugID(void) override
		{
			return 0x2A;
		}
	};
public:
	static bullet_hurt* singleton()
	{
		static bullet_hurt* instance = new bullet_hurt;
		return instance;
	}

	void initialize(HMODULE hBaseModule)
	{
		hInst = hBaseModule;
		_listener.start();
	}

	void stopit() {
		_listener.stop();
	}

	void on_fire_event(SSDK::IGameEvent* event)
	{
		if (!strcmp(event->GetName(), XorStr("player_hurt"))) {
			int hurted = event->GetInt(XorStr("userid"));

			C_CSPlayer* pHurtEnt = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetPlayerForUserID(event->GetInt("userid")));
			C_CSPlayer* attacker = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(SSDK::I::Engine()->GetPlayerForUserID(event->GetInt("attacker")));
			C_CSPlayer* pLocalPlayer = C_CSPlayer::GetLocalPlayer();
			if (pHurtEnt != pLocalPlayer && attacker == pLocalPlayer && pHurtEnt)
			{
				if (MenuOptions.Menu.LocalPlayerShotdata[pHurtEnt->EntIndex()].flLastTimeShot == SSDK::I::GlobalVars()->curtime)
				{
					//SSDK::I::CVar()->ConsolePrintf("Event HURT ! %i\n", pHurtEnt->EntIndex());
					MenuOptions.Menu.LocalPlayerShotdata[pHurtEnt->EntIndex()].hurt = true;
				}

				//Hitmarker
				if (MenuOptions.Misc.g_hitmarkerSound || MenuOptions.Misc.g_hitmarkerCrosshair)
				{

					//g_pSurface->PlaySound doesnt work for some reason
					if (MenuOptions.Misc.g_hitmarkerSound)
						PlaySoundA(MAKEINTRESOURCE(IDR_WAVE1), MenuOptions.Menu.hInst, SND_RESOURCE | SND_ASYNC);

					MenuOptions.Menu._flHurtTime = SSDK::I::GlobalVars()->curtime;
				}


				//Damage indic
				if (MenuOptions.ESP.g_damageIndicator)
				{
					DamageIndicator_t DmgIndicator;
					DmgIndicator.iDamage = event->GetInt("dmg_health");
					DmgIndicator.Player = pHurtEnt;
					DmgIndicator.flEraseTime = *pLocalPlayer->GetTickBase() * SSDK::I::GlobalVars()->interval_per_tick + 3.f;
					DmgIndicator.bInitialized = false;
					MenuOptions.Menu.DamageIndicator.push_back(DmgIndicator);
				}
			}
		}
	}

	void on_paint()
	{

	}

private:
	bullet_listener    _listener;
	HMODULE hInst;
};
