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
	0.3.7 R2 and older

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

// configuration
#include "config.inc"
#include "core/unfixes.inc"

// important lib
#include "lib/sa-mp-fixes/fixes"

// header files
#include "gang/gang.inc"
#include "player/player.inc"
#include "player/player_account.inc"
#include "player/player_spawn.inc"
#include "player/player_weapon.inc"
#include "player/player_money.inc"
#include "protections/armour.inc"
#include "protections/health.inc"
#include "protections/specialaction.inc"
#include "protections/teleport.inc"

// lib
#include "lib/streamer.inc"
#include "lib/sa-mp-foreach/foreach.inc"
#include "lib/sscanf2.inc"
#include "lib/zcmd.inc"
#include "lib/mxINI.inc"
#include "lib/time_t/time_t.inc"
#include "lib/mSelection.inc"
#include "lib/mapfix.inc"
#include "lib/textlist/textlist.inc"
#include "lib/shootingrange/shootingrange.inc"
#include "lib/zlang/zlang.inc"
#include "lib/zvehcomp/zvehcomp.inc"
#include "lib/zvehpaintjob/zvehpaintjob.inc"
#include "lib/zvehgetcolor/zvehgetcolor.inc"
#include "lib/mdialog/mdialog.inc"
#include "lib/skinselect/skinselect.inc"
#include "lib/GarageBlock.inc"
#include "lib/gpickup/gpickup.inc"
#include "lib/gtolib/gtolib.inc"
#include "lib/getspawninfo/getspawninfo.inc"
#include "lib/getweather/getweather.inc"

// core
#include "core/api.pwn"
#include "core/db.pwn"
#include "core/cfg.pwn"
#include "core/core.pwn"
#include "core/core_time.pwn"
#include "core/lang.pwn"
#include "core/log.pwn"
#include "core/color.pwn"
#include "core/widestrip.pwn"
#include "core/declension.pwn"
#include "core/spectate.pwn"
#include "core/zones.pwn"

// vehicle
#include "vehicle/vehicle_fuel.pwn"
#include "vehicle/vehicle.pwn"
#include "vehicle/vehicle_menu.pwn"
#include "vehicle/vehicle_info.pwn"
#include "vehicle/vehicle_damage.pwn"
#include "vehicle/vehicle_radio.pwn"
#include "vehicle/vehicle_premium.pwn"
#include "vehicle/vehicle_textdraw.pwn"
#include "vehicle/vehicle_doors.pwn"

// gang
#include "gang/gang_member.pwn"
#include "gang/gang_level.pwn"
#include "gang/gang_menu.pwn"
#include "gang/gang.pwn"

// player
#include "player/player_level.pwn"
#include "player/player_premium.pwn"
#include "player/player_weapon.pwn"
#include "player/player_weapon_drop.pwn"
#include "player/player_weapon_skill.pwn"
#include "player/player_health.pwn"
#include "player/player_fights.pwn"
#include "player/player_vehicle.pwn"
#include "player/player_status.pwn"
#include "player/player_account.pwn"
#include "player/player_spawn.pwn"
#include "player/player_login.pwn"
#include "player/player_pm.pwn"
#include "player/player_mute.pwn"
#include "player/player_ip.pwn"
#include "player/player_jail.pwn"
#include "player/player_report.pwn"
#include "player/player_click.pwn"
#include "player/player_menu.pwn"
#include "player/player_menu_vehicle.pwn"
#include "player/player_menu_teleport.pwn"
#include "player/player_menu_settings.pwn"
#include "player/player_menu_anims.pwn"
#include "player/player_quest.pwn"
#include "player/player_commands.pwn"
#include "player/player_kick.pwn"
#include "player/player_maptp.pwn"
#include "player/player_text.pwn"
#include "player/player_money.pwn"
#include "player/player_money_td.pwn"
#include "player/player_gang.pwn"
#include "player/player_skydive.pwn"
#include "player/message/message_alert.pwn"
#include "player/message/message_objective.pwn"
#include "player/player.pwn"

// custom
#include "custom/ls_beachside.pwn"
#include "custom/pickups.pwn"

// system
#include "system/weapons.pwn"
#include "system/world.pwn"
#include "system/housing.pwn"
#include "system/business.pwn"
#include "system/payday.pwn"
#include "system/groundhold.pwn"
#include "system/premium.pwn"
#include "system/click.pwn"
#include "system/enterexit.pwn"
#include "system/weather.pwn"

// admin
#include "admin/admin_commands.pwn"
#include "admin/admin_commands/cmd_announce.pwn"
#include "admin/admin_commands/cmd_armour.pwn"
#include "admin/admin_commands/cmd_camera.pwn"
#include "admin/admin_commands/cmd_health.pwn"
#include "admin/admin_commands/cmd_level.pwn"
#include "admin/admin_commands/cmd_money.pwn"
#include "admin/admin_commands/cmd_premium.pwn"
#include "admin/admin_commands/cmd_say.pwn"
#include "admin/admin_commands/cmd_vehicle.pwn"
#include "admin/admin_commands/cmd_weather.pwn"
#include "admin/admin_ban.pwn"
#include "admin/admin_mute.pwn"
#include "admin/admin_jail.pwn"
#include "admin/admin_spec.pwn"
#include "admin/admin_godmod.pwn"
#include "admin/admin_pm.pwn"
#include "admin/admin_hide.pwn"
#include "admin/admin_maptp.pwn"
#include "admin/admin_click.pwn"
#include "admin/admin_login.pwn"
#include "admin/admin.pwn"

// mission
#include "missions/missions.pwn"
#include "missions/trucker.pwn"
#include "missions/swagup.pwn"

// service
#include "services/bank.pwn"
#include "services/fastfood.pwn"
#include "services/bar.pwn"
#include "services/skinshop.pwn"
#include "services/lottery.pwn"
#include "services/vehshop.pwn"
#include "services/weaponshop.pwn"
#include "services/fightteacher.pwn"
#include "services/fuelstation.pwn"
#include "services/tuning.pwn"

// protection
#include "protections/idle.pwn"
#include "protections/rconhack.pwn"
#include "protections/highping.pwn"
#include "protections/chatguard.pwn"
#include "protections/specialaction.pwn"
#include "protections/teleport.pwn"
#include "protections/weaponhack.pwn"
#include "protections/health.pwn"
#include "protections/armour.pwn"
#include "protections/vehicleteleport.pwn"

main() {}

public OnGameModeInit()
{
	// Initialize everything that needs it
	cfg_LoadConfigs();
	lang_OnGameModeInit();
	Lang_OnGameModeInit();

	Core_OnGameModeInit();
	Vehicle_OnGameModeInit();
	Vehicle_Textdraw_OnGameModeInit();
	GangMember_OnGameModeInit();
	GangLevel_OnGameModeInit();
	Groundhold_OnGameModeInit();
	business_OnGameModeInit();
	housing_OnGameModeInit();
	Enterexit_OnGameModeInit();
	weapon_OnGameModeInit();
	Premium_OnGameModeInit();
	pl_weapon_OnGameModeInit();
	pl_money_td_OnGameModeInit();
	pl_level_OnGameModeInit();
	widestrip_OnGameModeInit();

	// custom
	Beachside_OnGameModeInit();
	Custom_OnGameModeInit();

	// missions
	Trucker_OnGameModeInit();
	swagup_OnGameModeInit();

	// services
	fastfood_OnGameModeInit();
	bar_OnGameModeInit();
	sshop_OnGameModeInit();
	vshop_OnGameModeInit();
	wshop_OnGameModeInit();
	Fuelstation_OnGameModeInit();
	bank_OnGameModeInit();
	fights_OnGameModeInit();
	Tuning_OnGameModeInit();
	Lottery_OnGameModeInit();

	// protection
	pt_idle_OnGameModeInit();
	pt_chat_OnGameModeInit();
	Prot_Teleport_OnGameModeInit();

	// custom
	#tryinclude "custom/mapicon"
	#tryinclude "custom/objects"
	Log_Game("SERVER: Custom mapicons, objects and pickups init");

	Time_Sync();
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
	pl_money_td_OnPlayerConnect(playerid);
	Vehicle_Textdraw_OnPlayerConn(playerid);
	Enterexit_OnPlayerConnect(playerid);
	Spectate_OnPlayerConnect(playerid);
	Beachside_OnPlayerConnect(playerid);
	Tuning_OnPlayerConnect(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (playerid == INVALID_PLAYER_ID || IsPlayerNPC(playerid)) {
		return 1;
	}
	player_OnPlayerDisconnect(playerid, reason);
	Trucker_OnPlayerDisconnect(playerid, reason);
	pt_chat_OnPlayerDisconnect(playerid, reason);
	Groundhold_OnPlayerDisconnect(playerid, reason);
	pl_weapon_OnPlayerDisconnect(playerid, reason);
	pl_money_td_OnPlayerDisconnect(playerid, reason);
	pveh_OnPlayerDisconnect(playerid, reason);
	Player_SetSpawned(playerid, 0);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	click_OnPlayerClickPlayer(playerid, clickedplayerid);
	return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
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
	if (Enterexit_OnPlayerPickUpPickup(playerid, pickupid)) {
		return 1;
	}
	return 1;
}

public OnPlayerPickUpGlobalPickup(playerid, pickupid, gpickupid, model)
{
	if (Custom_OnPlayerPickUpGP(playerid, pickupid, gpickupid, model)) {
		return 1;
	}
	return 1;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if (Trucker_OnPlayerEnterCheckpoint(playerid, checkpointid)) {
		return 1;
	}
	if (wshop_OnPlayerEnterCheckpoint(playerid, checkpointid)) {
		return 1;
	}
	if (bar_OnPlayerEnterCheckpoint(playerid, checkpointid)) {
		return 1;
	}
	if (fastfood_OnPlayerEnterCP(playerid, checkpointid)) {
		return 1;
	}
	if (ss_OnPlayerEnterCheckpoint(playerid, checkpointid)) {
		return 1;
	}
	if (fights_OnPlayerEnterCheckpoint(playerid, checkpointid)) {
		return 1;
	}
	if (bank_OnPlayerEnterCheckpoint(playerid, checkpointid)) {
		return 1;
	}
	return 1;
}

public OnPlayerEnterDynamicRaceCP(playerid, checkpointid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid) || IsPlayerNPC(killerid)) {
		return 1;
	}

	if (!pt_weapon_OnPlayerDeath(playerid, killerid, reason)) {
		return 1;
	}

	Player_SetSpawned(playerid, 0);

	if (killerid == INVALID_PLAYER_ID) {
		Log_Game("player: %s(%d): has died > Reason: (%d)", ReturnPlayerName(playerid), playerid, reason);
	} else {
		Log_Game("player: %s(%d): has killed player %s(%d)> Reason: (%d)", ReturnPlayerName(killerid), killerid, ReturnPlayerName(playerid), playerid, reason);
	}

	SendDeathMessage(killerid, playerid, reason);
	
	player_OnPlayerDeath(playerid, killerid, reason);
	Trucker_OnPlayerDeath(playerid, killerid, reason);
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

	// protections
	Prot_Teleport_OnPlayerSpawn(playerid);

	// ставим позицию
	SetPlayerPosToSpawn(playerid);

	// spawn player
	player_OnPlayerSpawn(playerid);
	weapon_OnPlayerSpawn(playerid);
	Spectate_OnPlayerSpawn(playerid);

	SetTimerEx("OnPlayerSpawned", 1000, 0, "d", playerid);
	return 1;
}

forward OnPlayerSpawned(playerid);
public OnPlayerSpawned(playerid)
{
	Player_SetSpawned(playerid, 1);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	Player_SetSpawned(playerid, 0);
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
	
	new pl_result = pl_text_OnPlayerText(playerid, text);
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
	if (bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (Enterexit_OnPlayerKeyStateChang(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (Beachside_OnPlayerKeySC(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (housing_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (business_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (PMenu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (Trucker_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (Tuning_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (Fuelstation_OnPlayerKeyStateCh(playerid, newkeys, oldkeys)) {
		return 1;
	}

	if (VMenu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
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
	Vehicle_OnPlayerStateChange(playerid, newstate, oldstate);
	Trucker_OnPlayerStateChange(playerid, newstate, oldstate);
	Spectate_OnPlayerStateChange(playerid, newstate, oldstate);
	weapon_OnPlayerStateChange(playerid, newstate, oldstate);
	Fuelstation_OnPlayerStateChange(playerid, newstate, oldstate);

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
	Vehicle_OnPlayerExitVehicle(playerid, vehicleid);
	pt_weapon_OnPlayerExitVehicle(playerid, vehicleid);
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
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
	Vehicle_Fuel_OnVehicleSpawn(vehicleid);
	vshop_OnVehicleSpawn(vehicleid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	Trucker_OnVehicleDeath(vehicleid, killerid);
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

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	weapon_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	pl_weapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	pt_weapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);

	if (hittype == BULLET_HIT_TYPE_VEHICLE) {
		Vehicle_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	}
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 0;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 0;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 0;
}

public OnVehicleTuningPaintjob(playerid, vehicleid, paintjobid)
{
	pveh_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
	return 1;
}

public OnVehicleTuningRespray(playerid, vehicleid, color1, color2)
{
	pveh_OnVehicleRespray(playerid, vehicleid, color1, color2);
	return 1;
}

public OnVehicleTuning(playerid, vehicleid, componentid)
{
	pveh_OnVehicleTuning(playerid, vehicleid, componentid);
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
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

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	return pt_vehtp_OnUnoccupiedVehicleU(vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
}

public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	if (Tuning_OnPlayerEnterDynamicArea(playerid, areaid)) {
		return 1;
	}

	if (Groundhold_OnPlayerEnterDynArea(playerid, areaid)) {
		return 1;
	}

	if (Fuelstation_OnPlayerEnterDyArea(playerid, areaid)) {
		return 1;
	}

	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	if (Tuning_OnPlayerLeaveDynamicArea(playerid, areaid)) {
		return 1;
	}

	if (Groundhold_OnPlayerLeaveDynArea(playerid, areaid)) {
		return 1;
	}

	if (Fuelstation_OnPlayerLeaveDyArea(playerid, areaid)) {
		return 1;
	}

	return 1;
}

public OnVehicleFilled(vehicleid, playerid, money)
{
	if (Fuelstation_OnVehicleFilled(vehicleid, playerid, money)) {
		return 1;
	}

	return 1;
}
