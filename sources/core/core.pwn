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

	Log_Game("SERVER: Core module init");
}
