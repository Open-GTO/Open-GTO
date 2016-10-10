/*

	About: skydive command
	Author: ziggi

*/

#if defined _player_skydive_included
	#endinput
#endif

#define _player_skydive_included

/*
	Vars
*/

static
	gPlayerPauseTime[MAX_PLAYERS];

/*
	Command
*/

COMMAND:skydive(playerid, params[])
{
	if (IsPlayerJailed(playerid) || GetPlayerInterior(playerid) != 0 || IsPlayerAtQuest(playerid)) {
		Lang_SendText(playerid, "SKYDIVING_ERROR");
		return 1;
	}

	new time_sky = GetPlayerSkydiveTime(playerid) - gettime();
	if (time_sky > 0) {
		new
			minutes = (time_sky / 60) % 60,
			seconds = time_sky % 60;

		Lang_SendText(playerid, "SKYDIVING_NEED_TIME",
		              minutes, Declension_ReturnMinutes(playerid, minutes),
		              seconds, Declension_ReturnSeconds(playerid, seconds));
		return 1;
	}

	GivePlayerWeapon(playerid, 46, 1);
	PlayerPlaySound(playerid, 1057, 0, 0, 0);

	new Float:pos_x, Float:pos_y, Float:pos_z;
	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	SetPlayerPos(playerid, pos_x, pos_y, pos_z + 1200);

	SetPlayerSkydiveTime(playerid, gettime() + SKYDIVE_TIME);

	Lang_SendText(playerid, "SKYDIVING_MSG");
	Lang_GameText(playerid, 4000, 5, "SKYDIVING_GAMETEXT");

	Lang_SendTextToAll("SKYDIVING_MSG_TO_ALL", ret_GetPlayerName(playerid), playerid);
	return 1;
}

/*
	Functions
*/

stock GetPlayerSkydiveTime(playerid)
{
	return gPlayerPauseTime[playerid];
}

stock SetPlayerSkydiveTime(playerid, time)
{
	gPlayerPauseTime[playerid] = time;
}
