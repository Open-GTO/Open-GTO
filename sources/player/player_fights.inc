/*

	Created: 29.05.2010
	Author: ziggi

*/

#if defined _player_fights_included
	#endinput
#endif

#define _player_fights_included
#pragma library player_fights


stock pl_fights_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused dialogid, inputtext
	if (!response) {
		show_User_Menu(playerid);
		return 0;
	}

	new styleid = listitem + 4;
	pl_fights_SetPlayerStyleUsed(playerid, styleid);
	SetPlayerFightingStyle(playerid, styleid);

	new string[MAX_STRING];
	fights_GetNameByID(styleid, string);
	
	format(string, sizeof(string), lang_texts[19][11], string);
	show_User_Menu_Dialog(playerid, lang_texts[19][2], string);
	return 1;
}

stock pl_fights_ShowStyleMenu(playerid)
{
	new string[MAX_STRING];
	if (pl_fights_GetPlayerStyle(playerid) >= FIGHT_STYLE_NORMAL) {
		strcat(string, lang_texts[19][15]);
	}

	if (pl_fights_GetPlayerStyle(playerid) >= FIGHT_STYLE_BOXING) {
		strcat(string, lang_texts[19][16]);
	}

	if (pl_fights_GetPlayerStyle(playerid) >= FIGHT_STYLE_KUNGFU) {
		strcat(string, lang_texts[19][17]);
	}

	if (pl_fights_GetPlayerStyle(playerid) >= FIGHT_STYLE_KNEEHEAD) {
		strcat(string, lang_texts[19][18]);
	}

	return ShowPlayerDialog(
		playerid, pl_fights_DialogID, DIALOG_STYLE_LIST,
		lang_texts[19][12],
		string,
		lang_texts[19][13], lang_texts[19][14]
	);
}

stock pl_fights_GetPlayerStyle(playerid)
{
	return GetPVarInt(playerid, "pl_fights_StyleLevel");
}

stock pl_fights_SetPlayerStyle(playerid, styleid)
{
	SetPVarInt(playerid, "pl_fights_StyleLevel", styleid);
}

stock pl_fights_GetPlayerStyleUsed(playerid)
{
	return GetPVarInt(playerid, "pl_fights_StyleUsed");
}

stock pl_fights_SetPlayerStyleUsed(playerid, styleid)
{
	SetPVarInt(playerid, "pl_fights_StyleUsed", styleid);
}
