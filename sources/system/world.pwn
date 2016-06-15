//
// Created:		05.09.06
// Aurthor:		Iain Gilbert
// Modded:		Peter Steenbergen
//

#if defined _world_included
	#endinput
#endif

#define _world_included


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
		Player_Save(id);
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
		Player_Sync(playerid);
		UpdatePlayerWeaponTextDraws(playerid);
		adm_spec_UpdateLabel(playerid);
	}

	Lottery_WaitTimer();
}

forward OneMinuteTimer();
public OneMinuteTimer()
{
	foreach (new playerid : Player) {
		if (IsPlayerLogin(playerid)) {
			pt_idle_PlayerTimer(playerid);
			Gang_GiveMemberXP(playerid);
		}
	}

	Time_Sync();
	TurnAround();
}

forward OneHourTimer();
public OneHourTimer()
{
	Bank_AddProfit();
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
		if (IsPlayerLogin(playerid)) {
			MutePlayerTimer(playerid);
			JailPlayerTimer(playerid);
			FreezePlayerTimer(playerid);
			CheckPlayerInvitedGangTime(playerid);
		}
	}

	Groundhold_CheckAll();
	RegenerationPlayerHealth();
	business_Update3DTextLabelText();
	housing_Update3DTextLabelText();
	payday_Check();
	vshop_SetVehiclesToRespawn();
	Weather_Update();
}
