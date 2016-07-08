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

/*
	OnGameModeInit
*/

Lang_OnGameModeInit()
{
	new
		lang_file[ZLANG_MAX_FILENAME_PATH];

	for (new Lang:lang; _:lang < sizeof(gLang); _:lang++) {
		format(lang_file, sizeof(lang_file), "%slang_%s" DATA_FILES_FORMAT, db_lang, gLangName[lang]);
		gLang[lang] = Lang_LoadText(lang_file);
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
		Lang:lang,
		langid,
		load_status,
		lang_file[ZLANG_MAX_FILENAME_PATH];

	for ( ; _:lang < sizeof(gLang); _:lang++) {
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
	for (new i; i < sizeof(gLang); i++) {
		if (gLang[Lang:i] == langid) {
			return Lang:i;
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
	for (new Lang:lang; _:lang < sizeof(gLangName); _:lang++) {
		if (strcmp(gLangName[lang], name, true) == 0) {
			return Lang_GetID(lang);
		}
	}
	return INVALID_LANG_ID;
}

stock Lang_GetCount()
{
	return sizeof(gLang);
}

stock Lang_GetTypeName(Lang:type, name[], const size = sizeof(name))
{
	return strcpy(name, gLangName[type], size);
}
