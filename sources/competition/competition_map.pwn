/*

	About: competition map system
	Author: ziggi

*/

#if defined _competition_map_included
	#endinput
#endif

#define _competition_map_included

/*
	Defines
*/

#if !defined MAX_COMPETITION_MAP_NAME
	#define MAX_COMPETITION_MAP_NAME 32
#endif

#define MAX_COMPETITION_MAPS 100
#define INVALID_COMPETITION_MAP_ID -1

/*
	Enums
*/

enum CompetitionMapParams {
	COMPETITION_MAP_NAME[MAX_COMPETITION_MAP_NAME],
	COMPETITION_MAP_TYPE,
}

/*
	Vars
*/

static
	gParam[MAX_COMPETITION_MAPS][CompetitionMapParams];

new
	Iterator:CompetitionMapIterator<MAX_COMPETITION_MAPS>;

/*
	CompetitionMap_Add
*/

stock CompetitionMap_Add(const cmap_params[CompetitionMapParams])
{
	new
		cmap = CompetitionMap_GetFreeSlot();

	if (cmap != INVALID_COMPETITION_MAP_ID) {
		gParam[cmap] = cmap_params;
	}

	return cmap;
}

/*
	CompetitionMap_Remove
*/

stock CompetitionMap_Remove(cmap)
{
	if (!CompetitionMap_IsActive(cmap)) {
		return 0;
	}

	CompetitionMap_SetActiveStatus(cmap, false);
	CompetitionMap_SetName(cmap, "");

	return 1;
}

/*
	Active status
*/

stock CompetitionMap_IsActive(cmap)
{
	return Iter_Contains(CompetitionMapIterator, cmap);
}

stock CompetitionMap_SetActiveStatus(cmap, bool:status)
{
	if (status) {
		Iter_Add(CompetitionMapIterator, cmap);
	} else {
		Iter_Remove(CompetitionMapIterator, cmap);
	}
}

stock CompetitionMap_GetFreeSlot()
{
	new
		slot;

	slot = Iter_Free(CompetitionMapIterator);

	if (slot == -1) {
		return INVALID_COMPETITION_MAP_ID;
	}

	return slot;
}

stock CompetitionMap_GetCount()
{
	return Iter_Count(CompetitionMapIterator);
}

stock CompetitionMap_GetRandom()
{
	return Iter_Random(CompetitionMapIterator);
}

/*
	Integer params
*/

stock CompetitionMap_SetParamInt(cmap, CompetitionMapParams:param, value)
{
	gParam[cmap][param] = value;
}

stock CompetitionMap_GetParamInt(cmap, CompetitionMapParams:param)
{
	return gParam[cmap][param];
}

/*
	Float params
*/

stock CompetitionMap_SetParamFloat(cmap, CompetitionMapParams:param, Float:value)
{
	gParam[cmap][param] = _:value;
}

stock Float:CompetitionMap_GetParamFloat(cmap, CompetitionMapParams:param)
{
	return Float:gParam[cmap][param];
}

/*
	String params
*/

stock CompetitionMap_SetParamString(cmap, CompetitionMapParams:param, const value[])
{
	strcpy(gParam[cmap][param], value, COMPETITION_MAX_STRING);
}

stock CompetitionMap_GetParamString(cmap, CompetitionMapParams:param, value[], size = sizeof(value))
{
	strcpy(value, gParam[cmap][param], size);
}
