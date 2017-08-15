#include "DrawManager.h"

SSDK::HFont DrawManager::hFont = 0;
SSDK::HFont DrawManager::hFont1 = 0;
SSDK::HFont DrawManager::WaterMarkFont = 0;
SSDK::HFont DrawManager::hWeapon = 0;

void DrawManager::InitializeFont(const char* fontName, int size) {
	hFont = SSDK::I::MatSurface()->CreateFont();
	SSDK::I::MatSurface()->SetFontGlyphSet(hFont, fontName, size, size * 10, 0, 0, (int)SSDK::FontFlags::FONTFLAG_OUTLINE);

	hFont1 = SSDK::I::MatSurface()->CreateFont();
	SSDK::I::MatSurface()->SetFontGlyphSet(hFont1, "Verdana", 50, 50 * 10, 0, 0, (int)SSDK::FontFlags::FONTFLAG_OUTLINE);

	hWeapon = SSDK::I::MatSurface()->CreateFont();
	SSDK::I::MatSurface()->SetFontGlyphSet(hWeapon, "Counter-Strike", 24, 0, 0, 0, (int)SSDK::FontFlags::FONTFLAG_OUTLINE);

	WaterMarkFont = SSDK::I::MatSurface()->CreateFont();
	SSDK::I::MatSurface()->SetFontGlyphSet(WaterMarkFont, fontName, size, size * 10, 0, 0, (int)SSDK::FontFlags::FONTFLAG_OUTLINE);

}

void DrawManager::DrawString(SSDK::Color cColor, const char* chString, int x, int y, bool center,bool left) {
	wchar_t formated[128] = { '\0' };
	wsprintfW(formated, L"%S", chString);

	int size, tall;
	SSDK::I::MatSurface()->GetTextSize(hFont, formated, size, tall);

	if (left)
		x -= size;

	if (center)
		x -= size / 2;

	SSDK::I::MatSurface()->DrawSetTextPos(x, y);
	SSDK::I::MatSurface()->DrawSetTextColor(cColor);
	SSDK::I::MatSurface()->DrawSetTextFont(hFont);
	SSDK::I::MatSurface()->DrawPrintText(formated, wcslen(formated));
}

void DrawManager::DrawLbyIndicator(SSDK::Color cColor, const char* chString, int x, int y, bool center) {
	wchar_t formated[128] = { '\0' };
	wsprintfW(formated, L"%S", chString);

	int size, tall;
	SSDK::I::MatSurface()->GetTextSize(hFont1, formated, size, tall);

	if (center)
		x -= size / 2;

	SSDK::I::MatSurface()->DrawSetTextPos(x, y);
	SSDK::I::MatSurface()->DrawSetTextColor(cColor);
	SSDK::I::MatSurface()->DrawSetTextFont(hFont1);
	SSDK::I::MatSurface()->DrawPrintText(formated, wcslen(formated));
}


void DrawManager::DrawStringValue(SSDK::Color cColor, int x, int y, const char* chString, ...) {
	char buffer[128];
	va_list list;
	va_start(list, chString);
	vsprintf_s(buffer, chString, list);
	va_end(list);
	wchar_t formated[128] = { '\0' };
	wsprintfW(formated, L"%S", buffer);
	SSDK::I::MatSurface()->DrawSetTextPos(x, y);
	SSDK::I::MatSurface()->DrawSetTextColor(cColor);
	SSDK::I::MatSurface()->DrawSetTextFont(hFont);
	SSDK::I::MatSurface()->DrawPrintText(formated, wcslen(formated));
}

void DrawManager::DrawCross(SSDK::Color cColor, int x, int y, int size) {
	SSDK::I::MatSurface()->DrawSetColor(cColor);
	SSDK::I::MatSurface()->DrawLine(x - (size / 2), y - 1, x + (size / 2), y);
	SSDK::I::MatSurface()->DrawLine(x - 1, y - (size / 2), x, y + (size / 2));

	int rectSize = size / 2;
	DrawOutlinedRect(cColor, x - rectSize / 2, y - rectSize / 2, x + rectSize / 2, y + rectSize / 2);
}

void DrawManager::DrawOutlinedRect(SSDK::Color cColor, float x0, float y0, float x1, float y1) {
	SSDK::I::MatSurface()->DrawSetColor(cColor);
	SSDK::I::MatSurface()->DrawOutlinedRect(x0, y0, x1, y1);
}

void DrawManager::DrawHealthBar(int health, float x0, float y0, float x1, float y1) {
	SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(0, 0, 0));
	SSDK::I::MatSurface()->DrawFilledRect(x0, y0, x1, y1);
	float height = y1 - y0;
	float calcHeight = height - ((height / 100) * health);
	SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(((100 - health)/100.0f)*255.0f, (health/100.0f)*255.0f, 0));
	SSDK::I::MatSurface()->DrawFilledRect(x0, y0 + calcHeight, x1, y1);
	DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0), x0 - 1, y0 - 1, x1 + 1, y1 + 1);
}

void DrawManager::DrawArmorBar(int armor, float x0, float y0, float x1, float y1) {
	SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(0, 0, 0, 210));
	SSDK::I::MatSurface()->DrawFilledRect(x0, y0, x1, y1);
	float height = y1 - y0;
	float calcHeight = height - ((height / 100) * armor);
	SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(0,(armor / 100.0f)*102.0f, (armor / 100.0f)*204.0f, 210));
	SSDK::I::MatSurface()->DrawFilledRect(x0, y0 + calcHeight, x1, y1);
	DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0), x0 - 1, y0 - 1, x1 + 1, y1 + 1);
}

void DrawManager::DrawPlayerName(int entIndex, int x, int y) {
	SSDK::PlayerInfo entityInfos;
	SSDK::I::Engine()->GetPlayerInfo(entIndex, &entityInfos);
	DrawString(SSDK::Color(255, 255, 255), entityInfos.szName, x, y, true);
}

void DrawManager::DrawBoneLine(SSDK::Color cColor, C_CSPlayer* pEntity, int p1, int p2) {
	SSDK::Vector screenPos1;
	SSDK::Vector screenPos2;
	if (Utils::W2S(Utils::GetEntityBone(pEntity, p1), screenPos1) & 
		Utils::W2S(Utils::GetEntityBone(pEntity, p2), screenPos2)) {
		SSDK::I::MatSurface()->DrawSetColor(cColor);
		SSDK::I::MatSurface()->DrawLine(screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y);
	}
}

void DrawManager::DrawBoneESP(SSDK::Color cColor, C_CSPlayer* pEnt) {
	SSDK::studiohdr_t* pStudioModel = SSDK::I::ModelInfo()->GetStudiomodel(pEnt->GetModel());
	if (pStudioModel)
	{
		for (int i = 0; i < pStudioModel->numbones; ++i)
		{
			SSDK::mstudiobone_t* pBone = pStudioModel->pBone(i);
			if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
				continue;

			DrawBoneLine(cColor, pEnt, i, pBone->parent);
		}
	}
}

void DrawManager::DrawBoneLine(SSDK::Color cColor, C_CSPlayer* pEntity, SSDK::matrix3x4_t mat[], int p1, int p2) {
	SSDK::Vector screenPos1;
	SSDK::Vector screenPos2;
	if (Utils::W2S(pEntity->GetEntityBoneFromMatrix(mat, p1), screenPos1) &
		Utils::W2S(pEntity->GetEntityBoneFromMatrix(mat, p2), screenPos2)) {
		SSDK::I::MatSurface()->DrawSetColor(cColor);
		SSDK::I::MatSurface()->DrawLine(screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y);
	}
}

void DrawManager::DrawBacktrack(SSDK::Color cColor, C_CSPlayer* pEnt, SSDK::matrix3x4_t mat[]) {
	SSDK::studiohdr_t* pStudioModel = SSDK::I::ModelInfo()->GetStudiomodel(pEnt->GetModel());
	if (!pStudioModel)
		return;

	for (int i = 0; i < pStudioModel->numbones; ++i)
	{
		SSDK::mstudiobone_t* pBone = pStudioModel->pBone(i);
		if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
			continue;

		DrawManager::DrawBoneLine(cColor, pEnt, mat, i, pBone->parent);
	}
}

void DrawManager::DrawDistLine(SSDK::Color cColor, C_CSPlayer* pEntity, float startposx, float startposy) {
	SSDK::Vector vecEntityOrigin = *pEntity->GetOrigin();
	SSDK::Vector vecEntityOrigin2D;
	if (Utils::W2S(vecEntityOrigin, vecEntityOrigin2D)) {
		SSDK::I::MatSurface()->DrawSetColor(cColor);
		SSDK::I::MatSurface()->DrawLine(vecEntityOrigin2D.x, vecEntityOrigin2D.y, startposx, startposy);
	}
}

void DrawManager::DrawCrossFHead(SSDK::Color cColor, int x, int y, int size) {
	SSDK::I::MatSurface()->DrawSetColor(cColor);
	SSDK::I::MatSurface()->DrawLine(x - (size / 2), y - 1, x + (size / 2), y);
	SSDK::I::MatSurface()->DrawLine(x - 1, y - (size / 2), x, y + (size / 2));
}

void DrawManager::DrawHeadCross(C_CSPlayer* pEntity) {
	SSDK::Vector screenPos1, screenPos2;
	if (Utils::W2S(Utils::GetEntityBone(pEntity, SSDK::ECSPlayerBones::head_0), screenPos1) &
		Utils::W2S(Utils::GetEntityBone(pEntity, SSDK::ECSPlayerBones::neck_0), screenPos2)) {
		SSDK::I::MatSurface()->DrawSetColor(SSDK::Color(193, 82, 82));
		DrawCrossFHead(SSDK::Color(0, 200, 0), screenPos1.x, screenPos1.y, (screenPos2.y - screenPos1.y) * 2);
	}
}

void DrawManager::DrawWeapon(C_CSPlayer* pEntity, float x, float y) {
	C_BaseCombatWeapon *activeWeapon = pEntity->GetActiveWeapon();
	if (activeWeapon) {
		wchar_t formated[64] = { '\0' };
		wsprintfW(formated, L"%c", activeWeapon->GetCSWpnData()->iconWeapon->cCharacterInFont);

		int tall;
		int size;
		SSDK::I::MatSurface()->GetTextSize(hWeapon, formated, size, tall);

		x -= size / 2;
		//y += tall / 2;

		SSDK::I::MatSurface()->DrawSetTextPos(x, y);
		SSDK::I::MatSurface()->DrawSetTextColor(SSDK::Color(255, 255, 255));
		SSDK::I::MatSurface()->DrawSetTextFont(hWeapon);
		SSDK::I::MatSurface()->DrawPrintText(formated, wcslen(formated));
	}
}

void DrawManager::DrawRect(SSDK::Color clr, float x0, float y0, float x1, float y1) {
	SSDK::I::MatSurface()->DrawSetColor(clr);
	SSDK::I::MatSurface()->DrawFilledRect(x0, y0, x1, y1);
}

void DrawManager::DrawESPRect(SSDK::Color clr, float x0, float y0, float x1, float y1) {
	DrawManager::DrawOutlinedRect(clr, x0, y0, x1, y1);
	DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0), x0 - 1, y0 - 1, x1 + 1, y1 + 1);
	DrawManager::DrawOutlinedRect(SSDK::Color(0, 0, 0), x0 + 1, y0 + 1, x1 - 1, y1 - 1);
}

void DrawManager::DrawAmmo(C_CSPlayer *pEntity, int x, int y) {
	C_BaseCombatWeapon *pActiveWeapon = pEntity->GetActiveWeapon();

	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->GetClip() < 0)
		return;

	SSDK::WeaponInfo_t *pWeaponInfo = pActiveWeapon->GetCSWpnData();

	if (!pWeaponInfo)
		return;

	int size, tall;
	SSDK::I::MatSurface()->GetTextSize(hFont, (const wchar_t*)"d", size, tall);

	DrawStringValue(SSDK::Color(255, 255, 255, 200), x, y + tall + 1, "%i / %i", (pActiveWeapon->GetNameWeap() == "Revolver" ? pActiveWeapon->GetClip() : pActiveWeapon->GetClip()), (pActiveWeapon->GetNameWeap() == "Revolver" ? pWeaponInfo->iMaxClip1+1 : pWeaponInfo->iMaxClip1));
}