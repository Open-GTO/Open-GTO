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
	gPlayerStartParams[MAX_PLAYERS][CompetitionParams],
	gPlayerJoinCompetitionID[MAX_PLAYERS],
	gAvailableWeather[] = {0, 1, 4, 8, 9, 19, 20};

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	gPlayerStartParams[playerid][COMPETITION_TYPE] = INVALID_COMPETITION_TYPE_ID;
	gPlayerStartParams[playerid][COMPETITION_WEATHER] = INVALID_WEATHER_ID;

	#if defined CompMenu_OnPlayerConnect
		return CompMenu_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect CompMenu_OnPlayerConnect
#if defined CompMenu_OnPlayerConnect
	forward CompMenu_OnPlayerConnect(playerid);
#endif

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
	new
		string[MAX_LANG_VALUE_STRING * 10 + 1],
		temp[MAX_LANG_VALUE_STRING];

	// type
	new
		ctype,
		ctype_name[MAX_COMPETITION_TYPE_NAME],
		ctype_color,
		ctype_color_code[7];

	ctype = gPlayerStartParams[playerid][COMPETITION_TYPE];

	if (ctype == INVALID_COMPETITION_TYPE_ID) {
		__(COMPETITION_START_PARAM_RANDOM, ctype_name);
		GetColorEmbeddingCode(COLOR_BLUEGREY_200, ctype_color_code);
	} else {
		CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
		ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);

		GetColorEmbeddingCode(ctype_color, ctype_color_code);
	}
	format(temp, sizeof(temp), _(COMPETITION_START_PARAM_TYPE), ctype_color_code, ctype_name);
	strcat(string, temp);

	// weather
	new
		cweather,
		cweather_string[MAX_WEATHER_NAME],
		cweather_color,
		cweather_color_code[7];

	cweather = gPlayerStartParams[playerid][COMPETITION_WEATHER];

	if (cweather == INVALID_WEATHER_ID) {
		__(COMPETITION_START_PARAM_RANDOM, cweather_string);
		cweather_color = COLOR_BLUEGREY_200;
	} else {
		Weather_GetName(cweather, cweather_string);
		cweather_color = COLOR_TEAL_400;
	}
	GetColorEmbeddingCode(cweather_color, cweather_color_code);
	format(temp, sizeof(temp), _(COMPETITION_START_PARAM_WEATHER), cweather_color_code, cweather_string);
	strcat(string, temp);

	// open dialog
	Dialog_Open(playerid, Dialog:CompetitionStartMenu, DIALOG_STYLE_TABLIST,
			_(COMPETITION_MENU_HEADER),
			string,
			_(COMPETITION_MENU_SELECT), _(COMPETITION_MENU_BACK)
		);
	return 1;
}

DialogResponse:CompetitionStartMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:CompetitionMenu);
		return 1;
	}

	switch (listitem) {
		// type
		case 0: {
			Dialog_Show(playerid, Dialog:CompetitionStartTypeMenu);
		}
		// weather
		case 1: {
			Dialog_Show(playerid, Dialog:CompetitionStartWeatherMenu);
		}
	}

	return 1;
}

DialogCreate:CompetitionStartTypeMenu(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * MAX_COMPETITION_TYPE_NAME + 1],
		ctype_name[MAX_COMPETITION_TYPE_NAME],
		ctype_color,
		ctype_color_code[7];

	__(COMPETITION_START_PARAM_RANDOM, string);
	strcat(string, "\n");

	foreach (new ctype : CompetitionTypeIterator) {
		CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
		ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);

		GetColorEmbeddingCode(ctype_color, ctype_color_code);

		format(string, sizeof(string), "%s{%s}%s\n", string, ctype_color_code, ctype_name);
	}

	Dialog_Open(playerid, Dialog:CompetitionStartTypeMenu, DIALOG_STYLE_LIST,
			_(COMPETITION_START_TYPE_HEADER),
			string,
			_(COMPETITION_MENU_SELECT), _(COMPETITION_MENU_BACK)
		);
}

DialogResponse:CompetitionStartTypeMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:CompetitionStartMenu);
		return 1;
	}

	if (listitem == 0) {
		gPlayerStartParams[playerid][COMPETITION_TYPE] = INVALID_COMPETITION_TYPE_ID;
	} else {
		gPlayerStartParams[playerid][COMPETITION_TYPE] = GetCompetitionTypeByListitem(listitem, 1);
	}

	Dialog_Show(playerid, Dialog:CompetitionStartMenu);
	return 1;
}

DialogCreate:CompetitionStartWeatherMenu(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * sizeof(gAvailableWeather) + 1],
		cweather_name[MAX_WEATHER_NAME];

	__(COMPETITION_START_PARAM_RANDOM, string);
	strcat(string, "\n");

	for (new i = 0; i < sizeof(gAvailableWeather); i++) {
		Weather_GetName(gAvailableWeather[i], cweather_name);
		strcat(string, cweather_name);
		strcat(string, "\n");
	}

	Dialog_Open(playerid, Dialog:CompetitionStartWeatherMenu, DIALOG_STYLE_LIST,
			_(COMPETITION_START_TYPE_HEADER),
			string,
			_(COMPETITION_MENU_SELECT), _(COMPETITION_MENU_BACK)
		);
}

DialogResponse:CompetitionStartWeatherMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:CompetitionStartMenu);
		return 1;
	}

	if (listitem == 0) {
		gPlayerStartParams[playerid][COMPETITION_WEATHER] = INVALID_WEATHER_ID;
	} else {
		gPlayerStartParams[playerid][COMPETITION_WEATHER] = GetCompetitionWeatherByListitem(listitem, 1);
	}

	Dialog_Show(playerid, Dialog:CompetitionStartMenu);
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

static stock GetCompetitionIdByListitem(listitem, offset = 0)
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

/*
	Get competition type by listitem
*/

static stock GetCompetitionTypeByListitem(listitem, offset = 0)
{
	new
		ctype,
		index;

	index += offset;

	foreach (ctype : CompetitionTypeIterator) {
		if (index == listitem) {
			return ctype;
		}

		index++;
	}

	return INVALID_COMPETITION_TYPE_ID;
}

/*
	Get competition weather by listitem
*/

static stock GetCompetitionWeatherByListitem(listitem, offset = 0)
{
	new
		i,
		index;

	index += offset;

	for (i = 0; i < sizeof(gAvailableWeather); i++) {
		if (index == listitem) {
			return gAvailableWeather[i];
		}

		index++;
	}

	return INVALID_WEATHER_ID;
}
