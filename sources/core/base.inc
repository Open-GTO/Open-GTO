//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//

#if defined _base_included
  #endinput
#endif

#define _base_included
#pragma library base


new ConfigDB[MAX_STRING] = DATA_FILES_FOLDER "config/base-cfg"DATA_FILES_FORMAT;
new LevelDB[MAX_STRING] = DATA_FILES_FOLDER "config/levels-cfg"DATA_FILES_FORMAT;
new GangDB[MAX_STRING] = DATA_FILES_FOLDER "gang/";
new AccountDB[MAX_STRING] = DATA_FILES_FOLDER "account/";
new PlayerDB[MAX_STRING] = DATA_FILES_FOLDER "player/";
new HousesDB[MAX_STRING] = DATA_FILES_FOLDER "housing/";
new BusinessDB[MAX_STRING] = DATA_FILES_FOLDER "business/";
new WeaponDB[MAX_STRING] = DATA_FILES_FOLDER "weapon/";
new GroundholdDB[MAX_STRING] = DATA_FILES_FOLDER "groundhold/";
new BanDB[MAX_STRING] = DATA_FILES_FOLDER "banned/";

new DeathmatchDB[MAX_STRING] = DATA_FILES_FOLDER "deathmatch/!deathmatches"DATA_FILES_FORMAT;
new DMBaseDB[MAX_STRING] = DATA_FILES_FOLDER "deathmatch/";
new DMRecordDB[MAX_STRING] = DATA_FILES_FOLDER "deathmatch/record/";

new RaceDB[MAX_STRING] = DATA_FILES_FOLDER "race/!races"DATA_FILES_FORMAT;
new RaceBaseDB[MAX_STRING] = DATA_FILES_FOLDER "race/";
new RaceRecordDB[MAX_STRING] = DATA_FILES_FOLDER "race/record/";

new nullstr[MAX_STRING];

enum CoordInfo {
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z
}

stock base_OnGameModeInit()
{
	SetGameModeText("Open-GTO "#VERSION);
	UsePlayerPedAnims();
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
	ShowNameTags(1);
	ManualVehicleEngineAndLights();
	EnableStuntBonusForAll(0);
	Log_Game("SERVER: Base module init");
	return 1;
}
