//
// Created:		05.09.06
// Aurthor:		Iain Gilbert
// Modded:		Peter Steenbergen
//

#if defined _world_included
	#endinput
#endif

#define _world_included
#pragma library world


#include "base"
#include "account"
#include "player"
#include "payday"


forward WorldSave(necessarily);
public WorldSave(necessarily) // save all
{
	static emptyServerSaved = 0;
	
	if (PlayerCount() == 0 && necessarily != 1) {
		if (emptyServerSaved == 0) {
			emptyServerSaved = 1;
		} else {
			return 0;
		}
	} else {
		emptyServerSaved = 0;
	}
	
	new starttime = GetTickCount();

	foreach (new id : Player) {
		player_Save(id);
		account_Save(id);
	}

	cfg_SaveConfigs();

	// other
	player_level_SaveLevelConfig();

	weapon_SaveAll();
	race_SaveAll();
	deathmatch_SaveAll();
	business_SaveAll();
	houses_SaveAll();
	gang_SaveAll();
	groundhold_SaveAll();
	
	GameMSG("World save complete! Time taken: %d milliseconds.", (GetTickCount() - starttime));
	return 1;
}

//----------------------------------------------------------------------------------------------------

stock SyncTime()
{
	if (RealTime == 0) {
		WorldTime++;
		
		if (WorldTime > 23) {
			WorldTime = 0;
		}
	} else {
		new hour;
		gettime(hour);
		
		hour += RealTimeOffset;
		if (hour > 23) {
			hour -= 23;
		}
		
		WorldTime = hour;
	}
	SetWorldTime(WorldTime);
}

//----------------------------------------------------------------------------------------------------

stock SpawnWorld() // run at startup to spawn world
{
	for(new i = SKINS_MINID; i <= SKINS_MAXID; i++) {
		switch (i) {
			case SKINS_IGNORE: {

			}
			default: AddPlayerClass(i, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
		}
	}
	GameMSG("SERVER: All Classes init");
}

forward OneSecTimer();
public OneSecTimer()
{
	foreach (new playerid : Player) {
		MapIcons_Streaming(playerid);
		trucker_RunnerTimer(playerid);
		player_Sync(playerid);
		UpdatePlayerWeaponTextDraws(playerid);
	}
	
	CheckDM();
	CheckRace();
}

forward OneMinuteTimer();
public OneMinuteTimer()
{
	foreach (new playerid : Player) {
		if (player_GetSkydiveTime(playerid) > 0) {
			player_SetSkydiveTime(playerid, player_GetSkydiveTime(playerid) - 1);
		}
		if (player_IsLogin(playerid)) {
			JailPlayerTimer(playerid);
			MutePlayerTimer(playerid);
			pt_idle_PlayerTimer(playerid);
			Gang_GiveXp(playerid);
		}
	}
	weather_WeatherUpdate();
	SyncTime();
	TurnAround();
}

forward OneHourTimer();
public OneHourTimer()
{
	Profit();
	CheckHousesOwners();
	CheckBusinessOwners();
	lottery_RunTimer();
	vshop_OneHourTimer();
}

forward TenMinuteTimer();
public TenMinuteTimer()
{
	HouseKeepUp();
}

forward FiveSecondTimer();
public FiveSecondTimer()
{
	CheckAllGround();
	PlayerHealthRegen();
	checkpoint_Sync();
	business_Update3DTextLabelText();
	housing_Update3DTextLabelText();
	PayDay();
	vshop_SetVehiclesToRespawn();

	foreach (new playerid : Player) {
		if (player_IsSpawned(playerid)) {
			SyncPlayerWeapons(playerid);
		}
	}
}
