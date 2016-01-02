/*

	About: declension system
	Author: ziggi

*/

#if defined _core_declension_included
  #endinput
#endif

#define _core_declension_included
#pragma library core_declension


stock Declension_ReturnWord(num, word_1[MAX_LANG_VALUE_STRING], word_2[MAX_LANG_VALUE_STRING], word_3[MAX_LANG_VALUE_STRING])
{
	num %= 100;
	
	if (num > 19) {
		num %= 10;
	}

	switch (num) {
		case 1: {
			return word_1;
		}
		case 2..4: {
			return word_2;
		}
	}
	return word_3;
}

stock Declension_GetMonths(value)
{
	new result[MAX_LANG_VALUE_STRING];
	result = Declension_ReturnWord(value, _(DECLENSION_MONTH_1), _(DECLENSION_MONTH_2), _(DECLENSION_MONTH_3))
	return result;
}

stock Declension_GetDays(value)
{
	new result[MAX_LANG_VALUE_STRING];
	result = Declension_ReturnWord(value, _(DECLENSION_DAY_1), _(DECLENSION_DAY_2), _(DECLENSION_DAY_3));
	return result;
}

stock Declension_GetHours(value)
{
	new result[MAX_LANG_VALUE_STRING];
	result = Declension_ReturnWord(value, _(DECLENSION_HOUR_1), _(DECLENSION_HOUR_2), _(DECLENSION_HOUR_3));
	return result;
}

stock Declension_GetMinutes(value)
{
	new result[MAX_LANG_VALUE_STRING];
	result = Declension_ReturnWord(value, _(DECLENSION_MINUTE_1), _(DECLENSION_MINUTE_2), _(DECLENSION_MINUTE_3));
	return result;
}

stock Declension_GetSeconds(value)
{
	new result[MAX_LANG_VALUE_STRING];
	result = Declension_ReturnWord(value, _(DECLENSION_SECOND_1), _(DECLENSION_SECOND_2), _(DECLENSION_SECOND_3));
	return result;
}

stock Declension_GetAmmo(value)
{
	new result[MAX_LANG_VALUE_STRING];
	result = Declension_ReturnWord(value, _(DECLENSION_AMMO_1), _(DECLENSION_AMMO_2), _(DECLENSION_AMMO_3));
	return result;
}
