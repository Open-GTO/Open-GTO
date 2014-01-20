/*
Project Name:	Open - Grand Theft Online (Open-GTO)
Web site:		http://open-gto.ru/
Date started:	5 November 2009

SA-MP version:		0.3e and older
Date started GTO:	12 August 2006

Developers:

	Open-GTO:
		0.6.4 Ц current		ziggi
		0.6.0 Ц 0.6.4		GhostTT, heufix, Elbi, ziggi

	GTO:
		0.5.9 Ц 0.6.0		SCALOLaz
		0.5.8 Ц 0.5.9		Robin Kikkert (dejavu), Lajos Pacsek (Asturel)
		0.5.7 Ц 0.5.8		Peter Steenbergen (j1nx)
		0.0.0 Ц 0.5.7		Iain Gilbert
	
	Translated to russian by Dmitry Borisoff (Beginner)

*/

#include <..\compiler\includes\a_samp>
#include "utils\foreach"
#include "config"
#include "base"
#include "lang"
#include "utils\mxINI"
#include "logging"
#include "utils\gtoutils"
#include "arrays"
#include "pickup"
#include "vehicles"
#include "player\level"
#include "player\vip"
#include "player\weapon"
#include "player\weapon_drop"
#include "player\weapon_skill"
#include "player\health"
#include "player\armour"
#include "player\vehicle"
#include "bank"
#include "fightstyles"
#include "account"
#include "player"
#include "weapons"
#include "zones"
#include "world"
#include "commands"
#include "gang"
#include "housing"
#include "business"
#include "streamers\mapicon_stream"
#include "streamers\checkpoint_stream"
#include "race"
#include "deathmatch"
#include "payday"
#include "groundhold"
#include "admin\functions"
#include "admin\admin_func"
#include "admin\mod_func"
#include "admin\admin_commands"
#include "admin\admin_commands_race"
#include "admin\admin_commands_dm"
#include "admin\admin_commands_sys"
#include "admin\adm_commands"
#include "admin\mod_commands"
#include "missions"
#include "missions\trucker"
#include "click"
#include "services\fastfood"
#include "services\bar"
#include "services\skinshop"
#include "services\lottery"
#include "services\vehshop"
#include "services\weaponshop"
#include "interior"
#include "weather"
#include "swagup"
#include "quidemsys"
#include "usermenu"
#include "anims"
#include "protections\money"
#include "protections\idle"
#include "protections\rconhack"
#include "protections\hightping"
#include "protections\chatguard"
#include "protections\jetpack"
#include "protections\speedhack"
#include "protections\weaponhack"
// Races
#tryinclude "races\race_monstertruck"
#tryinclude "races\race_thestrip"
#tryinclude "races\race_riversiderun"
#tryinclude "races\race_fleethecity"
#tryinclude "races\race_lostinsmoke"
#tryinclude "races\race_backstreetbang"
#tryinclude "races\race_flyingfree"
#tryinclude "races\race_murderhorn"
#tryinclude "races\race_roundwego"
#tryinclude "races\race_striptease"
#tryinclude "races\race_countrycruise"
#tryinclude "races\race_thegrove"
#tryinclude "races\race_mullholland"
#tryinclude "races\race_lv_julius"
#tryinclude "races\race_ls_trailer1"
#tryinclude "races\race_sf_fuckinwood1"
#tryinclude "races\race_ls_majestic1"

// Deathmatches
#tryinclude "deathmatches\dm_air"
#tryinclude "deathmatches\dm_area51"
#tryinclude "deathmatches\dm_badandugly"
#tryinclude "deathmatches\dm_bluemountains"
#tryinclude "deathmatches\dm_cargoship"
#tryinclude "deathmatches\dm_dildo"
#tryinclude "deathmatches\dm_mbase"
#tryinclude "deathmatches\dm_minigunmadness"
#tryinclude "deathmatches\dm_poolday"
#tryinclude "deathmatches\dm_usnavy"

main() {}

public OnGameModeInit()
{
	SetGameModeText("Open-GTO "#VERSION);
	// Initialize everything that needs it
	lang_OnGameModeInit();
	logging_OnGameModeInit();
	base_OnGameModeInit();
	account_OnGameModeInit();
	player_OnGameModeInit();
	gang_OnGameModeInit();
	payday_OnGameModeInit();
	vehicles_OnGameModeInit();
	race_OnGameModeInit();
	deathmatch_OnGameModeInit();
	groundhold_OnGameModeInit();
	business_OnGameModeInit();
	housing_OnGameModeInit();
	interior_OnGameModeInit();
	bank_OnGameModeInit();
	fights_OnGameModeInit();
	weapon_OnGameModeInit();
	quidemsys_OnGameModeInit();
	lottery_OnGameModeInit();
	swagup_OnGameModeInit();
	vip_OnGameModeInit();
	// missions
	mission_OnGameModeInit();
	trucker_OnGameModeInit();
	// services
	fastfood_OnGameModeInit();
	bar_OnGameModeInit();
	sshop_OnGameModeInit();
	vshop_OnGameModeInit();
	wshop_OnGameModeInit();
	//
	level_OnGameModeInit();
	antiidle_OnGameModeInit();
	antihightping_OnGameModeInit();
	health_OnGameModeInit();
	aah_OnGameModeInit();
	money_OnGameModeInit();
	chatguard_OnGameModeInit();
	antijetpack_OnGameModeInit();
	antirconhack_OnGameModeInit();
	ash_OnGameModeInit();
	awh_OnGameModeInit();
	//
	race_thestrip_init();
	race_riversiderun_init();
	race_fleethecity_init();
	race_lostinsmoke_init();
	race_backstreetbang_init();
	race_flyingfree_init();
	race_murderhorn_init();
	race_roundwego_init();
	race_striptease_init();
	race_monstertruck_init();
	race_countrycruise_init();
	race_thegrove_init();
	race_mullholland_init();
	race_lv_julius_init();
	race_ls_trailer1_init();
	race_sf_fuckinwood1_init();
	race_ls_majestic1_init();
	//
	dm_air_init();
	dm_area51_init();
	dm_badandugly_init();
	dm_bluemountains_init();
	dm_cargoship_init();
	dm_dildo_init();
	dm_mbase_init();
	dm_minigunmadness_init();
	dm_poolday_init();
	dm_usnavy_init();

	#tryinclude "custom\mapicon"
	#tryinclude "custom\pickups"
	#tryinclude "custom\objects"
	GameMSG("SERVER: Custom mapicons, objects and pickups init");

	SyncTime();
	SetWeather( mathrandom(9, 18) );
	SetTimer("OneSecTimer", 1000, 1); // 1 second
	SetTimer("FiveSecondTimer", 5000, 1); // 5 second
	SetTimer("OneMinuteTimer", 60000, 1); // 1 minute
	SetTimer("TenMinuteTimer", 600000, 1); // 10 minute
	SetTimer("OneHourTimer", 3600000, 1); // 1 hour
	if (AntiSpeedHackEnabled == 1)
	{
		SetTimer("AntiSpeedHackTimer", ANTI_SPEED_HACK_CHECK_TIME, 1);
	}
	SetTimerEx("WorldSave", WORLD_SAVE_TIME, 1, "d", 0);
	GameMSG("SERVER: Timers started");
	SpawnWorld();
	
	WorldSave(0);
	new hour, minute;
	gettime(hour, minute);
	GameMSG("SERVER: Open-GTO "#VERSION" initialization complete. [%02d:%02d]", hour, minute);
	return 1;
}

public OnGameModeExit()
{
	WorldSave(1);
	new hour, minute;
	gettime(hour, minute);
	GameMSG("SERVER: Open-GTO "#VERSION" turned off. [%02d:%02d]", hour, minute);
	return 1;
}

public OnPlayerConnect(playerid)
{
	if (IsPlayerNPC(playerid)) return 1;
	player_OnPlayerConnect(playerid);
	chatguard_OnPlayerConnect(playerid);
	level_OnPlayerConnect(playerid);
	weapon_OnPlayerConnect(playerid);
	qudemsys_OnPlayerConnect(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (playerid == INVALID_PLAYER_ID || IsPlayerNPC(playerid)) return 1;
	player_OnPlayerDisconnect(playerid, reason);
	trucker_OnPlayerDisconnect(playerid, reason);
	chatguard_OnPlayerDisconnect(playerid, reason);
	gh_OnPlayerDisconnect(playerid, reason);
	level_OnPlayerDisconnect(playerid, reason);
	weapon_OnPlayerDisconnect(playerid, reason);
	qudemsys_OnPlayerDisconnect(playerid, reason);
	pveh_OnPlayerDisconnect(playerid, reason);
	SetPVarInt(playerid, "Spawned", 0);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
		case account_Log_DialogID, account_Reg_DialogID:
		{
			account_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case house_DialogID, houses_DialogID, houses_upgrades_DialogID, houses_setrent_DialogID, house_sell_accept_DialogID:
		{
			housing_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case bis_DialogID, bis_Msg_DialogID, bis_sell_accept_DialogID:
		{
			business_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case fightstyles_DialogID, fightstyles_user_DialogID:
		{
			fights_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case user_menu_DialogID, user_menu_Return_DialogID,
			vehicle_menu_DialogID, spawnselect_menu_DialogID, vehicle_color_menu_DialogID, vehicle_radio_menu_DialogID,
			settings_menu_DialogID, changenick_menu_DialogID, changepass_menu_DialogID,
			teleport_menu_DialogID, 
			gang_menu_DialogID, gang_create_menu_DialogID, gang_invite_menu_DialogID, gang_color_menu_DialogID,
			gang_motd_menu_DialogID, gang_kick_menu_DialogID, gang_exit_accept_menu_DialogID,
			pveh_select_DialogID, pveh_do_DialogID:
		{
			usermenu_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case wshop_Select_DialogID, wshop_Buy_DialogID:
		{
			wshop_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case bank_Start_DialogID, bank_FirstList_DialogID, bank_Withdraw_DialogID, bank_Deposit_DialogID,
			gang_wmoney_menu_DialogID, gang_dmoney_menu_DialogID:
		{
			bank_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case click_DialogID, click_Resp_DialogID, click_Info_DialogID:
		{
			click_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case bar_DialogID:
		{
			bar_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case fastfood_DialogID:
		{
			fastfood_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case SkinShop_Buy_DialogID:
		{
			sshop_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case trucker_DialogID, trucker_cancel_DialogID:
		{
			trucker_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case vshop_DialogID:
		{
			vshop_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case anims_lib_menu_DialogID, anims_anim_menu_DialogID:
		{
			anims_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	click_OnPlayerClickPlayer(playerid, clickedplayerid);
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	weapon_OnPlayerPickUpPickup(playerid, pickupid);
	aah_OnPlayerPickUpPickup(playerid, pickupid);
	awh_OnPlayerPickUpPickup(playerid, pickupid);
	swagup_OnPlayerPickUpPickup(playerid, pickupid);
	vip_OnPlayerPickUpPickup(playerid, pickupid);
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	dm_OnPlayerEnterCheckpoint(playerid);
	trucker_OnPlayerEnterCheckpoint(playerid);
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	race_OnPlayerEnterCheckpoint(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return 1;
	SetPVarInt(playerid, "Spawned", 0);
	if (killerid == INVALID_PLAYER_ID)
		GameMSG("player: %s(%d): has died > Reason: (%d)", oGetPlayerName(playerid), playerid, reason);
	else
		GameMSG("player: %s(%d): has killed player %s(%d)> Reason: (%d)", oGetPlayerName(killerid), killerid, oGetPlayerName(playerid), playerid, reason);

	SendDeathMessage(killerid, playerid, reason);
	
	if (IsPlayerInAnyDM(playerid))
	{
		deathmatch_OnPlayerDeath(playerid, killerid);
		deathmatch_OnPlayerKill(killerid, playerid, reason);
		return 1;
	}
	
	player_OnPlayerDeath(playerid, killerid, reason);
	player_OnPlayerKill(killerid, playerid, reason);
	trucker_OnPlayerDeath(playerid, killerid, reason);
	gang_OnPlayerDeath(playerid, killerid, reason);
	weapon_OnPlayerDeath(playerid, killerid, reason);
	level_HideTextDraws(playerid);
	PlayCrimeReportForPlayer(killerid, killerid, random(18)+3);
	PlayCrimeReportForPlayer(playerid, killerid, random(18)+3);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if (IsPlayerNPC(playerid)) {
		return 1;
	}

	// spawn player
	player_OnPlayerSpawn(playerid);
	anims_OnPlayerSpawn(playerid);

	// после использовани€ TogglePlayerSpectating
	if (GetPVarInt(playerid, "spec_after_off") == 1) {
		DeletePVar(playerid, "spec_after_off");
		return 1;
	}

	// ставим позицию
	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID || DMPlayerStats[playerid][dm_player_active] == 0) {
		player_SetSpawnPos(playerid);
	} else {
		deathmatch_OnPlayerSpawn(playerid, dmid);
	}

	SetTimerEx("OnPlayerSpawned", 2500, 0, "d", playerid);
	return 1;
}

forward OnPlayerSpawned(playerid);
public OnPlayerSpawned(playerid)
{
	SetPVarInt(playerid, "Spawned", 1);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPVarInt(playerid, "Spawned", 0);
	player_OnPlayerRequestClass(playerid, classid);
	weapon_OnPlayerRequestClass(playerid, classid);
	level_HideTextDraws(playerid);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if (!IsPlayerLogin(playerid)) return 0;
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (!IsPlayerLogin(playerid))
	{
		return SendClientMessage(playerid, -1, lang_texts[1][46]);
	}
	// commands.inc
	command_register(cmdtext, "/help", 5, commands);
	command_register(cmdtext, "/commands", 9, commands);
	command_register(cmdtext, "/info", 5, commands);
	command_register(cmdtext, "/objective", 10, commands);
	command_register(cmdtext, "/sound", 6, commands);
	command_register(cmdtext, "/stats", 6, commands);
	command_register(cmdtext, "/status", 7, commands);
	command_register(cmdtext, "/stat", 5, commands);
	command_register(cmdtext, "/version", 8, commands);
	command_register(cmdtext, "/time", 5, commands);
	command_register(cmdtext, "/skydive", 8, commands);
	command_register(cmdtext, "/dance", 6, commands);
	command_register(cmdtext, "/handsup", 8, commands);
	command_register(cmdtext, "/piss", 5, commands);
	command_register(cmdtext, "/smoke", 6, commands);
	command_register(cmdtext, "/pm", 3, commands);
	
	// Lottery
	command_register(cmdtext, "/lottery", 8, lottery);
	
	// QuidemSys
	command_register(cmdtext, "/fill", 5, quidemsys);
	command_register(cmdtext, "/engine", 7, quidemsys);
	
	// vehicles
	command_register(cmdtext, "/vmenu", 6, vehicles);
	
	// gangs
	command_register(cmdtext, "/g", 2, gang);
	command_register(cmdtext, "/gang", 5, gang);
	
	// race
	command_register(cmdtext, "/races", 6, race);
	command_registerNR(cmdtext, "/race", 5, race);
	
	// admin race
	command_registerNR(cmdtext, "/race", 5, AdminRace);
	
	// rcon admins
	command_registerNR(cmdtext, "/cmdlist", 8, Admin);
	command_registerNR(cmdtext, "/about", 6, Admin);
	command_register(cmdtext, "/carinfo", 8, Admin);
	command_register(cmdtext, "/carrep", 7, Admin);
	command_register(cmdtext, "/repair", 7, Admin);
	command_register(cmdtext, "/go", 3, Admin);
	command_register(cmdtext, "/an", 3, Admin);
	command_register(cmdtext, "/payday", 7, Admin);
	command_register(cmdtext, "/boom", 5, Admin);
	command_register(cmdtext, "/setskin", 8, Admin);
	command_register(cmdtext, "/ssay", 5, Admin);
	command_register(cmdtext, "/skydiveall", 11, Admin);
	command_register(cmdtext, "/disarm", 7, Admin);
	command_register(cmdtext, "/disarmall", 10, Admin);
	command_register(cmdtext, "/paralyzeall", 12, Admin);
	command_register(cmdtext, "/deparalyzeall", 14, Admin);
	command_register(cmdtext, "/remcash", 8, Admin);
	command_register(cmdtext, "/remcashall", 11, Admin);
	command_register(cmdtext, "/setlvl", 7, Admin);
	command_register(cmdtext, "/setstatus", 10, Admin);
	command_register(cmdtext, "/allowport", 10, Admin);
	command_register(cmdtext, "/setvip", 7, Admin);
	command_register(cmdtext, "/underwater", 11, Admin);
	command_register(cmdtext, "/ahideme", 8, Admin);
	command_register(cmdtext, "/ashowme", 8, Admin);
	command_register(cmdtext, "/godmod", 7, Admin);

	// dm
	command_register(cmdtext, "/deathmatches", 13, dm);
	command_register(cmdtext, "/dms", 4, dm);
	command_registerNR(cmdtext, "/dm", 3, dm);
	
	// admin dm
	command_registerNR(cmdtext, "/dm", 3, AdminDM);
	
	// admin sys
	command_register(cmdtext, "/sys", 4, AdminSys);
	
	// admins
	command_registerNR(cmdtext, "/cmdlist", 8, Adm);
	command_registerNR(cmdtext, "/about", 6, Adm);
	command_register(cmdtext, "/say", 4, Adm);
	command_register(cmdtext, "/pinfo", 6, Adm);
	command_register(cmdtext, "/admincnn", 9, Adm);
	command_register(cmdtext, "/akill", 6, Adm);
	command_register(cmdtext, "/tele-set", 9, Adm);
	command_register(cmdtext, "/tele-loc", 9, Adm);
	command_register(cmdtext, "/tele-to", 8, Adm);
	command_register(cmdtext, "/tele-here", 10, Adm);
	command_register(cmdtext, "/tele-hereall", 13, Adm);
	command_register(cmdtext, "/tele-xyzi", 10, Adm);
	command_register(cmdtext, "/sethealth", 10, Adm);
	command_register(cmdtext, "/setarm", 7, Adm);
	command_register(cmdtext, "/givexp", 7, Adm);
	command_register(cmdtext, "/agivecash", 10, Adm);
	command_register(cmdtext, "/givegun", 8, Adm);
	command_register(cmdtext, "/paralyze", 9, Adm);
	command_register(cmdtext, "/deparalyze", 11, Adm);
	command_register(cmdtext, "/showpm", 7, Adm);
	command_register(cmdtext, "/getip", 6, Adm);
	command_register(cmdtext, "/ban", 4, Adm);
	command_register(cmdtext, "/unban", 6, Adm);
	
	// moderators
	command_registerNR(cmdtext, "/cmdlist", 8, Mod);
	command_registerNR(cmdtext, "/about", 6, Mod);
	command_register(cmdtext, "/plist", 6, Mod);
	command_register(cmdtext, "/remcar", 7, Mod);
	command_register(cmdtext, "/kick", 5, Mod);
	command_register(cmdtext, "/carresp", 8, Mod);
	command_register(cmdtext, "/carrespall", 11, Mod);
	command_register(cmdtext, "/mute", 5, Mod);
	command_register(cmdtext, "/unmute", 7, Mod);
	command_register(cmdtext, "/jail", 5, Mod);
	command_register(cmdtext, "/unjail", 7, Mod);
	command_register(cmdtext, "/mole", 5, Mod);
	command_register(cmdtext, "/spec", 5, Mod);
	command_register(cmdtext, "/clearchat", 10, Mod);
	command_register(cmdtext, "/weather", 8, Mod);

	new logstring[MAX_STRING];
	format(logstring, sizeof(logstring), "Player: %s"CHAT_SHOW_ID": %s", oGetPlayerName(playerid), playerid, cmdtext);
	WriteLog(CMDLog, logstring);
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if (!IsPlayerLogin(playerid))
	{
		SendClientMessage(playerid, -1, lang_texts[1][46]);
		return 0;
	}
	if (chatguard_OnPlayerText(playerid, text) == 0) return 0;

	new playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	
	new string[MAX_STRING];
	switch (text[0])
	{
		case '!':
		{
			if (GetPVarInt(playerid, "GangID") == 0 || GetPlayerMuteTime(playerid) > 0)
			{
				SendClientMessage(playerid, COLOUR_RED, lang_texts[1][14]);
				return 0;
			}
			if (strlen(text[1]) < 2) return 1;
			format(string, sizeof(string), "%s"CHAT_SHOW_ID" банде: {FFFFFF}%s", playername, playerid, text[1]);
			SendGangMessage(GetPVarInt(playerid, "GangID"), string, COLOUR_GANG_CHAT);
			format(string, sizeof(string), "Player: %s"CHAT_SHOW_ID": <GANG CHAT>: %s", playername, playerid, text[1]);
			WriteLog(ChatLog, string);
			return 0;
		}
		case '@','"':
		{
			if (strlen(text[1]) < 2) return 1;
			SendClientMessageToAdmins(playerid, text[1]);
			format(string, sizeof(string), "Player: %s"CHAT_SHOW_ID": <ADMIN TALK>: %s", playername, playerid, text[1]);
			WriteLog(ChatLog, string);
			return 0;
		}
		case '#','є':
		{
			if (strlen(text[1]) < 2) return 1;
			SendClientMessageToModers(playerid, text[1]);
			format(string, sizeof(string), "Player: %s"CHAT_SHOW_ID": <MODERATOR TALK>: %s", playername, playerid, text[1]);
			WriteLog(ChatLog, string);
			return 0;
		}
		case '$',';':
		{
			if (strlen(text[1]) < 2) return 1;
			if (GetPlayerMuteTime(playerid) > 0)
			{
				SendClientMessage(playerid, COLOUR_RED, lang_texts[1][14]);
				return 0;
			}
			SendClientMessageToBeside(playerid, 10, text[1]);
			format(string, sizeof(string), "Player: %s"CHAT_SHOW_ID": <SAY>: %s", playername, playerid, text[1]);
			WriteLog(ChatLog, string);
			return 0;
		}
	}
	if (GetPlayerMuteTime(playerid) != 0)  //«аткнут
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[1][14]);
		return 0;
	}
	format(string, sizeof(string), "%s"CHAT_SHOW_ID": {FFFFFF}%s", playername, playerid, text);
	SendClientMessageToAll(GetPlayerColor(playerid), string);
	format(string, sizeof(string), "Player: %s"CHAT_SHOW_ID": %s", playername, playerid, text);
	WriteLog(ChatLog, string);
	return 0;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if ( PRESSED( KEY_FIRE ) || PRESSED ( KEY_SECONDARY_ATTACK ) )
	{
		if ( GetPVarInt(playerid, "bar_Drinking") == 1 ) return bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	}
	if ( PRESSED ( KEY_USING ) )
	{
		if ( IsPlayerAtEnterExit(playerid) ) return interior_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtHouse(playerid) ) return housing_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtBusiness(playerid) ) return business_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtBank(playerid) ) return bank_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtWeaponShop(playerid) ) return wshop_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtFightTrener(playerid) ) return fights_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtFastFood(playerid) ) return fastfood_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtBar(playerid) ) return bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtSkinShop(playerid) ) return sshop_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		show_menu(playerid);
		return 1;
	}
	if ( PRESSED( KEY_SUBMISSION ) )
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if ( vehicleid != 0 && IsVehicleIsRunner(vehicleid) )
		{
			trucker_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		}
		return 1;
	}
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	vehicle_OnPlayerStateChange(playerid, newstate, oldstate);
	ash_OnPlayerStateChange(playerid, newstate, oldstate);
	qudemsys_OnPlayerStateChange(playerid, newstate, oldstate);
	if (newstate == PLAYER_STATE_DRIVER)
	{
	#if defined OLD_ENGINE_DO
		if (GetPlayerVehicleSeat(playerid) == 0)
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			
			vehicle_Engine(vehicleid, VEHICLE_PARAMS_ON);
			
			new hour;
			gettime(hour);
			if (hour > VEHICLE_LIGHTS_ON_TIME || hour < VEHICLE_LIGHTS_OFF_TIME)
			{
				vehicle_Light(vehicleid, VEHICLE_PARAMS_ON);
			}
		}
	#endif
		trucker_OnPlayerStateChange(playerid, newstate, oldstate);
		vip_OnPlayerStateChange(playerid, newstate, oldstate);
	}
	if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
	{
		vshop_OnPlayerStateChange(playerid, newstate, oldstate);
	}
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
#if defined OLD_ENGINE_DO
	if (GetPlayerVehicleSeat(playerid) == 0)
		vehicle_Engine(vehicleid, VEHICLE_PARAMS_OFF);
#endif
	modfunc_OnPlayerExitVehicle(playerid, vehicleid);
	awh_OnPlayerExitVehicle(playerid, vehicleid);
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	ash_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	modfunc_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	modfunc_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	bar_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	quidemsys_OnVehicleSpawn(vehicleid);
	vshop_OnVehicleSpawn(vehicleid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	trucker_OnVehicleDeath(vehicleid, killerid);
	pveh_OnVehicleDeath(vehicleid, killerid);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	switch(success)
	{
		case 0:
		{
			antirconhack_OnRconLoginAttempt(ip, password, success);
		}
		case 1:
		{
			// если игрок заходит ркон админом, то дадим ему полные права
			new pip[MAX_IP];
			foreach (Player, playerid)
			{
				GetPVarString(playerid, "IP", pip, sizeof(pip));
				if (!strcmp(ip, pip, false))
				{
					SetPlayerStatus(playerid, STATUS_LEVEL_RCON);
					break;
				}
			}
		}
	}
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	admin_OnPlayerClickMap(playerid, fX, fY, fZ);
	player_OnPlayerClickMap(playerid, fX, fY, fZ);
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid)
{
	admin_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid);
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	pveh_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	pveh_OnVehicleRespray(playerid, vehicleid, color1, color2);
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	pveh_OnVehicleMod(playerid, vehicleid, componentid);
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		if (enterexit == 1) {
			SetVehicleVirtualWorld(vehicleid, playerid + 1);
			SetPlayerVirtualWorld(playerid, playerid + 1);
		} else {
			SetVehicleVirtualWorld(vehicleid, 0);
			SetPlayerVirtualWorld(playerid, 0);
		}
	}
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (clickedid == Text:INVALID_TEXT_DRAW) {
		if (IsPlayerAtSkinShop(playerid)) {
			return sshop_OnPlayerClickTextDraw(playerid, clickedid);
		}
	}

	if (clickedid == clickText[sshop_left] || clickedid == clickText[sshop_right] || clickedid == clickText[sshop_buy] || 
		clickedid == clickText[sshop_list] || clickedid == clickText[sshop_exit]
		) {
		return sshop_OnPlayerClickTextDraw(playerid, clickedid);
	}
	return 0;
}