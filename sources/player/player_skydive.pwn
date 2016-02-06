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
		SendClientMessage(playerid, COLOR_WHITE, _(SKYDIVING_ERROR));
		return 1;
	}

	new string[MAX_STRING];
	new time_sky = GetPlayerSkydiveTime(playerid) - gettime();
	if (time_sky > 0) {
		new
			minutes = (time_sky / 60) % 60,
			seconds = time_sky % 60;

		format(string, sizeof(string),
			_(SKYDIVING_NEED_TIME),
			minutes, Declension_ReturnMinutes(minutes),
			seconds, Declension_ReturnSeconds(seconds)
		);

		SendClientMessage(playerid, COLOR_WHITE, string);
		return 1;
	}

	GivePlayerWeapon(playerid, 46, 1);
	PlayerPlaySound(playerid, 1057, 0, 0, 0);

	new Float:playerx, Float:playery, Float:playerz;
	GetPlayerPos(playerid, playerx, playery, playerz);
	SetPlayerPos(playerid, playerx, playery, playerz + 1200);
	
	SetPlayerSkydiveTime(playerid, gettime() + SKYDIVE_TIME);

	SendClientMessage(playerid, COLOR_WHITE, _(SKYDIVING_MSG));
	GameTextForPlayer(playerid, _(SKYDIVING_GAMETEXT), 4000, 5);
	
	format(string, sizeof(string), _(SKYDIVING_MSG_TO_ALL), ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_GREY, string);
	return 1;
}

/*
	Functions
*/

stock GetPlayerSkydiveTime(playerid) {
	return gPlayerPauseTime[playerid];
}

stock SetPlayerSkydiveTime(playerid, time) {
	gPlayerPauseTime[playerid] = time;
}
