/*

	About: competition race
	Author: ziggi

*/

#if defined _competition_race_included
	#endinput
#endif

#define _competition_race_included

/*
	Vars
*/


/*
	OnGameModeInit
*/

CompetitionRace_OnGameModeInit()
{
	new
		ctype,
		ctype_params[CompetitionTypeParams],
		ctype_name[Lang][MAX_COMPETITION_TYPE_NAME];

	foreach (new Lang:lang : LangIterator) {
		__l(Lang_GetID(lang), COMPETITION_TYPE_RACE_NAME, ctype_name[lang], MAX_COMPETITION_TYPE_NAME);
	}

	ctype_params[COMPETITION_TYPE_COLOR] = COLOR_BLUE_500;

	ctype = CompetitionType_Add(ctype_name, ctype_params);
	CompetitionType_SetActiveStatus(ctype, true);
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_ADD_CALLBACK, "CompetitionRace_OnAdd");
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_JOIN_CALLBACK, "CompetitionRace_OnJoin");
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_LEAVE_CALLBACK, "CompetitionRace_OnLeave");
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_START_CALLBACK, "CompetitionRace_OnStart");

	// map
	new
		cmap,
		cmap_params[CompetitionMapParams];

	cmap_params[COMPETITION_MAP_TYPE] = ctype;

	strcpy(cmap_params[COMPETITION_MAP_NAME], "test", MAX_COMPETITION_MAP_NAME);
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "עוסע", MAX_COMPETITION_MAP_NAME);
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "פגפûג", MAX_COMPETITION_MAP_NAME);
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);
}

/*
	CompetitionRace_OnAdd
*/

forward CompetitionRace_OnAdd(cid, playerid);
public CompetitionRace_OnAdd(cid, playerid)
{
	printf("CompetitionRace_OnAdd(%d, %d)", cid, playerid);
}

/*
	CompetitionRace_OnJoin
*/

forward CompetitionRace_OnJoin(cid, playerid);
public CompetitionRace_OnJoin(cid, playerid)
{
	printf("CompetitionRace_OnJoin(%d, %d)", cid, playerid);
}

/*
	CompetitionRace_OnLeave
*/

forward CompetitionRace_OnLeave(cid, playerid);
public CompetitionRace_OnLeave(cid, playerid)
{
	printf("CompetitionRace_OnLeave(%d, %d)", cid, playerid);
}

/*
	CompetitionRace_OnStart
*/

forward CompetitionRace_OnStart(cid);
public CompetitionRace_OnStart(cid)
{
	printf("CompetitionRace_OnStart(%d)", cid);
}
