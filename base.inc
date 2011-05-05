//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//

#if defined _base_included
  #endinput
#endif

#define _base_included
#pragma library base


new ConfigDB[MAX_STRING] = "GTO/Config/Base-cfg.txt";
new LevelDB[MAX_STRING] = "GTO/Config/Levels-cfg.txt";
new GangDB[MAX_STRING] = "GTO/Gang/";
new AccountDB[MAX_STRING] = "GTO/Account/";
new PlayerDB[MAX_STRING] = "GTO/Player/";
new HousesDB[MAX_STRING] = "GTO/Housing/";
new BusinessDB[MAX_STRING] = "GTO/Business/";
new WeaponDB[MAX_STRING] = "GTO/Weapon/";
new GroundholdDB[MAX_STRING] = "GTO/Groundhold/";

new DeathmatchDB[MAX_STRING] = "GTO/Deathmatch/!Deathmatches.txt";
new DMBaseDB[MAX_STRING] = "GTO/Deathmatch/";
new DMRecordDB[MAX_STRING] = "GTO/Deathmatch/Record/";

new RaceDB[MAX_STRING] = "GTO/Race/!Races.txt";
new RaceBaseDB[MAX_STRING] = "GTO/Race/";
new RaceRecordDB[MAX_STRING] = "GTO/Race/Record/";

new VerboseSave=-1; // do we save extra un-needed db info like weapons, races, vehicels, deathmatches, etc
new RealTime=0;
new ForceOverwrite = 0;
new KickList[MAX_PLAYERS];
new WorldTime;
new nullstr[MAX_STRING];

enum CoordInfo 
{
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z
}

enum LevelInfo 
{
	level_xp,
    level_name[MAX_NAME]
}

#define MAX_LEVEL 60
#define MAX_XP  60000000
#define MAX_LVLXP 60000000

new LevelList[MAX_LEVEL+1][LevelInfo] = { // experience required for each level
{  0,		"0 уровень" },
{  500,		"1 уровень" },
{  1000,	"2 уровень" },
{  2000,	"3 уровень" },
{  5000,	"4 уровень" },
{  7500,	"5 уровень" },
{  11000,	"6 уровень" },
{  16000,	"7 уровень" },
{  21000,	"8 уровень" },
{  31000,	"9 уровень" },
{  46000,	"10 уровень" },
{  70000,	"11 уровень" },
{  100000,	"12 уровень" },
{  135000,	"13 уровень" },
{  190000,	"14 уровень" },
{  225000,	"15 уровень" },
{  300000,	"16 уровень" },
{  380000,	"17 уровень" },
{  480000,	"18 уровень" },
{  590000,	"19 уровень" },
{  700000, 	"20 уровень" },
{  820000,	"21 уровень" },
{  950000,	"22 уровень" },
{  1200000,	"23 уровень" },
{  1350000,	"24 уровень" },
{  1420000, "25 уровень" },
{  1500000, "26 уровень" },
{  1600000, "27 уровень" },
{  1700000, "28 уровень" },
{  1800000, "29 уровень" },
{  2500000, "30 уровень" },
{  3000000, "31 уровень" },
{  3500000, "32 уровень" },
{  4000000, "33 уровень" },
{  5500000, "34 уровень" },
{  6000000, "35 уровень" },
{  6500000, "36 уровень" },
{  7000000, "37 уровень" },
{  7500000, "38 уровень" },
{  8000000, "39 уровень" },
{  10500000, "40 уровень" },
{  12000000, "41 уровень" },
{  13000000, "42 уровень" },
{  14200000, "43 уровень" },
{  15400000, "44 уровень" },
{  16600000, "45 уровень" },
{  17800000, "46 уровень" },
{  19000000, "47 уровень" },
{  21000000, "48 уровень" },
{  23000000, "49 уровень" },
{  26000000, "50 уровень" },
{  29000000, "51 уровень" },
{  32000000, "52 уровень" },
{  35000000, "53 уровень" },
{  38000000, "54 уровень" },
{  41000000, "55 уровень" },
{  43000000, "56 уровень" },
{  46000000, "57 уровень" },
{  50000000, "58 уровень" },
{  55000000, "59 уровень" },
{  60000000, "60 уровень" }
};

stock BaseLoadConfig()
{
	if(ini_Exist(ConfigDB))
	{
		new file_base = ini_Open(ConfigDB);
		ini_Get(file_base,"Level_DB",LevelDB);
		ini_GetInt(file_base,"Real_Time",RealTime);
		ini_GetInt(file_base,"Verbose_Save",VerboseSave);
		ini_Close(file_base);
	}
	if(ini_Exist(LevelDB))
	{
		new file_base_level = ini_Open(LevelDB);
		for(new i=0,temp[MAX_STRING];i<=MAX_LEVEL;i++)
		{
			format(temp,sizeof(temp),"Level%d_Experience",i);
			ini_GetInt(file_base_level,temp,LevelList[i][level_xp]);
			format(temp,sizeof(temp),"Level%d_Rankname",i);
			ini_Get(file_base_level,temp,LevelList[i][level_name],MAX_STRING);
		}
		ini_Close(file_base_level);
	}
	return 1;
}

stock BaseSaveConfig()
{
	new file_base = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_base,"Level_DB",LevelDB);
	ini_SetInt(file_base,"Real_Time",RealTime);
	ini_SetInt(file_base,"Verbose_Save",VerboseSave);
	ini_Close(file_base);

	new file_base_level = (ini_Exist(LevelDB)) ? ini_Open(LevelDB) : ini_Create(LevelDB);
	for(new i=0,temp[MAX_STRING];i<=MAX_LEVEL;i++)
	{
	    format(temp,sizeof(temp),"Level%d_Rankname",i);
		ini_Set(file_base_level,temp,LevelList[i][level_name]);
		format(temp,sizeof(temp),"Level%d_Experience",i);
		ini_SetInt(file_base_level,temp,LevelList[i][level_xp]);
	}
	ini_Close(file_base_level);
	return 1;
}

stock base_OnGameModeInit()
{
	BaseLoadConfig();
	UsePlayerPedAnims();
	AllowAdminTeleport(1);
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	return 1;
}

GetLevelName(level)
{
	new name[MAX_NAME];
	if((level < 0) || (level > MAX_LEVEL+1)) return name;
	set(name,LevelList[level][level_name]);
	return name;
}

stock show_Msg_Dialog(playerid,string_head[],string_content[],button[])
{
	return ShowPlayerDialog(playerid,message_DialogID,DIALOG_STYLE_MSGBOX,
		string_head,
		string_content,
		button,""
	);
}