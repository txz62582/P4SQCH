#pragma once

namespace SSDK {
	class PlayerInfo {
	public:
		typedef struct player_info_s
		{
			int64_t __pad0;
			union {
				int64_t xuid;
				struct {
					int xuidlow;
					int xuidhigh;
				};
			};
			char name[128];
			int userid;
			char guid[33];
			unsigned int friendsid;
			char friendsname[128];
			bool fakeplayer;
			bool ishltv;
			unsigned int customfiles[4];
			unsigned char filesdownloaded;
		} player_info_t;

		char           pad_0x0000[0x10]; //0x0000
		char           szName[32]; //0x0010 
		char           pad_0x0030[0x64]; //0x0030
		char           szSteamID[20]; //0x0094 SteamID on text format (STEAM_X:Y:Z)
		char           pad_0x00A8[0x10]; //0x00A8
		unsigned long  iSteamID; //0x00B8 
		char           unknown2[0x14C];
	};
}