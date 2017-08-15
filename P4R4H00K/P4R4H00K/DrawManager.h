#pragma once

#include "SqchSDK\SDK.h"
#include "Utils.h"

class DrawManager {
private:
	static SSDK::HFont hFont;
	static SSDK::HFont hFont1;
	static SSDK::HFont WaterMarkFont;
	static SSDK::HFont hWeapon;
public:
	static void InitializeFont(const char* fontName, int size);
	static void DrawString(SSDK::Color cColor, const char* chString, int x, int y, bool center = false, bool left = false);
	static void DrawLbyIndicator(SSDK::Color cColor, const char* chString, int x, int y, bool center = false);
	static void DrawStringValue(SSDK::Color cColor, int x, int y, const char* chString, ...);
	static void DrawCross(SSDK::Color cColor, int x, int y, int size);
	static void DrawOutlinedRect(SSDK::Color cColor, float x0, float y0, float x1, float y1);
	static void DrawHealthBar(int health, float x0, float y0, float x1, float y1);
	static void DrawArmorBar(int health, float x0, float y0, float x1, float y1);
	static void DrawPlayerName(int entIndex, int x, int y);
	static void DrawBoneLine(SSDK::Color cColor, C_CSPlayer* pEntity, int p1, int p2);
	static void DrawBoneLine(SSDK::Color cColor, C_CSPlayer* pEntity, SSDK::matrix3x4_t mat[], int p1, int p2);
	static void DrawBacktrack(SSDK::Color cColor, C_CSPlayer* pEnt, SSDK::matrix3x4_t mat[]);
	static void DrawBoneESP(SSDK::Color cColor, C_CSPlayer* pEnt);
	static void DrawDistLine(SSDK::Color cColor, C_CSPlayer* pEntity, float startposx, float startposy);
	static void DrawCrossFHead(SSDK::Color cColor, int x, int y, int size);
	static void DrawHeadCross(C_CSPlayer* pEntity);
	static void DrawWeapon(C_CSPlayer* pEntity, float x, float y);
	static void DrawRect(SSDK::Color clr, float x0, float y0, float x1, float y1);
	static void DrawESPRect(SSDK::Color clr, float x0, float y0, float x1, float y1);
	static void DrawAmmo(C_CSPlayer *pEntity, int x, int y);

	static void DrawManager::DrawHitmarker(int iScreenWidth, int iScreenHeight) {
		auto lineSize = 8;
		int screenCenterX = iScreenWidth / 2;
		int screenCenterY = iScreenHeight / 2;

		SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(200, 200, 200, 255));
		SSDK::I::MatSurface()->DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
		SSDK::I::MatSurface()->DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
		SSDK::I::MatSurface()->DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
		SSDK::I::MatSurface()->DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));

	}

	static void DrawManager::DrawZoom() {
		int iScreenWidth, iScreenHeight;
		SSDK::I::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

		SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(0, 0, 0));
		SSDK::I::MatSurface()->DrawLine(0, iScreenHeight / 2, iScreenWidth, iScreenHeight / 2);
		SSDK::I::MatSurface()->DrawLine(iScreenWidth / 2, 0, iScreenWidth / 2, iScreenHeight);
	}
};