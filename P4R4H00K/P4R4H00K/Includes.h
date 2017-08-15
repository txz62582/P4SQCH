#pragma once
#include <Windows.h>
#include <memory>
#include "SqchSDK\SDK.h"
#include <iostream>
#include "Hooks.h"
#include "Utils.h"

#define MainColor SSDK::Color(124, 252, 0)
#define EnemyEspColor SSDK::Color(0, 102, 153)
#define EnemyVisibleEspColor SSDK::Color(124, 252, 0)
#define EnemyEspColorT SSDK::Color(255, 0, 0)
#define EnemyVisibleEspColorT SSDK::Color(255, 255, 0)
#define WHITE_COLOR SSDK::Color(255, 255, 255)
#define USERNAME "P4R4DiSi4C"
#define TIME_TO_TICKS(dt)		((int)(0.5f + (float)(dt) / SSDK::I::GlobalVars()->interval_per_tick))