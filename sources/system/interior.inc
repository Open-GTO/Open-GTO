/*

	About: enter/exit system
	Author: ziggi & GhostTT
	
*/

#if defined _interior_included
	#endinput
#endif

#define _interior_included
#pragma library interior


enum e_i_Player_Info {
	e_pi_index,
	e_pi_enter_id,
}

static player_info[MAX_PLAYERS][e_i_Player_Info];

// types
enum {
	INTERIOR_TYPE_CHICKEN,
	INTERIOR_TYPE_BURGER,
	INTERIOR_TYPE_PIZZA,
	INTERIOR_TYPE_RESTAURANT,
	INTERIOR_TYPE_DONUTS,
	INTERIOR_TYPE_BAR,
	INTERIOR_TYPE_ALHAMBRA,
	INTERIOR_TYPE_JIZZY,
	INTERIOR_TYPE_CALIGULAS,
	INTERIOR_TYPE_REDSANS,
	INTERIOR_TYPE_24ON7,
	INTERIOR_TYPE_24ON7_BIG,
	INTERIOR_TYPE_AMMUNATION_1,
	INTERIOR_TYPE_AMMUNATION_2,
	INTERIOR_TYPE_AMMUNATION_3,
	INTERIOR_TYPE_AMMUNATION_4,
	INTERIOR_TYPE_AMMUNATION_5,
	INTERIOR_TYPE_BINCO,
	INTERIOR_TYPE_URBAN,
	INTERIOR_TYPE_INSIDETRACK,
	INTERIOR_TYPE_PIGPEN,
	INTERIOR_TYPE_TRAINHARD,
	INTERIOR_TYPE_LSPD,
	INTERIOR_TYPE_ZIP,
	INTERIOR_TYPE_SEXSHOP,
	INTERIOR_TYPE_LVPD,
	INTERIOR_TYPE_CITYPLANNING,
	INTERIOR_TYPE_NUDESTRIPPERS,
	INTERIOR_TYPE_LSGYM,
	INTERIOR_TYPE_COBRA,
	INTERIOR_TYPE_LVGYM,
	INTERIOR_TYPE_VICTIM,
	INTERIOR_TYPE_ZERORC,
	INTERIOR_TYPE_FOURDRAGONS,
	INTERIOR_TYPE_LVAIRPORT,
	INTERIOR_TYPE_LSHOTEL,
	INTERIOR_TYPE_LVDAM,
	INTERIOR_TYPE_LSSTADIUM,
	INTERIOR_TYPE_SFSTADIUM,
	INTERIOR_TYPE_ARENA,
	INTERIOR_TYPE_LVSTADIUM,
	INTERIOR_TYPE_LILPROBEINN,
	INTERIOR_TYPE_REECES,
	INTERIOR_TYPE_TATOOS_1,
	INTERIOR_TYPE_TATOOS_2,
	INTERIOR_TYPE_TATOOS_3,
	INTERIOR_TYPE_DIDIERSACHS,
	INTERIOR_TYPE_BARBER_1,
	INTERIOR_TYPE_BARBER_2
}

enum e_Interior_Type {
	e_i_interior,
	Float:e_i_pos_x,
	Float:e_i_pos_y,
	Float:e_i_pos_z,
	Float:e_i_pos_a,
	e_i_pickup_id,
}

static interior_types[][e_Interior_Type] = {
	{9, 364.7823, -11.3572, 1001.8516, 0.0000}, // INTERIOR_TYPE_CHICKEN
	{10, 363.0454, -74.9226, 1001.5078, 0.0000}, // INTERIOR_TYPE_BURGER
	{5, 372.3875, -133.2606, 1001.4922, 0.0000}, // INTERIOR_TYPE_PIZZA
	{1, -794.981400, 489.284700, 1376.195300, 0.0000}, // INTERIOR_TYPE_RESTAURANT
	{17, 377.2882, -192.9613, 1000.6401, 347.3418}, // INTERIOR_TYPE_DONUTS
	{11, 501.9147, -68.1193, 998.7578, 180.0000}, // INTERIOR_TYPE_BAR
	{17, 493.3302, -24.3971, 1000.6797, 0.0000}, // INTERIOR_TYPE_ALHAMBRA
	{3, -2636.6528, 1402.4633, 906.4609, 0.0000}, // INTERIOR_TYPE_JIZZY
	{1, 2234.0168, 1714.3533, 1012.3828, 180.0000}, // INTERIOR_TYPE_CALIGULAS
	{12, 1133.0773, -15.5890, 1000.6797, 0.0000}, // INTERIOR_TYPE_REDSANS
	{6, -27.4186, -57.8725, 1003.5469, 0.0000}, // INTERIOR_TYPE_24ON7
	{18, -30.9799, -91.6893, 1003.5469, 0.0000}, // INTERIOR_TYPE_24ON7_BIG
	{1, 285.3952, -41.5178, 1001.5156, 0.0000}, // INTERIOR_TYPE_AMMUNATION_1
	{4, 285.7551, -86.0345, 1001.5229, 355.9176}, // INTERIOR_TYPE_AMMUNATION_2
	{6, 296.8983, -111.7475, 1001.5156, 0.0000}, // INTERIOR_TYPE_AMMUNATION_3
	{7, 315.3875, -143.2163, 999.6016, 354.3509}, // INTERIOR_TYPE_AMMUNATION_4
	{6, 316.4863, -170.0589, 999.5938, 0.0000}, // INTERIOR_TYPE_AMMUNATION_5
	{15, 207.6846, -110.9347, 1005.1328, 0.0000}, // INTERIOR_TYPE_BINCO
	{1, 203.8098, -50.5994, 1001.8047, 0.0000}, // INTERIOR_TYPE_URBAN
	{3, 834.2251, 7.4040, 1004.1870, 90.0000}, // INTERIOR_TYPE_INSIDETRACK
	{2, 1204.8013, -13.5916, 1000.9219, 0.0000}, // INTERIOR_TYPE_PIGPEN
	{3, 207.0162, -140.2690, 1003.5078, 0.0000}, // INTERIOR_TYPE_TRAINHARD
	{6, 246.7515, 62.6663, 1003.6406, 0.0000}, // INTERIOR_TYPE_LSPD
	{18, 161.3583, -96.5063, 1001.8047, 0.0000}, // INTERIOR_TYPE_ZIP
	{3, -100.3775, -24.7286, 1000.7188, 0.0000}, // INTERIOR_TYPE_SEXSHOP
	{3, 238.6454, 139.0460, 1003.0234, 0.0000}, // INTERIOR_TYPE_LVPD
	{3, 390.3409, 173.7717, 1008.3828, 90.0000},  // INTERIOR_TYPE_CITYPLANNING
	{3, 1212.1140, -26.2612, 1000.9531, 180.0000}, // INTERIOR_TYPE_NUDESTRIPPERS
	{5, 772.2972, -5.0721, 1000.7287, 0.0000}, // INTERIOR_TYPE_LSGYM
	{6, 774.1264, -50.1067, 1000.5859, 0.0000}, // INTERIOR_TYPE_COBRA
	{7, 773.9219, -78.5094, 1000.6623, 0.0000}, // INTERIOR_TYPE_LVGYM
	{5, 227.0943, -8.2332, 1002.2109, 90.0000}, // INTERIOR_TYPE_VICTIM
	{6, -2240.6382, 128.3041, 1035.4141, 270.0000}, // INTERIOR_TYPE_ZERORC
	{10, 2018.7539, 1017.7650, 996.8750, 90.0000}, // INTERIOR_TYPE_FOURDRAGONS
	{10, 422.1958, 2536.4480, 10.0000, 90.0000}, // INTERIOR_TYPE_LVAIRPORT
	{15, 2214.7024, -1150.5114, 1025.7969, 270.0000}, // INTERIOR_TYPE_LSHOTEL
	{17, -959.6079, 1955.7019, 9.0000, 180.0000}, // INTERIOR_TYPE_LVDAM
	{7, -1405.987200, -261.408000, 1043.656300, 0.0000}, // INTERIOR_TYPE_LSSTADIUM
	{15,-1443.709800, 930.071700, 1036.481600, 0.0000}, // INTERIOR_TYPE_SFSTADIUM
	{14, -1464.7389, 1556.3762, 1052.5313, 183.4590}, // INTERIOR_TYPE_ARENA
	{4, -1437.187000, -637.602200, 1048.876500, 0.0000}, // INTERIOR_TYPE_LVSTADIUM
	{18, -228.9418, 1401.1884, 27.7656, 270.0000}, // INTERIOR_TYPE_LILPROBEINN
	{2, 411.5910, -22.5248, 1001.8047, 4.0264}, // INTERIOR_TYPE_REECES
	{16, -204.439987, -26.453998, 1002.273437, 0.0000}, // INTERIOR_TYPE_TATOOS_1
	{17, -204.4067, -9.0825, 1002.2734, 0.0000}, // INTERIOR_TYPE_TATOOS_2
	{3, -204.4351, -44.4028, 1002.2734, 0.0000}, // INTERIOR_TYPE_TATOOS_3
	{14, 204.1658, -165.7678, 1000.5234, 0.0000}, // INTERIOR_TYPE_DIDIERSACHS
	{3, 418.5845, -84.3686, 1001.8047, 0.0000}, // INTERIOR_TYPE_BARBER_1
	{12, 411.9952, -54.4468, 1001.8984, 0.0000} // INTERIOR_TYPE_BARBER_2
};

enum e_Interior_Enter {
	e_ie_type,
	e_ie_icon_id,
	Float:e_ie_pos_x,
	Float:e_ie_pos_y,
	Float:e_ie_pos_z,
	Float:e_ie_pos_a,
	e_ie_pickup_id,
	e_ie_pickup_exit_id,
}

static interior_enters[][e_Interior_Enter] = {
	{INTERIOR_TYPE_URBAN, 45, -2490.0488, -29.0154, 25.6172, 90.0000},
	{INTERIOR_TYPE_URBAN, 45, 2779.6235, 2453.8235, 11.0625, 180.0000},
	{INTERIOR_TYPE_URBAN, 45, 2112.9055, -1211.6233, 23.9631, 180.0000},
	{INTERIOR_TYPE_BINCO, 45, -2374.1487, 910.2123, 45.4326, 90.0000},
	{INTERIOR_TYPE_BINCO, 45, 2102.5891, 2257.3999, 11.0234, 270.0000},
	{INTERIOR_TYPE_BINCO, 45, 2244.4548, -1665.2813, 15.4766, 0.0000},
	{INTERIOR_TYPE_ZIP, 45, -1882.5308, 866.2903, 35.1719, 90.0000},
	{INTERIOR_TYPE_ZIP, 45, 2090.4858, 2224.3142, 11.0234, 180.0000},
	{INTERIOR_TYPE_VICTIM, 45, 2802.7837, 2430.5459, 11.0625, 90.0000},
	{INTERIOR_TYPE_TRAINHARD, 45, 2826.0435, 2407.5281, 11.0625, 180.0000},
	{INTERIOR_TYPE_TRAINHARD, 45, 499.5393, -1360.6158, 16.3675, 0.0000},
	{INTERIOR_TYPE_AMMUNATION_1, 6, -2625.9001, 208.2365, 4.8125, 0.0000},
	{INTERIOR_TYPE_AMMUNATION_1, 6, 1368.7693, -1279.7759, 13.5469, 90.0000},
	{INTERIOR_TYPE_AMMUNATION_1, 6, 2159.4817, 943.2319, 10.8203, 90.0000},
	{INTERIOR_TYPE_AMMUNATION_1, 6, 2333.0889, 61.5770, 26.7058, 270.0000}, 
	{INTERIOR_TYPE_AMMUNATION_2, 6, 242.8935, -178.4391, 1.5822, 90.0000},
	{INTERIOR_TYPE_AMMUNATION_2, 6, 2539.2190, 2083.8953, 10.8203, 90.0000},
	{INTERIOR_TYPE_AMMUNATION_3, 6, -2093.4211, -2464.6709, 30.6250, 270.0000},
	{INTERIOR_TYPE_AMMUNATION_3, 6, 2400.5042, -1981.6545, 13.5469, 0.0000},
	{INTERIOR_TYPE_AMMUNATION_5, 6, -1508.9017, 2610.4512, 55.8359, 180.0000},
	{INTERIOR_TYPE_AMMUNATION_5, 6, -315.6140, 829.7777, 14.2422, 262.7850},
	{INTERIOR_TYPE_CALIGULAS, 25, 2196.7908, 1677.1324, 12.3672, 90.0000},
	{INTERIOR_TYPE_REDSANS, 25, 1658.4637, 2250.4370, 11.0701, 0.0000},
	{INTERIOR_TYPE_FOURDRAGONS, 44, 2019.6924, 1007.8068, 10.8203, 270.0000},
	{INTERIOR_TYPE_ALHAMBRA, 48, 1836.6289, -1682.4927, 13.3400, 180.0000},
	{INTERIOR_TYPE_JIZZY, 48, -2624.5725, 1412.6639, 7.0938, 180.0000},
	{INTERIOR_TYPE_PIGPEN, 48, 2421.5637, -1219.6958, 25.5323, 0.0000},
	{INTERIOR_TYPE_SEXSHOP, -1, 2085.1982, 2074.0313, 11.0547, 270.0000},
	{INTERIOR_TYPE_NUDESTRIPPERS, 48, 2543.3179, 1025.2427, 10.8203, 180.0000},
	{INTERIOR_TYPE_INSIDETRACK, 25, 1288.9238, 271.1603, 19.5547, 270.0000},
	{INTERIOR_TYPE_INSIDETRACK, 25, 1631.8184, -1172.9304, 24.0843, 0.0000},
	{INTERIOR_TYPE_LVPD, 30, 2287.0217, 2432.0430, 10.8203, 180.0000},
	{INTERIOR_TYPE_LSPD, 30, 1555.0339, -1675.6987, 16.1953, 90.0000},
	{INTERIOR_TYPE_LSPD, 30, 627.1440, -571.8030, 17.9145, 270.0000},
	{INTERIOR_TYPE_CITYPLANNING, 20, 2412.5461, 1123.7743, 10.8203, 270.0000},
	{INTERIOR_TYPE_LSGYM, 54, 2229.7434, -1721.4860, 13.5635, 180.0000},
	{INTERIOR_TYPE_COBRA, 54, -2270.3320, -156.0104, 35.3203, 270.0000},
	{INTERIOR_TYPE_LVGYM, 54, 1968.7017, 2295.6604, 16.4559, 180.0000},
	{INTERIOR_TYPE_24ON7, 36, 1937.4680, 2307.2283, 10.8203, 90.0000},
	{INTERIOR_TYPE_24ON7, 36, 2097.5952, 2224.4607, 11.0234, 180.0000},
	{INTERIOR_TYPE_24ON7, 36, 2194.6233, 1991.0389, 12.2969, 90.0000},
	{INTERIOR_TYPE_24ON7, 36, 2247.7466, 2396.4775, 10.8203, 0.0000},
	{INTERIOR_TYPE_24ON7, 36, 2452.4536, 2064.8892, 10.8203, 180.0000},
	{INTERIOR_TYPE_24ON7, 36, -1561.9888, -2733.4324, 48.7435, 180.0000},
	{INTERIOR_TYPE_24ON7, 36, 648.7406, -520.0192, 16.5537, 182.7492},
	{INTERIOR_TYPE_24ON7, 36, 2303.0322, -16.2273, 26.4844, 84.9944},
	{INTERIOR_TYPE_24ON7, 36, -1272.0570, 2712.6667, 50.2663, 154.4422},
	{INTERIOR_TYPE_24ON7, 36, -180.2251, 1132.6233, 19.7422, 87.0274},
	{INTERIOR_TYPE_24ON7_BIG, 36, 2546.5791, 1972.1726, 10.8203, 18.0000},
	{INTERIOR_TYPE_24ON7_BIG, 36, 2884.8823, 2453.7986, 11.0690, 180.0000},
	{INTERIOR_TYPE_24ON7_BIG, 36, 1315.4572, -898.0416, 39.5781, 180.0000},
	{INTERIOR_TYPE_24ON7_BIG, 36, -553.6256, 2593.9050, 53.9348, 270.0000},
	{INTERIOR_TYPE_PIZZA, 29, -1721.2444, 1359.6973, 7.1853, 90.0000},
	{INTERIOR_TYPE_PIZZA, 29, -1808.4500, 945.7524, 24.8906, 180.0000},
	{INTERIOR_TYPE_PIZZA, 29, 1367.3691, 248.3987, 19.5669, 90.0000},
	{INTERIOR_TYPE_PIZZA, 29, 203.5036, -202.3890, 1.5781, 180.0000},
	{INTERIOR_TYPE_PIZZA, 29, 2083.3271, 2224.4194, 11.0234, 180.0000},
	{INTERIOR_TYPE_PIZZA, 29, 2105.1870, -1806.5176, 13.5547, 90.0000},
	{INTERIOR_TYPE_PIZZA, 29, 2332.2661, 75.0068, 26.6210, 270.0000},
	{INTERIOR_TYPE_PIZZA, 29, 2351.8105, 2533.3840, 10.8203, 180.0000},
	{INTERIOR_TYPE_PIZZA, 29, 2638.5471, 1849.8080, 11.0234, 90.0000},
	{INTERIOR_TYPE_CHICKEN, 14, -1213.5768, 1830.6093, 41.9297, 270.0000},
	{INTERIOR_TYPE_CHICKEN, 14, -2154.9976, -2460.4131, 30.8516, 180.0000},
	{INTERIOR_TYPE_CHICKEN, 14, -1816.5159, 618.3961, 35.1740, 180.0000},
	{INTERIOR_TYPE_CHICKEN, 14, -2671.4182, 258.3566, 4.6328, 0.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 172.7902, 1176.8433, 14.7578, 90.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 2102.1233, 2228.7795, 11.0234, 270.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 2393.2649, 2041.7802, 10.8203, 0.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 2397.8098, -1898.9425, 13.5469, 0.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 2420.1047, -1508.9398, 24.0000, 270.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 2638.0955, 1671.9607, 11.0234, 90.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 2838.3821, 2407.4861, 11.0690, 180.0000},
	{INTERIOR_TYPE_CHICKEN, 14, 928.5008, -1352.9829, 13.3438, 90.0000},
	{INTERIOR_TYPE_BURGER, 10, -1912.2883, 828.0991, 35.2209, 270.0000},
	{INTERIOR_TYPE_BURGER, 10, -2336.4890, -166.7664, 35.5547, 270.0000},
	{INTERIOR_TYPE_BURGER, 10, -2356.2266, 1008.1021, 50.8984, 90.0000},
	{INTERIOR_TYPE_BURGER, 10, 1158.2136, 2072.2249, 11.0625, 270.0000},
	{INTERIOR_TYPE_BURGER, 10, 1199.2291, -918.5574, 43.1203, 180.0000},
	{INTERIOR_TYPE_BURGER, 10, 1872.6147, 2071.7510, 11.0625, 270.0000},
	{INTERIOR_TYPE_BURGER, 10, 2169.6868, 2795.8181, 10.8203, 270.0000},
	{INTERIOR_TYPE_BURGER, 10, 2366.5193, 2071.0603, 10.8203, 90.0000},
	{INTERIOR_TYPE_BURGER, 10, 2472.5781, 2034.1549, 11.0625, 90.0000},
	{INTERIOR_TYPE_BURGER, 10, 810.9251, -1616.1669, 13.5469, 270.0000},
	{INTERIOR_TYPE_DONUTS, 17, -144.0737, 1224.7399, 19.8992, 186.9370},
	{INTERIOR_TYPE_DONUTS, 17, 291.8775, -195.2209, 1.5781, 81.3870},
	{INTERIOR_TYPE_DONUTS, 17, 1294.3978, 236.2599, 19.5547, 24.6957},
	{INTERIOR_TYPE_DONUTS, 17, -857.4582, 1535.7279, 22.5870, 320.1738},
	{INTERIOR_TYPE_DONUTS, 17, -2103.7915, -2342.1165, 30.6172, 89.5693},
	{INTERIOR_TYPE_RESTAURANT, 50, 1046.1045, -1130.9662, 23.8281, 180.0000},
	{INTERIOR_TYPE_BAR, 49, 2309.6880, -1643.8400, 14.8270, 90.0000},
	{INTERIOR_TYPE_BAR, 49, 2441.1147, 2065.0554, 10.8203, 180.0000},
	{INTERIOR_TYPE_BAR, 49, 172.9281, -151.7839, 1.5781, 314.3900},
	{INTERIOR_TYPE_BAR, 49, -180.7000, 1088.6160, 19.7422, 46.1461},
	{INTERIOR_TYPE_LVAIRPORT, 5, 423.5569, 2536.4775, 16.1484, 270.0000},
	{INTERIOR_TYPE_LSHOTEL, -1, 2232.9968, -1159.7925, 25.8906, 90.0000},
	{INTERIOR_TYPE_LVDAM, -1, -595.1345, 2017.8734, 60.5391, 0.0000},
	{INTERIOR_TYPE_LSSTADIUM, 33, 2695.5405, -1704.5406, 11.8438, 0.0000},
	{INTERIOR_TYPE_SFSTADIUM, 33, -2110.1619, -445.7126, 38.7344, 264.4261},
	{INTERIOR_TYPE_ARENA, 33, -2171.9771, 252.0907, 35.3382, 88.8802},
	{INTERIOR_TYPE_LVSTADIUM, 33, 1099.6008, 1601.5157, 12.5469, 0.0000},
	{INTERIOR_TYPE_LILPROBEINN, 49, -89.1717, 1378.3308, 10.4698, 270.0000},
	{INTERIOR_TYPE_ZERORC, 47, -2242.1392, 128.6178, 35.3203, 90.0000},
	{INTERIOR_TYPE_REECES, 7, 2070.9993, -1793.8484, 13.5533, 89.1373},
	{INTERIOR_TYPE_TATOOS_1, 39, 2069.0210, -1779.8516, 13.5594, 89.1373},
	{INTERIOR_TYPE_TATOOS_2, 39, -2490.9951, -38.8843, 25.6172, 89.1373},
	{INTERIOR_TYPE_TATOOS_3, 39, 2094.6965, 2122.8655, 10.8203, 89.1373},
	{INTERIOR_TYPE_DIDIERSACHS, 45, 454.2128, -1478.0035, 30.8147, 90.0000},
	{INTERIOR_TYPE_VICTIM, 45, 461.7163, -1500.8706, 31.0445, 90.0000},
	{INTERIOR_TYPE_BARBER_1, 7, 824.0370, -1588.2991, 13.5435, 90.0000},
	{INTERIOR_TYPE_BARBER_2, 7, 2724.4570, -2026.5754, 13.5547, 90.0000}
};

Interior_OnGameModeInit()
{
	DisableInteriorEnterExits();
	
	new
		i,
		world,
		type;

	for (i = 0; i < sizeof(interior_enters); i++) {
		// enter pos
		interior_enters[i][e_ie_pickup_id] = CreatePickup(19902, 1,
			interior_enters[i][e_ie_pos_x], interior_enters[i][e_ie_pos_y], interior_enters[i][e_ie_pos_z] - 1.0
		);
		Create3DTextLabel("Вход\n{33AA33}Нажмите кнопку '{FFFFFF}"KEY_NAME"{33AA33}'", 0xFFFFFFFF,
			interior_enters[i][e_ie_pos_x], interior_enters[i][e_ie_pos_y], interior_enters[i][e_ie_pos_z],
			20.0, 0, 1
		);

		if (interior_enters[i][e_ie_icon_id] != -1) {
			Mapicon_Create(interior_enters[i][e_ie_pos_x], interior_enters[i][e_ie_pos_y], interior_enters[i][e_ie_pos_z], interior_enters[i][e_ie_icon_id], 0);
		}

		// exit pos
		world = Interior_GetVirtualWorld(i);
		type = Interior_GetType(i);

		interior_enters[i][e_ie_pickup_exit_id] = CreatePickup(19902, 1,
			interior_types[type][e_i_pos_x], interior_types[type][e_i_pos_y], interior_types[type][e_i_pos_z] - 1.0, world
		);
		Create3DTextLabel("Выход\n{33AA33}Нажмите кнопку '{FFFFFF}"KEY_NAME"{33AA33}'", 0xFFFFFFFF,
			interior_types[type][e_i_pos_x], interior_types[type][e_i_pos_y], interior_types[type][e_i_pos_z],
			20.0, world, 1
		);

		// callback
		Interior_OnInteriorCreated(i, type, world);
	}

	Log_Game("SERVER: Interior module init");
	return 1;
}

Interior_OnInteriorCreated(id, type, world)
{
	fastfood_OnInteriorCreated(id, type, world);
	bar_OnInteriorCreated(id, type, world);
	bank_OnInteriorCreated(id, type, world);
	wshop_OnInteriorCreated(id, type, world);
	return 1;
}

Interior_OnPlayerConnect(playerid)
{
	Interior_SetPlayerEnterID(playerid, -1);
	Interior_SetPlayerIndex(playerid, -1);
	return 1;
}

Interior_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys

	new
		id = Interior_GetPlayerEnterID(playerid);

	if (id == -1) {
		return 0;
	}

	new
		type = Interior_GetType(id),
		index = Interior_GetPlayerIndex(playerid);

	if (index == -1) {
		if (IsPlayerInRangeOfPoint(playerid, 2,
				interior_enters[id][e_ie_pos_x], interior_enters[id][e_ie_pos_y], interior_enters[id][e_ie_pos_z])) {
			Interior_SetPlayerIndex(playerid, id);

			SetPlayerInterior(playerid, interior_types[type][e_i_interior]);
			SetPlayerVirtualWorld(playerid, Interior_GetVirtualWorld(id));
			SetPlayerPos(playerid, interior_types[type][e_i_pos_x], interior_types[type][e_i_pos_y], interior_types[type][e_i_pos_z]);
			SetPlayerFacingAngle(playerid, interior_types[type][e_i_pos_a]);
			return 1;
		}
	} else {
		if (IsPlayerInRangeOfPoint(playerid, 2,
				interior_types[type][e_i_pos_x], interior_types[type][e_i_pos_y], interior_types[type][e_i_pos_z])) {
			Interior_SetPlayerIndex(playerid, -1);

			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);
			SetPlayerPos(playerid, interior_enters[index][e_ie_pos_x], interior_enters[index][e_ie_pos_y], interior_enters[index][e_ie_pos_z]);
			SetPlayerFacingAngle(playerid, interior_enters[index][e_ie_pos_a]);
			return 1;
		}
	}
	return 0;
}

Interior_OnPlayerPickUpPickup(playerid, pickupid)
{
	Interior_SetPlayerEnterID(playerid, -1);

	for (new i = 0; i < sizeof(interior_enters); i++) {
		if (pickupid == interior_enters[i][e_ie_pickup_id] || pickupid == interior_enters[i][e_ie_pickup_exit_id]) {
			if (pickupid == interior_enters[i][e_ie_pickup_id]) {
				Interior_SetPlayerIndex(playerid, -1);
			}

			Interior_SetPlayerEnterID(playerid, i);
			return 1;
		}
	}
	return 0;
}

stock Interior_GetPlayerIndex(playerid)
{
	return player_info[playerid][e_pi_index];
}

stock Interior_SetPlayerIndex(playerid, index)
{
	player_info[playerid][e_pi_index] = index;
}

stock Interior_GetPlayerEnterID(playerid)
{
	return player_info[playerid][e_pi_enter_id];
}

stock Interior_SetPlayerEnterID(playerid, id)
{
	player_info[playerid][e_pi_enter_id] = id;
}

stock Interior_GetVirtualWorld(id)
{
	if (0 <= id < sizeof(interior_enters)) {
		return (id + 1);
	}
	return -1;
}

stock Interior_GetInterior(id)
{
	new type = Interior_GetType(id);

	if (0 <= type < sizeof(interior_types)) {
		return interior_types[type][e_i_interior];
	}
	
	return -1;
}

stock Interior_GetType(id)
{
	if (0 <= id < sizeof(interior_enters)) {
		return interior_enters[id][e_ie_type];
	}

	return -1;
}

stock Interior_GetPos(id, &Float:x, &Float:y, &Float:z)
{
	if (0 <= id < sizeof(interior_enters)) {
		x = interior_enters[id][e_ie_pos_x];
		y = interior_enters[id][e_ie_pos_y];
		z = interior_enters[id][e_ie_pos_z];
		return 1;
	}
	return 0;
}

stock Interior_IsValidInfo(index, interior, world)
{
	if (index != -1 && Interior_GetVirtualWorld(index) == world && Interior_GetInterior(index) == interior) {
		return 1;
	}

	return 0;
}
