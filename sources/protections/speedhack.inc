/*

	About: anti speed hack
	Author:	ziggi

*/

#if defined _speedhack_included
	#endinput
#endif

#define _speedhack_included
#pragma library speedhack


/*
 * Vars
 */

static
	IsEnabled = ANTI_SPEED_HACK_ENABLED,
	CheckTime = ANTI_SPEED_HACK_CHECK_TIME;


/*
 * Config
 */

pt_speed_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Speed_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Speed_CheckTime", CheckTime);
}

pt_speed_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Speed_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Speed_CheckTime", CheckTime);
}


/*
 * For public
 */

pt_speed_OnGameModeInit()
{
	if (IsEnabled) {
		SetTimer("pt_speed_Timer", CheckTime, 1);
	}
	return 1;
}

pt_speed_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (!IsEnabled) {
		return 0;
	}

	// если выпрыгнул из транспорта
	if (newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER) {
		pt_speed_Disable(playerid, 3000);
	}
	return 1;
}

pt_speed_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	switch (oldinteriorid) {
		case 1, 2, 3: {
			pt_speed_Disable(playerid, 1000);
		}
	}

	switch (newinteriorid) {
		case 1, 2, 3: {
			pt_speed_Disable(playerid, 1000);
		}
	}
	return 1;
}


/*
 * SetPlayerPos
 */

stock pt_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	pt_speed_Disable(playerid, 1000);
	SetPVarFloat(playerid, "ac_old_Coord_X", x);
	SetPVarFloat(playerid, "ac_old_Coord_Y", y);
	ORIG_SetPlayerPos(playerid, x, y, z);
	return 1;
}


/*
 * SetVehiclePos
 */

stock pt_SetVehiclePos(vehicleid, Float:x, Float:y, Float:z)
{
	foreach (new playerid : Player) {
		if (GetPlayerVehicleID(playerid) == vehicleid) {
			SetPVarFloat(playerid, "ac_old_Coord_X", x);
			SetPVarFloat(playerid, "ac_old_Coord_Y", y);
		}
	}

	ORIG_SetVehiclePos(vehicleid, x, y, z);
	return 1;
}


/*
 * GetPlayerSpeed
 */

stock Float:GetPlayerSpeed(playerid)
{
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	new Float:Dis = floatsqroot(
		floatpower( floatabs( floatsub(GetPVarFloat(playerid,"ac_old_Coord_X"), pos[0]) ), 2 )
		+
		floatpower( floatabs( floatsub(GetPVarFloat(playerid,"ac_old_Coord_Y"), pos[1]) ), 2 )
	);

	SetPVarFloat(playerid, "ac_old_Coord_X", pos[0]);
	SetPVarFloat(playerid, "ac_old_Coord_Y", pos[1]);
	return (Dis * 2.0);
}


/*
 * For timer
 */

forward pt_speed_Timer();
public pt_speed_Timer()
{
	if (!IsEnabled) {
		return 0;
	}

	foreach (new playerid : Player) {
		pt_speed_Sync(playerid);
	}
	return 1;
}

stock pt_speed_Disable(playerid, time)
{
	SetPVarInt(playerid, "ac_NotSpeedHack", 1);
	SetTimerEx("pt_speed_NotHack", time, 0, "d", playerid);
}

forward pt_speed_NotHack(playerid);
public pt_speed_NotHack(playerid)
{
	SetPVarInt(playerid, "ac_NotSpeedHack", 0);
}

pt_speed_Sync(playerid)
{
	if (!player_IsSpawned(playerid) || GetPVarInt(playerid, "ac_NotSpeedHack") == 1) {
		new Float:pos[3];
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

		SetPVarFloat(playerid, "ac_old_Coord_X", pos[0]);
		SetPVarFloat(playerid, "ac_old_Coord_Y", pos[1]);
		return 0;
	}

	new Float:player_speed = GetPlayerSpeed(playerid);
	
	switch ( GetPlayerState(playerid) ) {
		case PLAYER_STATE_ONFOOT: {
			new max_speed = 50;
			
			if (player_speed > max_speed) {
				switch (floatround(player_speed, floatround_round)) {
					// 26 - максимум на земле, 50 при полёте с парашютом
					case 50..349: {
						Admin_SendProtectReport(playerid, _(PROTECTION_SPEED_SPEEDHACK), player_speed, max_speed);
					}
					case 350..499: {
						Admin_SendProtectReport(playerid, _(PROTECTION_SPEED_AIRBREAK), player_speed, max_speed);
					}
					default: {
						Admin_SendProtectReport(playerid, _(PROTECTION_SPEED_TELEPORT), player_speed, max_speed);
					}
				}
			}
		}
		case PLAYER_STATE_DRIVER: {
			new max_speed = GetMaxVehicleModelSpeed( GetVehicleModel( GetPlayerVehicleID(playerid) ) );
			
			if (player_speed > max_speed) {
				switch ( floatround(player_speed, floatround_round) ) {
					case 151..250: {
						Admin_SendProtectReport(playerid, _(PROTECTION_SPEED_SPEEDHACK), player_speed, max_speed);
					}
					case 251..499: {
						Admin_SendProtectReport(playerid, _(PROTECTION_SPEED_AIRBREAK), player_speed, max_speed);
					}
					default: {
						Admin_SendProtectReport(playerid, _(PROTECTION_SPEED_TELEPORT), player_speed, max_speed);
					}
				}
			}
		}
	}
	return 1;
}
