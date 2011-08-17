/*
	
	Updated in 10.06.2011 by ZiGGi
	
*/

#if defined _lang_included
	#endinput
#endif

#define _lang_included
#pragma library lang


new lang_db[10] = GTO_FILES_FOLDER"";
new lang[10] = LANGUAGE_USE;
new lang_texts[20][99][MAX_STRING];

stock lang_LoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_lang = ini_Open(ConfigDB);
    ini_Get(file_lang,"Language",lang);
	ini_Close(file_lang);
	return 1;
}

stock lang_SaveConfig()
{
	new file_lang = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_lang,"Language",lang);
	ini_Close(file_lang);
	return 1;
}

stock lang_OnGameModeInit()
{
	lang_LoadConfig();
	new lang_file[50];
	format(lang_file,sizeof(lang_file),"%sLanguage-%s"GTO_FILES_FORMAT,lang_db,lang);
	lang_read(lang_file);
	GameMSG("SERVER: Lang module init(%s)", lang_file);
	return 1;
}

stock lang_read(filename[])
{
	new File:fohnd,
		tmpres[MAX_STRING],tmp1[MAX_STRING],tmp2[MAX_STRING],
		i,len,pos_s;
	fohnd=fopen(filename,io_read);
	if(!fohnd) return GameMSG("ERROR: No such language file '%s'", filename);
	while(fread(fohnd,tmpres,sizeof(tmpres)))
	{
		len = strlen(tmpres);
		if(len == 0) continue;
		if(strfind(tmpres, "//",true) == 0) continue;
		pos_s = strfind(tmpres, ":", true);
		if(pos_s != -1)
		{
			strmid(tmp1, tmpres, 0, pos_s);
			strmid(tmp2, tmpres, pos_s+2, len-strlen(tmp1));
			
			while( check_BB(tmp2) == 1) {}
			
			lang_texts[i][strval(tmp1)] = tmp2;
		}
		else i++;
	}
	fclose(fohnd);
	return 1;
}

stock check_BB(string[MAX_STRING])
{
	new buf;
	if( (buf = strfind(string,LANG_BB_N)) != -1 )
	{
		strdel(string,buf,buf+strlen(LANG_BB_N));
		strins(string,"\n",buf);
		return 1;
	}
	return 0;
}