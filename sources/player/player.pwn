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
		Lang_SendText(playerid, "MUTED_HELP_MESSAGE");
	}

	SetPlayerColor(playerid, GetPlayerGangColor(playerid));
	if (!Account_IsAfterRegistration(playerid)) {
		SetPlayerSkin(playerid, GetPlayerSkin(playerid));
	}
	SetPlayerMaxHealth(playerid);
	UpdatePlayerFightStyleUsed(playerid);
	GivePlayerOwnedWeapon(playerid);
	return 1;
}

Player_OnPlayerDisconnect(playerid, reason)
{
	// update params
	Account_SetAfterRegistration(playerid, false);
	Account_SetPlayedTime(playerid, Account_GetCurrentPlayedTime(playerid));

	// save
	Player_Save(playerid);
	Account_Save(playerid);

	// message
	new playername[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, playername, sizeof(playername));

	switch (reason) {
		case 0: {
			Lang_SendTextToAll("PLAYER_DISCONNECT_CRASH", playername, playerid);
		}
		case 1: {
			Lang_SendTextToAll("PLAYER_DISCONNECT_EXIT", playername, playerid);
		}
		case 2: {
			Lang_SendTextToAll("PLAYER_DISCONNECT_KICK", playername, playerid);
		}
	}

	// gang logout
	new gangid = GetPlayerGangID(playerid);

	new is_ok = Gang_MemberLogout(playerid, gangid);
	if (is_ok) {
		Gang_SendLangMessage(gangid, "GANG_MEMBER_LOGOUT", _, playername);
	}

	// other stuff
	StopAudioStreamForPlayer(playerid);
	SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);

	// reset login status
	SetPlayerLoginStatus(playerid, false);
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
	Lang_SendText(playerid, "PLAYER_LOADING");

	// store ip
	Player_UpdateIP(playerid);

	// check name
	if (!NameCharCheck(playername)) {
		Lang_SendText(playerid, "PLAYER_NICK_BAD_SYMBOLS", ALLOWED_NICK_SYMBOLS_STR);
		Lang_SendText(playerid, "PLAYER_NICK_IS_IP");
		KickPlayer(playerid, "Такой ник запрещён.");
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

	if (killerid == INVALID_PLAYER_ID) {
		return;
	}

	if (IsPlayerJailed(playerid)) {
		return;
	}

	// is gang kill
	new killer_gang_id = GetPlayerGangID(killerid);
	if (killer_gang_id != INVALID_GANG_ID) {
		if (Gang_PlayerKill(killer_gang_id, killerid, playerid) == 1) {
			new
				killername[MAX_PLAYER_NAME + 1],
				playername[MAX_PLAYER_NAME + 1];

			GetPlayerName(killerid, killername, sizeof(killername));
			GetPlayerName(playerid, playername, sizeof(playername));

			Gang_SendLangMessage(killer_gang_id, "GANG_KILL_TEAMMATE", _, killername, killerid, playername, playerid);
			return;
		}

		GiveGangXP(killer_gang_id, (GetGangLevel(killer_gang_id) + 1) * 20);
	}

	// give stats
	AddPlayerDeaths(playerid);
	AddPlayerKills(killerid);

	// give money
	new stolencash = (GetPlayerMoney(playerid) / 100) * PLAYER_MONEY_DEATH_MINUS_PROC;
	if (stolencash > 0) {
		GivePlayerMoney(playerid, -stolencash);
		GivePlayerMoney(killerid, stolencash);
	}

	// give xp
	new
		player_level = GetPlayerLevel(playerid),
		killer_level = GetPlayerLevel(killerid),
		max_level = GetMaxPlayerLevel(),
		player_xp = (player_level == max_level) ? GetXPToLevel(max_level) : GetPlayerXP(playerid);

	if (player_xp < 100) {
		player_xp = 100;
	}

	new
		xp_give_player = 0,
		xp_give_killer = (pow(player_level * 4, 2) / (killer_level + 10) + 20) * PLAYER_XP_KILL_TARIF;

	// check level difference
	if ((player_level - killer_level) > -10) {
		xp_give_player = (-player_xp / 100) * PLAYER_XP_DEATH_MINUS_PROC;
	}

	GivePlayerXP(killerid, xp_give_killer);
	GivePlayerXP(playerid, xp_give_player);

	Lang_SendText(playerid, "PLAYER_DEATH_MESSAGE", stolencash, -xp_give_player);
	Lang_SendText(killerid, "PLAYER_KILL_MESSAGE", stolencash, xp_give_killer);
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

public OnPlayerLogin(playerid)
{
	// spawn player
	new
		Float:spawn_pos_x,
		Float:spawn_pos_y,
		Float:spawn_pos_z,
		Float:spawn_pos_a;

	GetPlayerSpawnPos(playerid, spawn_pos_x, spawn_pos_y, spawn_pos_z, spawn_pos_a);
	SetSpawnInfo(playerid, NO_TEAM, GetPlayerSkin(playerid), spawn_pos_x, spawn_pos_y, spawn_pos_z, spawn_pos_a, 0, 0, 0, 0, 0, 0);
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

			GetCoordsInFront(spawn_pos_x, spawn_pos_y, spawn_pos_a, 2.0, camera_pos_x, camera_pos_y);
			SetPlayerCameraPos(playerid, camera_pos_x, camera_pos_y, camera_pos_z);
			SetPlayerCameraLookAt(playerid, spawn_pos_x, spawn_pos_y, spawn_pos_z + 0.5);

			SkinSelect_Start(playerid, SkinSelect:RegisterSkin);

			TogglePlayerInterfaceVisibility(playerid, false);
		#endif
	}

	// reset data
	SetPlayerQuestID(playerid, INVALID_QUEST_ID);

	// send message
	new
		string[MAX_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	Lang_SendText(playerid, "ACCOUNT_LOGIN_MESSAGE_0", VERSION_STRING);
	Lang_SendText(playerid, "ACCOUNT_LOGIN_MESSAGE_1");
	Lang_SendText(playerid, "ACCOUNT_LOGIN_MESSAGE_2");
	Lang_SendText(playerid, "ACCOUNT_LOGIN_MESSAGE_3");

	if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
		Lang_SendText(playerid, "ACCOUNT_LOGIN_ROOT");
	} else if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		Lang_SendText(playerid, "ACCOUNT_LOGIN_ADMIN");
	} else if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		Lang_SendText(playerid, "ACCOUNT_LOGIN_MODER");
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	} else {
		Lang_SendText(playerid, "ACCOUNT_SUCCESS_LOGIN");
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
	}

	foreach (new id : Player) {
		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			continue;
		}

		Lang_SendText(id, "PLAYER_CONNECT", playername, playerid);
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

			Lang_SendText(playerid, "GANG_MEMBER_LOGIN_REMOVED", gangname);
		} else {
			new is_ok = Gang_MemberLogin(playerid, gangid);

			if (is_ok) {
				Gang_SendLangMessage(gangid, "GANG_MEMBER_LOGIN", _, playername);
				Lang_SendText(playerid, "GANG_MEMBER_LOGIN_SELF", gangname, Gang_GetOnlineCount(gangid) - 1);

				Gang_GetMotd(gangid, string);

				if (strlen(string) > 0) {
					Lang_SendText(playerid, "GANG_MEMBER_LOGIN_MOTD", string);
				}
			}
		}
	}

	// hide widstrip
	Widestrip_HideForPlayer(playerid);

	Log(mainlog, INFO, "Player: %s [id: %d] logged in successfully.", playername, playerid);

	#if defined Player_OnPlayerLogin
		return Player_OnPlayerLogin(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerLogin
	#undef OnPlayerLogin
#else
	#define _ALS_OnPlayerLogin
#endif

#define OnPlayerLogin Player_OnPlayerLogin
#if defined Player_OnPlayerLogin
	forward Player_OnPlayerLogin(playerid);
#endif

SkinSelectResponse:RegisterSkin(playerid, SS_Response:type, oldskin, newskin)
{
	if (type == SS_Response:SS_Response_Stop || type == SS_Response:SS_Response_Select) {
		SetPlayerSkin(playerid, newskin);
		SetCameraBehindPlayer(playerid);

		TogglePlayerInterfaceVisibility(playerid, true);
	}

	if (type == SS_Response:SS_Response_Select) {
		SkinSelect_Stop(playerid);

		TogglePlayerInterfaceVisibility(playerid, true);
	}
}

stock Player_SetDefaultData(playerid)
{
	SetPlayerMoney(playerid, gPlayerStartMoney, false);
	SetPlayerBankMoney(playerid, 0);
	SetPlayerJailTime(playerid, -1);
	SetPlayerSpawnType(playerid, SPAWN_TYPE_NONE);

#if defined PLAYER_START_SKIN
	SetPlayerSkin(playerid, PLAYER_START_SKIN);
#else
	SetPlayerSkin(playerid, SKINS_MINID);
#endif

	SetPlayerLevel(playerid, PLAYER_START_LEVEL, false, false);
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

/*
	Weapon
*/

stock ShowPlayerWeaponsOnLevel(playerid, newlevel, oldlevel)
{
	new
		bool:is_found;

	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++) {
		if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
			continue;
		}

		if (oldlevel < GetWeaponLevel(weaponid) <= newlevel) {
			if (!is_found) {
				Lang_SendText(playerid, "PLAYER_LEVEL_NEW_WEAPON");
				is_found = true;
			}

			Lang_SendText(playerid, "PLAYER_LEVEL_NEW_WEAPON_ITEM",
			              ret_GetPlayerWeaponName(playerid, weaponid),
 			              GetWeaponCost(weaponid));
		}
	}
	return 1;
}

/*
	Name
*/

stock ret_GetPlayerName(playerid)
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
	Money
*/

stock GetPlayerTotalMoney(playerid)
{
	return GetPlayerMoney(playerid) + GetPlayerBankMoney(playerid);
}
