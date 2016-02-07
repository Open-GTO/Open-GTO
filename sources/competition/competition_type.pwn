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
	#define MAX_COMPETITION_TYPE_NAME MAX_LANG_VALUE_STRING
#endif

#define MAX_COMPETITION_TYPES 5
#define INVALID_COMPETITION_TYPE_ID -1

/*
	Enums
*/

enum e_Competition_Type_Info {
	e_ctName[MAX_COMPETITION_TYPE_NAME],
	e_ctColor,
}

/*
	Vars
*/

static
	gCompetitionTypes[MAX_COMPETITION_TYPES][e_Competition_Type_Info],
	Iterator:CompetitionType<MAX_COMPETITION_TYPES>;

/*
	CompetitionType_Add
*/

stock CompetitionType_Add(name[])
{
	new
		ctype = CompetitionType_GetFreeSlot();

	if (ctype != INVALID_COMPETITION_TYPE_ID) {
		CompetitionType_SetName(ctype, name);
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
	CompetitionType Name
*/

stock CompetitionType_SetName(ctype, name[])
{
	strmid(gCompetitionTypes[ctype][e_ctName], name, 0, strlen(name), MAX_COMPETITION_TYPE_NAME);
}

stock CompetitionType_GetName(ctype, name[], const size = sizeof(name))
{
	strmid(name, gCompetitionTypes[ctype][e_ctName], 0, strlen(gCompetitionTypes[ctype][e_ctName]), size);
}

/*
	CompetitionType Color
*/

stock CompetitionType_SetColor(ctype, color)
{
	gCompetitionTypes[ctype][e_ctColor] = color;
}

stock CompetitionType_GetColor(ctype)
{
	return gCompetitionTypes[ctype][e_ctColor];
}

stock CompetitionType_GetColorCode(ctype, code[], const size = sizeof(code))
{
	format(code, size, "%06x", CompetitionType_GetColor(ctype) >>> 8);
}

/*
	Active status
*/

stock CompetitionType_IsActive(type)
{
	return Iter_Contains(CompetitionType, type);
}

stock CompetitionType_SetActiveStatus(type, bool:status)
{
	if (status) {
		Iter_Add(CompetitionType, type);
	} else {
		Iter_Remove(CompetitionType, type);
	}
}

stock CompetitionType_GetFreeSlot()
{
	return Iter_Free(CompetitionType);
}

stock CompetitionType_GetCount()
{
	return Iter_Count(CompetitionType);
}
