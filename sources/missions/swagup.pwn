/*

	Описание: Скрипт поклаж
	Автор: ziggi
	Дата: 08.05.2011

*/

#if defined _swagup_included
	#endinput
#endif

#define _swagup_included


static
	g_pickup_id;

static Float:swagup_Coords[][CoordInfo] = { // from CashBox script by Sandra
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

stock swagup_OnGameModeInit()
{
	if (!IsMissionEnabled(mission_swagup)) {
		return 0;
	}

	swagup_SpawnPickup();

	Log_Game("LOG_SWAGUP_INIT");
	return 1;
}

stock swagup_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!IsMissionEnabled(mission_swagup) || pickupid != g_pickup_id) {
		return 0;
	}

	DestroyDynamicPickup(g_pickup_id);

	new
		win_money = mission_CalculateMoney(playerid, mission_swagup),
		win_xp = mission_CalculateXP(playerid, mission_swagup);

	GivePlayerMoney(playerid, win_money);
	GivePlayerXP(playerid, win_xp, 1);

	new string[MAX_STRING];
	Lang_SendTextToAll($ReturnPlayerName(playerid, playerid, win_money, win_xp);

	SetTimer("swagup_SpawnPickup", mission_GetPauseTime(mission_swagup), 0);
	return 1;
}

forward swagup_SpawnPickup();
public swagup_SpawnPickup()
{
	new swid = random( sizeof(swagup_Coords) );
	g_pickup_id = CreateDynamicPickup(410, 3, swagup_Coords[swid][Coord_X], swagup_Coords[swid][Coord_Y], swagup_Coords[swid][Coord_Z], -1);
	foreach (new playerid : Player) {
		Lang_SendText(playerid, "SWAGUP_BAG_CREATED");
	}
	return 1;
}
