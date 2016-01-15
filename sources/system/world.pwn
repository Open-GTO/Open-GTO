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


forward WorldSave(necessarily);
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
		player_Save(id);
		Account_Save(id);
	}

	cfg_SaveConfigs();

	// other
	weapon_SaveAll();
	business_SaveAll();
	houses_SaveAll();
	Gang_SaveAll();
	Groundhold_SaveAll();
	
	Log_Game("World save complete! Time taken: %d milliseconds.", (GetTickCount() - starttime));
	return 1;
}

//----------------------------------------------------------------------------------------------------

forward OneSecTimer();
public OneSecTimer()
{
	foreach (new playerid : Player) {
		player_Sync(playerid);
		UpdatePlayerWeaponTextDraws(playerid);
		adm_spec_UpdateLabel(playerid);
	}

	Lottery_WaitTimer();
}

forward OneMinuteTimer();
public OneMinuteTimer()
{
	foreach (new playerid : Player) {
		if (player_IsLogin(playerid)) {
			pt_idle_PlayerTimer(playerid);
			Gang_GiveMemberXP(playerid);
		}
	}
	weather_Update();
	Time_Sync();
	TurnAround();
}

forward OneHourTimer();
public OneHourTimer()
{
	bank_Profit();
	CheckBusinessOwners();
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
	foreach (new playerid : Player) {
		if (player_IsLogin(playerid)) {
			MutePlayerTimer(playerid);
			JailPlayerTimer(playerid);
			FreezePlayerTimer(playerid);
			CheckPlayerInvitedGangTime(playerid);
		}
	}

	Groundhold_CheckAll();
	pl_health_Regeneration();
	business_Update3DTextLabelText();
	housing_Update3DTextLabelText();
	payday_Check();
	vshop_SetVehiclesToRespawn();
}
