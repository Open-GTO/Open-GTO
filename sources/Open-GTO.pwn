/*
Project Name:
	Open - Grand Theft Online (Open-GTO)

Web site:
	http://open-gto.ru/
	https://github.com/ziggi/Open-GTO

Date started:
	Open-GTO:
		5 November 2009
	GTO:
		12 August 2006

SA-MP version:
	0.3.7 RC3 and older

Developers:
	Open-GTO:
		0.6.4 Ц current		ziggi
		0.6.0 Ц 0.6.4		GhostTT, heufix, Elbi, ziggi

	GTO:
		0.5.9 Ц 0.6.0		SCALOLaz
		0.5.8 Ц 0.5.9		Robin Kikkert (dejavu), Lajos Pacsek (Asturel)
		0.5.7 Ц 0.5.8		Peter Steenbergen (j1nx)
		0.0.0 Ц 0.5.7		Iain Gilbert

Thanks:
	MX_Master - mxINI, Chat-Guard.
	wups - Default map icons.
	Y_Less - foreach, fixes.
	ZeeX - zcmd.
	Dmitry Borisoff (Beginner) - Russian translation.

*/

#include <a_samp>
#include "config"
#include "core/unfixes"
#include "lib/sa-mp-fixes/fixes"
#include "lib/sa-mp-foreach/foreach"
#include "lib/zcmd"
#include "lib/mxINI"
#include "lib/mxDate"
#include "lib/mSelection"
#include "core/redefine"
#include "core/db"
#include "core/core_time"
#include "core/dialog"
#include "core/lang"
#include "core/zlang"
#include "core/log"
#include "lib/gtolib"
#include "core/color"
#include "core/widestrip"
#include "core/declension"
#include "core/shootingrange"
#include "core/spectate"
#include "vehicle/vehicles"
#include "vehicle/vehicle_fuel"
#include "vehicle/vehicle_menu"
#include "vehicle/vehicle_info"
#include "vehicle/vehicle_damage"
#include "vehicle/vehicle_radio"
#include "vehicle/vehicle_premium"
#include "vehicle/vehicle_component"
#include "player/player_textdraw"
#include "player/player_level"
#include "player/player_premium"
#include "player/player_weapon"
#include "player/player_weapon_drop"
#include "player/player_weapon_skill"
#include "player/player_health"
#include "player/player_fights"
#include "player/player_vehicle"
#include "player/player_status"
#include "player/player_account"
#include "player/player_login"
#include "player/player_spawn"
#include "player/player_pm"
#include "player/player_mute"
#include "player/player_jail"
#include "player/player_report"
#include "player/player_click"
#include "player/player_menu"
#include "player/player_menu_vehicle"
#include "player/player_menu_teleport"
#include "player/player_menu_settings"
#include "player/player_menu_anims"
#include "player/player_quest"
#include "player/player_commands"
#include "player/player_kick"
#include "player/player_maptp"
#include "player/player_message"
#include "player/player_money"
#include "player/player_alert"
#include "player/player"
#include "custom/pickups"
#include "custom/ls_beachside"
#include "system/weapons"
#include "core/zones"
#include "system/world"
#include "gang/gang"
#include "gang/gang_menu"
#include "gang/gang_level"
#include "system/housing"
#include "system/business"
#include "streamers/incognito_streamer"
#include "streamers/mapicon_stream"
#include "streamers/checkpoint_stream"
#include "system/race"
#include "system/deathmatch"
#include "system/payday"
#include "system/groundhold"
#include "system/premium"
#include "admin/admin_commands"
#include "admin/admin_ban"
#include "admin/admin_mute"
#include "admin/admin_jail"
#include "admin/admin_spec"
#include "admin/admin_godmod"
#include "admin/admin_pm"
#include "admin/admin_hide"
#include "admin/admin_maptp"
#include "admin/admin_click"
#include "admin/admin_login"
#include "admin/admin"
#include "missions/missions"
#include "missions/trucker"
#include "missions/swagup"
#include "system/click"
#include "system/interior"
#include "services/bank"
#include "services/fastfood"
#include "services/bar"
#include "services/skinshop"
#include "services/lottery"
#include "services/vehshop"
#include "services/weaponshop"
#include "services/fightteacher"
#include "services/fuelstation"
#include "system/weather"
#include "protections/idle"
#include "protections/rconhack"
#include "protections/highping"
#include "protections/chatguard"
#include "protections/jetpack"
#include "protections/speedhack"
#include "protections/weaponhack"
#include "protections/health"
#include "protections/armour"

#include "core/cfg"

#include "core/api"
// Races
#tryinclude "races/race_monstertruck"
#tryinclude "races/race_thestrip"
#tryinclude "races/race_riversiderun"
#tryinclude "races/race_fleethecity"
#tryinclude "races/race_lostinsmoke"
#tryinclude "races/race_backstreetbang"
#tryinclude "races/race_flyingfree"
#tryinclude "races/race_murderhorn"
#tryinclude "races/race_roundwego"
#tryinclude "races/race_striptease"
#tryinclude "races/race_countrycruise"
#tryinclude "races/race_thegrove"
#tryinclude "races/race_mullholland"
#tryinclude "races/race_julius"
#tryinclude "races/race_trailer"
#tryinclude "races/race_fuckinwood"
#tryinclude "races/race_majestic"
#tryinclude "races/race_toarea51"
#tryinclude "races/race_mountchilliad"
#tryinclude "races/race_m25"
#tryinclude "races/race_vinewood"
#tryinclude "races/race_fromsftolv"
#tryinclude "races/race_annoying"
#tryinclude "races/race_roadtohell"
#tryinclude "races/race_bayside_tour"

// Deathmatches
#tryinclude "deathmatches/dm_air"
#tryinclude "deathmatches/dm_area51"
#tryinclude "deathmatches/dm_badandugly"
#tryinclude "deathmatches/dm_bluemountains"
#tryinclude "deathmatches/dm_cargoship"
#tryinclude "deathmatches/dm_dildo"
#tryinclude "deathmatches/dm_mbase"
#tryinclude "deathmatches/dm_minigunmadness"
#tryinclude "deathmatches/dm_poolday"
#tryinclude "deathmatches/dm_usnavy"

main() {}

public OnGameModeInit()
{
	// Initialize everything that needs it
	cfg_LoadConfigs();
	lang_OnGameModeInit();
	Lang_OnGameModeInit();

	config_OnGameModeInit();
	vehicles_OnGameModeInit();
	race_OnGameModeInit();
	deathmatch_OnGameModeInit();
	groundhold_OnGameModeInit();
	business_OnGameModeInit();
	housing_OnGameModeInit();
	Interior_OnGameModeInit();
	weapon_OnGameModeInit();
	Premium_OnGameModeInit();
	pl_weapon_OnGameModeInit();
	pl_textdraw_OnGameModeInit();
	pl_alert_OnGameModeInit();
	Pickup_OnGameModeInit();
	widestrip_OnGameModeInit();
	Beachside_OnGameModeInit();

	// missions
	trucker_OnGameModeInit();
	swagup_OnGameModeInit();

	// services
	fastfood_OnGameModeInit();
	bar_OnGameModeInit();
	sshop_OnGameModeInit();
	vshop_OnGameModeInit();
	wshop_OnGameModeInit();
	fuelstation_OnGameModeInit();
	bank_OnGameModeInit();
	fights_OnGameModeInit();

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
	race_julius_init();
	race_trailer_init();
	race_fuckinwood_init();
	race_majestic_init();
	race_toarea51_init();
	race_mountchilliad_init();
	race_m25_init();
	race_vinewood_init();
	race_fromsftolv_init();
	race_annoying_init();
	race_roadtohell_init();
	race_bayside_tour_init();
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

	#tryinclude "custom/mapicon"
	#tryinclude "custom/objects"
	Log_Game("SERVER: Custom mapicons, objects and pickups init");

	time_Sync();
	weather_SetRandom();
	SetTimer("OneSecTimer", 1000, 1); // 1 second
	SetTimer("FiveSecondTimer", 5000, 1); // 5 second
	SetTimer("OneMinuteTimer", 60000, 1); // 1 minute
	SetTimer("TenMinuteTimer", 600000, 1); // 10 minute
	SetTimer("OneHourTimer", 3600000, 1); // 1 hour
	SetTimerEx("WorldSave", WORLD_SAVE_TIME, 1, "d", 0);
	Log_Game("SERVER: Timers started");
	
	WorldSave(0);
	Log_Game("SERVER: " GAMEMODE_TEXT " initialization complete.");
	return 1;
}

public OnGameModeExit()
{
	WorldSave(1);
	Log_Game("SERVER: " GAMEMODE_TEXT " turned off.");
	return 1;
}

public OnPlayerConnect(playerid)
{
	if (IsPlayerNPC(playerid)) {
		return 1;
	}
	player_OnPlayerConnect(playerid);
	pt_chat_OnPlayerConnect(playerid);
	pl_weapon_OnPlayerConnect(playerid);
	pl_textdraw_OnPlayerConnect(playerid);
	pl_alert_OnPlayerConnect(playerid);
	veh_fuel_OnPlayerConnect(playerid);
	Mapicon_OnPlayerConnect(playerid);
	Checkpoint_OnPlayerConnect(playerid);
	Interior_OnPlayerConnect(playerid);
	Spectate_OnPlayerConnect(playerid);
	Beachside_OnPlayerConnect(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (playerid == INVALID_PLAYER_ID || IsPlayerNPC(playerid)) {
		return 1;
	}
	player_OnPlayerDisconnect(playerid, reason);
	trucker_OnPlayerDisconnect(playerid, reason);
	pt_chat_OnPlayerDisconnect(playerid, reason);
	gh_OnPlayerDisconnect(playerid, reason);
	pl_weapon_OnPlayerDisconnect(playerid, reason);
	pl_textdraw_OnPlayerDisconnect(playerid, reason);
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
	if (pl_weapon_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	if (swagup_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	if (Premium_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	if (Pickup_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	if (Interior_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	dm_OnPlayerEnterCheckpoint(playerid);

	new cp = Checkpoint_GetPlayerVisible(playerid);

	if (trucker_OnPlayerEnterCheckpoint(playerid, cp)) {
		return 1;
	}
	if (wshop_OnPlayerEnterCheckpoint(playerid, cp)) {
		return 1;
	}
	if (bar_OnPlayerEnterCheckpoint(playerid, cp)) {
		return 1;
	}
	if (fastfood_OnPlayerEnterCP(playerid, cp)) {
		return 1;
	}
	if (ss_OnPlayerEnterCheckpoint(playerid, cp)) {
		return 1;
	}
	if (fights_OnPlayerEnterCheckpoint(playerid, cp)) {
		return 1;
	}
	if (bank_OnPlayerEnterCheckpoint(playerid, cp)) {
		return 1;
	}
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	race_OnPlayerEnterCheckpoint(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid) || IsPlayerNPC(killerid)) {
		return 1;
	}

	player_SetSpawned(playerid, 0);

	if (killerid == INVALID_PLAYER_ID) {
		Log_Game("player: %s(%d): has died > Reason: (%d)", ReturnPlayerName(playerid), playerid, reason);
	} else {
		Log_Game("player: %s(%d): has killed player %s(%d)> Reason: (%d)", ReturnPlayerName(killerid), killerid, ReturnPlayerName(playerid), playerid, reason);
	}

	SendDeathMessage(killerid, playerid, reason);
	
	if (IsPlayerInAnyDM(playerid)) {
		deathmatch_OnPlayerDeath(playerid, killerid);
		deathmatch_OnPlayerKill(killerid, playerid, reason);
		return 1;
	}
	
	player_OnPlayerDeath(playerid, killerid, reason);
	trucker_OnPlayerDeath(playerid, killerid, reason);
	gang_OnPlayerDeath(playerid, killerid, reason);
	pl_weapon_OnPlayerDeath(playerid, killerid, reason);

	PlayCrimeReportForPlayer(killerid, killerid, random(18)+3);
	PlayCrimeReportForPlayer(playerid, killerid, random(18)+3);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if (IsPlayerNPC(playerid)) {
		return 1;
	}

	// ставим позицию
	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID || DMPlayerStats[playerid][dm_player_active] == 0) {
		pl_spawn_SetSpawnPos(playerid);
	} else {
		deathmatch_OnPlayerSpawn(playerid, dmid);
	}

	// spawn player
	player_OnPlayerSpawn(playerid);
	Spectate_OnPlayerSpawn(playerid);

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
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if (!player_IsLogin(playerid)) {
		return 0;
	}
	return 1;
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if (!player_IsLogin(playerid)) {
		SendClientMessage(playerid, -1, _(ACCOUNT_LOGIN_FIRST));
		return 0;
	}
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	if (success) {
		Log_Player(_(PLAYER_COMMAND_LOG), ReturnPlayerName(playerid), playerid, cmdtext);
		return 1;
	}
	return 0;
}

public OnPlayerText(playerid, text[])
{
	if (!player_IsLogin(playerid)) {
		SendClientMessage(playerid, -1, _(ACCOUNT_LOGIN_FIRST));
		return 0;
	}

	new pt_result = pt_chat_OnPlayerText(playerid, text);
	if (pt_result == 0) {
		return 0;
	}
	
	new pl_result = pl_message_OnPlayerText(playerid, text);
	if (pl_result == 0) {
		return 0;
	}

	new adm_result = admin_OnPlayerText(playerid, text);
	if (adm_result == 0) {
		return 0;
	}

	new playername[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, playername, sizeof(playername));

	new string[MAX_STRING];
	format(string, sizeof(string), "%s(%d): {FFFFFF}%s", playername, playerid, text);
	SendClientMessageToAll(GetPlayerColor(playerid), string);

	Log_Player("Player: %s(%d): %s", playername, playerid, text);
	return 0;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (PRESSED(KEY_FIRE) || PRESSED(KEY_SECONDARY_ATTACK)) {
		if ( GetPVarInt(playerid, "bar_Drinking") == 1 ) {
			return bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		}
	}

	if (PRESSED(KEY_USING)) {
		if (Interior_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
			return 1;
		}

		if (Beachside_OnPlayerKeySC(playerid, newkeys, oldkeys)) {
			return 1;
		}

		if (IsPlayerAtHouse(playerid)) {
			return housing_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		}

		if (IsPlayerAtBusiness(playerid)) {
			return business_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		}
		
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

	if (PRESSED(KEY_SUBMISSION)) {
		new vehicleid = GetPlayerVehicleID(playerid);
		if ( vehicleid != 0 && IsVehicleIsRunner(vehicleid) ) {
			trucker_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
		}
		return 1;
	}

	if (PRESSED(KEY_ANALOG_UP) || PRESSED(KEY_ANALOG_DOWN) || PRESSED(KEY_ANALOG_LEFT) || PRESSED(KEY_ANALOG_RIGHT)) {
		new player_state = GetPlayerState(playerid);
		switch (player_state) {
			case PLAYER_STATE_DRIVER: {
				vh_menu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
			}
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
	trucker_OnPlayerStateChange(playerid, newstate, oldstate);
	Spectate_OnPlayerStateChange(playerid, newstate, oldstate);

	if (newstate == PLAYER_STATE_DRIVER) {
		Premium_OnPlayerStateChange(playerid, newstate, oldstate);
	}

	if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER) {
		vshop_OnPlayerStateChange(playerid, newstate, oldstate);
	}
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	vehicles_OnPlayerExitVehicle(playerid, vehicleid);
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	pt_speed_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	Spectate_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
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
	veh_fuel_OnVehicleSpawn(vehicleid);
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
	admin_OnRconLoginAttempt(ip, password, success);
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
	vehicles_OnEnterExitModShop(playerid, enterexit, interiorid);
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

public OnPlayerModelSelectionEx(playerid, response, extraid, modelid)
{
	if (extraid == ms_skinshop) {
		sshop_OnPlayerModelSelectionEx(playerid, response, extraid, modelid);
	}
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (hittype == BULLET_HIT_TYPE_VEHICLE) {
		vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	}
	return 1;
}

public OnActorStreamIn(actorid, forplayerid)
{
	if (bar_OnActorStreamIn(actorid, forplayerid)) {
		return 1;
	}
	if (fastfood_OnActorStreamIn(actorid, forplayerid)) {
		return 1;
	}
	if (bank_OnActorStreamIn(actorid, forplayerid)) {
		return 1;
	}
	if (wshop_OnActorStreamIn(actorid, forplayerid)) {
		return 1;
	}
	return 1;
}

public OnObjectMoved(objectid)
{
	Beachside_OnObjectMoved(objectid);
	return 1;
}

public OnPlayerSpectate(playerid, specid, status)
{
	adm_spec_OnPlayerSpectate(playerid, specid, status);
	return 1;
}