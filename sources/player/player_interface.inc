/*

	About: interface header system
	Author: ziggi

*/

#if defined _player_interface_h_included
	#endinput
#endif

#define _player_interface_h_included

/*
	Enums
*/

enum PlayerInterfaceParams {
	PIP_TextDraw,
	PIP_Visible,
}

enum PlayerInterface {
	PI_Armour,
	PI_Health,
	PI_Info,
	PI_LevelLineBackground,
	PI_LevelLine,
	PI_LevelLevel,
	PI_LevelPlus,
	PI_LevelXP,
	PI_LevelXPPlus,
	PI_MoneyBorder,
	PI_MoneyBackground,
	PI_MoneyMoney,
	PI_MoneyPlus,
	PI_WeaponSkill,
}

/*
	Forwards
*/

forward OnPlayerInterfaceChanged(playerid, PlayerInterface:componentid, PlayerInterfaceParams:paramid, oldvalue, newvalue);

/*
	Vars
*/

new
	gPlayerInterface[MAX_PLAYERS][PlayerInterface][PlayerInterfaceParams];
