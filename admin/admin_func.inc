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

stock SendMessageToAdmins(playerid, color, text[])
{
	new string[MAX_STRING],message[MAX_STRING];
	strmid(string,text,0,strlen(text));
	format(message,sizeof(message),lang_texts[1][24],oGetPlayerName(playerid),playerid,string);
	printf("%s",message);
	for(new aid=0;aid<MAX_PLAYERS;aid++)
	{
		if(IsPlayerAdmin(aid) && IsPlayerAdm(aid))
		{
			if(aid != playerid) 
			{
				format(message,sizeof(message), lang_texts[1][23] ,oGetPlayerName(playerid), playerid, Player[playerid][Status], string);
				SendClientMessage(aid,color,message);
			}
		}
	}
	return 0;
}

stock PushHide(playerid)// скрыть/показать игрока
{
	new hidecont;
	if(Player[playerid][Hide] > 0) hidecont = 0;
	else hidecont = PlayerGangColour(playerid);
	for(new id=0;id<MAX_PLAYERS;id++)
	{
		if(IsPlayerConnected(id) && !IsPlayerAdmin(id) && (id != playerid)) SetPlayerMarkerForPlayer(id,playerid,hidecont);
	}
	SetPlayerColor(playerid,PlayerGangColour(playerid));
	return 1;
}

stock LoadHide(playerid) // скрытие игроков при входе в игру
{
	if(Player[playerid][Hide] == 1)	PushHide(playerid);
	for(new id=0;id<MAX_PLAYERS;id++)	//Скрываем игроку всех кто имеет на это право...
	{
		if(IsPlayerConnected(id) && (Player[id][Hide] == 1) && !IsPlayerAdmin(playerid)) SetPlayerMarkerForPlayer(playerid,id,0);
	}
	return 1;
}

stock BanCheck(playerid)
{
    new player[MAX_PLAYER_NAME],string[MAX_STRING],filestr[MAX_STRING];
    GetPlayerName(playerid,player,sizeof player);
	format(string,sizeof(string),"GTO/Bans/Names/%s.txt",player);
	format(player,sizeof(player),"Banned");
	new File:file = fopen(string,io_read);
	if(file)
	{
	    while(fread(file,filestr,sizeof(filestr)))
	    {
	    	if(strfind(filestr,"Banned",true) != -1)
	    	{
	    	    SendClientMessage(playerid,red,"XpaП: Ou caaaiai ia na?aa?a NOOEA!");
	    	    return Kick(playerid);
	    	}
		}
	}
	return true;
}