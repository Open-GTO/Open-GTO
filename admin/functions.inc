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

new Float:Jail_PH[][CoordInfoJ] = {
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
	if(GetPlayerJailed(playerid) != 1) return 0;
	new jail_time = GetPlayerJailTime(playerid);
	SetPlayerWantedLevel(playerid,(jail_time == 0 || jail_time > 6) ? 6 : jail_time);
	// телепортируем в тюрьму
	SetPlayerInterior(playerid,6);
	SetPlayerPos(playerid,265.1273,77.6823,1001.0391);
	SetPlayerFacingAngle(playerid,-90);	
	TogglePlayerControllable(playerid,0);
	SetCameraBehindPlayer(playerid);
	SetPlayerArmour(playerid, 0);
	PlayerPlaySound(playerid,1082,198.3797,160.8905,1003.0300);
	return 1;
}

stock UnJailPlayer(playerid)
{
	if(GetPlayerJailed(playerid) != 0) return 0;
	SetPlayerJailTime(playerid,0);
	SetPlayerWantedLevel(playerid, 0);
	TogglePlayerControllable(playerid,1);
	// ставим игрока в рандомное место из всех мест освобождений
	new j_id = random( sizeof(Jail_PH) );
	SetPlayerInterior(playerid,0);
	SetPlayerPos(playerid, Jail_PH[j_id][CJ_X], Jail_PH[j_id][CJ_Y], Jail_PH[j_id][CJ_Z] );
	SetPlayerFacingAngle(playerid,Jail_PH[j_id][CJ_A]);
	SetCameraBehindPlayer(playerid);
	new string[MAX_STRING];
	format(string,sizeof(string), "~r~%s ~w~Now is Free", oGetPlayerName(playerid));
	GameTextForAll(string,5000,4);
	format(string,sizeof(string), "SERVER: %s был выпущен из тюрьмы (истекло время или амнистия)", oGetPlayerName(playerid));
	SendClientMessageToAll(COLOUR_WHITE, string);
	return 1;
}


stock SendMessageToAdmins(playerid, color, text[])
{
	new message[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(message,sizeof(message),lang_texts[1][24],playername,playerid,text);
	printf("%s",message);
	format(message,sizeof(message),lang_texts[1][23],playername,playerid,GetPlayerStatus(playerid),text);
	foreach(Player,aid)
	{
		if(aid == playerid || !IsPlayerAdm(aid)) continue;
		SendClientMessage(aid,color,message);
	}
	return 0;
}

stock PushHide(playerid)// скрыть/показать игрока
{
	new hidecont = (GetPVarInt(playerid,"Hide") == 1) ? 0 : PlayerGangColour(playerid);
	foreach(Player,id)
	{
		if(id != playerid)
			SetPlayerMarkerForPlayer(id,playerid,hidecont);
	}
	//SetPlayerColor(playerid,PlayerGangColour(playerid));
	return 1;
}

stock MuteCheck()
{
	new string[MAX_STRING];
	foreach(Player,i)
	{
		new mute_time = GetPlayerMuteTime(i);
		if (mute_time > 0)
		{
			mute_time--;
			SetPlayerMuteTime(i,mute_time);
		    if (mute_time <= 0)
		    {
				format(string,sizeof(string),"Игрок %s(%d) снова может говорить",oGetPlayerName(i),i);
				SendClientMessageToAll(COLOUR_YELLOW,string);
			}
		}
	}
}

stock JailTimer()
{
	new string[MAX_STRING];
	foreach (Player,i)
	{
		if (GetPlayerJailed(i) != 1) continue;
		
		new jail_time = GetPlayerJailTime(i);
		SetPlayerWantedLevel(i,(jail_time == 0 || jail_time > 6) ? 6 : jail_time);
		
		if (1 <= jail_time <= 4320)
		{
			jail_time--;
			SetPlayerJailTime(i,jail_time);
			if (jail_time <= 0)
			{
				SetPlayerJailed(i,0);
				SetPlayerJailTime(i,0);
				UnJailPlayer(i);
				printf("SERVER : %s Has Been auto-UNJailed",oGetPlayerName(i));
			}
			else
			{
				format(string,sizeof(string),"SERVER: До окончания ареста осталось около %d мин",jail_time);
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

stock IsPlayerRconAdmin(playerid)
{
	return (GetPlayerStatus(playerid) >= 3) ? 1 : 0;
}

stock IsPlayerAdm(playerid)
{
	return (GetPlayerStatus(playerid) >= 2) ? 1 : 0;
}

stock IsPlayerMod(playerid)
{
	return (GetPlayerStatus(playerid) >= 1) ? 1 : 0;
}
