//
// Created: 27.04.2010
// Aurthor: ZiGGi
//

#if defined _mod_func_included
 #endinput
#endif

#define _mod_func_included
#pragma library mod_func

stock modfunc_OnPlayerEnterVehicle(playerid,vehicleid,ispassenger)
{	
	#pragma unused ispassenger
	for(new id=0;id<MAX_PLAYERS;id++)
	{
		if(IsPlayerConnected(id) && Player[id][Spectating] && Player[id][SpecID] == playerid) PlayerSpectateVehicle(id,vehicleid);
	}
	return 1;
}

stock modfunc_OnPlayerExitVehicle(playerid,vehicleid)
{
	#pragma unused vehicleid
	for(new id=0;id<MAX_PLAYERS;id++)
	{
		if(IsPlayerConnected(id) && Player[id][Spectating] && Player[id][SpecID] == playerid) PlayerSpectatePlayer(id,playerid);
	}
	return 1;
}

stock modfunc_OnPlayerInteriorChange(playerid,newinteriorid,oldinteriorid)
{
	#pragma unused oldinteriorid
	for(new id=0;id<MAX_PLAYERS;id++)
	{
		if(IsPlayerConnected(id) && Player[id][Spectating] && Player[id][SpecID] == playerid)	SetPlayerInterior(id,newinteriorid);
	}
	return 1;
}

stock modfunc_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason,killerid
	for(new id=0;id<MAX_PLAYERS;id++) 
	{
		if(IsPlayerConnected(id) && Player[id][SpecID] == playerid  && Player[id][Spectating]) 
		{ 
			TogglePlayerSpectating(id,false); 
			Player[id][Spectating] = false; 
			Player[id][SpecID] = INVALID_PLAYER_ID; 
		}
	}
	return 1;
}

stock modfunc_OnPlayerText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	if(text[0] == '#')
	{
		new string[MAX_STRING];
		strmid(string,text,1,strlen(text));
		new count = 0;
		for(new adminid=0; adminid<MAX_PLAYERS;adminid++)
		{
			if(IsPlayerMod(adminid))
			{
				new message[MAX_STRING];
				format(message,sizeof(message), lang_texts[12][79] ,oGetPlayerName(playerid),string);
				SendPlayerFormattedText(adminid,message, 0,COLOUR_RED);
				count++;
			}
		}
		if(count == 0)
		{
			SendPlayerFormattedText(playerid, lang_texts[12][80] , 0,COLOUR_RED);
			return 0;
		} 
		else 
		{
			new message[MAX_STRING];
			format(message,sizeof(message), lang_texts[12][81] ,string);
			SendPlayerFormattedText(playerid,message, 0,COLOUR_GREEN);
			return 0;
		}
	}
	return 0;
}