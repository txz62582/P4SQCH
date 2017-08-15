#pragma once
#include "../EntityManager.h"
#include "Config.h"

struct DamageIndicator_t
{
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	C_CSPlayer * Player;
	SSDK::Vector Position;
};

struct Options
{
	Options()
	{
		
	}

	struct MENU_Options
	{
		std::vector<DamageIndicator_t> DamageIndicator;
		float _flHurtTime;
		bool g_bMainWindowOpen = false;

		struct EnemyEntity {
			int minDamage = 0;
			int minDamagePrio = 0;
			int g_hitChance = 0;
			bool autoHitbox = false;
			int hitbox = 7;
			bool baim = false;
			bool shotsbaim = false;
			bool healthbaim = false;
			int hitboxbaim = 3;
			int shotsfiredmin = 2;
			int g_baimMinHealth = 70;
			bool customResolve = false;
			int resolver = 3;
			bool customHitbox = false;
			bool customAutowall = false;
			bool customHitchance = false;
			bool customMinDamage = false;
			bool bFlip = false;
		} TargetList[64];

		const char* currentConfig = "No config";
		int currentConfigID = 0;
		bool isAwalling = false;
		char* nameToSpam = "P4R4H00K";
		bool bLocalLowerbodyUpdate = false;
		bool skyUsing = false;
		float nextTime = 0;
		int shotsFired = 0;
		int totalShotFired = 0;
		int playedJokeSentences[49] = { 0 };
		int playedFlameSentences[56] = { 0 };
		HMODULE hInst;
		int speedSonar = 0;
		bool activeSonar = false;
		bool unhittable_side = false;
		float currenthittableyaw;
		bool SaveLastConfig = false;
		C_CSPlayer* currentTarget = nullptr;
		struct LocalPlayerShot {
			float flLastTimeShot;
			bool hit;
			bool hurt;
			float angle;
		} LocalPlayerShotdata[64];
	} Menu;

	struct WEAPONS_Options
	{
		struct Zeus_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}Zeus;
		struct Duals_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}Duals;
		struct Deagle_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}Deagle;
		struct Revolver_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}Revolver;
		struct Autosnipers_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_autoBaim;
			bool g_shotsBaim;
			bool g_healthBaim;
			int g_shotsFiredMin;
			int g_hitboxBaim;
			int g_baimMinHealth;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}Autosnipers;
		struct SSG08_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}SSG08;
		struct AWP_Options
		{
			bool g_autoHitbox;
			bool  g_enabled;
			int g_hitChance;
			int g_minDamage;
			int g_minDamagePrio;
			int g_hitbox;
			float g_smooth;
			float g_fov;
			bool g_adaptiveMinDmg;
			int g_minDmgAdapt;
			int g_hitchanceAdapt;
			int g_minDmgPeeking;
		}AWP;
	}Weapons;

	struct OVERWRITTEN_Options
	{
		bool g_autoHitbox;
		int g_hitChance;
		int g_minDamage;
		int g_minDamagePrio;
		int hitbox;
		float smooth;
		float fov;
		bool g_autoBaim;
		bool g_shotsAutoBaim;
		bool g_healthAutoBaim;
		int shotsFiredMin;
		int baimMinHealth;
		int hitboxbaim;
		bool g_adaptiveMinDmg;
		int g_minDmgAdapt;
		int g_hitchanceAdapt;
		int g_minDmgPeeking;
	}Overwritten;

	struct ESP_Options
	{
		//ESP
		bool g_espON;
		bool g_espBox;
		bool g_espHealth;
		bool g_espArmor;
		bool g_espBone;
		bool g_espName;
		bool g_espHead;
		bool g_espDist;
		int g_chamsOrGlow;
		bool g_espDroppedWeapons;
		bool g_espWeapon;
		bool g_glowDroppedWeapons;
		bool g_ESPonlyEnemyTeam;
		bool g_ChamsGlowOnlyEnemyTeam;
		bool g_GrenadesEsp;
		bool g_glowGrenades;
		bool g_bombEsp;
		bool g_glowBomb;
		bool g_lbyUpdate;
		int g_weaponsChams;
		int g_armsChams;
		int g_playerChams;
		bool g_deadbodiesChams;
		bool g_ammoEsp;
		//bool g_scoped;
		bool g_damageIndicator;
		bool g_backtrackEsp;
	}ESP;

	struct AimbotOptions
	{
		//Legit aimbot
		int g_hitboxValue;
		float g_fovValue;
		float g_smoothValue;
		bool g_triggerbotON;
		bool g_rcs;
		float g_fovMouseAuto;

		//Rage aimbot
		int legitOrRage;//0 legit, 2 no aimbot, 1 rage
		bool g_autoshootON;
		bool g_autoWall = true;
		int iAutoWallType; //0 = old, 1 = New
		int g_hitChance;
		float g_hitchanceSeed;
		bool g_unhittableBestSide;
		int g_realYawAA;
		int g_movingRealYawAA;
		int g_fakeYawAA;
		int g_fakeUnhittable;
		int g_movingYawAA;
		int g_jumpingYawAA;
		int g_pitchAA;
		bool bNadeAA;
		int g_hitboxPrioritize;
		int g_minDamage;
		int g_minDamagePrio;
		int g_fakeLagAmount;
		int g_jumpingFakeLag;
		int g_movingFakeLag;
		bool g_mouseAuto;
		float g_spinSpeed;
		bool g_silentaim;
		bool g_novisrecoil;
		int g_resolver;
		bool g_autoHitbox;
		float g_headPointScale;
		float g_armsPointScale;
		bool g_autoRevolver;
		//BAIM
		bool g_autoBaim;
		bool g_shotsAutoBaim;
		bool g_healthAutoBaim;

		int g_shotsFiredMin;
		int g_baimMinHealth;
		int g_hitboxBaim;

		//Adaptive
		bool g_adaptiveMinDmg;
		int g_minDmgAdapt;
		int g_hitchanceAdapt;
		int g_minDmgPeeking;

		//Backtracking
		bool g_checkBacktracked;
	}Aimbot;

	struct MiscOptions
	{
		//Misc settings
	 	 bool g_autozeus;
		 bool g_bhopON;
		 bool g_nohands;
		 bool g_crosshair;
		 bool g_radar;
		 bool g_noflash;
		 bool g_nosmoke;
		 bool g_sonarON;
		 float g_sonarHZ;
		 float g_sonarDist;
		 bool g_chatbot;
		 bool g_airstuckON;
		 bool g_autocrouch;
		 bool g_autoscope;
		 bool g_watermark;
		 bool g_autostop;
		 bool g_animeClanTag;
		 float g_fovPlayer;
		 bool g_noscope;
		 bool g_thirdperson;
		 bool g_autostrafe;
		 bool g_postprocess;
		 bool g_spamchat;
		 int g_spamtype;
		 float g_spamspeed;
		 bool g_namespam;
		 bool g_wireframeArms;
		 float g_clantagspeed;
		 //1 = Joke 2=Flame
		 int g_chatbotDeadMode;
		 //1 = Rekt by P4R4H00K.XYZ 2=Joke 3=Flame
		 int g_chatbotKillMode;
		 bool g_generalChatbot;
		 bool g_circleStrafe;

		 //Hitmarker
		 bool g_hitmarkerCrosshair;
		 bool g_hitmarkerSound;

		 bool g_lbyIndicator;
		 bool g_spec;

	}Misc;

	//SKINCHANGER related
	struct SkinChanger
	{
		int iSelectedKnife = 0;
		int iCurPaintKit = 0;

		int deagle;
		int cz75;
		int p2000;
		int elites;
		int p250;
		int fiveseven;
		int tec9;
		int glock;
		int aug;
		int galil;
		int usps;
		int ak47;
		int famas;
		int m4a4;
		int m4a1s;
		int bizon;
		int mac10;
		int mp7;
		int mp9;
		int p90;
		int m249;
		int ump45;
		int negev;
		int mag7;
		int nova;
		int sawedoff;
		int xm1014;
		int scar20;
		int awp;
		int ssg08;
		int g3sg1;
		int magnum;
		int sg556;
	} Skins;

	//Keybinds
	struct KeyBinds {
		int g_aaside;
		int g_triggerbot;
		int g_mouseAutoshoot;
		int g_airstuck;
		int g_circleStrafe;
		int g_thirdperson;
		int g_disableAA;
	}Keys;

	void CheckWeapons(C_BaseCombatWeapon *activeWeapon)
	{
		if (activeWeapon)
		{
			CBaseAttributableItem *pAttribWeapon = (CBaseAttributableItem*)activeWeapon;

			switch (*pAttribWeapon->GetItemDefinitionIndex())
			{
				case SSDK::EItemDefinitionIndex::weapon_taser:
					if (this->Weapons.Zeus.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.Zeus.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.Zeus.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.Zeus.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.Zeus.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.Zeus.g_hitbox;
						this->Overwritten.fov = this->Weapons.Zeus.g_fov;
						this->Overwritten.smooth = this->Weapons.Zeus.g_smooth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.Zeus.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.Zeus.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.Zeus.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.Zeus.g_minDmgPeeking;
					}
					break;

				case SSDK::EItemDefinitionIndex::weapon_deagle:
					if (this->Weapons.Deagle.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.Deagle.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.Deagle.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.Deagle.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.Deagle.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.Deagle.g_hitbox;
						this->Overwritten.fov = this->Weapons.Deagle.g_fov;
						this->Overwritten.smooth = this->Weapons.Deagle.g_smooth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.Deagle.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.Deagle.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.Deagle.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.Deagle.g_minDmgPeeking;
					}
					break;

				case SSDK::EItemDefinitionIndex::weapon_revolver:
					if (this->Weapons.Revolver.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.Revolver.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.Revolver.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.Revolver.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.Revolver.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.Revolver.g_hitbox;
						this->Overwritten.fov = this->Weapons.Revolver.g_fov;
						this->Overwritten.smooth = this->Weapons.Revolver.g_smooth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.Revolver.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.Revolver.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.Revolver.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.Revolver.g_minDmgPeeking;
					}
					break;

				default:
					break;
			
			}

			switch (activeWeapon->GetId())
			{
				case SSDK::EItemDefinitionIndex::weapon_elite:
					if (this->Weapons.Duals.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.Duals.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.Duals.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.Duals.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.Duals.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.Duals.g_hitbox;
						this->Overwritten.fov = this->Weapons.Duals.g_fov;
						this->Overwritten.smooth = this->Weapons.Duals.g_smooth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.Duals.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.Duals.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.Duals.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.Duals.g_minDmgPeeking;
					}
					break;
				case SSDK::EItemDefinitionIndex::weapon_scar20:
					if (this->Weapons.Autosnipers.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.Autosnipers.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.Autosnipers.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.Autosnipers.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.Autosnipers.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.Autosnipers.g_hitbox;
						this->Overwritten.fov = this->Weapons.Autosnipers.g_fov;
						this->Overwritten.smooth = this->Weapons.Autosnipers.g_smooth;
						this->Overwritten.g_autoBaim = this->Weapons.Autosnipers.g_autoBaim;
						this->Overwritten.hitboxbaim = this->Weapons.Autosnipers.g_hitboxBaim;
						this->Overwritten.g_shotsAutoBaim = this->Weapons.Autosnipers.g_shotsBaim;
						this->Overwritten.g_healthAutoBaim = this->Weapons.Autosnipers.g_healthBaim;
						this->Overwritten.shotsFiredMin = this->Weapons.Autosnipers.g_shotsFiredMin;
						this->Overwritten.baimMinHealth = this->Weapons.Autosnipers.g_baimMinHealth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.Autosnipers.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.Autosnipers.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.Autosnipers.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.Autosnipers.g_minDmgPeeking;
					}
					break;
				case SSDK::EItemDefinitionIndex::weapon_g3sg1:
					if (this->Weapons.Autosnipers.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.Autosnipers.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.Autosnipers.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.Autosnipers.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.Autosnipers.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.Autosnipers.g_hitbox;
						this->Overwritten.fov = this->Weapons.Autosnipers.g_fov;
						this->Overwritten.smooth = this->Weapons.Autosnipers.g_smooth;
						this->Overwritten.g_autoBaim = this->Weapons.Autosnipers.g_autoBaim;
						this->Overwritten.hitboxbaim = this->Weapons.Autosnipers.g_hitboxBaim;
						this->Overwritten.g_shotsAutoBaim = this->Weapons.Autosnipers.g_shotsBaim;
						this->Overwritten.g_healthAutoBaim = this->Weapons.Autosnipers.g_healthBaim;
						this->Overwritten.shotsFiredMin = this->Weapons.Autosnipers.g_shotsFiredMin;
						this->Overwritten.baimMinHealth = this->Weapons.Autosnipers.g_baimMinHealth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.Autosnipers.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.Autosnipers.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.Autosnipers.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.Autosnipers.g_minDmgPeeking;
					}
					break;
				case SSDK::EItemDefinitionIndex::weapon_ssg08:
					if (this->Weapons.SSG08.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.SSG08.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.SSG08.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.SSG08.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.SSG08.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.SSG08.g_hitbox;
						this->Overwritten.fov = this->Weapons.SSG08.g_fov;
						this->Overwritten.smooth = this->Weapons.SSG08.g_smooth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.SSG08.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.SSG08.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.SSG08.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.SSG08.g_minDmgPeeking;
					}
					break;
				case SSDK::EItemDefinitionIndex::weapon_awp:
					if (this->Weapons.AWP.g_enabled)
					{
						this->Overwritten.g_autoHitbox = this->Weapons.AWP.g_autoHitbox;
						this->Overwritten.g_hitChance = this->Weapons.AWP.g_hitChance;
						this->Overwritten.g_minDamage = this->Weapons.AWP.g_minDamage;
						this->Overwritten.g_minDamagePrio = this->Weapons.AWP.g_minDamagePrio;
						this->Overwritten.hitbox = this->Weapons.AWP.g_hitbox;
						this->Overwritten.fov = this->Weapons.AWP.g_fov;
						this->Overwritten.smooth = this->Weapons.AWP.g_smooth;

						//Adaptive
						this->Overwritten.g_adaptiveMinDmg = this->Weapons.AWP.g_adaptiveMinDmg;
						this->Overwritten.g_minDmgAdapt = this->Weapons.AWP.g_minDmgAdapt;
						this->Overwritten.g_hitchanceAdapt = this->Weapons.AWP.g_hitchanceAdapt;
						this->Overwritten.g_minDmgPeeking = this->Weapons.AWP.g_minDmgPeeking;
					}
					break;
				default:
					break;
			}
		}
	}

	void CheckLegit(int EntIndex)
	{
		if (this->Aimbot.g_mouseAuto && GetAsyncKeyState(this->Keys.g_mouseAutoshoot))
		{
			this->Overwritten.fov = this->Aimbot.g_fovMouseAuto;
			this->Overwritten.smooth = 1;
		}

		if (this->Menu.TargetList[EntIndex].hitbox != 7)
		{
			this->Overwritten.hitbox = this->Menu.TargetList[EntIndex].hitbox;
		}
	}

	void SetDefaultLegitValues()
	{
		this->Overwritten.hitbox = this->Aimbot.g_hitboxPrioritize;
		this->Overwritten.fov = this->Aimbot.g_fovValue;
		this->Overwritten.smooth = this->Aimbot.g_smoothValue;
	}

	void SetDefaultAwallValues()
	{
		this->Overwritten.g_autoHitbox = this->Aimbot.g_autoHitbox;
		this->Overwritten.hitbox = this->Aimbot.g_hitboxPrioritize;
		this->Overwritten.g_minDamage = this->Aimbot.g_minDamage;
		this->Overwritten.g_minDamagePrio = this->Aimbot.g_minDamagePrio;
		this->Overwritten.g_autoBaim = this->Aimbot.g_autoBaim;
		this->Overwritten.hitboxbaim = this->Aimbot.g_hitboxBaim;
		this->Overwritten.g_healthAutoBaim = this->Aimbot.g_healthAutoBaim;
		this->Overwritten.g_shotsAutoBaim = this->Aimbot.g_shotsAutoBaim;
		this->Overwritten.shotsFiredMin = this->Aimbot.g_shotsFiredMin;
		this->Overwritten.baimMinHealth = this->Aimbot.g_baimMinHealth;
		this->Overwritten.g_hitChance = this->Aimbot.g_hitChance;
		//Adaptive
		this->Overwritten.g_adaptiveMinDmg = this->Aimbot.g_adaptiveMinDmg;
		this->Overwritten.g_minDmgAdapt = this->Aimbot.g_minDmgAdapt;
		this->Overwritten.g_hitchanceAdapt = this->Aimbot.g_hitchanceAdapt;
		this->Overwritten.g_minDmgPeeking = this->Aimbot.g_minDmgPeeking;
	}

	/*void SetDefaultRageValues()
	{
		this->Overwritten.hitbox = this->Aimbot.g_hitboxPrioritize;
		this->Overwritten.g_hitChance = this->Aimbot.g_hitChance;
	}*/

	void CheckTargetList(int EntIndex)
	{
		if (this->Menu.TargetList[EntIndex].customHitbox)
		{
			this->Overwritten.g_autoHitbox = this->Menu.TargetList[EntIndex].autoHitbox;

			if (!this->Menu.TargetList[EntIndex].autoHitbox)
			{
				this->Overwritten.hitbox = this->Menu.TargetList[EntIndex].hitbox;
			}
		}

		if (this->Menu.TargetList[EntIndex].g_hitChance != 0)
		{
			this->Overwritten.g_hitChance = this->Menu.TargetList[EntIndex].g_hitChance;
		}

		if (this->Menu.TargetList[EntIndex].customAutowall)
		{
			if (this->Menu.TargetList[EntIndex].customMinDamage)
			{
				this->Overwritten.g_minDamage = this->Menu.TargetList[EntIndex].minDamage;
				this->Overwritten.g_minDamagePrio = this->Menu.TargetList[EntIndex].minDamagePrio;
			}

			if (this->Menu.TargetList[EntIndex].baim)
			{
				if (this->Menu.TargetList[EntIndex].healthbaim)
				{
					this->Overwritten.g_healthAutoBaim = this->Menu.TargetList[EntIndex].healthbaim;
					this->Overwritten.baimMinHealth = this->Menu.TargetList[EntIndex].g_baimMinHealth;
				}
				if (this->Menu.TargetList[EntIndex].shotsbaim)
				{
					this->Overwritten.g_shotsAutoBaim = this->Menu.TargetList[EntIndex].shotsbaim;
					this->Overwritten.shotsFiredMin = this->Menu.TargetList[EntIndex].shotsfiredmin;
				}
				this->Overwritten.g_autoBaim = this->Menu.TargetList[EntIndex].baim;
				this->Overwritten.hitboxbaim = this->Menu.TargetList[EntIndex].hitboxbaim;		
			}
		}
	}

	void CheckTargetListRage(int EntIndex)
	{
		if (this->Menu.TargetList[EntIndex].customHitbox)
		{
			this->Overwritten.g_autoHitbox = this->Menu.TargetList[EntIndex].autoHitbox;

			if (!this->Menu.TargetList[EntIndex].autoHitbox)
			{
				this->Overwritten.hitbox = this->Menu.TargetList[EntIndex].hitbox;
			}
		}

		if (this->Menu.TargetList[EntIndex].customHitchance)
		{
			this->Overwritten.g_hitChance = this->Menu.TargetList[EntIndex].g_hitChance;
		}
	}
};
extern Options MenuOptions;