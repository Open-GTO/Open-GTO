/*
	
	About: competition menu
	Author: ziggi

*/

#if defined _competition_menu_included
	#endinput
#endif

#define _competition_menu_included

/*
	Vars
*/

static
	gPlayerJoinCompetitionID[MAX_PLAYERS];

/*
	CompetitionMenu
*/

DialogCreate:CompetitionMenu(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * (2 + MAX_COMPETITION) + 1],
		temp[MAX_LANG_VALUE_STRING];

	__(COMPETITION_MENU_LIST_HEADER, temp);
	strcat(string, temp);

	__(COMPETITION_MENU_LIST_START, temp);
	strcat(string, temp);

	new
		cname[COMPETITION_MAX_STRING],
		ctime,
		cstatus[MAX_LANG_VALUE_STRING],
		ctype,
		ctype_name[COMPETITION_MAX_STRING],
		ctype_color,
		ctype_color_code[7];

	foreach (new cid : CompetitionIterator) {
		Competition_GetParamString(ctype, COMPETITION_NAME, cname);
		ctime = Competition_GetParamInt(cid, COMPETITION_TIME);
		ctype = Competition_GetParamInt(cid, COMPETITION_TYPE);
		CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
		ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);
		GetColorEmbeddingCode(ctype_color, ctype_color_code);

		if (ctime > 0) {
			GetTimeStringFromSeconds(ctime, cstatus);
			format(cstatus, sizeof(cstatus), _(COMPETITION_MENU_STATUS_ENTER), cstatus);
		} else {
			GetTimeStringFromSeconds(-ctime, cstatus);
			format(cstatus, sizeof(cstatus), _(COMPETITION_MENU_STATUS_STARTED), cstatus);
		}

		__(COMPETITION_MENU_LIST_ITEM, temp);
		format(temp, sizeof(temp), temp, cname, ctype_color, ctype_name, cstatus);
		strcat(string, temp);
	}

	Dialog_Open(playerid, Dialog:CompetitionMenu, DIALOG_STYLE_TABLIST_HEADERS,
			_(COMPETITION_MENU_HEADER),
			string,
			_(COMPETITION_MENU_SELECT), _(COMPETITION_MENU_BACK)
		);
}

DialogResponse:CompetitionMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	// if start item
	if (listitem == 0) {
		Dialog_Show(playerid, Dialog:CompetitionStartMenu);
		return 1;
	}

	// if join item
	new
		string[MAX_LANG_MULTI_STRING],
		cid,
		cname[COMPETITION_MAX_STRING],
		ctime,
		cstatus[MAX_LANG_VALUE_STRING],
		ctype,
		ctype_name[COMPETITION_MAX_STRING],
		ctype_color,
		ctype_color_code[7];

	cid = GetCompetitionIdByListitem(listitem, 1);
	if (cid == INVALID_COMPETITION_ID) {
		Log_Debug("Error <DialogResponse:CompetitionMenu>: unable to get competition id by listitem.");
		return 0;
	}

	Competition_GetParamString(ctype, COMPETITION_NAME, cname);
	ctime = Competition_GetParamInt(cid, COMPETITION_TIME);
	ctype = Competition_GetParamInt(cid, COMPETITION_TYPE);
	CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
	ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);
	GetColorEmbeddingCode(ctype_color, ctype_color_code);

	if (ctime > 0) {
		GetTimeStringFromSeconds(ctime, cstatus);
		format(cstatus, sizeof(cstatus), _(COMPETITION_MENU_STATUS_ENTER), cstatus);
	} else {
		GetTimeStringFromSeconds(-ctime, cstatus);
		format(cstatus, sizeof(cstatus), _(COMPETITION_MENU_STATUS_STARTED), cstatus);
	}

	format(string, sizeof(string), _(COMPETITION_JOIN_MENU_MSG), cname, ctype_color_code, ctype_name, cstatus);

	if (Competition_IsPlayerCanJoin(cid, playerid)) {
		SetJoinCompetitionId(playerid, INVALID_COMPETITION_ID);

		Dialog_Open(playerid, Dialog:CompetitionJoinMenu, DIALOG_STYLE_MSGBOX,
				cname,
				string,
				_(COMPETITION_MENU_JOIN), _(COMPETITION_MENU_BACK)
			);
	} else {
		SetJoinCompetitionId(playerid, cid);

		Dialog_Open(playerid, Dialog:CompetitionJoinMenu, DIALOG_STYLE_MSGBOX,
				cname,
				string,
				_(COMPETITION_MENU_BACK), ""
			);
	}
	return 1;
}

DialogResponse:CompetitionJoinMenu(playerid, response, listitem, inputtext[])
{
	new cid = GetJoinCompetitionId(playerid);

	if (!response || !Competition_IsPlayerCanJoin(cid, playerid)) {
		Dialog_Show(playerid, Dialog:CompetitionMenu);
		return 1;
	}

	// join to competition

	return 1;
}

DialogCreate:CompetitionStartMenu(playerid)
{
	return 1;
}

DialogResponse:CompetitionStartMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:CompetitionMenu);
	}

	return 1;
}

/*
	JoinCompetition ID
*/

static stock SetJoinCompetitionId(playerid, cid)
{
	gPlayerJoinCompetitionID[playerid] = cid;
}

static stock GetJoinCompetitionId(playerid)
{
	return gPlayerJoinCompetitionID[playerid];
}

/*
	Get competition id by listitem
*/

static stock GetCompetitionIdByListitem(listitem, offset)
{
	new
		cid,
		index;

	index += offset;

	foreach (cid : CompetitionIterator) {
		if (index == listitem) {
			return cid;
		}

		index++;
	}

	return INVALID_COMPETITION_ID;
}
