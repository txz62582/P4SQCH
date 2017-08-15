#include "../P4Menu/P4Menu.h"

const char* configNames[] = {
	"No Config",
	"Last Config (AUTO)",
	"pHvH",
	"pHvH Rescue",
	"pHvH Backup",
	"Legit asf",
	"Legit blatant"
};

const char* configFileNames[] = {
	"",
	"LASTCONFIG.ini",
	"pHvH.ini",
	"pHvHRescue.ini",
	"pHvHBackup.ini",
	"LegitASF.ini",
	"Legit blatant.ini"
};

const char *pitchAA[] = 
{
	XorStr("NONE"),
	XorStr("Down"),
	XorStr("Down test"),
	XorStr("Mid"),
	XorStr("Up")
	//XorStr("Fake Mid UNTRUST"),
	//XorStr("Fake Down UNTRUST")
};

const char* aimBonesRage[] =
{
	XorStr("Head"), XorStr("Neck"), XorStr("Lower neck"), XorStr("Pelvis"), XorStr("Body"), XorStr("Thorax"),
	XorStr("Chest"),XorStr("Upper chest"), XorStr("Right thigh"), XorStr("Left thigh"), XorStr("Right calf"),
	XorStr("Left calf"), XorStr("Right foot"), XorStr("Left foot"), XorStr("Right hand"), XorStr("Left hand"),
	XorStr("Right upper arm"), XorStr("Right forearm"), XorStr("Left upper arm"), XorStr("Left forearm")
};

const char* aimBonesLegit[] =
{
	"HEAD",
	"NECK",
	"LOWER NECK",
	"PELVIS",
	"BODY",
	"THORAX"
};

const char* resolverOptions[] =
{
	"SIMPLE",
	"NONE"
};