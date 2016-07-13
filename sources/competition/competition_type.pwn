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

#if !defined MAX_FUNCTION_NAME
	#define MAX_FUNCTION_NAME 32
#endif

#define MAX_COMPETITION_TYPES 5
#define INVALID_COMPETITION_TYPE_ID -1

/*
	Enums
*/

enum CompetitionTypeParams {
	COMPETITION_TYPE_COLOR,
	COMPETITION_TYPE_ADD_CALLBACK[MAX_FUNCTION_NAME],
	COMPETITION_TYPE_JOIN_CALLBACK[MAX_FUNCTION_NAME],
	COMPETITION_TYPE_LEAVE_CALLBACK[MAX_FUNCTION_NAME],
	COMPETITION_TYPE_START_CALLBACK[MAX_FUNCTION_NAME],
}

/*
	Vars
*/

static
	gParam[MAX_COMPETITION_TYPES][CompetitionTypeParams],
	gParamName[MAX_COMPETITION_TYPES][Lang][MAX_COMPETITION_TYPE_NAME];

new
	Iterator:CompetitionTypeIterator<MAX_COMPETITION_TYPES>;

/*
	CompetitionType_Add
*/

stock CompetitionType_Add(ctype_name[Lang][MAX_COMPETITION_TYPE_NAME], ctype_params[CompetitionTypeParams])
{
	new
		ctype = CompetitionType_GetFreeSlot();

	if (ctype != INVALID_COMPETITION_TYPE_ID) {
		gParam[ctype] = ctype_params;

		foreach (new Lang:lang : LangIterator) {
			CompetitionType_SetName(ctype, lang, ctype_name[lang]);
		}
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
	CompetitionType_OnAdd
*/

CompetitionType_OnAdd(ctype, cid, playerid)
{
	new callback[MAX_FUNCTION_NAME];
	CompetitionType_GetParamString(ctype, COMPETITION_TYPE_ADD_CALLBACK, callback);
	CallLocalFunction(callback, "ii", cid, playerid);
}

/*
	CompetitionType_OnJoin
*/

CompetitionType_OnJoin(ctype, cid, playerid)
{
	new callback[MAX_FUNCTION_NAME];
	CompetitionType_GetParamString(ctype, COMPETITION_TYPE_JOIN_CALLBACK, callback);
	CallLocalFunction(callback, "ii", cid, playerid);
}

/*
	CompetitionType_OnLeave
*/

CompetitionType_OnLeave(ctype, cid, playerid)
{
	new callback[MAX_FUNCTION_NAME];
	CompetitionType_GetParamString(ctype, COMPETITION_TYPE_LEAVE_CALLBACK, callback);
	CallLocalFunction(callback, "ii", cid, playerid);
}

/*
	CompetitionType_OnStart
*/

CompetitionType_OnStart(ctype, cid)
{
	new callback[MAX_FUNCTION_NAME];
	CompetitionType_GetParamString(ctype, COMPETITION_TYPE_START_CALLBACK, callback);
	CallLocalFunction(callback, "i", cid);
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

stock CompetitionType_GetRandom()
{
	return Iter_Random(CompetitionTypeIterator);
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
	strcpy(gParam[ctype][param], value, COMPETITION_MAX_STRING);
}

stock CompetitionType_GetParamString(ctype, CompetitionTypeParams:param, value[], size = sizeof(value))
{
	strcpy(value, gParam[ctype][param], size);
}

/*
	Type name
*/

stock CompetitionType_SetName(ctype, Lang:lang, value[])
{
	strcpy(gParamName[ctype][lang], value, COMPETITION_MAX_STRING);
}

stock CompetitionType_GetName(ctype, Lang:lang, value[], size = sizeof(value))
{
	strcpy(value, gParamName[ctype][lang], size);
}
