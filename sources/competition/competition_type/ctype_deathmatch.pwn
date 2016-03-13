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

	// map
	new
		cmap,
		cmap_params[CompetitionMapParams];

	cmap_params[COMPETITION_MAP_TYPE] = ctype;

	strcpy(cmap_params[COMPETITION_MAP_NAME], "dm test");
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "הל עוסע");
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "נןע ללסט");
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);
}
