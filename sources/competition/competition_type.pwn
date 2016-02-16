/*
	
	About: competition type system
	Author: ziggi

*/

#if defined _competition_type_included
	#endinput
#endif

#define _competition_type_included

/*
	Defines
*/

#if !defined MAX_COMPETITION_TYPE_NAME
	#define MAX_COMPETITION_TYPE_NAME 32
#endif

#define MAX_COMPETITION_TYPES 5
#define INVALID_COMPETITION_TYPE_ID -1

/*
	Enums
*/

enum CompetitionTypeParams {
	COMPETITION_TYPE_NAME[MAX_COMPETITION_TYPE_NAME],
	COMPETITION_TYPE_COLOR,
}

/*
	Vars
*/

static
	gParam[MAX_COMPETITION_TYPES][CompetitionTypeParams],
	Iterator:CompetitionTypeIterator<MAX_COMPETITION_TYPES>;

/*
	CompetitionType_Add
*/

stock CompetitionType_Add(ctype_params[CompetitionParams])
{
	new
		ctype = CompetitionType_GetFreeSlot();

	if (ctype != INVALID_COMPETITION_TYPE_ID) {
		gParam[ctype] = ctype_params;
	}

	return ctype;
}

/*
	CompetitionType_Remove
*/

stock CompetitionType_Remove(ctype)
{
	if (!CompetitionType_IsActive(ctype)) {
		return 0;
	}

	CompetitionType_SetActiveStatus(ctype, false);
	CompetitionType_SetName(ctype, "");

	return 1;
}

/*
	Active status
*/

stock CompetitionType_IsActive(ctype)
{
	return Iter_Contains(CompetitionTypeIterator, ctype);
}

stock CompetitionType_SetActiveStatus(ctype, bool:status)
{
	if (status) {
		Iter_Add(CompetitionTypeIterator, ctype);
	} else {
		Iter_Remove(CompetitionTypeIterator, ctype);
	}
}

stock CompetitionType_GetFreeSlot()
{
	new
		slot;

	slot = Iter_Free(CompetitionTypeIterator);

	if (slot == -1) {
		return INVALID_COMPETITION_TYPE_ID;
	}

	return slot;
}

stock CompetitionType_GetCount()
{
	return Iter_Count(CompetitionTypeIterator);
}

/*
	Integer params
*/

stock CompetitionType_SetParamInt(ctype, CompetitionTypeParams:param, value)
{
	gParam[ctype][param] = value;
}

stock CompetitionType_GetParamInt(ctype, CompetitionTypeParams:param)
{
	return gParam[ctype][param];
}

/*
	Float params
*/

stock CompetitionType_SetParamFloat(ctype, CompetitionTypeParams:param, Float:value)
{
	gParam[ctype][param] = _:value;
}

stock Float:CompetitionType_GetParamFloat(ctype, CompetitionTypeParams:param)
{
	return Float:gParam[ctype][param];
}

/*
	String params
*/

stock CompetitionType_SetParamString(ctype, CompetitionTypeParams:param, value[])
{
	strmid(gParam[ctype][param], value, 0, strlen(value), COMPETITION_MAX_STRING);
}

stock CompetitionType_GetParamString(ctype, CompetitionTypeParams:param, value[], size = sizeof(value))
{
	strmid(value, gParam[ctype][param], 0, strlen(gParam[ctype][param]), size);
}
