/*
	
	Updated in 21.08.2011 by ZiGGi
	
*/

#if defined _lang_included
	#endinput
#endif

#define _lang_included
#pragma library lang


new lang_db[10] = GTO_FILES_FOLDER;
new lang[10] = LANGUAGE_USE;
new lang_texts[20][100][MAX_STRING+10];

stock lang_LoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_lang = ini_Open(ConfigDB);
    ini_Get(file_lang, "Language", lang);
	ini_Close(file_lang);
	return 1;
}

stock lang_SaveConfig()
{
	new file_lang = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_lang, "Language", lang);
	ini_Close(file_lang);
	return 1;
}

stock lang_OnGameModeInit()
{
	lang_LoadConfig();
	new lang_file[50];
	format(lang_file, sizeof(lang_file), "%sLanguage-%s"GTO_FILES_FORMAT, lang_db, lang);
	lang_read(lang_file);
	GameMSG("SERVER: Lang module init(%s)", lang_file);
	return 1;
}

stock lang_read(filename[])
{
	new File:fohnd,
		tmpres[MAX_STRING+32], tmp1[4],
		i, pos_s;
	
	fohnd = fopen(filename, io_read);
	if (!fohnd)
	{
		return GameMSG("ERROR: No such language file '%s'", filename);
	}
	while (fread(fohnd, tmpres, sizeof(tmpres)))
	{
		if (strlen(tmpres) == 0 || strfind(tmpres, "//", true) == 0) continue;
		pos_s = strfind(tmpres, ":", true);
		if (pos_s != -1)
		{
			lang_FixSpecialChar(tmpres);
			strmid(tmp1, tmpres, 0, pos_s);
			strmid(lang_texts[i][strval(tmp1)], tmpres, pos_s+2, strlen(tmpres) - pos_s);
		}
		else
		{
			i++;
		}
	}
	fclose(fohnd);
	return 1;
}

stock lang_FixSpecialChar(string[MAX_STRING+32])
{
	for (new i = 0; string[i] != '\0'; i++) {
		if (string[i] != '[' || string[i + 2] != ']') {
			continue;
		}
		switch (string[i+1]) {
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
