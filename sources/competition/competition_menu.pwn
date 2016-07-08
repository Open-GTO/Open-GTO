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
	gPlayerStartParams[playerid][COMPETITION_PLAYERID] = playerid;
	gPlayerStartParams[playerid][COMPETITION_TYPE] = INVALID_COMPETITION_TYPE_ID;
	gPlayerStartParams[playerid][COMPETITION_MAP] = INVALID_COMPETITION_MAP_ID;
	gPlayerStartParams[playerid][COMPETITION_TIME] = COMPETITION_DEFAULT_TIME;
	gPlayerStartParams[playerid][COMPETITION_JOIN_STATUS] = _:CompetitionJoinStatusAll;
	gPlayerStartParams[playerid][COMPETITION_MARKER_STATUS] = _:CompetitionMarkerStatusAll;
	gPlayerStartParams[playerid][COMPETITION_NAMETAG_STATUS] = _:CompetitionNametagStatusAll;
	gPlayerStartParams[playerid][COMPETITION_WEATHER] = INVALID_WEATHER_ID;
	gPlayerStartParams[playerid][COMPETITION_WORLD_TIME] = 12;
	gPlayerStartParams[playerid][COMPETITION_LEVEL_MIN] = GetMinPlayerLevel();
	gPlayerStartParams[playerid][COMPETITION_LEVEL_MAX] = GetMaxPlayerLevel();
	gPlayerStartParams[playerid][COMPETITION_COLLISION_STATUS] = _:CompetitionCollisionStatusAll;
	gPlayerStartParams[playerid][COMPETITION_FRIENDLY_FIRE] = true;

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

	__(playerid, COMPETITION_MENU_LIST_HEADER, temp);
	strcat(string, temp);

	__(playerid, COMPETITION_MENU_LIST_START, temp);
	strcat(string, temp);

	new
		cmap,
		cmap_name[COMPETITION_MAX_STRING],
		ctime,
		cstatus[MAX_LANG_VALUE_STRING],
		ctype,
		ctype_name[COMPETITION_MAX_STRING],
		ctype_color,
		ctype_color_code[7];

	foreach (new cid : CompetitionIterator) {
		cmap = Competition_GetParamInt(cid, COMPETITION_MAP);
		CompetitionMap_GetParamString(cmap, COMPETITION_MAP_NAME, cmap_name);
		ctime = Competition_GetParamInt(cid, COMPETITION_TIME);
		ctype = Competition_GetParamInt(cid, COMPETITION_TYPE);
		CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
		ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);
		GetColorEmbeddingCode(ctype_color, ctype_color_code);

		if (ctime > 0) {
			GetTimeStringFromSeconds(playerid, ctime, cstatus);
			format(cstatus, sizeof(cstatus), _(playerid, COMPETITION_MENU_STATUS_ENTER), cstatus);
		} else {
			GetTimeStringFromSeconds(playerid, -ctime, cstatus);
			format(cstatus, sizeof(cstatus), _(playerid, COMPETITION_MENU_STATUS_STARTED), cstatus);
		}

		format(temp, sizeof(temp), _(playerid, COMPETITION_MENU_LIST_ITEM), cmap_name, ctype_color_code, ctype_name, cstatus);
		strcat(string, temp);
	}

	Dialog_Open(playerid, Dialog:CompetitionMenu, DIALOG_STYLE_TABLIST_HEADERS,
			_(playerid, COMPETITION_MENU_HEADER),
			string,
			_(playerid, COMPETITION_MENU_SELECT), _(playerid, COMPETITION_MENU_BACK)
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
		if (Competition_GetFreeSlot() == INVALID_COMPETITION_ID) {
			Dialog_MessageEx(playerid, Dialog:CompetitionReturnMenu,
					_(playerid, COMPETITION_MENU_HEADER),
					_(playerid, COMPETITION_MENU_NO_FREE_SLOT),
					_(playerid, COMPETITION_MENU_BACK), _(playerid, COMPETITION_MENU_CANCEL)
				);
		} else {
			Dialog_Show(playerid, Dialog:CompetitionStartMenu);
		}
		return 1;
	}

	// if join item
	new
		string[MAX_LANG_MULTI_STRING],
		cid,
		cplayer_id,
		cplayer_name[MAX_PLAYER_NAME + 1],
		cmap,
		cmap_name[COMPETITION_MAX_STRING],
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

	cplayer_id = Competition_GetParamInt(cid, COMPETITION_PLAYERID);
	GetPlayerName(cplayer_id, cplayer_name, sizeof(cplayer_name));
	cmap = Competition_GetParamInt(cid, COMPETITION_MAP);
	CompetitionMap_GetParamString(cmap, COMPETITION_MAP_NAME, cmap_name);
	ctime = Competition_GetParamInt(cid, COMPETITION_TIME);
	ctype = Competition_GetParamInt(cid, COMPETITION_TYPE);
	CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
	ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);
	GetColorEmbeddingCode(ctype_color, ctype_color_code);

	if (ctime > 0) {
		GetTimeStringFromSeconds(playerid, ctime, cstatus);
		format(cstatus, sizeof(cstatus), _(playerid, COMPETITION_MENU_STATUS_ENTER), cstatus);
	} else {
		GetTimeStringFromSeconds(playerid, -ctime, cstatus);
		format(cstatus, sizeof(cstatus), _(playerid, COMPETITION_MENU_STATUS_STARTED), cstatus);
	}

	format(string, sizeof(string), _(playerid, COMPETITION_JOIN_MENU_MSG), cmap_name, ctype_color_code, ctype_name, cplayer_name, cplayer_id, cstatus);

	if (Competition_IsPlayerCanJoin(cid, playerid)) {
		SetJoinCompetitionId(playerid, cid);

		Dialog_Open(playerid, Dialog:CompetitionJoinMenu, DIALOG_STYLE_MSGBOX,
				cmap_name,
				string,
				_(playerid, COMPETITION_MENU_JOIN), _(playerid, COMPETITION_MENU_BACK)
			);
	} else {
		SetJoinCompetitionId(playerid, INVALID_COMPETITION_ID);

		Dialog_Open(playerid, Dialog:CompetitionJoinMenu, DIALOG_STYLE_MSGBOX,
				cmap_name,
				string,
				_(playerid, COMPETITION_MENU_BACK), ""
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
	CompetitionType_OnJoin(Competition_GetParamInt(cid, COMPETITION_TYPE), cid, playerid);

	return 1;
}

DialogCreate:CompetitionStartMenu(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * 10 + 1],
		temp[MAX_LANG_VALUE_STRING];

	// start
	__(playerid, COMPETITION_START_START, string);

	// type
	new
		ctype,
		ctype_name[MAX_COMPETITION_TYPE_NAME],
		ctype_color,
		ctype_color_code[7];

	ctype = gPlayerStartParams[playerid][COMPETITION_TYPE];

	if (ctype == INVALID_COMPETITION_TYPE_ID) {
		__(playerid, COMPETITION_START_PARAM_RANDOM, ctype_name);
		GetColorEmbeddingCode(COLOR_BLUEGREY_200, ctype_color_code);
	} else {
		CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
		ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);

		GetColorEmbeddingCode(ctype_color, ctype_color_code);
	}
	format(temp, sizeof(temp), _(playerid, COMPETITION_START_PARAM_TYPE), ctype_color_code, ctype_name);
	strcat(string, temp);

	// map
	new
		cmap,
		cmap_name[COMPETITION_MAX_STRING],
		cmap_color,
		cmap_color_code[7];

	cmap = gPlayerStartParams[playerid][COMPETITION_MAP];

	if (cmap == INVALID_COMPETITION_MAP_ID) {
		__(playerid, COMPETITION_START_PARAM_RANDOM, cmap_name);
		cmap_color = COLOR_BLUEGREY_200;
	} else {
		CompetitionMap_GetParamString(cmap, COMPETITION_MAP_NAME, cmap_name);
		cmap_color = COLOR_TEAL_400;
	}
	GetColorEmbeddingCode(cmap_color, cmap_color_code);
	format(temp, sizeof(temp), _(playerid, COMPETITION_START_PARAM_MAP), cmap_color_code, cmap_name);
	strcat(string, temp);

	// weather
	new
		cweather,
		cweather_string[MAX_WEATHER_NAME],
		cweather_color,
		cweather_color_code[7];

	cweather = gPlayerStartParams[playerid][COMPETITION_WEATHER];

	if (cweather == INVALID_WEATHER_ID) {
		__(playerid, COMPETITION_START_PARAM_RANDOM, cweather_string);
		cweather_color = COLOR_BLUEGREY_200;
	} else {
		Weather_GetName(cweather, Lang_GetPlayerLangType(playerid), cweather_string);
		cweather_color = COLOR_TEAL_400;
	}
	GetColorEmbeddingCode(cweather_color, cweather_color_code);
	format(temp, sizeof(temp), _(playerid, COMPETITION_START_PARAM_WEATHER), cweather_color_code, cweather_string);
	strcat(string, temp);

	// open dialog
	Dialog_Open(playerid, Dialog:CompetitionStartMenu, DIALOG_STYLE_TABLIST,
			_(playerid, COMPETITION_MENU_HEADER),
			string,
			_(playerid, COMPETITION_MENU_SELECT), _(playerid, COMPETITION_MENU_BACK)
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
		// start
		case 0: {
			new
				start_params[CompetitionParams];

			start_params = gPlayerStartParams[playerid];

			// random type
			if (start_params[COMPETITION_TYPE] == INVALID_COMPETITION_TYPE_ID) {
				start_params[COMPETITION_TYPE] = CompetitionType_GetRandom();
			}

			// random map
			if (start_params[COMPETITION_MAP] == INVALID_COMPETITION_MAP_ID) {
				start_params[COMPETITION_MAP] = CompetitionMap_GetRandom();
			}

			// random weather
			if (start_params[COMPETITION_WEATHER] == INVALID_WEATHER_ID) {
				new idx = random(sizeof(gAvailableWeather));
				start_params[COMPETITION_WEATHER] = gAvailableWeather[idx];
			}

			// start
			new cid = Competition_Add(start_params);
			CompetitionType_OnAdd(start_params[COMPETITION_TYPE], cid, playerid);
		}
		// type
		case 1: {
			Dialog_Show(playerid, Dialog:CompetitionStartTypeMenu);
		}
		// map
		case 2: {
			Dialog_Show(playerid, Dialog:CompetitionStartMapMenu);
		}
		// weather
		case 3: {
			Dialog_Show(playerid, Dialog:CompetitionStartWeatherMenu);
		}
	}

	return 1;
}

DialogCreate:CompetitionStartTypeMenu(playerid)
{
	static
		string[MAX_LANG_VALUE_STRING * MAX_COMPETITION_TYPES + 1];

	new
		ctype,
		ctype_name[MAX_COMPETITION_TYPE_NAME],
		ctype_color,
		ctype_color_code[7];

	format(string, sizeof(string), "{B0BEC5}%s\n", _(playerid, COMPETITION_START_PARAM_RANDOM));

	foreach (ctype : CompetitionTypeIterator) {
		CompetitionType_GetParamString(ctype, COMPETITION_TYPE_NAME, ctype_name);
		ctype_color = CompetitionType_GetParamInt(ctype, COMPETITION_TYPE_COLOR);

		GetColorEmbeddingCode(ctype_color, ctype_color_code);

		format(string, sizeof(string), "%s{%s}%s\n", string, ctype_color_code, ctype_name);
	}

	Dialog_Open(playerid, Dialog:CompetitionStartTypeMenu, DIALOG_STYLE_LIST,
			_(playerid, COMPETITION_START_TYPE_HEADER),
			string,
			_(playerid, COMPETITION_MENU_SELECT), _(playerid, COMPETITION_MENU_BACK)
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
		gPlayerStartParams[playerid][COMPETITION_MAP] = INVALID_COMPETITION_MAP_ID;
	} else {
		new
			ctype;

		ctype = GetCompetitionTypeByListitem(listitem, 1);

		if (ctype != gPlayerStartParams[playerid][COMPETITION_TYPE]) {
			gPlayerStartParams[playerid][COMPETITION_TYPE] = ctype;
			gPlayerStartParams[playerid][COMPETITION_MAP] = INVALID_COMPETITION_MAP_ID;
		}
	}

	Dialog_Show(playerid, Dialog:CompetitionStartMenu);
	return 1;
}

DialogCreate:CompetitionStartMapMenu(playerid)
{
	static
		string[MAX_LANG_VALUE_STRING * MAX_COMPETITION_MAPS + 1];

	new
		ctype,
		cmap,
		cmap_type,
		cmap_name[MAX_COMPETITION_MAP_NAME];

	format(string, sizeof(string), "{B0BEC5}%s\n", _(playerid, COMPETITION_START_PARAM_RANDOM));

	ctype = gPlayerStartParams[playerid][COMPETITION_TYPE];

	if (ctype != INVALID_COMPETITION_TYPE_ID) {
		foreach (cmap : CompetitionMapIterator) {
			cmap_type = CompetitionMap_GetParamInt(cmap, COMPETITION_MAP_TYPE);

			if (cmap_type != ctype) {
				continue;
			}

			CompetitionMap_GetParamString(cmap, COMPETITION_MAP_NAME, cmap_name);

			strcat(string, cmap_name);
			strcat(string, "\n");
		}
	}

	Dialog_Open(playerid, Dialog:CompetitionStartMapMenu, DIALOG_STYLE_LIST,
			_(playerid, COMPETITION_START_TYPE_HEADER),
			string,
			_(playerid, COMPETITION_MENU_SELECT), _(playerid, COMPETITION_MENU_BACK)
		);
}

DialogResponse:CompetitionStartMapMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:CompetitionStartMenu);
		return 1;
	}

	if (listitem == 0) {
		gPlayerStartParams[playerid][COMPETITION_MAP] = INVALID_COMPETITION_MAP_ID;
	} else {
		gPlayerStartParams[playerid][COMPETITION_MAP] = GetCompetitionMapByListitem(listitem, gPlayerStartParams[playerid][COMPETITION_TYPE], 1);
	}

	Dialog_Show(playerid, Dialog:CompetitionStartMenu);
	return 1;
}

DialogCreate:CompetitionStartWeatherMenu(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * sizeof(gAvailableWeather) + 1],
		cweather_name[MAX_WEATHER_NAME];

	format(string, sizeof(string), "{B0BEC5}%s\n", _(playerid, COMPETITION_START_PARAM_RANDOM));

	for (new i = 0; i < sizeof(gAvailableWeather); i++) {
		Weather_GetName(gAvailableWeather[i], Lang_GetPlayerLangType(playerid), cweather_name);
		strcat(string, cweather_name);
		strcat(string, "\n");
	}

	Dialog_Open(playerid, Dialog:CompetitionStartWeatherMenu, DIALOG_STYLE_LIST,
			_(playerid, COMPETITION_START_TYPE_HEADER),
			string,
			_(playerid, COMPETITION_MENU_SELECT), _(playerid, COMPETITION_MENU_BACK)
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
	CompetitionReturnMenu
*/

DialogResponse:CompetitionReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
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
	Get competition map by listitem
*/

static stock GetCompetitionMapByListitem(const listitem, const ctype, const offset = 0)
{
	new
		cmap,
		cmap_type,
		index;

	index += offset;

	foreach (cmap : CompetitionMapIterator) {
		cmap_type = CompetitionMap_GetParamInt(cmap, COMPETITION_MAP_TYPE);

		if (cmap_type != ctype) {
			continue;
		}

		if (index == listitem) {
			return cmap;
		}

		index++;
	}

	return INVALID_COMPETITION_MAP_ID;
}

/*
	Get competition type by listitem
*/

static stock GetCompetitionTypeByListitem(const listitem, const offset = 0)
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

static stock GetCompetitionWeatherByListitem(const listitem, const offset = 0)
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
