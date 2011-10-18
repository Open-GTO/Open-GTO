//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
// Updated in 02.09.2011 by ZiGGi

#if defined _player_included
	#endinput
#endif

#define _player_included
#pragma library player


#include "base"
#include "utils\gtoutils"
#include "account"
#include "weapons"

#define GetPlayerKills(%0) GetPVarInt(%0,"Kills")
#define GetPlayerDeaths(%0) GetPVarInt(%0,"Deaths")

#define GetPlayerJailed(%0) GetPVarInt(%0,"Jailed")
#define SetPlayerJailed(%0,%1) SetPVarInt(%0,"Jailed",%1)

#define GetPlayerJailTime(%0) GetPVarInt(%0,"JailTime")
#define SetPlayerJailTime(%0,%1) SetPVarInt(%0,"JailTime",%1)

#define GetPlayerMuted(%0) GetPVarInt(%0,"Muted")
#define SetPlayerMuted(%0,%1) SetPVarInt(%0,"Muted",%1)

#define GetPlayerMuteTime(%0) GetPVarInt(%0,"MuteTime")
#define SetPlayerMuteTime(%0,%1) SetPVarInt(%0,"MuteTime",%1)

#define GetPlayerSkinModel(%0) GetPVarInt(%0,"SkinModel")
#define SetPlayerSkinModel(%0,%1) SetPVarInt(%0,"SkinModel",%1)

#define GetPlayerSpecID(%0) GetPVarInt(%0,"SpecID")
#define SetPlayerSpecID(%0,%1) SetPVarInt(%0,"SpecID",%1)

#define GetPlayerSpectating(%0) GetPVarInt(%0,"Spectating")
#define SetPlayerSpectating(%0,%1) SetPVarInt(%0,"Spectating",%1)

#define GetPlayerStatus(%0) GetPVarInt(%0,"Status")
#define SetPlayerStatus(%0,%1) SetPVarInt(%0,"Status",%1)

#define GetPlayerHideStatus(%0) GetPVarInt(%0,"Hide")
#define SetPlayerHideStatus(%0,%1) SetPVarInt(%0,"Hide",%1)

#define GetPlayerSpawnHouseID(%0) GetPVarInt(%0,"SpawnHouseID")
#define SetPlayerSpawnHouseID(%0,%1) SetPVarInt(%0,"SpawnHouseID",%1)

stock GetPlayerGangName(playerid)
{
	new string[MAX_NAME];
	GetPVarString(playerid,"GangName",string,sizeof(string));
	return string;
}
#define SetPlayerGangName(%0,%1) SetPVarString(%0,"GangName",%1)

enum Quest_Stats {
	Quest_Name[MAX_NAME]
}

new PlayerQuest[MAX_PLAYERS]; // holds id of quest player is currently doing
new Quests[MAX_QUESTS][Quest_Stats]; // table of all registered quests
new QuestCount;

new PlayerStartMoney = PLAYER_START_MONEY;
new PlayerStartWeapon1 = PLAYER_START_WEAPON_1;
new PlayerStartWeapon2 = PLAYER_START_WEAPON_2;
new PlayerStartWeapon3 = PLAYER_START_WEAPON_3;

enum PlayerSpawnInfo {
	playerspawn_level,
	Float:playerspawn_x,
	Float:playerspawn_y,
	Float:playerspawn_z,
	Float:playerspawn_heading
}

#define PLAYERSPAWNS_SIZE 38
new Float:LeveledPlayerSpawns[PLAYERSPAWNS_SIZE][PlayerSpawnInfo] = {
	{0,2517.5344,-1694.0607,18.4772,47.2527}, // lvl0spawn1
	{0,2494.6113,-1694.2920,23.5697,7.8082}, // lvl0ps
	{0,2524.2092,-1675.8125,19.9302,65.8183}, // lvl0ps
	{3,2066.9448,-1700.1219,14.1484,276.0525}, // lvl3spawn
	{3,2065.0754,-1732.0588,18.7969,308.6771}, // lvl3spawn
	{3,2042.3877,-1722.0016,13.5469,339.7588}, // lvl3spawn
	{3,2040.6575,-1646.1099,13.5469,1.2393}, // lvl3spawn
	{5,1641.0234,-1545.1201,13.5803,293.5606}, // lvl5spawn
	{5,1585.7887,-1539.0266,13.5864,265.2574}, // lv5spaawn
	{5,1606.7871,-1476.6110,13.5804,0.8114}, // lv5 spawn
	{6,1885.8452,-1087.4312,23.9185,279.8816}, // lv6spawn
	{6,2008.4741,-1098.7870,24.9058,256.2135}, // lv6spawn
	{6,2045.3113,-1158.0294,23.4437,145.6347}, // lv6spawn
	{6,2022.5231,-1210.7472,21.7610,35.7139}, // lv6spawn
	{6,1933.6676,-1226.6060,20.1364,97.0786}, // lv6spawn
	{7,1332.1099,-911.0746,39.5781,168.9972}, // lv7spawn
	{7,1309.1066,-847.0045,64.8932,315.8327}, // lv7spawn
	{7,1285.2598,-830.9579,83.1406,171.4035}, // lv7spawn
	{7,1243.5209,-743.7954,94.9519,191.1576}, // lv7spawn
	{8,655.1062,-543.5223,16.3281,349.4992}, // lvl8player
	{8,702.5857,-463.9641,16.3359,189.5556}, // lvl8play
	{8,852.9753,-587.0893,18.0406,8.3576}, // play8
	{8,744.7605,-582.7914,16.9987,84.0696}, // play8
	{9,248.1220,-278.0332,1.5781,52.9390}, // play9
	{9,326.3771,-53.1227,1.5285,111.6351}, // play9
	{9,214.2944,-89.7956,1.5710,312.5051}, // play9
	{10,614.6899,43.1528,0.0748,344.7167}, // play10
	{10,867.4291,-30.7355,63.1953,176.7125}, // play10
	{10,1011.3226,11.4864,93.0156,302.5121}, // play10
	{11,751.2596,383.9003,23.1719,335.8973}, // play10
	{11,1222.3271,300.4901,19.5547,155.6648}, // play11
	{11,1284.9962,175.7558,20.3423,73.2174}, // play11
	{12,1426.9465,371.7816,18.8869,257.0709}, // play11
	{12,1572.3761,36.5942,24.5907,242.6472}, // play11
	{14,2215.1565,123.8511,26.4844,350.1021}, // play12
	{15,2281.3704,-49.9920,27.0176,207.7730}, // play12
	{17,2162.9019,-102.3568,2.7500,27.7265}, // play12
	{18,2459.7744,-40.9547,26.4844,25.8460} // play12
};


// LVDMSpawns
new Float:LVDMPlayerSpawns[23][CoordInfo] = {
	{1958.3783,1343.1572,15.3746},
	{2199.6531,1393.3678,10.8203},
	{2483.5977,1222.0825,10.8203},
	{2637.2712,1129.2743,11.1797},
	{2000.0106,1521.1111,17.0625},
	{2024.8190,1917.9425,12.3386},
	{2261.9048,2035.9547,10.8203},
	{2262.0986,2398.6572,10.8203},
	{2244.2566,2523.7280,10.8203},
	{2335.3228,2786.4478,10.8203},
	{2150.0186,2734.2297,11.1763},
	{2158.0811,2797.5488,10.8203},
	{1969.8301,2722.8564,10.8203},
	{1652.0555,2709.4072,10.8265},
	{1564.0052,2756.9463,10.8203},
	{1271.5452,2554.0227,10.8203},
	{1441.5894,2567.9099,10.8203},
	{1480.6473,2213.5718,11.0234},
	{1400.5906,2225.6960,11.0234},
	{1598.8419,2221.5676,11.0625},
	{1318.7759,1251.3580,10.8203},
	{1558.0731,1007.8292,10.8125},
	{1705.2347,1025.6808,10.8203}
};

stock PlayerLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_player = ini_Open(ConfigDB);
	ini_Get(file_player,"Player_DB",PlayerDB);
	ini_GetInt(file_player,"Player_Start_Money",PlayerStartMoney);
	ini_GetInt(file_player,"Player_Start_Weapon1",PlayerStartWeapon1);
	ini_GetInt(file_player,"Player_Start_Weapon2",PlayerStartWeapon2);
	ini_GetInt(file_player,"Player_Start_Weapon3",PlayerStartWeapon3);
	ini_Close(file_player);
	return 1;
}

stock PlayerSaveConfig()
{
	new file_player = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_player,"Player_DB",PlayerDB);
	ini_SetInt(file_player,"Player_Start_Money",PlayerStartMoney);
	ini_SetInt(file_player,"Player_Start_Weapon1",PlayerStartWeapon1);
	ini_SetInt(file_player,"Player_Start_Weapon2",PlayerStartWeapon2);
	ini_SetInt(file_player,"Player_Start_Weapon3",PlayerStartWeapon3);
	ini_Close(file_player);
	return 1;
}

stock player_OnGameModeInit()
{
	PlayerLoadConfig();
	GameMSG("SERVER: Player module init");
	return 1;
}

stock player_OnPlayerSpawn(playerid)
{
	SetPVarInt(playerid,"Spawned",1);
	new house_id = GetPlayerSpawnHouseID(playerid);
	SetPlayerInterior(playerid,0);
	if(house_id == -1)
	{
		if(GetPVarFloat(playerid,"Coord_X") != 0.0 && GetPVarFloat(playerid,"Coord_Y") != 0.0 && GetPVarFloat(playerid,"Coord_Z") != 0.0)
		{
			SetPlayerPos(playerid,GetPVarFloat(playerid,"Coord_X"),GetPVarFloat(playerid,"Coord_Y"),GetPVarFloat(playerid,"Coord_Z")); // Warp the player
			SetPVarFloat(playerid,"Coord_X",0.0);
			SetPVarFloat(playerid,"Coord_Y",0.0);
			SetPVarFloat(playerid,"Coord_Z",0.0);
		}
		else
		{
			if(GetPlayerLevel(playerid) > 12)
			{
				new rand = random( sizeof(LVDMPlayerSpawns) );
				SetPlayerPos(playerid,LVDMPlayerSpawns[rand][Coord_X],LVDMPlayerSpawns[rand][Coord_Y],LVDMPlayerSpawns[rand][Coord_Z]); // Warp the player
			}
			else
			{
				new spawnid = GetRndPlayerSpawnByLevel(GetPlayerLevel(playerid));
				SetPlayerPos(playerid,LeveledPlayerSpawns[spawnid][playerspawn_x],LeveledPlayerSpawns[spawnid][playerspawn_y],LeveledPlayerSpawns[spawnid][playerspawn_z]); // Warp the player
			}
		}
	}
	else
	{
		SetPlayerPosToHouse(playerid,house_id);
		new tmp[MAX_STRING];
		format(tmp,sizeof(tmp),
			lang_texts[8][51],
			Houses[house_id][Houses_Name],Houses[house_id][Houses_UpKeepLeft],Houses[house_id][Houses_UpKeep]*2800
		);
		SendClientMessage(playerid,COLOUR_RED,tmp);
	}
	SetPlayerFacingAngle(playerid,GetPVarFloat(playerid,"Coord_A"));
	
	if(!IsPlayerRegistered(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED, lang_texts[9][10]);
		SendClientMessage(playerid,COLOUR_RED, lang_texts[9][11]);
		SendClientMessage(playerid,COLOUR_RED, lang_texts[9][12]);
	}
	GivePlayerOwnedWeapons(playerid);
	oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
	weapon_SetSkills(playerid);
	SetPlayerFightStyleUsed(playerid,GetPlayerFightStyle(playerid));
	SetPlayerFightingStyle(playerid,GetPlayerFightStyleUsed(playerid));
	PushHide(playerid);
	return 1;
}

stock player_OnPlayerDisconnect(playerid,reason)
{
	if(IsPlayerRegistered(playerid))
	{
		PlayerSave(playerid);
		AccountSave(playerid);
	}
	DMPlayerDisconnect(playerid);
	new string[MAX_STRING];
	format(string,sizeof(string),lang_texts[1][18],oGetPlayerName(playerid),playerid);
	switch(reason)
	{
	    case 0: strcat(string," (Вылетел)",sizeof(string));
	    case 1: strcat(string," (Вышел)",sizeof(string));
	    case 2: strcat(string," (Кикнут)",sizeof(string));
	}
	SendClientMessageToAll(COLOUR_GREY,string);
	GangMemberLogout(playerid,GetPVarInt(playerid,"GangID"));
	player_ResetStats(playerid);
	DisablePlayerRaceCheckpoint(playerid);
	SendDeathMessage(INVALID_PLAYER_ID,playerid,201);
}

stock player_OnPlayerConnect(playerid)
{
    new plrIP[16];
    GetPlayerIp(playerid,plrIP,sizeof(plrIP));
	SetPVarString(playerid,"IP",plrIP);
	player_ResetStats(playerid);
	if(!NameCharCheck( oGetPlayerName(playerid) ))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[9][13]);
		KickPlayer(playerid,"Такой ник запрещён.");
	}
	SetPlayerColor(playerid,COLOUR_PLAYER);
	
	new string[MAX_STRING];
	format(string, sizeof(string),lang_texts[15][85], VERSION);
	GameTextForPlayer(playerid,string,5500,1);
	SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][86]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][87]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][88]);
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][90]);
	SendClientMessage(playerid, COLOUR_GREEN, "_________________________________________________________________________________");

#if defined _testserver_included
	testserver_OnPlayerConnect(playerid);
#endif
}

stock player_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason
	SetPVarInt(playerid,"Spawned",0);
	GivePVarInt(playerid,"Deaths",1);
	oGivePlayerMoney(playerid,-100,0);
	modfunc_OnPlayerDeath(playerid,killerid,reason);
}

stock player_OnPlayerKill(killerid, victimid, reason) // earn xp, kills++
{
	if(GetPVarInt(killerid,"GangID") != 0)
	{
		if(GangKill(GetPVarInt(killerid,"GangID"),killerid,victimid,reason) == 1) return;
	}

	GivePVarInt(killerid,"Kills",1);

	// Give XP
	new XP_give_victim = -((GetPlayerXP(victimid) / 100) * PLAYER_XP_DEATH_MINUS_PROC);
	new XP_give_killer = (GetPlayerLevel(victimid) + 1) * 75;

	if((GetPlayerXP(killerid) + XP_give_killer) > MAX_XP) SetPlayerXP(killerid,MAX_XP);
	else if(XP_give_killer != 0)
	{
		GivePlayerXP(killerid,XP_give_killer,1);
		CheckPlayerLevel(killerid);
	}
	if(XP_give_victim != 0)
	{
		GivePlayerXP(victimid,XP_give_victim,1);
		CheckPlayerLevel(victimid);
	}
	//
	// Give Money
	new stolencash = (oGetPlayerMoney(victimid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if(stolencash != 0)
	{
		oGivePlayerMoney(victimid,-stolencash,1);
		oGivePlayerMoney(killerid,stolencash,1);
	}
	//

	return;
}

// needs to be ran by a timer
// sync player hp/money and check for mismatch
stock SyncPlayers()
{
	foreach(Player,playerid)
	{
		CheckPlayerKick(playerid);
		if(GetPVarInt(playerid,"Spawned") == 0) continue; // если игрок мертв, то защиты не срабатывают
		SyncMoney(playerid);
		SyncHealth(playerid);
		JailPlayer(playerid);
		CheckPing(playerid);
	}
	return 1;
}

// Check if any players have been marked to kick, and kicks them
stock CheckPlayerKick(playerid)
{
	if (KickList[playerid] == 1) // if we wanna kick this player
	{
		GameTextForPlayer(playerid,"~r~Connection Lost.",999,5); //  send msg first
		TogglePlayerControllable(playerid,0);
		KickList[playerid]=2;
	}
	else if (KickList[playerid] >=2) // wait a second before doing the kicking (for msg to get through)
	{
		GameMSG("player: %s(%d): has been kicked",oGetPlayerName(playerid),playerid);
		KickList[playerid] = 0;
		Kick(playerid);
		return;
	}
}

stock KickPlayer(playerid,reason[])
{
	if(!IsPlayerConnected(playerid)) return 1;
	new string[MAX_STRING];
	if(IsPlayerRconAdmin(playerid))
	{
		format(string,sizeof(string), lang_texts[9][4], reason);
		SendClientMessage(playerid,COLOUR_YELLOW, string);
		return 1;
	}
	if(KickList[playerid] != 0) return 1;
	KickList[playerid]++;
	new playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	if(strlen(reason) == 0)
	{
		set(reason,"None");
	}
	format(string,sizeof(string),lang_texts[9][5],reason);
	SendClientMessage(playerid,COLOUR_RED, string);
	format(string,sizeof(string),lang_texts[9][6],playername,reason);
	SendClientMessageToAll(COLOUR_MISC, string);
	GameMSG("player: %s(%d): has been kicked. Reason: %s",playername,playerid,reason);
	return 1;
}

stock GetRndPlayerSpawnByLevel(level)
{
	new spawnlevel=0,leveledspawns[PLAYERSPAWNS_SIZE+1],idx=0,spawnid;
	for(new lookupid=0;lookupid<PLAYERSPAWNS_SIZE;lookupid++)
	{
		if((LeveledPlayerSpawns[lookupid][playerspawn_level] <= level) && (LeveledPlayerSpawns[lookupid][playerspawn_level] > spawnlevel))
		{
			spawnlevel = LeveledPlayerSpawns[lookupid][playerspawn_level];
		}
	}
	for(new lookupid=0;lookupid<PLAYERSPAWNS_SIZE;lookupid++)
	{
		if(LeveledPlayerSpawns[lookupid][playerspawn_level] == spawnlevel)
		{
			leveledspawns[idx] = lookupid;
			idx++;
		}
	}
	spawnid = (idx != 0) ? leveledspawns[random(idx)] : leveledspawns[0];
	return spawnid;
}

stock player_ResetStats(playerid)
{
	DeletePVar(playerid,"PlayerRegistered");
	oSetPlayerMoney(playerid,0);
	KickList[playerid] = 0;
	ResetQuest(playerid);
	SetPlayerSkinModel(playerid,1);
	SetPlayerFightStyle(playerid,4);
	SetPlayerFightStyleUsed(playerid,4);
	weapon_ResetSkills(playerid);
	SetPlayerSpawnHouseID(playerid,-1);
}

stock IsPlayerRegistered(playerid)
{
	if(GetPVarInt(playerid,"PlayerRegistered") == 0) return false;
	return true;
}

stock RegisterQuest(name[])
{
	if (QuestCount >= MAX_QUESTS) return INVALID_QUEST_ID;
	QuestCount++;
	new questid = QuestCount;
	set(Quests[questid][Quest_Name],name);
	return questid;
}

stock ResetQuest(playerid)
{
	oDisablePlayerCheckpoint(playerid);
	PlayerQuest[playerid] = 0;
}

stock oGetPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock PlayerSave(playerid)
{
	if(!IsPlayerRegistered(playerid)) return 0;
	player_save_db_ini(playerid);
	GameMSG("player: %s(%d): player_saved successfully",oGetPlayerName(playerid),playerid);
	return 1;
}

stock CreatePlayer(playerid)
{
	oSetPlayerMoney(playerid,PlayerStartMoney);
	SetPlayerXP(playerid,0);
	ResetPlayerWeapons(playerid);
	new bullets = 100;
	if(PlayerStartWeapon1 != 0) oGivePlayerWeapon(playerid,PlayerStartWeapon1,bullets);
	if(PlayerStartWeapon2 != 0) oGivePlayerWeapon(playerid,PlayerStartWeapon2,bullets);
	if(PlayerStartWeapon3 != 0) oGivePlayerWeapon(playerid,PlayerStartWeapon3,bullets);
	player_create_db_ini(playerid);
	GameMSG("player: %s(%d): player_created successfully",oGetPlayerName(playerid),playerid);
	return 1;
}

stock player_create_db_ini(playerid)
{
	new filename_player[MAX_STRING];
	format(filename_player,sizeof(filename_player),"%s%s"GTO_FILES_FORMAT,PlayerDB,oGetPlayerName(playerid));

	if(ini_Exist(filename_player)) return SendClientMessage(playerid,COLOUR_RED, lang_texts[9][30]);
	player_save_db_ini(playerid);
	return 1;
}

stock player_save_db_ini(playerid)
{
	new filename_player[MAX_STRING],playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(filename_player,sizeof(filename_player),"%s%s"GTO_FILES_FORMAT,PlayerDB,playername);
	new file_player = (!ini_Exist(filename_player)) ? ini_Create(filename_player) : ini_Open(filename_player);
	ini_Set(file_player,"Name",playername);
	ini_Set(file_player,"Gang",GetPlayerGangName(playerid));
	ini_SetInt(file_player,"Level",GetPlayerLevel(playerid));
	ini_SetInt(file_player,"XP",GetPlayerXP(playerid));
	ini_SetInt(file_player,"Money",oGetPlayerMoney(playerid));
	ini_SetInt(file_player,"BankMoney",GetPlayerBankMoney(playerid));
	ini_SetInt(file_player,"Deaths",GetPlayerDeaths(playerid));
	ini_SetInt(file_player,"Kills",GetPlayerKills(playerid));
	ini_SetInt(file_player,"Jailed",GetPlayerJailed(playerid));
	ini_SetInt(file_player,"JailTime",GetPlayerJailTime(playerid));
	ini_SetInt(file_player,"Muted",GetPlayerMuted(playerid));
	ini_SetInt(file_player,"MuteTime",GetPlayerMuteTime(playerid));
	ini_SetInt(file_player,"FightStyle",GetPlayerFightStyle(playerid));
	new Float:pos[4];
	GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
	GetPlayerFacingAngle(playerid,pos[3]);
	if(pos[2] > 900) {pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 0;}
	new string[MAX_STRING];
	format(string,sizeof(string),"%f,%f,%f,%f",pos[0],pos[1],pos[2],pos[3]);
	ini_Set(file_player,"Coords",string);
	ini_SetInt(file_player,"Status",GetPlayerStatus(playerid));
	ini_Set(file_player,"Weapons",CreateWeaponDBString(playerid));
	ini_Set(file_player,"WeaponsSkills",CreateWeaponSkillsDBString(playerid));
	ini_SetInt(file_player,"SkinModel",GetPlayerSkinModel(playerid));
	ini_Set(file_player,"VIP",GetPlayerVIPDate(playerid));
	ini_SetInt(file_player,"Hide",GetPlayerHideStatus(playerid));
	ini_SetInt(file_player,"SpawnHouseID",GetPlayerSpawnHouseID(playerid));
	ini_Close(file_player);
	return 1;
}

stock PlayerLoadStats(playerid)
{
	if(!IsPlayerRegistered(playerid)) return 0; // login to acct first
	new string[MAX_STRING];
	format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,PlayerDB,oGetPlayerName(playerid));
	if(!ini_Exist(string)) CreatePlayer(playerid);
	player_load_db_ini(playerid);
	SetPVarInt(playerid,"IsLogin",1);
	return 1;
}

stock player_load_db_ini(playerid)
{
	new filename_player[MAX_STRING];
	format(filename_player,sizeof(filename_player),"%s%s"GTO_FILES_FORMAT,PlayerDB,oGetPlayerName(playerid));
	if(!ini_Exist(filename_player)) return 1;
	new file_player = ini_Open(filename_player);
	new buf,s_buf[MAX_STRING];
	
    ini_GetInt(file_player,"Level",buf);
	SetPlayerLevel(playerid,buf);
	
    ini_GetInt(file_player,"XP",buf);
	SetPlayerXP(playerid,buf);
	
    ini_GetInt(file_player,"Money",buf);
    oSetPlayerMoney(playerid,buf);
	
    ini_GetInt(file_player,"BankMoney",buf);
	SetPVarInt(playerid,"BankMoney",buf);
	
    ini_GetInt(file_player,"Deaths",buf);
    SetPVarInt(playerid,"Deaths",buf);
	
	ini_GetInt(file_player,"Kills",buf);
	SetPVarInt(playerid,"Kills",buf);
	
    ini_GetInt(file_player,"Jailed",buf);
	SetPlayerJailed(playerid,buf);
	
    ini_GetInt(file_player,"JailTime",buf);
	SetPlayerJailTime(playerid,buf);
	
    ini_GetInt(file_player,"Muted",buf);
	SetPlayerMuted(playerid,buf);
	
    ini_GetInt(file_player,"MuteTime",buf);
	SetPlayerMuteTime(playerid,buf);
	
    ini_GetInt(file_player,"FightStyle",buf);
	SetPlayerFightStyle(playerid,buf);
	
	buf=0;
	ini_Get(file_player,"Coords",s_buf);
	SetPVarFloat(playerid,"Coord_X",floatstr(strcharsplit(s_buf,buf,',')));
	SetPVarFloat(playerid,"Coord_Y",floatstr(strcharsplit(s_buf,buf,',')));
	SetPVarFloat(playerid,"Coord_Z",floatstr(strcharsplit(s_buf,buf,',')));
	SetPVarFloat(playerid,"Coord_A",floatstr(strcharsplit(s_buf,buf,',')));
	
	ini_GetInt(file_player,"Status",buf);
	SetPlayerStatus(playerid,buf);
	if(IsPlayerAdmin(playerid)) SetPlayerStatus(playerid,3);
	
	SetPlayerFightingStyle(playerid,GetPlayerFightStyle(playerid));

	ini_Get(file_player,"Gang",s_buf);
	if(strlen(s_buf) > 0)
	{
		new tmpname[MAX_NAME];
		format(tmpname,sizeof(tmpname),"%s%s"GTO_FILES_FORMAT,GangDB,s_buf);
		if(!ini_Exist(tmpname))
		{
			format(tmpname,sizeof(tmpname),"Банда `%s` была расформирована.",s_buf);
			SendClientMessage(playerid,COLOUR_ORANGE,tmpname);
			ini_Set(file_player,"Gang",nullstr);
			SetPVarInt(playerid,"GangID",0);
		}
		else
		{
			GangMemberLogin(playerid,GangLoad(s_buf));
		}
	}
	else SetPVarInt(playerid,"GangID",0); // no gang

	ini_Get(file_player,"Weapons",s_buf);
	SetWeaponsFromDBString(playerid,s_buf);
	ini_Get(file_player,"WeaponsSkills",s_buf);
	SetWeaponsSkillsFromDBString(playerid,s_buf);
	
	ini_GetInt(file_player,"SkinModel",buf);
	SetPlayerSkinModel(playerid,buf);
	
	ini_Get(file_player,"VIP",s_buf);
	SetPlayerVIPDate(playerid,s_buf);
	
	if(VIPCheck( GetPlayerVIPDate(playerid) ))
		SetPlayerVIP(playerid,1);
	else
		SetPlayerVIP(playerid,0);
	
	ini_GetInt(file_player,"Hide",buf);
	SetPlayerHideStatus(playerid,buf);
	
	ini_GetInt(file_player,"SpawnHouseID",buf);
	SetPlayerSpawnHouseID(playerid,buf);
	
	ini_Close(file_player);
	UpdatePlayerScore(playerid);
	return 1;
}
