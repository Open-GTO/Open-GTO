//
// Created: 27.04.2010
// Aurthor: ZiGGi
//

#if defined _mod_func_included
 #endinput
#endif

#define _mod_func_included
#pragma library mod_func

stock modfunc_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{	
	#pragma unused ispassenger
	foreach (Player, id)
	{
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid)
		{
			PlayerSpectateVehicle(id, vehicleid);
		}
	}
	return 1;
}

stock modfunc_OnPlayerExitVehicle(playerid, vehicleid)
{
	#pragma unused vehicleid
	foreach (Player, id)
	{
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid)
		{
			PlayerSpectatePlayer(id, playerid);
		}
	}
	return 1;
}

stock modfunc_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	#pragma unused oldinteriorid
	foreach (Player, id)
	{
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid)
		{
			SetPlayerInterior(id, newinteriorid);
		}
	}
	return 1;
}

stock modfunc_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason, killerid
	foreach (Player, id)
	{
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid)
		{
			TogglePlayerSpectating(id, 0);
			player_SetSpectating(id, 0);
			player_SetSpecID(id, INVALID_PLAYER_ID);
			SetPlayerInterior(playerid, GetPVarInt(id, "spec_interior"));
			DeletePVar(id, "spec_interior");
		}
	}
	return 1;
}

stock SendClientMessageToModers(senderid, text[])
{
	new message[MAX_STRING], count = 0;
	format(message, sizeof(message), lang_texts[12][79], oGetPlayerName(senderid), senderid, text);
	foreach (Player, adminid)
	{
		if (IsPlayerMod(adminid))
		{
			SendClientMessage(adminid, COLOUR_RED, message);
			count++;
		}
	}
	if (count == 0)
	{
		SendClientMessage(senderid, COLOUR_RED, lang_texts[12][80]);
		return 0;
	} 
	else 
	{
		format(message, sizeof(message), lang_texts[12][81], text);
		SendClientMessage(senderid, COLOUR_GREEN, message);
		return 0;
	}
}
