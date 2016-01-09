/*

	About: player mute system
	Author:	ziggi

*/

#if defined _pl_mute_included
	#endinput
#endif

#define _pl_mute_included
#pragma library pl_mute

#if !defined MAX_MUTE_REASON_LENGTH
	#define MAX_MUTE_REASON_LENGTH 64
#endif

stock pl_mute_OnPlayerText(playerid, text[])
{
	#pragma unused text

	if (player_IsMuted(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(MUTED_HELP_MESSAGE));
		return 0;
	}

	return 1;
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

stock MutePlayerTimer(playerid)
{
	new mutetime = player_GetMuteTime(playerid);

	if (mutetime != 0 && gettime() >= mutetime) {
		UnMutePlayer(playerid);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[13][54], ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_YELLOW, string);
	}
}

stock player_GetMuteCount(playerid) {
	return GetPVarInt(playerid, "Muted");
}

stock player_SetMuteCount(playerid, jailed) {
	SetPVarInt(playerid, "Muted", jailed);
}

stock player_GetMuteTime(playerid) {
	return GetPVarInt(playerid, "MuteTime");
}

stock player_SetMuteTime(playerid, time) {
	SetPVarInt(playerid, "MuteTime", time);
}

stock player_IsMuted(playerid) {
	return GetPVarInt(playerid, "MuteTime") != 0 ? 1 : 0;
}
