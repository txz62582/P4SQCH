#include "Menu.h"

void Menu::Initialize() {
	this->flStartPosX = 300;
	this->flStartPosY = 100;
	this->flItemHeight = 20;
	this->flItemWidth = 225;
	this->iItemCount = 0;

	this->AddItem("ESP", 0, 0, 1, true);
	this->AddItem("- Box", 0, 0, 1, false);
	this->AddItem("- Health", 0, 0, 1, false);
	this->AddItem("- Bone", 0, 0, 1, false);
	this->AddItem("- Name", 0, 0, 1, false);
	this->AddItem("- Head", 0, 0, 1, false);
	this->AddItem("- Dist", 0, 0, 1, false);
	this->AddItem("- Dropped weapons", 0, 0, 1, false);
	this->AddItem("- Glow", 1, 0, 1, false); //8

	this->AddItem("AIMBOT", 1, 0, 1, true);
	this->AddItem("- LegitAim", 1, 0, 1, false);//10
	this->AddItem("- Hitbox", 6, 0, 6, false);
	this->AddItem("- FOV", 10, 0, 360, false);
	this->AddItem("- Smooth", 6, 1, 30, false);
	this->AddItem("- Silent aim", 0, 0, 1, false); // 14
	this->AddItem("- rageAIM Visible", 0, 0, 1, false);
	this->AddItem("- rageAIM Autowall", 0, 0, 1, false); //16
	this->AddItem("- Aimbot Hitchance", 50, 0, 100, false);
	this->AddItem("- Auto crouch", 1, 0, 1, false);
	this->AddItem("- Auto stop", 1, 0, 1, false);
	this->AddItem("- Auto scope", 1, 0, 1, false);
	this->AddItem("- RCS", 0, 0, 1, false); //21
	this->AddItem("- Triggerbot", 1, 0, 1, false); //22

	this->AddItem("MISC", 1, 0, 1, true);
	this->AddItem("- Auto Bhop", 1, 0, 1, false);//24
	this->AddItem("- No Hands", 0, 0, 1, false);
	this->AddItem("- Crosshair", 0, 0, 1, false);
	this->AddItem("- Watermark", 1, 0, 1, false);
	this->AddItem("- Radar", 1, 0, 1, false); 
	this->AddItem("- No flash", 0, 0, 1, false); //29
	this->AddItem("- No smoke", 0, 0, 1, false);
	this->AddItem("- Chams ON/OFF", 1, 0, 1, false);
	this->AddItem("- Rank revealer", 1, 0, 1, false);
	this->AddItem("- Airstuck", 1, 0, 1, false); //33
	this->AddItem("- Anti-Aim", 0, 0, 10, false); //34

}

void Menu::AddItem(char* pTitle, float flVal, float flMin, float flMax, bool isBigTitle) {
	this->itemsList[iItemCount].cTitle = pTitle;
	this->itemsList[iItemCount].flValue = flVal;
	this->itemsList[iItemCount].flMin = flMin;
	this->itemsList[iItemCount].flMax = flMax;
	this->itemsList[iItemCount].isBigTitle = isBigTitle;
	this->iItemCount += 1;
}

void Menu::DrawMenu() {
	for (int i = 0; i < this->iItemCount; ++i) {
		if (itemsList[i].isBigTitle) {
			DrawManager::DrawRect(SSDK::Color(0, 0, 0, 200), flStartPosX, flStartPosY + i*flItemHeight, flStartPosX + flItemWidth, flStartPosY + (i + 1)*flItemHeight);
			DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0, 150), flStartPosX, flStartPosY + i*flItemHeight, flStartPosX + flItemWidth, flStartPosY + (i + 1)*flItemHeight);
			DrawManager::DrawString(SSDK::Color(255, 255, 255, 200), itemsList[i].cTitle, flStartPosX + 5, flStartPosY + (i*flItemHeight) + 3);
			DrawManager::DrawStringValue(SSDK::Color(255, 255, 255, 200), flStartPosX + flItemWidth - 15, flStartPosY + (i*flItemHeight) + 3, ">");
		} else {
			if (i == iItemIndex) {
				DrawManager::DrawRect(SSDK::Color(150, 150, 150, 175), flStartPosX, flStartPosY + i*flItemHeight, flStartPosX + flItemWidth, flStartPosY + (i + 1)*flItemHeight);
				DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0, 150), flStartPosX, flStartPosY + i*flItemHeight, flStartPosX + flItemWidth, flStartPosY + (i + 1)*flItemHeight);
				DrawManager::DrawString(SSDK::Color(255, 255, 255, 200), itemsList[i].cTitle, flStartPosX + 5, flStartPosY + (i*flItemHeight) + 3);
				DrawManager::DrawStringValue(SSDK::Color(255, 255, 255, 200), flStartPosX + flItemWidth - 28, flStartPosY + (i*flItemHeight) + 3, "%2.2f", itemsList[i].flValue);
			}
			else {
				DrawManager::DrawRect(SSDK::Color(64, 64, 64, 175), flStartPosX, flStartPosY + i*flItemHeight, flStartPosX + flItemWidth, flStartPosY + (i + 1)*flItemHeight);
				DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0, 150), flStartPosX, flStartPosY + i*flItemHeight, flStartPosX + flItemWidth, flStartPosY + (i + 1)*flItemHeight);
				DrawManager::DrawString(SSDK::Color(255, 255, 255, 200), itemsList[i].cTitle, flStartPosX + 5, flStartPosY + (i*flItemHeight) + 3);
				DrawManager::DrawStringValue(SSDK::Color(255, 255, 255, 200), flStartPosX + flItemWidth - 28, flStartPosY + (i*flItemHeight) + 3, "%2.2f", itemsList[i].flValue);
			}
		}
	}
}

bool Menu::shouldShowMenu() {
	return this->bMenuShow;
}

void Menu::keyHandler() {
	if (GetAsyncKeyState(VK_INSERT) & 0x1) {
		bMenuShow = !bMenuShow;
	}
	if (bMenuShow) {
		if (GetAsyncKeyState(VK_DOWN) & 0x1) {
			if (iItemIndex + 1 < iItemCount) {
				iItemIndex += 1;
			} else {
				iItemIndex = 0;
			}
		}
		if (GetAsyncKeyState(VK_UP) & 0x1) {
			if (iItemIndex - 1 > 0) {
				iItemIndex -= 1;
			}
			else {
				iItemIndex = iItemCount;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x1) {
			if (itemsList[iItemIndex].flValue + 1 > itemsList[iItemIndex].flMax) {
				itemsList[iItemIndex].flValue = 0.f;
			}
			else {
				itemsList[iItemIndex].flValue += 1.f;
			}
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x1) {
			if (itemsList[iItemIndex].flValue - 1 < itemsList[iItemIndex].flMin) {
				itemsList[iItemIndex].flValue = itemsList[iItemIndex].flMax;
			}
			else {
				itemsList[iItemIndex].flValue -= 1.f;
			}
		}
	}
}

float Menu::getValue(int index) {
	return itemsList[index].flValue;
}