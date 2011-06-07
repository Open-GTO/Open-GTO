//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
// Updated in 27.03.2010 by ZiGGi

#if defined _player_included
#endinput
#endif

#define _player_included
#pragma library player

#include "base"
#include "utils\gtoutils"
#include "account"

enum PlayerInfo {
	Status,		      // 0-user,1>-moderator,admins
	JailTime,	     // Time for Jails
	MuteTime,	     // Time for Mute
	VIPDate[15]
}

new Player[MAX_PLAYERS][PlayerInfo];

#define GetPlayerTotalMoney(%0)	GetPVarInt(%0,"Money")+GetPVarInt(%0,"BankMoney")
#define GetPlayerGangName(%0) Gang[GetPVarInt(%0,"GangID")][gang_name]
#define GetPlayerKills(%0) GetPVarInt(%0,"Kills")
#define GetPlayerDeaths(%0) GetPVarInt(%0,"Deaths")

#define GetPlayerBankMoney(%0) GetPVarInt(%0,"BankMoney")
#define GivePlayerBankMoney(%0,%1) SetPVarInt(%0,"BankMoney",GetPVarInt(%0,"BankMoney") + %1)

#define GetPlayerJailed(%0) GetPVarInt(%0,"Jailed")
#define SetPlayerJailed(%0,%1) SetPVarInt(%0,"Jailed",%1)

#define GetPlayerFightStyle(%0) GetPVarInt(%0,"FightStyle")
#define SetPlayerFightStyle(%0,%1) SetPVarInt(%0,"FightStyle",%1)

#define GetPlayerFightStyleUsed(%0) GetPVarInt(%0,"FightStyleUsed")
#define SetPlayerFightStyleUsed(%0,%1) SetPVarInt(%0,"FightStyleUsed",%1)

#define GetPlayerMuted(%0) GetPVarInt(%0,"Muted")
#define SetPlayerMuted(%0,%1) SetPVarInt(%0,"Muted",%1)

#define GetPlayerSkinModel(%0) GetPVarInt(%0,"SkinModel")
#define SetPlayerSkinModel(%0,%1) SetPVarInt(%0,"SkinModel",%1)

#define GetPlayerSpecID(%0) GetPVarInt(%0,"SpecID")
#define SetPlayerSpecID(%0,%1) SetPVarInt(%0,"SpecID",%1)

#define GetPlayerSpectating(%0) GetPVarInt(%0,"Spectating")
#define SetPlayerSpectating(%0,%1) SetPVarInt(%0,"Spectating",%1)

#define GetPlayerClickedPlayerid(%0) GetPVarInt(%0,"ClickedPlayerid")
#define SetPlayerClickedPlayerid(%0,%1) SetPVarInt(%0,"ClickedPlayerid",%1)

#define GetPlayerVIP(%0) GetPVarInt(%0,"VIP")
#define SetPlayerVIP(%0,%1) SetPVarInt(%0,"VIP",%1)

#include "weapons"

new PlayerSkin[MAX_PLAYERS];

#define PLAYER_WEAPON_SLOTS 13//  drop weapons
enum PWeap {
	pwid,
	pbullets
}
new PlayerWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap]; // 0 - weaponid, 1 - ammo
new PlayerTempWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap];

#define PLAYER_WEAPON_SKILLS 10
new PlayerWeaponsSkills[MAX_PLAYERS][PLAYER_WEAPON_SKILLS];

new PlayerCurrentKills[MAX_PLAYERS];
new PlayerGangName[MAX_PLAYERS][MAX_NAME];
new PlayerSpawn[MAX_PLAYERS][CoordInfo];
new SpawnTime[MAX_PLAYERS];

enum Quest_Stats {
	Quest_Name[MAX_NAME]
}

new PlayerQuest[MAX_PLAYERS]; // holds id of quest player is currently doing
new Quests[MAX_QUESTS][Quest_Stats]; // table of all registered quests
new QuestCount;

new Float:PlayerCP[MAX_PLAYERS][CoordInfo];
new bool:PlayerCPActive[MAX_PLAYERS];

new PlayerJustTeleported[MAX_PLAYERS];
new PlayerStartMoney = PLAYER_START_MONEY;
new PlayerJailed = 0;
new PlayerMuted = 0;
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
	ini_GetInt(file_player,"Player_Jailed",PlayerJailed);
	ini_GetInt(file_player,"Player_Muted",PlayerMuted);
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
	ini_SetInt(file_player,"Player_Jailed",PlayerJailed);
	ini_SetInt(file_player,"Player_Muted",PlayerMuted);
	ini_Close(file_player);
	return 1;
}

stock player_OnGameModeInit()
{
	PlayerLoadConfig();
	GameMSG("SERVER: Player module init");
	return 1;
}

stock SpawnTimer()
{
	for(new playerid=0;playerid<=GetPlayerLastID();playerid++)
	{
		if(IsPlayerConnected(playerid)) SpawnTime[playerid]++;
	}
}

// needs to be ran by a timer
// sync player hp/money and check for mismatch
stock SyncPlayers()
{
	for(new playerid=0;playerid<=GetPlayerLastID();playerid++)
	{
		if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) continue;
		if(GetPVarInt(playerid,"Spawned") == 0) continue; // если игрок мертв, то защиты не срабатывают
		if(PlayerJustTeleported[playerid] > 0) PlayerJustTeleported[playerid]--;
		SyncMoney(playerid);
		SyncHealth(playerid);
		CheckPlayerKick(playerid);
		JailPlayer(playerid);
		CheckPing(playerid);
	}
	return 1;
}

// Check if any players have been marked to kick, and kicks them
stock CheckPlayerKick(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		if (KickList[playerid] == 1) // if we wanna kick this player
		{
			GameTextForPlayer(playerid,"~r~Connection Lost.",999,5); //  send msg first
			TogglePlayerControllable(playerid,0);
			KickList[playerid]=2;
		}
		else if (KickList[playerid] >=2) // wait a second before doing the kicking (for msg to get through)
		{
			new logstring[MAX_STRING];
			format(logstring, sizeof (logstring), "player: %d:  %s: has been auto kicked",playerid,oGetPlayerName(playerid));
			WriteLog(GameLog,logstring);
			KickList[playerid] = 0;
			Kick(playerid);
			return;
		}
	}
}

stock KickPlayer(playerid,reason[])
{
	if(!IsPlayerConnected(playerid)) return 1;
	new string[MAX_STRING];
	if(IsPlayerAdmin(playerid))
	{
		format(string,sizeof(string), lang_texts[9][4], reason);
		SendClientMessage(playerid,COLOUR_YELLOW, string);
		return 1;
	}
	if(KickList[playerid] != 0) return 1;
	KickList[playerid]++;
	if(strlen(reason) > 0)
	{
		format(string,sizeof(string), lang_texts[9][5] ,reason);
		SendClientMessage(playerid,COLOUR_RED, string);
		format(string,sizeof(string), lang_texts[9][6] ,oGetPlayerName(playerid),reason);
		SendClientMessageToAll(COLOUR_MISC, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has been auto kicked. Reason: %s",playerid,oGetPlayerName(playerid),reason);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock GivePlayerOwnedWeapons(playerid)
{
	ResetPlayerWeapons(playerid);
	for(new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{
		if(PlayerWeapons[playerid][weaponslot][pwid] <= 0) continue;
		if(!IsPlayerAllowedWeapon(playerid,PlayerWeapons[playerid][weaponslot][pwid])) continue;
		if(PlayerWeapons[playerid][weaponslot][pwid] > 0)
		{
			GivePlayerWeapon(playerid,PlayerWeapons[playerid][weaponslot][pwid],PlayerWeapons[playerid][weaponslot][pbullets]);
		}
	}
}

stock SendPlayerOwnedWeapons(playerid)
{
	new string[MAX_STRING];
	SendClientMessage(playerid,COLOUR_GREEN,lang_texts[9][7]);
	for (new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{
		if (PlayerWeapons[playerid][weaponslot][pwid] <= 0) continue;
		if (PlayerWeapons[playerid][weaponslot][pbullets] > 1)
		{
			format(string,sizeof(string),lang_texts[9][8],oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]),PlayerWeapons[playerid][weaponslot][pbullets]);
			SendClientMessage(playerid,COLOUR_MISC,string);
		}
		else if (PlayerWeapons[playerid][weaponslot][pbullets] == 1)
		{
			format(string,sizeof(string), lang_texts[9][9] ,oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]));
			SendClientMessage(playerid,COLOUR_MISC,string);
		}
	}
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

stock player_OnPlayerSpawn(playerid)
{
	SetPVarInt(playerid,"Spawned",1);
	new GH = ganghouse(playerid);
	SpawnTime[playerid] = 0;
	SetPlayerInterior(playerid,0);
	if(GH == -1)
	{ 
		if((PlayerSpawn[playerid][Coord_X] !=0.0) && (PlayerSpawn[playerid][Coord_Y] !=0.0) && (PlayerSpawn[playerid][Coord_Z] !=0.0))
		{
			SetPlayerPos(playerid,PlayerSpawn[playerid][Coord_X],PlayerSpawn[playerid][Coord_Y],PlayerSpawn[playerid][Coord_Z]); // Warp the player
			PlayerSpawn[playerid][Coord_X] = 0.0;
			PlayerSpawn[playerid][Coord_Y] = 0.0;
			PlayerSpawn[playerid][Coord_Z] = 0.0;
		}
		else
		{
			if(GetPlayerLevel(playerid) > 12)
			{
				new rand = random(sizeof(LVDMPlayerSpawns));
				SetPlayerPos(playerid,LVDMPlayerSpawns[rand][Coord_X],LVDMPlayerSpawns[rand][Coord_Y],LVDMPlayerSpawns[rand][Coord_Z]); // Warp the player
				SetPlayerFacingAngle(playerid,271.8075);
			}
			else
			{
				new spawnid = GetRndPlayerSpawnByLevel(GetPlayerLevel(playerid));
				SetPlayerPos(playerid,LeveledPlayerSpawns[spawnid][playerspawn_x],LeveledPlayerSpawns[spawnid][playerspawn_y],LeveledPlayerSpawns[spawnid][playerspawn_z]); // Warp the player
			}
		}
	}
	else gang2house(playerid,GH);
	if(!IsPlayerRegistered(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED, lang_texts[9][10]);
		SendClientMessage(playerid,COLOUR_RED, lang_texts[9][11]);
		SendClientMessage(playerid,COLOUR_RED, lang_texts[9][12]);
	}
	GivePlayerOwnedWeapons(playerid);
	oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
	PlayerJustTeleported[playerid]+=5;

	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
		SetPlayerSkillLevel(playerid,i,PlayerWeaponsSkills[playerid][i]);

	SetPlayerFightStyleUsed(playerid,GetPlayerFightStyle(playerid));
	SetPlayerFightingStyle(playerid,GetPlayerFightStyleUsed(playerid));
	return 1;
}

stock player_ResetStats(playerid)
{
	SetPVarInt(playerid,"PlayerRegistered",0);
	oSetPlayerMoney(playerid,0);
	KickList[playerid] = 0;
	ResetQuest(playerid);
	SpawnTime[playerid] = 0;
	set(PlayerGangName[playerid],nullstr);
	PlayerSkin[playerid] = 0;
	ADMDropAmmo(playerid);
	PlayerJustTeleported[playerid] = 5;
	Player[playerid][JailTime] = 0;
	Player[playerid][MuteTime] = 0;
	Player[playerid][Status] = 0;
	SetPlayerSkinModel(playerid,1);
	SetPlayerFightStyle(playerid,4);
	SetPlayerFightStyleUsed(playerid,4);
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
	{
		PlayerWeaponsSkills[playerid][i] = 0;
		SetPlayerSkillLevel(playerid,i,0);
	}
}

stock IsPlayerRegistered(playerid)
{
	if(!IsPlayerConnected(playerid) || GetPVarInt(playerid,"PlayerRegistered") == 0) return false;
	return true;
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
	    case 0: format(string,sizeof(string),"%s (Вылетел)",string);
	    case 1: format(string,sizeof(string),"%s (Вышел)",string);
	    case 2: format(string,sizeof(string),"%s (Кикнут)",string);
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
	PlayerCurrentKills[playerid] = 0;
	oGivePlayerMoney(playerid,-100,0);

	new WeaponSkillID = GetWeaponSkillID(reason);
	SetPlayerSkillLevel(killerid,WeaponSkillID,PlayerWeaponsSkills[killerid][WeaponSkillID] += 1);

	modfunc_OnPlayerDeath(playerid,killerid,reason);
}

stock player_OnPlayerKill(killerid, victimid, reason) // earn xp, kills++
{
	if(GetPVarInt(killerid,"GangID") != 0)
	{
		if(GangKill(GetPVarInt(killerid,"GangID"),killerid,victimid,reason) == 1) return;
	}

	new slot=GetWeaponSlot(reason);
	if(IsWeapon(reason))
	{
		if (PlayerTempWeapons[killerid][slot][pwid] > 0)
		{
			if (PlayerWeapons[killerid][slot][pbullets] > 0)
			{
				if(!IsWeaponHandToHand(reason))
				{
					PlayerWeapons[killerid][slot][pbullets] -= Vary(2,1);	
					if (PlayerWeapons[killerid][slot][pbullets] < 0) PlayerWeapons[killerid][slot][pbullets] = 0;	
				}
			}
			else if(reason != 0)
			{
				if(!IsStoreWeapon(reason))
				{
					new string[MAX_STRING];
					format(string,sizeof(string), lang_texts[9][14] ,reason);
					KickPlayer(killerid,string); //kick player, they used illegal weapon
					return;
				}
			}
		}
	}

	GivePVarInt(killerid,"Kills",1);
	PlayerCurrentKills[killerid]++;

	// Give XP
	new XP_give_victim = -((GetPlayerXP(victimid) / 100) * PLAYER_XP_DEATH_MINUS_PROC);
	new XP_give_killer = (GetPlayerLevel(victimid) + 1) * 75;

	if((GetPlayerXP(killerid) + XP_give_killer) > MAX_LVLXP) SetPlayerXP(killerid,MAX_LVLXP);
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

stock RegisterQuest(name[])
{
	if (QuestCount >= MAX_QUESTS) return INVALID_QUEST_ID;
	QuestCount++;
	new questid =QuestCount;
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

stock oGivePlayerWeapon(playerid,weaponid,ammo)
{
	if(!IsPlayerAllowedWeapon(playerid,weaponid)) return;
	new slot = GetWeaponSlot(weaponid);
	if(ammo > 0)
	{
		if(!IsWeaponHandToHand(weaponid))
		{
			PlayerWeapons[playerid][slot][pwid] = weaponid;
			PlayerWeapons[playerid][slot][pbullets] += ammo;
		}
		else
		{
			PlayerWeapons[playerid][slot][pwid] = weaponid;
			PlayerWeapons[playerid][slot][pbullets] = 1;
		}
		GivePlayerWeapon(playerid,weaponid,ammo);
	}
}

stock PlayerSave(playerid)
{
	if(!IsPlayerRegistered(playerid)) return 0;
	player_save_db_ini(playerid);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "player: %d:  %s: player_saved successfully",playerid,oGetPlayerName(playerid));
	WriteLog(GameLog,logstring);
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
	new logstring[MAX_STRING];
	format(logstring, sizeof(logstring), "player: %d:  %s: player_created successfully",playerid,oGetPlayerName(playerid));
	WriteLog(GameLog,logstring);
	return 1;
}

player_create_db_ini(playerid)
{
	new filename_player[MAX_STRING];
	format(filename_player,sizeof(filename_player),"%s%s"GTO_FILES_FORMAT,PlayerDB,oGetPlayerName(playerid));

	if(ini_Exist(filename_player)) return SendClientMessage(playerid,COLOUR_RED, lang_texts[9][30]);
	player_save_db_ini(playerid);
	return 1;
}

stock SetWeaponsFromDBString(playerid,dbstring[])
{
	new idx;
	for(new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerWeapons[playerid][i][pwid] = strval(strcharsplit(dbstring,idx,'/'));
		PlayerWeapons[playerid][i][pbullets] = strval(strcharsplit(dbstring,idx,'|'));
	}
}

stock CreateWeaponDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,PlayerWeapons[playerid][i][pwid]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
		valstr(temp,PlayerWeapons[playerid][i][pbullets]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = '|';
	}
	return wepstr;
}
stock SetWeaponsSkillsFromDBString(playerid,dbstring[])
{
	new idx;
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
		PlayerWeaponsSkills[playerid][i] = strval(strcharsplit(dbstring,idx,'/'));
}
stock CreateWeaponSkillsDBString(playerid)
{
	new wepstr[MAX_STRING];
	for(new i=0;i<PLAYER_WEAPON_SKILLS;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,PlayerWeaponsSkills[playerid][i]);
		strins(wepstr,temp,strlen(wepstr));
		wepstr[strlen(wepstr)] = '/';
	}
	return wepstr;
}


stock player_save_db_ini(playerid)
{
	if(strlen(oGetPlayerName(playerid)) < 1) return 1;
	new filename_player[MAX_STRING];
	format(filename_player,sizeof(filename_player),"%s%s"GTO_FILES_FORMAT,PlayerDB,oGetPlayerName(playerid));
	new file_player;
	if(!ini_Exist(filename_player)) file_player = ini_Create(filename_player);
	else file_player = ini_Open(filename_player);
	ini_Set(file_player,"Name",oGetPlayerName(playerid));
	ini_Set(file_player,"Gang",PlayerGangName[playerid]);
	ini_SetInt(file_player,"Level",GetPlayerLevel(playerid));
	ini_SetInt(file_player,"XP",GetPlayerXP(playerid));
	ini_SetInt(file_player,"Money",oGetPlayerMoney(playerid));
	ini_SetInt(file_player,"BankMoney",GetPlayerBankMoney(playerid));
	ini_SetInt(file_player,"Deaths",GetPlayerDeaths(playerid));
	ini_SetInt(file_player,"Kills",GetPlayerKills(playerid));
	ini_SetInt(file_player,"Jailed",GetPlayerJailed(playerid));
	ini_SetInt(file_player,"Muted",GetPlayerMuted(playerid));
	ini_SetInt(file_player,"JailTime",GetPlayerFightStyle(playerid));
	ini_SetInt(file_player,"MuteTime",Player[playerid][MuteTime]);
	ini_SetInt(file_player,"FightStyle",GetPlayerFightStyle(playerid));
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	if(z > 900) {x=0;y=0;z=0;}
	new string[MAX_STRING];
	format(string,sizeof(string),"%f,%f,%f",x,y,z);
	ini_Set(file_player,"Coord",string);
	ini_SetInt(file_player,"Status",Player[playerid][Status]);
	ini_Set(file_player,"Weapons",CreateWeaponDBString(playerid));
	ini_Set(file_player,"WeaponsSkills",CreateWeaponSkillsDBString(playerid));
	ini_SetInt(file_player,"SkinModel",GetPlayerSkinModel(playerid));
	ini_Set(file_player,"VIP",Player[playerid][VIPDate]);
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
	new buf;
	
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
	
    ini_GetInt(file_player,"Muted",buf);
	SetPlayerMuted(playerid,buf);
	
    ini_GetInt(file_player,"JailTime",buf);
	SetPlayerFightStyle(playerid,buf);
	
    ini_GetInt(file_player,"MuteTime",Player[playerid][MuteTime]);
    ini_GetInt(file_player,"FightStyle",buf);
	SetPlayerFightStyle(playerid,buf);
	new string[MAX_STRING],idx=0;
	ini_Get(file_player,"Coords",string);
	PlayerSpawn[playerid][Coord_X] = floatstr(strcharsplit(string,idx,','));
	PlayerSpawn[playerid][Coord_Y] = floatstr(strcharsplit(string,idx,','));
	PlayerSpawn[playerid][Coord_Z] = floatstr(strcharsplit(string,idx,','));
    ini_GetInt(file_player,"Status",Player[playerid][Status]);
	SetPlayerFightingStyle(playerid,GetPlayerFightStyle(playerid));
	new gangname[MAX_NAME];
	new tmpname[MAX_NAME];
	ini_Get(file_player,"Gang",gangname);
	if(strlen(gangname) > 0)
	{
		format(tmpname,sizeof(tmpname),"%s%s"GTO_FILES_FORMAT,GangDB,gangname);
		if(!ini_Exist(tmpname))
		{
			format(tmpname,sizeof(tmpname),"Банда `%s` была расформирована.",gangname);
			SendClientMessage(playerid,COLOUR_ORANGE,tmpname);
			ini_Set(file_player,"Gang",nullstr);
			SetPVarInt(playerid,"GangID",0);
			goto not_gang;
		}
		set(PlayerGangName[playerid],gangname);
		SetPVarInt(playerid,"GangID",GangLoad(gangname));
		GangMemberLogin(playerid,GetPVarInt(playerid,"GangID"));
		if(GetPVarInt(playerid,"GangID") == 0)
		{
			set(PlayerGangName[playerid],nullstr);
			PlayerGangName[playerid][0] = 0;
			SetPVarInt(playerid,"GangID",0);
		}
	}
	else SetPVarInt(playerid,"GangID",0); // no gang
not_gang:
	new wepstr[MAX_STRING];
	ini_Get(file_player,"Weapons",wepstr);
	SetWeaponsFromDBString(playerid,wepstr);
	ini_Get(file_player,"WeaponsSkills",wepstr);
	SetWeaponsSkillsFromDBString(playerid,wepstr);
	
	ini_GetInt(file_player,"SkinModel",buf);
	SetPlayerSkinModel(playerid,buf);
	
	ini_Get(file_player,"VIP",Player[playerid][VIPDate],15);
	if(VIPCheck(Player[playerid][VIPDate]))
		SetPlayerVIP(playerid,1);
	else
		SetPlayerVIP(playerid,0);
	UpdatePlayerScore(playerid);
	ini_Close(file_player);
	return 1;
}

stock VIPCheck(date[])
{
	new h[5],day,month,year,day1,month1,year1;
	strmid(h,date,0,2); 
	day=strval(h);
	strmid(h,date,3,5); 
	month=strval(h);
	strmid(h,date,6,10); 
	year=strval(h);
	getdate(year1,month1,day1);
	if(year > year1) return true;
	else if(year == year1)
	{
		if(month > month1) return true;
		else if(month == month1)
		{
			if(day >= day1) return true;
		}
	}
	return false;
}

stock ADMDropAmmo(playerid)
{
	if(IsPlayerConnected(playerid)==0) return 0;
	for(new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerWeapons[playerid][i][pwid] = 0;
		PlayerWeapons[playerid][i][pbullets] = 0;
	}
	PlayerWeapons[playerid][0][pwid] = 0; // fists
	PlayerWeapons[playerid][0][pbullets] = 1;
	return 1;
}

stock IsPlayerAdm(playerid)
{
	if(Player[playerid][Status] >= 11) return 1;
	return 0;
}

stock IsPlayerMod(playerid)
{
	if(Player[playerid][Status] > 0) return 1;
	return 0;
}

stock GetWeaponModel(weaponid)
{
	new model;
	switch(weaponid)
	{
		case 1: model=331; case 2: model=333; case 3: model=334;
		case 4: model=335;  case 5: model=336; case 6: model=337;
		case 7: model=338;  case 8: model=339; case 9: model=341;
		case 10: model=321; case 11: model=322; case 12: model=323;
		case 13: model=324; case 14: model=325; case 15: model=326;
		case 16: model=342; case 17: model=343; case 18: model=344;
		case 22: model=346; case 23: model=347; case 24: model=348;
		case 25: model=349; case 26: model=350; case 27: model=351;
		case 28: model=352; case 29: model=353; case 30: model=355;
		case 31: model=356; case 32: model=372; case 33: model=357;
		case 34: model=358; case 35: model=359; case 36: model=360;
		case 37: model=361; case 38: model=362; case 39: model=363;
		case 41: model=365; case 42: model=366; case 46: model=371;
	}
	if(model<300 || model>371) return -1;
	return model;
}

stock GetWeaponBulletsDown(weaponid)
{
	new dbw;
	switch(weaponid)
	{
		case 1: dbw=1; case 2: dbw=1;  case 3: dbw=1;
		case 4: dbw=1;  case 5: dbw=1;  case 6: dbw=1;
		case 7: dbw=1;  case 8: dbw=1;  case 9: dbw=1;
		case 10: dbw=1;  case 11: dbw=1;  case 12: dbw=1;
		case 13: dbw=1;  case 14: dbw=1;  case 15: dbw=1;
		case 16: dbw=8;  case 17: dbw=1;  case 18: dbw=1;
		case 22: dbw=30;  case 23: dbw=10;  case 24: dbw=10;
		case 25: dbw=15;  case 26: dbw=10;  case 27: dbw=10;
		case 28: dbw=60;  case 29: dbw=60;  case 30: dbw=80;
		case 31: dbw=80;  case 32: dbw=60;  case 33: dbw=20;
		case 34: dbw=10;  case 35: dbw=4;  case 36: dbw=3;
		case 37: dbw=10;  case 38: dbw=500;  case 39: dbw=5;
		case 41: dbw=500;  case 42: dbw=500;  case 46: dbw=1;
	}
	if(dbw<1 || dbw>500) return 1;
	return dbw;
}
#define DeletePickup(%1) DestroyStreamPickup(%1)

stock GetWeaponSkillID(weaponid)
{
	switch(weaponid)
	{
		case 22: return WEAPONSKILL_PISTOL;
		case 23: return WEAPONSKILL_PISTOL_SILENCED;
		case 24: return WEAPONSKILL_DESERT_EAGLE;
		case 25: return WEAPONSKILL_SHOTGUN;
		case 26: return WEAPONSKILL_SAWNOFF_SHOTGUN;
		case 27: return WEAPONSKILL_SPAS12_SHOTGUN;
		case 28: return WEAPONSKILL_MICRO_UZI;
		case 29: return WEAPONSKILL_MP5;
		case 30: return WEAPONSKILL_AK47;
		case 31: return WEAPONSKILL_M4;
		case 34: return WEAPONSKILL_SNIPERRIFLE;
	}
	return -1;
}

stock GetWeaponSlot(weaponid)
{
	switch(weaponid)
	{
	    case 0,1: return 0;
	    case 2..9: return 1;
	    case 22..24: return 2;
	    case 25..27: return 3;
	    case 28,29,32: return 4;
	    case 30,31: return 5;
	    case 33,34: return 6;
	    case 35..38: return 7;
	    case 16..18,39: return 8;
	    case 41..43: return 9;
		case 10..15: return 10;
		case 45,46: return 11;
		case 40: return 12;
		default: return -1;
	}
	return -1;
}
