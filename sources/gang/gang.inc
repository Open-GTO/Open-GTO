/*
	
	About: gang system
	Author: ziggi

*/

#if defined _gang_included
	#endinput
#endif

#define _gang_included
#pragma library gang

/*
	Defines
*/

#if !defined MAX_GANG
	#define MAX_GANG MAX_PLAYERS
#endif

#if !defined MAX_GANG_SIZE
	#define MAX_GANG_SIZE 12
#endif

#if !defined MIN_GANG_SIZE
	#define MIN_GANG_SIZE 3
#endif

#if !defined MAX_GANG_NAME
	#define MAX_GANG_NAME 40
#endif

#if !defined MAX_GANG_MOTD
	#define MAX_GANG_MOTD 64
#endif

#if !defined GANG_CREATE_COST
	#define GANG_CREATE_COST 500000
#endif

#if !defined GANG_COLOR_COST
	#define GANG_COLOR_COST 20000
#endif

#if !defined MAX_GANG_BANK
	#define MAX_GANG_BANK 2000000000
#endif

/*
	Enums
*/

enum e_Gang_Member_Info {
	e_gmName[MAX_PLAYER_NAME + 1],
	e_gmID,
	GangMemberRank:e_gmRank,
	bool:e_gmIsExists,
}

enum e_Gang_Info {
	e_gName[MAX_GANG_NAME],
	e_gMotd[MAX_GANG_MOTD],
	e_gColor,
	e_gKills,
	e_gScore,
	e_gXP,
	e_gLevel,
	e_gMoney,
	e_gHouse,
}

/*
	Vars
*/

static
	gGangDB[] = DATA_FILES_FOLDER "gang/",

	gGang[MAX_GANG][e_Gang_Info],
	gMember[MAX_GANG][MAX_GANG_SIZE][e_Gang_Member_Info],

	gCreateCost = GANG_CREATE_COST,
	gColorCost = GANG_COLOR_COST,

	gPlayerGiveXP[MAX_PLAYERS],

	Iterator:LoadedGangs<MAX_GANG>,
	Iterator:LoadedGangMembers[MAX_GANGS]<MAX_GANG_SIZE>;

/*
	Config
*/

stock Gang_LoadConfig(file_config)
{
	ini_getString(file_config, "Gang_DB", gGangDB);
	ini_getInteger(file_config, "Gang_Create_Cost", gCreateCost);
	ini_getInteger(file_config, "Gang_Color_Cost", gColorCost);
}

stock Gang_SaveConfig(file_config)
{
	ini_setString(file_config, "Gang_DB", gGangDB);
	ini_setInteger(file_config, "Gang_Create_Cost", gCreateCost);
	ini_setInteger(file_config, "Gang_Color_Cost", gColorCost);
}

/*
	Gang_Load
*/

stock Gang_Load(gangname[])
{
	new gangid;
	
	gangid = Gang_GetID(gangname);
	if (gangid != INVALID_GANG_ID) {
		return INVALID_GANG_ID;
	}

	gangid = Gang_GetFreeSlot();
	if (gangid == INVALID_GANG_ID) {
		Log_Debug("Error <gang:Gang_Load>: free slot not found (%s).", gangname);
		return INVALID_GANG_ID;
	}

	// load db
	new string[MAX_STRING];

	format(string, sizeof(string), "%s%s" DATA_FILES_FORMAT, gGangDB, gangname);

	if (!ini_fileExist(string)) {
		Log_Debug("Error <gang:Gang_Load>: file not found (%s).", string);
		return INVALID_GANG_ID;
	}

	new file = ini_openFile(string);

	ini_getString(file, "Name", gGang[gangid][e_gName], MAX_GANG_NAME);
	ini_getString(file, "Motd", gGang[gangid][e_gMotd], MAX_GANG_MOTD);
	ini_getInteger(file, "Color", gGang[gangid][e_gColor]);
	ini_getInteger(file, "Kills", gGang[gangid][e_gKills]);
	ini_getInteger(file, "Score", gGang[gangid][e_gScore]);
	ini_getInteger(file, "XP", gGang[gangid][e_gXP]);
	ini_getInteger(file, "Level", gGang[gangid][e_gLevel]);
	ini_getInteger(file, "Money", gGang[gangid][e_gMoney]);
	ini_getInteger(file, "House", gGang[gangid][e_gHouse]);

	for (new memberid = 0, tmp_int; memberid < MAX_GANG_SIZE; memberid++) {
		gMember[gangid][memberid][e_gmID] = INVALID_PLAYER_ID;
		
		format(string, sizeof(string), "Member%d", memberid);
		ini_getString(file, string, gMember[gangid][memberid][e_gmName], MAX_PLAYER_NAME + 1);

		if (strlen(gMember[gangid][memberid][e_gmName]) > 0) {
			gMember[gangid][memberid][e_gmIsExists] = true;
		}

		format(string, sizeof(string), "MemberRank%d", memberid);
		ini_getInteger(file, string, _:gMember[gangid][memberid][e_gmRank]);
		gMember[gangid][memberid][e_gmRank] = GangMemberRank:tmp_int;
	}

	ini_closeFile(file);

	// load other
	Iter_Init(LoadedGangMembers);

	return gangid;
}

/*
	Gang_Unload
*/

stock Gang_Unload(gangid)
{
	Gang_Save(gangid);
	
	foreach (new memberid : LoadedGangMembers[gangid]) {
		gMember[gangid][memberid][e_gmID] = INVALID_PLAYER_ID;

		Iter_SafeRemove(LoadedGangMembers[gangid], memberid, memberid);
	}

	Iter_Remove(LoadedGangs, gangid);
}

/*
	Gang_Save
*/

stock Gang_Save(gangid)
{
	if (!Gang_IsActive(gangid)) {
		return 0;
	}

	new
		string[MAX_STRING],
		file;

	Gang_GetName(gangid, string);
	format(string, sizeof(string), "%s%s" DATA_FILES_FORMAT, gGangDB, string);

	if (!ini_fileExist(string)) {
		file = ini_createFile(string);
	} else {
		file = ini_openFile(string);
	}

	ini_setString(file, "Name", gGang[gangid][e_gName]);
	ini_setString(file, "Motd", gGang[gangid][e_gMotd]);
	ini_setInteger(file, "Color", gGang[gangid][e_gColor]);
	ini_setInteger(file, "Kills", gGang[gangid][e_gKills]);
	ini_setInteger(file, "Score", gGang[gangid][e_gScore]);
	ini_setInteger(file, "XP", gGang[gangid][e_gXP]);
	ini_setInteger(file, "Level", gGang[gangid][e_gLevel]);
	ini_setInteger(file, "Money", gGang[gangid][e_gMoney]);
	ini_setInteger(file, "House", gGang[gangid][e_gHouse]);

	for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
		format(string, sizeof(string), "Member%d", memberid);
		ini_setString(file, string, gMember[gangid][memberid][e_gmName]);

		gMember[gangid][memberid][e_gmIsExists] = false;

		format(string, sizeof(string), "MemberRank%d", memberid);
		ini_setInteger(file, string, _:gMember[gangid][memberid][e_gmRank]);
	}

	ini_closeFile(file);
	return 1;
}

/*
	Gang_SaveAll
*/

stock Gang_SaveAll()
{
	foreach (new gangid : LoadedGangs) {
		Gang_Save(gangid);

		if (Gang_GetOnlineCount(gangid) == 0) {
			Gang_Unload(gangid);
		}
	}
}

/*
	Gang_Create
*/

stock Gang_Create(playerid, gangname[], gangcolour)
{
	new 
		string[MAX_STRING];

	format(string, sizeof(string), "%s%s" DATA_FILES_FORMAT, gGangDB, gangname);

	if (ini_fileExist(string)) {
		Log_Debug("Error <gang:Gang_Create>: gang file already exists (%s).", gangname);
		return INVALID_GANG_ID;
	}

	new gangid = Gang_GetFreeSlot();
	if (gangid == INVALID_GANG_ID) {
		Log_Debug("Error <gang:Gang_Create>: free slot not found (%s).", gangname);
		return INVALID_GANG_ID;
	}

	strmid(gGang[gangid][e_gName], gangname, 0, strlen(gangname), MAX_GANG_NAME);
	gGang[gangid][e_gMotd][0] = '\0';
	gGang[gangid][e_gColor] = gangcolour;
	gGang[gangid][e_gKills] = 0;
	gGang[gangid][e_gScore] = 0;
	gGang[gangid][e_gXP] = 0;
	gGang[gangid][e_gLevel] = 0;
	gGang[gangid][e_gMoney] = 0;
	gGang[gangid][e_gHouse] = Player_GetSpawnHouseID(playerid);

	for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
		gMember[gangid][memberid][e_gmID] = INVALID_PLAYER_ID;
		gMember[gangid][memberid][e_gmName][0] = '\0';
		gMember[gangid][memberid][e_gmIsExists] = false;
		gMember[gangid][memberid][e_gmRank] = GangMemberSoldier;
	}

	Gang_SetActiveStatus(gangid, true);
	Gang_Save(gangid);

	Gang_MemberJoin(gangid, playerid, GangMemberLeader);

	return gangid;
}

/*
	Gang_Remove
*/

stock Gang_Remove(gangid)
{
	new 
		string[MAX_STRING];

	Gang_GetName(gangid, string);
	format(string, sizeof(string), "%s%s" DATA_FILES_FORMAT, gGangDB, string);

	if (!ini_fileExist(string)) {
		Log_Debug("Error <gang:Gang_Create>: gang file already exists (%s).", string);
		return 0;
	}

	foreach (new memberid : LoadedGangMembers[gangid]) {
		Gang_MemberRemove(gangid, memberid);
	}

	Gang_Unload(gangid);

	ini_fileRemove(string);
	return 1;
}

/*
	Gang_MemberJoin
*/

stock Gang_MemberJoin(gangid, playerid, GangMemberRank:rank = GangMemberSoldier)
{
	if (!Gang_IsActive(gangid)) {
		Log_Debug("Error <gang:Gang_MemberJoin>: gang is not active.");
		return 0;
	}

	if (GetPlayerGangID(playerid) != 0) {
		Log_Debug("Error <gang:Gang_MemberJoin>: player already in a gang.");
		return 0;
	}

	if (Gang_GetOnlineCount(gangid) >= MAX_GANG_SIZE) {
		Log_Debug("Error <gang:Gang_MemberJoin>: gang is already full.");
		return 0;
	}

	new memberid = GangMember_GetFreeSlot(gangid);
	if (memberid == INVALID_MEMBER_ID) {
		Log_Debug("Error <gang:Gang_MemberJoin>: free slot not found.");
		return 0;
	}

	GetPlayerName(playerid, gMember[gangid][memberid][e_gmName], MAX_PLAYER_NAME + 1);
	gMember[gangid][memberid][e_gmID] = playerid;
	gMember[gangid][memberid][e_gmIsExists] = true;
	gMember[gangid][memberid][e_gmRank] = rank;

	return Gang_MemberLogin(playerid, gangid, memberid);
}

/*
	Gang_MemberRemove
*/

stock Gang_MemberRemove(gangid, memberid)
{
	new playerid = gMember[gangid][memberid][e_gmID];
	
	gMember[gangid][memberid][e_gmID] = INVALID_PLAYER_ID;
	gMember[gangid][memberid][e_gmName][0] = '\0';
	gMember[gangid][memberid][e_gmIsExists] = false;
	gMember[gangid][memberid][e_gmRank] = GangMemberSoldier;

	SetPlayerGangID(playerid, INVALID_GANG_ID);
	SetPlayerGangName(playerid, "");
	SetPlayerGangMemberID(playerid, INVALID_MEMBER_ID);
	SetPlayerColor(playerid, Gang_GetColor(gangid));

	if (Player_GetSpawnType(playerid) == SPAWN_TYPE_GANG) {
		Player_SetSpawnType(playerid, SPAWN_TYPE_NONE);
	}

	return 1;
}

/*
	Gang_MemberLogin
*/

stock Gang_MemberLogin(playerid, gangid, memberid = INVALID_MEMBER_ID)
{
	if (!Gang_IsActive(gangid)) {
		Log_Debug("Error <gang:Gang_MemberLogin>: gang is not active.");
		return 0;
	}

	new
		gang_name[MAX_GANG_NAME + 1];

	Gang_GetName(gangid, gang_name);

	if (memberid == INVALID_MEMBER_ID) {
		memberid = GangMember_GetFreeSlot(gangid);
	}

	gMember[gangid][memberid][e_gmID] = playerid;
	GangMember_SetActiveStatus(gangid, memberid, true);

	SetPlayerGangID(playerid, gangid);
	SetPlayerGangName(playerid, gang_name);
	SetPlayerGangMemberID(playerid, memberid);
	SetPlayerColor(playerid, Gang_GetColor(gangid));
	SetPlayerInvitedGangID(playerid, gangid, false);

	return 1;
}

/*
	Gang_MemberLogout
*/

stock Gang_MemberLogout(playerid, gangid)
{
	if (!Gang_IsActive(gangid)) {
		Log_Debug("Error <gang:Gang_MemberLogout>: gang is not active.");
		return 0;
	}

	new memberid = GetPlayerGangMemberID(playerid);

	gMember[gangid][memberid][e_gmID] = INVALID_PLAYER_ID;
	GangMember_SetActiveStatus(gangid, memberid, false);

	if (Gang_GetOnlineCount(gangid) == 0) {
		Gang_Unload(gangid);
	} else {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][74], ReturnPlayerName(playerid));
		Gang_SendMessage(gangid, string, COLOR_GANG);
	}

	return 1;
}

/*
	Gang sharing experience
*/

stock Gang_GiveMemberXP(playerid)
{
	new xp_amount = gPlayerGiveXP[playerid];
	if (xp_amount == 0) {
		return;
	}
	
	gPlayerGiveXP[playerid] = 0;
	
	GivePlayerXP(playerid, xp_amount, 0);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][79], xp_amount);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
}


stock Gang_GiveXpFromPlayer(giverid, xpamount)
{
	new gangid = GetPlayerGangID(giverid);
	if (gangid == INVALID_GANG_ID) {
		return 0;
	}
	
	new amount = xpamount / 4;
	
	gGang[gangid][e_gScore] += amount;

	new
		giveamount = amount / Gang_GetOnlineCount(gangid),
		playerid;
	
	foreach (new memberid : LoadedGangMembers[gangid]) {
		playerid = GangMember_GetID(gangid, memberid);

		if (playerid == giverid) {
			continue;
		}

		if (GetPlayerLevel(playerid) < GetMaxPlayerLevel()) {
			gPlayerGiveXP[playerid] += giveamount;
		}
	}

	return 1;
}

/*
	For publics
*/

Gang_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason, playerid
	if (killerid == INVALID_PLAYER_ID) {
		return 0;
	}

	new gangid = GetPlayerGangID(killerid);
	Gang_GiveXP(gangid, (Gang_GetLevel(gangid) + 1) * 20);
	return 1;
}

/*
	On player kill
*/

stock Gang_PlayerKill(gangid, killerid, victimid)
{
	if (GetPlayerGangID(killerid) == GetPlayerGangID(victimid)) {
		return 1;
	}
	
	if (victimid != INVALID_PLAYER_ID) {
		gGang[gangid][e_gKills]++;
	}

	return 0;
}

/*
	Gang Message
*/

stock Gang_SendMessage(gangid, message[], color = COLOR_GANG)
{
	foreach (new memberid : LoadedGangMembers[gangid]) {
		SendClientMessage(gMember[gangid][memberid][e_gmID], color, message);
	}
}

/*
	Gang House
*/

stock Gang_SetHouseID(gangid, houseid)
{
	gGang[gangid][e_gHouse] = houseid;
}

stock Gang_GetHouseID(gangid)
{
	return gGang[gangid][e_gHouse];
}

/*
	Gang_IsPlayerLeader
*/

stock Gang_IsPlayerLeader(playerid)
{
	new
		gangid = GetPlayerGangID(playerid),
		memberid = GetPlayerGangMemberID(playerid);
	
	return gMember[gangid][memberid][e_gmRank] == GangMemberLeader;
}

/*
	Gang_GetOnlineCount
*/

stock Gang_GetOnlineCount(gangid)
{
	return Iter_Count(LoadedGangMembers[gangid]);
}

/*
	Gang Kills
*/

stock Gang_GetKills(gangid)
{
	return gGang[gangid][e_gKills];
}

stock Gang_SetKills(gangid, amount)
{
	gGang[gangid][e_gKills] = amount;
}

/*
	Gang Score
*/

stock Gang_GetScore(gangid)
{
	return gGang[gangid][e_gScore];
}

stock Gang_SetScore(gangid, amount)
{
	gGang[gangid][e_gScore] = amount;
}

/*
	Gang Level
*/

stock Gang_GetLevel(gangid)
{
	return gGang[gangid][e_gLevel];
}

stock Gang_SetLevel(gangid, level)
{
	gGang[gangid][e_gLevel] = level;
}

stock Gang_GetXP(gangid)
{
	return gGang[gangid][e_gXP];
}

stock Gang_SetXP(gangid, xp)
{
	gGang[gangid][e_gXP] = xp;
}

stock Gang_GiveXP(gangid, amount)
{
	gGang[gangid][e_gXP] += amount;
}

/*
	Costs for color and create
*/

stock Gang_GetCreateCost()
{
	return gCreateCost;
}

stock Gang_GetColorCost()
{
	return gColorCost;
}

/*
	Money
*/

stock Gang_GiveMoney(gangid, &amount)
{
	if (gGang[gangid][e_gMoney] + amount > MAX_GANG_BANK) {
		amount = MAX_GANG_BANK - gGang[gangid][e_gMoney];
	}

	gGang[gangid][e_gMoney] += amount;
}

stock Gang_TakeMoney(gangid, &amount)
{
	if (gGang[gangid][e_gMoney] - amount < 0) {
		amount = gGang[gangid][e_gMoney];
	}

	gGang[gangid][e_gMoney] -= amount;
}

stock Gang_GetMoney(gangid)
{
	return gGang[gangid][e_gMoney];
}

/*
	Color
*/

stock Gang_SetColor(gangid, color)
{
	gGang[gangid][e_gColor] = color;

	foreach (new memberid : LoadedGangMembers[gangid]) {
		SetPlayerColor(gMember[gangid][memberid][e_gmID], color);
	}
}

stock Gang_GetColor(gangid)
{
	return gGang[gangid][e_gColor];
}

/*
	Name
*/

stock Gang_GetName(gangid, name[], const size = sizeof(name))
{
	strmid(name, gGang[gangid][e_gName], 0, strlen(gGang[gangid][e_gName]), size);
}

stock Gang_ReturnName(gangid)
{
	return gGang[gangid][e_gName];
}

/*
	Motd
*/

stock Gang_GetMotd(gangid, text[], const size = sizeof(text))
{
	strmid(text, gGang[gangid][e_gMotd], 0, strlen(gGang[gangid][e_gMotd]), size);
}

stock Gang_SetMotd(gangid, text[])
{
	strmid(gGang[gangid][e_gMotd], text, 0, strlen(text), MAX_GANG_MOTD);
}

stock Gang_ReturnMotd(gangid)
{
	return gGang[gangid][e_gMotd];
}

/*
	Get ID
*/

stock Gang_GetID(gangname[])
{
	foreach (new gangid : LoadedGangs) {
		if (!strcmp(gangname, gGang[gangid][e_gName], false)) {
			return gangid;
		}
	}

	return INVALID_GANG_ID;
}

/*
	Active status
*/

stock Gang_IsActive(gangid)
{
	return Iter_Contains(LoadedGangs, gangid);
}

stock Gang_SetActiveStatus(gangid, bool:status)
{
	if (status) {
		Iter_Add(LoadedGangs, gangid);
	} else {
		Iter_Remove(LoadedGangs, gangid);
	}
}

stock Gang_GetFreeSlot()
{
	return Iter_Free(LoadedGangs);
}

/*
	Member ID
*/

stock GangMember_GetID(gangid, memberid)
{
	return gMember[gangid][memberid][e_gmID];
}

stock GangMember_SetID(gangid, memberid, id)
{
	gMember[gangid][memberid][e_gmID] = id;
}

/*
	Active member status
*/

stock GangMember_IsActive(gangid, memberid)
{
	return Iter_Contains(LoadedGangMembers[gangid], memberid);
}

stock GangMember_SetActiveStatus(gangid, memberid, bool:status)
{
	if (status) {
		Iter_Add(LoadedGangMembers[gangid], memberid);
	} else {
		Iter_Remove(LoadedGangMembers[gangid], memberid);
	}
}

stock GangMember_GetFreeSlot(gangid)
{
	return Iter_Free(LoadedGangMembers[gangid]);
}

/*
	Gang Member Name
*/

stock GangMember_SetName(gangid, memberid, name[])
{
	strmid(gMember[gangid][memberid][e_gmName], name, 0, strlen(name), MAX_PLAYER_NAME);
}

stock GangMember_GetName(gangid, memberid, name[], const size = sizeof(name))
{
	strmid(name, gMember[gangid][memberid][e_gmName], 0, strlen(gMember[gangid][memberid][e_gmName]), size);
}

/*
	Gang Member Exists
*/

stock GangMember_IsExists(gangid, memberid)
{
	return _:gMember[gangid][memberid][e_gmIsExists];
}
