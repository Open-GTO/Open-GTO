//
// Created: 27.04.2010
// Aurthor: ZiGGi
//

#if defined _admin_func_included
 #endinput
#endif

#define _admin_func_included
#pragma library admin_func

stock adminfunc_OnPlayerText(playerid,text[])
{
	if(text[0] == '@')
	{
		new string[MAX_STRING],count = 0;
		strmid(string,text,1,strlen(text));
		for(new adminid=0;adminid<MAX_PLAYERS;adminid++)
		{
			if(IsPlayerAdmin(adminid) && IsPlayerAdm(adminid))
			{
				new message[MAX_STRING];
				format(message,sizeof(message),lang_texts[12][28],oGetPlayerName(playerid), string);
				SendClientMessage(adminid,COLOUR_RED,message);
				count++;
			}
		}
		if(count == 0)
		{
			SendClientMessage(playerid,COLOUR_RED,lang_texts[12][29]);
			return 0;
		} 
		else 
		{
			new message[MAX_STRING];
			format(message,sizeof(message),lang_texts[12][30],string);
			SendClientMessage(playerid,COLOUR_GREEN,message);
			return 0;
		}
	}
	return 0;
}
