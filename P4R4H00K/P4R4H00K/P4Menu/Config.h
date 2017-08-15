#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <lmcons.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <shlobj.h>
#include <time.h>
#include <random>
#include <sstream>
#include <fstream>
#include <shlwapi.h>
#include <iomanip>
#include <ctime>
#include "Options.h"

template <typename T>
class ConfigItem
{
	std::string category, name;
	T* value;
public:
	ConfigItem( std::string category, std::string name, T* value )
	{
		this->category = category;
		this->name = name;
		this->value = value;
	}
};

template <typename T>
class ConfigValue
{
public:
	ConfigValue( std::string category_, std::string name_, T* value_ )
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

template <typename T>
class ConfigSkinValue
{
public:
	ConfigSkinValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
protected:
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
	std::vector<ConfigSkinValue<int>*> skinInts;
	std::vector<ConfigSkinValue<bool>*> skinBools;
	std::vector<ConfigSkinValue<float>*> skinFloats;
private:
	void SetupValue( int&, int, std::string, std::string );
	void SetupValue( bool&, bool, std::string, std::string );
	void SetupValue( float&, float, std::string, std::string );
	void SetupSkinValue(int&, int, std::string, std::string);
	void SetupSkinValue(bool&, bool, std::string, std::string);
	void SetupSkinValue(float&, float, std::string, std::string);
public:
	CConfig()
	{
		Setup();
		SetupSkins();
	}

	void Setup();
	void SetupSkins();

	void Save(int isSkin = 0);
	void Load(int isSkin = 0);
	bool FileExists(const std::string&);
};

extern CConfig* Config;
