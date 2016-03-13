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
}
