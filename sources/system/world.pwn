//
// Created:		05.09.06
// Aurthor:		Iain Gilbert
// Modded:		Peter Steenbergen
//

#if defined _world_included
	#endinput
#endif

#define _world_included

/*
	Forwards
*/

forward WorldSave(necessarily);

forward OneSecTimer();
forward OneMinuteTimer();
forward OneHourTimer();
forward TenMinuteTimer();
forward FiveSecondTimer();

/*
	WorldSave
*/

public WorldSave(necessarily) // save all
{
	static emptyServerSaved = 0;

	if (GetPlayersCount() == 0 && necessarily != 1) {
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
		Player_Save(id);
		Account_Save(id);
	}

	cfg_SaveConfigs();

	// other
	Weapon_Save();
	business_SaveAll();
	houses_SaveAll();
	Gang_SaveAll();
	Groundhold_SaveAll();

	Log(mainlog, INFO, "World save complete! Time taken: %d milliseconds.", GetTickDiff(GetTickCount(), starttime));
	return 1;
}

/*
	Timers
*/

public OneSecTimer()
{
	foreach (new playerid : Player) {
		PlayerWSkillTD_UpdateString(playerid);
		AdminSpectate_UpdateLabel(playerid);
		if (IsPlayerLogin(playerid)) {
			FreezePlayerTimer(playerid);
		}
	}

	Lottery_WaitTimer();
	return 1;
}

public FiveSecondTimer()
{
	foreach (new playerid : Player) {
		if (IsPlayerLogin(playerid)) {
			MutePlayerTimer(playerid);
			JailPlayerTimer(playerid);
			CheckPlayerInvitedGangTime(playerid);
		}
	}

	Groundhold_CheckAll();
	RegenerationPlayerHealth();
	payday_Check();
	VehShop_SetVehiclesToRespawn();
	Weather_Update();
	return 1;
}

/*
	OneMinuteTimer
*/

public OneMinuteTimer()
{
	foreach (new playerid : Player) {
		if (IsPlayerLogin(playerid)) {
			Gang_GiveMemberXP(playerid);
		}
	}

	Time_Sync();
	TurnAround();
	#if defined World_OneMinuteTimer
		return World_OneMinuteTimer();
	#else
		return 1;
	#endif
}
#if defined _ALS_OneMinuteTimer
	#undef OneMinuteTimer
#else
	#define _ALS_OneMinuteTimer
#endif

#define OneMinuteTimer World_OneMinuteTimer
#if defined World_OneMinuteTimer
	forward World_OneMinuteTimer();
#endif

public TenMinuteTimer()
{
	HouseKeepUp();
	return 1;
}

public OneHourTimer()
{
	Bank_AddProfit();
	CheckBusinessOwners();
	VehShop_OneHourTimer();
	return 1;
}
