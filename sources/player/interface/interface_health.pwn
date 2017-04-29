/*

	About: player health interface system
	Author:	ziggi

*/

#if defined _player_health_int_included
	#endinput
#endif

#define _player_health_int_included

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	PlayerHealthTD_CreateTextDraw(playerid);

	#if defined PlayerHealthTD_OnPlayerConnect
		return PlayerHealthTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PlayerHealthTD_OnPlayerConnect
#if defined PlayerHealthTD_OnPlayerConnect
	forward PlayerHealthTD_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	PlayerHealthTD_UpdateString(playerid);

	#if defined PlayerHealthTD_OnPlayerSpawn
		return PlayerHealthTD_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn PlayerHealthTD_OnPlayerSpawn
#if defined PlayerHealthTD_OnPlayerSpawn
	forward PlayerHealthTD_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerTakeDamage
*/

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new
		Float:health;

	GetPlayerHealth(playerid, health);

	PlayerHealthTD_UpdateString(playerid, health - amount);

	#if defined PlayerHealthTD_OnPlayerTakeDmg
		return PlayerHealthTD_OnPlayerTakeDmg(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif

#define OnPlayerTakeDamage PlayerHealthTD_OnPlayerTakeDmg
#if defined PlayerHealthTD_OnPlayerTakeDmg
	forward PlayerHealthTD_OnPlayerTakeDmg(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

/*
	OnPlayerInterfaceChanged
*/

public OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue)
{
	if (!PlayerHealthTD_IsValidComponent(componentid)) {
	#if defined PlayerHealthTD_OnPlayerIntChng
		return PlayerHealthTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
	}

	if (paramid == PIP_Visible) {
		if (newvalue) {
			PlayerHealthTD_UpdateString(playerid);
		} else {
			PlayerHealthTD_HideTextDraw(playerid);
		}
	}

	#if defined PlayerHealthTD_OnPlayerIntChng
		return PlayerHealthTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInterfaceChanged
	#undef OnPlayerInterfaceChanged
#else
	#define _ALS_OnPlayerInterfaceChanged
#endif

#define OnPlayerInterfaceChanged PlayerHealthTD_OnPlayerIntChng
#if defined PlayerHealthTD_OnPlayerIntChng
	forward PlayerHealthTD_OnPlayerIntChng(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);
#endif

/*
	Functions
*/

stock PlayerHealthTD_CreateTextDraw(playerid)
{
	new
		PlayerText:td_temp;

	td_temp = CreatePlayerTextDraw(playerid, 577.0, 67.0, "_");
	PlayerTextDrawLetterSize(playerid, td_temp, 0.34, 0.8);
	PlayerTextDrawAlignment(playerid, td_temp, 2);
	PlayerTextDrawColor(playerid, td_temp, 0xFFFFFFFF);
	PlayerTextDrawSetOutline(playerid, td_temp, 1);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 255);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_Health, PIP_TextDraw, td_temp);
}

stock PlayerHealthTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Health, PIP_TextDraw));
}

stock PlayerHealthTD_IsValidComponent(PlayerInterface:componentid)
{
	switch (componentid) {
		case PI_Health: {
			return 1;
		}
	}
	return 0;
}

stock PlayerHealthTD_ShowTextDraw(playerid)
{
	if (!GetPlayerInterfaceParam(playerid, PI_Health, PIP_Visible) || !IsPlayerInterfaceVisible(playerid)) {
		return;
	}

	PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Health, PIP_TextDraw));
}

stock PlayerHealthTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Health, PIP_TextDraw));
}

stock PlayerHealthTD_UpdateString(playerid, Float:health = -1.0)
{
	if (!GetPlayerInterfaceParam(playerid, PI_Health, PIP_Visible) || !IsPlayerInterfaceVisible(playerid)) {
		return;
	}

	new
		string[4];

	if (IsPlayerGodmod(playerid)) {
		Lang_GetPlayerText(playerid, "ADMIN_GODMOD_INTERFACE_TEXT", string);
	} else {
		if (health == -1.0) {
			GetPlayerHealth(playerid, health);
		}

		format(string, sizeof(string), "%.0f", health);
	}

	if (health <= 0.0) {
		PlayerHealthTD_HideTextDraw(playerid);
	} else {
		PlayerTextDrawSetString(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_Health, PIP_TextDraw), string);

		PlayerHealthTD_ShowTextDraw(playerid);
	}
}
