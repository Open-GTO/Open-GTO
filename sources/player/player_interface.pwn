/*

	About: interface system
	Author: ziggi

*/

#if defined _player_interface_included
	#endinput
#endif

#define _player_interface_included

/*
	Variables
*/

static
	bool:gPlayerInterfaceVisible[MAX_PLAYERS];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	for (new i = 0; i < sizeof(gPlayerInterface[]); i++) {
		for (new j = 0; j < sizeof(gPlayerInterface[][]); j++) {
			gPlayerInterface[playerid][PlayerInterface:i][PlayerInterfaceParams:j] = 0;
		}
	}
	TogglePlayerInterfaceVisibility(playerid, true, true);

	#if defined PInter_OnPlayerConnect
		return PInter_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PInter_OnPlayerConnect
#if defined PInter_OnPlayerConnect
	forward PInter_OnPlayerConnect(playerid);
#endif

/*
	OnAccountLanguageChanged
*/

public OnAccountLanguageChanged(playerid, Lang:lang)
{
	PlayerLevelTD_UpdateString(playerid);

	#if defined PInter_OnAccountLanguageChanged
		return PInter_OnAccountLanguageChanged(playerid, Lang:lang);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnAccountLanguageChanged
	#undef OnAccountLanguageChanged
#else
	#define _ALS_OnAccountLanguageChanged
#endif

#define OnAccountLanguageChanged PInter_OnAccountLanguageChanged
#if defined PInter_OnAccountLanguageChanged
	forward PInter_OnAccountLanguageChanged(playerid, Lang:lang);
#endif

/*
	OnPlayerRussifierSelect
*/

public OnPlayerRussifierSelect(playerid, bool:changed, RussifierType:type)
{
	if (changed) {
		PlayerLevelTD_UpdateString(playerid);
	}

	#if defined PInter_OnPlayerRussifierSelect
		return PInter_OnPlayerRussifierSelect(playerid, changed, type);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerRussifierSelect
	#undef OnPlayerRussifierSelect
#else
	#define _ALS_OnPlayerRussifierSelect
#endif

#define OnPlayerRussifierSelect PInter_OnPlayerRussifierSelect
#if defined PInter_OnPlayerRussifierSelect
	forward PInter_OnPlayerRussifierSelect(playerid, bool:changed, RussifierType:type);
#endif

/*
	Functions
*/

stock GetPlayerInterfaceParam(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid)
{
	return gPlayerInterface[playerid][componentid][paramid];
}

stock SetPlayerInterfaceParam(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, {PlayerText, bool, _}:value)
{
	new oldvalue = gPlayerInterface[playerid][componentid][paramid];
	gPlayerInterface[playerid][componentid][paramid] = _:value;

	if (oldvalue != _:value) {
		CallLocalFunction("OnPlayerInterfaceChanged", "iiiii", playerid, _:componentid, _:paramid, oldvalue, _:value);
	}
}

stock EncodePlayerInterfaceData(playerid, PlayerInterfaceParams:paramid)
{
	new
		data;

	for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
		data |= GetPlayerInterfaceParam(playerid, PlayerInterface:pinterface, paramid) << pinterface;
	}

	return data;
}

stock DecodePlayerInterfaceData(playerid, PlayerInterfaceParams:paramid, data)
{
	for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
		SetPlayerInterfaceParam(playerid, PlayerInterface:pinterface, paramid, data >> pinterface & 1);
	}
}

stock TogglePlayerInterfaceVisibility(playerid, bool:show, bool:skip_init = false, PlayerInterface:...)
{
	gPlayerInterfaceVisible[playerid] = show;

	if (skip_init) {
		return;
	}

	new args_count = numargs();

	if (args_count > 3) {
		for (new i = 3, pinterface; i < args_count; i++) {
			pinterface = getarg(i);

			if (show && !gPlayerInterface[playerid][PlayerInterface:pinterface][PIP_Visible]) {
				continue;
			}

			CallLocalFunction("OnPlayerInterfaceChanged", "iiiii", playerid, pinterface, _:PIP_Visible, 0, _:show);
		}
	} else {
		for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
			if (show && !gPlayerInterface[playerid][PlayerInterface:pinterface][PIP_Visible]) {
				continue;
			}

			CallLocalFunction("OnPlayerInterfaceChanged", "iiiii", playerid, pinterface, _:PIP_Visible, 0, _:show);
		}
	}
}

stock IsPlayerInterfaceVisible(playerid)
{
	return _:gPlayerInterfaceVisible[playerid];
}

stock PlayerTD_Update(playerid, PlayerText:textdraw, color, value, const prefix[] = "")
{
	static
		str[MAX_LANG_VALUE_STRING];

	FormatNumberToString(str, sizeof(str), value);

	if (!isnull(prefix)) {
		format(str, sizeof(str), "%s%s", prefix, str);
	}

	PlayerTextDrawHide(playerid, textdraw);
	PlayerTextDrawColor(playerid, textdraw, color);
	PlayerTextDrawSetString(playerid, textdraw, str);
	PlayerTextDrawShow(playerid, textdraw);
}
