/*

	Created: 29.05.2010
	Author: ziggi

*/

#if defined _player_fights_included
	#endinput
#endif

#define _player_fights_included
#pragma library player_fights


DialogCreate:PlayerFights(playerid)
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

	Dialog_Open(playerid, Dialog:PlayerFights, DIALOG_STYLE_LIST, lang_texts[19][12], string, lang_texts[19][3], lang_texts[19][14]);
}

DialogResponse:PlayerFights(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 0;
	}

	new styleid = listitem + 4;
	pl_fights_SetPlayerStyleUsed(playerid, styleid);
	SetPlayerFightingStyle(playerid, styleid);

	new string[MAX_STRING];
	fights_GetNameByID(listitem, string);
	
	format(string, sizeof(string), lang_texts[19][11], string);
	Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX, lang_texts[19][2], string, lang_texts[19][14], lang_texts[19][13]);
	return 1;
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
