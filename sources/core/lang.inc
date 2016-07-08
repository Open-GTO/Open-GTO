/*

	About: lang load header
	Author: ziggi

*/

#if defined _lang_h_included
	#endinput
#endif

#define _lang_h_included

/*
	Defines
*/

#define MAX_LANG_NAME 16

/*
	Enums
*/

enum Lang {
	LangEN,
	LangRU,
}

/*
	Vars
*/

// for get size in arrays
new
	gLangSize[Lang];
#pragma unused gLangSize
