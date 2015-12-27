/*
	
	About: gang member
	Author: ziggi

*/

#if defined _gang_member_included
	#endinput
#endif

#define _gang_member_included
#pragma library gang_member

/*
	Defines
*/

#define MAX_GANG_RANK_NAME 32
#define MAX_GANG_RANK_COUNT 5

/*
	Enums
*/

// higher - better
enum GangMemberRank {
	GangMemberLeader,
	GangMemberHelper,
	GangMemberPaymaster,
	GangMemberInviter,
	GangMemberSoldier,
}

enum e_Gang_Member_Info {
	e_gmName[MAX_PLAYER_NAME + 1],
	e_gmID,
	GangMemberRank:e_gmRank,
	bool:e_gmIsExists,
}

/*
	Vars
*/

static
	gMember[MAX_GANG][MAX_GANG_SIZE][e_Gang_Member_Info],
	gRank[GangMemberRank][MAX_GANG_RANK_NAME];

new
	Iterator:LoadedGangMembers[MAX_GANGS]<MAX_GANG_SIZE>;

/*
	For publics
*/


GangMember_OnGameModeInit()
{
	Iter_Init(LoadedGangMembers);

	// init rank names
	GangRankMember_SetName(GangMemberLeader, _(GANG_RANK_LEADER));
	GangRankMember_SetName(GangMemberHelper, _(GANG_RANK_HELPER));
	GangRankMember_SetName(GangMemberPaymaster, _(GANG_RANK_PAYMASTER));
	GangRankMember_SetName(GangMemberInviter, _(GANG_RANK_INVITER));
	GangRankMember_SetName(GangMemberSoldier, _(GANG_RANK_SOLDIER));
	return 1;
}

/*
	Member ID
*/

stock GangMember_GetID(gangid, memberid)
{
	return gMember[gangid][memberid][e_gmID];
}

stock GangMember_SetID(gangid, memberid, id)
{
	gMember[gangid][memberid][e_gmID] = id;
}

/*
	Active member status
*/

stock GangMember_IsActive(gangid, memberid)
{
	return Iter_Contains(LoadedGangMembers[gangid], memberid);
}

stock GangMember_SetActiveStatus(gangid, memberid, bool:status)
{
	if (status) {
		Iter_Add(LoadedGangMembers[gangid], memberid);
	} else {
		Iter_Remove(LoadedGangMembers[gangid], memberid);
	}
}

stock GangMember_GetFreeSlot(gangid)
{
	return Iter_Free(LoadedGangMembers[gangid]);
}

/*
	Gang Member Name
*/

stock GangMember_SetName(gangid, memberid, name[])
{
	strmid(gMember[gangid][memberid][e_gmName], name, 0, strlen(name), MAX_PLAYER_NAME);
}

stock GangMember_GetName(gangid, memberid, name[], const size = sizeof(name))
{
	strmid(name, gMember[gangid][memberid][e_gmName], 0, strlen(gMember[gangid][memberid][e_gmName]), size);
}

/*
	Gang Member Exists
*/

stock GangMember_IsExists(gangid, memberid)
{
	return _:gMember[gangid][memberid][e_gmIsExists];
}

stock GangMember_SetExists(gangid, memberid, bool:status)
{
	gMember[gangid][memberid][e_gmIsExists] = status;
}


/*
	Gang Member Rank
*/

stock GangMemberRank:GangMember_GetRank(gangid, memberid)
{
	return gMember[gangid][memberid][e_gmRank];
}

stock GangMember_SetRank(gangid, memberid, GangMemberRank:rank)
{
	gMember[gangid][memberid][e_gmRank] = rank;
}

stock GangMember_IsHaveRank(gangid, memberid, GangMemberRank:rank)
{
	if (gMember[gangid][memberid][e_gmRank] <= rank) {
		return 1;
	}

	return 0;
}

stock GangMember_GetRankName(gangid, memberid, name[], const size = sizeof(name))
{
	GangRankMember_GetName(GangMember_GetRank(gangid, memberid), name, size);
}

/*
	Gang Rank Member
*/

stock GangRankMember_GetName(GangMemberRank:rank, name[], const size = sizeof(name))
{
	strmid(name, gRank[rank], 0, strlen(gRank[rank]), size);
}

stock GangRankMember_SetName(GangMemberRank:rank, name[])
{
	strmid(gRank[rank], name, 0, strlen(name), MAX_GANG_RANK_NAME);
}
