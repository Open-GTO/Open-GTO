/*
	Created: 04.06.11
	Aurthor: ZiGGi
*/

#if defined _bar_included
	#endinput
#endif

#define _bar_included
#pragma library bar


new Bars[][CoordInfo] = {
	{495.3609, -76.0381, 998.7578}, // green bar
	{681.5761, -453.8308, -25.6172}, // Welcome Pump(Dillimore)
	{-2654.0112, 1413.2083, 906.2771}, // Jizzy's
	{1955.3743, 1017.9493, 992.4688}, // 4 Dragons Casino
	{1139.7212, -4.2430, 1000.6719}, // Redsands Casino
	{-224.7835, 1407.4834, 27.7734} // Lil' Probe Inn
};

enum DrinkInfo {
	Name[MAX_NAME],
	Cost,
	Float:Hp,
	Float:Alcohol,
	Action,
}
new Drinks[][DrinkInfo] = {
	{"Водка Зелёная марка", 100, 30.0, 40.0, SPECIAL_ACTION_DRINK_SPRUNK},
	{"Пиво Жигулёвское", 30, 10.0, 4.5, SPECIAL_ACTION_DRINK_BEER},
	{"Глинтвейн", 30, 10.0, 10.0, SPECIAL_ACTION_DRINK_WINE},
	{"Чай с лимоном и сахаром", 15, 10.0, -20.0, SPECIAL_ACTION_DRINK_SPRUNK}
};

stock bar_OnGameModeInit()
{
	for (new b_id = 0; b_id < sizeof(Bars); b_id++)
	{
	    CreatePickup(1551, 23, Bars[b_id][Coord_X], Bars[b_id][Coord_Y], Bars[b_id][Coord_Z]);
		CreatePickupText("Бар", Bars[b_id][Coord_X], Bars[b_id][Coord_Y], Bars[b_id][Coord_Z]);
	}
	GameMSG(lang_texts[17][12]);
	return 1;
}

stock bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	if ( PRESSED ( KEY_USING ) )
	{
		return bar_ShowDialog(playerid);
	}
	if (GetPVarInt(playerid, "bar_Drinking") == 1 && GetPVarInt(playerid, "bar_Gulping") == 0)
	{
		GivePVarInt(playerid, "bar_Gulps", 1);
		SetPVarInt(playerid, "bar_Gulping", 1);
		SetTimerEx("bar_Gulp", 2000, 0, "d", playerid);
		return 1;
	}
	return 1;
}

forward bar_Gulp(playerid);
public bar_Gulp(playerid)
{
	if (GetPVarInt(playerid, "bar_Gulps") >= MAX_GULPS)
	{
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		SetPVarInt(playerid, "bar_Drinking", 0);
		SetPVarInt(playerid, "bar_Gulps", 0);
		SetPVarInt(playerid, "bar_Gulping", 0);
	}
	SetPVarInt(playerid, "bar_Gulping", 0);
	return 1;
}

stock bar_ShowDialog(playerid)
{
	new string[256];
	for(new i = 0; i < sizeof(Drinks); i++)
	{
		format(string, sizeof(string),
			lang_texts[17][13],
			string, Drinks[i][Name], Drinks[i][Cost], Drinks[i][Alcohol], Drinks[i][Hp]
		);
	}
	return ShowPlayerDialog(playerid, bar_DialogID, DIALOG_STYLE_LIST,
		lang_texts[17][14],
		string,
		lang_texts[17][15], lang_texts[17][16]
	);
}

stock bar_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	if (dialogid == bar_DialogID)
	{
		if (!response) return 1;
		if (oGetPlayerMoney(playerid) < Drinks[listitem][Cost])
		{
			return show_Msg_Dialog(playerid, lang_texts[17][14], lang_texts[17][17], lang_texts[17][18]);
		}
		oGivePlayerMoney(playerid,-Drinks[listitem][Cost], 1);
		SetPlayerSpecialAction(playerid, Drinks[listitem][Action]);
		SetPVarInt(playerid, "bar_Drinking", 1);
		SetPlayerDrunkLevel(playerid, GetPlayerDrunkLevel(playerid) + floatround(Drinks[listitem][Alcohol]*100, floatround_round));
		if(Drinks[listitem][Hp] + oGetPlayerHealth(playerid) > GetMaxHealth(playerid))
		{
			oSetPlayerHealth(playerid, GetMaxHealth(playerid));
		}
		else
		{
			oSetPlayerHealth(playerid, oGetPlayerHealth(playerid) + Drinks[listitem][Hp]);
		}
		new string[MAX_STRING];
		format(string, sizeof(string),
			lang_texts[17][19],
			Drinks[listitem][Name], Drinks[listitem][Cost], Drinks[listitem][Alcohol], Drinks[listitem][Hp]
		);
		show_Msg_Dialog(playerid, lang_texts[17][14], string, lang_texts[17][18]);
		return 1;
	}
	return 1;
}

stock IsPlayerAtBar(playerid)
{
	for (new b_id = 0; b_id < sizeof(Bars); b_id++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2, Bars[b_id][Coord_X], Bars[b_id][Coord_Y], Bars[b_id][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}
