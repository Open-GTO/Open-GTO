/*

	About: interface system
	Author: ziggi

*/

#if defined _player_interface_included
	#endinput
#endif

#define _player_interface_included

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

stock TogglePlayerInterfaceVisibility(playerid, bool:show)
{
	for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
		if (show && !gPlayerInterface[playerid][PlayerInterface:pinterface][PIP_Visible]) {
			continue;
		}

		CallLocalFunction("OnPlayerInterfaceChanged", "iiiii", playerid, pinterface, _:PIP_Visible, 0, _:show);
	}
}

stock PlayerTD_Update(playerid, PlayerText:textdraw, color, value, prefix[] = "")
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
