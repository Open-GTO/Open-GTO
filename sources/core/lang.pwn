/*

	About: lang load system
	Author: ziggi

*/

#if defined _lang_included
	#endinput
#endif

#define _lang_included

/*
	Defines
*/

#define MAX_LANG_FILE_NAME  16

/*
	Vars
*/

static
	gCurrentLanguage[MAX_LANG_FILE_NAME] = LANGUAGE_USE;

/*
	Config
*/

stock Lang_LoadConfig(file_config)
{
	ini_getString(file_config, "Language", gCurrentLanguage);
}

stock Lang_SaveConfig(file_config)
{
	ini_setString(file_config, "Language", gCurrentLanguage);
}

/*
	OnGameModeInit
*/

stock Lang_OnGameModeInit()
{
	new
		lang_file[MAX_STRING],
		rcon_command[8 + 1 + 1 + MAX_LANG_FILE_NAME + 1];

	format(lang_file, sizeof(lang_file), "%slang_%s" DATA_FILES_FORMAT, db_lang, gCurrentLanguage);
	Lang_LoadText(lang_file);

	Lang_GetLang(rcon_command);
	format(rcon_command, sizeof(rcon_command), "language %c%s", toupper(rcon_command[0]), rcon_command[1]);
	SendRconCommand(rcon_command);

	Log_Game("SERVER: Lang module init(%s)", lang_file);
}

/*
	Lang_[G|S]etLang
*/

stock Lang_GetLang(lang[], const size = sizeof(lang))
{
	strcpy(lang, gCurrentLanguage, size);
}

stock Lang_SetLang(langname[])
{
	strcpy(gCurrentLanguage, langname);
	Lang_OnGameModeInit();
}
