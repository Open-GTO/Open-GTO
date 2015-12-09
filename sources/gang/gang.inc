//
// Created:		14.09.06
// Aurthor:		Iain Gilbert
// Last modify 06.02.2011 by ZiGGi
//

#if defined _gang_included
  #endinput
#endif

#define _gang_included
#pragma library gang


new Gang_Create_Cost = GANG_CREATE_COST,
	Gang_Color_Cost = GANG_COLOR_COST; 

enum MemberInfo {
	member_name[MAX_PLAYER_NAME+1],
	member_playerid
}

new GangMemberInfo[MAX_GANGS][MAX_GANG_SIZE][MemberInfo];

enum GangInfo {
	bool:gang_active, //bool
	gang_name[MAX_NAME],
	gang_motd[MAX_STRING],
	gang_colour,
	gang_kills,
	gang_score,
	gang_xp,
	gang_level,
	gang_money,
	gang_house,
}
new Gangs[MAX_GANGS][GangInfo];

stock gang_LoadConfig(file_config)
{
	ini_getString(file_config, "Gang_DB", db_gang);
	ini_getInteger(file_config, "Gang_Create_Cost", Gang_Create_Cost);
	ini_getInteger(file_config, "Gang_Color_Cost", Gang_Color_Cost);
}

stock gang_SaveConfig(file_config)
{
	ini_setString(file_config, "Gang_DB", db_gang);
	ini_setInteger(file_config, "Gang_Create_Cost", Gang_Create_Cost);
	ini_setInteger(file_config, "Gang_Color_Cost", Gang_Color_Cost);
}

stock FindGangFreeSlot()
{
	for (new id = 1; id < MAX_GANGS; id++)
	{
		if (Gangs[id][gang_active] != true)
		{
			return id;
		}
	}
	return 0;
}

stock GangLoad(gangname[])
{
	for (new id = 1; id < MAX_GANGS; id++)
	{
		if (Gangs[id][gang_active] == true && !strcmp(gangname, Gangs[id][gang_name], false))
		{
			return id; 
		}
	}

	new gangid = FindGangFreeSlot();
	if (gangid == 0)
	{
		return Log_Game("ERROR: Свободный слот для банды не найден");
	}
	Gangs[gangid][gang_active] = true;
	gang_load_db_ini(gangid, gangname);
	return gangid;
}

stock gang_load_db_ini(gangid, gangname[])
{
	new filename_gang[MAX_STRING];
	format(filename_gang, sizeof(filename_gang), "%s%s"DATA_FILES_FORMAT, db_gang, gangname);
	if (!ini_fileExist(filename_gang)) return;
	new file_gang = ini_openFile(filename_gang);
	ini_getString(file_gang, "Name", Gangs[gangid][gang_name], MAX_NAME);
	ini_getString(file_gang, "Motd", Gangs[gangid][gang_motd], MAX_STRING);
	ini_getInteger(file_gang, "Colour", Gangs[gangid][gang_colour]);
	ini_getInteger(file_gang, "Kills", Gangs[gangid][gang_kills]);
	ini_getInteger(file_gang, "Score", Gangs[gangid][gang_score]);
	ini_getInteger(file_gang, "XP", Gangs[gangid][gang_xp]);
	ini_getInteger(file_gang, "Level", Gangs[gangid][gang_level]);
	ini_getInteger(file_gang, "Money", Gangs[gangid][gang_money]);
	ini_getInteger(file_gang, "House", Gangs[gangid][gang_house]);
	for (new gangmember = 0, string[MAX_STRING]; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		
		format(string, sizeof(string), "Member%d", gangmember);
		ini_getString(file_gang, string, GangMemberInfo[gangid][gangmember][member_name], MAX_PLAYER_NAME+1);
	}
	ini_closeFile(file_gang);
	return;
}

stock gang_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason, playerid
	if (killerid == INVALID_PLAYER_ID) {
		return 0;
	}
	new gangid = player_GetGangID(killerid);
	GiveGangXP(gangid, (1+Gangs[gangid][gang_level]) * 20);
	return 1;
}

stock PlayerGangColour(playerid)
{
	new gangid = player_GetGangID(playerid);
	if (gangid != 0 && Gangs[gangid][gang_colour] != 0)
	{
		return Gangs[gangid][gang_colour];
	}
	return COLOR_PLAYER;
}

stock GangJoinMember(gangid, playerid)
{
	if (gangid == 0)
	{
		Log_Game("script_warning: invalid gang id.");
		return 0;
	}
	if (player_GetGangID(playerid) != 0)
	{
		Log_Game("script_warning: player already in a gang!");
		return 0;
	}
	if (GangOnlineCount(gangid) >= MAX_GANG_SIZE)
	{
		Log_Game("script_warning: Gang is already full.");
		return 0;
	}
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (strlen(GangMemberInfo[gangid][gangmember][member_name]) != 0)
		{
			continue;
		}

		set(GangMemberInfo[gangid][gangmember][member_name], ReturnPlayerName(playerid));
		GangMemberInfo[gangid][gangmember][member_playerid] = playerid;
		player_SetGangID(playerid, gangid);
		player_SetGangName(playerid, Gangs[gangid][gang_name]);
		GangMemberLogin(playerid, gangid);
		SetPlayerColor(playerid, PlayerGangColour(playerid));
		new houseid = gang_GetHouseID(gangid);
		if (houseid != -1)
		{
			player_SetSpawnHouseID(playerid, houseid);
		}
		return 1;
	}
	return 0;
}

stock GangRemoveMember(gangid, kickplayername[])
{
	new playerid = INVALID_PLAYER_ID, string[MAX_STRING];
	// если вышел лидер, то удаляем банду
	if (!strcmp(GangMemberInfo[gangid][0][member_name], kickplayername, true))
	{
		format(string, sizeof(string), "Вы удалили банду '%s'!", Gangs[gangid][gang_name]);
		Dialog_Message(GangMemberInfo[gangid][0][member_playerid], "Банда", string, "ОК");
		format(string, sizeof(string), "Лидер банды %s(%d) удалил банду '%s'.", kickplayername, GangMemberInfo[gangid][0][member_playerid], Gangs[gangid][gang_name]);
		for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
		{
			if (!IsPlayerConnected(GangMemberInfo[gangid][gangmember][member_playerid]))
			{
				continue;
			}
			playerid = GangMemberInfo[gangid][gangmember][member_playerid];
			player_SetGangID(playerid, 0);
			player_SetGangName(playerid, "");
			if (player_GetSpawnHouseID(playerid) == SPAWN_HOUSE_GANG)
			{
				player_SetSpawnHouseID(playerid, SPAWN_HOUSE_NONE);
			}
			GangMemberInfo[gangid][gangmember][member_name][0] = '\0';
			SendClientMessage(playerid, COLOR_RED, string);
			SetPlayerColor(playerid, PlayerGangColour(playerid));
			GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		}
		format(string, sizeof(string), "%s%s"DATA_FILES_FORMAT, db_gang, Gangs[gangid][gang_name]);
		ini_fileRemove(string);
		Gangs[gangid][gang_active] = false;
		return 1;
	}
	playerid = INVALID_PLAYER_ID;
	for (new gangmember = 1; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (strcmp(GangMemberInfo[gangid][gangmember][member_name], kickplayername, true))
		{
			continue;
		}

		playerid = GangMemberInfo[gangid][gangmember][member_playerid];
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		GangMemberInfo[gangid][gangmember][member_name][0] = '\0';
		if (player_GetSpawnHouseID(playerid) == SPAWN_HOUSE_GANG)
		{
			player_SetSpawnHouseID(playerid, SPAWN_HOUSE_NONE);
		}
		format(string, sizeof(string), lang_texts[6][70], kickplayername);
		SendGangMessage(gangid, string, COLOR_GANG);
		Log_Game("%s has been removed from gang '%s'", kickplayername, Gangs[gangid][gang_name]);
		if (playerid != INVALID_PLAYER_ID)
		{
			format(string, sizeof(string), lang_texts[6][71], Gangs[gangid][gang_name]);
			SendClientMessage(playerid, COLOR_GANG, string);
			player_SetGangID(playerid, 0);
			player_SetGangName(playerid, "");
		}
		SetPlayerColor(playerid, PlayerGangColour(playerid));
		if (GangOnlineCount(gangid) == 0)
		{
			GangUnload(gangid);
		}
		return 1;
	}
	return 0;
}

stock GangCreate(leaderid, gangname[], gangcolour)
{
	if (!player_IsLogin(leaderid)) return 0;
	new filename_gang[MAX_STRING];
	format(filename_gang, sizeof(filename_gang), "%s%s"DATA_FILES_FORMAT, db_gang, gangname);
	if (ini_fileExist(filename_gang))
	{
		SendClientMessage(leaderid, COLOR_RED, lang_texts[6][73]);
		return 0;
	}
	new gangid = FindGangFreeSlot();
	if (gangid == 0)
	{
		return Log_Game("[ERROR] Свободный слот для банды не найден");
	}
	Gangs[gangid][gang_active] = true;
	set(Gangs[gangid][gang_name], gangname);
	Gangs[gangid][gang_colour] = gangcolour;
	Gangs[gangid][gang_kills] = 0;
	Gangs[gangid][gang_score] = 0;
	Gangs[gangid][gang_level] = 0;
	Gangs[gangid][gang_xp] = 0;
	Gangs[gangid][gang_money] = 0;
	Gangs[gangid][gang_house] = player_GetSpawnHouseID(leaderid);
	Gangs[gangid][gang_motd][0] = '\0';
	for (new gangmember =0 ; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
		GangMemberInfo[gangid][gangmember][member_name][0] = '\0';
	}
	set(GangMemberInfo[gangid][0][member_name], ReturnPlayerName(leaderid));
	GangMemberInfo[gangid][0][member_playerid] = leaderid;
	player_SetGangName(leaderid, gangname);
	player_SetGangID(leaderid, gangid);
	SetPlayerColor(leaderid, PlayerGangColour(leaderid));
	GangSave(gangid);
	return gangid;
}

stock gang_SaveAll()
{
	for (new i = 1; i < MAX_GANGS; i++)
	{
		if (Gangs[i][gang_active])
		{
			GangSave(i);
			if (GangOnlineCount(i) == 0)
			{
				GangUnload(i);
			}
		}
	}
}

stock GangMemberLogout(playerid, gangid)
{
	SetPVarInt(playerid, "GangInvite", 0);
	if (gangid == 0)
	{
		return;
	}
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (GangMemberInfo[gangid][gangmember][member_playerid] == playerid)
		{
			GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
			break;
		}
	}
	if (GangOnlineCount(gangid) == 0)
	{
		GangUnload(gangid);
	}
	else
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][74], ReturnPlayerName(playerid));
		SendGangMessage(gangid, string, COLOR_GANG);
	}
	return;
}

stock GangSave(gangid)
{
	if (gangid == 0 || !Gangs[gangid][gang_active] || !strlen(Gangs[gangid][gang_name]))
	{
		return 0;
	}
	gang_save_db_ini(gangid);
	return 1;
}

stock gang_save_db_ini(gangid)
{
	new filename_gang[MAX_STRING];
	format(filename_gang, sizeof(filename_gang), "%s%s"DATA_FILES_FORMAT, db_gang, Gangs[gangid][gang_name]);
	new file_gang = (!ini_fileExist(filename_gang)) ? ini_createFile(filename_gang) : ini_openFile(filename_gang);
	ini_setString(file_gang, "Name", Gangs[gangid][gang_name]);
	ini_setString(file_gang, "Motd", Gangs[gangid][gang_motd]);
	ini_setInteger(file_gang, "Colour", Gangs[gangid][gang_colour]);
	ini_setInteger(file_gang, "Kills", Gangs[gangid][gang_kills]);
	ini_setInteger(file_gang, "Score", Gangs[gangid][gang_score]);
	ini_setInteger(file_gang, "XP", Gangs[gangid][gang_xp]);
	ini_setInteger(file_gang, "Level", Gangs[gangid][gang_level]);
	ini_setInteger(file_gang, "Money", Gangs[gangid][gang_money]);
	ini_setInteger(file_gang, "House", Gangs[gangid][gang_house]);
	for (new gangmember = 0, string[MAX_STRING]; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		format(string, sizeof(string), "Member%d", gangmember);
		ini_setString(file_gang, string, GangMemberInfo[gangid][gangmember][member_name]);
	}
	ini_closeFile(file_gang);
}

stock GangUnload(gangid)
{
	GangSave(gangid);
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		GangMemberInfo[gangid][gangmember][member_playerid] = INVALID_PLAYER_ID;
	}
	Gangs[gangid][gang_active] = false;
}

stock GangKill(gangid, killerid, victimid, reason)
{
	#pragma unused reason
	if (player_GetGangID(killerid) == player_GetGangID(victimid)) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][76], ReturnPlayerName(killerid));
		SendGangMessage(player_GetGangID(killerid), string, COLOR_GANG);
		return 1;
	}
	
	if (victimid != INVALID_PLAYER_ID) {
		Gangs[gangid][gang_kills]++;
	}
	return 0;
}

stock GangMemberLogin(playerid, gangid)
{
	if (gangid == 0) return 0;
	new pl_name[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, pl_name, sizeof(pl_name));
	for (new gangmember = 0, string[MAX_STRING]; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (strcmp(GangMemberInfo[gangid][gangmember][member_name], pl_name, false))
		{
			continue;
		}
		
		format(string, sizeof(string), lang_texts[6][77], pl_name);
		SendGangMessage(gangid, string, COLOR_GANG);
		GangMemberInfo[gangid][gangmember][member_playerid] = playerid;
		player_SetGangID(playerid, gangid);
		player_SetGangName(playerid, Gangs[gangid][gang_name]);
		format(string, sizeof(string), lang_texts[6][78], Gangs[gangid][gang_name], GangOnlineCount(gangid)-1);
		SendClientMessage(playerid, COLOR_GANG, string);
		if (strlen(Gangs[gangid][gang_motd]) > 0)
		{
			format(string, sizeof(string), lang_texts[6][80], Gangs[gangid][gang_motd]);
			SendClientMessage(playerid, COLOR_GANG, string);
		}
		SetPVarInt(playerid, "GangInvite", 0);
		SetPlayerColor(playerid, Gangs[gangid][gang_colour]);
		return 1;
	}
	return 0;
}

stock SendGangMessage(gangid, message[], colour)
{
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (gang_IsPlayerConnect(gangid, gangmember))
		{
			SendClientMessage(GangMemberInfo[gangid][gangmember][member_playerid], colour, message);
		}
	}
}

stock GangOnlineCount(gangid)
{
	new memberscount = 0;
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (gang_IsPlayerConnect(gangid, gangmember))
		{
			memberscount++;
		}
	}
	return memberscount;
}

stock GangGiveXP(gangid, xpamount, giverid)
{
	Gangs[gangid][gang_score] += xpamount;
	new giveamount = xpamount / GangOnlineCount(gangid),
		playerid;
	
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++) {
		playerid = GangMemberInfo[gangid][gangmember][member_playerid];
		
		if (!gang_IsPlayerConnect(gangid, gangmember) || giverid == playerid) {
			continue;
		}
		
		if (GetPlayerLevel(playerid) < GetMaxPlayerLevel()) {
			GivePVarInt(playerid, "gang_xp_from_players", giveamount);
		}
	}
}

stock Gang_GiveXp(playerid)
{
	new xp_amount = GetPVarInt(playerid, "gang_xp_from_players");
	if (xp_amount == 0) {
		return;
	}
	
	DeletePVar(playerid, "gang_xp_from_players");
	
	GivePlayerXP(playerid, xp_amount, 0);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][79], xp_amount);
	SendClientMessage(playerid, COLOR_XP_GOOD, string);
}

stock SetGangColour(gangid, colour)
{
	Gangs[gangid][gang_colour] = colour;
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (gang_IsPlayerConnect(gangid, gangmember))
		{
			SetPlayerColor(GangMemberInfo[gangid][gangmember][member_playerid], colour);
		}
	}
}

stock GangGivePlayerXP(playerid, xpamount)
{
	if (player_GetGangID(playerid) == 0 || xpamount <= 100) return;
	GangGiveXP(player_GetGangID(playerid), xpamount / 4, playerid);
	return;
}

stock gang_RenameMember(gangid, old_name[MAX_PLAYER_NAME+1], new_name[MAX_PLAYER_NAME+1])
{
	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++)
	{
		if (!strcmp(GangMemberInfo[gangid][gangmember][member_name], old_name, true))
		{
			set(GangMemberInfo[gangid][gangmember][member_name], new_name);
			return 1;
		}
	}
	return 0;
}


stock gang_IsLeader(playerid)
{
	new gangid = player_GetGangID(playerid);
	if (gangid != 0 && playerid == GangMemberInfo[gangid][0][member_playerid])
	{
		return 1;
	}
	return 0;
}

stock gang_SetHouse(gangid, houseid)
{
	Gangs[gangid][gang_house] = houseid;
}

stock gang_GetHouseID(gangid)
{
	return Gangs[gangid][gang_house];
}

stock gang_IsPlayerConnect(gangid, gangmember)
{
	return GangMemberInfo[gangid][gangmember][member_playerid] == INVALID_PLAYER_ID ? 0 : 1;
}

stock GiveGangMoney(gangid, &amount)
{
	if (Gangs[gangid][gang_money] + amount > MAX_GANG_BANK) {
		amount = MAX_GANG_BANK - Gangs[gangid][gang_money];
	}
	Gangs[gangid][gang_money] += amount;
}

stock TakeGangMoney(gangid, &amount)
{
	if (Gangs[gangid][gang_money] - amount < 0) {
		amount = Gangs[gangid][gang_money];
	}
	Gangs[gangid][gang_money] -= amount;
}

stock GetGangMoney(gangid)
{
	return Gangs[gangid][gang_money];
}
