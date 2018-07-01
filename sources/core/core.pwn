/*

	About: gto_core functions
	Author: ziggi

*/

#if defined _gto_core_included
	#endinput
#endif

#define _gto_core_included


/*
	Core_OnGameModeInit
*/

Core_OnGameModeInit()
{
	SetGameModeText(GAMEMODE_TEXT);
#if defined USE_PLAYER_PED_ANIMS
	UsePlayerPedAnims();
#endif
	ShowPlayerMarkers(USED_PLAYER_MARKERS);
	ShowNameTags(1);
	ManualVehicleEngineAndLights();
	EnableStuntBonusForAll(0);

	new string[MAX_LANG_VALUE_STRING];
	Lang_GetText(Lang_Get("ru"), "PLAYER_MENU_SETTINGS_RUSSIFIER_TEXT", string);
	Russifier_SetText(string);

	Log_Init("core", "Core module init.");
}
