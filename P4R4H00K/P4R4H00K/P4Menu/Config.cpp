#include "Config.h"
#include "Options.h"
#include"P4Menu.h"

void CConfig::Setup()
{
	//ESP
	SetupValue(MenuOptions.ESP.g_espON, false, strenc("ESP"), strenc("ESP Enabled"));
	SetupValue(MenuOptions.ESP.g_espBox, true, strenc("ESP"), strenc("ESP Box"));
	SetupValue(MenuOptions.ESP.g_espHealth, true, strenc("ESP"), strenc("ESP Health"));
	SetupValue(MenuOptions.ESP.g_espArmor, true, strenc("ESP"), strenc("ESP Armor"));	
	SetupValue(MenuOptions.ESP.g_espBone, true, strenc("ESP"), strenc("ESP Bone"));
	SetupValue(MenuOptions.ESP.g_espName, true, strenc("ESP"), strenc("ESP Name"));
	SetupValue(MenuOptions.ESP.g_espHead, true, strenc("ESP"), strenc("ESP Head"));
	SetupValue(MenuOptions.ESP.g_espDist, true, strenc("ESP"), strenc("ESP Distance"));
	SetupValue(MenuOptions.ESP.g_chamsOrGlow, 0, strenc("ESP"), strenc("Chams / Glow"));
	SetupValue(MenuOptions.ESP.g_espDroppedWeapons, false, strenc("ESP"), strenc("ESP Dropped Weapons"));
	SetupValue(MenuOptions.ESP.g_glowDroppedWeapons, true, strenc("ESP"), strenc("Glow Dropped Weapons"));
	SetupValue(MenuOptions.ESP.g_espWeapon, true, strenc("ESP"), strenc("ESP Weapons"));
	SetupValue(MenuOptions.ESP.g_ESPonlyEnemyTeam, true, strenc("ESP"), strenc("ESP Only enemies"));
	SetupValue(MenuOptions.ESP.g_ChamsGlowOnlyEnemyTeam, false, strenc("ESP"), strenc("Chams / Glow Only enemies"));
	SetupValue(MenuOptions.ESP.g_GrenadesEsp, true, strenc("ESP"), strenc("ESP Grenades"));
	SetupValue(MenuOptions.ESP.g_glowGrenades, true, strenc("ESP"), strenc("Glow Grenades"));
	SetupValue(MenuOptions.ESP.g_bombEsp, true, strenc("ESP"), strenc("Bomb ESP"));
	SetupValue(MenuOptions.ESP.g_glowBomb, true, strenc("ESP"), strenc("Glow Bomb"));
	SetupValue(MenuOptions.ESP.g_lbyUpdate, true, strenc("ESP"), strenc("LBY Update ESP"));
	SetupValue(MenuOptions.ESP.g_weaponsChams, 0, strenc("ESP"), strenc("Weapons Chams"));
	SetupValue(MenuOptions.ESP.g_armsChams, 0, strenc("ESP"), strenc("Arms Chams"));
	SetupValue(MenuOptions.ESP.g_playerChams, 0, strenc("ESP"), strenc("Players Chams"));	
	SetupValue(MenuOptions.ESP.g_deadbodiesChams, true, strenc("ESP"), strenc("Dead bodies Chams"));
	SetupValue(MenuOptions.ESP.g_ammoEsp, true, strenc("ESP"), strenc("Ammo ESP"));
	//SetupValue(MenuOptions.ESP.g_scoped, true, strenc("ESP"), strenc("Scoped ESP"));
	SetupValue(MenuOptions.ESP.g_damageIndicator, true, strenc("ESP"), strenc("Damage ESP"));
	SetupValue(MenuOptions.ESP.g_backtrackEsp, true, strenc("ESP"), strenc("Backtrack ESP"));
	

	
	//Aimbot
	SetupValue(MenuOptions.Aimbot.legitOrRage, 0, strenc("Aimbot"), strenc("Legit/Rage"));
	SetupValue(MenuOptions.Aimbot.g_hitboxPrioritize, 0, strenc("Aimbot"), strenc("Hitbox"));
	

	//Rage Aimbot
	SetupValue(MenuOptions.Aimbot.g_autoshootON, true, strenc("Aimbot Rage"), strenc("Autoshoot"));
	SetupValue(MenuOptions.Aimbot.iAutoWallType, 0, strenc("Aimbot Rage"), strenc("Autowall Type"));
	SetupValue(MenuOptions.Aimbot.g_autoWall, true, strenc("Aimbot Rage"), strenc("Autowall"));
	SetupValue(MenuOptions.Aimbot.g_fakeYawAA, 0, strenc("Aimbot Rage"), strenc("Fake Yaw AntiAim"));
	SetupValue(MenuOptions.Aimbot.g_fakeUnhittable, 0, strenc("Aimbot Rage"), strenc("Fake Unhittable Yaw AntiAim"));
	SetupValue(MenuOptions.Aimbot.g_realYawAA, 0, strenc("Aimbot Rage"), strenc("Real Yaw AntiAim"));
	SetupValue(MenuOptions.Aimbot.g_movingRealYawAA, 0, strenc("Aimbot Rage"), strenc("Real Yaw AntiAim Moving"));
	SetupValue(MenuOptions.Aimbot.g_movingYawAA, 0, strenc("Aimbot Rage"), strenc("Moving Yaw AntiAim"));
	SetupValue(MenuOptions.Aimbot.g_jumpingYawAA, 0, strenc("Aimbot Rage"), strenc("Jumping Yaw AntiAim"));
	SetupValue(MenuOptions.Aimbot.g_pitchAA, 0, strenc("Aimbot Rage"), strenc("Pitch AntiAim"));
	SetupValue(MenuOptions.Aimbot.g_minDamage, 7, strenc("Aimbot Rage"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Aimbot.g_minDamagePrio, 25, strenc("Aimbot Rage"), strenc("Min. Damage Prioritized"));
	SetupValue(MenuOptions.Aimbot.g_adaptiveMinDmg, true, strenc("Aimbot Rage"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Aimbot.g_minDmgAdapt, 7, strenc("Aimbot Rage"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Aimbot.g_minDmgPeeking, 40, strenc("Aimbot Rage"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Aimbot.g_hitchanceAdapt, 0, strenc("Aimbot Rage"), strenc("Hitchance Adaptive"));
	SetupValue(MenuOptions.Aimbot.g_hitChance, 35, strenc("Aimbot Rage"), strenc("Hitchance"));
	SetupValue(MenuOptions.Aimbot.g_hitchanceSeed, 256.f, strenc("Aimbot Rage"), strenc("Random Seed"));
	SetupValue(MenuOptions.Aimbot.g_fakeLagAmount, 1, strenc("Aimbot Rage"), strenc("Fakelag Amount"));
	SetupValue(MenuOptions.Aimbot.g_jumpingFakeLag, 1, strenc("Aimbot Rage"), strenc("Fakelag Jumping"));
	SetupValue(MenuOptions.Aimbot.g_movingFakeLag, 1, strenc("Aimbot Rage"), strenc("Fakelag Moving"));
	SetupValue(MenuOptions.Aimbot.g_spinSpeed, 0.1f, strenc("Aimbot Rage"), strenc("Spin Speed"));
	SetupValue(MenuOptions.Aimbot.g_silentaim, true, strenc("Aimbot Rage"), strenc("Silentaim SRV"));
	SetupValue(MenuOptions.Aimbot.g_novisrecoil, true, strenc("Aimbot Rage"), strenc("NoVisRecoil"));
	SetupValue(MenuOptions.Aimbot.g_autoBaim, false, strenc("Aimbot Rage"), strenc("Autobaim"));
	SetupValue(MenuOptions.Aimbot.g_shotsAutoBaim, false, strenc("Aimbot Rage"), strenc("Autobaim Shots Fired"));
	SetupValue(MenuOptions.Aimbot.g_healthAutoBaim, false, strenc("Aimbot Rage"), strenc("Autobaim Health"));
	SetupValue(MenuOptions.Aimbot.g_shotsFiredMin, 2, strenc("Aimbot Rage"), strenc("Shots Fired Min."));
	SetupValue(MenuOptions.Aimbot.g_baimMinHealth, 70, strenc("Aimbot Rage"), strenc("Health baim"));
	SetupValue(MenuOptions.Aimbot.g_hitboxBaim, 3, strenc("Aimbot Rage"), strenc("Hitbox Baim"));
	SetupValue(MenuOptions.Aimbot.g_autoHitbox, false, strenc("Aimbot Rage"), strenc("Auto Hitbox"));
	SetupValue(MenuOptions.Aimbot.g_resolver, 0, strenc("Aimbot Rage"), strenc("Resolver"));
	SetupValue(MenuOptions.Aimbot.bNadeAA, true, strenc("Aimbot Rage"), strenc("Nade AA"));
	SetupValue(MenuOptions.Aimbot.g_headPointScale, 0.85f, strenc("Aimbot Rage"), strenc("Point Scale Head"));
	SetupValue(MenuOptions.Aimbot.g_armsPointScale, 0.7f, strenc("Aimbot Rage"), strenc("Point Scale Arms"));
	SetupValue(MenuOptions.Aimbot.g_autoRevolver, true, strenc("Aimbot Rage"), strenc("Auto Revolver"));
	SetupValue(MenuOptions.Aimbot.g_checkBacktracked, true, strenc("Aimbot Rage"), strenc("Backtracking"));
	SetupValue(MenuOptions.Aimbot.g_unhittableBestSide, false, strenc("Aimbot Rage"), strenc("Unhittable Best Side"));


	//Legit Aimbot
	SetupValue(MenuOptions.Aimbot.g_fovValue, 20.f, strenc("Aimbot Legit"), strenc("FOV"));
	SetupValue(MenuOptions.Aimbot.g_smoothValue, 6.f, strenc("Aimbot Legit"), strenc("Smooth"));
	SetupValue(MenuOptions.Aimbot.g_triggerbotON, true, strenc("Aimbot Legit"), strenc("Triggerbot"));
	SetupValue(MenuOptions.Aimbot.g_rcs, true, strenc("Aimbot Legit"), strenc("RCS"));
	SetupValue(MenuOptions.Aimbot.g_mouseAuto, true, strenc("Aimbot Legit"), strenc("Mouse Autoshoot"));
	SetupValue(MenuOptions.Aimbot.g_fovMouseAuto, 170.f, strenc("Aimbot Legit"), strenc("FOV Mouse Autoshoot"));

	

	//Misc settings
	SetupValue(MenuOptions.Misc.g_bhopON, true, strenc("Misc"), strenc("Bunnyhop"));
	SetupValue(MenuOptions.Misc.g_nohands, false, strenc("Misc"), strenc("NoHands"));
	SetupValue(MenuOptions.Misc.g_crosshair, false, strenc("Misc"), strenc("Crosshair"));
	SetupValue(MenuOptions.Misc.g_radar, true, strenc("Misc"), strenc("Radar"));
	SetupValue(MenuOptions.Misc.g_noflash, false, strenc("Misc"), strenc("NoFlash"));
	SetupValue(MenuOptions.Misc.g_nosmoke, false, strenc("Misc"), strenc("NoSmoke"));
	SetupValue(MenuOptions.Misc.g_sonarON, false, strenc("Misc"), strenc("Sonar"));
	SetupValue(MenuOptions.Misc.g_sonarHZ, 500.f, strenc("Misc"), strenc("Sonar HZ"));
	SetupValue(MenuOptions.Misc.g_sonarDist, 800.f, strenc("Misc"), strenc("Sonar Dist"));
	SetupValue(MenuOptions.Misc.g_chatbot, false, strenc("Misc"), strenc("ChatBot"));
	SetupValue(MenuOptions.Misc.g_airstuckON, false, strenc("Misc"), strenc("Airstuck"));
	SetupValue(MenuOptions.Misc.g_autocrouch, false, strenc("Misc"), strenc("AutoCrouch"));
	SetupValue(MenuOptions.Misc.g_autoscope, false, strenc("Misc"), strenc("AutoScope"));
	SetupValue(MenuOptions.Misc.g_autostop, false, strenc("Misc"), strenc("AutoStop"));
	SetupValue(MenuOptions.Misc.g_animeClanTag, false, strenc("Misc"), strenc("Anime Clantag"));
	SetupValue(MenuOptions.Misc.g_fovPlayer, 90.f, strenc("Misc"), strenc("FOV Player"));
	SetupValue(MenuOptions.Misc.g_noscope, true, strenc("Misc"), strenc("No Scope"));
	SetupValue(MenuOptions.Misc.g_thirdperson, false, strenc("Misc"), strenc("Thirdperson"));
	SetupValue(MenuOptions.Misc.g_autostrafe, false, strenc("Misc"), strenc("Autostrafe"));
	SetupValue(MenuOptions.Misc.g_circleStrafe, false, strenc("Misc"), strenc("Circlestrafer"));
	SetupValue(MenuOptions.Misc.g_postprocess, true, strenc("Misc"), strenc("Post Process"));
	SetupValue(MenuOptions.Misc.g_spamchat, false, strenc("Misc"), strenc("SpamChat"));
	SetupValue(MenuOptions.Misc.g_spamtype, 0, strenc("Misc"), strenc("Spam Type"));
	SetupValue(MenuOptions.Misc.g_spamspeed, 0.7f, strenc("Misc"), strenc("Spam Speed"));
	SetupValue(MenuOptions.Misc.g_namespam, false, strenc("Misc"), strenc("NameSpam"));
	SetupValue(MenuOptions.Misc.g_watermark, true, strenc("Misc"), strenc("Watermark"));
	SetupValue(MenuOptions.Misc.g_clantagspeed, 0.3f, strenc("Misc"), strenc("Clantag Speed"));
	SetupValue(MenuOptions.Misc.g_chatbotDeadMode, 0, strenc("Misc"), strenc("Chatbot Dead Mode"));
	SetupValue(MenuOptions.Misc.g_chatbotKillMode, 0, strenc("Misc"), strenc("Chatbot Kill Mode"));
	SetupValue(MenuOptions.Misc.g_wireframeArms, false, strenc("Misc"), strenc("Wireframe Arms"));
	SetupValue(MenuOptions.Misc.g_generalChatbot, false, strenc("Misc"), strenc("General Chatbot"));
	SetupValue(MenuOptions.Misc.g_hitmarkerCrosshair, false, strenc("Misc"), strenc("Hitmarker Crosshair"));
	SetupValue(MenuOptions.Misc.g_hitmarkerSound, false, strenc("Misc"), strenc("Hitmarker Sound"));
	SetupValue(MenuOptions.Misc.g_lbyIndicator, false, strenc("Misc"), strenc("LBY Indicator"));
	SetupValue(MenuOptions.Misc.g_autozeus, false, strenc("Misc"), strenc("Auto Zeus"));
	SetupValue(MenuOptions.Misc.g_spec, false, strenc("Misc"), strenc("Spectators"));


	//WEAPONS CONFIGS
	//DUALS
	SetupValue(MenuOptions.Weapons.Duals.g_enabled, false, strenc("Duals"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.Duals.g_hitChance, 1, strenc("Duals"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.Duals.g_minDamage, 1, strenc("Duals"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.Duals.g_minDamagePrio, 1, strenc("Duals"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.Duals.g_hitbox, 0, strenc("Duals"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.Duals.g_autoHitbox, false, strenc("Duals"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.Duals.g_fov, 1.f, strenc("Duals"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.Duals.g_smooth, 1.f, strenc("Duals"), strenc("Smooth"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.Duals.g_adaptiveMinDmg, false, strenc("Duals"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.Duals.g_minDmgAdapt, 7, strenc("Duals"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.Duals.g_minDmgPeeking, 40, strenc("Duals"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.Duals.g_hitchanceAdapt, 0, strenc("Duals"), strenc("Hitchance Adaptive"));

	//Zeus
	SetupValue(MenuOptions.Weapons.Zeus.g_enabled, false, strenc("Zeus"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.Zeus.g_hitChance, 1, strenc("Zeus"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.Zeus.g_minDamage, 1, strenc("Zeus"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.Zeus.g_minDamagePrio, 1, strenc("Zeus"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.Zeus.g_hitbox, 0, strenc("Zeus"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.Zeus.g_autoHitbox, false, strenc("Zeus"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.Zeus.g_fov, 1.f, strenc("Zeus"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.Zeus.g_smooth, 1.f, strenc("Zeus"), strenc("Smooth"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.Zeus.g_adaptiveMinDmg, false, strenc("Zeus"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.Zeus.g_minDmgAdapt, 7, strenc("Zeus"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.Zeus.g_minDmgPeeking, 40, strenc("Zeus"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.Zeus.g_hitchanceAdapt, 0, strenc("Zeus"), strenc("Hitchance Adaptive"));


	//DEAGLE
	SetupValue(MenuOptions.Weapons.Deagle.g_enabled, false, strenc("Deagle"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.Deagle.g_hitChance, 1, strenc("Deagle"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.Deagle.g_minDamage, 1, strenc("Deagle"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.Deagle.g_minDamagePrio, 1, strenc("Deagle"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.Deagle.g_hitbox, 0, strenc("Deagle"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.Deagle.g_autoHitbox, false, strenc("Deagle"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.Deagle.g_fov, 1.f, strenc("Deagle"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.Deagle.g_smooth, 1.f, strenc("Deagle"), strenc("Smooth"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.Deagle.g_adaptiveMinDmg, false, strenc("Deagle"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.Deagle.g_minDmgAdapt, 7, strenc("Deagle"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.Deagle.g_minDmgPeeking, 40, strenc("Deagle"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.Deagle.g_hitchanceAdapt, 0, strenc("Deagle"), strenc("Hitchance Adaptive"));

	//REVOLVER
	SetupValue(MenuOptions.Weapons.Revolver.g_enabled, false, strenc("Revolver"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.Revolver.g_hitChance, 1, strenc("Revolver"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.Revolver.g_minDamage, 1, strenc("Revolver"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.Revolver.g_minDamagePrio, 1, strenc("Revolver"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.Revolver.g_hitbox, 0, strenc("Revolver"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.Revolver.g_autoHitbox, false, strenc("Revolver"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.Revolver.g_fov, 1.f, strenc("Revolver"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.Revolver.g_smooth, 1.f, strenc("Revolver"), strenc("Smooth"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.Revolver.g_adaptiveMinDmg, false, strenc("Revolver"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.Revolver.g_minDmgAdapt, 7, strenc("Revolver"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.Revolver.g_minDmgPeeking, 40, strenc("Revolver"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.Revolver.g_hitchanceAdapt, 0, strenc("Revolver"), strenc("Hitchance Adaptive"));

	//AUTOSNIPERS
	SetupValue(MenuOptions.Weapons.Autosnipers.g_enabled, false, strenc("Autosnipers"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_hitChance, 1, strenc("Autosnipers"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_minDamage, 1, strenc("Autosnipers"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_minDamagePrio, 1, strenc("Autosnipers"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_hitbox, 0, strenc("Autosnipers"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_autoHitbox, false, strenc("Autosnipers"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_fov, 1.f, strenc("Autosnipers"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_smooth, 1.f, strenc("Autosnipers"), strenc("Smooth"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_autoBaim, false, strenc("Autosnipers"), strenc("Autobaim"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_shotsBaim, false, strenc("Autosnipers"), strenc("Autobaim Shots Fired"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_healthBaim, false, strenc("Autosnipers"), strenc("Autobaim Health"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_shotsFiredMin, 2, strenc("Autosnipers"), strenc("Shots Fired Min."));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_baimMinHealth, 70, strenc("Autosnipers"), strenc("Health baim"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_hitboxBaim, 3, strenc("Autosnipers"), strenc("Hitbox Baim"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.Autosnipers.g_adaptiveMinDmg, false, strenc("Autosnipers"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_minDmgAdapt, 7, strenc("Autosnipers"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_minDmgPeeking, 40, strenc("Autosnipers"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.Autosnipers.g_hitchanceAdapt, 0, strenc("Autosnipers"), strenc("Hitchance Adaptive"));

	//SSG08
	SetupValue(MenuOptions.Weapons.SSG08.g_enabled, false, strenc("SSG"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.SSG08.g_hitChance, 1, strenc("SSG"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.SSG08.g_minDamage, 1, strenc("SSG"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.SSG08.g_minDamagePrio, 1, strenc("SSG"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.SSG08.g_hitbox, 0, strenc("SSG"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.SSG08.g_autoHitbox, false, strenc("SSG"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.SSG08.g_fov, 1.f, strenc("SSG"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.SSG08.g_smooth, 1.f, strenc("SSG"), strenc("Smooth"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.SSG08.g_adaptiveMinDmg, false, strenc("SSG08"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.SSG08.g_minDmgAdapt, 7, strenc("SSG08"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.SSG08.g_minDmgPeeking, 40, strenc("SSG08"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.SSG08.g_hitchanceAdapt, 0, strenc("SSG08"), strenc("Hitchance Adaptive"));

	//AWP
	SetupValue(MenuOptions.Weapons.AWP.g_enabled, false, strenc("AWP"), strenc("Enabled"));
	SetupValue(MenuOptions.Weapons.AWP.g_hitChance, 1, strenc("AWP"), strenc("Hitchance"));
	SetupValue(MenuOptions.Weapons.AWP.g_minDamage, 1, strenc("AWP"), strenc("Min. Damage"));
	SetupValue(MenuOptions.Weapons.AWP.g_minDamagePrio, 1, strenc("AWP"), strenc("Min. Damage Prio"));
	SetupValue(MenuOptions.Weapons.AWP.g_hitbox, 0, strenc("AWP"), strenc("Hitbox"));
	SetupValue(MenuOptions.Weapons.AWP.g_autoHitbox, false, strenc("AWP"), strenc("Auto hitbox"));
	SetupValue(MenuOptions.Weapons.AWP.g_fov, 1.f, strenc("AWP"), strenc("FOV"));
	SetupValue(MenuOptions.Weapons.AWP.g_smooth, 1.f, strenc("AWP"), strenc("Smooth"));
	//Adaptive
	SetupValue(MenuOptions.Weapons.AWP.g_adaptiveMinDmg, false, strenc("AWP"), strenc("Adaptive Mindmg"));
	SetupValue(MenuOptions.Weapons.AWP.g_minDmgAdapt, 7, strenc("AWP"), strenc("Min. Damage Adaptive"));
	SetupValue(MenuOptions.Weapons.AWP.g_minDmgPeeking, 40, strenc("AWP"), strenc("Min. Damage Peeking"));
	SetupValue(MenuOptions.Weapons.AWP.g_hitchanceAdapt, 0, strenc("AWP"), strenc("Hitchance Adaptive"));

	//Keybinds
	SetupValue(MenuOptions.Keys.g_aaside, 86, strenc("Key Binds"), strenc("Side AA Key"));
	SetupValue(MenuOptions.Keys.g_triggerbot, 5, strenc("Key Binds"), strenc("Triggerbot Key"));
	SetupValue(MenuOptions.Keys.g_mouseAutoshoot, 4, strenc("Key Binds"), strenc("Mouse autoshoot Key"));
	SetupValue(MenuOptions.Keys.g_airstuck, 113, strenc("Key Binds"), strenc("Airstuck Key"));
	SetupValue(MenuOptions.Keys.g_circleStrafe, 67, strenc("Key Binds"), strenc("CircleStrafer Key"));
	SetupValue(MenuOptions.Keys.g_thirdperson, 5, strenc("Key Binds"), strenc("Thirdperson Key"));
	SetupValue(MenuOptions.Keys.g_disableAA, 69, strenc("Key Binds"), strenc("Disable AA"));
	
}

void CConfig::SetupSkins()
{
	//SKINS
	SetupSkinValue(MenuOptions.Skins.iSelectedKnife, 0, strenc("Skins"), strenc("Knife"));
	SetupSkinValue(MenuOptions.Skins.iCurPaintKit, 0, strenc("Skins"), strenc("Knife PaintKit"));
	SetupSkinValue(MenuOptions.Skins.deagle, 0, strenc("Skins"), strenc("Deagle"));
	SetupSkinValue(MenuOptions.Skins.cz75, 0, strenc("Skins"), strenc("CZ75"));
	SetupSkinValue(MenuOptions.Skins.p2000, 0, strenc("Skins"), strenc("P2000"));
	SetupSkinValue(MenuOptions.Skins.elites, 0, strenc("Skins"), strenc("Elites"));
	SetupSkinValue(MenuOptions.Skins.p250, 0, strenc("Skins"), strenc("P250"));
	SetupSkinValue(MenuOptions.Skins.fiveseven, 0, strenc("Skins"), strenc("Fiveseven"));
	SetupSkinValue(MenuOptions.Skins.tec9, 0, strenc("Skins"), strenc("TEC9"));
	SetupSkinValue(MenuOptions.Skins.glock, 0, strenc("Skins"), strenc("Glock"));
	SetupSkinValue(MenuOptions.Skins.aug, 0, strenc("Skins"), strenc("AUG"));
	SetupSkinValue(MenuOptions.Skins.galil, 0, strenc("Skins"), strenc("GALIL"));
	SetupSkinValue(MenuOptions.Skins.usps, 0, strenc("Skins"), strenc("USPS"));
	SetupSkinValue(MenuOptions.Skins.ak47, 0, strenc("Skins"), strenc("AK47"));
	SetupSkinValue(MenuOptions.Skins.famas, 0, strenc("Skins"), strenc("FAMAS"));
	SetupSkinValue(MenuOptions.Skins.m4a4, 0, strenc("Skins"), strenc("M4A4"));
	SetupSkinValue(MenuOptions.Skins.m4a1s, 0, strenc("Skins"), strenc("M4A1s"));
	SetupSkinValue(MenuOptions.Skins.bizon, 0, strenc("Skins"), strenc("Bizon"));
	SetupSkinValue(MenuOptions.Skins.mac10, 0, strenc("Skins"), strenc("MAC10"));
	SetupSkinValue(MenuOptions.Skins.mp7, 0, strenc("Skins"), strenc("MP7"));
	SetupSkinValue(MenuOptions.Skins.mp9, 0, strenc("Skins"), strenc("MP9"));
	SetupSkinValue(MenuOptions.Skins.p90, 0, strenc("Skins"), strenc("P90"));
	SetupSkinValue(MenuOptions.Skins.m249, 0, strenc("Skins"), strenc("M249"));
	SetupSkinValue(MenuOptions.Skins.ump45, 0, strenc("Skins"), strenc("UMP45"));
	SetupSkinValue(MenuOptions.Skins.negev, 0, strenc("Skins"), strenc("NEGEV"));
	SetupSkinValue(MenuOptions.Skins.mag7, 0, strenc("Skins"), strenc("MAG7"));
	SetupSkinValue(MenuOptions.Skins.nova, 0, strenc("Skins"), strenc("NOVA"));
	SetupSkinValue(MenuOptions.Skins.sawedoff, 0, strenc("Skins"), strenc("SAWEDOFF"));
	SetupSkinValue(MenuOptions.Skins.xm1014, 0, strenc("Skins"), strenc("XM1014"));
	SetupSkinValue(MenuOptions.Skins.scar20, 0, strenc("Skins"), strenc("SCAR20"));
	SetupSkinValue(MenuOptions.Skins.awp, 0, strenc("Skins"), strenc("AWP"));
	SetupSkinValue(MenuOptions.Skins.ssg08, 0, strenc("Skins"), strenc("SSG08"));
	SetupSkinValue(MenuOptions.Skins.g3sg1, 0, strenc("Skins"), strenc("G3SG1"));
	SetupSkinValue(MenuOptions.Skins.magnum, 0, strenc("Skins"), strenc("MAGNUM"));
	SetupSkinValue(MenuOptions.Skins.sg556, 0, strenc("Skins"), strenc("SG556"));
}

void CConfig::SetupSkinValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	skinInts.push_back(new ConfigSkinValue<int>(category, name, &value));
}

void CConfig::SetupSkinValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	skinFloats.push_back(new ConfigSkinValue<float>(category, name, &value));
}

void CConfig::SetupSkinValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	skinBools.push_back(new ConfigSkinValue<bool>(category, name, &value));
}

void CConfig::SetupValue( int &value, int def, std::string category, std::string name )
{
	value = def;
	ints.push_back( new ConfigValue<int>( category, name, &value ) );
}

void CConfig::SetupValue( float &value, float def, std::string category, std::string name )
{
	value = def;
	floats.push_back( new ConfigValue<float>( category, name, &value ) );
}

void CConfig::SetupValue( bool &value, bool def, std::string category, std::string name )
{
	value = def;
	bools.push_back( new ConfigValue<bool>( category, name, &value ) );
}

void CConfig::Save(int isSkin)
{
	const char* configFile;
	static TCHAR path[ MAX_PATH ];
	std::string folder;

	if (!isSkin)
		configFile = configFileNames[MenuOptions.Menu.currentConfigID];
	else
		configFile = "SKINS.ini";

	std::string file("\\P4R4H00K-Configs\\");
	file.append(configFile);

	if( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, path ) ) )
	{
		folder = std::string(path) + strenc("\\P4R4H00K-Configs\\");
		file = std::string(path) + strenc(file);
	}

	CreateDirectory( folder.c_str(), NULL );

	
	
	if (!isSkin)
	{

		MenuOptions.Menu.currentConfig = configNames[MenuOptions.Menu.currentConfigID];

		for (auto value : ints)
			WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

		for (auto value : floats)
			WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

		for (auto value : bools)
			WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
	}
	else
	{
		for (auto value : skinInts)
			WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

		for (auto value : skinFloats)
			WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

		for (auto value : skinBools)
			WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
	}
}

void CConfig::Load(int isSkin)
{
	const char* configFile;
	static TCHAR path[MAX_PATH];
	std::string folder;

	if (!isSkin)
		configFile = configFileNames[MenuOptions.Menu.currentConfigID];
	else
		configFile = "SKINS.ini";

	std::string file("\\P4R4H00K-Configs\\");
	file.append(configFile);

	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, path ) ) )
	{
		folder = std::string(path) + strenc("\\P4R4H00K-Configs\\");
		file = std::string(path) + strenc(file);
	}

	CreateDirectory( folder.c_str(), NULL );

	if (FileExists(file))
	{
		char value_l[32] = { '\0' };
		
		if (isSkin == 0)
		{
			MenuOptions.Menu.currentConfig = configNames[MenuOptions.Menu.currentConfigID];

			for (auto value : ints)
			{
				GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
				*value->value = atoi(value_l);
			}

			for (auto value : floats)
			{
				GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
				*value->value = atof(value_l);
			}

			for (auto value : bools)
			{
				GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
				*value->value = !strcmp(value_l, "true");
			}
		}
		else
		{
			for (auto value : skinInts)
			{
				GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
				*value->value = atoi(value_l);
			}

			for (auto value : skinFloats)
			{
				GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
				*value->value = atof(value_l);
			}

			for (auto value : skinBools)
			{
				GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
				*value->value = !strcmp(value_l, "true");
			}
		}
	}
	else if (!isSkin)
	{
		MenuOptions.Menu.currentConfigID = 0;
	}
}

bool CConfig::FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}


CConfig* Config = new CConfig();
Options MenuOptions;