/*

	About: anti speed hack
	Author:	ziggi

*/

#if defined _speedhack_included
	#endinput
#endif

#define _speedhack_included
#pragma library speedhack


static
	IsEnabled = ANTI_SPEED_HACK_ENABLED,
	CheckTime = ANTI_SPEED_HACK_CHECK_TIME;

stock pt_speed_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Speed_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Speed_CheckTime", CheckTime);
}

stock pt_speed_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Speed_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Speed_CheckTime", CheckTime);
}

stock pt_speed_OnGameModeInit()
{
	if (IsEnabled) {
		SetTimer("pt_speed_Timer", CheckTime, 1);
	}
	return 1;
}

stock pt_speed_OnPlayerStateChange(playerid, newstate, oldstate)
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

stock pt_speed_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
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

stock oSetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	pt_speed_Disable(playerid, 1000);
	SetPVarFloat(playerid, "ac_old_Coord_X", x);
	SetPVarFloat(playerid, "ac_old_Coord_Y", y);
	SetPlayerPos(playerid, x, y, z);
	return 1;
}

stock oSetVehiclePos(vehicleid, Float:x, Float:y, Float:z)
{
	foreach (new playerid : Player) {
		if (GetPlayerVehicleID(playerid) == vehicleid) {
			SetPVarFloat(playerid, "ac_old_Coord_X", x);
			SetPVarFloat(playerid, "ac_old_Coord_Y", y);
		}
	}

	SetVehiclePos(vehicleid, x, y, z);
	return 1;
}

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

stock pt_speed_FixSpeed(playerid)
{
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	SetPVarFloat(playerid, "ac_old_Coord_X", pos[0]);
	SetPVarFloat(playerid, "ac_old_Coord_Y", pos[1]);
	return 1;
}

stock pt_speed_Sync(playerid)
{
	if (!player_IsSpawned(playerid) || GetPVarInt(playerid, "ac_NotSpeedHack") == 1) {
		pt_speed_FixSpeed(playerid);
		return 0;
	}

	new Float:player_speed = GetPlayerSpeed(playerid);
	
	switch ( GetPlayerState(playerid) ) {
		case PLAYER_STATE_ONFOOT: {
			if (player_speed > 50) {
				switch (floatround(player_speed, floatround_round)) {
					// 26 - максимум на земле, 50 при полёте с парашютом
					case 50..349: KickPlayer(playerid, lang_texts[11][17]);
					case 350..499: KickPlayer(playerid, lang_texts[11][18]);
					default: KickPlayer(playerid, lang_texts[11][19]);
				}
			}
		}
		case PLAYER_STATE_DRIVER: {
			new max_speed = GetMaxVehicleSpeed( GetVehicleModel( GetPlayerVehicleID(playerid) ) );
			
			if (player_speed > max_speed) {
				switch ( floatround(player_speed, floatround_round) ) {
					case 151..250: {
						KickPlayer(playerid, lang_texts[11][17]);
					}
					case 251..499: {
						KickPlayer(playerid, lang_texts[11][18]);
					}
					default: {
						KickPlayer(playerid, lang_texts[11][19]);
					}
				}
			}
		}
	}
	return 1;
}
