/*


*/

#if defined _level_included
	#endinput
#endif

#define _level_included
#pragma library level


#define GetPlayerLevel(%1) GetPVarInt(%1,"Level")
#define SetPlayerLevel(%1,%2) SetPVarInt(%1,"Level",%2)
#define GetPlayerXP(%1) GetPVarInt(%1,"XP")
#define SetPlayerXP(%1,%2) SetPVarInt(%1,"XP",%2)

enum LevelInfo 
{
	level_xp,
    level_name[MAX_NAME]
}

#define MAX_LEVEL	60
#define MAX_XP		60000000

new LevelList[MAX_LEVEL+1][LevelInfo] = {
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

stock level_LoadConfig()
{
	if(ini_Exist(ConfigDB))
	{
		new file_level = ini_Open(ConfigDB);
		ini_Get(file_level,"Level_DB",LevelDB);
		ini_Close(file_level);
	}
	if(ini_Exist(LevelDB))
	{
		new file_base_level = ini_Open(LevelDB);
		for(new i=0,temp[MAX_STRING];i<=MAX_LEVEL;i++)
		{
			format(temp,sizeof(temp),"Level%d_Experience",i);
			ini_GetInt(file_base_level,temp,LevelList[i][level_xp]);
			format(temp,sizeof(temp),"Level%d_Rankname",i);
			ini_Get(file_base_level,temp,LevelList[i][level_name],MAX_NAME);
		}
		ini_Close(file_base_level);
	}
	return 1;
}

stock level_SaveConfig()
{
	new file_level = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_level,"Level_DB",LevelDB);
	ini_Close(file_level);
	
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

stock level_OnGameModeInit()
{
	level_LoadConfig();
	GameMSG("SERVER: (player)Level module init");
	return 1;
}

stock GetLevelName(level)
{
	new name[MAX_NAME];
	if(level < 0 || level > MAX_LEVEL+1) return name;
	set(name,LevelList[level][level_name]);
	return name;
}

stock GetXPToLevel(playerid,level)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if((level <= MAX_LEVEL) && (level > GetPlayerLevel(playerid)))
	{
		return (LevelList[level][level_xp] - GetPlayerXP(playerid));
	}
	else return 0;
}

stock CalculatePlayerLevel(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	new level;
	for(new i;i<=MAX_LEVEL;i++)
	{
		if(GetPlayerXP(playerid) >=LevelList[i][level_xp]) level = i;
	}
	return level;
}

stock GivePlayerXP(playerid, xpamount,showtext=0) // showtext = do we display "You gain %d XP"  to client
{
	if(!IsPlayerConnected(playerid) || xpamount == 0) return 0;
	GangGivePlayerXP(playerid,xpamount);

	new texttime = (xpamount > 5000) ? 2000 : 1000;
	
	if(GetPlayerXP(playerid) + xpamount < 0)
	{
		xpamount = -GetPlayerXP(playerid);
	}
	if(GetPlayerXP(playerid) + xpamount >= MAX_XP)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[9][17] );
		xpamount = MAX_XP - GetPlayerXP(playerid);
	}
	new string[MAX_STRING];
	if(xpamount > 0)
	{
		format(string, sizeof(string),  lang_texts[9][18] , xpamount);
		GameTextForPlayer(playerid,string,texttime,3);
		if(showtext == 1)
		{
			format(string, sizeof(string),  lang_texts[9][20] , xpamount);
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[9][19] , -xpamount);
		GameTextForPlayer(playerid,string,texttime,3);
		if(showtext == 1)
		{
			format(string, sizeof(string),  lang_texts[9][21] , -xpamount);
			SendClientMessage(playerid, COLOUR_XP_BAD, string);
		}
	}
	SetPlayerXP(playerid,GetPlayerXP(playerid) + xpamount);
	CheckPlayerLevel(playerid);
	return 1;
}

stock CheckPlayerLevel(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;
	new level = CalculatePlayerLevel(playerid);
	if(GetPlayerLevel(playerid) < level)
		PlayerLevelUp(playerid,level);
	else if(GetPlayerLevel(playerid) > level)
		PlayerLevelDown(playerid,level);
	return 1;
}

stock PlayerLevelDown(playerid,level)
{
	if(!IsPlayerConnected(playerid)) return 1;
	SetPVarInt(playerid,"Level",level);
	PlayerPlaySoundOnPlayer(playerid,1057);
	UpdatePlayerScore(playerid);
	new string[MAX_STRING];
	format(string, sizeof(string),  lang_texts[9][22] , GetPlayerLevel(playerid));
	SendClientMessage(playerid, COLOUR_XP_BAD, string);
	GameMSG("player: %d:  %s: decreased his level to %d",playerid,oGetPlayerName(playerid),GetPlayerLevel(playerid));
	return 1;
}

stock PlayerLevelUp(playerid,level)
{
	if(!IsPlayerConnected(playerid)) return 1;
	SetPVarInt(playerid,"Level",level);
	SetMaxPlayerHealth(playerid);
	PlayerPlaySoundOnPlayer(playerid,1057);
	UpdatePlayerScore(playerid);
	new string[MAX_STRING];
	format(string, sizeof(string),  lang_texts[9][23] , GetPlayerLevel(playerid));
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	
	ShowWeaponsOnLevel(playerid,GetPlayerLevel(playerid));
	
	GameMSG("player: %d:  %s: increased his level to %d",playerid,oGetPlayerName(playerid),GetPlayerLevel(playerid));
	return 1;
}
