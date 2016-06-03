/*

	About: admin spec system
	Author:	ziggi

*/

#if defined _adm_spec_included
	#endinput
#endif

#define _adm_spec_included


#define ADMIN_SPEC_LABEL_COLOR 0x20B2AAFF

static Text3D:spec_label[MAX_PLAYERS];

forward adm_spec_UpdateLabel(playerid);

adm_spec_OnPlayerSpectate(playerid, specid, status)
{
	if (status == 0) {
		adm_spec_DestroyLabel(playerid);
	} else {
		adm_spec_CreateLabel(playerid, specid);
	}
}

stock adm_spec_CreateLabel(playerid, specid)
{
	new
		string[MAX_LANG_VALUE_STRING * MAX_PLAYER_INFO_LINES];

	GetPlayerInfoString(specid, string, sizeof(string), playerid);

	spec_label[playerid] = CreateDynamic3DTextLabel(string, ADMIN_SPEC_LABEL_COLOR, 0.0, 0.0, -1.5, 20.0, .playerid = playerid, .attachedplayer = specid);
}

stock adm_spec_DestroyLabel(playerid)
{
	DestroyDynamic3DTextLabel(spec_label[playerid]);
	spec_label[playerid] = Text3D:INVALID_3DTEXT_ID;
}

public adm_spec_UpdateLabel(playerid)
{
	if (!Spectate_IsSpectating(playerid) || spec_label[playerid] == Text3D:INVALID_3DTEXT_ID) {
		return 0;
	}

	new
		string[MAX_LANG_VALUE_STRING * MAX_PLAYER_INFO_LINES];

	GetPlayerInfoString(Spectate_GetSpecID(playerid), string, sizeof(string), playerid);

	UpdateDynamic3DTextLabelText(spec_label[playerid], ADMIN_SPEC_LABEL_COLOR, string);
	return 1;
}
