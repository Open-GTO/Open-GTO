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
#include "utils\zcmd"
#include "utils\gtodialog"
#include "config"
#include "base"
#include "lang"
#include "utils\mxINI"
#include "logging"
#include "utils\gtoutils"
#include "arrays"
#include "pickup"
#include "vehicles"
#include "vehicle\quidemsys"
#include "vehicle\vehicle_menu"
#include "vehicle\vehicle_damage"
#include "player\level"
#include "player\vip"
#include "player\weapon"
#include "player\weapon_drop"
#include "player\weapon_skill"
#include "player\player_health"
#include "player\player_fights"
#include "player\vehicle"
#include "player\status"
#include "player\account"
#include "player\login"
#include "player\player_spawn"
#include "player\player_pm"
#include "player\player_mute"
#include "player\player_click"
#include "player\player_menu"
#include "player\player_menu_vehicle"
#include "player\player_menu_teleport"
#include "player\player_menu_settings"
#include "player\player_quest"
#include "quest"
#include "bank"
#include "player"
#include "weapons"
#include "zones"
#include "world"
#include "commands"
#include "gang"
#include "gang\gang_menu"
#include "gang\gang_level"
#include "housing"
#include "business"
#include "streamers\mapicon_stream"
#include "streamers\checkpoint_stream"
#include "race"
#include "deathmatch"
#include "payday"
#include "groundhold"
#include "admin\admin_commands_race"
#include "admin\admin_commands_dm"
#include "admin\admin_commands"
#include "admin\admin_ban"
#include "admin\admin_mute"
#include "admin\admin_jail"
#include "admin\admin_spec"
#include "admin\admin_godmod"
#include "admin\admin_pm"
#include "admin\admin_hide"
#include "admin\admin_maptp"
#include "admin\admin_click"
#include "admin"
#include "missions"
#include "missions\trucker"
#include "click"
#include "services\fastfood"
#include "services\bar"
#include "services\skinshop"
#include "services\lottery"
#include "services\vehshop"
#include "services\weaponshop"
#include "services\fightteacher"
#include "interior"
#include "weather"
#include "swagup"
#include "anims"
#include "protections\money"
#include "protections\idle"
#include "protections\rconhack"
#include "protections\heightping"
#include "protections\chatguard"
#include "protections\jetpack"
#include "protections\speedhack"
#include "protections\weaponhack"
#include "protections\health"
#include "protections\armour"

#include "cfg"
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
	cfg_LoadConfigs();

	lang_OnGameModeInit();
	base_OnGameModeInit();
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
	swagup_OnGameModeInit();
	vip_OnGameModeInit();
	pl_weapon_OnGameModeInit();
	// missions
	trucker_OnGameModeInit();
	// services
	fastfood_OnGameModeInit();
	bar_OnGameModeInit();
	sshop_OnGameModeInit();
	vshop_OnGameModeInit();
	wshop_OnGameModeInit();
	// protection
	pt_idle_OnGameModeInit();
	pt_chat_OnGameModeInit();
	pt_speed_OnGameModeInit();
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
	pt_chat_OnPlayerConnect(playerid);
	pl_level_OnPlayerConnect(playerid);
	pl_weapon_OnPlayerConnect(playerid);
	qudemsys_OnPlayerConnect(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (playerid == INVALID_PLAYER_ID || IsPlayerNPC(playerid)) return 1;
	player_OnPlayerDisconnect(playerid, reason);
	trucker_OnPlayerDisconnect(playerid, reason);
	pt_chat_OnPlayerDisconnect(playerid, reason);
	gh_OnPlayerDisconnect(playerid, reason);
	pl_level_OnPlayerDisconnect(playerid, reason);
	pl_weapon_OnPlayerDisconnect(playerid, reason);
	qudemsys_OnPlayerDisconnect(playerid, reason);
	pveh_OnPlayerDisconnect(playerid, reason);
	player_SetSpawned(playerid, 0);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	click_OnPlayerClickPlayer(playerid, clickedplayerid);
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	pl_weapon_OnPlayerPickUpPickup(playerid, pickupid);
	pt_armour_OnPlayerPickUpPickup(playerid, pickupid);
	pt_weapon_OnPlayerPickUpPickup(playerid, pickupid);
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
	if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) {
		return 1;
	}

	player_SetSpawned(playerid, 0);

	if (killerid == INVALID_PLAYER_ID) {
		GameMSG("player: %s(%d): has died > Reason: (%d)", oGetPlayerName(playerid), playerid, reason);
	} else {
		GameMSG("player: %s(%d): has killed player %s(%d)> Reason: (%d)", oGetPlayerName(killerid), killerid, oGetPlayerName(playerid), playerid, reason);
	}

	SendDeathMessage(killerid, playerid, reason);
	
	if (IsPlayerInAnyDM(playerid)) {
		deathmatch_OnPlayerDeath(playerid, killerid);
		deathmatch_OnPlayerKill(killerid, playerid, reason);
		return 1;
	}
	
	player_OnPlayerDeath(playerid, killerid, reason);
	admin_OnPlayerDeath(playerid, killerid, reason);
	trucker_OnPlayerDeath(playerid, killerid, reason);
	gang_OnPlayerDeath(playerid, killerid, reason);
	pl_weapon_OnPlayerDeath(playerid, killerid, reason);
	pl_level_OnPlayerDeath(playerid, killerid, reason);

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
		pl_spawn_SetSpawnPos(playerid);
	} else {
		deathmatch_OnPlayerSpawn(playerid, dmid);
	}

	SetTimerEx("OnPlayerSpawned", 2500, 0, "d", playerid);
	return 1;
}

forward OnPlayerSpawned(playerid);
public OnPlayerSpawned(playerid)
{
	player_SetSpawned(playerid, 1);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	player_SetSpawned(playerid, 0);
	player_OnPlayerRequestClass(playerid, classid);
	pl_weapon_OnPlayerRequestClass(playerid, classid);
	pl_level_OnPlayerRequestClass(playerid, classid);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if (!player_IsLogin(playerid)) return 0;
	return 1;
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if (!player_IsLogin(playerid)) {
		SendClientMessage(playerid, -1, lang_texts[1][46]);
		return 0;
	}
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	if (success) {
		new logstring[MAX_STRING];
		format(logstring, sizeof(logstring), "Player: %s"CHAT_SHOW_ID": %s", oGetPlayerName(playerid), playerid, cmdtext);
		WriteLog(CMDLog, logstring);
		return 1;
	}
	return 0;
}

public OnPlayerText(playerid, text[])
{
	if (!player_IsLogin(playerid)) {
		SendClientMessage(playerid, -1, lang_texts[1][46]);
		return 0;
	}

	if (pt_chat_OnPlayerText(playerid, text) == 0) {
		return 0;
	}

	new playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	
	new string[MAX_STRING];
	switch (text[0])
	{
		case '!':
		{
			if (GetPVarInt(playerid, "GangID") == 0 || player_IsMuted(playerid))
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
			if (player_IsMuted(playerid))
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

	new adm_result = admin_OnPlayerText(playerid, text);
	if (adm_result == 0) {
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
		if ( fights_IsPlayerAtTeacher(playerid) ) return fights_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtFastFood(playerid) ) return fastfood_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtBar(playerid) ) return bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		if ( IsPlayerAtSkinShop(playerid) ) return sshop_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		
		new player_state = GetPlayerState(playerid);
		switch (player_state) {
			case PLAYER_STATE_ONFOOT: {
				Dialog_Show(playerid, Dialog:PlayerMenu);
			}
			case PLAYER_STATE_DRIVER: {
				Dialog_Show(playerid, Dialog:VehicleMenu);
			}
		}
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
	vehicles_OnPlayerStateChange(playerid, newstate, oldstate);
	pt_speed_OnPlayerStateChange(playerid, newstate, oldstate);
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
	admin_OnPlayerExitVehicle(playerid, vehicleid);
	pt_weapon_OnPlayerExitVehicle(playerid, vehicleid);
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	pt_speed_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	admin_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	admin_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
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
	switch (success) {
		case 0: {
			pt_rcon_OnRconLoginAttempt(ip, password, success);
		}
		case 1: {
			// если игрок заходит ркон админом, то дадим ему полные права
			new pip[MAX_IP];

			foreach (new playerid: Player) {
				GetPVarString(playerid, "IP", pip, sizeof(pip));

				if (!strcmp(ip, pip, false)) {
					player_SetStatus(playerid, STATUS_LEVEL_RCON);
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

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	admin_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
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

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (hittype == BULLET_HIT_TYPE_VEHICLE) {
		vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	}
	return 1;
}