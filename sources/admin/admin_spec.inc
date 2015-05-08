/*

	About: admin spec system
	Author:	ziggi

*/

#if defined _adm_spec_included
	#endinput
#endif

#define _adm_spec_included
#pragma library adm_spec


#define ADMIN_SPEC_LABEL_COLOR 0x20B2AAFF

static PlayerText3D:spec_label[MAX_PLAYERS];

forward adm_spec_UpdateLabel(playerid);


COMMAND:spec(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		if (Spectate_IsSpectating(playerid)) {
			Spectate_Stop(playerid);
		} else {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][19]);
		}
		return 1;
	}

	new specid = strval(params);
	if (!IsPlayerConnected(specid) || specid == playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][20]);
		return 1;
	}

	Spectate_Start(playerid, specid);
	
	SendClientMessage(playerid, COLOUR_BLUE, lang_texts[13][21]);
	return 1;
}
