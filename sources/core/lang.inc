/*
	
	Updated in 21.08.2011 by ZiGGi
	
*/

#if defined _lang_included
	#endinput
#endif

#define _lang_included
#pragma library lang

#define MAX_LANG_STRING		MAX_STRING * 2

new lang_use[10] = LANGUAGE_USE;
new lang_texts[20][100][MAX_LANG_STRING + 10];

stock lang_LoadConfig(file_config)
{
	ini_getString(file_config, "Language", lang_use);
}

stock lang_SaveConfig(file_config)
{
	ini_setString(file_config, "Language", lang_use);
}

stock lang_OnGameModeInit()
{
	new lang_file[MAX_STRING];
	format(lang_file, sizeof(lang_file), "%slang_%s%s", db_lang, lang_use, DATA_FILES_FORMAT);
	lang_ReadFile(lang_file);

	Log_Game("SERVER: Lang module init(%s)", lang_file);
	return 1;
}

stock lang_ReadFile(filename[])
{
	new File:fhandle = fopen(filename, io_read);
	if (!fhandle) {
		Log_Game("ERROR: No such language file '%s'", filename);
		return 0;
	}

	new
		tmp[4], string[MAX_LANG_STRING + 32],
		section_id = 0,
		sep_pos;

	while (fread(fhandle, string, sizeof(string))) {
		if (strlen(string) == 0 || strfind(string, "//", true) == 0) {
			continue;
		}

		sep_pos = strfind(string, ":", true);
		if (sep_pos != -1) {
			StripNL(string);
			lang_FixSpecialChar(string);

			strmid(tmp, string, 0, sep_pos);
			strmid(lang_texts[section_id][ strval(tmp) ], string, sep_pos + 2, strlen(string));
		} else {
			section_id++;
		}
	}

	fclose(fhandle);
	return 1;
}

stock lang_FixSpecialChar(string[MAX_LANG_STRING + 32])
{
	for (new i = 0; string[i] != '\0'; i++) {
		if (string[i] != '[' || string[i + 2] != ']') {
			continue;
		}
		switch (string[i + 1]) {
			case 'n': {
				strdel(string, i, i + 3);
				strins(string, "\n", i);
			}
			case 't': {
				strdel(string, i, i + 3);
				strins(string, "\t", i);
			}
		}
	}
}

stock lang_GetLang()
{
	return lang_use;
}

stock lang_SetLang(lang)
{
	strmid(lang_use, lang, 0, strlen(lang));
	lang_OnGameModeInit();
}
