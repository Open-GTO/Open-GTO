/*
	
	About: player gang
	Author: ziggi

*/

#if defined _player_gang_included
	#endinput
#endif

#define _player_gang_included
#pragma library player_gang

/*
	Defines
*/

#if !defined MAX_GANG_INVITES
	#define MAX_GANG_INVITES 3
#endif

/*
	Vars
*/

static
	gPlayerGangID[MAX_PLAYERS],
	gPlayerGangName[MAX_PLAYERS][MAX_NAME],
	gPlayerInvitedGangID[MAX_PLAYERS][MAX_GANG_INVITES char],
	gPlayerGangMemberID[MAX_PLAYERS char];

/*
	Command
*/


/*
	Invite ID
*/

stock IsPlayerInvitedInGang(playerid, gangid) {
	return gPlayerInvitedGangID[playerid]{gangid} != 0;
}

stock GetPlayerInvitedGangID(playerid, invited_gangs[], &size) {
	size = 0;

	for (new i = 0; i < sizeof(gPlayerInvitedGangID[]); i++) {
		if (gPlayerInvitedGangID[playerid]{i} != 0) {
			invited_gangs[size++] = gPlayerInvitedGangID[playerid]{i};
		}
	}

	return size != 0;
}

stock SetPlayerInvitedGangID(playerid, gangid, bool:status) {
	for (new i = 0; i < sizeof(gPlayerInvitedGangID[]); i++) {
		if (status) {
			if (gPlayerInvitedGangID[playerid]{i} == 0) {
				gPlayerInvitedGangID[playerid]{i} = gangid;
				return 1;
			}
		} else {
			if (gPlayerInvitedGangID[playerid]{i} == gangid) {
				gPlayerInvitedGangID[playerid]{i} = 0;
				return 1;
			}
		}
	}

	return 0;
}

/*
	IsPlayersTeammates
*/

stock IsPlayersTeammates(...)
{
	new gangid = GetPlayerGangID(getarg(0));
	if (gangid == 0) {
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

stock GetPlayerGangName(playerid, name[], const size = sizeof(name)) {
	strmid(name, gPlayerGangName[playerid], 0, strlen(gPlayerGangName[playerid]), size);
}

stock ReturnPlayerGangName(playerid) {
	return gPlayerGangName[playerid];
}

stock SetPlayerGangName(playerid, name[]) {
	strmid(gPlayerGangName[playerid], name, 0, strlen(name), MAX_NAME);
}

/*
	GangID
*/

stock IsPlayerInGang(playerid) {
	return gPlayerGangID[playerid] != 0;
}

stock GetPlayerGangID(playerid) {
	return gPlayerGangID[playerid];
}

stock SetPlayerGangID(playerid, gangid) {
	gPlayerGangID[playerid] = gangid;
}


/*
	GangMemberID
*/

stock GetPlayerGangMemberID(playerid) {
	return gPlayerGangMemberID{playerid};
}

stock SetPlayerGangMemberID(playerid, memberid) {
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

