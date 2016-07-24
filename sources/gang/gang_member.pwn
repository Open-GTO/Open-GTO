/*

	About: gang member
	Author: ziggi

*/

#if defined _gang_member_included
	#endinput
#endif

#define _gang_member_included

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
	gRankVarName[GangMemberRank][MAX_GANG_RANK_NAME] = {
		"GANG_RANK_LEADER",
		"GANG_RANK_HELPER",
		"GANG_RANK_PAYMASTER",
		"GANG_RANK_INVITER",
		"GANG_RANK_SOLDIER"
	},
	gMember[MAX_GANG][MAX_GANG_SIZE][e_Gang_Member_Info];

new
	Iterator:LoadedGangMembers[MAX_GANGS]<MAX_GANG_SIZE>;

/*
	For publics
*/


GangMember_OnGameModeInit()
{
	Iter_Init(LoadedGangMembers);
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

stock GangMember_SetActiveStatus(gangid, memberid, bool:status, bool:safe = false, &next_value = 0)
{
	if (status) {
		Iter_Add(LoadedGangMembers[gangid], memberid);
	} else {
		if (safe) {
			Iter_SafeRemove(LoadedGangMembers[gangid], memberid, next_value);
		} else {
			Iter_Remove(LoadedGangMembers[gangid], memberid);
		}
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
	strcpy(gMember[gangid][memberid][e_gmName], name, MAX_PLAYER_NAME);
}

stock GangMember_GetName(gangid, memberid, name[], const size = sizeof(name))
{
	strcpy(name, gMember[gangid][memberid][e_gmName], size);
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

stock GangMember_IsPlayerHaveRank(playerid, GangMemberRank:rank)
{
	new
		gangid,
		memberid;

	gangid = GetPlayerGangID(playerid);
	memberid = GetPlayerGangMemberID(playerid);

	if (gangid == INVALID_GANG_ID || memberid == INVALID_MEMBER_ID) {
		return 0;
	}

	return GangMember_IsHaveRank(gangid, memberid, rank);
}

stock GangMember_GetRankName(gangid, memberid, Lang:lang, name[], const size = sizeof(name))
{
	new
		GangMemberRank:rank,
		var_name[MAX_GANG_NAME];

	rank = GangMember_GetRank(gangid, memberid);

	GangRankMember_GetVarName(gRankVarName[rank], var_name);
	Lang_GetText(lang, var_name, name, size);
}
