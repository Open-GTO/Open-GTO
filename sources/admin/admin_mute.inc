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
	new mutetime = player_GetMuteTime(playerid);

	if (mutetime != 0 && gettime() >= mutetime) {
		UnMutePlayer(playerid);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[13][54], oGetPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOUR_YELLOW, string);
	}
}

stock MutePlayer(playerid, mutetime)
{
	player_SetMuteTime(playerid, gettime() + mutetime * 60);
	player_SetMuteCount(playerid, player_GetMuteCount(playerid) + 1);
	return 1;
}

stock UnMutePlayer(playerid)
{
	player_SetMuteTime(playerid, 0);
	return 1;
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

	if (!IsPlayerConnected(muteid)) {
		SendClientMessage(playerid, COLOUR_RED, "Этот игрок отключился!");
		return 1;
	}

	MutePlayer(muteid, mutetime);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][24], oGetPlayerName(playerid));
	SendClientMessage(muteid, COLOUR_RED, string);

	format(string, sizeof(string), lang_texts[12][50], oGetPlayerName(muteid), mutetime);
	SendClientMessageToAll(COLOUR_WHITE, string);

	GameMSG(lang_texts[13][7], oGetPlayerName(muteid), oGetPlayerName(playerid), mutetime);
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

	UnMutePlayer(unmuteid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][27], oGetPlayerName(playerid));
	SendClientMessage(unmuteid, COLOUR_GREEN, string);
	
	format(string, sizeof(string), lang_texts[12][51], oGetPlayerName(unmuteid));
	SendClientMessageToAll(COLOUR_WHITE, string);
	
	GameMSG(lang_texts[13][14], oGetPlayerName(unmuteid), oGetPlayerName(playerid));
	return 1;
}
