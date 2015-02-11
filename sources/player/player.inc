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
	}
	
	if (player_IsJailed(playerid)) {
		new jail_time = player_GetJailTime(playerid);
		JailPlayer(playerid, jail_time);
	}

	UpdatePlayerLevelTextDraws(playerid);
	UpdatePlayerWeaponTextDraws(playerid);

	SetPlayerColor(playerid, PlayerGangColour(playerid));
	player_SetSkin(playerid, player_GetSkin(playerid));
	SetPlayerMaxHealth(playerid);
	SetPlayerFightingStyle(playerid, pl_fights_GetPlayerStyleUsed(playerid));
	GivePlayerAllWeapon(playerid);
	weapon_SetSkills(playerid);
	PushHide(playerid);
	pl_anims_OnPlayerSpawn(playerid);
	pl_textdraw_Show(playerid);
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

	chat_Clean(playerid);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[9][35]);

    new plrIP[MAX_IP], string[MAX_STRING];
    GetPlayerIp(playerid, plrIP, sizeof(plrIP));
	SetPVarString(playerid, "IP", plrIP);
	if (!NameCharCheck( oGetPlayerName(playerid) )) {
		format(string, sizeof(string), lang_texts[9][12], ALLOWED_NICK_SYMBOLS_STR);
		SendClientMessage(playerid, COLOUR_RED, string);
		SendClientMessage(playerid, COLOUR_RED, lang_texts[9][13]);
		KickPlayer(playerid, "Такой ник запрещён.");
	}
	SetPlayerColor(playerid, COLOUR_PLAYER);
	oBan_Check(playerid);
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
	if (stolencash > 0) {
		oGivePlayerMoney(playerid, -stolencash, 1);
		oGivePlayerMoney(killerid, stolencash, 1);
	}
	
	if (player_GetGangID(killerid) != 0) {
		if (GangKill(player_GetGangID(killerid), killerid, playerid, reason) == 1) {
			return;
		}
	}

	// Give XP
	new xp_give_player = -GetPlayerXP(playerid) / 100 * PLAYER_XP_DEATH_MINUS_PROC;
	new xp_give_killer = (pow(GetPlayerLevel(playerid) * 4, 2) / (GetPlayerLevel(killerid) + 10) + 20) * PLAYER_XP_KILL_TARIF;

	new level_difference = GetPlayerLevel(playerid) - GetPlayerLevel(killerid);

	if (level_difference <= -10) {
		xp_give_player = 0;
	}

	GivePlayerXP(killerid, xp_give_killer, 1);
	GivePlayerXP(playerid, xp_give_player, 1);
}

stock player_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	pl_textdraw_Hide(playerid);

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
	new Float:spawn_pos[4];
	pl_spawn_GetSpawnPos(playerid, spawn_pos[0], spawn_pos[1], spawn_pos[2], spawn_pos[3]);
	SetSpawnInfo(playerid, 0, player_GetSkin(playerid), spawn_pos[0], spawn_pos[1], spawn_pos[2], spawn_pos[3], 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, 0);

	// reset data
	player_SetQuestID(playerid, INVALID_QUEST_ID);

	// send message
	new string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	format(string, sizeof(string), lang_texts[15][86], VERSION);
	SendClientMessage(playerid, COLOUR_LIGHTRED, string);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][87]);
	SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][88]);
	SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][89]);

	if (IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][25]);
	} else if (IsPlayerAdm(playerid)) {
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][26]);
	} else if (IsPlayerMod(playerid)) {
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][27]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[1][2]);
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	foreach (new id : Player) {
		if (IsPlayerRconAdmin(playerid)) {
			continue;
		}

		format(string, sizeof(string), lang_texts[1][17], playername, playerid);
		SendClientMessage(id, COLOUR_WHITE, string);
	}

	// admin
	if (IsPlayerAdmin(playerid)) {
		player_SetStatus(playerid, STATUS_LEVEL_RCON);
	}

	// gang
	new gangname[MAX_NAME];
	set(gangname, player_GetGangName(playerid));

	if (strlen(gangname) > 0) {
		new tmpname[MAX_STRING];
		format(tmpname, sizeof(tmpname), "%s%s"GTO_FILES_FORMAT, GangDB, gangname);

		if (!ini_fileExist(tmpname)) {
			player_SetGangName(playerid, nullstr);
			player_SetGangID(playerid, 0);

			format(tmpname, sizeof(tmpname), "Банда '%s' была расформирована.", gangname);
			SendClientMessage(playerid, COLOUR_ORANGE, tmpname);
		} else {
			GangMemberLogin(playerid, GangLoad(gangname));
		}
	} else {
		player_SetGangID(playerid, 0);
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
	SetPlayerXP(playerid, 0);

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