#include "../SqchSDK/SDK.h"
#include "menu.h"

void MenuElements::Button::Think()
{
	visible = parent->GetVisible();

	if (!visible) return;

	if (Input->Clicked(rx, ry, width, height))
		callback();
}

void MenuElements::Button::Paint()
{
	if (!visible) return;

	SSDK::Color color = SSDK::Color(27, 195, 106);
	SSDK::Color outlineColor = SSDK::Color(color.r(), color.g(), color.b(), 100);

	DrawManager::DrawOutlinedRect(SSDK::Color(140, 140, 140), rx, ry, width, height);
	DrawManager::DrawRect(SSDK::Color(27, 27, 27), rx + 1, ry + 1, width - 2, height - 2);

	DrawManager::DrawString(SSDK::Color(255, 255, 255), title, rx + width / 2, ry + height / 2);
}