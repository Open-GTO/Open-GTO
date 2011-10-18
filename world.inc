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
	new starttime = Now();
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
	trucker_SaveConfig();
	// systems
	level_SaveConfig();
	health_SaveConfig();
	money_SaveConfig();
	lang_SaveConfig();
	// protections
	AntiIdleSaveConfig();
	AntiHightPingSaveConfig();
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
	GameMSG("World save complete! Time taken: %s.",ConvertSeconds(Now() - starttime));
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
	for(new i;i<300;i++)
 	{
	 	if(i!=0 && i!=3 && i!=4 && i!=5 && i!=6 && i!=8 && i!=42 && i!=65 && i!=74 && i!=86 && i!=119 && i!=149 && i!=208 && i!=273 && i!=289)
		{
			AddPlayerClass(i,0.0,0.0,0.0,0.0,0,0,0,0,0,0);
		}
	}
	GameMSG("SERVER: All Classes init");
}

forward OneSecTimer();
public OneSecTimer()
{
	CheckDM();
	CheckRace();
	SyncPlayers();
	MapIcons_Streaming();
	trucker_RunnerTimer();
}

forward OneMinuteTimer();
public OneMinuteTimer()
{
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
