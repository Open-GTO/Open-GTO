/*
	
	About: save/load config system
	Author: ziggi

*/

#if defined _cfg_included
  #endinput
#endif

#define _cfg_included
#pragma library cfg


stock cfg_LoadConfigs()
{
	if (!ini_fileExist(db_config)) {
		return 0;
	}
	
	new file_config = ini_openFile(db_config);

	lang_LoadConfig(file_config);
	Log_LoadConfig(file_config);
	Time_LoadConfig(file_config);
	groundhold_LoadConfig(file_config);
	gang_LoadConfig(file_config);
	mission_LoadConfig(file_config);
	payday_LoadConfig(file_config);
	account_LoadConfig(file_config);
	player_LoadConfig(file_config);
	Vehicle_LoadConfig(file_config);
	Vehicle_Fuel_LoadConfig(file_config);
	weapon_LoadConfig(file_config);
	bank_LoadConfig(file_config);
	business_LoadConfig(file_config);
	houses_LoadConfig(file_config);
	admin_LoadConfig(file_config);

	// player/
	pl_level_LoadConfig(file_config);
	wdrop_LoadConfig(file_config);
	wskill_LoadConfig(file_config);

	lottery_LoadConfig(file_config);

	// protections
	pt_idle_LoadConfig(file_config);
	pt_ping_LoadConfig(file_config);
	pt_chat_LoadConfig(file_config);
	pt_spac_LoadConfig(file_config);
	pt_rcon_LoadConfig(file_config);
	pt_speed_LoadConfig(file_config);
	pt_weapon_LoadConfig(file_config);
	pt_health_LoadConfig(file_config);
	pt_armour_LoadConfig(file_config);
	pt_vehtp_LoadConfig(file_config);

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
	
	lang_SaveConfig(file_config);
	Log_SaveConfig(file_config);
	Time_SaveConfig(file_config);
	groundhold_SaveConfig(file_config);
	gang_SaveConfig(file_config);
	mission_SaveConfig(file_config);
	payday_SaveConfig(file_config);
	account_SaveConfig(file_config);
	player_SaveConfig(file_config);
	Vehicle_Fuel_SaveConfig(file_config);
	Vehicle_SaveConfig(file_config);
	weapon_SaveConfig(file_config);
	bank_SaveConfig(file_config);
	business_SaveConfig(file_config);
	houses_SaveConfig(file_config);
	admin_SaveConfig(file_config);

	// player/
	pl_level_SaveConfig(file_config);
	wdrop_SaveConfig(file_config);
	wskill_SaveConfig(file_config);

	lottery_SaveConfig(file_config);

	// protections
	pt_idle_SaveConfig(file_config);
	pt_ping_SaveConfig(file_config);
	pt_chat_SaveConfig(file_config);
	pt_spac_SaveConfig(file_config);
	pt_rcon_SaveConfig(file_config);
	pt_speed_SaveConfig(file_config);
	pt_weapon_SaveConfig(file_config);
	pt_health_SaveConfig(file_config);
	pt_armour_SaveConfig(file_config);
	pt_vehtp_SaveConfig(file_config);

	ini_closeFile(file_config);
}
