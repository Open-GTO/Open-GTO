/*

	About: admin mute system
	Author:	ziggi

*/

#if defined _adm_mute_included
	#endinput
#endif

#define _adm_mute_included
#pragma library adm_mute


stock adm_mute_OnPlayerText(playerid, text[])
{
	#pragma unused text
	if (player_IsMuted(playerid)) {
		return 0;
	}
	return 1;
}

stock MutePlayerTimer(playerid)
{
	new string[MAX_STRING],
		mute_time = player_GetMuteTime(playerid);
	
	if (mute_time > 0)
	{
		mute_time--;
		player_SetMuteTime(playerid, mute_time);
	    if (mute_time <= 0)
	    {
			format(string, sizeof(string), lang_texts[13][54], oGetPlayerName(playerid), playerid);
			SendClientMessageToAll(COLOUR_YELLOW, string);
		}
	}
}

COMMAND:mute(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx,
		muteid = strval(strcharsplit(params, idx, ' ')),
		mutetime = strval(strcharsplit(params, idx, ' '));

	if (mutetime < 1) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][13]);
		return 1;
	}

	if (IsPlayerConnected(muteid)) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[13][12], oGetPlayerName(playerid), oGetPlayerName(muteid), mutetime);
		SendClientMessageToAll(COLOUR_YELLOW, string);
	}

	player_SetMuteTime(muteid, mutetime);
	player_SetMuteCount(muteid, player_GetMuteCount(muteid) + 1); // сколько раз всего затыкали игрока

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][24], oGetPlayerName(playerid));
	SendClientMessage(muteid, COLOUR_RED, string);

	format(string, sizeof(string), lang_texts[12][50], oGetPlayerName(muteid));
	SendClientMessageToAll(COLOUR_WHITE, string);
	return 1;
}

COMMAND:unmute(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	new idx;
	new unmuteid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(unmuteid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}

	player_SetMuteTime(unmuteid, 0);
	SetPlayerWantedLevel(unmuteid, 0);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][27], oGetPlayerName(playerid));
	SendClientMessage(unmuteid, COLOUR_GREEN, string);
	
	format(string, sizeof(string), lang_texts[12][51], oGetPlayerName(unmuteid));
	SendClientMessageToAll(COLOUR_WHITE, string);
	
	GameMSG(lang_texts[13][14], oGetPlayerName(unmuteid), oGetPlayerName(playerid));
	return 1;
}
