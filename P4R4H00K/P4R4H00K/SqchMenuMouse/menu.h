#pragma once

#include "elements.h"
#include "config.h"
#include "input.h"
#include "../DrawManager.h"
#include <vector>

class CMenu
{
private:
	std::vector<MenuElements::Element*> elements;
	MenuElements::Element* frame;
public:
	void Setup();
	void Paint();
	void Add(MenuElements::Element* element);
	MenuElements::Element* GetFrame();
}; extern CMenu* Menu;