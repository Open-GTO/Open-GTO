/*

	About: admin jail system
	Author:	ziggi

*/

#if defined _adm_jail_included
	#endinput
#endif

#define _adm_jail_included
#pragma library adm_jail


enum JailCoordInfo {
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z,
	Float:Coord_A
}

new Float:jail_release_point[][JailCoordInfo] = {
	{633.365, -571.78, 16.340, 265.773},
	{1544.982, -1675.470, 13.600, 93.446},
	{-2164.396, -2388.342, 30.650, 140.551},
	{-1605.378, 717.512, 12.000, 310.972},
	{-1391.040, 2634.686, 55.984, 116.935},
	{-215.718, 985.399, 19.400, 240.854},
	{2335.229, 2455.809, 14.968, 136.734},
	{1225.165, 245.328, 19.554, 306.501}
};

stock JailPlayer(playerid, &jail_time)
{
	if (jail_time == 0) {
		jail_time = (player_GetJailCount(playerid) + 1) * 5;
	}

	player_SetJailTime(playerid, gettime() + jail_time * 60);
	player_SetJailCount(playerid, player_GetJailCount(playerid) + 1);
	
	SetPlayerInterior(playerid, 6);
	SetPlayerPos(playerid, 265.1273, 77.6823, 1001.0391);
	SetPlayerFacingAngle(playerid,-90);	
	TogglePlayerControllable(playerid, 0);
	SetCameraBehindPlayer(playerid);
	SetPlayerArmour(playerid, 0);
	PlayerPlaySound(playerid, 1082, 265.1273, 77.6823, 1001.0391);
	return 1;
}

stock JailPlayerCheck(playerid)
{
	if (player_IsJailed(playerid) && !IsPlayerInRangeOfPoint(playerid, 5, 265.1273, 77.6823, 1001.0391)) {
		SetPlayerPos(playerid, 265.1273, 77.6823, 1001.0391);
	}
	return 1;
}

stock UnJailPlayer(playerid)
{
	player_SetJailTime(playerid, -1);
	TogglePlayerControllable(playerid, 1);

	// ставим игрока в рандомное место из всех мест освобождений
	new j_id = random( sizeof(jail_release_point) );
	SetPlayerInterior(playerid, 0);
	SetPlayerPos(playerid, jail_release_point[j_id][Coord_X], jail_release_point[j_id][Coord_Y], jail_release_point[j_id][Coord_Z] );
	SetPlayerFacingAngle(playerid, jail_release_point[j_id][Coord_A]);
	SetCameraBehindPlayer(playerid);
	return 1;
}

stock JailPlayerTimer(playerid)
{
	new jail_time = player_GetJailTime(playerid);
	
	if (jail_time == -1) {
		return 0;
	}

	if (gettime() >= jail_time) {
		UnJailPlayer(playerid);

		new string[MAX_STRING],
			playername[MAX_PLAYER_NAME + 1];

		GetPlayerName(playerid, playername, sizeof(playername));
		
		format(string, sizeof(string), lang_texts[13][53], playername);
		SendClientMessageToAll(COLOR_WHITE, string);

		Log_Game(lang_texts[13][55], playername);
	}
	return 1;
}

COMMAND:jail(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new jailid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(jailid) || (IsPlayerHavePrivilege(jailid, PlayerPrivilegeRcon) && !IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon))) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new jailt = strval(strcharsplit(params, idx, ' '));

	JailPlayer(jailid, jailt);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][18], ReturnPlayerName(playerid));
	SendClientMessage(jailid, COLOR_RED, string);

	format(string, sizeof(string), lang_texts[13][15], jailt);
	format(string, sizeof(string), lang_texts[13][17], ReturnPlayerName(jailid), string);
	SendClientMessageToAll(COLOR_WHITE, string);

	GameTextForPlayer(jailid, lang_texts[13][18], 5000, 4);
	return 1;
}

COMMAND:unjail(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new unjailid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(unjailid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[12][2]);
		return 1;
	}

	if (!player_IsJailed(unjailid)) {
		SendClientMessage(playerid, COLOR_RED, lang_texts[13][22]);
		return 1;
	}

	UnJailPlayer(unjailid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][21], ReturnPlayerName(playerid));
	SendClientMessage(unjailid, COLOR_GREEN, string);
	return 1;
}
