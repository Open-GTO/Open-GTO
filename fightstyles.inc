/*
	Created: 29.05.2010
	Author: ZiGGi
*/

#if defined _fightstyles_included
	#endinput
#endif

#define _fightstyles_included
#pragma library fightstyles


#define GetPlayerFightStyle(%0) GetPVarInt(%0, "FightStyle")
#define SetPlayerFightStyle(%0,%1) SetPVarInt(%0, "FightStyle",%1)

#define GetPlayerFightStyleUsed(%0) GetPVarInt(%0, "FightStyleUsed")
#define SetPlayerFightStyleUsed(%0,%1) SetPVarInt(%0, "FightStyleUsed",%1)

enum FightInfo {
	Style,
	StyleName[MAX_STRING],
	Cost,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z,
};

new FightTreners[][FightInfo] = {
	{FIGHT_STYLE_NORMAL, "нормальный", 0, 0.0, 0.0, 0.0},
	{FIGHT_STYLE_BOXING, "бокс", 1000, 767.6851, 12.8269, 1000.7025}, // LS
	{FIGHT_STYLE_KUNGFU, "кунг-фу", 2000, 768.5967,-22.9764, 1000.5859}, // SF
	{FIGHT_STYLE_KNEEHEAD, "колено-голова", 3000, 766.5240,-76.6523, 1000.6563} // LV
};

stock fights_OnGameModeInit()
{
	for (new fight_id = 1; fight_id < sizeof(FightTreners); fight_id++)
	{
	    CreatePickup(1314, 23, FightTreners[fight_id][Coord_X], FightTreners[fight_id][Coord_Y], FightTreners[fight_id][Coord_Z]);
	}
	GameMSG(lang_texts[19][0]);
	return 1;
}

stock fights_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	new string[MAX_STRING];
	new TrenerID = GetPlayerFightTrenerID(playerid);
	if (FightTreners[TrenerID][Style] <= GetPlayerFightStyle(playerid))
	{
		format(string, sizeof(string), lang_texts[19][1], FightTreners[TrenerID][StyleName]);
		return show_Msg_Dialog(playerid, lang_texts[19][2], string, lang_texts[19][3]);
	}
	if (FightTreners[TrenerID][Style]-1 > GetPlayerFightStyle(playerid))
	{
		return show_Msg_Dialog(playerid, lang_texts[19][2], lang_texts[19][4], lang_texts[19][3]);
	}
	format(string, sizeof(string), lang_texts[19][5], FightTreners[TrenerID][StyleName], FightTreners[TrenerID][Cost]);
	ShowPlayerDialog(playerid, fightstyles_DialogID, DIALOG_STYLE_MSGBOX, lang_texts[19][6], string, lang_texts[19][7], lang_texts[19][8]);
	return 1;
}

stock fights_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
    new string[MAX_STRING];
	if (dialogid == fightstyles_DialogID)
	{
	    if (!response) return 1;
		new TrenerID = GetPlayerFightTrenerID(playerid);
		if (oGetPlayerMoney(playerid) < FightTreners[TrenerID][Cost])
		{
			format(string, sizeof(string), lang_texts[19][9], FightTreners[TrenerID][Cost]);
			return show_Msg_Dialog(playerid, lang_texts[19][2], string, lang_texts[19][3]);
		}
		SetPlayerFightStyle(playerid, FightTreners[TrenerID][Style]);
		oGivePlayerMoney(playerid,-FightTreners[TrenerID][Cost], 0);
		format(string, sizeof(string), lang_texts[19][10], FightTreners[TrenerID][StyleName], FightTreners[TrenerID][Cost]);
		show_Msg_Dialog(playerid, lang_texts[19][2], string, lang_texts[19][3]);
		return 1;
	}
	else if (dialogid == fightstyles_user_DialogID)
	{
		if (!response)
		{
			return show_User_Menu(playerid);
		}
		listitem += 4; // теперь listitem является идом выбранного стиля
		SetPlayerFightStyleUsed(playerid, listitem);
		SetPlayerFightingStyle(playerid, listitem);
		format(string, sizeof(string), lang_texts[19][11], ConvertFightIdToName( GetPlayerFightStyleUsed(playerid) ));
		show_User_Menu_Dialog(playerid, lang_texts[19][2], string);
		return 1;
	}
	return 1;
}

stock GetPlayerFightTrenerID(playerid)
{
	for (new fight_id = 1; fight_id < sizeof(FightTreners); fight_id++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2.0, FightTreners[fight_id][Coord_X], FightTreners[fight_id][Coord_Y], FightTreners[fight_id][Coord_Z]))
		{
			return fight_id;
		}
	}
	return -1;
}

// выбор стиля в меню игрока
stock showFightStyleMenu(playerid)
{
	new string[MAX_STRING];
	if (GetPlayerFightStyle(playerid) >= FIGHT_STYLE_NORMAL) strcat(string, lang_texts[19][15]);
	if (GetPlayerFightStyle(playerid) >= FIGHT_STYLE_BOXING) strcat(string, lang_texts[19][16]);
	if (GetPlayerFightStyle(playerid) >= FIGHT_STYLE_KUNGFU) strcat(string, lang_texts[19][18]);
	if (GetPlayerFightStyle(playerid) >= FIGHT_STYLE_KNEEHEAD) strcat(string, lang_texts[19][17]);
	return ShowPlayerDialog(
		playerid, fightstyles_user_DialogID, DIALOG_STYLE_LIST,
		lang_texts[19][12],
		string,
		lang_texts[19][13], lang_texts[19][14]
	);
}
//

stock ConvertFightIdToName(fstyleid)
{
	new fname[MAX_STRING];
	switch (fstyleid)
	{
		case FIGHT_STYLE_NORMAL: set(fname, FightTreners[0][StyleName]);
		case FIGHT_STYLE_BOXING: set(fname, FightTreners[1][StyleName]);
		case FIGHT_STYLE_KUNGFU: set(fname, FightTreners[3][StyleName]);
		case FIGHT_STYLE_KNEEHEAD: set(fname, FightTreners[2][StyleName]);
	}
	return fname;
}
