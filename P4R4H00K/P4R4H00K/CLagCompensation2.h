#pragma once

#include "SqchSDK\SDK.h"

#include "EntityManager.h"
#include "DrawManager.h"
#include "P4Menu\Config.h"
#include "P4Menu\Options.h"
#include "Utils.h"

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / SSDK::I::GlobalVars()->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( SSDK::I::GlobalVars()->interval_per_tick * ( t ) )

struct StoredNetvars {
	float lowerbodyyaw;
	SSDK::Vector origin;
	SSDK::Vector min;
	SSDK::Vector max;
	int flags;
	float simulationtime;
	SSDK::QAngle eyeangles;
	float poseparam[24];
	int sequence;
	float cycle;

	SSDK::Vector velo;

	bool lbyupdate;
};

class CLagCompensation2 {
private:
	void SaveNetvars(StoredNetvars *dest, C_CSPlayer *pEntity);
	void RestoreNetvars(StoredNetvars *src, C_CSPlayer *pEntity);
public:
	std::vector<StoredNetvars> vecLagRecord[64];
	StoredNetvars *pRecordRollback[64];

	float GetLatency(int type);
	bool isValidTick(int tick);
	float GetLerpTime();
	void SetValidTickCount(C_CSPlayer* pEntity, SSDK::CUserCmd* pCmd);
	void StoreDatas(C_CSPlayer* pEntity);
	int GetLastLbyRecord(C_CSPlayer* pEntity);
	void RestorePlayer(C_CSPlayer* pEntity);
	bool isLbyUpdate(C_CSPlayer* pEntity);
};

extern CLagCompensation2 LagCompensation2;