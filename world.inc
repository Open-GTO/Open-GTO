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

new LastWorldSave = 1;

forward WorldSave(necessarily);
public WorldSave(necessarily) // save all
{
	if(necessarily == 1) goto nee;
	if(PlayerCount() > 0)
	{
		LastWorldSave = 1;
	}
	else if(LastWorldSave == 1)
	{
		LastWorldSave = 0;
		goto nee;
	}
	if(LastWorldSave != 1) return 0;
nee:
	new starttime = gettime();
	foreach(Player,id)
	{
		if(!IsPlayerRegistered(id)) continue;
		PlayerSave(id);
		AccountSave(id);
	}
	BaseSaveConfig();
	AccountSaveConfig();
	PlayerSaveConfig();
	GangSaveConfig();
	PaydaySaveConfig();
	LoggingSaveConfig();
	BankSaveConfig();
	vehicles_SaveConfig();
	groundhold_SaveConfig();
	groundhold_SaveAll();
	mission_SaveConfig();
	QuidemSys_SaveConfig();
	// systems
	level_SaveConfig();
	health_SaveConfig();
	money_SaveConfig();
	lang_SaveConfig();
	// protections
	AntiIdleSaveConfig();
	AntiHightPingSaveConfig();
	AntiSpeedHackSaveConfig();
	AntiWeaponHackSaveConfig();
	AntiRconHackSaveConfig();
	AntiArmourHackSaveConfig();
	AntiJetPackSaveConfig();
	ChatGuardSaveConfig();
	BanWeaponsSaveConfig();
	// other
	WeaponSaveAll();
	RaceSaveAll();
	DeathmatchSaveAll();
	BusinessSaveAll();
	HousesSaveAll();
	GangSaveAll();
	RaceSaveScores();
	DeathmatchSaveScores();
	//
	GameMSG("World save complete! Time taken: %d seconds.",(gettime() - starttime));
	return 1;
}

//----------------------------------------------------------------------------------------------------

stock SyncTime()
{
	if(RealTime != 1)
	{
		if(WorldTime >= 23)
			WorldTime = 0;
		else
			WorldTime++;
		SetWorldTime(WorldTime);
	} 
	else 
	{
		new hour;
		gettime(hour);
		SetWorldTime(hour);
	}
}

//----------------------------------------------------------------------------------------------------

stock SpawnWorld() // run at startup to spawn world
{
	for(new i=0;i<300;i++)
 	{
	 	if(i!=0 && i!=74)
		{
			AddPlayerClass(i,0.0,0.0,0.0,0.0,0,0,0,0,0,0);
		}
	}
	GameMSG("SERVER: All Classes init");
}

forward OneSecTimer();
public OneSecTimer()
{
	foreach (Player,playerid)
	{
		MapIcons_Streaming(playerid);
		trucker_RunnerTimer(playerid);
		SyncPlayers(playerid);
		UpdatePlayerWeaponTextDraws(playerid);
	}
	
	CheckDM();
	CheckRace();
}

forward OneMinuteTimer();
public OneMinuteTimer()
{
	foreach (Player, playerid)
	{
		if (GetPlayerSkydiveTime(playerid) > 0)
		{
			SetPlayerSkydiveTime(playerid, GetPlayerSkydiveTime(playerid) - 1);
		}
	}
	weather_WeatherUpdate();
	SyncTime();
	settime();
	MuteCheck();
	JailTimer();
	AntiIdle();
	TurnAround();
}

forward OneHourTimer();
public OneHourTimer()
{
	Profit();
	CheckHousesOwners();
	CheckBusinessOwners();
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
	SyncActiveCP();
	business_Update3DTextLabelText();
	housing_Update3DTextLabelText();
	PayDay();
}
