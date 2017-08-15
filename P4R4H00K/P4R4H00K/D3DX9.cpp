#include "Includes.h"
#include "imgui\imgui.h"
#include "imgui\DX9\imgui_impl_dx9.h"
#include "P4Menu\P4Menu.h"
#include "P4Menu\Config.h"
#include "P4Menu\Options.h"
#include "Antiaim\Antiaim.h"
#include "Misc.h"
#include <future>

EndScene_t	g_fnOriginalEndScene = nullptr;
Reset_t	g_fnOriginalReset = nullptr;
WNDPROC	g_pOldWindowProc = nullptr;
HWND	g_hWindow = nullptr; //Handle to the CSGO window


static ImFont* ruda14;
static ImFont* ruda16;
static ImFont* ruda18;
static ImFont* ruda22;
static ImFont* ruda26;
static ImFont* ruda36;

bool	g_bWasInitialized = false;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const char* keyNames[] =
{
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};



//Initialize the GUI of the menu. With colors and everything
void GUI_Init(IDirect3DDevice9* pDevice)
{
	//Initializes the GUI and the renderer
	ImGui_ImplDX9_Init(g_hWindow, pDevice);

	//Set style of GUI
	ImGuiStyle& style = ImGui::GetStyle();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 14);
	ruda14 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 14);
	ruda16 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 16);
	ruda18 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 18);
	ruda22 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 22);
	ruda26 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 26);
	ruda36 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 36);

	style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.00f); //Text color
	style.Colors[ImGuiCol_WindowBg] = ImVec4(.137f, .137f, .137f, 1.00f); //Menu background color
																		  //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);	
	style.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.6f, 0.84f, 1.00f); //Borders color of the menu
	style.Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f); //Button color
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.f, 0.4f, 0.6f, 0.9f); //Buton color when we go with the mouse
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f); //Button color when we press
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.f, 0.4f, 0.6f, 1.00f); //Title of the menu background color
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.f, 0.4f, 0.6f, 1.00f); //Title of the active menu background color
																		   //style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.9f); //Background color of the checkbox ...
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f); //Background of the item when hovered
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2f, 0.3f, 0.70f, 0.99f); //Background color of the active item

	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.8f); //Combobox background color
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.f, 0.6f, 0.84f, 1.00f); //Radiobutton or checkbox mark color

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f); //Scrollbar background color
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.f, 0.6f, 0.84f, 1.00f); //Scrollbar color
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.f, 0.6f, 0.84f, 0.6f); //Scrollbar color when hovered
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f); //Scrollbar color when grabed
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.f, 1.f, 1.f, 0.8f); //Slider color 
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.f, 0.6f, 0.84f, 1.00f);//Slider color when grabed
	style.Colors[ImGuiCol_Header] = ImVec4(0.f, 0.4f, 0.6f, 1.00f);//Color of the selected item in a combolist
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.f, 0.4f, 0.6f, 0.6f);//Color of the hovered item in a combolist
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);//Color of the selected item in a combolist when pressed


	style.WindowRounding = 0.f;
	style.FramePadding = ImVec2(4, 1);
	style.ScrollbarSize = 10.f;
	style.ScrollbarRounding = 0.f;
	style.GrabMinSize = 10.f;

	g_bWasInitialized = true;
}

void DRAWESP()
{
	static const char *chams[] = {
		XorStr("NONE"),
		XorStr("White"),
		XorStr("Bubble"),
		XorStr("Crystal Blue"),
		XorStr("Crytal Clear"),
		XorStr("MP3"),
		XorStr("Platinum"),
		XorStr("Gold"),
		XorStr("Dark Chrome"),
		XorStr("Plastic Glass"),
		XorStr("Velvet"),
		XorStr("Dogtags Lightray"),
		XorStr("Dogtags Outline"),
		XorStr("Dreamhack Star Blur"),
		XorStr("Gloss"),
		XorStr("Fishing Net"),
		XorStr("Contributor"),
		XorStr("Urban Tree"),
		XorStr("Speech Info"),
		XorStr("WildFire Gold Detail"),
		XorStr("Vanguard Gold Detail"),
		XorStr("Gold Dust"),
		XorStr("Phoenix Gold"),
		XorStr("Payback Gold"),
		XorStr("Sticker Backing")
	};
	static const char *playersChams[] = {
		XorStr("Team"),
		XorStr("Bubble"),
		XorStr("Crystal Blue"),
		XorStr("Crytal Clear"),
		XorStr("MP3"),
		XorStr("Platinum"),
		XorStr("Gold"),
		XorStr("Dark Chrome"),
		XorStr("Plastic Glass"),
		XorStr("Velvet"),
		XorStr("Dogtags Lightray"),
		XorStr("Dogtags Outline"),
		XorStr("Dreamhack Star Blur"),
		XorStr("Gloss"),
		XorStr("Fishing Net"),
		XorStr("Contributor"),
		XorStr("Urban Tree"),
		XorStr("Speech Info"),
		XorStr("WildFire Gold Detail"),
		XorStr("Vanguard Gold Detail"),
		XorStr("Gold Dust"),
		XorStr("Phoenix Gold"),
		XorStr("Payback Gold"),
		XorStr("Sticker Backing")
	};


	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "ESP");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::PushFont(ruda16);
	ImGui::Columns(2, XorStr("##c1"), false);
	ImGui::Checkbox(XorStr("ENABLE ESP"), &MenuOptions.ESP.g_espON);
	ImGui::NextColumn();
	ImGui::Checkbox(XorStr("ESP ONLY ENEMIES"), &MenuOptions.ESP.g_ESPonlyEnemyTeam);
	ImGui::Separator();
	ImGui::Columns(2, XorStr("##c1"), false);
	ImGui::Checkbox(XorStr("Box"), &MenuOptions.ESP.g_espBox);
	ImGui::Checkbox(XorStr("Weapon"), &MenuOptions.ESP.g_espWeapon);
	ImGui::Checkbox(XorStr("Health"), &MenuOptions.ESP.g_espHealth);
	ImGui::Checkbox(XorStr("Armor"), &MenuOptions.ESP.g_espArmor);
	ImGui::Checkbox(XorStr("Bone"), &MenuOptions.ESP.g_espBone);
	ImGui::Checkbox(XorStr("Glow Dropped Weapons"), &MenuOptions.ESP.g_glowDroppedWeapons);
	ImGui::Checkbox(XorStr("Bomb"), &MenuOptions.ESP.g_bombEsp);
	ImGui::Checkbox(XorStr("Damages"), &MenuOptions.ESP.g_damageIndicator);
	ImGui::Checkbox(XorStr("Backtrack"), &MenuOptions.ESP.g_backtrackEsp);
	ImGui::NextColumn();
	ImGui::Checkbox(XorStr("Name"), &MenuOptions.ESP.g_espName);
	ImGui::Checkbox(XorStr("Head"), &MenuOptions.ESP.g_espHead);
	ImGui::Checkbox(XorStr("Dist"), &MenuOptions.ESP.g_espDist);
	ImGui::Checkbox(XorStr("Dropped Weapons"), &MenuOptions.ESP.g_espDroppedWeapons);
	ImGui::Checkbox(XorStr("Grenades"), &MenuOptions.ESP.g_GrenadesEsp);
	ImGui::Checkbox(XorStr("Glow Grenades"), &MenuOptions.ESP.g_glowGrenades);
	ImGui::Checkbox(XorStr("Glow Bomb"), &MenuOptions.ESP.g_glowBomb);
	ImGui::Checkbox(XorStr("LBY Update"), &MenuOptions.ESP.g_lbyUpdate);
	//ImGui::Checkbox(XorStr("Scoped"), &MenuOptions.ESP.g_scoped);
	ImGui::PopFont();
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "CHAMS | GLOW");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::PushFont(ruda16);
	ImGui::Checkbox(XorStr("CHAMS / GLOW ONLY ENEMIES"), &MenuOptions.ESP.g_ChamsGlowOnlyEnemyTeam);
	ImGui::Separator();
	ImGui::Columns(4, XorStr("##c1"), false);
	ImGui::RadioButton("NONE", &MenuOptions.ESP.g_chamsOrGlow, 3);
	ImGui::NextColumn();
	ImGui::RadioButton("CHAMS | NO WH", &MenuOptions.ESP.g_chamsOrGlow, 0);
	ImGui::NextColumn();
	ImGui::RadioButton("Chams | WH", &MenuOptions.ESP.g_chamsOrGlow, 1);
	ImGui::NextColumn();
	ImGui::RadioButton("GLOW", &MenuOptions.ESP.g_chamsOrGlow, 2);
	ImGui::Columns(1, XorStr("##c1"), false);
	ImGui::Checkbox(XorStr("Dead bodies Chams"), &MenuOptions.ESP.g_deadbodiesChams);
	ImGui::Columns(3, XorStr("##chamsmodes"), false);
	ImGui::Combo(XorStr("Weapon Chams"), &MenuOptions.ESP.g_weaponsChams, chams, ARRAYSIZE(chams));
	ImGui::NextColumn();
	ImGui::Combo(XorStr("Arms Chams"), &MenuOptions.ESP.g_armsChams, chams, ARRAYSIZE(chams));
	ImGui::NextColumn();
	ImGui::Combo(XorStr("Players Chams"), &MenuOptions.ESP.g_playerChams, playersChams, ARRAYSIZE(playersChams));
	ImGui::PopFont();	
}

void DRAWAIMBOT()
{
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "AIMBOT");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::Columns(3, XorStr("##c1"), false);
	ImGui::PushFont(ruda16);
	ImGui::RadioButton("No Aimbot", &MenuOptions.Aimbot.legitOrRage, 2);
	ImGui::NextColumn();
	ImGui::RadioButton("Legit", &MenuOptions.Aimbot.legitOrRage, 0);
	ImGui::NextColumn();
	ImGui::RadioButton("Rage", &MenuOptions.Aimbot.legitOrRage, 1);
	ImGui::PopFont();
	ImGui::Columns(1);

	//NOAIMBOT
	if (MenuOptions.Aimbot.legitOrRage == 2)
	{
		ImGui::Separator();
		ImGui::Columns(2, XorStr("##c1"), false);
		ImGui::PushFont(ruda16);
		ImGui::Text(XorStr("RCS"));
		ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Aimbot.g_rcs);
		ImGui::NextColumn();
		ImGui::Text(XorStr("Triggerbot"));
		ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Aimbot.g_triggerbotON);
		ImGui::PopFont();
	}
	else if (MenuOptions.Aimbot.legitOrRage == 0)//LEGIT
	{
		ImGui::Separator();
		ImGui::Columns(2, XorStr("##c1"), false);
		ImGui::PushFont(ruda16);
		ImGui::SliderFloat("FOV", &MenuOptions.Aimbot.g_fovValue, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Aimbot.g_smoothValue, 1.f, 50.f, "%.1f");
		ImGui::Text(XorStr("Triggerbot"));
		ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Aimbot.g_triggerbotON);
		ImGui::NextColumn();
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Aimbot.g_hitboxPrioritize, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
		ImGui::Checkbox(XorStr("Mouse Autoshoot"), &MenuOptions.Aimbot.g_mouseAuto);
		if (MenuOptions.Aimbot.g_mouseAuto)
		{
			ImGui::SliderFloat("FOV Mouse Autoshoot", &MenuOptions.Aimbot.g_fovMouseAuto, 1.f, 360.f, "%.1f");
		}
		ImGui::Checkbox(XorStr("RCS"), &MenuOptions.Aimbot.g_rcs);
		ImGui::Columns(1);
		ImGui::PopFont();
	}

	else if (MenuOptions.Aimbot.legitOrRage == 1)//RAGE
	{
		static const char *aAntiAims[] = {
			XorStr("NONE"),
			XorStr("Jitter LBY Basic"),
			XorStr("Jitter LBY Reversed Basic"),
			XorStr("Back Jitter LBY Basic"),
			XorStr("Spin LBY Basic"),
			XorStr("Backward Basic"), 
			XorStr("Side Unhittable"),
			XorStr("FJitter2")
			//XorStr("Jitter Unhittable"),
			//XorStr("Back Jitter Unhittable"), 
			//XorStr("Moving unhittable"),
			//XorStr("Edge unhittable")
		};

		static const char *aAntiAimsFakeUnhittable[] = {
			XorStr("Anti Bad Bruteforce"),
			XorStr("Basic inverse"),
			XorStr("Random 30-140"),
			XorStr("Jitter"),
			XorStr("Basic 160"),
			XorStr("Basic 140-160"),
			XorStr("Basic 135"),
			XorStr("Basic 70")
		};

		ImGui::Separator();
		ImGui::Columns(2, XorStr("##c1"), false);
		ImGui::PushFont(ruda16);
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Aimbot.g_autoHitbox);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Hitscan without priority");
		if (!MenuOptions.Aimbot.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Aimbot.g_hitboxPrioritize, aimBonesRage, ARRAYSIZE(aimBonesRage));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Hitscan to prioritize");
		}
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Aimbot.g_hitChance, 0, 100);
		ImGui::SliderFloat(XorStr("Hitchance Seed"), &MenuOptions.Aimbot.g_hitchanceSeed, 0.0f, 256.f, "%.1f");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Nb. of points to be scanned");
		ImGui::SliderInt(XorStr("Static Fakelag"), &MenuOptions.Aimbot.g_fakeLagAmount, 1, 100);
		ImGui::SliderInt(XorStr("Moving Fakelag"), &MenuOptions.Aimbot.g_movingFakeLag, 1, 100);
		ImGui::SliderInt(XorStr("Jumping Fakelag"), &MenuOptions.Aimbot.g_jumpingFakeLag, 1, 100);
		ImGui::Checkbox(XorStr("Autoshoot"), &MenuOptions.Aimbot.g_autoshootON);
		ImGui::Checkbox(XorStr("Auto Revolver | ALPHA"), &MenuOptions.Aimbot.g_autoRevolver);
		if (MenuOptions.Aimbot.g_autoWall)
		{
			ImGui::Checkbox(XorStr("Backtracking"), &MenuOptions.Aimbot.g_checkBacktracked);
		}
		ImGui::NextColumn();
		ImGui::SliderFloat(XorStr("Head Point Scale"), &MenuOptions.Aimbot.g_headPointScale, 0.f, 1.f, "%.3f");
		ImGui::SliderFloat(XorStr("Arms Point Scale"), &MenuOptions.Aimbot.g_armsPointScale, 0.f, 1.f, "%.3f");
		ImGui::Combo(XorStr("Resolver"), &MenuOptions.Aimbot.g_resolver, resolverOptions, ARRAYSIZE(resolverOptions));
		ImGui::Checkbox(XorStr("Silentaim"), &MenuOptions.Aimbot.g_silentaim);
		ImGui::Checkbox(XorStr("No visual recoil"), &MenuOptions.Aimbot.g_novisrecoil);
		ImGui::PopFont();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PushFont(ruda22);
		ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "ANTIAIM");
		ImGui::PopFont();
		ImGui::Separator();
		ImGui::Columns(2, XorStr("##c1"), false);
		ImGui::PushFont(ruda16);
		if (MenuOptions.Aimbot.g_realYawAA == 6)
		{
			ImGui::Checkbox(XorStr("LBY Indicator"), &MenuOptions.Misc.g_lbyIndicator);
			ImGui::Checkbox(XorStr("Best Side"), &MenuOptions.Aimbot.g_unhittableBestSide);
		}
		ImGui::Combo(XorStr("AA - PITCH"), &MenuOptions.Aimbot.g_pitchAA, pitchAA, ARRAYSIZE(pitchAA));
		ImGui::Combo(XorStr("AA - STAT. REAL YAW"), &MenuOptions.Aimbot.g_realYawAA, aAntiAims, ARRAYSIZE(aAntiAims));
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Static Real Yaw (When Not moving)");
		if (MenuOptions.Aimbot.g_realYawAA == 6)
		{
			ImGui::Combo(XorStr("AA - FAKE UNHITTABLE YAW"), &MenuOptions.Aimbot.g_fakeUnhittable, aAntiAimsFakeUnhittable, ARRAYSIZE(aAntiAimsFakeUnhittable));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Hidden yaw with unhittable AA");
		}
		ImGui::Combo(XorStr("AA - FAKE YAW"), &MenuOptions.Aimbot.g_fakeYawAA, aAntiAims, ARRAYSIZE(aAntiAims));
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Visible YAW without resolver");
		ImGui::SliderFloat(XorStr("Spin Speed"), &MenuOptions.Aimbot.g_spinSpeed, 0.001f, 1.0f, "%.3f");
		ImGui::NextColumn();
		ImGui::Combo(XorStr("AA - MOV. REAL YAW"), &MenuOptions.Aimbot.g_movingRealYawAA, aAntiAims, ARRAYSIZE(aAntiAims));
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Moving Real Yaw (When Moving)");
		ImGui::Combo(XorStr("AA - MOVING YAW"), &MenuOptions.Aimbot.g_movingYawAA, aAntiAims, ARRAYSIZE(aAntiAims));
		ImGui::Combo(XorStr("AA - JUMPING YAW"), &MenuOptions.Aimbot.g_jumpingYawAA, aAntiAims, ARRAYSIZE(aAntiAims));
		ImGui::Checkbox(XorStr("Nade AA"), &MenuOptions.Aimbot.bNadeAA);
		ImGui::PopFont();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PushFont(ruda22);
		ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "AUTOWALL");
		ImGui::PopFont();
		ImGui::Separator();
		ImGui::Columns(2, XorStr("##c1"), false);
		ImGui::PushFont(ruda16);
		//ImGui::Checkbox(XorStr("Autowall"), &MenuOptions.Aimbot.g_autoWall);
		if (MenuOptions.Aimbot.g_autoWall)
		{
			//ImGui::Combo(XorStr("Autowall type"), &MenuOptions.Aimbot.iAutoWallType, aAutoWallType, ARRAYSIZE(aAutoWallType));
			ImGui::SliderInt(XorStr("Min. Damage"), &MenuOptions.Aimbot.g_minDamage, 0, 100);
			if (!MenuOptions.Aimbot.g_autoHitbox)
			{
				ImGui::SliderInt(XorStr("Min. Damage P."), &MenuOptions.Aimbot.g_minDamagePrio, 0, 100);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Min. Damage before trying to hit prioritized hitbox.");
			}
			ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Aimbot.g_adaptiveMinDmg);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

			if (MenuOptions.Aimbot.g_adaptiveMinDmg)
			{
				ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Aimbot.g_minDmgAdapt, 0, 100);
				ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Aimbot.g_minDmgPeeking, 0, 100);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

				ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Aimbot.g_hitchanceAdapt, 0, 100);
			}
		}
		ImGui::NextColumn();
		ImGui::Checkbox(XorStr("General AutoBaim"), &MenuOptions.Aimbot.g_autoBaim);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Autobaim for every weapons");

		if (MenuOptions.Aimbot.g_autoBaim)
		{
			ImGui::Combo(XorStr("Hitbox Baim"), &MenuOptions.Aimbot.g_hitboxBaim, aimBonesRage, ARRAYSIZE(aimBonesRage));
			ImGui::Checkbox(XorStr("Shots Fired Baim"), &MenuOptions.Aimbot.g_shotsAutoBaim);
			if (MenuOptions.Aimbot.g_shotsAutoBaim)
			{
				ImGui::InputInt("Min. Shots", &MenuOptions.Aimbot.g_shotsFiredMin);
			}
			ImGui::Checkbox(XorStr("Health Baim"), &MenuOptions.Aimbot.g_healthAutoBaim);
			if (MenuOptions.Aimbot.g_healthAutoBaim)
			{
				ImGui::SliderInt("Baim at X health: ", &MenuOptions.Aimbot.g_baimMinHealth, 0, 100);
			}
		}
		ImGui::PopFont();
	}
}

void DRAWMISC()
{
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "MISC");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::Columns(2, XorStr("##c1"), false);
	ImGui::PushFont(ruda16);
	ImGui::Checkbox(XorStr("Bunnyhop"), &MenuOptions.Misc.g_bhopON);
	ImGui::Checkbox(XorStr("Watermark"), &MenuOptions.Misc.g_watermark);
	ImGui::Checkbox(XorStr("Wireframe Arms"), &MenuOptions.Misc.g_wireframeArms);
	ImGui::Checkbox(XorStr("NoHands"), &MenuOptions.Misc.g_nohands);
	ImGui::Checkbox(XorStr("NoFlash"), &MenuOptions.Misc.g_noflash);
	ImGui::Checkbox(XorStr("NoSmoke"), &MenuOptions.Misc.g_nosmoke);
	ImGui::Checkbox(XorStr("Radar"), &MenuOptions.Misc.g_radar);
	ImGui::Checkbox(XorStr("Crosshair"), &MenuOptions.Misc.g_crosshair);
	ImGui::Checkbox(XorStr("AutoZeus"), &MenuOptions.Misc.g_autozeus);
	ImGui::Checkbox(XorStr("Autocrouch"), &MenuOptions.Misc.g_autocrouch);
	ImGui::Checkbox(XorStr("Autoscope"), &MenuOptions.Misc.g_autoscope);
	ImGui::Checkbox(XorStr("Autostop"), &MenuOptions.Misc.g_autostop);
	ImGui::Checkbox(XorStr("Autostrafe"), &MenuOptions.Misc.g_autostrafe);
	ImGui::Checkbox(XorStr("Circlestrafe"), &MenuOptions.Misc.g_circleStrafe);
	ImGui::Checkbox(XorStr("Thirdperson"), &MenuOptions.Misc.g_thirdperson);
	if (ImGui::Button("Night mode"))
		MiscExt.SetNightMode();
	ImGui::SliderFloat(XorStr("FOV"), &MenuOptions.Misc.g_fovPlayer, 60.0f, 120.f, "%.0f");
	ImGui::Checkbox(XorStr("Post Process"), &MenuOptions.Misc.g_postprocess);
	ImGui::Checkbox(XorStr("No Scope"), &MenuOptions.Misc.g_noscope);
	ImGui::Checkbox(XorStr("Name Spam"), &MenuOptions.Misc.g_namespam);
	ImGui::Checkbox(XorStr("Spectators"), &MenuOptions.Misc.g_spec);
	ImGui::PopFont();
	ImGui::NextColumn();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "Sonar");
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::Checkbox(XorStr("Enabled##SonarON"), &MenuOptions.Misc.g_sonarON);
	ImGui::SliderFloat(XorStr("Distance"), &MenuOptions.Misc.g_sonarDist, 800.f, 1000.f, "%.0f");
	ImGui::SliderFloat(XorStr("Sound HZ"), &MenuOptions.Misc.g_sonarHZ, 500.f, 1000.f, "%.0f");
	ImGui::PopFont();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "Spam");
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::Checkbox(XorStr("Enabled##SpamON"), &MenuOptions.Misc.g_spamchat);
	ImGui::RadioButton(XorStr("Rekt##SpamRekt"), &MenuOptions.Misc.g_spamtype, 0); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Jokes##JokesRekt"), &MenuOptions.Misc.g_spamtype, 1); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Flame##FlameRekt"), &MenuOptions.Misc.g_spamtype, 2);
	ImGui::SliderFloat(XorStr("Spam Speed"), &MenuOptions.Misc.g_spamspeed, 0.1f, 10.0f, "%.1f");
	ImGui::PopFont();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "Chatbot");
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::Checkbox(XorStr("Enabled##Chatbot"), &MenuOptions.Misc.g_chatbot);
	ImGui::Text("GENERAL:");
	ImGui::Checkbox(XorStr("Enabled##GeneralChatbot"), &MenuOptions.Misc.g_generalChatbot);
	ImGui::Text("KILL:");
	ImGui::RadioButton(XorStr("NONE##ChatbotKillNone"), &MenuOptions.Misc.g_chatbotKillMode, 0); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Rekt##ChatbotKillRekt"), &MenuOptions.Misc.g_chatbotKillMode, 1); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Joke##ChatbotKillJoke"), &MenuOptions.Misc.g_chatbotKillMode, 2); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Flame##ChatbotKillFlame"), &MenuOptions.Misc.g_chatbotKillMode, 3);
	ImGui::Text("DEAD:");
	ImGui::RadioButton(XorStr("NONE##ChatbotDeadNone"), &MenuOptions.Misc.g_chatbotDeadMode, 0); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Joke##ChatbotDeadJoke"), &MenuOptions.Misc.g_chatbotDeadMode, 1); ImGui::SameLine();
	ImGui::RadioButton(XorStr("Flame##ChatbotDeadFlame"), &MenuOptions.Misc.g_chatbotDeadMode, 2);
	ImGui::Checkbox(XorStr("Airstuck |  F2"), &MenuOptions.Misc.g_airstuckON);
	ImGui::Checkbox(XorStr("Hitmarker Sound"), &MenuOptions.Misc.g_hitmarkerSound);
	ImGui::Checkbox(XorStr("Hitmarker Crosshair"), &MenuOptions.Misc.g_hitmarkerCrosshair);
	ImGui::PopFont();
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::PushItemWidth(110);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "CUSTOM CLANTAG / NAME");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::Columns(2, XorStr("##c1"), false);
	ImGui::PushFont(ruda16);
	ImGui::Text(XorStr("CLANTAG"));
	static char clantag[128] = "";
	ImGui::PushItemWidth(100);
	ImGui::InputText(XorStr("##CLANTag"), clantag, 128);
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Change Tag"), ImVec2(110, 30))) Hookers::SetClanTag(clantag, clantag);

	ImGui::Text(XorStr("NAME"));
	static char name[128] = "";
	ImGui::InputText(XorStr("##NAME"), name, 128);
	if (ImGui::Button(XorStr("Change Name"), ImVec2(110, 30)))
	{
		if (name != "")
			Hookers::NET_SetConVar(name, "name");
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Spam Name"), ImVec2(110, 30)))
	{
		if (name != "")
		{
			MenuOptions.Menu.nameToSpam = name;

			MenuOptions.Misc.g_namespam = true;
		}
	}
	ImGui::NextColumn();
	ImGui::Text(XorStr("PREMADE TAGS"));

	if (ImGui::Button(XorStr("Valve"), ImVec2(80, 30))) Hookers::SetClanTag("[VALV\xE1\xB4\xB1]", "Valve");
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Hide Name"), ImVec2(80, 30))) Hookers::SetClanTag("\n HideMe", "HideMe");
	if (ImGui::Button(XorStr("AIMWARE"), ImVec2(80, 30))) Hookers::SetClanTag("AIMWARE", "AIMWARE");
	ImGui::SameLine();
	if (ImGui::Button(XorStr("P4R4H00K"), ImVec2(80, 30)))
	{
		MenuOptions.Misc.g_animeClanTag = !MenuOptions.Misc.g_animeClanTag;
	}
	ImGui::SameLine();
	if (MenuOptions.Misc.g_animeClanTag)
		ImGui::TextColored(ImVec4(0.f, 1.0f, 0.0f, 1.0f), "P4R4H00K: ON");
	else
		ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.0f), "P4R4H00K: OFF");
	ImGui::SliderFloat(XorStr("Clantag Speed"), &MenuOptions.Misc.g_clantagspeed, 0.1f, 10.0f, "%.1f");

	ImGui::PopFont();
}

void DRAWTARGETS()
{
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "TARGETS");
	ImGui::PopFont();
	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(1);

	if (SSDK::I::Engine()->isInGame())
	{
		C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalPlayer();

		for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i)
		{
			C_CSPlayer *pClient = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);

			if (pClient)
			{

				if (pLocalPlayer->GetTeamNum() != pClient->GetTeamNum()) {
					{
						SSDK::PlayerInfo entityInfos;
						SSDK::I::Engine()->GetPlayerInfo(i, &entityInfos);

						ImGui::AlignFirstTextHeightToWidgets();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.

						if (ImGui::TreeNode(entityInfos.szName))
						{
							ImGui::PushFont(ruda16);
							if (MenuOptions.Aimbot.legitOrRage == 1)
							{
								ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "AIMBOT OPTIONS");
								ImGui::Checkbox(XorStr("HITBOX OPTIONS"), &MenuOptions.Menu.TargetList[i].customHitbox);
								if (MenuOptions.Menu.TargetList[i].customHitbox)
								{
									ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Menu.TargetList[i].autoHitbox);
									if (!MenuOptions.Menu.TargetList[i].autoHitbox)
									{
										ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Menu.TargetList[i].hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
									}

								}

								ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "CUSTOM RESOLVER OPTIONS");
								ImGui::Checkbox(XorStr("RESOLVER OPTIONS"), &MenuOptions.Menu.TargetList[i].customResolve);
								if (MenuOptions.Menu.TargetList[i].customResolve)
								{
									ImGui::Combo(XorStr("Resolver"), &MenuOptions.Menu.TargetList[i].resolver, resolverOptions, ARRAYSIZE(resolverOptions));
								}

								ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "CUSTOM RAGE OPTIONS");
								ImGui::Checkbox(XorStr("CUSTOM HITCHANCE"), &MenuOptions.Menu.TargetList[i].customHitchance);

								if (MenuOptions.Menu.TargetList[i].customHitchance)
								{
									ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Menu.TargetList[i].g_hitChance, 0, 100);
								}

								ImGui::Checkbox(XorStr("AUTOWALL OPTIONS"), &MenuOptions.Menu.TargetList[i].customAutowall);

								if (MenuOptions.Aimbot.g_autoWall && MenuOptions.Menu.TargetList[i].customAutowall)
								{
									ImGui::Checkbox(XorStr("Auto Baim"), &MenuOptions.Menu.TargetList[i].baim);
									ImGui::Checkbox(XorStr("Min Damage"), &MenuOptions.Menu.TargetList[i].customMinDamage);
									if (MenuOptions.Menu.TargetList[i].customMinDamage)
									{
										ImGui::SliderInt(XorStr("Min. Damage"), &MenuOptions.Menu.TargetList[i].minDamage, 0, 100);
										if (!MenuOptions.Menu.TargetList[i].autoHitbox)
										{
											ImGui::SliderInt(XorStr("Min. Damage Prio"), &MenuOptions.Menu.TargetList[i].minDamagePrio, 0, 100);
										}
									}

									if (MenuOptions.Menu.TargetList[i].baim)
									{
										ImGui::Combo(XorStr("Hitbox Baim"), &MenuOptions.Menu.TargetList[i].hitboxbaim, aimBonesRage, ARRAYSIZE(aimBonesRage));
										
										ImGui::Checkbox(XorStr("Shots Fired Baim"), &MenuOptions.Menu.TargetList[i].shotsbaim);
										if (MenuOptions.Menu.TargetList[i].shotsbaim)
										{
											ImGui::InputInt(XorStr("Shots Fired Min."), &MenuOptions.Menu.TargetList[i].shotsfiredmin);
										}
										ImGui::Checkbox(XorStr("Health Baim"), &MenuOptions.Menu.TargetList[i].healthbaim);
										if (MenuOptions.Menu.TargetList[i].healthbaim)
										{
											ImGui::SliderInt(XorStr("Baim at X health: "), &MenuOptions.Menu.TargetList[i].g_baimMinHealth, 0, 100);
										}
									}
								}
							}

							ImGui::PopFont();
							ImGui::TreePop();
						}
					}
				}
			}
		}
	}

	ImGui::Separator();
	ImGui::PopStyleVar();
}

void DRAWWEAPONS()
{
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "WEAPONS");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::Columns(2, XorStr("##c1"), false);
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("Duals"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#Duals", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.Duals.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.Duals.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.Duals.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.Duals.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.Duals.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.Duals.g_autoHitbox);
		if (!MenuOptions.Weapons.Duals.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Duals.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.Duals.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.Duals.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.Duals.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.Duals.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.Duals.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.Duals.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.Duals.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Duals.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::PopFont();
	//-------------------------------
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("Zeus"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#Zeus", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.Zeus.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.Zeus.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.Zeus.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.Zeus.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.Zeus.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.Zeus.g_autoHitbox);
		if (!MenuOptions.Weapons.Zeus.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Zeus.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.Zeus.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.Zeus.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.Zeus.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.Zeus.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.Zeus.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.Zeus.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.Zeus.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Zeus.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::PopFont();
	//-------------------------------
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("Deagle"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#Deagle", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.Deagle.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.Deagle.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.Deagle.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.Deagle.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.Deagle.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.Deagle.g_autoHitbox);
		if (!MenuOptions.Weapons.Deagle.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Deagle.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.Deagle.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.Deagle.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.Deagle.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.Deagle.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.Deagle.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.Deagle.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.Deagle.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Deagle.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::PopFont();
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("Autosnipers"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#Autosnipers", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.Autosnipers.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.Autosnipers.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.Autosnipers.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.Autosnipers.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.Autosnipers.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.Autosnipers.g_autoHitbox);
		if (!MenuOptions.Weapons.Autosnipers.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Autosnipers.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}
		ImGui::Checkbox(XorStr("AutoBaim"), &MenuOptions.Weapons.Autosnipers.g_autoBaim);
		if (MenuOptions.Weapons.Autosnipers.g_autoBaim)
		{
			ImGui::Combo(XorStr("Hitbox Baim"), &MenuOptions.Weapons.Autosnipers.g_hitboxBaim, aimBonesRage, ARRAYSIZE(aimBonesRage));
	
			ImGui::Checkbox(XorStr("Shots Fired Baim"), &MenuOptions.Weapons.Autosnipers.g_shotsBaim);
			if (MenuOptions.Weapons.Autosnipers.g_shotsBaim)
			{
				ImGui::InputInt("Min. Shots", &MenuOptions.Weapons.Autosnipers.g_shotsFiredMin);
			}
			ImGui::Checkbox(XorStr("Health Baim"), &MenuOptions.Weapons.Autosnipers.g_healthBaim);
			if (MenuOptions.Weapons.Autosnipers.g_healthBaim)
			{
				ImGui::SliderInt(XorStr("Baim at X health: "), &MenuOptions.Weapons.Autosnipers.g_baimMinHealth, 0, 100);
			}
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.Autosnipers.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.Autosnipers.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.Autosnipers.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.Autosnipers.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.Autosnipers.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.Autosnipers.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.Autosnipers.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Autosnipers.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::PopFont();
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("SSG"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#SSG", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.SSG08.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.SSG08.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.SSG08.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.SSG08.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.SSG08.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.SSG08.g_autoHitbox);
		if (!MenuOptions.Weapons.SSG08.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.SSG08.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.SSG08.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.SSG08.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.SSG08.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.SSG08.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.SSG08.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.SSG08.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.SSG08.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.SSG08.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::PopFont();
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("Revolver"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#Revolver", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.Revolver.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.Revolver.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.Revolver.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.Revolver.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.Revolver.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.Revolver.g_autoHitbox);
		if (!MenuOptions.Weapons.Revolver.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Revolver.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.Revolver.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.Revolver.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.Revolver.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.Revolver.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.Revolver.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.Revolver.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.Revolver.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.Revolver.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::PopFont();
	ImGui::PushFont(ruda22);
	ImGui::Text(XorStr("AWP"));
	ImGui::PopFont();
	ImGui::PushFont(ruda16);
	ImGui::BeginChild("#AWP", ImVec2(0, 180), true);
	ImGui::Checkbox(XorStr("Enabled"), &MenuOptions.Weapons.AWP.g_enabled);
	if (MenuOptions.Aimbot.legitOrRage == 1)
	{
		ImGui::Text(XorStr("Rage"));
		ImGui::SliderInt(XorStr("Hitchance"), &MenuOptions.Weapons.AWP.g_hitChance, 0, 100);
		ImGui::SliderInt(XorStr("Min. Dmg"), &MenuOptions.Weapons.AWP.g_minDamage, 0, 100);
		if (!MenuOptions.Weapons.AWP.g_autoHitbox)
		{
			ImGui::SliderInt(XorStr("Min. Dmg P."), &MenuOptions.Weapons.AWP.g_minDamagePrio, 0, 100);
		}
		ImGui::Checkbox(XorStr("Auto Hitbox"), &MenuOptions.Weapons.AWP.g_autoHitbox);
		if (!MenuOptions.Weapons.AWP.g_autoHitbox)
		{
			ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.AWP.g_hitbox, aimBonesRage, ARRAYSIZE(aimBonesRage));
		}

		ImGui::Checkbox(XorStr("Adaptive / moving"), &MenuOptions.Weapons.AWP.g_adaptiveMinDmg);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adaptive hitchance and min damage when you move.");

		if (MenuOptions.Weapons.AWP.g_adaptiveMinDmg)
		{
			ImGui::SliderInt(XorStr("Adaptive Min. Damage"), &MenuOptions.Weapons.AWP.g_minDmgAdapt, 0, 100);
			ImGui::SliderInt(XorStr("Dmg / Peeking Guy"), &MenuOptions.Weapons.AWP.g_minDmgPeeking, 0, 100);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Min. Dmg when someone peeks you without ducking.");

			ImGui::SliderInt(XorStr("Adaptive Hitchance"), &MenuOptions.Weapons.AWP.g_hitchanceAdapt, 0, 100);
		}
	}
	if (MenuOptions.Aimbot.legitOrRage == 0 || MenuOptions.Aimbot.legitOrRage == 2)
	{
		ImGui::Text(XorStr("Legit"));
		ImGui::SliderFloat("FOV", &MenuOptions.Weapons.AWP.g_fov, 1.f, 360.f, "%.1f");
		ImGui::SliderFloat("SMOOTH", &MenuOptions.Weapons.AWP.g_smooth, 1.f, 30.f, "%.1f");
		ImGui::Combo(XorStr("Hitbox"), &MenuOptions.Weapons.AWP.g_hitbox, aimBonesLegit, ARRAYSIZE(aimBonesLegit));
	}
	ImGui::EndChild();
	ImGui::Columns(1);
	ImGui::PopFont();
}

void DRAWSKINS() {
	ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.0f), "You have to be ingame and take the gun you want in hands ! Save and wait to die/rebuy weapon to see change");
	if (SSDK::I::Engine()->isInGame()) {
		static const char* aKnifes[] =
		{
			"No Knife",
			"Bayonet",
			"Flip Knife",
			"Gut Knife",
			"Karambit",
			"M9 Bayonet",
			"Huntsman Knife",
			"Falchion Knife",
			"Bowie Knife",
			"Butterfly Knife",
			"Shadow Daggers"
		};

		ImGui::Combo("Knife", &MenuOptions.Skins.iSelectedKnife, aKnifes, ARRAYSIZE(aKnifes));
		ImGui::InputInt(XorStr("Knife PaintKit"), &MenuOptions.Skins.iCurPaintKit);

		CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)C_CSPlayer::GetLocalPlayer()->GetActiveWeapon();;

		// Get the weapons item definition index.
		int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();


		if (!pWeapon)
			return;

		if (pWeapon) {
			// Get the weapons item definition index.
			int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();

			if (nWeaponIndex == 1)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.deagle);
			}
			if (nWeaponIndex == 7)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.ak47);
			}
			if (nWeaponIndex == 8)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.aug);
			}
			if (nWeaponIndex == 9)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.awp);
			}
			if (nWeaponIndex == 26)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.bizon);
			}
			if (nWeaponIndex == 63)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.cz75);
			}
			if (nWeaponIndex == 2)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.elites);
			}
			if (nWeaponIndex == 10)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.famas);
			}
			if (nWeaponIndex == 3)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.fiveseven);
			}
			if (nWeaponIndex == 11)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.g3sg1);
			}
			if (nWeaponIndex == 13)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.galil);
			}
			if (nWeaponIndex == 4)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.glock);
			}
			if (nWeaponIndex == 14)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.m249);
			}
			if (nWeaponIndex == 60)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.m4a1s);
			}
			if (nWeaponIndex == 16)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.m4a4);
			}
			if (nWeaponIndex == 17)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.mac10);
			}
			if (nWeaponIndex == 27)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.mag7);
			}
			if (nWeaponIndex == 64)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.magnum);
			}
			if (nWeaponIndex == 33)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.mp7);
			}
			if (nWeaponIndex == 34)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.mp9);
			}
			if (nWeaponIndex == 28)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.negev);
			}
			if (nWeaponIndex == 35)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.nova);
			}
			if (nWeaponIndex == 32)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.p2000);
			}
			if (nWeaponIndex == 61)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.usps);
			}
			if (nWeaponIndex == 19)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.p90);
			}
			if (nWeaponIndex == 36)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.p250);
			}
			if (nWeaponIndex == 29)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.sawedoff);
			}
			if (nWeaponIndex == 38)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.scar20);
			}
			if (nWeaponIndex == 39)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.sg556);
			}
			if (nWeaponIndex == 40)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.ssg08);
			}
			if (nWeaponIndex == 30)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.tec9);
			}
			if (nWeaponIndex == 24)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.ump45);
			}
			if (nWeaponIndex == 25)
			{
				ImGui::InputInt(XorStr("PaintKit"), &MenuOptions.Skins.xm1014);
			}
		}
	}

	if (ImGui::Button("SAVE SKINS", ImVec2(90, 30))) Config->Save(1);
}

void DRAWKEYS() {
	ImGui::Separator();
	ImGui::PushFont(ruda22);
	ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "KEYBINDS");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::Columns(3, XorStr("##c1"), false);
	ImGui::InputInt(XorStr("Change AA Side"), &MenuOptions.Keys.g_aaside);
	ImGui::NextColumn();
	ImGui::InputInt(XorStr("Triggerbot"), &MenuOptions.Keys.g_triggerbot);
	ImGui::NextColumn();
	ImGui::InputInt(XorStr("Mouse Autoshoot (Legit)"), &MenuOptions.Keys.g_mouseAutoshoot);
	ImGui::NextColumn();
	ImGui::InputInt(XorStr("Airstuck"), &MenuOptions.Keys.g_airstuck);
	ImGui::NextColumn();
	ImGui::InputInt(XorStr("Circle Strafer"), &MenuOptions.Keys.g_circleStrafe);
	ImGui::NextColumn();
	ImGui::InputInt(XorStr("Thirdperson"), &MenuOptions.Keys.g_thirdperson);
	ImGui::NextColumn();
	ImGui::InputInt(XorStr("Disable AA"), &MenuOptions.Keys.g_disableAA);
}

void DRAWSPECS()
{
	int specs = 0;
	int modes = 0;
	std::string spect = "";
	std::string mode = "";
	if (SSDK::I::Engine()->isInGame() && SSDK::I::Engine()->isConnected())
	{
		C_CSPlayer* pLocalEntity = C_CSPlayer::GetLocalPlayer();
		if (pLocalEntity)
		{
			for (int i = 0; i < SSDK::I::Engine()->GetMaxClients(); i++)
			{
				C_CSPlayer* pBaseEntity = (C_CSPlayer*)SSDK::I::EntityList()->GetClientEntity(i);
				if (!pBaseEntity)
					continue;
				if (pBaseEntity->GetHealth() > 0)
					continue;
				if (pBaseEntity == pLocalEntity)
					continue;
				if (pBaseEntity->IsDormant())
					continue;
				if (pBaseEntity->GetObserved() != pLocalEntity)
					continue;

				SSDK::PlayerInfo::player_info_t pInfo;
				
				SSDK::I::Engine()->GetPlayerInfo_t(pBaseEntity->EntIndex(), &pInfo);
				if (pInfo.ishltv)
					continue;

				spect += pInfo.name;
				spect += "\n";
				specs++;

				switch (pBaseEntity->GetObserverMode())
				{
				case SSDK::ObserverMode_t::OBS_MODE_IN_EYE:
					mode += "Perspective";
					break;
				case SSDK::ObserverMode_t::OBS_MODE_CHASE:
					mode += "3rd Person";
					break;
				case SSDK::ObserverMode_t::OBS_MODE_ROAMING:
					mode += "No Clip";
					break;
				case SSDK::ObserverMode_t::OBS_MODE_DEATHCAM:
					mode += "Deathcam";
					break;
				case SSDK::ObserverMode_t::OBS_MODE_FREEZECAM:
					mode += "Freezecam";
					break;
				case SSDK::ObserverMode_t::OBS_MODE_FIXED:
					mode += "Fixed";
					break;
				default:
					break;
				}

				mode += "\n";
				modes++;
			}
		}
	}

	//0.4F
	if (ImGui::Begin(XorStr("Spectator List"), &MenuOptions.Misc.g_spec, ImVec2(0, 0), .9f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
	{
		if (specs > 0) spect += "\n";
		if (modes > 0) mode += "\n";
		ImVec2 size = ImGui::CalcTextSize(spect.c_str());
		ImGui::SetWindowSize(ImVec2(200, 60 + size.y));
		ImGui::Columns(2, XorStr("##SpecCol"), false);
		ImGui::Separator();
		ImGui::PushFont(ruda16);
		ImGui::Text("Name");
		ImGui::NextColumn();

		ImGui::Text("Mode");
		ImGui::NextColumn();
		ImGui::Separator();

		ImGui::Text(spect.c_str());
		ImGui::NextColumn();

		ImGui::Text(mode.c_str());
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopFont();
	}
	ImGui::End();
}
//Hook of EndScene, will draw the imgui menu.
HRESULT __stdcall Hookers::Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	//Check if the gui has been initialized
	if (!g_bWasInitialized)
	{
		//Init the gui
		GUI_Init(pDevice);
	}
	else
	{
		//We don't want ImGui to draw the cursor when the main window isnt open
		ImGui::GetIO().MouseDrawCursor = MenuOptions.Menu.g_bMainWindowOpen;

		//Begins a new ImGui frame.
		ImGui_ImplDX9_NewFrame();

		if(MenuOptions.Misc.g_spec)
			DRAWSPECS();

		//Check if the menu is open
		if (MenuOptions.Menu.g_bMainWindowOpen)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));

			//std::string buf("P4R4H00K-Private ");
			//buf.append(Username);
			//buf.append(" 's build");

			//std::string buf2("Version 3.0 | ");
			//buf2.append(Username);
			//buf2.append(" 's build");

			char buf[128]; // On alloue 128 char en mémoire et ensuite on formate comme je t'ai appris, on gagne au moins 5x en vitesse
			sprintf(buf, "P4R4H00K-Private %s's build", USERNAME);
			char buf2[128];
			sprintf(buf2, "Version 7.0 |  %s's build", USERNAME);

			//Draw the main menu of the cheat
			ImGui::Begin(XorStr(buf), &MenuOptions.Menu.g_bMainWindowOpen, ImVec2(800, 580), .9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			{
				ImGui::PushFont(ruda36);
				ImGui::TextColored(ImVec4(0.f, 0.4f, 0.6f, 1.0f), "P4R4H00K ");
				ImGui::SameLine();
				ImGui::Text(buf2);
				ImGui::PopFont();

				static char buttons[][16] = { "AIMBOT", "ESP", "MISC", "TARGETS", "SKINS", "WEAPONS", "KEYS"};

				//iTab return tab selected id
				static int iTab = 0;
				for (int i = 0; i < 7; i++)
				{
					if (i > 0) ImGui::SameLine(0.0f, -0.3f);

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
					ImGui::PushFont(ruda22);

					if (ImGui::Button(buttons[i], ImVec2(105, 30)))
						iTab = i;

					ImGui::PopStyleColor(1);
					ImGui::PopFont();
				}

				//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
				switch (iTab) {
				case 0:
					DRAWAIMBOT();
					break;
				case 1:
					DRAWESP();
					break;
				case 2:
					DRAWMISC();
					break;
				case 3:
					DRAWTARGETS();
					break;
				case 4:
					DRAWSKINS();
					break;
				case 5:
					DRAWWEAPONS();
					break;
				case 6:
					DRAWKEYS();
					break;
				}

			}

			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y + 2), ImGuiSetCond_Always);
			ImGui::Begin("", &MenuOptions.Menu.g_bMainWindowOpen, ImVec2(800, 30), .9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
			ImGui::PushFont(ruda16);
			ImGui::Columns(2, XorStr("##ConfigsCol"), false);
			ImGui::Combo(XorStr("##ConfigsCombo"), &MenuOptions.Menu.currentConfigID, configNames, ARRAYSIZE(configNames));
			ImGui::NextColumn();
			if (ImGui::Button("Save", ImVec2(60, 20))) {
				if (MenuOptions.Menu.currentConfigID != 0)
					Config->Save(0);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Save Config");
			ImGui::SameLine();
			if (ImGui::Button("Load", ImVec2(60, 20))) {
				if (MenuOptions.Menu.currentConfigID != 0)
					Config->Load(0);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Load Config");
			ImGui::PopFont();
			ImGui::End();

			ImGui::End();
			ImGui::PopStyleColor(1);
		}


		//Renders the GUI
		ImGui::Render();

	}

	//Call original EndScene now
	return g_fnOriginalEndScene(pDevice);
}

//It's necessary to have it if we minimize the game or alt+tab
HRESULT __stdcall Hookers::Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{

	//Correctly handling Reset calls is very important if you have a DirectX hook.
	//IDirect3DDevice9::Reset is called when you minimize the game, Alt-Tab or change resolutions.
	//When it is called, the IDirect3DDevice9 is placed on "lost" state and many related resources are released
	//This means that we need to recreate our own resources when that happens. If we dont, we crash.

	//GUI wasnt initialized yet, just call Reset and return
	if (!g_bWasInitialized) return g_fnOriginalReset(pDevice, pPresentationParameters);

	//Device is on LOST state.

	ImGui_ImplDX9_InvalidateDeviceObjects(); //Invalidate GUI resources

											 //Call original Reset.
	auto hr = g_fnOriginalReset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects(); //Recreate GUI resources
	return hr;
}

//Hook the csgo window and captures the press of buttons for the button.
LRESULT __stdcall Hookers::Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static SSDK::ConVar *conMouseEnable = SSDK::I::CVar()->FindVar(XorStr("cl_mouseenable"));
	static int realXButton;

	if (uMsg == WM_KEYUP && wParam == MenuOptions.Keys.g_thirdperson) {
		MenuOptions.Misc.g_thirdperson = !MenuOptions.Misc.g_thirdperson;
	}

	if (uMsg == WM_KEYUP && wParam == VK_INSERT) {
		//Invert the boolean
		MenuOptions.Menu.g_bMainWindowOpen = !MenuOptions.Menu.g_bMainWindowOpen;
		conMouseEnable->SetValue(!MenuOptions.Menu.g_bMainWindowOpen);

		if(!MenuOptions.Menu.g_bMainWindowOpen)
			MenuOptions.Menu.SaveLastConfig = true;
	}

	switch (MenuOptions.Keys.g_aaside)
	{
		case 5:
			realXButton = XBUTTON1;
			break;
		case 6:
			realXButton = XBUTTON2;
			break;
	}

	if (MenuOptions.Keys.g_aaside == 4)
	{
		if (uMsg == WM_MBUTTONDOWN) {
			MenuOptions.Menu.unhittable_side = !MenuOptions.Menu.unhittable_side;
		}
	}
	else if (uMsg == WM_KEYUP && wParam == MenuOptions.Keys.g_aaside)
	{
		MenuOptions.Menu.unhittable_side = !MenuOptions.Menu.unhittable_side;
	}
	else if (realXButton && (uMsg == WM_XBUTTONUP && GET_XBUTTON_WPARAM(wParam) == realXButton)) {
		MenuOptions.Menu.unhittable_side = !MenuOptions.Menu.unhittable_side;
	}


	//Processes the user input using ImGui_ImplDX9_WndProcHandler
	if (g_bWasInitialized && MenuOptions.Menu.g_bMainWindowOpen && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true; //Input was consumed, return

	//Input was not consumed by the GUI, call original WindowProc to pass the input to the game
	return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
}