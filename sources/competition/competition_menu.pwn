/*
	
	About: competition menu
	Author: ziggi

*/

#if defined _competition_menu_included
	#endinput
#endif

#define _competition_menu_included

/*
	CompetitionMenu
*/

DialogCreate:CompetitionMenu(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * (2 + MAX_COMPETITION_CURRENT) + 1],
		temp[MAX_LANG_VALUE_STRING];

	__(COMPETITION_MENU_LIST_HEADER, temp);
	strcat(string, temp);

	__(COMPETITION_MENU_LIST_START, temp);
	strcat(string, temp);

	new
		type_name[MAX_LANG_VALUE_STRING],
		type_color[7],
		competition_name[MAX_LANG_VALUE_STRING],
		competition_time,
		competition_time_string[MAX_LANG_VALUE_STRING],
		competition_status[MAX_LANG_VALUE_STRING];

	foreach (new cid : CurrentCompetition) {
		CompetitionType_GetName(Competition_GetType(cid), type_name);
		CompetitionType_GetColorCode(Competition_GetType(cid), type_color);
		Competition_GetName(cid, competition_name);
		competition_time = Competition_GetTime(cid);

		if (competition_time > 0) {
			__(COMPETITION_MENU_STATUS_ENTER, competition_status);
			GetTimeStringFromSeconds(competition_time, competition_time_string);

			format(competition_status, sizeof(competition_status), competition_status, competition_time_string);
		} else {
			__(COMPETITION_MENU_STATUS_STARTED, competition_status);
			GetTimeStringFromSeconds(-competition_time, competition_time_string);

			format(competition_status, sizeof(competition_status), competition_status, competition_time_string);
		}

		__(COMPETITION_MENU_LIST_ITEM, temp);
		format(temp, sizeof(temp), temp, competition_name, type_color, type_name, competition_status);
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
		Disalog_Show(playerid, Dialog:CompetitionStartMenu);
		return 1;
	}

	new
		string[MAX_LANG_MULTI_STRING],
		cid,
		cname[MAX_COMPETITION_NAME],
		ctype,
		ctime,
		ctype_name[MAX_COMPETITION_TYPE_NAME],
		ctype_color_code[7];

	cid = listitem - 1;
	Competition_GetName(cid, cname);
	ctype = Competition_GetType(cid);
	ctime = Competition_GetTime(cid);
	CompetitionType_GetName(ctype, ctype_name);
	CompetitionType_GetColorCode(ctype, ctype_color_code);

	format(string, sizeof(string), _(COMPETITION_JOIN_MENU_MSG), cname, ctype_color_code, ctype_name),

	Dialog_Open(playerid, Dialog:CompetitionJoinMenu, DIALOG_STYLE_MSGBOX,
			cname,
			string,
			_(COMPETITION_MENU_JOIN), _(COMPETITION_MENU_BACK)
		);
	return 1;
}

DialogResponse:CompetitionJoinMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
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
