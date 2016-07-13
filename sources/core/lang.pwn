/*

	About: lang load system
	Author: ziggi

*/

#if defined _lang_included
	#endinput
#endif

#define _lang_included

/*
	Vars
*/

static
	gLangName[Lang][MAX_LANG_NAME] = {
		"english",
		"russian"
	},
	gLang[Lang];

const
	LANG_COUNT = sizeof(gLang);

new
	Iterator:LangIterator<LANG_COUNT>;

/*
	OnGameModeInit
*/

Lang_OnGameModeInit()
{
	new
		lang_file[ZLANG_MAX_FILENAME_PATH];

	for (new Lang:lang; _:lang < LANG_COUNT; _:lang++) {
		format(lang_file, sizeof(lang_file), "%slang_%s" DATA_FILES_FORMAT, db_lang, gLangName[lang]);
		gLang[lang] = Lang_LoadText(lang_file);
		Iter_Add(LangIterator, gLang[lang]);
		Log_Game("SERVER: Language loaded (%s)", gLangName[lang]);
	}

	Lang_SetDefaultLanguage(gLang[Lang:0]);

	new
		temp[MAX_LANG_NAME],
		rcon_command[(MAX_LANG_NAME + 1) * sizeof(gLangName)] = "language ";

	for (new i = 0; i < sizeof(gLangName); i++) {
		format(temp, sizeof(temp), "%c%s", toupper(gLangName[Lang:i][0]), gLangName[Lang:i][1]);
		strcat(rcon_command, temp);

		if (i < sizeof(gLangName) - 1) {
			strcat(rcon_command, "/");
		}
	}
	SendRconCommand(rcon_command);

	Log_Game("SERVER: Lang module init");
}

stock Lang_Reload()
{
	new
		langid,
		load_status,
		lang_file[ZLANG_MAX_FILENAME_PATH];

	foreach (new Lang:lang : LangIterator) {
		langid = Lang_GetID(lang);

		format(lang_file, sizeof(lang_file), "%slang_%s" DATA_FILES_FORMAT, db_lang, gLangName[lang]);
		load_status = Lang_ReloadText(lang_file, langid, true);
		if (load_status) {
			Log_Game("SERVER: Language reloaded (%s)", gLangName[lang]);
		} else {
			Log_Game("SERVER: Language NOT reloaded (%s)", gLangName[lang]);
		}
	}
}

/*
	Lang functions
*/

stock Lang:Lang_GetType(langid)
{
	foreach (new Lang:lang : LangIterator) {
		if (gLang[lang] == langid) {
			return lang;
		}
	}
	return LangEN;
}

stock Lang:Lang_GetPlayerLangType(playerid)
{
	return Lang_GetType(Lang_GetPlayerLanguage(playerid));
}

stock Lang_GetID(Lang:type)
{
	return gLang[type];
}

stock Lang_GetIDFromName(name[])
{
	foreach (new Lang:lang : LangIterator) {
		if (strcmp(gLangName[lang], name, true) == 0) {
			return Lang_GetID(lang);
		}
	}
	return INVALID_LANG_ID;
}

stock Lang_GetCount()
{
	return LANG_COUNT;
}

stock Lang_GetTypeName(Lang:type, name[], const size = sizeof(name))
{
	return strcpy(name, gLangName[type], size);
}
