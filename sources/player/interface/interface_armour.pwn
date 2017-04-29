/*

	About: player armour interface system
	Author:	ziggi

*/

#if defined _player_armour_int_included
	#endinput
#endif

#define _player_armour_int_included

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	PlayerArmourTD_CreateTextDraw(playerid);

	#if defined PlayerArmourTD_OnPlayerConnect
		return PlayerArmourTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PlayerArmourTD_OnPlayerConnect
#if defined PlayerArmourTD_OnPlayerConnect
	forward PlayerArmourTD_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerArmourTD_UpdateString(playerid);

	#if defined PlayerArmourTD_OnPlayerSpawn
		return PlayerArmourTD_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn PlayerArmourTD_OnPlayerSpawn
#if defined PlayerArmourTD_OnPlayerSpawn
	forward PlayerArmourTD_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerTakeDamage
*/

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new
		Float:armour;

	GetPlayerArmour(playerid, armour);

	if (issuerid != INVALID_PLAYER_ID) {
		armour -= amount;
	}

	PlayerArmourTD_UpdateString(playerid, armour);

	#if defined PlayerArmourTD_OnPlayerTakeDmg
		return PlayerArmourTD_OnPlayerTakeDmg(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif

#define OnPlayerTakeDamage PlayerArmourTD_OnPlayerTakeDmg
#if defined PlayerArmourTD_OnPlayerTakeDmg
	forward PlayerArmourTD_OnPlayerTakeDmg(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	OnPlayerInterfaceChanged
*/

public OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue)
{
	if (!PlayerArmourTD_IsValidComponent(componentid)) {
	#if defined PlayerArmourTD_OnPlayerIntChng
		return PlayerArmourTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
	}

	if (paramid == PIP_Visible) {
		if (newvalue) {
			PlayerArmourTD_UpdateString(playerid);
		} else {
			PlayerArmourTD_HideTextDraw(playerid);
		}
	}

	#if defined PlayerArmourTD_OnPlayerIntChng
		return PlayerArmourTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInterfaceChanged
	#undef OnPlayerInterfaceChanged
#else
	#define _ALS_OnPlayerInterfaceChanged
#endif

#define OnPlayerInterfaceChanged PlayerArmourTD_OnPlayerIntChng
#if defined PlayerArmourTD_OnPlayerIntChng
	forward PlayerArmourTD_OnPlayerIntChng(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);
#endif

/*
	Functions
*/

stock PlayerArmourTD_CreateTextDraw(playerid)
{
	new
		PlayerText:td_temp;

	td_temp = CreatePlayerTextDraw(playerid, 577.0, 45.0, "_");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.34, 0.8);
	PlayerTextDrawAlignment(playerid, td_temp, 2);
	PlayerTextDrawColor(playerid, td_temp, 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, td_temp, 1);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 255);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_Armour, PIP_TextDraw, td_temp);
}

stock PlayerArmourTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Armour, PIP_TextDraw));
}

stock PlayerArmourTD_IsValidComponent(PlayerInterface:componentid)
{
	switch (componentid) {
		case PI_Armour: {
			return 1;
		}
	}
	return 0;
}

stock PlayerArmourTD_ShowTextDraw(playerid)
{
	if (!GetPlayerInterfaceParam(playerid, PI_Armour, PIP_Visible) || !IsPlayerInterfaceVisible(playerid)) {
		return;
	}

	PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Armour, PIP_TextDraw));
}

stock PlayerArmourTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Armour, PIP_TextDraw));
}

stock PlayerArmourTD_UpdateString(playerid, Float:armour = -1.0)
{
	if (!GetPlayerInterfaceParam(playerid, PI_Armour, PIP_Visible) || !IsPlayerInterfaceVisible(playerid)) {
		return;
	}

	new
		string[4];

	if (armour == -1.0) {
		GetPlayerArmour(playerid, armour);
	}

	if (armour <= 0.0) {
		PlayerArmourTD_HideTextDraw(playerid);
	} else {
		format(string, sizeof(string), "%.0f", armour);

		PlayerTextDrawSetString(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Armour, PIP_TextDraw), string);

		PlayerArmourTD_ShowTextDraw(playerid);
	}
}
