/*

	Author: ziggi

*/

#if defined _admin_included
	#endinput
#endif

#define _admin_included
#pragma library admin


stock admin_SaveConfig(file_config)
{
	adm_maptp_SaveConfig(file_config);
}

stock admin_LoadConfig(file_config)
{
	adm_maptp_LoadConfig(file_config);
}

stock admin_OnPlayerText(playerid, text[])
{
	new mute_result = adm_mute_OnPlayerText(playerid, text);
	if (mute_result == 0) {
		SendClientMessage(playerid, COLOUR_RED, _(MUTED_HELP_MESSAGE));
	}
	return mute_result;
}

stock admin_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	adm_godmod_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
	return 1;
}

stock admin_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	adm_spec_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	return 1;
}

stock admin_OnPlayerExitVehicle(playerid, vehicleid)
{
	adm_spec_OnPlayerExitVehicle(playerid, vehicleid);
	return 1;
}

stock admin_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	adm_spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	return 1;
}

stock admin_OnPlayerDeath(playerid, killerid, reason)
{
	adm_spec_OnPlayerDeath(playerid, killerid, reason);
	return 1;
}

stock admin_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	adm_maptp_OnPlayerClickMap(playerid, fX, fY, fZ);
	return 1;
}

stock admin_OnRconLoginAttempt(ip[], password[], success)
{
	adm_login_OnRconLoginAttempt(ip, password, success);
	return 1;
}

stock SendClientMessageToAdmins(senderid, text[])
{
	new message[MAX_STRING],
		count = 0;
	
	format(message, sizeof(message), lang_texts[12][28], ReturnPlayerName(senderid), senderid, text);
	foreach (new adminid : Player) {
		if (IsPlayerAdm(adminid)) {
			SendClientMessage(adminid, COLOUR_RED, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, COLOUR_RED, lang_texts[12][29]);
	} else {
		format(message, sizeof(message), lang_texts[12][30], text);
		SendClientMessage(senderid, COLOUR_GREEN, message);
	}
	return 0;
}

stock SendClientMessageToModers(senderid, text[])
{
	new message[MAX_STRING],
		count = 0;

	format(message, sizeof(message), lang_texts[12][79], ReturnPlayerName(senderid), senderid, text);
	foreach (new adminid : Player) {
		if (IsPlayerMod(adminid)) {
			SendClientMessage(adminid, COLOUR_RED, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, COLOUR_RED, lang_texts[12][80]);
	} else {
		format(message, sizeof(message), lang_texts[12][81], text);
		SendClientMessage(senderid, COLOUR_GREEN, message);
	}
	return 0;
}
