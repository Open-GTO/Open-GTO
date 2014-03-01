/*
	
	Updated in 21.08.2011 by ZiGGi
	
*/

#if defined _lang_included
	#endinput
#endif

#define _lang_included
#pragma library lang


new lang_use[10] = LANGUAGE_USE;
new lang_texts[20][100][MAX_STRING + 10];

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
	format(lang_file, sizeof(lang_file), "%sLanguage-%s%s", GTO_FILES_FOLDER, lang_use, GTO_FILES_FORMAT);
	lang_ReadFile(lang_file);

	GameMSG("SERVER: Lang module init(%s)", lang_file);
	return 1;
}

stock lang_ReadFile(filename[])
{
	new File:fhandle = fopen(filename, io_read);
	if (!fhandle) {
		GameMSG("ERROR: No such language file '%s'", filename);
		return 0;
	}

	new
		tmp[4], string[MAX_STRING + 32],
		section_id = 0,
		sep_pos, length;

	while (fread(fhandle, string, sizeof(string))) {
		length = strlen(string);
		if (length == 0 || strfind(string, "//", true) == 0) {
			continue;
		}

		sep_pos = strfind(string, ":", true);
		if (sep_pos != -1) {
			lang_FixSpecialChar(string);

			strmid(tmp, string, 0, sep_pos);
			strmid(lang_texts[section_id][ strval(tmp) ], string, sep_pos + 2, length - sep_pos);
		} else {
			section_id++;
		}
	}

	fclose(fhandle);
	return 1;
}

stock lang_FixSpecialChar(string[MAX_STRING + 32])
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
