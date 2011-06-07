/*

	11.05.2011
	ZiGGi

*/

#if defined _admod_functions_included
	#endinput
#endif

#define _admod_functions_included
#pragma library admod_functions


enum CoordInfoJ {
    Float:CJ_X,
    Float:CJ_Y,
    Float:CJ_Z,
    Float:CJ_A
}
#define UNJP 8
new Float:JPH[UNJP][CoordInfoJ] = {
{633.365, -571.78, 16.340, 265.773},
{1544.982, -1675.470, 13.600, 93.446},
{-2164.396, -2388.342, 30.650, 140.551},
{-1605.378, 717.512, 12.000, 310.972},
{-1391.040, 2634.686, 55.984, 116.935},
{-215.718, 985.399, 19.400, 240.854},
{2335.229, 2455.809, 14.968, 136.734},
{1225.165, 245.328, 19.554, 306.501}
};

stock JailPlayer(playerid)
{
	if(GetPlayerJailed(playerid) == 1)
	{
		SetPlayerInterior(playerid,6);
		SetPlayerPos(playerid,265.1273,77.6823,1001.0391);
		SetPlayerFacingAngle(playerid,-90);
		new spwl=Player[playerid][JailTime];
		if(spwl==0 || spwl>6) spwl=6;
		SetPlayerWantedLevel(playerid,spwl);
		TogglePlayerControllable(playerid,0);
		SetCameraBehindPlayer(playerid);
		SetPlayerArmour(playerid, 0);
	}
}

stock UnJailPlayer(playerid)
{
	if(GetPlayerJailed(playerid) == 0)
	{
		SetPlayerJailed(playerid,0);
		Player[playerid][JailTime] = 0;
	}
	new u = random(UNJP);
	SetPlayerInterior(playerid,0);
	SetPlayerPos(playerid, JPH[u][CJ_X], JPH[u][CJ_Y], JPH[u][CJ_Z] );
	SetPlayerFacingAngle(playerid,JPH[u][CJ_A]);
	SetCameraBehindPlayer(playerid);
	SetPlayerWantedLevel(playerid, 0);
	TogglePlayerControllable(playerid,1);
	new string[MAX_STRING];
	format(string,sizeof(string), "~r~%s ~w~Now is Free", oGetPlayerName(playerid));
	GameTextForAll(string,5000,4);
	format(string,sizeof(string), "SERVER: %s был Выпущен из тюрьмы (Истекло время или Амнистия)", oGetPlayerName(playerid));
	SendClientMessageToAll(COLOUR_WHITE, string);
}


stock SendMessageToAdmins(playerid, color, text[])
{
	new string[MAX_STRING],message[MAX_STRING];
	strmid(string,text,0,strlen(text));
	format(message,sizeof(message),lang_texts[1][24],oGetPlayerName(playerid),playerid,string);
	printf("%s",message);
	for(new aid=0;aid<=GetPlayerLastID();aid++)
	{
		if(!IsPlayerConnected(aid) || IsPlayerNPC(aid) || aid == playerid || (!IsPlayerAdmin(aid) && !IsPlayerAdm(aid))) continue;
		format(message,sizeof(message), lang_texts[1][23] ,oGetPlayerName(playerid), playerid, Player[playerid][Status], string);
		SendClientMessage(aid,color,message);
	}
	return 0;
}

stock PushHide(playerid)// скрыть/показать игрока
{
	new hidecont;
	if(GetPVarInt(playerid,"Hide") > 0)
		hidecont = 0;
	else
		hidecont = PlayerGangColour(playerid);
	for(new id=0;id<=GetPlayerLastID();id++)
	{
		if(IsPlayerConnected(id) && !IsPlayerNPC(id) && !IsPlayerAdmin(id) && id != playerid)
			SetPlayerMarkerForPlayer(id,playerid,hidecont);
	}
	SetPlayerColor(playerid,PlayerGangColour(playerid));
	return 1;
}

stock LoadHide(playerid) // скрытие игроков при входе в игру
{
	if(GetPVarInt(playerid,"Hide") == 1) PushHide(playerid);
	for(new id=0;id<=GetPlayerLastID();id++)
	{
		if(IsPlayerConnected(id) && !IsPlayerNPC(id) && (GetPVarInt(playerid,"Hide") == 1) && !IsPlayerAdmin(playerid))
			SetPlayerMarkerForPlayer(playerid,id,0);
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

stock MuteCheck()
{
	for(new i=0;i<=GetPlayerLastID();i++)
	{
		if(!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
		if(Player[i][MuteTime] > 0)
		{
		    Player[i][MuteTime]--;
		    if(Player[i][MuteTime] == 0)
		    {
				new string[MAX_STRING];
				format(string, sizeof(string)," Игрок %s(%d) снова может говорить",oGetPlayerName(i),i);
				SendClientMessageToAll(COLOUR_YELLOW,string);
			}
		}
	}
}

stock JailTimer()
{
	for(new i=0;i<=GetPlayerLastID();i++)
	{
		if(!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
		if(GetPlayerJailed(i) == 1)
		{
			new spwl = Player[i][JailTime];
			if(spwl == 0 || spwl > 6) spwl = 6;
			SetPlayerWantedLevel(i,spwl);
		}
		if(GetPlayerJailed(i) == 1 && Player[i][JailTime] >= 1)
		{
			Player[i][JailTime]--;
			if(Player[i][JailTime] <= 0)
			{
				SetPlayerJailed(i,0);
				Player[i][JailTime] = 0;
				UnJailPlayer(i);
				printf("SERVER : %s Has Been auto-UNJailed",oGetPlayerName(i));
				return 1;
			}
			else
			{
				new string[MAX_STRING];
				format(string,sizeof(string), "SERVER: До окончания ареста осталось около %d мин", Player[i][JailTime]);
				SendClientMessage(i,COLOUR_LIGHTRED, string);
			}
		}
	}
	return 1;
}

stock IsSkinValid(SkinID)
{
	switch(SkinID)
	{
		case 0,7,9..41,43..64,66..73,75..85,87..118,120..148,150..207,209..264,274..288,290..299: return true;
	}
	return false;
}