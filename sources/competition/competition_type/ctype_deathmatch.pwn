/*

	About: competition deathmatch
	Author: ziggi

*/

#if defined _competition_dm_included
	#endinput
#endif

#define _competition_dm_included

/*
	Vars
*/


/*
	OnGameModeInit
*/

CompetitionDM_OnGameModeInit()
{
	new
		ctype,
		ctype_params[CompetitionTypeParams];

	__(COMPETITION_TYPE_DM_NAME, ctype_params[COMPETITION_TYPE_NAME]);
	ctype_params[COMPETITION_TYPE_COLOR] = COLOR_RED_500;

	ctype = CompetitionType_Add(ctype_params);
	CompetitionType_SetActiveStatus(ctype, true);
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_ADD_CALLBACK, "CompetitionDM_OnAdd");
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_JOIN_CALLBACK, "CompetitionDM_OnJoin");
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_LEAVE_CALLBACK, "CompetitionDM_OnLeave");
	CompetitionType_SetParamString(ctype, COMPETITION_TYPE_START_CALLBACK, "CompetitionDM_OnStart");

	// map
	new
		cmap,
		cmap_params[CompetitionMapParams];

	cmap_params[COMPETITION_MAP_TYPE] = ctype;

	strcpy(cmap_params[COMPETITION_MAP_NAME], "dm test", MAX_COMPETITION_MAP_NAME);
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "הל עוסע", MAX_COMPETITION_MAP_NAME);
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "נןע ללסט", MAX_COMPETITION_MAP_NAME);
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);
}

/*
	CompetitionDM_OnAdd
*/

forward CompetitionDM_OnAdd(cid, playerid);
public CompetitionDM_OnAdd(cid, playerid)
{
	printf("CompetitionDM_OnAdd(%d, %d)", cid, playerid);
}

/*
	CompetitionDM_OnJoin
*/

forward CompetitionDM_OnJoin(cid, playerid);
public CompetitionDM_OnJoin(cid, playerid)
{
	printf("CompetitionDM_OnJoin(%d, %d)", cid, playerid);
}

/*
	CompetitionDM_OnLeave
*/

forward CompetitionDM_OnLeave(cid, playerid);
public CompetitionDM_OnLeave(cid, playerid)
{
	printf("CompetitionDM_OnLeave(%d, %d)", cid, playerid);
}

/*
	CompetitionDM_OnStart
*/

forward CompetitionDM_OnStart(cid);
public CompetitionDM_OnStart(cid)
{
	printf("CompetitionDM_OnStart(%d)", cid);
}
