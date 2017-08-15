#include "CLagCompensation2.h"

CLagCompensation2 LagCompensation2;

void CLagCompensation2::SaveNetvars(StoredNetvars *dest, C_CSPlayer *pEntity) {
	dest->origin = *pEntity->GetOrigin();

	dest->flags = *pEntity->GetFlags();
	dest->simulationtime = *pEntity->GetSimulationTime();
	dest->min = *pEntity->GetMin();
	dest->max = *pEntity->GetMax();
	dest->lowerbodyyaw = *pEntity->GetLowerBodyYaw();
	dest->eyeangles = *pEntity->GetEyeAngles();

	dest->sequence = *pEntity->GetSequence();
	dest->cycle = *pEntity->GetCycle();

	dest->velo = *pEntity->GetVelocityPointer();

	std::memcpy(dest->poseparam, pEntity->GetPosePosition(), 24 * sizeof(float));

	//not big deal proper anim fix
}

void CLagCompensation2::RestoreNetvars(StoredNetvars *src, C_CSPlayer *pEntity) {
	*pEntity->GetOrigin() = src->origin;

	*pEntity->GetFlags() = src->flags;
	*pEntity->GetSimulationTime() = src->simulationtime;
	*pEntity->GetMin() = src->min;
	*pEntity->GetMax() = src->max;
	*pEntity->GetLowerBodyYaw() = src->lowerbodyyaw;
	*pEntity->GetEyeAngles() = src->eyeangles;

	*pEntity->GetSequence() = src->sequence;
	*pEntity->GetCycle() = src->cycle;

	*pEntity->GetVelocityPointer() = src->velo;

	std::memcpy(pEntity->GetPosePosition(), src->poseparam, 24 * sizeof(float)); //skeet way of doing shit
}

float CLagCompensation2::GetLerpTime() {
	float updaterate = SSDK::I::CVar()->FindVar(XorStr("cl_updaterate"))->GetFloat();
	float ratio = SSDK::I::CVar()->FindVar(XorStr("cl_interp_ratio"))->GetFloat();
	float lerp = SSDK::I::CVar()->FindVar(XorStr("cl_interp"))->GetFloat();

	return max(lerp, ratio / updaterate);
}

float CLagCompensation2::GetLatency(int type) {
	SSDK::INetChannelInfo *nci = SSDK::I::Engine()->GetNetChannelInfo();

	if (nci) {
		return nci->GetLatency(type);
	}

	return 0.f;
}

bool CLagCompensation2::isValidTick(int tick) {
	if (TICKS_TO_TIME(*C_CSPlayer::GetLocalPlayer()->GetTickBase() - tick) < Utils::ClampValue((.2f + GetLatency(FLOW_INCOMING) - GetLatency(FLOW_OUTGOING)), 0.f, .9f))
		return true;

	return false;
}

void CLagCompensation2::SetValidTickCount(C_CSPlayer* pEntity, SSDK::CUserCmd* pCmd) {
	int iEntityId = pEntity->EntIndex();

	if (MenuOptions.Aimbot.g_checkBacktracked && pRecordRollback[iEntityId]) {
		pCmd->tick_count = TIME_TO_TICKS(pRecordRollback[iEntityId]->simulationtime + GetLerpTime());
	}
	else {
		pCmd->tick_count = TIME_TO_TICKS(*pEntity->GetSimulationTime() + GetLerpTime());
	}
}

void CLagCompensation2::StoreDatas(C_CSPlayer* pEntity) {
	int iEntityId = pEntity->EntIndex();

	for (int j = 0; j < vecLagRecord[iEntityId].size(); ++j) {
		if (!isValidTick(TIME_TO_TICKS(vecLagRecord[iEntityId][j].simulationtime + GetLerpTime()))) {
			vecLagRecord[iEntityId].erase(vecLagRecord[iEntityId].begin() + j);
			break;
		}
	}

	//LOWERBODYUPDATE
	static float flOldLowerBody[64];

	float flCurLowerBody = *pEntity->GetLowerBodyYaw();

	bool bLbyUpdate = false;

	if (flOldLowerBody[iEntityId] != flCurLowerBody) {
		flOldLowerBody[iEntityId] = flCurLowerBody;
		bLbyUpdate = true;
	}

	//CORE
	static float flOldSimTime[64];

	float flCurSimTime = *pEntity->GetSimulationTime();

	if (fabs(flOldSimTime[pEntity->EntIndex()] - flCurSimTime) > 5.f)
		flOldSimTime[pEntity->EntIndex()] = 0.f;

	StoredNetvars LagRecord;

	if (flOldSimTime[iEntityId] < flCurSimTime) {
		SaveNetvars(&LagRecord, pEntity);
		LagRecord.lbyupdate = bLbyUpdate;

		vecLagRecord[iEntityId].emplace_back(LagRecord);

		flOldSimTime[iEntityId] = flCurSimTime;
	}
}

int CLagCompensation2::GetLastLbyRecord(C_CSPlayer* pEntity) {
	int iEntityId = pEntity->EntIndex();

	C_CSPlayer* pLocalPlayer = C_CSPlayer::GetLocalPlayer();

	if (!pLocalPlayer)
		return false;

	if (pEntity->GetTeamNum() == pLocalPlayer->GetTeamNum())
		return -1;

	if (vecLagRecord[iEntityId].empty())
		return -1;

	int result = -1;

	for (int i = 0; i < vecLagRecord[iEntityId].size(); ++i) {
		if (vecLagRecord[iEntityId][i].lbyupdate)
			result = i;
	}

	return result;
}

void CLagCompensation2::RestorePlayer(C_CSPlayer* pEntity) {
	if (!MenuOptions.Aimbot.g_checkBacktracked)
		return;

	if (vecLagRecord[pEntity->EntIndex()].empty())
		return;

	int bestLbyTarget = GetLastLbyRecord(pEntity);
	StoredNetvars *src;
	if (bestLbyTarget != -1 && !pEntity->isMoving()) {
		src = &vecLagRecord[pEntity->EntIndex()][bestLbyTarget];
	}
	else {
		src = &vecLagRecord[pEntity->EntIndex()].back();
	}

	pRecordRollback[pEntity->EntIndex()] = src;

	RestoreNetvars(src, pEntity);
}

bool CLagCompensation2::isLbyUpdate(C_CSPlayer* pEntity) {
	//if (!MenuOptions.ESP.g_lbyUpdate)
	//return false;

	int iEntityId = pEntity->EntIndex();

	if (pEntity->GetTeamNum() == C_CSPlayer::GetLocalPlayer()->GetTeamNum())
		return false;

	if (vecLagRecord[iEntityId].empty())
		return false;

	for (int i = 0; i < vecLagRecord[iEntityId].size(); ++i) {
		if (vecLagRecord[iEntityId][i].lbyupdate)
			return true;
	}

	return false;
}