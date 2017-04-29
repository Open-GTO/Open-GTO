/*

	About: admin spectate system
	Author:	ziggi

*/

#if defined _admin_spectate_included
	#endinput
#endif

#define _admin_spectate_included

/*
	Defines
*/

#define ADMIN_SPECTATE_LABEL_COLOR 0x20B2AAFF

/*
	Vars
*/

static
	Text3D:gSpectateLabel[MAX_PLAYERS];

/*
	Functions
*/

AdminSpectate_OnPlayerSpectate(playerid, specid, status)
{
	if (status == 0) {
		AdminSpectate_DestroyLabel(playerid);
	} else {
		AdminSpectate_CreateLabel(playerid, specid);
	}
}

stock AdminSpectate_CreateLabel(playerid, specid)
{
	static
		string[MAX_LANG_VALUE_STRING * MAX_PLAYER_INFO_LINES];

	GetPlayerInfoString(specid, string, sizeof(string), playerid);

	gSpectateLabel[playerid] = CreateDynamic3DTextLabel(string, ADMIN_SPECTATE_LABEL_COLOR, 0.0, 0.0, -1.5, 20.0, .playerid = playerid, .attachedplayer = specid);
}

stock AdminSpectate_DestroyLabel(playerid)
{
	DestroyDynamic3DTextLabel(gSpectateLabel[playerid]);
	gSpectateLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
}

forward AdminSpectate_UpdateLabel(playerid);
public AdminSpectate_UpdateLabel(playerid)
{
	if (!Spectate_IsSpectating(playerid) || gSpectateLabel[playerid] == Text3D:INVALID_3DTEXT_ID) {
		return 0;
	}

	static
		string[MAX_LANG_VALUE_STRING * MAX_PLAYER_INFO_LINES];

	GetPlayerInfoString(Spectate_GetSpecID(playerid), string, sizeof(string), playerid);

	UpdateDynamic3DTextLabelText(gSpectateLabel[playerid], ADMIN_SPECTATE_LABEL_COLOR, string);
	return 1;
}
