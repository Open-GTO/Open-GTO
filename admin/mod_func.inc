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
	for(new id=0;id<=GetPlayerLastID();id++)
	{
		if(IsPlayerConnected(id) && GetPlayerSpectating(id) == 1 && SetPlayerSpecID(id,playerid)) PlayerSpectateVehicle(id,vehicleid);
	}
	return 1;
}

stock modfunc_OnPlayerExitVehicle(playerid,vehicleid)
{
	#pragma unused vehicleid
	for(new id=0;id<=GetPlayerLastID();id++)
	{
		if(IsPlayerConnected(id) && GetPlayerSpectating(id) == 1 && SetPlayerSpecID(id,playerid)) PlayerSpectatePlayer(id,playerid);
	}
	return 1;
}

stock modfunc_OnPlayerInteriorChange(playerid,newinteriorid,oldinteriorid)
{
	#pragma unused oldinteriorid
	for(new id=0;id<=GetPlayerLastID();id++)
	{
		if(IsPlayerConnected(id) && GetPlayerSpectating(id) == 1 && SetPlayerSpecID(id,playerid))	SetPlayerInterior(id,newinteriorid);
	}
	return 1;
}

stock modfunc_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason,killerid
	for(new id=0;id<=GetPlayerLastID();id++)
	{
		if(IsPlayerConnected(id) && SetPlayerSpecID(id,playerid) && GetPlayerSpectating(id) == 1) 
		{ 
			TogglePlayerSpectating(id,false);
			SetPlayerSpectating(id,0);
			SetPlayerSpecID(id,INVALID_PLAYER_ID);
		}
	}
	return 1;
}

stock SendClientMessageToModers(senderid,text[])
{
	new message[MAX_STRING],count = 0;
	format(message,sizeof(message),lang_texts[12][79],oGetPlayerName(senderid),senderid,text);
	for(new adminid=0;adminid<=GetPlayerLastID();adminid++)
	{
		if(IsPlayerMod(adminid))
		{
			SendClientMessage(adminid,COLOUR_RED,message);
			count++;
		}
	}
	if(count == 0)
	{
		SendClientMessage(senderid,COLOUR_RED,lang_texts[12][80]);
		return 0;
	} 
	else 
	{
		format(message,sizeof(message),lang_texts[12][81],text);
		SendClientMessage(senderid,COLOUR_GREEN,message);
		return 0;
	}
}
