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
	ctype_params[COMPETITION_TYPE_COLOR] = 0x2196F3FF;

	ctype = CompetitionType_Add(ctype_params);
	CompetitionType_SetActiveStatus(ctype, true);
}
