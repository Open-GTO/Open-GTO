//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
// Updated in 02.09.2011 by ZiGGi

#if defined _player_included
	#endinput
#endif

#define _player_included
#pragma library player

new PlayerStartMoney = PLAYER_START_MONEY;

stock player_LoadConfig(file_config)
{
	ini_getString(file_config, "Player_DB", PlayerDB);
	ini_getInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	new s_buf[MAX_STRING];
	ini_getString(file_config, "Player_Start_Weapon", s_buf);
	SetPSWFromDBString(s_buf);
}

stock player_SaveConfig(file_config)
{
	ini_setString(file_config, "Player_DB", PlayerDB);
	ini_setInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	ini_setString(file_config, "Player_Start_Weapon", CreatePSWDBString());
}

stock player_OnPlayerSpawn(playerid)
{
	if (player_IsMuted(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[1][14]);
		SetPlayerWantedLevel(playerid, 3);
	}
	
	if (player_IsJailed(playerid)) {
		JailPlayer(playerid, player_GetJailTime(playerid));
	}

	UpdatePlayerLevelTextDraws(playerid);
	UpdatePlayerWeaponTextDraws(playerid);

	SetPlayerColor(playerid, PlayerGangColour(playerid));
	player_SetSkin(playerid, player_GetSkin(playerid));
	SetMaxPlayerHealth(playerid);
	SetPlayerFightingStyle(playerid, pl_fights_GetPlayerStyleUsed(playerid));
	GivePlayerAllWeapon(playerid);
	weapon_SetSkills(playerid);
	PushHide(playerid);
	pl_anims_OnPlayerSpawn(playerid);
	return 1;
}

stock player_OnPlayerDisconnect(playerid, reason)
{
	// save
	player_Save(playerid);
	account_Save(playerid);
	
	DMPlayerDisconnect(playerid);
	StopAudioStreamForPlayer(playerid);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[1][18], oGetPlayerName(playerid), playerid);
	
	switch (reason) {
	    case 0: strcat(string, " (вылетел)", sizeof(string));
	    case 1: strcat(string, " (вышел)", sizeof(string));
	    case 2: strcat(string, " (кикнут)", sizeof(string));
	}

	SendClientMessageToAll(COLOUR_GREY, string);

	GangMemberLogout(playerid, player_GetGangID(playerid));
	DisablePlayerRaceCheckpoint(playerid);
	SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
}

stock player_OnPlayerConnect(playerid)
{
	pl_spawn_UpdateCamera(playerid);

    new plrIP[MAX_IP], string[MAX_STRING];
    GetPlayerIp(playerid, plrIP, sizeof(plrIP));
	SetPVarString(playerid, "IP", plrIP);
	if (!NameCharCheck( oGetPlayerName(playerid) ))
	{
		format(string, sizeof(string), lang_texts[9][12], ALLOWED_NICK_SYMBOLS_STR);
		SendClientMessage(playerid, COLOUR_RED, string);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[9][13]);
		KickPlayer(playerid, "“акой ник запрещЄн.");
	}
	SetPlayerColor(playerid, COLOUR_PLAYER);
	oBan_Check(playerid);
	
	format(string, sizeof(string), lang_texts[15][85], VERSION);
	GameTextForPlayer(playerid, string, 5500, 1);
	SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][86]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][87]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][88]);
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][90]);
	SendClientMessage(playerid, COLOUR_GREEN, "_________________________________________________________________________________");
}

stock player_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason
	pl_spawn_OnPlayerDeath(playerid, killerid, reason);

	GivePVarInt(playerid, "Deaths", 1);
	GivePVarInt(killerid, "Kills", 1);

	if (killerid == INVALID_PLAYER_ID) {
		return;
	}

	new stolencash = (oGetPlayerMoney(playerid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if (stolencash != 0) {
		oGivePlayerMoney(playerid, -stolencash, 1);
		oGivePlayerMoney(killerid, stolencash, 1);
	}
	
	if (player_GetGangID(killerid) != 0) {
		if (GangKill(player_GetGangID(killerid), killerid, playerid, reason) == 1) {
			return;
		}
	}

	// Give XP
	new xp_give_player = -((GetPlayerXP(playerid) / 100) * PLAYER_XP_DEATH_MINUS_PROC);
	new xp_give_killer = 0;
	new level_difference = GetPlayerLevel(playerid) - GetPlayerLevel(killerid);

	switch (level_difference) {
		case -MAX_LEVEL..-10: {
			xp_give_killer = GetPlayerLevel(playerid) * PLAYER_XP_KILL_TARIF;
		}
		case -9..-1: {
			xp_give_killer = ( ((GetPlayerLevel(playerid)+1) * (GetPlayerLevel(playerid)+1)) / (-level_difference) ) * PLAYER_XP_KILL_TARIF;
		}
		default: {
			xp_give_killer = ( (GetPlayerLevel(playerid)+1) * 2 ) * ( (GetPlayerLevel(playerid)+1) * 2 ) * PLAYER_XP_KILL_TARIF;
		}
	}

	if (GetPlayerXP(killerid) + xp_give_killer > MAX_XP) {
		GivePlayerXP(killerid, MAX_XP - GetPlayerXP(killerid), 1);
	} else {
		GivePlayerXP(killerid, xp_give_killer, 1);
	}

	GivePlayerXP(playerid, xp_give_player, 1);
}

stock player_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	// spawn system
	pl_spawn_OnPlayerRequestClass(playerid, classid);

	// show login dialog
	if (!player_IsLogin(playerid)) {
		account_ShowDialog(playerid);
	}
	return 1;
}

stock player_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	pl_maptp_OnPlayerClickMap(playerid, fX, fY, fZ);
	return 1;
}

stock player_OnLogin(playerid)
{
	// spawn player
	TogglePlayerSpectating(playerid, 0);

	// reset data
	player_SetQuestID(playerid, INVALID_QUEST_ID);

	// send message
	new string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (IsPlayerRconAdmin(playerid)) {
		format(string, sizeof(string), lang_texts[1][39], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][25]);
	} else if (IsPlayerAdm(playerid)) {
		format(string, sizeof(string), lang_texts[1][40], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][26]);
	} else if (IsPlayerMod(playerid)) {
		format(string, sizeof(string), lang_texts[1][41], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][27]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		format(string, sizeof(string), lang_texts[1][42], playername);
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	GameTextForPlayer(playerid, string, 2000, 1);

	foreach (new id : Player) {
		if (IsPlayerRconAdmin(playerid)) {
			continue;
		}

		format(string, sizeof(string), lang_texts[1][17], playername, playerid);
		SendClientMessage(id, COLOUR_WHITE, string);
	}
	GameMSG("player: %s(%d): logged in successfully.", playername, playerid);
}

stock player_SetDefaultData(playerid)
{
	oSetPlayerMoney(playerid, PlayerStartMoney);
	player_SetJailTime(playerid, -1);
	player_SetSpawnHouseID(playerid, SPAWN_HOUSE_NONE);
	player_SetSkin(playerid, PLAYER_DEFAULT_SKIN);

	SetPlayerLevel(playerid, FIRST_LEVEL);
	SetPlayerXP(playerid, level_xp_list[FIRST_LEVEL]);

	pl_fights_SetPlayerStyle(playerid, 4);
	pl_fights_SetPlayerStyleUsed(playerid, 4);

	oResetPlayerWeapons(playerid);
	weapon_ResetSkills(playerid);

	for (new i = 0; i < sizeof(PlayerStartWeapon); i++) {
		GivePlayerWeaponCheck(playerid, PlayerStartWeapon[i][psw_id], PlayerStartWeapon[i][psw_bull]);
	}
}

stock player_Sync(playerid)
{
	// если игрок мертв, то защиты не срабатывают
	if (!player_IsSpawned(playerid)) {
		return 0;
	}

	pt_weapon_Sync(playerid);
	pt_money_Sync(playerid);
	pt_ping_Check(playerid);
	pt_jetpack_Check(playerid);
	pt_health_Sync(playerid);
	pt_armour_Sync(playerid);

	JailPlayerCheck(playerid);
	return 1;
}

stock GetPlayerID(playername[])
{
	new name[MAX_PLAYER_NAME+1];
	foreach (Player, playerid)
	{
		GetPlayerName(playerid, name, sizeof(name));
		if (!strcmp(playername, name, true))
		{
			return playerid;
		}
	}
	return INVALID_PLAYER_ID;
}

stock oGetPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock player_GetKills(playerid) {
	return GetPVarInt(playerid, "Kills");
}

stock player_GetDeaths(playerid) {
	return GetPVarInt(playerid, "Deaths");
}

stock player_GetSkin(playerid) {
	return GetPVarInt(playerid, "SkinModel");
}

stock player_SetSkin(playerid, skinid) {
	SetPlayerSkin(playerid, skinid);
	SetPVarInt(playerid, "SkinModel", skinid);
}

stock player_GetSpecID(playerid) {
	return GetPVarInt(playerid, "SpecID");
}

stock player_SetSpecID(playerid, specid) {
	SetPVarInt(playerid, "SpecID", specid);
}

stock player_IsSpectating(playerid) {
	return GetPVarInt(playerid, "Spectating");
}

stock player_SetSpectating(playerid, isspec) {
	SetPVarInt(playerid, "Spectating", isspec);
}

stock player_GetSkydiveTime(playerid) {
	return GetPVarInt(playerid, "SkydiveTime");
}

stock player_SetSkydiveTime(playerid, skydivetime) {
	SetPVarInt(playerid, "SkydiveTime", skydivetime);
}

stock player_GetGangName(playerid) {
	new string[MAX_NAME];
	GetPVarString(playerid, "GangName", string, sizeof(string));
	return string;
}

stock player_SetGangName(playerid, gangname[]) {
	SetPVarString(playerid, "GangName", gangname);
}

stock player_GetGangID(playerid) {
	return GetPVarInt(playerid, "GangID");
}

stock player_SetGangID(playerid, gangid) {
	SetPVarInt(playerid, "GangID", gangid);
}

stock player_IsGodmod(playerid) {
	return GetPVarInt(playerid, "godmod");
}

stock player_SetGodmod(playerid, isgodmod) {
	SetPVarInt(playerid, "godmod", isgodmod);
}