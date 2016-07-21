/*

	About: declension system
	Author: ziggi

*/

#if defined _core_declension_included
	#endinput
#endif

#define _core_declension_included

/*
	Vars
*/

static
	gWord_1[MAX_LANG_VALUE_STRING],
	gWord_2[MAX_LANG_VALUE_STRING],
	gWord_3[MAX_LANG_VALUE_STRING];

/*
	Get functions
*/

stock Declension_GetWord(result[], const size = sizeof(result), num, word_1[MAX_LANG_VALUE_STRING], word_2[MAX_LANG_VALUE_STRING], word_3[MAX_LANG_VALUE_STRING])
{
	num %= 100;

	if (num > 19) {
		num %= 10;
	}

	switch (num) {
		case 1: {
			strcpy(result, word_1, size);
		}
		case 2..4: {
			strcpy(result, word_2, size);
		}
		default: {
			strcpy(result, word_3, size);
		}
	}
}

stock Declension_GetMonths(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_MONTH_1", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_MONTH_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_MONTH_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetDays(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_DAY_1", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_DAY_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_DAY_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetHours(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_HOUR_1", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_HOUR_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_HOUR_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetMinutes(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_MINUTE_1", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_MINUTE_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_MINUTE_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetSeconds(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_SECOND_1", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_SECOND_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_SECOND_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetSeconds2(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_SECOND_4", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_SECOND_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_SECOND_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetAmmo(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "DECLENSION_AMMO_1", gWord_1);
	Lang_GetPlayerText(playerid, "DECLENSION_AMMO_2", gWord_2);
	Lang_GetPlayerText(playerid, "DECLENSION_AMMO_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

stock Declension_GetAdminWarn(playerid, value, result[], const size = sizeof(result))
{
	Lang_GetPlayerText(playerid, "ADMIN_WARN_DECLENSION_1", gWord_1);
	Lang_GetPlayerText(playerid, "ADMIN_WARN_DECLENSION_2", gWord_2);
	Lang_GetPlayerText(playerid, "ADMIN_WARN_DECLENSION_3", gWord_3);

	Declension_GetWord(result, size, value, gWord_1, gWord_2, gWord_3);
}

/*
	Return functions
*/

stock Declension_ReturnWord(num, word_1[MAX_LANG_VALUE_STRING], word_2[MAX_LANG_VALUE_STRING], word_3[MAX_LANG_VALUE_STRING])
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetWord(result, sizeof(result), num, word_1, word_2, word_3);
	return result;
}

stock Declension_ReturnMonths(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetMonths(playerid, value, result);
	return result;
}

stock Declension_ReturnDays(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetDays(playerid, value, result);
	return result;
}

stock Declension_ReturnHours(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetHours(playerid, value, result);
	return result;
}

stock Declension_ReturnMinutes(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetMinutes(playerid, value, result);
	return result;
}

stock Declension_ReturnSeconds(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetSeconds(playerid, value, result);
	return result;
}

stock Declension_ReturnSeconds2(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetSeconds2(playerid, value, result);
	return result;
}

stock Declension_ReturnAmmo(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetAmmo(playerid, value, result);
	return result;
}

stock Declension_ReturnAdminWarn(playerid, value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetAdminWarn(playerid, value, result);
	return result;
}
