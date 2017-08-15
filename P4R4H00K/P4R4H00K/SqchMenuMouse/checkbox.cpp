#include "../SqchSDK/SDK.h"
#include "menu.h"

void MenuElements::Checkbox::Think()
{
	visible = parent->GetVisible();

	if (!visible) return;

	if (Input->Clicked(rx, ry, size, size))
	{
		value->m_bValue = !value->m_bValue;
	}
}

void MenuElements::Checkbox::Paint()
{
	if (!visible) return;

	SSDK::Color color = SSDK::Color(27, 195, 106);
	SSDK::Color outlineColor = SSDK::Color(color.r(), color.g(), color.b(), 100);

	DrawManager::DrawOutlinedRect(SSDK::Color(140, 140, 140), rx, ry, size, size);
	
	if (value->m_bValue)
	{
		DrawManager::DrawOutlinedRect(outlineColor, rx + 2, ry + 2, size - 4, size - 4);
		DrawManager::DrawRect(color, rx + 3, ry + 3, size - 6, size - 6);
	}
	
	DrawManager::DrawString(SSDK::Color(255, 255, 255), title, rx + size + 4, ry + 8);
}