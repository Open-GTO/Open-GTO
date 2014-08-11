/*

	About: admin jail system
	Author:	ziggi

*/

#if defined _adm_jail_included
	#endinput
#endif

#define _adm_jail_included
#pragma library adm_jail


enum CoordInfoJ {
    Float:CJ_X,
    Float:CJ_Y,
    Float:CJ_Z,
    Float:CJ_A
}

new Float:Jail_PH[][CoordInfoJ] = {
	{633.365, -571.78, 16.340, 265.773},
	{1544.982, -1675.470, 13.600, 93.446},
	{-2164.396, -2388.342, 30.650, 140.551},
	{-1605.378, 717.512, 12.000, 310.972},
	{-1391.040, 2634.686, 55.984, 116.935},
	{-215.718, 985.399, 19.400, 240.854},
	{2335.229, 2455.809, 14.968, 136.734},
	{1225.165, 245.328, 19.554, 306.501}
};

stock JailPlayer(playerid, jail_time)
{
	player_SetJailTime(playerid, jail_time);
	SetPlayerWantedLevel(playerid, (jail_time == 0 || jail_time > 6) ? 6 : jail_time);
	
	SetPlayerInterior(playerid, 6);
	oSetPlayerPos(playerid, 265.1273, 77.6823, 1001.0391);
	SetPlayerFacingAngle(playerid,-90);	
	TogglePlayerControllable(playerid, 0);
	SetCameraBehindPlayer(playerid);
	oSetPlayerArmour(playerid, 0);
	PlayerPlaySound(playerid, 1082, 265.1273, 77.6823, 1001.0391);
	return 1;
}

stock JailPlayerCheck(playerid)
{
	if (player_IsJailed(playerid) && !IsPlayerInRangeOfPoint(playerid, 5, 265.1273, 77.6823, 1001.0391)) {
		oSetPlayerPos(playerid, 265.1273, 77.6823, 1001.0391);
	}
	return 1;
}

stock UnJailPlayer(playerid)
{
	player_SetJailTime(playerid, -1);
	SetPlayerWantedLevel(playerid, 0);
	TogglePlayerControllable(playerid, 1);
	// ставим игрока в рандомное место из всех мест освобождений
	new j_id = random( sizeof(Jail_PH) );
	SetPlayerInterior(playerid, 0);
	oSetPlayerPos(playerid, Jail_PH[j_id][CJ_X], Jail_PH[j_id][CJ_Y], Jail_PH[j_id][CJ_Z] );
	SetPlayerFacingAngle(playerid, Jail_PH[j_id][CJ_A]);
	SetCameraBehindPlayer(playerid);
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[13][53], oGetPlayerName(playerid));
	SendClientMessageToAll(COLOUR_WHITE, string);
	return 1;
}

stock JailPlayerTimer(playerid)
{
	new string[MAX_STRING];
	new jail_time = player_GetJailTime(playerid);
	
	if (jail_time == -1)
	{
		return 0;
	}
	SetPlayerWantedLevel(playerid, (jail_time == 0 || jail_time > 6) ? 6 : jail_time);
	
	if (jail_time >= 1 && jail_time <= 4320)
	{
		jail_time--;
		player_SetJailTime(playerid, jail_time);
		if (jail_time <= 0)
		{
			UnJailPlayer(playerid);
			GameMSG(lang_texts[13][55], oGetPlayerName(playerid));
		}
		else
		{
			format(string, sizeof(string), lang_texts[13][56], jail_time);
			SendClientMessage(playerid, COLOUR_LIGHTRED, string);
		}
	}
	return 1;
}

COMMAND:jail(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new jailid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(jailid) || (IsPlayerRconAdmin(jailid) && !IsPlayerRconAdmin(playerid))) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new jailt = strval(strcharsplit(params, idx, ' '));
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][18], oGetPlayerName(playerid));
	SendClientMessage(jailid, COLOUR_RED, string);
	
	new string2[40];
	if (jailt >= 1 && jailt <= 4320) {
		format(string2, sizeof(string2), lang_texts[13][15], jailt);
	} else {
		set(string2, lang_texts[13][16]);
	}

	JailPlayer(jailid, jailt);
	player_SetJailCount(jailid, player_GetJailCount(jailid) + 1);
	
	format(string, sizeof(string), lang_texts[13][17], oGetPlayerName(jailid), string2);
	SendClientMessageToAll(COLOUR_WHITE, string);

	GameTextForPlayer(jailid, lang_texts[13][18], 5000, 4);
	return 1;
}

COMMAND:unjail(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new unjailid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(unjailid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	if (!player_IsJailed(unjailid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][22]);
		return 1;
	}

	UnJailPlayer(unjailid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][21], oGetPlayerName(playerid));
	SendClientMessage(unjailid, COLOUR_GREEN, string);
	return 1;
}