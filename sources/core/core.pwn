/*
	
	About: gto_core functions
	Author: ziggi

*/

#if defined _gto_core_included
	#endinput
#endif

#define _gto_core_included
#pragma library gto_core


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
	
	new rcon_command[64];
	rcon_command = lang_GetLang();
	format(rcon_command, sizeof(rcon_command), "language %c%s", toupper(rcon_command[0]), rcon_command[1]);
	SendRconCommand(rcon_command);
	Log_Game("SERVER: Core module init");
}
