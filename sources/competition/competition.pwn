/*
	
	About: player competition system
	Author: ziggi

*/

#if defined _competition_included
	#endinput
#endif

#define _competition_included

/*
	Defines
*/

#if !defined MAX_COMPETITION_NAME
	#define MAX_COMPETITION_NAME MAX_LANG_VALUE_STRING
#endif

#if !defined MAX_COMPETITION_CURRENT
	#define MAX_COMPETITION_CURRENT 15
#endif

#if !defined COMPETITION_DEFAULT_WAIT_TIME
	#define COMPETITION_DEFAULT_WAIT_TIME 60
#endif

#define COMPETITION_VIRTUAL_WORLD_PADDING 2000

/*
	Enums
*/

enum e_Competition_Current_Info {
	e_cpName[MAX_COMPETITION_NAME],
	e_cpType,
	e_cpTime,
	e_cpWorldTime,
	e_cpVirtualWorld,
}

/*
	Vars
*/

static
	gCompetitionCurrent[MAX_COMPETITION_CURRENT][e_Competition_Current_Info];

new
	Iterator:CurrentCompetition<MAX_COMPETITION_CURRENT>;

/*
	Competition Add
*/

stock Competition_Add(name[], type = -1, time = -1, virtual_world = -1)
{
	new cid = Competition_GetFreeSlot();
	if (cid == INVALID_COMPETITION_ID) {
		Log_Debug("Error <competition:Competition_Start>: free slot not found (%s).", name);
		return INVALID_COMPETITION_ID;
	}

	Competition_SetActiveStatus(cid, true);

	if (type == -1) {
		type = random(CompetitionType_GetCount());
	}

	if (time == -1) {
		time = COMPETITION_DEFAULT_WAIT_TIME;
	}

	if (virtual_world == -1) {
		virtual_world = COMPETITION_VIRTUAL_WORLD_PADDING + cid;
	}

	Competition_SetName(cid, name);	
	Competition_SetType(cid, type);
	Competition_SetTime(cid, time);
	Competition_SetVirtualWorld(cid, virtual_world);

	return cid;
}

stock Competition_Remove(cid)
{
	if (!Competition_IsActive(cid)) {
		return 0;
	}

	Competition_SetActiveStatus(cid, false);

	return 1;
}

/*
	Competition Name
*/

stock Competition_GetName(cid, name[], const size = sizeof(name))
{
	strmid(name, gCompetitionCurrent[cid][e_cpName], 0, strlen(gCompetitionCurrent[cid][e_cpName]), size);
}

stock Competition_SetName(cid, name[])
{
	strmid(gCompetitionCurrent[cid][e_cpName], name, 0, strlen(name), MAX_COMPETITION_NAME);
}

/*
	Competition Type
*/

stock Competition_GetType(cid)
{
	return gCompetitionCurrent[cid][e_cpType];
}

stock Competition_SetType(cid, type)
{
	gCompetitionCurrent[cid][e_cpType] = type;
}

/*
	Competition Time
*/

stock Competition_GetTime(cid)
{
	return gCompetitionCurrent[cid][e_cpTime];
}

stock Competition_SetTime(cid, time)
{
	gCompetitionCurrent[cid][e_cpTime] = time;
}

/*
	Competition Virtual World
*/

stock Competition_GetVirtualWorld(cid)
{
	return gCompetitionCurrent[cid][e_cpVirtualWorld];
}

stock Competition_SetVirtualWorld(cid, virtual_world)
{
	gCompetitionCurrent[cid][e_cpVirtualWorld] = virtual_world;
}

/*
	Active status
*/

stock Competition_IsActive(cid)
{
	return Iter_Contains(CurrentCompetition, cid);
}

stock Competition_SetActiveStatus(cid, bool:status)
{
	if (status) {
		Iter_Add(CurrentCompetition, cid);
	} else {
		Iter_Remove(CurrentCompetition, cid);
	}
}

stock Competition_GetFreeSlot()
{
	return Iter_Free(CurrentCompetition);
}
