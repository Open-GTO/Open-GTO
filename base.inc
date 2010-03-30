//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//

#if defined _base_included
  #endinput
#endif

#define _base_included
#pragma library base

#include "utils\mxINI"

#define VERSION "v0.6.2"

#define MAX_STRING 256 // !!!НЕ МЕНЯТЬ!!!, а то мод будет плющить(ZiGGi)
#define MAX_NAME 40 // number of char's a name/password/misc_word can be
#define INACTIVE_PLAYER_ID 255
#define INVALID_QUEST_ID 0
#define MAX_QUESTS 255
#define MAX_WIN 900000 // Maximum gambling win
#define MAX_SPAWNPOINTS 50
#define MAX_COUNTDOWN 5
#define MAX_PING 500
#define KEY_USING KEY_WALK // Кнопка 'Ходьба' - отвечает за открытие диалогов(банк, покупка чегонибудь...), входов/выходов


#define TimerTick 1000 // used for how often timer function is called

#define COLOUR_GREY 0xAFAFAFAA // grey
#define COLOUR_GREEN 0x33AA33AA // green
#define COLOUR_LIGHTGREEN 0x9ACD32AA //lightgreen
#define COLOUR_RED 0xAA3333AA // red
#define COLOUR_LIGHTRED 0xFF6347AA // lightred
#define COLOUR_YELLOW 0xFFFF00AA // yellow
#define COLOUR_WHITE 0xFFFFFFAA // white
#define COLOUR_BLUE 0x33AAFFAA // blue
#define COLOUR_LIGHTBLUE 0x33CCFFAA // LightBlue
#define COLOUR_ORANGE 0xFF9900AA //orange
#define COLOUR_PURPLE 0xC2A2DAAA //purple
#define COLOUR_CRIMSON 0xDC143CAA
#define COLOUR_LIMEGREEN 0x32CD32AA
#define COLOUR_AQUA 0x00FFFFAA
#define COLOUR_FLBLUE 0x6495EDAA
#define COLOUR_MAGENTA 0xFF00FFFF
#define COLOUR_GOLD 0xB8860BAA
#define COLOUR_PINK 0xFFC0CBAA
#define COLOUR_MISC 0xAFAFAFAA // Grey - Misc
#define COLOUR_CHAT 0xFFFFFFAA // White - Chat
#define COLOUR_PLAYER 0xAAAAAAAA // White - Player
#define COLOUR_XP_GOOD 0x00C7FFAA // Blue - XP
#define COLOUR_XP_BAD 0x00008BAA // Blue - XP
#define COLOUR_COMBAT_GOOD 0xAA3333AA // Red - Combat
#define COLOUR_COMBAT_BAD 0xBB3333AA // Red - Combat
#define COLOUR_MONEY_GOOD 0xFFFF00AA // Yellow - Money
#define COLOUR_MONEY_BAD 0xDAA520AA // Yellow - Money
#define COLOUR_RACE 0x005407AA // green - dark
#define COLOUR_RACE_BAD 0x545407AA // yellow-green-dark
#define COLOUR_GANG 0xFF8C00AA // orange
#define COLOUR_GANG_CHAT 0xFFA500AA // orange
#define COLOUR_DM 0xFF0066AA // red - purple
#define COLOUR_DM_BAD 0xFF0066AA // red - purple

#define MIN_HEALTH 20
#define MAX_HEALTH 100
#define NUMVALUES 4	

#define SOUND_MUSIC_CLASSSELECT      1185
#define SOUND_MUSIC_RACEFINISH       1183
#define SOUND_MUSIC_RACERECORD       1097
#define SOUND_MUSIC_MISSIONCOMPLETE  1185
#define SOUND_OFF                    1184
#define SOUND_BLIP1                  1056
#define SOUND_BLIP2                  1057
#define SOUND_EXPLOSION              1159
#define SOUND_PURCHASE               1149
#define SOUND_CHECKPOINT             1058
#define SOUND_DEATH                 1057
#define SOUND_DMKILL                 1056
#define SOUND_DMDEATH                 1057

#define OUTPUTLEVEL 0 // Amount of imformation outputted to console
//              0   -   off, no output
//              1   -   + Minimal output
//              2   -   + DB saves, etc
//              3   -   + XP/Level/Money changes output

#define DBTYPE 0 //	Database type (olny flat file ATM)
//				0	-	dini - flat file
//				1	-	TODO: SQL

forward WeatherUpdate();

new ConfigDB[MAX_STRING] = "GTO/Config/Base-cfg.txt";
new LevelDB[MAX_STRING] = "GTO/Config/Levels-cfg.txt";
new VerboseSave=-1; // do we save extra un-needed db info like weapons, races, vehicels, deathmatches, etc
new RealTime=0;
new ForceOverwrite = 0;
new KickList[MAX_PLAYERS];
new WorldTime;
new DBSeperator= 124; // seperate db fields.atm for db writing im just sending a string, which is written to a file.
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

BaseLoadConfig()
{
	new temp[MAX_STRING],file;
	if(!ini_Exist(ConfigDB))
	{
		file = ini_Open(ConfigDB);
		ini_Get(file,"Level_DB",LevelDB);
		ini_GetInt(file,"Verbose_Save",VerboseSave);
		ini_GetInt(file,"Real_Time",RealTime);
		ini_Close(file);
	}
	if(!ini_Exist(LevelDB)) BaseSaveConfig();
	for(new i=0;i<=MAX_LEVEL;i++)
	{
	    file = ini_Open(LevelDB);
	    format(temp,sizeof(temp),"Level%d_Rankname",i);
		ini_Get(file,temp,LevelList[i][level_name],MAX_STRING);
		format(temp,sizeof(temp),"Level%d_Experience",i);
		ini_GetInt(file,temp,LevelList[i][level_xp]);
		ini_Close(file);
	}
	return;
}

BaseSaveConfig()
{
	new temp[MAX_STRING],file;
	if(ini_Exist(ConfigDB))
	{
	    file = ini_Open(ConfigDB);
		ini_Set(file,"Level_DB",LevelDB);
		ini_SetInt(file,"Verbose_Save",VerboseSave);
		ini_SetInt(file,"Real_Time",RealTime);
		ini_Close(file);
	}
	
	if(!ini_Exist(LevelDB)) {new File:fhnd = fopen(LevelDB,io_write);fclose(fhnd);}
	for(new i=0;i<=MAX_LEVEL;i++)
	{
	    file = ini_Open(LevelDB);
	    format(temp,sizeof(temp),"Level%d_Rankname",i);
		ini_Set(file,temp,LevelList[i][level_name]);
		format(temp,sizeof(temp),"Level%d_Experience",i);
		ini_SetInt(file,temp,LevelList[i][level_xp]);
		ini_Close(file);
	}
	return;
}

stock base_OnGameModeInit()
{
	BaseLoadConfig();
	//EnableTirePopping(1);
	//EnableZoneNames(1);
	UsePlayerPedAnims();
	AllowAdminTeleport(1);
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	return 1;
}

GetLevelName(level)
{
	new name[MAX_NAME];
	if ((level < 0) || (level > MAX_LEVEL+1)) return name;
	set(name,LevelList[level][level_name]);
	return name;
}
