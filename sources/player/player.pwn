/*

	Author: Iain Gilbert (05.09.06)
	Contributor: ziggi

*/

#if defined _player_included
	#endinput
#endif

#define _player_included

/*
	Vars
*/

static
	gPlayerDeaths[MAX_PLAYERS],
	gPlayerKills[MAX_PLAYERS],
	gPlayerSkin[MAX_PLAYERS],

	gPlayerStartMoney = PLAYER_START_MONEY;

/*
	Config
*/

Player_LoadConfig(file_config)
{
	ini_getString(file_config, "Player_DB", db_player);
	ini_getInteger(file_config, "Player_Start_Money", gPlayerStartMoney);

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

Player_SaveConfig(file_config)
{
	ini_setString(file_config, "Player_DB", db_player);
	ini_setInteger(file_config, "Player_Start_Money", gPlayerStartMoney);
	ini_setString(file_config, "Player_Start_Weapons", CreatePlayerStartWeaponsString());
	ini_setString(file_config, "Player_Start_Bullets", CreatePlayerStartBulletsString());
}

/*
	For public
*/

Player_OnPlayerSpawn(playerid)
{
	if (IsPlayerMuted(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(playerid, MUTED_HELP_MESSAGE));
	}

	PlayerLevelTD_ShowTextDraw(playerid);
	PlayerWSkillTD_UpdateString(playerid);

	SetPlayerColor(playerid, GetPlayerGangColor(playerid));
	if (!Account_IsAfterRegistration(playerid)) {
		SetPlayerSkin(playerid, GetPlayerSkin(playerid));
	}
	SetPlayerMaxHealth(playerid);
	UpdatePlayerFightStyleUsed(playerid);
	GivePlayerOwnedWeapon(playerid);
	PlayerMoneyTD_Show(playerid);
	return 1;
}

Player_OnPlayerDisconnect(playerid, reason)
{
	// update params
	Account_SetPlayedTime(playerid, Account_GetCurrentPlayedTime(playerid));

	// save
	Player_Save(playerid);
	Account_Save(playerid);

	// message
	new string[MAX_LANG_VALUE_STRING];
	format(string, sizeof(string), _(playerid, PLAYER_DISCONNECT), ReturnPlayerName(playerid), playerid);

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

	// gang logout
	new gangid = GetPlayerGangID(playerid);

	new is_ok = Gang_MemberLogout(playerid, gangid);
	if (is_ok) {
		format(string, sizeof(string), _(playerid, GANG_MEMBER_LOGOUT), ReturnPlayerName(playerid));
		Gang_SendMessage(gangid, string, COLOR_GANG);
	}

	// other stuff
	StopAudioStreamForPlayer(playerid);
	SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
}

Player_OnPlayerConnect(playerid)
{
	new
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	// load language
	Account_LoadLanguage(playerid, playername);

	// connect message
	Chat_Clear(playerid);
	SendClientMessage(playerid, COLOR_WHITE, _(playerid, PLAYER_LOADING));

	// store ip
	Player_UpdateIP(playerid);

	// check name
	if (!NameCharCheck(playername)) {
		new string[MAX_LANG_VALUE_STRING];
		format(string, sizeof(string), _(playerid, PLAYER_NICK_BAD_SYMBOLS), ALLOWED_NICK_SYMBOLS_STR);
		SendClientMessage(playerid, COLOR_RED, string);
		SendClientMessage(playerid, COLOR_RED, _(playerid, PLAYER_NICK_IS_IP));
		KickPlayer(playerid, "“акой ник запрещЄн.");
	}

	// set color
	SetPlayerColor(playerid, COLOR_PLAYER);

	// check ban
	oBan_Check(playerid);
}

Player_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason
	UpdatePlayerSpawnInfo(playerid);
	SetPlayerArmour(playerid, 0.0);

	if (killerid == INVALID_PLAYER_ID || IsPlayersTeammates(playerid, killerid)) {
		return;
	}

	AddPlayerDeaths(playerid);
	AddPlayerKills(killerid);

	if (IsPlayerJailed(playerid)) {
		return;
	}

	// gang kill
	new killer_gang_id = GetPlayerGangID(killerid);
	if (killer_gang_id != INVALID_GANG_ID) {
		if (Gang_PlayerKill(killer_gang_id, killerid, playerid) == 1) {
			new string[MAX_LANG_VALUE_STRING];

			GetPlayerName(killerid, string, sizeof(string));
			format(string, sizeof(string), _(playerid, GANG_KILL_TEAMMATE), string);

			Gang_SendMessage(killer_gang_id, string, COLOR_GANG);
			return;
		}

		GiveGangXP(killer_gang_id, (GetGangLevel(killer_gang_id) + 1) * 20);
	}

	// give money
	new stolencash = (GetPlayerMoney(playerid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if (stolencash > 0) {
		GivePlayerMoney(playerid, -stolencash);
		GivePlayerMoney(killerid, stolencash);
	}

	// give xp
	new xp_give_player = -GetPlayerXP(playerid) / 100 * PLAYER_XP_DEATH_MINUS_PROC;
	new xp_give_killer = (pow(GetPlayerLevel(playerid) * 4, 2) / (GetPlayerLevel(killerid) + 10) + 20) * PLAYER_XP_KILL_TARIF;

	new level_difference = GetPlayerLevel(playerid) - GetPlayerLevel(killerid);

	if (level_difference <= -10) {
		xp_give_player = 0;
	}

	GivePlayerXP(killerid, xp_give_killer);
	GivePlayerXP(playerid, xp_give_player);
}

Player_OnPlayerRequestClass(playerid, classid)
{
	#pragma unused classid
	PlayerMoneyTD_Hide(playerid);

	// show login dialog
	if (!IsPlayerLogin(playerid)) {
		Account_ShowDialog(playerid);
	}
	return 1;
}

stock Player_OnLogin(playerid)
{
	// spawn player
	new
		Float:spawn_pos_x,
		Float:spawn_pos_y,
		Float:spawn_pos_z,
		Float:spawn_pos_a;

	GetPlayerSpawnPos(playerid, spawn_pos_x, spawn_pos_y, spawn_pos_z, spawn_pos_a);
	SetSpawnInfo(playerid, 0, GetPlayerSkin(playerid), spawn_pos_x, spawn_pos_y, spawn_pos_z, spawn_pos_a, 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, 0);

	// skin select
	if (Account_IsAfterRegistration(playerid)) {
		#if defined PLAYER_START_SKIN
			SetPlayerSkin(playerid, PLAYER_START_SKIN);
		#else
			new
				Float:camera_pos_x,
				Float:camera_pos_y,
				Float:camera_pos_z;

			camera_pos_z = spawn_pos_z + 1.0;

			GetCoordsBefore(spawn_pos_x, spawn_pos_y, spawn_pos_a, 2.0, camera_pos_x, camera_pos_y);
			SetPlayerCameraPos(playerid, camera_pos_x, camera_pos_y, camera_pos_z);
			SetPlayerCameraLookAt(playerid, spawn_pos_x, spawn_pos_y, spawn_pos_z + 0.5);

			SkinSelect_Start(playerid, SkinSelect:RegisterSkin);
		#endif
	}

	// reset data
	SetPlayerQuestID(playerid, INVALID_QUEST_ID);

	// send message
	new
		string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	format(string, sizeof(string), _(playerid, ACCOUNT_LOGIN_MESSAGE_0), VERSION_STRING);
	SendClientMessage(playerid, COLOR_LIGHTRED, string);
	SendClientMessage(playerid, COLOR_WHITE, _(playerid, ACCOUNT_LOGIN_MESSAGE_1));
	SendClientMessage(playerid, COLOR_WHITE, _(playerid, ACCOUNT_LOGIN_MESSAGE_2));
	SendClientMessage(playerid, COLOR_GREEN, _(playerid, ACCOUNT_LOGIN_MESSAGE_3));

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
		SendClientMessage(playerid, COLOR_GREEN, _(playerid, ACCOUNT_LOGIN_ROOT));
	} else if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		SendClientMessage(playerid, COLOR_GREEN, _(playerid, ACCOUNT_LOGIN_ADMIN));
	} else if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		SendClientMessage(playerid, COLOR_GREEN, _(playerid, ACCOUNT_LOGIN_MODER));
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		SendClientMessage(playerid, COLOR_GREEN, _(playerid, ACCOUNT_SUCCESS_LOGIN));
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	foreach (new id : Player) {
		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			continue;
		}

		format(string, sizeof(string), _(playerid, PLAYER_CONNECT), playername, playerid);
		SendClientMessage(id, COLOR_WHITE, string);
	}

	// set privilejes if player is rcon admin
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

			format(string, sizeof(string), _(playerid, GANG_MEMBER_LOGIN_REMOVED), gangname);
			SendClientMessage(playerid, COLOR_ORANGE, string);
		} else {
			new is_ok = Gang_MemberLogin(playerid, gangid);

			if (is_ok) {
				format(string, sizeof(string), _(playerid, GANG_MEMBER_LOGIN), playername);
				Gang_SendMessage(gangid, string, COLOR_GANG);

				format(string, sizeof(string), _(playerid, GANG_MEMBER_LOGIN_SELF), gangname, Gang_GetOnlineCount(gangid) - 1);
				SendClientMessage(playerid, COLOR_GANG, string);

				Gang_GetMotd(gangid, string);

				if (strlen(string) > 0) {
					format(string, sizeof(string), _(playerid, GANG_MEMBER_LOGIN_MOTD), string);
					SendClientMessage(playerid, COLOR_GANG, string);
				}
			}
		}
	}

	// hide widstrip
	Widestrip_HideForPlayer(playerid);

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

stock Player_SetDefaultData(playerid)
{
	SetPlayerMoney(playerid, gPlayerStartMoney);
	SetPlayerJailTime(playerid, -1);
	SetPlayerSpawnType(playerid, SPAWN_TYPE_NONE);

#if defined PLAYER_START_SKIN
	SetPlayerSkin(playerid, PLAYER_START_SKIN);
#else
	SetPlayerSkin(playerid, SKINS_MINID);
#endif

	SetPlayerLevel(playerid, PLAYER_START_LEVEL, 0, 0);
	SetPlayerPrivilege(playerid, PlayerPrivilegePlayer);
	SetPlayerFightStyleUsed(playerid, FIGHT_STYLE_NORMAL);

	ResetPlayerRandomSpawnID(playerid);
	ResetPlayerGangData(playerid);
	ResetPlayerWeapons(playerid);
	ResetPlayerSkillLevel(playerid);
	ResetPlayerQuest(playerid);

	for (new i = 0; i < sizeof(PlayerStartWeapon); i++) {
		GivePlayerWeapon(playerid, PlayerStartWeapon[i][pwid], PlayerStartWeapon[i][pbullets], true);
	}

	for (new pinterface; pinterface < sizeof(gPlayerInterface[]); pinterface++) {
		SetPlayerInterfaceParam(playerid, PlayerInterface:pinterface, PIP_Visible, true);
	}
}

stock Player_Sync(playerid)
{
	// если игрок мертв, то защиты не срабатывают
	if (!IsPlayerSpawned(playerid)) {
		return 0;
	}

	pt_ping_Check(playerid);
	pt_spac_Check(playerid);
	pt_health_Sync(playerid);
	pt_armour_Sync(playerid);
	pt_weapon_Check(playerid);
	return 1;
}

/*
	Weapon
*/

stock ShowPlayerWeaponsOnLevel(playerid, newlevel, oldlevel)
{
	new
		string[MAX_LANG_VALUE_STRING],
		bool:is_found;

	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++) {
		if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
			continue;
		}

		if (oldlevel < GetWeaponLevel(weaponid) <= newlevel) {
			if (!is_found) {
				SendClientMessage(playerid, COLOR_GREEN, _(playerid, PLAYER_LEVEL_NEW_WEAPON));
				is_found = true;
			}
			format(string, sizeof(string), _(playerid, PLAYER_LEVEL_NEW_WEAPON_ITEM), ReturnPlayerWeaponName(playerid, weaponid), GetWeaponCost(weaponid));
			SendClientMessage(playerid, COLOR_MISC, string);
		}
	}
	return 1;
}

/*
	Name
*/

stock ReturnPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

/*
	Kills
*/

stock GetPlayerKills(playerid)
{
	return gPlayerKills[playerid];
}

stock SetPlayerKills(playerid, amount)
{
	gPlayerKills[playerid] = amount;
}

stock AddPlayerKills(playerid, amount = 1)
{
	gPlayerKills[playerid] += amount;
}

/*
	Deaths
*/

stock GetPlayerDeaths(playerid)
{
	return gPlayerDeaths[playerid];
}

stock SetPlayerDeaths(playerid, amount)
{
	gPlayerDeaths[playerid] = amount;
}

stock AddPlayerDeaths(playerid, amount = 1)
{
	gPlayerDeaths[playerid] += amount;
}

/*
	Kills-Deaths ratio
*/

stock Float:GetPlayerKillDeathRatio(playerid)
{
	new deaths = GetPlayerDeaths(playerid);
	if (deaths != 0) {
		return float(GetPlayerKills(playerid)) / float(deaths);
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
	Money
*/

stock GetPlayerTotalMoney(playerid)
{
	return GetPlayerMoney(playerid) + GetPlayerBankMoney(playerid);
}
