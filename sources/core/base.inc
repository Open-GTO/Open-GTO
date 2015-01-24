//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//

#if defined _base_included
  #endinput
#endif

#define _base_included
#pragma library base


new ConfigDB[MAX_STRING] = GTO_FILES_FOLDER "Config/Base-cfg"GTO_FILES_FORMAT;
new LevelDB[MAX_STRING] = GTO_FILES_FOLDER "Config/Levels-cfg"GTO_FILES_FORMAT;
new GangDB[MAX_STRING] = GTO_FILES_FOLDER "Gang/";
new AccountDB[MAX_STRING] = GTO_FILES_FOLDER "Account/";
new PlayerDB[MAX_STRING] = GTO_FILES_FOLDER "Player/";
new HousesDB[MAX_STRING] = GTO_FILES_FOLDER "Housing/";
new BusinessDB[MAX_STRING] = GTO_FILES_FOLDER "Business/";
new WeaponDB[MAX_STRING] = GTO_FILES_FOLDER "Weapon/";
new GroundholdDB[MAX_STRING] = GTO_FILES_FOLDER "Groundhold/";
new BanDB[MAX_STRING] = GTO_FILES_FOLDER "Banned/";

new DeathmatchDB[MAX_STRING] = GTO_FILES_FOLDER "Deathmatch/!Deathmatches"GTO_FILES_FORMAT;
new DMBaseDB[MAX_STRING] = GTO_FILES_FOLDER "Deathmatch/";
new DMRecordDB[MAX_STRING] = GTO_FILES_FOLDER "Deathmatch/Record/";

new RaceDB[MAX_STRING] = GTO_FILES_FOLDER "Race/!Races"GTO_FILES_FORMAT;
new RaceBaseDB[MAX_STRING] = GTO_FILES_FOLDER "Race/";
new RaceRecordDB[MAX_STRING] = GTO_FILES_FOLDER "Race/Record/";

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
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	ManualVehicleEngineAndLights();
	EnableStuntBonusForAll(0);
	GameMSG("SERVER: Base module init");
	return 1;
}
