//MapFix by Nexius v3.0.0

#if defined _mapfix_included
	#endinput
#endif
#define _mapfix_included

#include <a_samp>

#define MAPFIX_VERSION		"3.0.0"

#define MAX_MAPFIX_OBJECTS	332

//Uncomment to enable
//#define USE_INCOGNITO_STREAMER

static
	MapFixObjects[MAX_MAPFIX_OBJECTS],
	MapFixIsEnabled = 1;

#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnGameModeInit()
#else
	public OnGameModeInit()
#endif
{
	#if defined GetSVarInt\
		&& defined SetSVarInt
		if(GetSVarInt("MapFixIsEnabled"))
		{
			print("MapFix already included!");
			MapFixIsEnabled++;
		}
		else SetSVarInt("MapFixIsEnabled", 1);
	#endif
	if(MapFixIsEnabled < 2)
	{
		printf("MapFix by Nexius v%s loaded (include version).\a", MAPFIX_VERSION);
		CreateMapFixObjects();
	}
	#if !defined _inc_y_hooks && !defined _INC_y_hooks\
		&& defined mf_OnGameModeInit
		return mf_OnGameModeInit();
	#else
		return 1;
	#endif
}

#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit mf_OnGameModeInit
	#if defined mf_OnGameModeInit
		forward mf_OnGameModeInit();
	#endif
#endif

#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnGameModeExit()
#else
	public OnGameModeExit()
#endif
{
	if(MapFixIsEnabled < 2)
	{
		printf("MapFix by Nexius v%s unloaded (include version).\a", MAPFIX_VERSION);
		DestroyMapFixObjects();
		#if defined DeleteSVar
			DeleteSVar("MapFixIsEnabled");
		#endif
	}
	#if !defined _inc_y_hooks && !defined _INC_y_hooks\
		&& defined mf_OnGameModeExit
		return mf_OnGameModeExit();
	#else
		return 1;
	#endif
}

#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif
	#define OnGameModeExit mf_OnGameModeExit
	#if defined mf_OnGameModeExit
		forward mf_OnGameModeExit();
	#endif
#endif

CreateMapFixObjects()
{
	#if defined USE_INCOGNITO_STREAMER\
		&& defined CreateDynamicObject
		#define mf_CreateObject(%0,%1,%2,%3,%4,%5,%6,%7) CreateDynamicObject(%0, %1, %2, %3, %4, %5, %6, -1, -1, -1, %7)
		#define mf_SetObjectMaterial SetDynamicObjectMaterial
	#else
		#define mf_CreateObject CreateObject
		#define mf_SetObjectMaterial SetObjectMaterial
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_1
		MapFixObjects[0] = mf_CreateObject(2904, 1277.5, 2529.6, 16.9, 0.0, 90.0, 90.0, 2.0);
		MapFixObjects[1] = mf_CreateObject(2634, 1276.4, 2532.7, 16.8, 0.0, 0.0, 128.0, 2.0);
		MapFixObjects[2] = mf_CreateObject(2634, 1276.4, 2526.5, 16.8, 0.0, 0.0, 52.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_2
		MapFixObjects[3] = mf_CreateObject(3037, 2005.9, -1592.1, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[4] = mf_CreateObject(3037, 2016.7, -1592.2, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[5] = mf_CreateObject(3037, 1996.6, -1592.0, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[6] = mf_CreateObject(3037, 1993.3, -1588.7, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[7] = mf_CreateObject(3037, 1953.0, -1558.2, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[8] = mf_CreateObject(3037, 1939.5, -1558.8, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[9] = mf_CreateObject(5066, 1935.0, -1555.1, 16.16, 0.0, 270.0, 45.0, 4.0);
		MapFixObjects[10] = mf_CreateObject(3037, 1957.7, -1590.1, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[11] = mf_CreateObject(3037, 1950.2, -1582.6, 16.13, 0.0, 270.0, 45.0, 6.0);
		MapFixObjects[12] = mf_CreateObject(3117, 1967.85, -1590.1, 16.2, 0.0, 0.0, 315.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_3
		MapFixObjects[13] = mf_CreateObject(1555, -1377.2998, 492.5, 5.6, 0.0, 0.0, 90.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_4
		MapFixObjects[14] = mf_CreateObject(3037, -653.3, -1693.5, 40.0, 345.0, 340.0, 86.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[14], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[15] = mf_CreateObject(3037, -643.8, -1694.2, 43.0, 345.0, 345.0, 87.0, 6.0);
		MapFixObjects[16] = mf_CreateObject(3037, -662.7, -1693.0, 38.0, 345.0, 337.0, 85.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_5
		MapFixObjects[17] = mf_CreateObject(1566, -1406.7, 1.4, 6.5, 0.0, 0.0, 0.0, 2.0);
		MapFixObjects[18] = mf_CreateObject(1566, -1406.7, 1.4, 9.4, 0.0, 0.0, 0.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_6
		MapFixObjects[19] = mf_CreateObject(3037, 1908.5, 2222.8, 14.1, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[20] = mf_CreateObject(3037, 1908.5, 2211.9, 14.1, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[21] = mf_CreateObject(3037, 1908.5, 2201.1, 14.1, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[22] = mf_CreateObject(3037, 1843.6, 2201.1, 14.1, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[23] = mf_CreateObject(3037, 1843.6, 2211.9, 14.1, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[24] = mf_CreateObject(3037, 1843.6, 2222.8, 14.1, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[25] = mf_CreateObject(3037, 1855.3, 2234.5, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[26] = mf_CreateObject(3037, 1865.7, 2235.1, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[27] = mf_CreateObject(3037, 1876.1, 2235.1, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[28] = mf_CreateObject(3037, 1886.5, 2235.1, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[29] = mf_CreateObject(3037, 1896.9, 2234.5, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[30] = mf_CreateObject(3037, 1855.3, 2189.5, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[31] = mf_CreateObject(3037, 1865.7, 2189.5, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[32] = mf_CreateObject(3037, 1876.1, 2189.5, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[33] = mf_CreateObject(3037, 1886.5, 2189.5, 14.1, 0.0, 270.0, 90.0, 6.0);
		MapFixObjects[34] = mf_CreateObject(3037, 1896.9, 2189.5, 14.1, 0.0, 270.0, 90.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_7
		MapFixObjects[35] = mf_CreateObject(3117, 2104.2, 2367.6, 11.9, 0.0, 90.0, 90.0, 3.0);
		MapFixObjects[36] = mf_CreateObject(1566, 2115.2, 2368.8, 12.0, 0.0, 0.0, 0.0, 2.0);
		MapFixObjects[37] = mf_CreateObject(1566, 2115.2, 2367.3, 12.0, 0.0, 0.0, 90.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_8
		MapFixObjects[38] = mf_CreateObject(3084, -1613.0, 621.7, 37.0, 0.0, 2.0, 226.6, 7.0);
		mf_SetObjectMaterial(MapFixObjects[38], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[39] = mf_CreateObject(3084, -1604.5, 630.7, 37.3, 0.0, 2.0, 226.6, 7.0);
		mf_SetObjectMaterial(MapFixObjects[39], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[40] = mf_CreateObject(3084, -1596.0, 639.7, 37.7, 0.0, 2.0, 226.6, 7.0);
		mf_SetObjectMaterial(MapFixObjects[40], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[41] = mf_CreateObject(3084, -1587.5, 648.7, 38.1, 0.0, 2.0, 226.6, 7.0);
		mf_SetObjectMaterial(MapFixObjects[41], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[42] = mf_CreateObject(3084, -1579.7, 656.9, 38.5, 0.0, 2.0, 226.6, 7.0);
		mf_SetObjectMaterial(MapFixObjects[42], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_9
		MapFixObjects[43] = mf_CreateObject(10558, -1955.0, 745.9, 49.0, 90.0, 90.0, 90.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_10
		MapFixObjects[44] = mf_CreateObject(8957, -2060.3, 474.3, 36.0, 0.0, 0.0, 270.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_11
		MapFixObjects[45] = mf_CreateObject(5066, 394.6, -2053.2, 10.0, 0.0, 0.0, 0.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[45], 0, 0, "none", "none", 0x00000000);
		mf_SetObjectMaterial(MapFixObjects[45], 1, 0, "none", "none", 0x00000000);
		MapFixObjects[46] = mf_CreateObject(2957, 396.71, -2056.7, 10.0, 0.0, 0.0, 0.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[46], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_12
		MapFixObjects[47] = mf_CreateObject(2904, 2181.7, -2218.2, 15.5, 0.0, 0.0, 45.0, 2.0);
		MapFixObjects[48] = mf_CreateObject(2904, 2188.9, -2210.9, 15.5, 0.0, 0.0, 45.0, 2.0);
		MapFixObjects[49] = mf_CreateObject(2904, 2196.5, -2203.7, 15.5, 0.0, 0.0, 45.0, 2.0);
		MapFixObjects[50] = mf_CreateObject(2904, 2203.8, -2197.2, 15.5, 0.0, 0.0, 45.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_13
		MapFixObjects[51] = mf_CreateObject(10558, 1038.2, -1383.0, 17.07, 0.0, 90.0, 90.0, 5.0);
		MapFixObjects[52] = mf_CreateObject(3117, 1041.95, -1383.0, 17.2, 0.0, 0.0, 315.0, 3.0);
		MapFixObjects[53] = mf_CreateObject(3117, 1042.9, -1381.9, 17.2, 0.0, 0.0, 90.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_14
		MapFixObjects[54] = mf_CreateObject(3084, 501.0, -1364.9, 20.3, 270.0, 0.0, 107.0, 7.0);
		MapFixObjects[55] = mf_CreateObject(3037, 503.5, -1375.0, 20.3, 0.0, 270.0, 17.0, 6.0);
		MapFixObjects[56] = mf_CreateObject(3037, 496.4, -1362.4, 20.3, 0.0, 270.0, 115.0, 6.0);
		MapFixObjects[57] = mf_CreateObject(3084, 507.4, -1386.0, 19.3, 270.0, 180.0, 285.0, 7.0);
		MapFixObjects[58] = mf_CreateObject(3084, 510.6, -1397.9, 19.3, 270.0, 180.0, 285.0, 7.0);
		MapFixObjects[59] = mf_CreateObject(3084, 507.6, -1404.7, 19.3, 270.0, 0.0, 194.5, 7.0);
		MapFixObjects[60] = mf_CreateObject(3055, 498.9, -1406.3, 19.3, 270.0, 0.0, 14.5, 5.0);
		MapFixObjects[61] = mf_CreateObject(3055, 481.4, -1412.1, 20.5, 270.0, 0.0, 20.5, 5.0);
		mf_SetObjectMaterial(MapFixObjects[61], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[62] = mf_CreateObject(3109, 484.8, -1413.2, 19.3, 0.0, 90.0, 20.5, 2.0);
		mf_SetObjectMaterial(MapFixObjects[62], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[63] = mf_CreateObject(3109, 479.6, -1415.2, 19.3, 0.0, 90.0, 20.5, 2.0);
		mf_SetObjectMaterial(MapFixObjects[63], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[64] = mf_CreateObject(3037, 471.0, -1417.1, 24.6, 0.0, 90.0, 300.5, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_15
		MapFixObjects[65] = mf_CreateObject(5066, -1540.2, -444.9, 6.7, 0.0, 0.0, 315.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[65], 0, 0, "none", "none", 0x00000000);
		mf_SetObjectMaterial(MapFixObjects[65], 1, 0, "none", "none", 0x00000000);
		MapFixObjects[66] = mf_CreateObject(10150, -1539.4, -440.9, 6.5, 0.0, 0.0, 45.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[66], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[67] = mf_CreateObject(10150, -1544.4, -445.8, 6.5, 0.0, 0.0, 45.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[67], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_16
		MapFixObjects[68] = mf_CreateObject(5066, -1230.8, 51.1, 14.7, 0.0, 0.0, 45.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[68], 0, 0, "none", "none", 0x00000000);
		mf_SetObjectMaterial(MapFixObjects[68], 1, 0, "none", "none", 0x00000000);
		MapFixObjects[69] = mf_CreateObject(10150, -1226.8, 50.3, 14.5, 0.0, 0.0, 315.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[69], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[70] = mf_CreateObject(10150, -1231.7, 55.1, 14.5, 0.0, 0.0, 315.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[70], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_17
		MapFixObjects[71] = mf_CreateObject(8957, 2781.0, -1358.6, 27.5, 0.0, 0.0, 0.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[71], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[72] = mf_CreateObject(8957, 2781.0, -1333.3, 33.0, 0.0, 0.0, 0.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[72], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[73] = mf_CreateObject(8957, 2781.2, -1306.4, 39.5, 0.0, 0.0, 0.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[73], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[74] = mf_CreateObject(8957, 2781.2, -1281.3, 45.5, 0.0, 0.0, 0.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[74], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_18
		MapFixObjects[75] = mf_CreateObject(3117, -2240.7, 80.5, 37.0, 0.0, 270.0, 45.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_19
		MapFixObjects[76] = mf_CreateObject(1566, -180.7, 1129.48, 20.8, 0.0, 0.0, 0.0, 2.0);
		MapFixObjects[77] = mf_CreateObject(3061, -180.7, 1128.4, 20.5, 0.0, 0.0, 0.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_20
		MapFixObjects[78] = mf_CreateObject(5846, 1547.4, 885.1, 7.88, 7.5, 90.0, 70.0, 12.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_21
		MapFixObjects[79] = mf_CreateObject(5846, -1883.4, 1436.6, -33.7, 357.5, 90.0, 60.0, 12.0);
		mf_SetObjectMaterial(MapFixObjects[79], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_22
		MapFixObjects[80] = mf_CreateObject(8957, 1771.9, 2166.1, 6.0, 0.0, 0.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_23
		MapFixObjects[81] = mf_CreateObject(7930, 839.69, -1062.65, 32.57, 0.0, 90.0, 34.997, 5.0);
		MapFixObjects[82] = mf_CreateObject(1965, 835.25, -1058.65, 32.66, 0.0, 90.0, 298.25, 2.0);
		MapFixObjects[83] = mf_CreateObject(3109, 834.67, -1056.3, 32.63, 0.0, 90.0, 264.0, 2.0);
		MapFixObjects[84] = mf_CreateObject(3109, 835.1, -1054.7, 32.63, 0.0, 90.0, 246.996, 2.0);
		MapFixObjects[85] = mf_CreateObject(1965, 836.8, -1052.6, 32.66, 0.0, 90.0, 219.5, 2.0);
		MapFixObjects[86] = mf_CreateObject(8957, 850.9, -1057.5, 29.2, 0.0, 90.0, 305.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_24
		MapFixObjects[87] = mf_CreateObject(3117, 689.6, -1424.4, 19.8, 0.0, 90.0, 0.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_25
		MapFixObjects[88] = CreateObject(8957, 681.4, -447.4, -26.7, 0.0, 270.0, 0.0, 5.0);
		MapFixObjects[89] = mf_CreateObject(8957, 680.35, -447.1, -25.0, 0.0, 0.0, 0.0, 5.0);
		MapFixObjects[90] = mf_CreateObject(8957, 682.7, -447.1, -25.0, 0.0, 0.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_26
		MapFixObjects[91] = mf_CreateObject(2904, 2344.1, 377.5, 25.568, 84.0, 0.0, 354.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[91], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_27
		MapFixObjects[92] = mf_CreateObject(3084, 1077.8, 1394.7, 6.4, 0.0, 0.0, 90.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_28
		MapFixObjects[93] = mf_CreateObject(8957, 2399.1, 2166.2, 13.4, 0.0, 270.0, 90.0, 5.0);
		MapFixObjects[94] = mf_CreateObject(8957, 2416.0, 2166.2, 13.4, 0.0, 270.0, 90.0, 5.0);
		MapFixObjects[95] = mf_CreateObject(8957, 2441.2, 2166.2, 13.4, 0.0, 270.0, 90.0, 5.0);
		MapFixObjects[96] = mf_CreateObject(8957, 2449.6, 2166.2, 13.4, 0.0, 270.0, 90.0, 5.0);
		MapFixObjects[97] = mf_CreateObject(8957, 2474.9, 2166.2, 13.4, 0.0, 270.0, 90.0, 5.0);
		MapFixObjects[98] = mf_CreateObject(8957, 2491.7, 2166.2, 13.4, 0.0, 270.0, 90.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_29
		MapFixObjects[99] = mf_CreateObject(3117, 2275.1, 2351.4, 18.67, 0.0, 0.0, 45.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_30
		MapFixObjects[100] = mf_CreateObject(2954, 539.85, 50.05, 21.6, 0.0, 0.0, 100.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[100], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_31
		MapFixObjects[101] = mf_CreateObject(5846, -2438.7, -1398.1, 338.95, 356.0, 252.0, 304.0, 12.0);
		MapFixObjects[102] = mf_CreateObject(5846, -2455.8, -1405.6, 342.2, 353.0, 249.0, 304.0, 12.0);
		MapFixObjects[103] = mf_CreateObject(8957, -2470.0, -1406.9, 345.5, 15.0, 115.0, 118.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_32
		MapFixObjects[104] = mf_CreateObject(3117, 1076.9, 1362.25, 12.3, 0.0, 90.0, 95.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_33
		MapFixObjects[105] = mf_CreateObject(3084, 2356.5, 1881.5, 14.1, 90.0, 0.0, 0.0, 7.0);
		MapFixObjects[106] = mf_CreateObject(3084, 2344.0, 1881.5, 14.1, 90.0, 0.0, 0.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_34
		MapFixObjects[107] = mf_CreateObject(10558, -2187.4, 715.9, 78.6, 0.0, 90.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_35
		MapFixObjects[108] = mf_CreateObject(8957, 2862.1, -1406.1, 16.2, 0.0, 88.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_36
		MapFixObjects[109] = mf_CreateObject(3117, 2823.5, -1597.4, 12.7, 0.0, 0.0, 68.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[109], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_37
		MapFixObjects[110] = mf_CreateObject(2938, 2863.7, -1439.5, 15.0, 0.0, 90.0, 0.0, 7.0);
		MapFixObjects[111] = mf_CreateObject(3117, 2864.2, -1446.2, 15.1, 0.0, 0.0, 50.0, 3.0);
		MapFixObjects[112] = mf_CreateObject(3117, 2864.1, -1432.6, 15.1, 0.0, 0.0, 132.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_38
		MapFixObjects[113] = mf_CreateObject(8957, 1003.8, -919.5, 45.5, 0.0, 90.0, 8.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_39
		MapFixObjects[114] = mf_CreateObject(3084, -2904.4, 1247.8, 5.75, 0.0, 0.0, 45.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[114], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[115] = mf_CreateObject(3084, -2860.1, 1256.8, 5.7, 0.0, 0.0, 45.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[115], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[116] = mf_CreateObject(3084, -2852.3, 1264.6, 5.7, 0.0, 0.0, 45.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[116], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_40
		MapFixObjects[117] = mf_CreateObject(3084, 966.3, -1038.8, 28.9, 270.0, 0.0, 90.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_41
		MapFixObjects[118] = mf_CreateObject(3084, 2403.5, 2661.2, 9.0, 296.0, 0.0, 354.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_42
		MapFixObjects[119] = mf_CreateObject(3117, 1198.5, -916.78, 45.0, 0.0, 0.0, 7.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_43
		MapFixObjects[120] = mf_CreateObject(3037, 428.1, -1641.33, 43.5, 90.0, 0.0, 311.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_44
		MapFixObjects[121] = mf_CreateObject(8957, 1301.9, -966.1, 36.2, 0.0, 0.0, 90.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_45
		MapFixObjects[122] = mf_CreateObject(3117, 2504.6, 1144.86, 18.5, 0.0, 90.0, 90.0, 3.0);
		MapFixObjects[123] = mf_CreateObject(3117, 2508.9, 1144.86, 18.5, 0.0, 90.0, 90.0, 3.0);
		MapFixObjects[124] = mf_CreateObject(3117, 2513.1, 1144.86, 18.5, 0.0, 90.0, 90.0, 3.0);
		MapFixObjects[125] = mf_CreateObject(3117, 2515.73, 1146.4, 18.5, 0.0, 90.0, 0.0, 3.0);
		MapFixObjects[126] = mf_CreateObject(3117, 2515.73, 1152.5, 18.5, 0.0, 90.0, 0.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_46
		MapFixObjects[127] = mf_CreateObject(3084, -1869.2, -162.8, 16.6, 90.0, 0.0, 86.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_47
		MapFixObjects[128] = mf_CreateObject(3117, 1623.63, -1714.5, 25.1, 0.0, 0.0, 96.0, 3.0);
		MapFixObjects[129] = mf_CreateObject(3117, 1584.8, -1717.85, 25.1, 0.0, 0.0, 96.0, 3.0);
		MapFixObjects[130] = mf_CreateObject(3117, 1587.0, -1743.67, 25.1, 0.0, 0.0, 95.0, 3.0);
		MapFixObjects[131] = mf_CreateObject(3117, 1625.6, -1740.3, 25.1, 0.0, 0.0, 95.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_48
		MapFixObjects[132] = mf_CreateObject(3084, -1919.1, 699.6, 74.3, 90.0, 0.0, 0.0, 7.0);
		MapFixObjects[133] = mf_CreateObject(3117, -1913.3, 699.1, 74.3, 0.0, 180.0, 99.0, 3.0);
		MapFixObjects[134] = mf_CreateObject(3084, -1931.7, 699.6, 74.3, 90.0, 0.0, 0.0, 7.0);
		MapFixObjects[135] = mf_CreateObject(3117, -1939.2, 699.6, 74.3, 0.0, 180.0, 90.0, 3.0);
		MapFixObjects[136] = mf_CreateObject(3084, -1951.7, 699.5, 74.3, 90.0, 0.0, 0.0, 7.0);
		MapFixObjects[137] = mf_CreateObject(8957, -1940.7, 697.5, 71.8, 68.0, 0.0, 0.0, 5.0);
		MapFixObjects[138] = mf_CreateObject(8957, -1946.7, 697.5, 71.8, 68.0, 0.0, 0.0, 5.0);
		MapFixObjects[139] = mf_CreateObject(8957, -1943.8, 700.0, 70.3, 68.0, 90.0, 0.0, 5.0);
		MapFixObjects[140] = mf_CreateObject(8957, -1956.6, 697.5, 71.8, 68.0, 0.0, 0.0, 5.0);
		MapFixObjects[141] = mf_CreateObject(8957, -1962.6, 697.5, 71.8, 68.0, 0.0, 0.0, 5.0);
		MapFixObjects[142] = mf_CreateObject(8957, -1959.6, 700.0, 70.3, 68.0, 90.0, 0.0, 5.0);
		MapFixObjects[143] = mf_CreateObject(3084, -1984.3, 699.6, 74.3, 90.0, 0.0, 0.0, 7.0);
		MapFixObjects[144] = mf_CreateObject(3084, -1971.6, 699.6, 74.3, 90.0, 0.0, 0.0, 7.0);
		MapFixObjects[145] = mf_CreateObject(3117, -1964.1, 699.6, 74.3, 0.0, 180.0, 90.0, 3.0);
		MapFixObjects[146] = mf_CreateObject(3117, -1990.1, 699.1, 74.3, 0.0, 180.0, 82.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_49
		MapFixObjects[147] = mf_CreateObject(8957, -2456.6, 763.4, 43.0, 29.0, 0.0, 0.0, 5.0);
		MapFixObjects[148] = mf_CreateObject(8957, -2456.6, 772.0, 42.8, 44.0, 0.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_50
		MapFixObjects[149] = mf_CreateObject(3109, -1650.4, 537.3, 37.9, 0.0, 0.0, 46.0, 2.0);
		MapFixObjects[150] = mf_CreateObject(3109, -1674.3, 560.0, 37.9, 0.0, 0.0, 46.0, 2.0);
		MapFixObjects[151] = mf_CreateObject(9583, -1651.2, 537.6, 38.6, 0.0, 82.0, 46.5, 12.0);
		mf_SetObjectMaterial(MapFixObjects[151], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[152] = mf_CreateObject(9583, -1674.7, 560.65, 38.6, 0.0, 82.0, 46.5, 12.0);
		mf_SetObjectMaterial(MapFixObjects[152], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_51
		MapFixObjects[153] = mf_CreateObject(17540, 2735.7, -1267.8, 62.9, 0.0, 0.0, 90.0, 14.0);
		mf_SetObjectMaterial(MapFixObjects[153], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_52
		MapFixObjects[154] = mf_CreateObject(8957, -755.95, -1848.4, 14.3, 5.0, 335.0, 349.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[154], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_53
		MapFixObjects[155] = mf_CreateObject(906, -609.8, -1899.4, 3.5, 0.0, 10.0, 156.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[155], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_54
		MapFixObjects[156] = mf_CreateObject(2957, -405.0, 2242.8, 44.0, 0.0, 34.0, 285.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[156], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[157] = mf_CreateObject(2957, -405.4, 2244.5, 44.0, 0.0, 326.0, 285.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_55
		MapFixObjects[158] = mf_CreateObject(1965, -1119.3, 852.2, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[158], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[159] = mf_CreateObject(1965, -1121.03, 854.6, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[159], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[160] = mf_CreateObject(1965, -1121.95, 855.9, 31.1, 0.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[160], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[161] = mf_CreateObject(1965, -1131.0, 868.3, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[161], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[162] = mf_CreateObject(1965, -1129.24, 865.87, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[162], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[163] = mf_CreateObject(1965, -1128.3, 864.58, 31.1, 0.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[163], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[164] = mf_CreateObject(1965, -1134.4, 841.1, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[164], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[165] = mf_CreateObject(1965, -1136.17, 843.53, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[165], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[166] = mf_CreateObject(1965, -1136.5, 845.2, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[166], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[167] = mf_CreateObject(1965, -1146.15, 857.3, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[167], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[168] = mf_CreateObject(1965, -1144.38, 854.87, 30.5, 0.0, 90.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[168], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[169] = mf_CreateObject(1965, -1142.8, 854.0, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[169], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[170] = mf_CreateObject(1965, -1075.0, 889.9, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[170], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[171] = mf_CreateObject(1965, -1081.3, 898.7, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[171], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[172] = mf_CreateObject(1965, -1102.2, 870.1, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[172], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[173] = mf_CreateObject(1965, -1108.6, 878.8, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[173], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[174] = mf_CreateObject(1965, -1155.6, 831.3, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[174], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[175] = mf_CreateObject(1965, -1162.0, 840.0, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[175], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[176] = mf_CreateObject(1965, -1183.0, 811.4, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[176], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[177] = mf_CreateObject(1965, -1189.3, 820.2, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[177], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[178] = mf_CreateObject(1965, -1328.3, 705.8, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[178], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[179] = mf_CreateObject(1965, -1334.6, 714.6, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[179], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[180] = mf_CreateObject(1965, -1355.6, 686.0, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[180], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[181] = mf_CreateObject(1965, -1362.0, 694.7, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[181], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[182] = mf_CreateObject(1965, -1374.9, 672.0, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[182], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[183] = mf_CreateObject(1965, -1381.3, 680.7, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[183], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[184] = mf_CreateObject(1965, -1390.1, 660.9, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[184], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[185] = mf_CreateObject(1965, -1396.4, 669.7, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[185], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[186] = mf_CreateObject(1965, -1409.3, 647.0, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[186], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[187] = mf_CreateObject(1965, -1415.8, 655.7, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[187], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[188] = mf_CreateObject(1965, -1436.8, 627.0, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[188], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[189] = mf_CreateObject(1965, -1443.1, 635.8, 31.1, 90.0, 0.0, 306.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[189], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[190] = mf_CreateObject(3037, -1440.6, 630.9, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[190], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[191] = mf_CreateObject(3037, -1413.1, 650.9, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[191], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[192] = mf_CreateObject(3037, -1358.3, 690.7, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[192], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[193] = mf_CreateObject(3037, -1330.8, 710.7, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[193], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[194] = mf_CreateObject(3037, -1186.9, 815.3, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[194], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[195] = mf_CreateObject(3037, -1159.4, 835.2, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[195], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[196] = mf_CreateObject(3037, -1104.7, 875.0, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[196], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[197] = mf_CreateObject(3037, -1077.2, 895.0, 30.73, 0.0, 90.0, 36.0, 6.0);
		mf_SetObjectMaterial(MapFixObjects[197], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_56
		MapFixObjects[198] = mf_CreateObject(8957, 1725.6, 2122.3, 13.1, 0.0, 0.0, 90.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_57
		MapFixObjects[199] = mf_CreateObject(3084, 1682.3, 1379.8, 16.1, 0.0, 0.0, 299.0, 7.0);
		MapFixObjects[200] = mf_CreateObject(3084, 1672.8, 1396.5, 16.1, 0.0, 0.0, 300.5, 7.0);
		MapFixObjects[201] = mf_CreateObject(8957, 1677.5, 1388.2, 14.8, 0.0, 0.0, 30.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_58
		MapFixObjects[202] = mf_CreateObject(2904, 2143.0, 1621.3, 1001.9, 0.0, 0.0, 0.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[202], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_59
		MapFixObjects[203] = mf_CreateObject(3117, 1175.7, 1224.0, 13.5, 0.0, 90.0, 90.0, 3.0);
		MapFixObjects[204] = mf_CreateObject(2954, 1174.8, 1224.18, 13.2, 0.0, 0.0, 0.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[204], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[205] = mf_CreateObject(2954, 1174.7, 1224.18, 15.2, 0.0, 90.0, 0.0, 2.0);
		mf_SetObjectMaterial(MapFixObjects[205], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_60
		MapFixObjects[206] = mf_CreateObject(8957, -208.3, 1055.7, 22.8, 0.0, 90.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_61
		MapFixObjects[207] = mf_CreateObject(5846, 2319.5, 1733.2, 7.5, 7.5, 270.0, 0.0, 12.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_62
		MapFixObjects[208] = mf_CreateObject(8957, 2393.8, 1483.7, 9.7, 0.0, 90.0, 90.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_63
		MapFixObjects[209] = mf_CreateObject(8957, -2690.7, 518.5, 17.1, 0.0, 0.0, 90.0, 5.0);
		MapFixObjects[210] = mf_CreateObject(3084, -2688.8, 512.1, 20.2, 90.0, 0.0, 90.0, 7.0);
		MapFixObjects[211] = mf_CreateObject(3084, -2688.8, 499.6, 20.2, 90.0, 0.0, 90.0, 7.0);
		MapFixObjects[212] = mf_CreateObject(3084, -2688.8, 487.1, 20.2, 90.0, 0.0, 90.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_64
		MapFixObjects[213] = mf_CreateObject(3084, 1003.8, -1161.3, 27.0, 90.0, 0.0, 0.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_65
		MapFixObjects[214] = mf_CreateObject(3093, 807.0, -1032.6, 26.4, 0.0, 90.0, 26.0, 2.0);
		MapFixObjects[215] = mf_CreateObject(3093, 812.5, -1030.2, 26.4, 0.0, 90.0, 26.0, 2.0);
		MapFixObjects[216] = mf_CreateObject(3093, 818.1, -1027.4, 26.4, 0.0, 90.0, 26.3, 2.0);
		MapFixObjects[217] = mf_CreateObject(3093, 823.8, -1024.6, 26.4, 0.0, 90.0, 26.3, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_66
		MapFixObjects[218] = mf_CreateObject(3084, 1653.1, -1333.3, 108.7, 90.0, 0.0, 90.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[218], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[219] = mf_CreateObject(3084, 1653.1, -1353.3, 108.7, 90.0, 0.0, 90.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[219], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[220] = mf_CreateObject(3055, 1653.7, -1343.3, 108.7, 90.0, 0.0, 90.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[220], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[221] = mf_CreateObject(3084, 1681.5, -1324.9, 108.7, 90.0, 0.0, 0.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[221], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[222] = mf_CreateObject(3084, 1661.5, -1324.9, 108.7, 90.0, 0.0, 0.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[222], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[223] = mf_CreateObject(3055, 1671.5, -1325.5, 108.7, 90.0, 0.0, 0.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[223], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[224] = mf_CreateObject(3084, 1689.9, -1333.3, 108.7, 90.0, 0.0, 90.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[224], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[225] = mf_CreateObject(3084, 1689.9, -1353.3, 108.7, 90.0, 0.0, 90.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[225], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[226] = mf_CreateObject(3055, 1688.7, -1343.3, 108.7, 90.0, 0.0, 90.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[226], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[227] = mf_CreateObject(3084, 1661.5, -1361.7, 108.7, 90.0, 0.0, 0.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[227], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[228] = mf_CreateObject(3084, 1681.5, -1361.7, 108.7, 90.0, 0.0, 0.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[228], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[229] = mf_CreateObject(3055, 1671.5, -1360.5, 108.7, 90.0, 0.0, 0.0, 5.0);
		mf_SetObjectMaterial(MapFixObjects[229], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_67
		MapFixObjects[230] = mf_CreateObject(8957, 2194.0, -1099.1, 27.9, 0.0, 90.0, 66.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_68
		MapFixObjects[231] = mf_CreateObject(8957, 2617.6, 1078.2, 9.5, 0.0, 90.0, 0.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_69
		MapFixObjects[232] = mf_CreateObject(5020, 1838.5, 1285.5, 9.7, 0.0, 88.0, 0.0, 4.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_70
		MapFixObjects[233] = mf_CreateObject(4100, 1902.1, -1055.2, 24.11, 1.5, 0.0, 319.7, 8.0);
		mf_SetObjectMaterial(MapFixObjects[233], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_71
		MapFixObjects[234] = mf_CreateObject(3168, -640.5, 2717.2, 71.4, 0.0, 0.0, 42.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_72 //By Jimmi
		MapFixObjects[235] = mf_CreateObject(19866, 2417.3059, 2387.3217, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[236] = mf_CreateObject(19866, 2417.0258, 2387.3217, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[237] = mf_CreateObject(19866, 2417.3059, 2382.3217, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[238] = mf_CreateObject(19866, 2417.0258, 2382.3217, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[239] = mf_CreateObject(19866, 2417.3051, 2377.301, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[240] = mf_CreateObject(19866, 2417.031, 2377.3283, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[241] = mf_CreateObject(19866, 2417.3059, 2372.321, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[242] = mf_CreateObject(19866, 2417.031, 2372.3283, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[243] = mf_CreateObject(19866, 2417.3059, 2367.321, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[244] = mf_CreateObject(19866, 2417.031, 2367.3283, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[245] = mf_CreateObject(19866, 2417.3059, 2363.1809, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[246] = mf_CreateObject(19866, 2417.0258, 2363.1809, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[247] = mf_CreateObject(19866, 2417.0258, 2359.301, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[248] = mf_CreateObject(19866, 2417.3059, 2359.3007, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[249] = mf_CreateObject(19866, 2417.3059, 2397.062, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[250] = mf_CreateObject(19866, 2417.3059, 2400.7619, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[251] = mf_CreateObject(19866, 2417.0258, 2397.0617, 9.8151, 0.0, 0.0, 0.0, 3.0);
		MapFixObjects[252] = mf_CreateObject(19866, 2417.0258, 2400.7817, 9.8151, 0.0, 0.0, 0.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_73
		MapFixObjects[253] = mf_CreateObject(1498, 2523.6, -1680.1, 1014.5, 0.0, 0.0, 90.0, 2.0);
		MapFixObjects[254] = CreateObject(2938, 2527.4, -1683.4, 1014.4, 0.0, 270.0, 90.0, 7.0);
		MapFixObjects[255] = mf_CreateObject(5020, 2525.9, -1681.1, 1016.2, 0.0, 0.0, 90.0, 4.0);
		MapFixObjects[256] = mf_CreateObject(5020, 2527.8, -1684.2, 1016.2, 0.0, 0.0, 180.0, 4.0);
		MapFixObjects[257] = mf_CreateObject(5020, 2533.3, -1684.1, 1016.2, 0.0, 0.0, 180.0, 4.0);
		MapFixObjects[258] = mf_CreateObject(5020, 2530.5, -1685.9, 1016.2, 0.0, 0.0, 270.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[258], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[259] = mf_CreateObject(3117, 2532.0, -1681.1, 1016.2, 0.0, 89.0, 90.0, 3.0);
		MapFixObjects[260] = mf_CreateObject(3117, 2532.0, -1681.1, 1016.2, 0.0, 89.0, 270.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_74 //By Apec
		MapFixObjects[261] = mf_CreateObject(19464, 293.1319, -1616.1232, 34.7051, 0.0, 0.0, 80.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[261], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[262] = mf_CreateObject(19464, 287.3273, -1615.127, 34.7051, 0.0, 0.0, 80.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[262], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[263] = mf_CreateObject(19464, 281.4706, -1614.0808, 34.7051, 0.0, 0.0, 80.0, 4.0);
		mf_SetObjectMaterial(MapFixObjects[263], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_75
		MapFixObjects[264] = mf_CreateObject(5020, 1861.7, 1363.3, 55.8, 0.0, 0.0, 90.0, 4.0);
		MapFixObjects[265] = mf_CreateObject(5020, 1907.2, 1298.8, 55.8, 0.0, 0.0, 180.0, 4.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_76
		MapFixObjects[266] = mf_CreateObject(3037, 1714.5, 896.8, 14.3, 0.0, 270.0, 0.0, 6.0);
		MapFixObjects[267] = mf_CreateObject(3037, 1714.5, 906.3, 14.3, 0.0, 270.0, 0.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_77
		MapFixObjects[268] = mf_CreateObject(3084, -880.8, 1640.2, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[268], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[269] = mf_CreateObject(3084, -887.22, 1650.9, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[269], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[270] = mf_CreateObject(3084, -893.65, 1661.6, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[270], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[271] = mf_CreateObject(3084, -900.07, 1672.3, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[271], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[272] = mf_CreateObject(3084, -906.5, 1683.0, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[272], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[273] = mf_CreateObject(3084, -870.1, 1646.8, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[273], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[274] = mf_CreateObject(3084, -876.53, 1657.5, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[274], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[275] = mf_CreateObject(3084, -882.95, 1668.2, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[275], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[276] = mf_CreateObject(3084, -889.37, 1678.9, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[276], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[277] = mf_CreateObject(3084, -895.8, 1689.6, 25.3, 0.0, 0.0, 301.0, 7.0);
		mf_SetObjectMaterial(MapFixObjects[277], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_78
		MapFixObjects[278] = mf_CreateObject(1566, -2105.4, -498.5, 49.5, 0.0, 0.0, 90.0, 2.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_79
		MapFixObjects[279] = mf_CreateObject(10149, -2557.25, 991.2, 80.6, 90.0, 0.0, 0.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[279], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[280] = mf_CreateObject(10149, -2557.25, 986.4, 80.6, 90.0, 0.0, 0.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[280], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[281] = mf_CreateObject(10149, -2557.25, 984.9, 82.3, 0.0, 0.0, 0.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[281], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[282] = mf_CreateObject(10149, -2557.25, 992.6, 82.3, 0.0, 0.0, 0.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[282], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_80
		MapFixObjects[283] = mf_CreateObject(10184, -2283.1, 916.7, 85.5, 270.0, 0.0, 0.0, 9.0);
		MapFixObjects[284] = mf_CreateObject(2957, -2281.1, 916.6, 94.4, 90.0, 0.0, 90.0, 3.0);
		MapFixObjects[285] = mf_CreateObject(2957, -2283.0, 916.5, 75.2, 0.0, 0.0, 90.0, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_81 //By ziggi
		MapFixObjects[286] = mf_CreateObject(1498, 2401.755, -1714.5, 13.1243, 0.0, 0.0, 0.0, 150.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_82
		MapFixObjects[287] = mf_CreateObject(3084, 2235.9, 2233.2, 14.1, 270.0, 0.0, 90.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_83
		MapFixObjects[288] = mf_CreateObject(3037, -2313.0, 771.7, 57.0, 0.0, 0.0, 358.5, 6.0);
		MapFixObjects[289] = mf_CreateObject(3037, -2313.3, 761.5, 57.0, 0.0, 0.0, 358.5, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_84
		MapFixObjects[290] = mf_CreateObject(3037, 1082.5, -1191.5, 19.1, 0.0, 0.0, 0.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_85
		MapFixObjects[291] = mf_CreateObject(3037, 1227.2, 312.9, 21.1, 0.0, 270.0, 66.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_86
		MapFixObjects[292] = mf_CreateObject(1966, -2730.6, 834.9, 59.8, 0.0, 0.0, 90.0, 5.0);
		MapFixObjects[293] = mf_CreateObject(1966, -2734.15, 831.4, 59.8, 0.0, 0.0, 0.0, 5.0);
		MapFixObjects[294] = mf_CreateObject(1966, -2734.0, 838.4, 59.8, 0.0, 0.0, 0.0, 5.0);
		MapFixObjects[295] = mf_CreateObject(5340, -2737.7, 835.0, 60.6, 0.0, 0.0, 0.0, 4.0);
		MapFixObjects[296] = mf_CreateObject(3117, -2737.7, 833.3, 59.2, 0.0, 0.0, 90.0, 3.0);
		MapFixObjects[297] = mf_CreateObject(1966, -2735.38, 835.0, 58.8, 270.0, 0.0, 90.0, 5.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_87
		MapFixObjects[298] = mf_CreateObject(3084, 1328.7, -997.1, 47.1, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[299] = mf_CreateObject(3084, 1328.7, -984.8, 47.1, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[300] = mf_CreateObject(3084, 1328.7, -972.4, 47.1, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[301] = mf_CreateObject(3084, 1322.5, -1003.2, 47.1, 0.0, 0.0, 0.0, 7.0);
		MapFixObjects[302] = mf_CreateObject(3084, 1310.6, -1003.2, 47.1, 0.0, 0.0, 0.0, 7.0);
		MapFixObjects[303] = mf_CreateObject(3084, 1304.4, -997.1, 47.1, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[304] = mf_CreateObject(3084, 1304.4, -984.7, 47.1, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[305] = mf_CreateObject(3084, 1304.4, -972.4, 47.1, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[306] = mf_CreateObject(3084, 1310.6, -966.2, 47.1, 0.0, 0.0, 0.0, 7.0);
		MapFixObjects[307] = mf_CreateObject(3084, 1322.5, -966.2, 47.1, 0.0, 0.0, 0.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_88
		MapFixObjects[308] = mf_CreateObject(3084, -1800.3, 1516.5, -32.9, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[309] = mf_CreateObject(3084, -1800.3, 1504.1, -32.9, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[310] = mf_CreateObject(3084, -1800.3, 1491.7, -32.9, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[311] = mf_CreateObject(3084, -1820.2, 1447.4, -32.7, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[312] = mf_CreateObject(3084, -1820.2, 1459.8, -32.7, 0.0, 0.0, 90.0, 7.0);
		MapFixObjects[313] = mf_CreateObject(3084, -1820.2, 1472.2, -32.7, 0.0, 0.0, 90.0, 7.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_89
		MapFixObjects[314] = mf_CreateObject(3037, -2340.1, 796.8, 51.8, 90.0, 0.0, 90.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_90
		MapFixObjects[315] = mf_CreateObject(3117, 2091.1, -2079.3, 25.1, 90.0, 0.0, 90.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[315], 0, 0, "none", "none", 0x00000000);
		MapFixObjects[316] = mf_CreateObject(3117, 2009.6, -2079.3, 25.1, 90.0, 0.0, 90.0, 3.0);
		mf_SetObjectMaterial(MapFixObjects[316], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_91 //By KinG7
		MapFixObjects[317] = mf_CreateObject(1501, -383.46, -1439.64, 25.33, 0.0, 0.0, 90.0, 150.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_92 //By KinG7
		MapFixObjects[318] = mf_CreateObject(1505, -2574.495, 1153.023, 54.669, 0.0, 0.0, -19.444, 150.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_93 //By KinG7
		MapFixObjects[319] = mf_CreateObject(1496, -1800.7048, 1201.0538, 24.1293, 0.0, 0.0, 0.0, 150.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_94 //By KinG7
		MapFixObjects[320] = mf_CreateObject(1522, -1390.7784, 2639.247, 54.9744, 0.0, 0.0, 0.0, 150.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_95 //By KinG7
		MapFixObjects[321] = mf_CreateObject(1498, 2038.036, 2721.37, 10.53, 0.0, 0.0, -180.0, 150.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_96
		MapFixObjects[322] = mf_CreateObject(2957, -2550.5, 193.9, 6.5, 0.0, 0.0, 285.5, 3.0);
		mf_SetObjectMaterial(MapFixObjects[322], 0, 0, "none", "none", 0x00000000);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_97
		MapFixObjects[323] = mf_CreateObject(3037, 875.6, -1565.0, 16.7, 0.0, 270.0, 293.0, 6.0);
		MapFixObjects[324] = mf_CreateObject(2957, 869.3, -1567.8, 16.7, 90.0, 0.0, 12.0, 3.0);
		MapFixObjects[325] = mf_CreateObject(2957, 882.3, -1562.4, 16.7, 90.0, 0.0, 31.997, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_98
		MapFixObjects[326] = mf_CreateObject(3037, 770.2, -3.6, 1004.2, 0.0, 90.0, 90.0, 6.0);
		MapFixObjects[327] = mf_CreateObject(3037, 760.0, -3.6, 1004.2, 0.0, 90.0, 90.0, 6.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_99
		MapFixObjects[328] = mf_CreateObject(3117, 145.2, -199.6, 3.95, 0.0, 0.0, 315.25, 3.0);
	#endif
	#if !defined DISABLE_MAPFIX_PLACE_100
		MapFixObjects[329] = mf_CreateObject(10184, 353.2, -1603.6, 36.1, 0.0, 90.0, 88.25, 9.0);
		MapFixObjects[330] = mf_CreateObject(10184, 370.2, -1604.1, 36.1, 0.0, 90.0, 88.25, 9.0);
		MapFixObjects[331] = mf_CreateObject(10184, 387.2, -1604.6, 36.1, 0.0, 90.0, 88.25, 9.0);
	#endif
	#undef mf_CreateObject
	#undef mf_SetObjectMaterial
	return 1;
}

DestroyMapFixObjects()
{
	for(new i = sizeof(MapFixObjects) - 1; i != -1; --i)
	{
		if(MapFixObjects[i])
		{
			#if defined USE_INCOGNITO_STREAMER\
			&& defined DestroyDynamicObject
				if(i == 88) DestroyObject(MapFixObjects[88]);
				else DestroyDynamicObject(MapFixObjects[i]);
			#else
				DestroyObject(MapFixObjects[i]);
			#endif
			MapFixObjects[i] = 0;
		}
	}
	return 1;
}
