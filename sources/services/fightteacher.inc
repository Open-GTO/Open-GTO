/*

	Created: 29.05.2010
	Author: ziggi

*/

#if defined _fightteacher_included
	#endinput
#endif

#define _fightteacher_included
#pragma library fightteacher


enum FightInfo {
	StyleID,
	StyleName[MAX_STRING],
	bool:IsHaveTeacher,
	Cost,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z,
	Checkpoint
};

new fights_Teachers[][FightInfo] = {
	{FIGHT_STYLE_NORMAL, "нормальный", false},
	{FIGHT_STYLE_BOXING, "бокс", true, 1000, 767.6851, 12.8269, 1000.7025}, // LS
	{FIGHT_STYLE_KUNGFU, "кунг-фу", true, 2000, 768.5967,-22.9764, 1000.5859}, // SF
	{FIGHT_STYLE_KNEEHEAD, "колено-голова", true, 3000, 766.5240,-76.6523, 1000.6563} // LV
};

stock fights_OnGameModeInit()
{
	for (new id = 0; id < sizeof(fights_Teachers); id++) {
		if (fights_Teachers[id][IsHaveTeacher] == false) {
			continue;
		}

		fights_Teachers[id][Checkpoint] = Checkpoint_Create(fights_Teachers[id][Coord_X], fights_Teachers[id][Coord_Y], fights_Teachers[id][Coord_Z], 1.5, .streamdistance = 20.0);
	}
	Log_Game(lang_texts[19][0]);
	return 1;
}

stock fights_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	new string[MAX_STRING],
		teacherid = fights_GetPlayerTeacherID(playerid);
	
	if (fights_Teachers[teacherid][StyleID] <= pl_fights_GetPlayerStyle(playerid)) {
		format(string, sizeof(string), lang_texts[19][1], fights_Teachers[teacherid][StyleName]);
		Dialog_Message(playerid, lang_texts[19][2], string, lang_texts[19][3]);
		return 0;
	}

	if (fights_Teachers[teacherid][StyleID] - 1 > pl_fights_GetPlayerStyle(playerid)) {
		Dialog_Message(playerid, lang_texts[19][2], lang_texts[19][4], lang_texts[19][3]);
		return 0;
	}

	Dialog_Show(playerid, Dialog:ServiceFights);
	return 1;
}

stock fights_OnPlayerEnterCheckpoint(playerid, cp)
{
	new
		string[MAX_STRING],
		teacherid = fights_GetPlayerTeacherID(playerid);

	for (new id = 0; id < sizeof(fights_Teachers); id++) {
		if (cp == fights_Teachers[id][Checkpoint]) {
			if (fights_Teachers[teacherid][StyleID] <= pl_fights_GetPlayerStyle(playerid)) {
				format(string, sizeof(string), lang_texts[19][1], fights_Teachers[teacherid][StyleName]);
				Dialog_Message(playerid, lang_texts[19][2], string, lang_texts[19][3]);
				return 1;
			}

			if (fights_Teachers[teacherid][StyleID] - 1 > pl_fights_GetPlayerStyle(playerid)) {
				Dialog_Message(playerid, lang_texts[19][2], lang_texts[19][4], lang_texts[19][3]);
				return 1;
			}

			Dialog_Show(playerid, Dialog:ServiceFights);
			return 1;
		}
	}
	return 0;
}

DialogCreate:ServiceFights(playerid)
{
	new string[MAX_STRING],
		teacherid = fights_GetPlayerTeacherID(playerid);
	
	format(string, sizeof(string), lang_texts[19][5], fights_Teachers[teacherid][StyleName], fights_Teachers[teacherid][Cost]);
	
	Dialog_Open(playerid, Dialog:ServiceFights, DIALOG_STYLE_MSGBOX, lang_texts[19][6], string, lang_texts[19][7], lang_texts[19][8]);
}

DialogResponse:ServiceFights(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}

	new string[MAX_STRING],
		teacherid = fights_GetPlayerTeacherID(playerid);

	if (oGetPlayerMoney(playerid) < fights_Teachers[teacherid][Cost]) {
		format(string, sizeof(string), lang_texts[19][9], fights_Teachers[teacherid][Cost]);
		Dialog_Message(playerid, lang_texts[19][2], string, lang_texts[19][3]);
		return 0;
	}

	pl_fights_SetPlayerStyle(playerid, fights_Teachers[teacherid][StyleID]);
	oGivePlayerMoney(playerid, -fights_Teachers[teacherid][Cost], 0);

	format(string, sizeof(string), lang_texts[19][10], fights_Teachers[teacherid][StyleName], fights_Teachers[teacherid][Cost]);
	Dialog_Message(playerid, lang_texts[19][2], string, lang_texts[19][3]);
	return 1;
}

stock fights_GetPlayerTeacherID(playerid)
{
	for (new id = 0; id < sizeof(fights_Teachers); id++) {
		if (fights_Teachers[id][IsHaveTeacher] == false) {
			continue;
		}

		if (IsPlayerInRangeOfPoint(playerid, 2.0, fights_Teachers[id][Coord_X], fights_Teachers[id][Coord_Y], fights_Teachers[id][Coord_Z])) {
			return id;
		}
	}
	return -1;
}

stock fights_IsPlayerAtTeacher(playerid)
{
	if (fights_GetPlayerTeacherID(playerid) == -1) {
		return 0;
	}
	return 1;
}

stock fights_GetNameByID(fstyleid, fstylename[MAX_STRING])
{
	if (fstyleid < 0 || fstyleid > sizeof(fights_Teachers) - 1) {
		return 0;
	}

	set(fstylename, fights_Teachers[fstyleid][StyleName]);
	return 1;
}

stock fights_GetNameByLevel(fstylelevel, fstylename[MAX_STRING])
{
	return fights_GetNameByID(fstylelevel - 4, fstylename);
}