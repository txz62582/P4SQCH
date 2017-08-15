#pragma once

#include <map>
#include "../DrawManager.h"

#define MAX_ITEMS 64

class Menu {
public:
	void Initialize();
	void DrawMenu();
	bool shouldShowMenu();
	void keyHandler();
	float getValue(int);
private:
	struct items {
		char *cTitle;
		float flValue;
		float flMin;
		float flMax;
		bool isBigTitle; //Diff color
	};

	float flStartPosX;
	float flStartPosY;
	float flItemWidth;
	float flItemHeight;
	int iItemCount = 0;
	int iItemIndex = 0;

	bool bMenuShow = false;

	items itemsList[MAX_ITEMS];
	void AddItem(char*, float, float, float, bool);
};