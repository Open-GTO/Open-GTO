/*

	About: declension system
	Author: ziggi

*/

#if defined _core_declension_included
	#endinput
#endif

#define _core_declension_included

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

stock Declension_GetMonths(value, result[], const size = sizeof(result))
{
	Declension_GetWord(result, size, value, _(DECLENSION_MONTH_1), _(DECLENSION_MONTH_2), _(DECLENSION_MONTH_3))
}

stock Declension_GetDays(value, result[], const size = sizeof(result))
{
	Declension_GetWord(result, size, value, _(DECLENSION_DAY_1), _(DECLENSION_DAY_2), _(DECLENSION_DAY_3));
}

stock Declension_GetHours(value, result[], const size = sizeof(result))
{
	Declension_GetWord(result, size, value, _(DECLENSION_HOUR_1), _(DECLENSION_HOUR_2), _(DECLENSION_HOUR_3));
}

stock Declension_GetMinutes(value, result[], const size = sizeof(result))
{
	Declension_GetWord(result, size, value, _(DECLENSION_MINUTE_1), _(DECLENSION_MINUTE_2), _(DECLENSION_MINUTE_3));
}

stock Declension_GetSeconds(value, result[], const size = sizeof(result))
{
	Declension_GetWord(result, size, value, _(DECLENSION_SECOND_1), _(DECLENSION_SECOND_2), _(DECLENSION_SECOND_3));
}

stock Declension_GetAmmo(value, result[], const size = sizeof(result))
{
	Declension_GetWord(result, size, value, _(DECLENSION_AMMO_1), _(DECLENSION_AMMO_2), _(DECLENSION_AMMO_3));
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

stock Declension_ReturnMonths(value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetMonths(value, result);
	return result;
}

stock Declension_ReturnDays(value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetDays(value, result);
	return result;
}

stock Declension_ReturnHours(value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetHours(value, result);
	return result;
}

stock Declension_ReturnMinutes(value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetMinutes(value, result);
	return result;
}

stock Declension_ReturnSeconds(value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetSeconds(value, result);
	return result;
}

stock Declension_ReturnAmmo(value)
{
	new result[MAX_LANG_VALUE_STRING];
	Declension_GetAmmo(value, result);
	return result;
}
