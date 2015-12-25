/*

	About: admin hide system
	Author:	ziggi

*/

#if defined _adm_hide_included
	#endinput
#endif

#define _adm_hide_included
#pragma library adm_hide


stock PushHide(playerid)
{
	new hidecont = (admin_GetHideStatus(playerid) == 1) ? 0 : GetPlayerGangColor(playerid);

	foreach (new id : Player) {
		if (id != playerid) {
			SetPlayerMarkerForPlayer(id, playerid, hidecont);
		}
	}

	return 1;
}

stock admin_GetHideStatus(playerid) {
	return GetPVarInt(playerid, "Hide");
}

stock admin_SetHideStatus(playerid, hidestatus) {
	SetPVarInt(playerid, "Hide", hidestatus);
	PushHide(playerid);
}

COMMAND:ahideme(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}
	
	admin_SetHideStatus(playerid, 1);
	SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][25]);
	return 1;
}

COMMAND:ashoweme(playerid, params[])
{
	if (!IsPlayerRconAdmin(playerid)) {
		return 0;
	}
	
	admin_SetHideStatus(playerid, 0);
	SendClientMessage(playerid, COLOR_WHITE, lang_texts[12][26]);
	return 1;
}
