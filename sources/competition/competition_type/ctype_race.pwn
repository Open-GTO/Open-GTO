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
		ctype_params[CompetitionTypeParams];

	__(COMPETITION_TYPE_RACE_NAME, ctype_params[COMPETITION_TYPE_NAME]);
	ctype_params[COMPETITION_TYPE_COLOR] = COLOR_BLUE_500;

	ctype = CompetitionType_Add(ctype_params);
	CompetitionType_SetActiveStatus(ctype, true);

	// map
	new
		cmap,
		cmap_params[CompetitionMapParams];

	cmap_params[COMPETITION_MAP_TYPE] = ctype;

	strcpy(cmap_params[COMPETITION_MAP_NAME], "test");
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "עוסע");
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);

	strcpy(cmap_params[COMPETITION_MAP_NAME], "פגפûג");
	cmap = CompetitionMap_Add(cmap_params);
	CompetitionMap_SetActiveStatus(cmap, true);
}
