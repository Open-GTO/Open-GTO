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
}
