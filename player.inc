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
#include "weapons"

forward SpawnTimer();
forward PlayerHealthRegen();
forward SyncPlayers();
forward CheckPlayerKick(playerid);
forward KickPlayer(playerid,reason[]);
forward HealthRegen(id);
forward GivePlayerOwnedWeapons(playerid);
forward SendPlayerOwnedWeapons(playerid);
forward player_ResetStats(playerid);
forward IsPlayerRegistered(playerid);
forward RegisterQuest(name[]);
forward ResetQuest(playerid);
forward oSetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size);
forward oDisablePlayerCheckpoint(playerid);
forward oIsPlayerInCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist);
forward CheckPlayerLevel(playerid);
forward PlayerLevelDown(playerid,level);
forward PlayerLevelUp(playerid,level);
forward GetPlayerTotalMoney(playerid);
forward oGetPlayerMoney(playerid);
forward PlayerSave(playerid);
forward CreatePlayer(playerid);
forward PlayerLoadStats(playerid);
forward JailPlayer(playerid);
forward UnJailPlayer(playerid);
forward GiveWantedLevel(playerid,level);
forward SetWantedLevel(playerid,level);
forward CheckWantedLevel(playerid);
forward CurrentCriminalsCheck(playerid);
forward SetColorForWantedLevel(playerid);
forward ADMDropAmmo(playerid);
forward IsPlayerAdm(playerid);
forward IsPlayerMod(playerid);

forward DropPlayerWeapons(playerid);
forward DeletePickup(pickupid);
forward GetWeaponModel(weaponid);
forward oGivePlayerWeapon(playerid,weaponid,ammo);
forward GetWeaponBulletsDown(weaponid);
forward GetWeaponSlot(weaponid);
forward MuteCheck();
forward JailTimer();
forward PlayerUpdate(playerid);

#define MAX_MONEY 50000000
#define PLAYER_SYNC_DELAY 1000
#define MAX_DESYNC 5000


new PlayerDB[MAX_STRING] = "GTO/Player/";

enum PlayerInfo {
	IsActive,      // is player online
	Float:Health,   // health
	Money,           // Money
	Bank,            // Money in bank
	Level,           // Level
	XP,              // XP
	Deaths,          // Deaths
	Kills,           // Kills
	Jailed,           // Jailed status
	Muted,            // Muted status
	CarKills,         // cars killed
	Status,		      // 0-user,1>-moderator,admins
	SkinModel,	     // Skin Model
	Hide,		     // Hide on the radar
	JailTime,	     // Time for Jails
	MuteTime,	     // Time for Mute
	ClickedPlayerid, // clickled player
	FightStyle,
	Name[20],
	Last_Name[20],
}

new Player[MAX_PLAYERS][PlayerInfo];

new PlayerSkin[MAX_PLAYERS];

#define PLAYER_WEAPON_SLOTS 13//  drop weapons
enum PWeap {
	pwid,
	pbullets
}
new PlayerWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap]; // 0 - weaponid, 1 - ammo
new PlayerTempWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap];

//DROP WEAPONS
new WDropTimer=10000;
new PlayerCurrentKills[MAX_PLAYERS];
new PlayerGangid[MAX_PLAYERS];
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
new PlayerStartMoney = 500;
new PlayerJailed = 0;
new PlayerMuted = 0;
new PlayerStartWeapon1 = WEAPON_COLT45;
new PlayerStartWeapon2 = 0;
new PlayerStartWeapon3 = 0;

enum PlayerSpawnInfo {
	playerspawn_level,
	Float:playerspawn_x,
	Float:playerspawn_y,
	Float:playerspawn_z,
	Float:playerspawn_heading
}

#define PLAYERSPAWNS_SIZE 38
new Float:LeveledPlayerSpawns[PLAYERSPAWNS_SIZE][PlayerSpawnInfo] = {
	{0,946.8751,1733.2366,8.8516,270.7313}, // lvl0spawn1
	{0,946.8751,1733.2366,8.8516,270.7313}, // lvl0ps
	{0,946.8751,1733.2366,8.8516,270.7313}, // lvl0ps
	{3,946.8751,1733.2366,8.8516,270.7313}, // lvl3spawn
	{3,946.8751,1733.2366,8.8516,270.7313}, // lvl3spawn
	{3,946.8751,1733.2366,8.8516,270.7313}, // lvl3spawn
	{3,946.8751,1733.2366,8.8516,270.7313}, // lvl3spawn
	{5,2033.1472,1530.3121,10.8203,278.1910}, // lvl5spawn
	{5,2033.1472,1530.3121,10.8203,278.1910}, // lv5spaawn
	{5,946.8751,1733.2366,8.8516,270.7313}, // lv5 spawn
	{6,946.8751,1733.2366,8.8516,270.7313}, // lv6spawn
	{6,2428.2900,1625.6614,10.8203,179.1623}, // lv6spawn
	{6,946.8751,1733.2366,8.8516,270.7313}, // lv6spawn
	{6,2428.2900,1625.6614,10.8203,179.1623}, // lv6spawn
	{6,2428.2900,1625.6614,10.8203,179.1623}, // lv6spawn
	{7,946.8751,1733.2366,8.8516,270.7313}, // lv7spawn
	{7,2087.9827,1381.0945,10.8203,193.2659}, // lv7spawn
	{7,946.8751,1733.2366,8.8516,270.7313}, // lv7spawn
	{7,2087.9827,1381.0945,10.8203,193.2659}, // lv7spawn
	{8,946.8751,1733.2366,8.8516,270.7313}, // lvl8player
	{8,946.8751,1733.2366,8.8516,270.7313}, // lvl8play
	{8,2033.1472,1530.3121,10.8203,278.1910}, // play8
	{8,2033.1472,1530.3121,10.8203,278.1910}, // play8
	{9,2428.2900,1625.6614,10.8203,179.1623}, // play9
	{9,2428.2900,1625.6614,10.8203,179.1623}, // play9
	{9,2428.2900,1625.6614,10.8203,179.1623}, // play9
	{10,946.8751,1733.2366,8.8516,270.7313}, // play10
	{10,2087.9827,1381.0945,10.8203,193.2659}, // play10
	{10,2087.9827,1381.0945,10.8203,193.2659}, // play10
	{11,2033.1472,1530.3121,10.8203,278.1910}, // play10
	{11,946.8751,1733.2366,8.8516,270.7313}, // play11
	{11,1284.9962,175.7558,20.3423,73.2174}, // play11
	{12,1426.9465,371.7816,18.8869,257.0709}, // play11
	{12,1572.3761,36.5942,24.5907,242.6472}, // play11
	{14,946.8751,1733.2366,8.8516,270.7313}, // play12
	{15,946.8751,1733.2366,8.8516,270.7313}, // play12
	{17,946.8751,1733.2366,8.8516,270.7313}, // play12
	{18,946.8751,1733.2366,8.8516,270.7313} // play12
};

enum CoordInfoJ {
    Float:CJ_X,
    Float:CJ_Y,
    Float:CJ_Z,
    Float:CJ_A
}
#define UNJP 8
new Float:JPH[UNJP][CoordInfoJ] = {
{633.365, -571.78, 16.340, 265.773},
{1544.982, -1675.470, 13.600, 93.446},
{-2164.396, -2388.342, 30.650, 140.551},
{-1605.378, 717.512, 12.000, 310.972},
{-1391.040, 2634.686, 55.984, 116.935},
{-215.718, 985.399, 19.400, 240.854},
{2335.229, 2455.809, 14.968, 136.734},
{1225.165, 245.328, 19.554, 306.501}
};

// LVDMSpawns
new Float:LVDMPlayerSpawns[8][CoordInfo] = {
	{945.9234,1733.2213,8.8516}, 
	{945.9234,1733.2213,8.8516},
	{945.9234,1733.2213,8.8516},
	{945.9234,1733.2213,8.8516},
	{945.9234,1733.2213,8.8516},
	{945.9234,1733.2213,8.8516},
	{945.9234,1733.2213,8.8516},
	{945.9234,1733.2213,8.8516}
};

PlayerLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_Get(file,"Player_DB",PlayerDB);
	ini_GetInt(file,"Player_Start_Money",PlayerStartMoney);
	ini_GetInt(file,"Player_Start_Weapon1",PlayerStartWeapon1);
	ini_GetInt(file,"Player_Start_Weapon2",PlayerStartWeapon2);
	ini_GetInt(file,"Player_Start_Weapon3",PlayerStartWeapon3);
	ini_GetInt(file,"Player_Jailed",PlayerJailed);
	ini_GetInt(file,"Player_Muted",PlayerMuted);
	ini_Close(file);
	return 1;
}

PlayerSaveConfig()
{
	if(!ini_Exist(ConfigDB)) return 1;
	new file = ini_Open(ConfigDB);
	ini_Set(file,"Player_DB",PlayerDB);
	ini_SetInt(file,"Player_Start_Money",PlayerStartMoney);
	ini_SetInt(file,"Player_Start_Weapon1",PlayerStartWeapon1);
	ini_SetInt(file,"Player_Start_Weapon2",PlayerStartWeapon2);
	ini_SetInt(file,"Player_Start_Weapon3",PlayerStartWeapon3);
	ini_SetInt(file,"Player_Jailed",PlayerJailed);
	ini_SetInt(file,"Player_Muted",PlayerMuted);
	ini_Close(file);
	return 1;
}

stock player_OnGameModeInit()
{
	PlayerLoadConfig();
	return 1;
}

stock Float:GetMaxHealth(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0.0;
	new Float:maxhealth=50.0;
	if(Player[playerid][Level] == 0) return 50.0;
	if(Player[playerid][Level] >= 10) return 100.0;
	maxhealth = (50.0 + (Player[playerid][Level] * 5.0));
	return maxhealth;
}

stock Float:oGetPlayerHealth(playerid)
{
	new Float:health;
	GetPlayerHealth(playerid, health);
	return health;
}

public SpawnTimer()
{
	for(new playerid;playerid<MAX_PLAYERS;playerid++)
	{
		if(IsPlayerConnected(playerid)) SpawnTime[playerid]++;
	}
}

public PlayerHealthRegen()
{
	for(new playerid;playerid<MAX_PLAYERS;playerid++) HealthRegen(playerid);
	return 1;
}

// needs to be ran by a timer
// sync player hp/money and check for mismatch
public SyncPlayers()
{
	for(new playerid;playerid<MAX_PLAYERS;playerid++)
	{
		if(PlayerJustTeleported[playerid] > 0) PlayerJustTeleported[playerid]--;
		SyncMoney(playerid);
		SyncHealth(playerid);
		CheckPlayerKick(playerid);
		JailPlayer(playerid);
	}
	return 1;
}

stock SyncMoney(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;
	new clientmoney = GetPlayerMoney(playerid);
	if(clientmoney < 0) return oSetPlayerMoney(playerid,0);
	if(clientmoney == Player[playerid][Money]) return 1;
	//if(clientmoney < Player[playerid][Money]) return oSetPlayerMoney(playerid,clientmoney);
	if(clientmoney > Player[playerid][Money]) return oSetPlayerMoney(playerid,Player[playerid][Money]);
	return 1;
}

stock SyncHealth(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;
	new Float:clienthealth = oGetPlayerHealth(playerid);
	if(clienthealth < 0) return oSetPlayerHealth(playerid,0);
	if(clienthealth == Player[playerid][Health]) return 1;
	if(clienthealth < Player[playerid][Health])
	{
		Player[playerid][Health] = clienthealth;
		return 1;
	}
	if(clienthealth > Player[playerid][Health]) return oSetPlayerHealth(playerid,Player[playerid][Health]);
	return 1;
}

// Check if any players have been marked to kick, and kicks them
public CheckPlayerKick(playerid)
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
			GangMemberLogout(playerid,PlayerGangid[playerid]);
			Kick(playerid);
			return;
		}
	}
}

public KickPlayer(playerid,reason[])
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
		SendClientMessageToRegistered(COLOUR_MISC, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: has been auto kicked. Reason: %s",playerid,oGetPlayerName(playerid),reason);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

public HealthRegen(id)
{
	if((Player[id][Health] <= 0) || (Player[id][Health] >= 100)) return 1;
	if(Player[id][Health] < MIN_HEALTH+(Player[id][Level]*5.0)) // if we regenerate health
	{
		new Float:newhealth;
		newhealth = Player[id][Health]+((Player[id][Level]*0.25));
		if(newhealth > 100) newhealth = 100;
		oSetPlayerHealth(id, newhealth); // give some health
	}
	return 1;
}

public GivePlayerOwnedWeapons(playerid)
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

public SendPlayerOwnedWeapons(playerid)
{
	new string[MAX_STRING];
	SendPlayerFormattedText(playerid,  lang_texts[9][7]  , 0,COLOUR_GREEN);
	for (new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{
		if (PlayerWeapons[playerid][weaponslot][pwid] <= 0) continue;
		if (PlayerWeapons[playerid][weaponslot][pbullets] > 1)
		{
			format(string,sizeof(string), lang_texts[9][8] ,oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]),PlayerWeapons[playerid][weaponslot][pbullets]);
			SendPlayerFormattedText(playerid, string,0,COLOUR_MISC);
		}
		else if (PlayerWeapons[playerid][weaponslot][pbullets] == 1)
		{
			format(string,sizeof(string), lang_texts[9][9] ,oGetWeaponName(PlayerWeapons[playerid][weaponslot][pwid]));
			SendPlayerFormattedText(playerid, string,0,COLOUR_MISC);
		}
	}
}
GetRndPlayerSpawnByLevel(level)
{
	new spawnlevel=0;
	for (new lookupid=0;lookupid<PLAYERSPAWNS_SIZE;lookupid++)
	{ // first we will get our highest available spawn point level (not all have spawnpoints, so we get the level below instead)
		if ((LeveledPlayerSpawns[lookupid][playerspawn_level] <= level) && (LeveledPlayerSpawns[lookupid][playerspawn_level] > spawnlevel))
		{
			spawnlevel = LeveledPlayerSpawns[lookupid][playerspawn_level];
		}
	}
	new leveledspawns[PLAYERSPAWNS_SIZE+1];
	new idx=0;
	for (new lookupid=0;lookupid<PLAYERSPAWNS_SIZE;lookupid++)
	{ // next we will create a list of all our found spawnpoints of that level
		if (LeveledPlayerSpawns[lookupid][playerspawn_level] == spawnlevel)
		{
			leveledspawns[idx] = lookupid;
			idx++;
		}
	}

	new spawnid;
	// now we will randomly select one of the spawns from our list
	if (idx != 0)
	{
		new rnd = random(idx);
		spawnid = leveledspawns[rnd];
	}
	else
	{
		spawnid = leveledspawns[0];
	}
	return spawnid; // return the id of the spawn we found
}

stock player_OnPlayerSpawn(playerid)
{
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
			if(Player[playerid][Level] > 12)
			{
				new rand = random(sizeof(LVDMPlayerSpawns));
				SetPlayerPos(playerid,LVDMPlayerSpawns[rand][Coord_X],LVDMPlayerSpawns[rand][Coord_Y],LVDMPlayerSpawns[rand][Coord_Z]); // Warp the player
				SetPlayerFacingAngle(playerid,271.8075);
			}
			else
			{
				new spawnid=GetRndPlayerSpawnByLevel(Player[playerid][Level]);
				SetPlayerPos(playerid, LeveledPlayerSpawns[spawnid][playerspawn_x], LeveledPlayerSpawns[spawnid][playerspawn_y], LeveledPlayerSpawns[spawnid][playerspawn_z]); // Warp the player
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
	oSetPlayerMoney(playerid,Player[playerid][Money]);
	SetPlayerXP(playerid,Player[playerid][XP]);
	PlayerJustTeleported[playerid]+=5;
	switch(Player[playerid][FightStyle])
	{
		case 0:SetPlayerFightingStyle(playerid, FIGHT_STYLE_NORMAL);
		case 1:SetPlayerFightingStyle(playerid, FIGHT_STYLE_BOXING);
		case 2:SetPlayerFightingStyle(playerid, FIGHT_STYLE_KUNGFU);
		case 3:SetPlayerFightingStyle(playerid, FIGHT_STYLE_KNEEHEAD);
		case 4:SetPlayerFightingStyle(playerid, FIGHT_STYLE_GRABKICK);
	}
}

public player_ResetStats(playerid)
{
	PlayerRegistered[playerid] = false;
	Player[playerid][Level] = 0;
	Player[playerid][XP] = 0;
	oSetPlayerMoney(playerid,0);
	Player[playerid][Kills] = 0;
	Player[playerid][Deaths] = 0;
	KickList[playerid] = 0;
	ResetQuest(playerid);
	SpawnTime[playerid] = 0;
	PlayerGangid[playerid] = 0;
	set(PlayerGangName[playerid],nullstr);
	PlayerSkin[playerid] = 0;
	ADMDropAmmo(playerid);
	PlayerJustTeleported[playerid] = 5;
	Player[playerid][Jailed] = 0;
	Player[playerid][Muted] = 0;
	Player[playerid][JailTime] = 0;
	Player[playerid][MuteTime] = 0;
	Player[playerid][Status] = 0;
	Player[playerid][Hide] = 0;
	Player[playerid][SkinModel] = 0;
	Player[playerid][FightStyle] = 0;
}

public IsPlayerRegistered(playerid)
{
	if(playerid == INVALID_PLAYER_ID) return 0;
	if(Player[playerid][IsActive] == 0) return 0;
	if(!IsPlayerConnected(playerid)) return 0;
	if(PlayerRegistered[playerid] == 0) return 0;
	return 1;
}

stock player_OnPlayerDisconnect(playerid,reason)
{
	PlaySoundForAll(1058);
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
	    case 0: format(string,sizeof(string),"%s. (Вылетел)",string);
	    case 1: format(string,sizeof(string),"%s. (Вышел)",string);
	    case 2: format(string,sizeof(string),"%s. (Кикнут)",string);
	}
	SendClientMessageToRegistered(COLOUR_GREY,string);
	GangMemberLogout(playerid,PlayerGangid[playerid]);
	Player[playerid][IsActive] = 0;
	player_ResetStats(playerid);
	DisablePlayerRaceCheckpoint(playerid);
	SendDeathMessage(INACTIVE_PLAYER_ID,playerid,201);
}

stock player_OnPlayerConnect(playerid)
{
	Player[playerid][IsActive] = 1;
	player_ResetStats(playerid);
	new playername[MAX_STRING];
	set(playername,oGetPlayerName(playerid));
	if(!NameCharCheck(playername))
	{
		SendPlayerFormattedText(playerid,  lang_texts[9][13] , 0,COLOUR_RED);
		KickPlayer(playerid,"Invalid character in playername."); // kick player
	}
	SetPlayerColor(playerid,COLOUR_PLAYER);
	PlaySoundForAll(1058);

	new string[MAX_STRING];
	format(string, sizeof(string),lang_texts[15][85], VERSION);
	GameTextForPlayer(playerid,string,5500,1);
	SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][86]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][87]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][88]);
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][90]);
	SendClientMessage(playerid, COLOUR_GREEN, "_________________________________________________________________________________");

	SetPlayerSkin(playerid,Player[playerid][SkinModel]);
	#if defined _testserver_included
	testserver_OnPlayerConnect(playerid);
	#endif
}

stock player_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused reason
	Player[playerid][Deaths]++;
	PlayerCurrentKills[playerid] = 0;
	if(!IsPlayerInAnyDM(playerid)){DropPlayerWeapons(playerid);}
	if(oGetPlayerHealth(killerid) > 100) KickPlayer(killerid,"Invalid health amount. (cheat?)");
	if((Player[playerid][Money] - 100) > 0) Player[playerid][Money] = Player[playerid][Money] - 100;
	else oSetPlayerMoney(playerid,0);
	if((Player[playerid][XP] - (Player[playerid][XP] / 30)) > 0)
	{
		if(!IsPlayerAdmin(playerid))
		{
			GivePlayerXP(playerid, (0 - (Player[playerid][XP] / 30)),1); // take death penalty xp
			Player[playerid][XP]=GetPlayerXP(playerid);
			CheckPlayerLevel(playerid);
		}
	}
	else SetPlayerXP(playerid,0);
}

stock player_OnPlayerKill(killerid, victimid, reason) // earn xp, kills++
{
	if(PlayerGangid[killerid] != 0)
	{
		if(GangKill(PlayerGangid[killerid],killerid,victimid,reason) == 1) return;
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
					new lostbullets;
					lostbullets = Vary(2,1);
					PlayerWeapons[killerid][slot][pbullets] = PlayerWeapons[killerid][slot][pbullets] - lostbullets;	
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

	Player[killerid][Kills]++;
	PlayerCurrentKills[killerid]++;
	
	// earn xp
	new earnedXP = ((1+Player[victimid][Level]) * 75);
	if (earnedXP > (1+Player[killerid][Level]) * 100)
	{
		earnedXP = ((1+Player[killerid][Level]) * 100);
	}
	if ((Player[killerid][XP] + earnedXP) <= MAX_LVLXP)
	{
		if (Player[killerid][XP] < MAX_LVLXP) 
		{
		GivePlayerXP(victimid,(0-earnedXP),1);
		GivePlayerXP(killerid, earnedXP,1);
		Player[killerid][XP]=GetPlayerXP(killerid);
		Player[victimid][XP]=GetPlayerXP(victimid);
		CheckPlayerLevel(victimid);
		CheckPlayerLevel(killerid);
		}
		else
		{
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[9][17]);
			SendClientMessage(killerid, COLOUR_RED, string);
		}
	}
	else SetPlayerXP(killerid, MAX_LVLXP);

	// steal cash
	new stolencash;
	if(oGetPlayerMoney(victimid) > 1000) stolencash = (oGetPlayerMoney(victimid) - (oGetPlayerMoney(victimid) / 7));
	else stolencash = oGetPlayerMoney(victimid);

	//ЕСЛИ У убийцы предел наличных, stolencash будет соответствовать!

	if((MAX_MONEY-Player[killerid][Money]) < stolencash) stolencash = (MAX_MONEY-Player[killerid][Money]);
	oGivePlayerMoney(victimid,0-stolencash,1);
	oGivePlayerMoney(killerid,stolencash,1);
	
	return;
}

public RegisterQuest(name[])
{
	if (QuestCount >= MAX_QUESTS) return INVALID_QUEST_ID;
	QuestCount++;
	new questid =QuestCount;
	set(Quests[questid][Quest_Name],name);
	return questid;
}

public ResetQuest(playerid)
{
	oDisablePlayerCheckpoint(playerid);
	PlayerQuest[playerid] = 0;
}

public oSetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size)
{
	DisablePlayerCheckpoint(playerid);
	SetPlayerCheckpoint(playerid, 0.0, 0.0, 0.0, 8);//size); // TODO: size bugged
	DisablePlayerCheckpoint(playerid);
	PlayerCP[playerid][Coord_X] = x;
	PlayerCP[playerid][Coord_Y] = y;
	PlayerCP[playerid][Coord_Z] = z;
	PlayerCPActive[playerid] = true;
	SetPlayerCheckpoint(playerid, x, y, z, 8);//size); // TODO: size bugged
}

public oDisablePlayerCheckpoint(playerid)
{
	DisablePlayerCheckpoint(playerid);
	PlayerCPActive[playerid] = false;
	PlayerCP[playerid][Coord_X] = 0.0;
	PlayerCP[playerid][Coord_Y] = 0.0;
	PlayerCP[playerid][Coord_Z] = 0.0;
}

public oIsPlayerInCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist)
{
	if (!IsPlayerConnected(playerid)) {return 0;}
	if (!PlayerCPActive[playerid]) {return 0;}
	if (!loccmp(cpx,cpy,cpz,PlayerCP[playerid][Coord_X],PlayerCP[playerid][Coord_Y],PlayerCP[playerid][Coord_Z])) return 0;
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	if(GetDistanceXYZtoXYZ(playerx,playery,playerz,cpx,cpy,cpz) < dist) return IsPlayerInCheckpoint(playerid);
	return 0;
}

stock GetXPToLevel(playerid,level)
{
	if(!IsPlayerConnected(playerid)) return 0;
	new xptolevel;
	if((level <= MAX_LEVEL-1) && (level > Player[playerid][Level]))
	{
		xptolevel = LevelList[level][level_xp] - Player[playerid][XP];
	}
	else xptolevel = 0;
	return xptolevel;
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

stock GetPlayerLevel(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	return Player[playerid][Level];
}

stock GetPlayerXP(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	return Player[playerid][XP];
}

stock SetPlayerXP(playerid, xpamount)
{
	if (!IsPlayerConnected(playerid)) return 1;
	return Player[playerid][XP] = xpamount;
}

stock GivePlayerXP(playerid, xpamount,showtext) // showtext = do we display "You gain %d XP"  to client
{
	if(!IsPlayerConnected(playerid)) return 1;
	if(PlayerGangid[playerid] != 0)
	{
		if(xpamount > 100)
		{
			new gangxp = xpamount / 4;
			GangGiveXP(PlayerGangid[playerid],gangxp,playerid);
		}
	}
	
	new texttime;
	if(xpamount <= 50) texttime = 500;
	else if(xpamount < 200) texttime = 1000;
	else if(xpamount < 500) texttime = 1500;
	else if(xpamount < 1000) texttime = 2000;
	else if(xpamount < 2000) texttime = 4000;
	else texttime = 5000;
	
	new string[MAX_STRING];
	if((GetPlayerXP(playerid) + xpamount) < 0) // if after add xp, playerxp < 0
	{
		SetPlayerXP(playerid, 0);
		if (showtext == 1)
		{
			format(string, sizeof(string),  lang_texts[9][15] , xpamount);
			SendClientMessage(playerid, COLOUR_XP_BAD, string);
		}
		return 1;
	}

	if((GetPlayerXP(playerid) + xpamount) > MAX_LVLXP) // if after add, playerxp > MAX
	{
		if(showtext == 1)
		{
			format(string, sizeof(string),  lang_texts[9][16] , MAX_LVLXP - GetPlayerXP(playerid));
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
			SendClientMessage(playerid, COLOUR_RED, lang_texts[9][17] );
		}
		SetPlayerXP(playerid, MAX_LVLXP);
		return 1;
	}

	if(xpamount >= 0)
	{
		format(string, sizeof(string),  lang_texts[9][18] , xpamount);
		GameTextForPlayer(playerid,string,texttime,3);
	}
	else
	{
		new levelxp = LevelList[GetPlayerLevel(playerid)][level_xp];
		new xpfromlevel = GetPlayerXP(playerid) - levelxp;
		if ((xpfromlevel + xpamount) < levelxp)
		{
			xpamount = 0-xpfromlevel;
		}
		if(xpamount > 0)
		{
			format(string, sizeof(string),  lang_texts[9][19] , xpamount);
			GameTextForPlayer(playerid,string,texttime,3);
		}
	}

	SetPlayerXP(playerid, (GetPlayerXP(playerid) + xpamount));
	if(showtext == 1)
	{
		if(xpamount > 0)
		{
			format(string, sizeof(string),  lang_texts[9][20] , xpamount);
			SendClientMessage(playerid, COLOUR_XP_GOOD, string);
		}
		else if(xpamount > 0)
		{
			format(string, sizeof(string),  lang_texts[9][21] , xpamount);
			SendClientMessage(playerid, COLOUR_XP_BAD, string);
		}
	}
	Player[playerid][XP]=GetPlayerXP(playerid);
	CheckPlayerLevel(playerid);
	return 1;
}

public CheckPlayerLevel(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;
	new level = CalculatePlayerLevel(playerid);
	if(Player[playerid][Level] < level) PlayerLevelUp(playerid,level);
	else if(Player[playerid][Level] > level) PlayerLevelDown(playerid,level);
	Player[playerid][XP]=GetPlayerXP(playerid);
	return 1;
}

public PlayerLevelDown(playerid,level)
{
	if(!IsPlayerConnected(playerid)) return 1;
	new string[MAX_STRING];
	Player[playerid][Level] = level;
	format(string, sizeof(string),  lang_texts[9][22] , Player[playerid][Level]);
	SendClientMessage(playerid, COLOUR_XP_BAD, string);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "player: %d:  %s: decreased his level to %d",playerid,oGetPlayerName(playerid),Player[playerid][Level]);
	WriteLog(GameLog,logstring);
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	PlayerPlaySound(playerid,1057,playerx,playery,playerz);
	UpdatePlayerScore(playerid);
	return 1;
}

public PlayerLevelUp(playerid,level)
{
	if(!IsPlayerConnected(playerid)) return 1;
	new string[MAX_STRING];
	Player[playerid][Level] = level;
	oSetPlayerHealth(playerid,GetMaxHealth(playerid));
	format(string, sizeof(string),  lang_texts[9][23] , Player[playerid][Level]);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "player: %d:  %s: increased his level to %d",playerid,oGetPlayerName(playerid),Player[playerid][Level]);
	WriteLog(GameLog,logstring);
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	PlayerPlaySound(playerid,1057,playerx,playery,playerz);
	UpdatePlayerScore(playerid);

	new wepfound;
	for(new weaponid=1;weaponid<MAX_WEAPONS;weaponid++)
	{
		new lookupid=GetWeaponLookupID(weaponid);
		if(lookupid == -1) continue;
		if(Weapons[lookupid][Weapon_Allowed] == 0) continue;
		if(Weapons[lookupid][Weapon_Level] == GetPlayerLevel(playerid))
		{
			if(wepfound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN,  lang_texts[9][24] );
				wepfound=1;
			}
			format(string, sizeof(string),  lang_texts[9][25] ,oGetWeaponName(weaponid),GetWeaponCost(weaponid));
			SendClientMessage(playerid, COLOUR_MISC, string);
		}
	}
	return 1;
}

stock oSetPlayerMoney(playerid,money)
{
    Player[playerid][Money] = money;
	ResetPlayerMoney(playerid);
	return GivePlayerMoney(playerid,money);
}

stock oGivePlayerMoney(playerid,money,showtext)
{
	new string[MAX_STRING];
	if((Player[playerid][Money] + money) > MAX_MONEY)
	{
		oSetPlayerMoney(playerid,MAX_MONEY);
		format(string, sizeof(string),  lang_texts[9][26] , money);
		SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: is on max money",playerid,oGetPlayerName(playerid));
		WriteLog(GameLog,logstring);
		return 1;
	}

	if(showtext == 1)
	{
		if(money >= 0)
		{
			format(string, sizeof(string), lang_texts[9][28], money);
			SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
		}
		else
		{
			format(string, sizeof(string), lang_texts[9][29], money);
			SendClientMessage(playerid, COLOUR_MONEY_BAD, string);
		}
	}
	return oSetPlayerMoney(playerid,Player[playerid][Money] + money);
}

stock GetPlayerTotalMoney(playerid)
{
	new money;
	money += Player[playerid][Money];
	money += Player[playerid][Bank];
	return money;
}

stock oGetPlayerMoney(playerid)
{
	return Player[playerid][Money];
}

stock UpdatePlayerScore(playerid)
{
	SetPlayerScore(playerid,Player[playerid][Level]);
}

stock oGetPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock oSetPlayerHealth(playerid,Float:health)
{
	Player[playerid][Health] = health;
	return SetPlayerHealth(playerid, health);
}

public oGivePlayerWeapon(playerid,weaponid,ammo)
{
	if (!IsPlayerAllowedWeapon(playerid,weaponid)) return;	//Mad
	new slot = GetWeaponSlot(weaponid);	//Mad

	if (ammo > 0)
	{
		if (!IsWeaponHandToHand(weaponid))
		{
//			PlayerWeapons[playerid][weaponid] = PlayerWeapons[playerid][weaponid] + ammo;
			PlayerWeapons[playerid][slot][pwid] = weaponid;
			PlayerWeapons[playerid][slot][pbullets] = PlayerWeapons[playerid][slot][pbullets] + ammo;

		}
		else
		{
//			PlayerWeapons[playerid][weaponid] = PlayerWeapons[playerid][weaponid] + ammo;
			PlayerWeapons[playerid][slot][pwid] = weaponid;
			PlayerWeapons[playerid][slot][pbullets] = 1;

		}

		GivePlayerWeapon(playerid,weaponid,ammo);
	}
}

public PlayerSave(playerid)
{
	if(!IsPlayerRegistered(playerid)) return 0;
	player_save_db_ini(playerid);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "player: %d:  %s: player_saved successfully",playerid,oGetPlayerName(playerid));
	WriteLog(GameLog,logstring);
	return 1;
}

public CreatePlayer(playerid)
{
	oSetPlayerMoney(playerid,PlayerStartMoney);
	SetPlayerXP(playerid,0);
	ResetPlayerWeapons(playerid);
	new bullets = 100;
	if(PlayerStartWeapon1 != 0)
	{
		if(bullets > GetWeaponMaxAmmo(PlayerStartWeapon1)) oGivePlayerWeapon(playerid,PlayerStartWeapon1,GetWeaponMaxAmmo(PlayerStartWeapon1));
		else oGivePlayerWeapon(playerid,PlayerStartWeapon1,bullets);
	}
	if(PlayerStartWeapon2 != 0)
	{
		if(bullets > GetWeaponMaxAmmo(PlayerStartWeapon2)) oGivePlayerWeapon(playerid,PlayerStartWeapon2,GetWeaponMaxAmmo(PlayerStartWeapon2));
		else oGivePlayerWeapon(playerid,PlayerStartWeapon2,bullets);
	}
	if(PlayerStartWeapon3 != 0)
	{
		if(bullets > GetWeaponMaxAmmo(PlayerStartWeapon3)) oGivePlayerWeapon(playerid,PlayerStartWeapon3,GetWeaponMaxAmmo(PlayerStartWeapon3));
		else oGivePlayerWeapon(playerid,PlayerStartWeapon3,bullets);
	}
	player_create_db_ini(playerid);
	new logstring[MAX_STRING];
	format(logstring, sizeof(logstring), "player: %d:  %s: player_created successfully",playerid,oGetPlayerName(playerid));
	WriteLog(GameLog,logstring);
	return 1;
}

player_create_db_ini(playerid)
{
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",PlayerDB,oGetPlayerName(playerid));

	if(ini_Exist(filename)) return SendClientMessage(playerid,COLOUR_RED, lang_texts[9][30]);
	player_save_db_ini(playerid);
	return 1;
}

SetWeaponsFromDBString(playerid,dbstring[])
{
	new idx;
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{

		if (IsPlayerAllowedWeapon(playerid,PlayerWeapons[playerid][i][pwid]))
		{
			if(PlayerWeapons[playerid][i][pbullets]>25535) PlayerWeapons[playerid][i][pbullets]=1;
		}
		else
		{
//			Заменить ИД текущего оружия на ИД разрешенного в данном слоте i
//			PlayerWeapons[playerid][i][pbullets] = 0;
		}

		PlayerWeapons[playerid][i][pwid] = strval(strcharsplit(dbstring,idx,strchar("|")));
		PlayerWeapons[playerid][i][pbullets] = strval(strcharsplit(dbstring,idx,strchar("/")));
		if (PlayerWeapons[playerid][i][pbullets] > 12000) {PlayerWeapons[playerid][i][pbullets]=12000;}
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
		wepstr[strlen(wepstr)] = strchar("/");
		valstr(temp,PlayerWeapons[playerid][i][pbullets]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] = strchar("|");
	}
	return wepstr;
}

player_save_db_ini(playerid)
{
	if(strlen(oGetPlayerName(playerid)) < 1) return 1;
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",PlayerDB,oGetPlayerName(playerid));
	if(!ini_Exist(filename)) {new File:fhnd = fopen(filename,io_write);fclose(fhnd);}
	new file = ini_Open(filename);
	ini_Set(file,"Name",oGetPlayerName(playerid));
	ini_Set(file,"Gang",PlayerGangName[playerid]);
	ini_SetInt(file,"Level",Player[playerid][Level]);
	ini_SetInt(file,"XP",Player[playerid][XP]);
	ini_SetInt(file,"Money",Player[playerid][Money]);
	ini_SetInt(file,"BankMoney",Player[playerid][Bank]);
	ini_SetInt(file,"Deaths",Player[playerid][Deaths]);
	ini_SetInt(file,"Kills",Player[playerid][Kills]);
	ini_SetInt(file,"Jailed",Player[playerid][Jailed]);
	ini_SetInt(file,"Muted",Player[playerid][Muted]);
	ini_SetInt(file,"JailTime",Player[playerid][JailTime]);
	ini_SetInt(file,"MuteTime",Player[playerid][MuteTime]);
	ini_SetInt(file,"FightStyle",Player[playerid][FightStyle]);
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	if(z > 900) {x=0;y=0;z=0;}
	new string[MAX_STRING];
	format(string,sizeof(string),"%f,%f,%f",x,y,z);
	ini_Set(file,"Coord",string);
	ini_SetInt(file,"Status",Player[playerid][Status]);
	ini_Set(file,"Weapons",CreateWeaponDBString(playerid));
	ini_SetInt(file,"SkinModel",Player[playerid][SkinModel]);
	ini_Close(file);
	return 1;
}

public PlayerLoadStats(playerid)
{
	if (!IsPlayerRegistered(playerid)) return 0; // login to acct first
	player_load_db_ini(playerid);
	return 1;
}

player_load_db_ini(playerid)
{
	new filename[MAX_STRING];
	format(filename,sizeof(filename),"%s%s.txt",PlayerDB,oGetPlayerName(playerid));
	if(!ini_Exist(filename)) return 1;
	new file = ini_Open(filename);
    ini_GetInt(file,"Level",Player[playerid][Level]);
    ini_GetInt(file,"XP",Player[playerid][XP]);
	new money;
    ini_GetInt(file,"Money",money);
    oSetPlayerMoney(playerid,money);
    ini_GetInt(file,"BankMoney",Player[playerid][Bank]);
    ini_GetInt(file,"Deaths",Player[playerid][Deaths]);
    ini_GetInt(file,"Kills",Player[playerid][Kills]);
    ini_GetInt(file,"Jailed",Player[playerid][Jailed]);
    ini_GetInt(file,"Muted",Player[playerid][Muted]);
    ini_GetInt(file,"JailTime",Player[playerid][JailTime]);
    ini_GetInt(file,"MuteTime",Player[playerid][MuteTime]);
    ini_GetInt(file,"FightStyle",Player[playerid][FightStyle]);
    ini_GetInt(file,"Level",Player[playerid][Level]);
	new string[MAX_STRING],idx=0;
	ini_Get(file,"Coords",string);
	PlayerSpawn[playerid][Coord_X] = floatstr(strcharsplit(string,idx,strchar(",")));
	PlayerSpawn[playerid][Coord_Y] = floatstr(strcharsplit(string,idx,strchar(",")));
	PlayerSpawn[playerid][Coord_Z] = floatstr(strcharsplit(string,idx,strchar(",")));
    ini_Get(file,"Status",Player[playerid][Status]);
	SetPlayerFightingStyle(playerid,Player[playerid][FightStyle]);
	new gangname[MAX_NAME];
	ini_Get(file,"Gang",gangname);
	if(strlen(gangname) > 0)
	{
		set(PlayerGangName[playerid],gangname);
		PlayerGangid[playerid] = GangLoad(gangname);
		GangMemberLogin(playerid,PlayerGangid[playerid]);
		if(PlayerGangid[playerid] == 0)
		{
			new tmpname[MAX_NAME];
			set(PlayerGangName[playerid],tmpname);
			PlayerGangName[playerid][0] = 0;
			PlayerGangid[playerid] = 0;
		}
	}
	else PlayerGangid[playerid] = 0; // no gang

	new wepstr[MAX_STRING];
	ini_Get(file,"Weapons",wepstr);
	SetWeaponsFromDBString(playerid,wepstr);
	ini_GetInt(file,"SkinModel",Player[playerid][SkinModel]);
	UpdatePlayerScore(playerid);
	set(Player[playerid][Last_Name],oGetPlayerName(playerid));
	ini_Close(file);
	return 1;
}

public JailPlayer(playerid)
{
	if(Player[playerid][Jailed] == 1)
	{
		SetPlayerInterior(playerid,6);
		SetPlayerPos(playerid,265.1273,77.6823,1001.0391);
		SetPlayerFacingAngle(playerid,-90);
		new spwl=Player[playerid][JailTime];
		if(spwl==0 || spwl>6) spwl=6;
		SetPlayerWantedLevel(playerid,spwl);
		TogglePlayerControllable(playerid,0);
		SetCameraBehindPlayer(playerid);
		SetPlayerArmour(playerid, 0);
	}
}

public UnJailPlayer(playerid)
{
	if(Player[playerid][Jailed] == 0)
	{
		Player[playerid][Jailed] = 0;
		Player[playerid][JailTime] = 0;
	}
	new u = random(UNJP);
	SetPlayerInterior(playerid,0);
	SetPlayerPos(playerid, JPH[u][CJ_X], JPH[u][CJ_Y], JPH[u][CJ_Z] );
	SetPlayerFacingAngle(playerid,JPH[u][CJ_A]);
	SetCameraBehindPlayer(playerid);
	SetPlayerWantedLevel(playerid, 0);
	TogglePlayerControllable(playerid,1);
	new string[MAX_STRING];
	format(string,sizeof(string), "~r~%s ~w~Now is Free", oGetPlayerName(playerid));
	GameTextForAll(string,5000,4);
	format(string,sizeof(string), "SERVER: %s был Выпущен из тюрьмы (Истекло время или Амнистия)", oGetPlayerName(playerid));
	SendClientMessageToAll(COLOUR_WHITE, string);
}

public MuteCheck()
{
	for(new i=0;i<MAX_PLAYERS;i++)
	{
		if(IsPlayerConnected(i))
		{
			if(Player[i][MuteTime]>0)
			{
			    Player[i][MuteTime]--;
			    if(Player[i][MuteTime]==0)
			    {
					new string[MAX_STRING];
					format(string, sizeof(string)," Игрок %s снова может говорить",oGetPlayerName(i));
					SendClientMessageToAll(COLOUR_YELLOW,string);
				}
			}
		}
	}
}

public JailTimer()
{
	for(new i=0;i<MAX_PLAYERS;i++)
	{
		if(IsPlayerConnected(i))
		{
			if(Player[i][Jailed] == 1)
			{
				new spwl=Player[i][JailTime];
				if(spwl == 0 || spwl > 6) spwl=6;
				SetPlayerWantedLevel(i,spwl);
			}
			if(Player[i][Jailed] == 1 && Player[i][JailTime] >= 1)
			{
				Player[i][JailTime]--;
				if(Player[i][JailTime]<=0)
				{
					Player[i][Jailed] = 0;
					Player[i][JailTime] = 0;
					UnJailPlayer(i);
					printf("SERVER : %s Has Been auto-UNJailed",oGetPlayerName(i));
					return 1;
				}
				else
				{
					new string[MAX_STRING];
					format(string,sizeof(string), "SERVER: До окончания ареста осталось около %d мин", Player[i][JailTime]);
					SendClientMessage(i,COLOUR_LIGHTRED, string);
				}
			}
		}
	}
	return 1;
}

public ADMDropAmmo(playerid)
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

public IsPlayerAdm(playerid)
{
	if(Player[playerid][Status] >= 11) return 1;
	return 0;
}

public IsPlayerMod(playerid)
{
	if(Player[playerid][Status] > 0) return 1;
	return 0;
}

public GetWeaponModel(weaponid)
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

public GetWeaponBulletsDown(weaponid)
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

public DeletePickup(pickupid)
{
	DestroyPickup(pickupid);
	return 1;
}

public DropPlayerWeapons(playerid)
{
	new pickupid,playerweapons[13][2];
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid, x, y, z);
	new drpick = random(12)+1;
	for(new i=0;i<13;i++)
	{
    	GetPlayerWeaponData(playerid, i, playerweapons[i][0], playerweapons[i][1]);
    	new model=GetWeaponModel(playerweapons[i][0]);
        new dbw=GetWeaponBulletsDown(playerweapons[i][0]);
        new Float:X=x+(random(4)-random(4));
        new Float:Y=y+(random(4)-random(4));
        if(playerweapons[i][1]!=0)
		{
			if(model!=-1 && model!=359 && model!=360 && model!=362)
			{
				pickupid = CreatePickup(model, 3, X, Y, z);
				SetTimerEx("DeletePickup", WDropTimer+(i*500), false, "d", pickupid);
				pickupid++;
				if((PlayerWeapons[playerid][i][pbullets] < dbw) && (PlayerWeapons[playerid][i][pbullets] > 0) )
				{
					PlayerWeapons[playerid][i][pbullets] = 0;
					PlayerWeapons[playerid][i][pwid] = 0;
				}
				if (PlayerWeapons[playerid][i][pbullets] >= dbw) PlayerWeapons[playerid][i][pbullets] -= dbw;
				if (PlayerWeapons[playerid][i][pwid]==46)
				{
					PlayerWeapons[playerid][i][pwid]=0;
					PlayerWeapons[playerid][i][pbullets] = 0;
				}
				drpick--;
				if(drpick==0) return 1;
			}
		}
		CreateWeaponDBString(playerid);
	}
	return 1;
}

public GetWeaponSlot(weaponid)
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
