/*

	About: player weapon skill interface system
	Author:	ziggi

*/

#if defined _player_wskill_int_included
	#endinput
#endif

#define _player_wskill_int_included

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	if (IsWeaponSkillEnabled()) {
		PlayerWSkillTD_CreateTextDraw(playerid);
	}

	#if defined PlayerWSkillTD_OnPlayerConnect
		return PlayerWSkillTD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PlayerWSkillTD_OnPlayerConnect
#if defined PlayerWSkillTD_OnPlayerConnect
	forward PlayerWSkillTD_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerDeath
*/

public OnPlayerDeath(playerid, killerid, reason)
{
	if (IsWeaponSkillEnabled()) {
		PlayerWSkillTD_HideTextDraw(playerid);
	}

	#if defined PlayerWSkillTD_OnPlayerDeath
		return PlayerWSkillTD_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath PlayerWSkillTD_OnPlayerDeath
#if defined PlayerWSkillTD_OnPlayerDeath
	forward PlayerWSkillTD_OnPlayerDeath(playerid, killerid, reason);
#endif

/*
	OnPlayerRequestClass
*/

public OnPlayerRequestClass(playerid, classid)
{
	if (IsWeaponSkillEnabled()) {
		PlayerWSkillTD_HideTextDraw(playerid);
	}

	#if defined PlayerWSkillTD_OnPlayerReqClass
		return PlayerWSkillTD_OnPlayerReqClass(playerid, classid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerRequestClass
	#undef OnPlayerRequestClass
#else
	#define _ALS_OnPlayerRequestClass
#endif

#define OnPlayerRequestClass PlayerWSkillTD_OnPlayerReqClass
#if defined PlayerWSkillTD_OnPlayerReqClass
	forward PlayerWSkillTD_OnPlayerReqClass(playerid, classid);
#endif

/*
	OnPlayerInterfaceChanged
*/

public OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue)
{
	if (IsWeaponSkillEnabled()) {
		if (componentid == PlayerInterface:PI_WeaponSkill && paramid == PlayerInterfaceParams:PIP_Visible) {
			if (newvalue) {
				PlayerWSkillTD_UpdateString(playerid);
			} else {
				PlayerWSkillTD_HideTextDraw(playerid);
			}
		}
	}

	#if defined PlayerWSkillTD_OnPlayerIntChng
		return PlayerWSkillTD_OnPlayerIntChng(playerid, componentid, paramid, oldvalue, newvalue);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInterfaceChanged
	#undef OnPlayerInterfaceChanged
#else
	#define _ALS_OnPlayerInterfaceChanged
#endif

#define OnPlayerInterfaceChanged PlayerWSkillTD_OnPlayerIntChng
#if defined PlayerWSkillTD_OnPlayerIntChng
	forward PlayerWSkillTD_OnPlayerIntChng(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);
#endif

/*
	Functions
*/

stock PlayerWSkillTD_CreateTextDraw(playerid)
{
	new
		PlayerText:td_temp;

	td_temp = CreatePlayerTextDraw(playerid, 499.000000, 13.000000, "0/" #MAX_WEAPON_SKILL_LEVEL);
	PlayerTextDrawBackgroundColor(playerid, td_temp, 255);
	PlayerTextDrawFont(playerid, td_temp, 1);
	PlayerTextDrawLetterSize(playerid, td_temp, 0.280000, 1.000000);
	PlayerTextDrawColor(playerid, td_temp, -1);
	PlayerTextDrawSetOutline(playerid, td_temp, 0);
	PlayerTextDrawSetProportional(playerid, td_temp, 1);
	PlayerTextDrawSetShadow(playerid, td_temp, 1);

	SetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_TextDraw, td_temp);
}

stock PlayerWSkillTD_DestroyTextDraw(playerid)
{
	PlayerTextDrawDestroy(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_TextDraw));
}

stock PlayerWSkillTD_ShowTextDraw(playerid)
{
	if (!GetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_Visible)) {
		return;
	}

	PlayerTextDrawShow(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_TextDraw));
}

stock PlayerWSkillTD_HideTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, PlayerText:GetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_TextDraw));
}

stock PlayerWSkillTD_UpdateString(playerid)
{
	if (!IsWeaponSkillEnabled()) {
		return;
	}

	if (!GetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_Visible)) {
		return;
	}

	if (!IsPlayerSpawned(playerid)) {
		return;
	}

	static
		PlayerText:td_wskill,
		skillid,
		string[3 + 1 + 3 + 1];

	td_wskill = PlayerText:GetPlayerInterfaceParam(playerid, PI_WeaponSkill, PIP_TextDraw);
	skillid = GetWeaponSkillID( GetPlayerWeapon(playerid) );

	if (skillid != -1) {
		format(string, sizeof(string), "%03d/" #MAX_WEAPON_SKILL_LEVEL, GetPlayerSkillLevel(playerid, skillid));

		PlayerTextDrawSetString(playerid, td_wskill, string);
		PlayerTextDrawShow(playerid, td_wskill);
	} else {
		PlayerTextDrawHide(playerid, td_wskill);
	}
}
