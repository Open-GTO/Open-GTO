//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//

#if defined _base_included
  #endinput
#endif

#define _base_included
#pragma library base


new ConfigDB[MAX_STRING] = GTO_FILES_FOLDER"Config/Base-cfg"GTO_FILES_FORMAT;
new LevelDB[MAX_STRING] = GTO_FILES_FOLDER"Config/Levels-cfg"GTO_FILES_FORMAT;
new GangDB[MAX_STRING] = GTO_FILES_FOLDER"Gang/";
new AccountDB[MAX_STRING] = GTO_FILES_FOLDER"Account/";
new PlayerDB[MAX_STRING] = GTO_FILES_FOLDER"Player/";
new HousesDB[MAX_STRING] = GTO_FILES_FOLDER"Housing/";
new BusinessDB[MAX_STRING] = GTO_FILES_FOLDER"Business/";
new WeaponDB[MAX_STRING] = GTO_FILES_FOLDER"Weapon/";
new GroundholdDB[MAX_STRING] = GTO_FILES_FOLDER"Groundhold/";

new DeathmatchDB[MAX_STRING] = GTO_FILES_FOLDER"Deathmatch/!Deathmatches"GTO_FILES_FORMAT;
new DMBaseDB[MAX_STRING] = GTO_FILES_FOLDER"Deathmatch/";
new DMRecordDB[MAX_STRING] = GTO_FILES_FOLDER"Deathmatch/Record/";

new RaceDB[MAX_STRING] = GTO_FILES_FOLDER"Race/!Races"GTO_FILES_FORMAT;
new RaceBaseDB[MAX_STRING] = GTO_FILES_FOLDER"Race/";
new RaceRecordDB[MAX_STRING] = GTO_FILES_FOLDER"Race/Record/";

new VerboseSave=-1; // do we save extra un-needed db info like weapons, races, vehicels, deathmatches, etc
new RealTime=0;
new KickList[MAX_PLAYERS];
new WorldTime;
new nullstr[MAX_STRING];

enum CoordInfo {
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z
}

stock BaseLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_base = ini_Open(ConfigDB);
	ini_GetInt(file_base,"Real_Time",RealTime);
	ini_GetInt(file_base,"Verbose_Save",VerboseSave);
	ini_Close(file_base);
	return 1;
}

stock BaseSaveConfig()
{
	new file_base = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_base,"Real_Time",RealTime);
	ini_SetInt(file_base,"Verbose_Save",VerboseSave);
	ini_Close(file_base);
	return 1;
}

stock base_OnGameModeInit()
{
	BaseLoadConfig();
	UsePlayerPedAnims();
	AllowAdminTeleport(1);
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	ManualVehicleEngineAndLights();
	GameMSG("SERVER: Base module init");
	return 1;
}

stock show_Msg_Dialog(playerid,string_head[],string_content[],button[])
{
	return ShowPlayerDialog(playerid,message_DialogID,DIALOG_STYLE_MSGBOX,
		string_head,
		string_content,
		button,""
	);
}
