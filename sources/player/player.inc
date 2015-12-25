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
	ini_getString(file_config, "Player_DB", db_player);
	ini_getInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	new s_buf[MAX_STRING];
	ini_getString(file_config, "Player_Start_Weapon", s_buf);
	SetPSWFromDBString(s_buf);
}

stock player_SaveConfig(file_config)
{
	ini_setString(file_config, "Player_DB", db_player);
	ini_setInteger(file_config, "Player_Start_Money", PlayerStartMoney);
	ini_setString(file_config, "Player_Start_Weapon", CreatePSWDBString());
}

stock player_OnPlayerSpawn(playerid)
{
	if (player_IsMuted(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(MUTED_HELP_MESSAGE));
	}
	
	if (player_IsJailed(playerid)) {
		new jail_time = player_GetJailTime(playerid);
		JailPlayer(playerid, jail_time);
	}

	UpdatePlayerLevelTextDraws(playerid);
	UpdatePlayerWeaponTextDraws(playerid);

	SetPlayerColor(playerid, GetPlayerGangColor(playerid));
	if (!account_IsAfterRegister(playerid)) {
		SetPlayerSkin(playerid, Player_GetPlayerSkin(playerid));
	}
	SetPlayerMaxHealth(playerid);
	pl_fights_UpdatePlayerStyleUsed(playerid);
	GivePlayerOwnedWeapon(playerid);
	weapon_SetSkills(playerid);
	PushHide(playerid);
	pl_money_td_ShowTextDraw(playerid);
	UpdatePlayerRandomSpawnID(playerid);
	return 1;
}

stock player_OnPlayerDisconnect(playerid, reason)
{
	// save
	player_Save(playerid);
	account_Save(playerid);

	StopAudioStreamForPlayer(playerid);

	new string[MAX_STRING];
	format(string, sizeof(string), _(PLAYER_DISCONNECT), ReturnPlayerName(playerid), playerid);
	
	switch (reason) {
		case 0: strcat(string, " (вылетел)", sizeof(string));
		case 1: strcat(string, " (вышел)", sizeof(string));
		case 2: strcat(string, " (кикнут)", sizeof(string));
	}

	SendClientMessageToAll(COLOR_GREY, string);

	Gang_MemberLogout(playerid, GetPlayerGangID(playerid));
	DisablePlayerRaceCheckpoint(playerid);
	SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
}

stock player_OnPlayerConnect(playerid)
{
	Chat_Clear(playerid);
	SendClientMessage(playerid, COLOR_WHITE, lang_texts[9][35]);

	new plrIP[MAX_IP], string[MAX_STRING];
	GetPlayerIp(playerid, plrIP, sizeof(plrIP));
	SetPVarString(playerid, "IP", plrIP);
	if (!NameCharCheck( ReturnPlayerName(playerid) )) {
		format(string, sizeof(string), lang_texts[9][12], ALLOWED_NICK_SYMBOLS_STR);
		SendClientMessage(playerid, COLOR_RED, string);
		SendClientMessage(playerid, COLOR_RED, lang_texts[9][13]);
		KickPlayer(playerid, "Такой ник запрещён.");
	}
	SetPlayerColor(playerid, COLOR_PLAYER);
	oBan_Check(playerid);
}

stock player_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason
	UpdatePlayerSpawnInfo(playerid);

	if (killerid == INVALID_PLAYER_ID) {
		return;
	}

	AddPlayerDeaths(playerid);
	AddPlayerKills(killerid);

	new stolencash = (GetPlayerMoney(playerid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if (stolencash > 0) {
		GivePlayerMoney(playerid, -stolencash, 1);
		GivePlayerMoney(killerid, stolencash, 1);
	}
	
	new killer_gang_id = GetPlayerGangID(killerid);
	if (killer_gang_id != INVALID_GANG_ID) {
		if (Gang_PlayerKill(killer_gang_id, killerid, playerid) == 1) {
			new string[MAX_STRING];
			GetPlayerName(killerid, string, sizeof(string));
			format(string, sizeof(string), lang_texts[6][76], string);
			Gang_SendMessage(killer_gang_id, string, COLOR_GANG);
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
	pl_money_td_HideTextDraw(playerid);

	// spawn system
	Player_Spawn_OnPlayerRequestC(playerid, classid);

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
	new interior, world, Float:spawn_pos[4];
	GetPlayerSpawnPos(playerid, spawn_pos[0], spawn_pos[1], spawn_pos[2], spawn_pos[3], interior, world);
	SetSpawnInfo(playerid, 0, Player_GetPlayerSkin(playerid), spawn_pos[0], spawn_pos[1], spawn_pos[2], spawn_pos[3], 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, 0);

	// skin select
	if (account_IsAfterRegister(playerid)) {
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

	if (IsPlayerRconAdmin(playerid)) {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_ROOT));
	} else if (IsPlayerAdm(playerid)) {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_ADMIN));
	} else if (IsPlayerMod(playerid)) {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_LOGIN_MODER));
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		SendClientMessage(playerid, COLOR_GREEN, _(ACCOUNT_SUCCESS_LOGIN));
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	foreach (new id : Player) {
		if (IsPlayerRconAdmin(playerid)) {
			continue;
		}

		format(string, sizeof(string), _(PLAYER_CONNECT), playername, playerid);
		SendClientMessage(id, COLOR_WHITE, string);
	}

	// admin
	if (IsPlayerAdmin(playerid)) {
		player_SetStatus(playerid, STATUS_LEVEL_RCON);
	}

	// gang
	new gangname[MAX_GANG_NAME + 1];
	GetPlayerGangName(playerid, gangname);

	new gangid = Gang_Load(gangname);

	if (gangid == INVALID_GANG_ID) {
		SetPlayerGangName(playerid, "");
		SetPlayerGangID(playerid, INVALID_GANG_ID);

		format(string, sizeof(string), "Банда '%s' была расформирована.", gangname);
		SendClientMessage(playerid, COLOR_ORANGE, string);
	} else {
		new is_ok = Gang_MemberLogin(playerid, gangid);

		if (is_ok) {
			format(string, sizeof(string), lang_texts[6][77], playername);
			Gang_SendMessage(gangid, string, COLOR_GANG);

			format(string, sizeof(string), lang_texts[6][78], gangname, Gang_GetOnlineCount(gangid) - 1);
			SendClientMessage(playerid, COLOR_GANG, string);

			Gang_GetMotd(gangid, string);

			if (strlen(string) > 0) {
				format(string, sizeof(string), lang_texts[6][80], string);
				SendClientMessage(playerid, COLOR_GANG, string);
			}
		}
	}

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
	player_SetJailTime(playerid, -1);
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
		GivePlayerWeapon(playerid, PlayerStartWeapon[i][psw_id], PlayerStartWeapon[i][psw_bull], true);
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

	JailPlayerCheck(playerid);
	return 1;
}

stock GetPlayerID(playername[])
{
	new name[MAX_PLAYER_NAME+1];

	foreach (new playerid : Player) {
		GetPlayerName(playerid, name, sizeof(name));

		if (!strcmp(playername, name, true)) {
			return playerid;
		}
	}

	return INVALID_PLAYER_ID;
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

stock Player_GetPlayerSkin(playerid) {
	return gPlayerSkin[playerid];
}

stock Player_SetPlayerSkin(playerid, skinid)
{
	gPlayerSkin[playerid] = skinid;
	return SetPlayerSkin(playerid, skinid);
}
#if defined _ALS_SetPlayerSkin
	#undef SetPlayerSkin
#else
	#define _ALS_SetPlayerSkin
#endif
 
#define SetPlayerSkin Player_SetPlayerSkin

/*
	Team
*/

stock GTO_GetPlayerTeam(playerid)
{
	if (!IS_IN_RANGE(playerid, 0, MAX_PLAYERS - 1) || !IsPlayerConnected(playerid)) {
		return NO_TEAM;
	}

	return GetPVarInt(playerid, "team");
}

#if defined _ALS_GetPlayerTeam
	#undef GetPlayerTeam
#else
	#define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam GTO_GetPlayerTeam


stock GTO_SetPlayerTeam(playerid, team)
{
	if (!IS_IN_RANGE(playerid, 0, MAX_PLAYERS - 1)) {
		return 0;
	}

	SetPVarInt(playerid, "team", team);
	return 1;
}

#if defined _ALS_SetPlayerTeam
	#undef SetPlayerTeam
#else
	#define _ALS_SetPlayerTeam
#endif
#define SetPlayerTeam GTO_SetPlayerTeam
