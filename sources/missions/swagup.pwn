/*

	About: package pickup system
	Author: ziggi
	Thanks: Sandra for coords in CashBox script

*/

#if defined _swagup_included
	#endinput
#endif

#define _swagup_included

/*
	Vars
*/

static
	gPickupID,
	gPreviousTime;

static Float:gCoords[][CoordInfo] = {
	{2227.74, 1516.43, 10.82},
	{-724.44, 1402.81, 13.07},
	{-1940.61, 1086.14, 53.09},
	{-2344.37, -459.59, 80.01},
	{-1674.65, -543.03, 14.14},
	{-2415.30, -2142.08, 52.37},
	{-1527.47, -2291.62, -5.63},
	{-1111.05, -2470.04, 76.59},
	{-288.23, -2163.67, 28.63},
	{-376.27, -2583.97, 138.17},
	{378.32, -1885.70, 2.05},
	{1120.86, -2065.82, 74.42},
	{1583.25, -2286.55, 13.53},
	{2718.67, -2385.16, 13.63},
	{2744.79, -1944.55, 17.32},
	{2041.71, -1715.58, 13.54},
	{2771.79, -1354.51, 50.00},
	{1102.96, -1092.89, 28.46},
	{727.73, -1276.13, 13.64},
	{755.31, -591.31, 18.01},
	{360.91, -110.02, 1.22},
	{-557.35, -541.28, 25.52},
	{-273.04, -955.98, 38.30},
	{1242.36, 327.17, 19.75},
	{2791.64, 2225.64, 14.66},
	{-1955.01, -986.97, 35.89},
	{-2108.89, -2376.97, 30.62},
	{1853.35, 2045.54, 10.85},
	{2478.85, -1437.22, 25.49},
	{-2241.50, 2462.93, 4.98},
	{703.36, 267.78, 21.44},
	{-599.09, -1080.95, 23.66},
	{-2677.68, 1503.98, 2.07},
	{2582.18, -2115.22, 1.11},
	{-1954.97, -986.27, 35.89},
	{541.41, 830.33, -39.44},
	{-2876.66, 292.77, 6.96},
	{-2192.16, 2409.51, 4.95},
	{2615.91, -1730.39, 6.24},
	{-529.81, -991.29, 24.55},
	{-1048.33, -1306.72, 128.50},
	{1227.20, 2584.68, 10.82},
	{1016.48, 1411.63, 10.82},
	{-1593.62, 802.54, 6.82},
	{-2238.23, -2478.96, 31.19},
	{917.43, 2402.92, 10.82},
	{1312.97, -1965.60, 29.46},
	{-120.59, -1531.31, 3.07},
	{-2508.68, -53.19, 25.65},
	{-2762.04, 105.18, 6.99},
	{1367.53, 194.54, 19.55},
	{-2655.83, -102.70, 3.99},
	{2161.85, -102.76, 2.75},
	{2535.77, 1342.55, 10.82},
	{-2059.10, 890.89, 61.85},
	{1188.14, 231.38, 19.56},
	{1088.81, 1073.79, 10.83},
	{422.96, 2539.45, 16.52},
	{-227.49, 2709.91, 62.98},
	{-347.38, 1581.21, 76.30},
	{-208.08, 1127.93, 19.74},
	{33.24, 1155.21, 19.69},
	{1582.07, -2691.56, 6.12},
	{1715.25, -1912.01, 13.56},
	{286.36, -1145.15, 80.91}
};

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	if (!IsMissionEnabled(MISSION_SWAGUP)) {
		return 0;
	}

	Swagup_SetTimeout();

	Log_Init("missions", "Swagup module init.");
	#if defined Swagup_OnGameModeInit
		return Swagup_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Swagup_OnGameModeInit
#if defined Swagup_OnGameModeInit
	forward Swagup_OnGameModeInit();
#endif

/*
	OnPlayerPickUpDynamicPickup
*/

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	if (!IsMissionEnabled(MISSION_SWAGUP) || pickupid != gPickupID) {
		return 0;
	}

	DestroyDynamicPickup(gPickupID);

	new
		win_money = Mission_CalculateMoney(playerid, MISSION_SWAGUP),
		win_xp = Mission_CalculateXP(playerid, MISSION_SWAGUP);

	GivePlayerMoney(playerid, win_money);
	GivePlayerXP(playerid, win_xp);

	Lang_SendTextToAll("SWAGUP_BAG_FOUND", ret_GetPlayerName(playerid), playerid, win_money, win_xp);

	Swagup_SetTimeout();
	#if defined Swagup_OnPlayerPickUpDP
		return Swagup_OnPlayerPickUpDP(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpDP
	#undef OnPlayerPickUpDynamicPickup
#else
	#define _ALS_OnPlayerPickUpDP
#endif

#define OnPlayerPickUpDynamicPickup Swagup_OnPlayerPickUpDP
#if defined Swagup_OnPlayerPickUpDP
	forward Swagup_OnPlayerPickUpDP(playerid, pickupid);
#endif

/*
	OneMinuteTimer
*/

public OneMinuteTimer()
{
	if (!Swagup_IsPaused() && gPreviousTime > gettime()) {
		Lang_SendTextToAll("SWAGUP_BAG_CREATED");

		new swid = random( sizeof(gCoords) );
		gPickupID = CreateDynamicPickup(410, 3, gCoords[swid][Coord_X], gCoords[swid][Coord_Y], gCoords[swid][Coord_Z], -1);
		Swagup_SetPaused();
	}
	#if defined Swagup_OneMinuteTimer
		return Swagup_OneMinuteTimer();
	#else
		return 1;
	#endif
}
#if defined _ALS_OneMinuteTimer
	#undef OneMinuteTimer
#else
	#define _ALS_OneMinuteTimer
#endif

#define OneMinuteTimer Swagup_OneMinuteTimer
#if defined Swagup_OneMinuteTimer
	forward Swagup_OneMinuteTimer();
#endif

/*
	Functions
*/

stock Swagup_IsPaused()
{
	return gPreviousTime == -1;
}

stock Swagup_SetPaused()
{
	gPreviousTime = -1;
}

stock Swagup_SetTimeout()
{
	gPreviousTime = gettime() + Mission_GetPauseTime(MISSION_SWAGUP);
}
