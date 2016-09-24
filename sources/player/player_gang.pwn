/*

	About: player gang
	Author: ziggi

*/

#if defined _player_gang_included
	#endinput
#endif

#define _player_gang_included

/*
	Defines
*/

#if !defined MAX_GANG_INVITES
	#define MAX_GANG_INVITES 3
#endif

#if !defined MAX_GANG_INVITE_TIME
	#define MAX_GANG_INVITE_TIME 60
#endif

/*
	Enums
*/

enum e_Player_Invited_Gang_Info {
	e_pigID,
	e_pigTime,
}

/*
	Vars
*/

static
	gPlayerGangID[MAX_PLAYERS],
	gPlayerGangName[MAX_PLAYERS][MAX_GANG_NAME],
	gPlayerInvitedGang[MAX_PLAYERS][MAX_GANG_INVITES][e_Player_Invited_Gang_Info],
	gPlayerGangMemberID[MAX_PLAYERS char];

/*
	Invite ID
*/

stock IsPlayerInvitedInAnyGang(playerid)
{
	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] != INVALID_GANG_ID) {
			return 1;
		}
	}

	return 0;
}

stock IsPlayerInvitedInGang(playerid, gangid)
{
	if (gangid == INVALID_GANG_ID) {
		return 0;
	}

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] == gangid) {
			return 1;
		}
	}

	return 0;
}

stock IsPlayerInvitedInGangByName(playerid, gangname[])
{
	new
		temp_name[MAX_GANG_NAME + 1];

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] == INVALID_GANG_ID) {
			continue;
		}

		Gang_GetName(gPlayerInvitedGang[playerid][i][e_pigID], temp_name);

		if (strcmp(temp_name, gangname, true) == 0) {
			return 1;
		}
	}

	return 0;
}

stock GetPlayerInvitedGangArrayID(playerid, invited_gangs[], &size)
{
	size = 0;

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] != INVALID_GANG_ID) {
			invited_gangs[size++] = gPlayerInvitedGang[playerid][i][e_pigID];
		}
	}

	return size != 0;
}

stock SetPlayerInvitedGangID(playerid, gangid, bool:status)
{
	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (status) {
			if (gPlayerInvitedGang[playerid][i][e_pigID] == INVALID_GANG_ID) {
				gPlayerInvitedGang[playerid][i][e_pigID] = gangid;
				gPlayerInvitedGang[playerid][i][e_pigTime] = gettime();
				return 1;
			}
		} else {
			if (gPlayerInvitedGang[playerid][i][e_pigID] == gangid) {
				gPlayerInvitedGang[playerid][i][e_pigID] = INVALID_GANG_ID;
				gPlayerInvitedGang[playerid][i][e_pigTime] = 0;
				return 1;
			}
		}
	}

	return 0;
}

stock GetPlayerInvitedGangArrayTime(playerid, invited_times[], &size)
{
	size = 0;

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] != INVALID_GANG_ID) {
			invited_times[size++] = gPlayerInvitedGang[playerid][i][e_pigTime];
		}
	}

	return size != 0;
}

stock GetPlayerInvitedGangArrayReTime(playerid, remaining_times[], &size)
{
	new
		current_time;

	size = 0;
	current_time = gettime();

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] != INVALID_GANG_ID) {
			remaining_times[size++] = gPlayerInvitedGang[playerid][i][e_pigTime] + MAX_GANG_INVITE_TIME - current_time;
		}
	}

	return size != 0;
}

stock GetPlayerInvitedGangArrayInfo(playerid, invited_gangs[] = "", invited_times[] = "", remaining_times[] = "", &size = 0)
{
	new
		current_time = gettime();

	size = 0;

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (gPlayerInvitedGang[playerid][i][e_pigID] != INVALID_GANG_ID) {
			invited_gangs[size] = gPlayerInvitedGang[playerid][i][e_pigID];
			invited_times[size] = gPlayerInvitedGang[playerid][i][e_pigTime];
			remaining_times[size] = gPlayerInvitedGang[playerid][i][e_pigTime] + MAX_GANG_INVITE_TIME - current_time;
			size++;
		}
	}

	return size != 0;
}

stock CheckPlayerInvitedGangTime(playerid)
{
	new
		current_time = gettime();

	for (new i = 0; i < MAX_GANG_INVITES; i++) {
		if (current_time >= MAX_GANG_INVITE_TIME + gPlayerInvitedGang[playerid][i][e_pigTime]) {
			SetPlayerInvitedGangID(playerid, gPlayerInvitedGang[playerid][i][e_pigID], false);
		}
	}
}

/*
	ResetPlayerGangData
*/

stock ResetPlayerGangData(playerid)
{
	SetPlayerGangID(playerid, INVALID_GANG_ID);
	SetPlayerGangName(playerid, "");
	SetPlayerGangMemberID(playerid, INVALID_MEMBER_ID);
	SetPlayerColor(playerid, GetPlayerGangColor(playerid));

	if (GetPlayerSpawnType(playerid) == SPAWN_TYPE_GANG) {
		SetPlayerSpawnType(playerid, SPAWN_TYPE_NONE);
	}
}

/*
	IsPlayersTeammates
*/

stock IsPlayersTeammates(...)
{
	new gangid = GetPlayerGangID(getarg(0));
	if (gangid == INVALID_GANG_ID) {
		return 0;
	}

	for (new i = numargs() - 1; i != 0; i--) {
		if (gangid != GetPlayerGangID(getarg(i))) {
			return 0;
		}
	}

	return 1;
}

/*
	GangName
*/

stock GetPlayerGangName(playerid, name[], const size = sizeof(name))
{
	strcpy(name, gPlayerGangName[playerid], size);
}

stock ReturnPlayerGangName(playerid)
{
	return gPlayerGangName[playerid];
}

stock SetPlayerGangName(playerid, name[])
{
	strcpy(gPlayerGangName[playerid], name, MAX_GANG_NAME);
}

/*
	GangID
*/

stock IsPlayerInGang(playerid)
{
	return gPlayerGangID[playerid] != INVALID_GANG_ID;
}

stock GetPlayerGangID(playerid)
{
	return gPlayerGangID[playerid];
}

stock SetPlayerGangID(playerid, gangid)
{
	gPlayerGangID[playerid] = gangid;
}


/*
	GangMemberID
*/

stock GetPlayerGangMemberID(playerid)
{
	return gPlayerGangMemberID{playerid};
}

stock SetPlayerGangMemberID(playerid, memberid)
{
	gPlayerGangMemberID{playerid} = memberid;
}

/*
	Player Color
*/

stock GetPlayerGangColor(playerid)
{
	new gangid = GetPlayerGangID(playerid);

	if (gangid != INVALID_GANG_ID) {
		return Gang_GetColor(gangid);
	}

	return COLOR_PLAYER;
}

