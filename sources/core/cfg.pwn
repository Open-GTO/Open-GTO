/*

	About: save/load config system
	Author: ziggi

*/

#if defined _cfg_included
  #endinput
#endif

#define _cfg_included


stock cfg_LoadConfigs()
{
	if (!ini_fileExist(db_config)) {
		return 0;
	}

	new file_config = ini_openFile(db_config);

	Time_LoadConfig(file_config);
	Groundhold_LoadConfig(file_config);
	Gang_LoadConfig(file_config);
	GangLevel_LoadConfig(file_config);
	Mission_LoadConfig(file_config);
	payday_LoadConfig(file_config);
	Account_LoadConfig(file_config);
	Player_LoadConfig(file_config);
	Vehicle_LoadConfig(file_config);
	Vehicle_Fuel_LoadConfig(file_config);
	Weapon_LoadConfig(file_config);
	Bank_LoadConfig(file_config);
	business_LoadConfig(file_config);
	houses_LoadConfig(file_config);
	AdminMapTP_LoadConfig(file_config);
	Weather_LoadConfig(file_config);

	// player/
	PLevel_LoadConfig(file_config);
	PWDrop_LoadConfig(file_config);
	PWSkill_LoadConfig(file_config);

	Lottery_LoadConfig(file_config);

	ini_closeFile(file_config);
	return 1;
}

stock cfg_SaveConfigs()
{
	new file_config;
	if (ini_fileExist(db_config)) {
		file_config = ini_openFile(db_config);
	} else {
		file_config = ini_createFile(db_config);
	}

	Time_SaveConfig(file_config);
	Groundhold_SaveConfig(file_config);
	Gang_SaveConfig(file_config);
	GangLevel_SaveConfig(file_config);
	Mission_SaveConfig(file_config);
	payday_SaveConfig(file_config);
	Account_SaveConfig(file_config);
	Player_SaveConfig(file_config);
	Vehicle_Fuel_SaveConfig(file_config);
	Vehicle_SaveConfig(file_config);
	Weapon_SaveConfig(file_config);
	Bank_SaveConfig(file_config);
	business_SaveConfig(file_config);
	houses_SaveConfig(file_config);
	AdminMapTP_SaveConfig(file_config);
	Weather_SaveConfig(file_config);

	// player/
	PLevel_SaveConfig(file_config);
	PWDrop_SaveConfig(file_config);
	PWSkill_SaveConfig(file_config);

	Lottery_SaveConfig(file_config);

	ini_closeFile(file_config);
}
