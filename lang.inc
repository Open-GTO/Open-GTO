//
// Updated in 27.03.2010 by ZiGGi
//

#if defined _lang_included
  #endinput
#endif

#define _lang_included
#pragma library lang

#include "base"

new lang_db[255] = "GTO/";
new lang[10] = "Russian";
new lang_texts[20][99][255];

stock lang_OnGameModeInit()
{
	new lang_file[50];
	format(lang_file,sizeof(lang_file),"%sLanguage-%s.txt",lang_db,lang);
	if(!ini_Exist(lang_file)) return printf("ERROR: No such file %s", lang_file);
	lang_read(lang_file);
	return 1;
}

stock lang_read(filename[])
{
	new File:fohnd, tmpres[255], tmp1[255], tmp2[255], i;
	fohnd=fopen(filename,io_read);
	if(!fohnd) return 1;
	while(fread(fohnd,tmpres,sizeof(tmpres)))
	{
		if(strlen(tmpres) == 0) return 1;
		if(strfind(tmpres, "//",true) != -1) continue;
		if(strfind(tmpres, ":", true) != -1)
		{
			strmid(tmp1, tmpres, 0, strfind(tmpres, ":", true));
			strmid(tmp2, tmpres, strfind(tmpres, ":", true)+2, (strlen(tmpres)-strlen(tmp1))); 
			lang_texts[i][strval(tmp1)] = tmp2;
		} 
		else if(strfind(tmpres, ":", true) == -1) i++;
	}
	fclose(fohnd);
	return 1;
}
