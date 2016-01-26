//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
// Updated in 02.09.2011 by ZiGGi

#if defined _player_included
	#endinput
#endif

#define _player_included
#pragma library player

/*
	
*/

new PlayerStartMoney = PLAYER_START_MONEY;

stock player_LoadConfig(file_config)
{
	ini_getString(file_config, "Player_DB", db_player);
	ini_getInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	
	new
		weapons[START_PLAYER_WEAPON_SLOTS],
		bullets[START_PLAYER_WEAPON_SLOTS],
		s_buf[MAX_STRING];

	ini_getString(file_config, "Player_Start_Weapons", s_buf);
	sscanf(s_buf, "p</>a<i>[" #START_PLAYER_WEAPON_SLOTS "]", weapons);

	ini_getString(file_config, "Player_Start_Bullets", s_buf);
	sscanf(s_buf, "p</>a<i>[" #START_PLAYER_WEAPON_SLOTS "]", bullets);

	SetStartPlayerWeaponsFromArray(weapons);
	SetStartPlayerBulletsFromArray(bullets);
}

stock player_SaveConfig(file_config)
{
	ini_setString(file_config, "Player_DB", db_player);
	ini_setInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	ini_setString(file_config, "Player_Start_Weapons", CreatePlayerStartWeaponsString());
	ini_setString(file_config, "Player_Start_Bullets", CreatePlayerStartBulletsString());
}

stock player_OnPlayerSpawn(playerid)
{
	if (IsPlayerMuted(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(MUTED_HELP_MESSAGE));
	}

	UpdatePlayerLevelTextDraws(playerid);
	UpdatePlayerWeaponTextDraws(playerid);

	SetPlayerColor(playerid, GetPlayerGangColor(playerid));
	if (!Account_IsAfterRegistration(playerid)) {
		SetPlayerSkin(playerid, GetPlayerSkin(playerid));
	}
	SetPlayerMaxHealth(playerid);
	pl_fights_UpdatePlayerStyleUsed(playerid);
	GivePlayerOwnedWeapon(playerid);
	weapon_SetSkills(playerid);
	pl_money_td_ShowTextDraw(playerid);
	UpdatePlayerRandomSpawnID(playerid);
	return 1;
}

stock player_OnPlayerDisconnect(playerid, reason)
{
	// update params
	Account_SetPlayedTime(playerid, Account_GetCurrentPlayedTime(playerid));

	// save
	player_Save(playerid);
	Account_Save(playerid);

	StopAudioStreamForPlayer(playerid);

	new string[MAX_LANG_VALUE_STRING];
	format(string, sizeof(string), _(PLAYER_DISCONNECT), ReturnPlayerName(playerid), playerid);
	
	switch (reason) {
		case 0: {
			strcat(string, " (вылетел)", sizeof(string));
		}
		case 1: {
			strcat(string, " (вышел)", sizeof(string));
		}
		case 2: {
			strcat(string, " (кикнут)", sizeof(string));
		}
	}

	SendClientMessageToAll(COLOR_GREY, string);

	new gangid = GetPlayerGangID(playerid);

	new is_ok = Gang_MemberLogout(playerid, gangid);
	if (is_ok) {
		format(string, sizeof(string), _(GANG_MEMBER_LOGOUT), ReturnPlayerName(playerid));
		Gang_SendMessage(gangid, string, COLOR_GANG);
	}

	DisablePlayerRaceCheckpoint(playerid);
	SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
}

stock player_OnPlayerConnect(playerid)
{
	Chat_Clear(playerid);
	SendClientMessage(playerid, COLOR_WHITE, _(PLAYER_LOADING));

	Player_UpdateIP(playerid);
	
	if (!NameCharCheck( ReturnPlayerName(playerid) )) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(PLAYER_NICK_BAD_SYMBOLS), ALLOWED_NICK_SYMBOLS_STR);
		SendClientMessage(playerid, COLOR_RED, string);
		SendClientMessage(playerid, COLOR_RED, _(PLAYER_NICK_IS_IP));
		KickPlayer(playerid, "“акой ник запрещЄн.");
	}
	SetPlayerColor(playerid, COLOR_PLAYER);
	oBan_Check(playerid);
}

stock player_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason
	UpdatePlayerSpawnInfo(playerid);

	if (killerid == INVALID_PLAYER_ID || IsPlayersTeammates(playerid, killerid)) {
		return;
	}

	AddPlayerDeaths(playerid);
	AddPlayerKills(killerid);

	new stolencash = (GetPlayerMoney(playerid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if (stolencash > 0) {
		GivePlayerMoney(playerid, -stolencash);
		GivePlayerMoney(killerid, stolencash);
	}
	
	new killer_gang_id = GetPlayerGangID(killerid);
	if (killer_gang_id != INVALID_GANG_ID) {
		if (Gang_PlayerKill(killer_gang_id, killerid, playerid) == 1) {
			new string[MAX_STRING];
			GetPlayerName(killerid, string, sizeof(string));
			format(string, sizeof(string), _(GANG_KILL_TEAMMATE), string);
			Gang_SendMessage(killer_gang_id, string, COLOR_GANG);
			return;
		}

		GiveGangXP(killer_gang_id, (GetGangLevel(killer_gang_id) + 1) * 20);
	}

	// Give XP
	new xp_give_player = -GetPlayerXP(playerid) / 100 * PLAYER_XP_DEATH_MINUS_PROC;
	new xp_give_killer = (pow(GetPlayerLevel(playerid) * 4, 2) / (GetPlayerLevel(killerid) + 10) + 20) * PLAYER_XP_KILL_TARIF;

	new level_difference = GetPlayerLevel(playerid) - GetPlayerLevel(killerid);

	if (level_difference <= -10) {
		xp_give_player = 0;
	}

	GivePlayerXP(killerid, xp_give_killer);
	GivePlayerXP(playerid, xp_give_player);
}

stock player_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	pl_money_td_HideTextDraw(playerid);

	// spawn system
	Player_Spawn_OnPlayerRequestC(playerid, classid);

	// show login dialog
	if (!player_IsLogin(playerid)) {
		Account_ShowDialog(playerid);
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
	new interior, world, Float:spawn_pos[4];
	GetPlayerSpawnPos(playerid, spawn_pos[0], spawn_pos[1], spawn_pos[2], spawn_pos[3], interior, world);
	SetSpawnInfo(playerid, 0, GetPlayerSkin(playerid), spawn_pos[0], spawn_pos[1], spawn_pos[2], spawn_pos[3], 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, 0);

	// skin select
	if (Account_IsAfterRegistration(playerid)) {
		#if defined PLAYER_START_SKIN
			SetPlayerSkin(playerid, PLAYER_START_SKIN);
		#else
			new Float:camera_pos[3];
			camera_pos[2] = spawn_pos[2] + 1.0;

			GetCoordsBefore(spawn_pos[0], spawn_pos[1], spawn_pos[3], 2.0, camera_pos[0], camera_pos[1]);
			SetPlayerCameraPos(playerid, camera_pos[0], camera_pos[1], camera_pos[2]);
			SetPlayerCameraLookAt(playerid, spawn_pos[0], spawn_pos[1], spawn_pos[2] + 0.5);

			SkinSelect_Start(playerid, SkinSelect:RegisterSkin);
		#endif
	}

	// reset data
	player_SetQuestID(playerid, INVALID_QUEST_ID);

	// send message
	new
		string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	format(string, sizeof(string), _(ACCOUNT_LOGIN_MESSAGE_0), VERSION_STRING);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);
	SendClientMessage(playerid, COLOR_WHITE, _(ACCOUNT_LOGIN_MESSAGE_1));
	SendClientMessage(playerid, COLOR_WHITE, _(ACCOUNT_LOGIN_MESSAGE_2));
	SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_MESSAGE_3));

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_ROOT));
	} else if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_ADMIN));
	} else if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_MODER));
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_SUCCESS_LOGIN));
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	foreach (new id : Player) {
		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			continue;
		}

		format(string, sizeof(string), _(PLAYER_CONNECT), playername, playerid);
		SendClientMessage(id, COLOR_WHITE, string);
	}

	// admin
	if (IsPlayerAdmin(playerid)) {
		SetPlayerPrivilege(playerid, PlayerPrivilegeAdmin);
	}

	// gang
	SetPlayerGangID(playerid, INVALID_GANG_ID);
	SetPlayerGangMemberID(playerid, INVALID_MEMBER_ID);

	new gangname[MAX_GANG_NAME + 1];
	GetPlayerGangName(playerid, gangname);

	if (strlen(gangname) > 0) {
		new gangid = Gang_Load(gangname);

		if (gangid == INVALID_GANG_ID) {
			SetPlayerGangName(playerid, "");

			format(string, sizeof(string), _(GANG_MEMBER_LOGIN_REMOVED), gangname);
			SendClientMessage(playerid, COLOR_ORANGE, string);
		} else {
			new is_ok = Gang_MemberLogin(playerid, gangid);

			if (is_ok) {
				format(string, sizeof(string), _(GANG_MEMBER_LOGIN), playername);
				Gang_SendMessage(gangid, string, COLOR_GANG);

				format(string, sizeof(string), _(GANG_MEMBER_LOGIN_SELF), gangname, Gang_GetOnlineCount(gangid) - 1);
				SendClientMessage(playerid, COLOR_GANG, string);

				Gang_GetMotd(gangid, string);

				if (strlen(string) > 0) {
					format(string, sizeof(string), _(GANG_MEMBER_LOGIN_MOTD), string);
					SendClientMessage(playerid, COLOR_GANG, string);
				}
			}
		}
	}

	// hide widstrip
	widestrip_Hide(playerid);

	Log_Game("player: %s(%d): logged in successfully.", playername, playerid);
}

SkinSelectResponse:RegisterSkin(playerid, SS_Response:type, oldskin, newskin)
{
	if (type == SS_Response:SS_Response_Stop || type == SS_Response:SS_Response_Select) {
		SetPlayerSkin(playerid, newskin);
		SetCameraBehindPlayer(playerid);
	}

	if (type == SS_Response:SS_Response_Select) {
		SkinSelect_Stop(playerid);
	}
}

stock player_SetDefaultData(playerid)
{
	SetPlayerMoney(playerid, PlayerStartMoney);
	SetPlayerJailTime(playerid, -1);
	Player_SetSpawnType(playerid, SPAWN_TYPE_NONE);

#if defined PLAYER_START_SKIN
	SetPlayerSkin(playerid, PLAYER_START_SKIN);
#else
	SetPlayerSkin(playerid, SKINS_MINID);
#endif

	SetPlayerLevel(playerid, PLAYER_START_LEVEL, 0, 0);

	pl_fights_SetPlayerStyleUsed(playerid, FIGHT_STYLE_NORMAL);

	ResetPlayerWeapons(playerid);
	weapon_ResetSkills(playerid);

	for (new i = 0; i < sizeof(PlayerStartWeapon); i++) {
		GivePlayerWeapon(playerid, PlayerStartWeapon[i][pwid], PlayerStartWeapon[i][pbullets], true);
	}
}

stock player_Sync(playerid)
{
	// если игрок мертв, то защиты не срабатывают
	if (!Player_IsSpawned(playerid)) {
		return 0;
	}
	
	pt_ping_Check(playerid);
	pt_spac_Check(playerid);
	pt_health_Sync(playerid);
	pt_armour_Sync(playerid);
	pt_weapon_Check(playerid);
	return 1;
}

stock ReturnPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

static
	gPlayerDeaths[MAX_PLAYERS],
	gPlayerKills[MAX_PLAYERS],
	gPlayerSkin[MAX_PLAYERS];

/*
	Kills
*/

stock GetPlayerKills(playerid) {
	return gPlayerKills[playerid];
}

stock SetPlayerKills(playerid, amount) {
	gPlayerKills[playerid] = amount;
}

stock AddPlayerKills(playerid, amount = 1) {
	gPlayerKills[playerid] += amount;
}

/*
	Deaths
*/

stock GetPlayerDeaths(playerid) {
	return gPlayerDeaths[playerid];
}

stock SetPlayerDeaths(playerid, amount) {
	gPlayerDeaths[playerid] = amount;
}

stock AddPlayerDeaths(playerid, amount = 1) {
	gPlayerDeaths[playerid] += amount;
}

/*
	Kills-Deaths ratio
*/

stock Float:GetPlayerKillDeathRatio(playerid) {
	if (GetPlayerDeaths(playerid) != 0) {
		return float(GetPlayerKills(playerid)) / float(GetPlayerDeaths(playerid));
	}

	return 0.0;
}

/*
	Skin
*/

stock REDEF_GetPlayerSkin(playerid) {
	return gPlayerSkin[playerid];
}

stock REDEF_SetPlayerSkin(playerid, skinid)
{
	gPlayerSkin[playerid] = skinid;
	return ORIG_SetPlayerSkin(playerid, skinid);
}

/*
	Team
*/

stock REDEF_GetPlayerTeam(playerid)
{
	if (!IS_IN_RANGE(playerid, 0, MAX_PLAYERS - 1) || !IsPlayerConnected(playerid)) {
		return NO_TEAM;
	}

	return GetPVarInt(playerid, "team");
}

stock REDEF_SetPlayerTeam(playerid, team)
{
	if (!IS_IN_RANGE(playerid, 0, MAX_PLAYERS - 1)) {
		return 0;
	}

	SetPVarInt(playerid, "team", team);
	return 1;
}
