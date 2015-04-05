/*

	About: directories base
	Author: ziggi

*/

#if defined _db_included
  #endinput
#endif

#define _db_included
#pragma library db


new db_config[] = DATA_FILES_FOLDER "config/base-cfg"DATA_FILES_FORMAT;
new db_level[] = DATA_FILES_FOLDER "config/levels-cfg"DATA_FILES_FORMAT;
new db_gang[] = DATA_FILES_FOLDER "gang/";
new db_account[] = DATA_FILES_FOLDER "account/";
new db_player[] = DATA_FILES_FOLDER "player/";
new db_houses[] = DATA_FILES_FOLDER "housing/";
new db_business[] = DATA_FILES_FOLDER "business/";
new db_weapon[] = DATA_FILES_FOLDER "weapon/";
new db_groundhold[] = DATA_FILES_FOLDER "groundhold/";
new db_ban[] = DATA_FILES_FOLDER "banned/";

new db_deathmatch[] = DATA_FILES_FOLDER "deathmatch/!deathmatches"DATA_FILES_FORMAT;
new db_dm_base[] = DATA_FILES_FOLDER "deathmatch/";
new db_dm_record[] = DATA_FILES_FOLDER "deathmatch/record/";

new db_race[] = DATA_FILES_FOLDER "race/!races"DATA_FILES_FORMAT;
new db_race_base[] = DATA_FILES_FOLDER "race/";
new db_race_record[] = DATA_FILES_FOLDER "race/record/";
