/*
	Author:	ZiGGi
*/

#if defined _speedhack_included
	#endinput
#endif

#define _speedhack_included
#pragma library speedhack


new AntiSpeedHackEnabled = ANTI_SPEED_HACK_ENABLED;

stock AntiSpeedHackLoadConfig()
{
	if(!ini_Exist(ConfigDB)) return 0;
	new file_ash = ini_Open(ConfigDB);
	ini_GetInt(file_ash,"Anti_Speed_Hack_Enabled",AntiSpeedHackEnabled);
	ini_Close(file_ash);
	return 1;
}

stock AntiSpeedHackSaveConfig()
{
	new file_ash = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_ash,"Anti_Speed_Hack_Enabled",AntiSpeedHackEnabled);
	ini_Close(file_ash);
	return 1;
}

stock ash_OnGameModeInit()
{
	AntiSpeedHackLoadConfig();
	GameMSG("SERVER: (protections)AntiSpeedHack module init");
	return 1;
}

stock ash_OnPlayerStateChange(playerid, newstate, oldstate)
{
	// если выпрыгнул из транспорта
	if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
	{
		SetPVarInt(playerid,"ac_NotSpeedHack",1);
		SetTimerEx("NotSpeedHack",2000,0,"d",playerid);
	}
	return 1;
}

forward NotSpeedHack(playerid);
public NotSpeedHack(playerid)
{
	SetPVarInt(playerid,"ac_NotSpeedHack",0);
}

stock oSetPlayerPos(playerid,Float:x,Float:y,Float:z)
{
	SetPVarFloat(playerid,"ac_old_Coord_X",x);
	SetPVarFloat(playerid,"ac_old_Coord_Y",y);
	SetPlayerPos(playerid,x,y,z);
	return 1;
}

stock Float:GetPlayerSpeed(playerid)
{
	new Float:pos[3];
	GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
	new Float:Dis = floatsqroot(
		floatpower( floatabs( floatsub(GetPVarFloat(playerid,"ac_old_Coord_X"),pos[0]) ),2 )
		+
		floatpower( floatabs( floatsub(GetPVarFloat(playerid,"ac_old_Coord_Y"),pos[1]) ),2 )
	);
	SetPVarFloat(playerid,"ac_old_Coord_X",pos[0]);
	SetPVarFloat(playerid,"ac_old_Coord_Y",pos[1]);
	return (Dis * 2.0);
}

stock SyncSpeed(playerid)
{
	if (AntiSpeedHackEnabled != 1) return 0;
	new Float:player_speed = GetPlayerSpeed(playerid),
		max_speed;
	
	switch ( GetPlayerState(playerid) )
	{
		case PLAYER_STATE_ONFOOT:
		{
			if (player_speed > 25 && GetPVarInt(playerid,"ac_NotSpeedHack") != 1)
			{
				switch (floatround(player_speed,floatround_round))
				{
					case 26..349: KickPlayer(playerid,lang_texts[11][17]);
					case 350..499: KickPlayer(playerid,lang_texts[11][18]);
					default: KickPlayer(playerid,lang_texts[11][19]);
				}
			}
		}
		case PLAYER_STATE_DRIVER:
		{
			switch ( GetVehicleModel( GetPlayerVehicleID(playerid) ) )
			{
				case 481,509,510,462,448,471: max_speed = 40;
				case 411: max_speed = 130;
				case 520: max_speed = 180;
				default: max_speed = 150;
			}
			if (player_speed > max_speed)
			{
				switch ( floatround(player_speed,floatround_round) )
				{
					case 151..250: KickPlayer(playerid,lang_texts[11][17]);
					case 251..499: KickPlayer(playerid,lang_texts[11][18]);
					default: KickPlayer(playerid,lang_texts[11][19]);
				}
			}
		}
	}
	return 1;
}
