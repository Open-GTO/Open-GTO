/*

	About: admin pm system
	Author:	ziggi

*/

#if defined _adm_pm_included
	#endinput
#endif

#define _adm_pm_included
#pragma library adm_pm


COMMAND:showpm(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	if (GetPVarInt(playerid, "Admin_PMshowing") != 1) {
		SetPVarInt(playerid, "Admin_PMshowing", 1);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][28]);
	} else {
		SetPVarInt(playerid, "Admin_PMshowing", 0);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][29]);
	}
	return 1;
}