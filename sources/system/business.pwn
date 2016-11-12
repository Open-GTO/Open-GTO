/*
	Created by Peter Steenbergen
	Modified by ZiGGi
*/

#if defined _business_included
	#endinput
#endif

#define _business_included

enum BusinessInfo {
	Business_Name[MAX_NAME],
	Business_Cost,
	Business_Value,
	Business_Level, // minumum level
	Business_Owner[MAX_PLAYER_NAME + 1],
	Business_Buyout, // Buyout price
	Business_Vault,
	Business_Upgrade,
	Business_ShowIcon,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z
}

new Businesses[][BusinessInfo] = {
	//NAME, COST, VALUE, LEVEL, OWNER, BUYOUT, upgrade, icon, x, y, z
	{"Alhambra", 500000, 2000, 10, "Unknown", 0, 0, 1, 0, 502.6747, -11.6962, 1000.6796},
	{"Atrium", 500000, 3000, 5, "Unknown", 0, 0, 1, 0, 1729.1752, -1669.0836, 22.6151},
	{"Avispa Country Club", 1500000, 9500, 10, "Unknown", 0, 0, 1, 1, -2720.3601, -318.1620, 7.8438},
	{"Bandits Stadium", 500000, 2000, 5, "Unknown", 0, 0, 1, 1, 1477.7498, 2248.6311, 11.0234},
	{"Below the Belt Gym", 150000, 1500, 5, "Unknown", 0, 0, 1, 0, 773.9650, -72.9702, 1000.6484},
	{"Biffin Bridge Hotel", 500000, 3000, 5, "Unknown", 0, 0, 1, 1, -2462.5876, 132.7333, 35.1719},
	{"Binco Clothing", 95000, 1000, 5, "Unknown", 0, 0, 1, 0, 207.6300, -98.0614, 1005.2578},
	{"Blueberry Liquor Store", 80000, 800, 1, "Unknown", 0, 0, 1, 1, 251.5441, -56.6669, 1.5703},
	{"Burger Shot", 150000, 800, 1, "Unknown", 0, 0, 1, 0, 372.8161, -65.8081, 1001.5078},
	{"Caligula's Palace", 5000000, 20000, 20, "Unknown", 0, 0, 1, 0, 2235.7639, 1677.0742, 1008.3594},
	{"Casino Floor", 2000000, 10000, 10, "Unknown", 0, 0, 1, 0, 1130.6415, -1.7442, 1000.6797},
	{"City Planning Department", 350000, 1800, 5, "Unknown", 0, 0, 1, 0, 357.2710, 173.6531, 1008.3818},
	{"Cluckin' Bell", 100000, 500, 1, "Unknown", 0, 0, 1, 0, 380.3091, -8.7951, 1001.8516},
	{"Cobra Marital Arts Gym", 150000, 1000, 1, "Unknown", 0, 0, 1, 0, 768.2480, -36.7117, 1000.6865},
	{"Coutt and Schutz", 180000, 1800, 5, "Unknown", 0, 0, 1, 1, 2131.8169, -1148.7053, 24.3476},
	{"Da-Nang Boys Freighter", 300000, 2200, 5, "Unknown", 0, 0, 1, 1, -2473.4648, 1552.8909, 33.2273},
	{"Didier Sachs Clothing", 250000, 2500, 5, "Unknown", 0, 0, 1, 0, 204.6664, -156.9822, 1000.5234},
	{"Easter Bay Chemicals", 850000, 3300, 5, "Unknown", 0, 0, 1, 1, -1055.0157, -696.3296, 32.3516},
	{"Ferris Wheel", 275000, 1200, 1, "Unknown", 0, 0, 1, 1, 385.6278, -2028.7147, 7.8359},
	{"Ganton Gym", 125000, 1000, 1, "Unknown", 0, 0, 1, 0, 760.6678, 5.4534, 1000.7083},
	{"Greenglass College", 500000, 5000, 5, "Unknown", 0, 0, 1, 1, 1045.7098, 1014.6323, 11.0000},
	{"Hemlock Tattoo", 50000, 500, 1, "Unknown", 0, 0, 1, 0, -201.6768, -8.6403, 1002.2734},
	{"Hippy Shopper", 500000, 3000, 5, "Unknown", 0, 0, 1, 1, -2590.0884, 59.8131, 4.3359},
	{"Inside Track", 300000, 2800, 5, "Unknown", 0, 0, 1, 0, 823.2686, 3.0071, 1004.1797},
	{"Jefferson Motel", 300000, 2400, 5, "Unknown", 0, 0, 1, 0, 2222.0686, -1150.7019, 1025.7969},
	{"Jizzy's Pleasure Domes", 375000, 3000, 5, "Unknown", 0, 0, 1, 0, -2652.5295, 1410.2631, 906.2734},
	{"Lil' Probe'Inn", 100000, 1800, 5, "Unknown", 0, 0, 1, 0, -217.4339, 1402.6681, 27.7734},
	{"Linden Station", 500000, 5000, 5, "Unknown", 0, 0, 1, 1, 2840.1270, 1290.5465, 11.3906},
	{"Macisla Unisex Hair Salon", 50000, 1000, 5, "Unknown", 0, 0, 1, 0, 412.0497, -49.3973, 1001.9025},
	{"Marco's Bistro", 800000, 3000, 10, "Unknown", 0, 0, 1, 0, -789.2899, 510.2562, 1367.3745},
	{"Nude Strippers Daily", 200000, 1800, 5, "Unknown", 0, 0, 1, 0, 1207.9760, -31.6712, 1000.9531},
	{"Ocean Docks Warehouse", 400000, 2800, 5, "Unknown", 0, 0, 1, 1, 2129.5725, -2281.6481, 20.6643},
	{"Otto's Autos", 500000, 2500, 5, "Unknown", 0, 0, 1, 1, -1657.8228, 1207.0295, 7.2500},
	{"Pro-Laps Clothing", 165000, 1500, 5, "Unknown", 0, 0, 1, 0, 207.0665, -127.5535, 1003.5078},
	{"Reece's Hair Facial Studio", 50000, 900, 1, "Unknown", 0, 0, 1, 0, 411.7967, -13.2574, 1001.8047},
	{"Rodeo Hotel", 915000, 4500, 5, "Unknown", 0, 0, 1, 1, 328.6830, -1512.8302, 36.0391},
	{"Sex-Shop", 200000, 1500, 5, "Unknown", 0, 0, 1, 0, -103.9304, -22.4892, 1000.7188},
	{"Sherman Dam", 500000, 3000, 5, "Unknown", 0, 0, 1, 0, -961.3422, 1946.3208, 9.0000},
	{"Sub-Urban Clothing", 130000, 1300, 5, "Unknown", 0, 0, 1, 0, 204.1066, -40.7047, 1001.8047},
	{"Ten Green Bottles", 200000, 1500, 5, "Unknown", 0, 0, 1, 0, 501.5465, -75.2806, 998.7578},
	{"The Emerald Isle", 975000, 4800, 5, "Unknown", 0, 0, 1, 1, 2127.6370, 2371.0024, 10.8203},
	{"The Four Dragons Casino", 4250000, 18000, 20, "Unknown", 0, 0, 1, 0, 1993.5176, 1017.8817, 994.8906},
	{"The Pig Pen", 500000, 2800, 5, "Unknown", 0, 0, 1, 0, 1210.7899, 4.4515, 1000.9219},
	{"The Visage", 1000000, 5000, 5, "Unknown", 0, 0, 1, 1, 2021.4015, 1916.4310, 12.3403},
	{"The Well Stacked Pizza", 200000, 1000, 1, "Unknown", 0, 0, 1, 0, 375.9720, -119.2062, 1001.4995},
	{"Unity Station", 330000, 2400, 5, "Unknown", 0, 0, 1, 1, 1731.6018, -1912.0779, 13.5625},
	{"Verdant Meadows Airfield", 100000, 1500, 5, "Unknown", 0, 0, 1, 0, 419.2619, 2536.4739, 10.0000},
	{"Victim Clothing", 200000, 2000, 5, "Unknown", 0, 0, 1, 0, 204.5330, -7.8327, 1001.2109},
	{"Vinewood Tennis Courts", 500000, 5000, 5, "Unknown", 0, 0, 1, 1, 726.4949, -1276.3582, 13.6484},
	{"Wang Cars", 750000, 3000, 5, "Unknown", 0, 0, 1, 1, -1952.3914, 293.8974, 35.4688},
	{"Yellow Bell Golf-Club", 500000, 5000, 5, "Unknown", 0, 0, 1, 1, 1456.5182, 2773.3857, 10.8203},
	{"Zero RC", 180000, 1800, 5, "Unknown", 0, 0, 1, 0, -2230.7651, 132.2124, 1035.4141},
	{"Zip Clothing", 180000, 1800, 5, "Unknown", 0, 0, 1, 0, 161.3900, -79.9809, 1001.8047},
	{"Zombotech Corporation", 500000, 3000, 5, "Unknown", 0, 0, 1, 1, -1951.7206, 689.0686, 46.5625},
	{"Crack Den", 150000, 2400, 8, "Unknown", 0, 0, 1, 0, 323.3233, 1118.7882, 1083.8828},
	{"Big Smoke's Crack Palace", 10000000, 24000, 25, "Unknown", 0, 0, 1, 0, 2546.8057, -1281.2859, 1060.9844},
	{"B-Dup's Crack Palace", 1000000, 3400, 10, "Unknown", 0, 0, 1, 0, 1526.0604, -45.5709, 1002.1310}
};

#define SetPlayerToBusinessID(%0,%1) SetPVarInt(%0, "BusID",%1)
#define GetPlayerToBusinessID(%0) GetPVarInt(%0, "BusID")
new
	business_icon[ sizeof(Businesses) ],
	Float:BusinessDistanceOfShowLabel=20.0;

static
	Text3D:gLabelID[ sizeof(Businesses) ][MAX_PLAYERS];

stock business_LoadConfig(file_config)
{
	ini_getString(file_config, "Business_DB", db_business);
	ini_getFloat(file_config, "Business_DistanceOfShowLabel", BusinessDistanceOfShowLabel);
}

stock business_SaveConfig(file_config)
{
	ini_setString(file_config, "Business_DB", db_business);
	ini_setFloat(file_config, "Business_DistanceOfShowLabel", BusinessDistanceOfShowLabel);
}

stock business_LoadAll()
{
	new file_business, db_businessname[MAX_STRING];
	for (new i = 0;i < sizeof(Businesses); i++)
	{
		format(db_businessname, sizeof(db_businessname), "%s%s"DATA_FILES_FORMAT, db_business, Businesses[i][Business_Name]);
		if (!ini_fileExist(db_businessname))
		{
			continue;
		}
		file_business = ini_openFile(db_businessname);
		ini_getString(file_business, "Name", Businesses[i][Business_Name], MAX_NAME);
		ini_getInteger(file_business, "Cost", Businesses[i][Business_Cost]);
		ini_getInteger(file_business, "Value", Businesses[i][Business_Value]);
		ini_getInteger(file_business, "Level", Businesses[i][Business_Level]);
		ini_getString(file_business, "Owner", Businesses[i][Business_Owner], MAX_PLAYER_NAME);
		ini_getInteger(file_business, "Buyout", Businesses[i][Business_Buyout]);
		ini_getInteger(file_business, "Vault", Businesses[i][Business_Vault]);
		ini_getInteger(file_business, "Upgrade", Businesses[i][Business_Upgrade]);
		ini_getInteger(file_business, "ShowIcon", Businesses[i][Business_ShowIcon]);
		ini_getFloat(file_business, "Coord_X", Businesses[i][Coord_X]);
		ini_getFloat(file_business, "Coord_Y", Businesses[i][Coord_Y]);
		ini_getFloat(file_business, "Coord_Z", Businesses[i][Coord_Z]);
		ini_closeFile(file_business);
	}
	return;
}

stock business_SaveAll()
{
	new file_business, db_businessname[MAX_STRING];
	for (new i = 0; i < sizeof(Businesses); i++)
	{
		format(db_businessname, sizeof(db_businessname), "%s%s"DATA_FILES_FORMAT, db_business, Businesses[i][Business_Name]);
		file_business = (!ini_fileExist(db_businessname)) ? ini_createFile(db_businessname) : ini_openFile(db_businessname);
		ini_setString(file_business, "Name", Businesses[i][Business_Name]);
		ini_setInteger(file_business, "Cost", Businesses[i][Business_Cost]);
		ini_setInteger(file_business, "Value", Businesses[i][Business_Value]);
		ini_setInteger(file_business, "Level", Businesses[i][Business_Level]);
		ini_setString(file_business, "Owner", Businesses[i][Business_Owner]);
		ini_setInteger(file_business, "Buyout", Businesses[i][Business_Buyout]);
		ini_setInteger(file_business, "Vault", Businesses[i][Business_Vault]);
		ini_setInteger(file_business, "Upgrade", Businesses[i][Business_Upgrade]);
		ini_setInteger(file_business, "ShowIcon", Businesses[i][Business_ShowIcon]);
		ini_setFloat(file_business, "Coord_X", Businesses[i][Coord_X]);
		ini_setFloat(file_business, "Coord_Y", Businesses[i][Coord_Y]);
		ini_setFloat(file_business, "Coord_Z", Businesses[i][Coord_Z]);
		ini_closeFile(file_business);
	}
	return;
}

business_OnGameModeInit()
{
	business_LoadAll();

	new
		icon_type;

	for (new id = 0; id < sizeof(Businesses); id++)
	{
		CreateDynamicPickup(1274, 49, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z], -1);

		if (Businesses[id][Business_ShowIcon] == 1)
		{
			if (strcmp(Businesses[id][Business_Owner], "Unknown")) {
				icon_type = 36;
			} else {
				icon_type = 52;
			}
			business_icon[id] = CreateDynamicMapIcon(Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z], icon_type, 0);
		}
	}
	Log_Init("system", "Business module init.");
}


public OnPlayerLogin(playerid)
{
	for (new id = 0; id < sizeof(Businesses); id++) {
		business_CreatePlayerLabel(playerid, id);
	}
	#if defined Business_OnPlayerLogin
		return Business_OnPlayerLogin(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerLogin
	#undef OnPlayerLogin
#else
	#define _ALS_OnPlayerLogin
#endif

#define OnPlayerLogin Business_OnPlayerLogin
#if defined Business_OnPlayerLogin
	forward Business_OnPlayerLogin(playerid);
#endif

business_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new id = 0; id < sizeof(Businesses); id++) {
		business_DestroyPlayerLabel(playerid, id);
	}
}

stock IsPlayerAtBusiness(playerid)
{
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

stock GetBusinessID(playerid)
{
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]))
		{
			return id;
		}
	}
	return -1;
}

stock TurnAround()
{
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		if (strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			if (Businesses[id][Business_Vault] < Businesses[id][Business_Value] * Businesses[id][Business_Upgrade] * 50)
			{
				Businesses[id][Business_Vault] += (Businesses[id][Business_Value] * Businesses[id][Business_Upgrade]) / 1000;
			}
			else
			{
				new playerid;

				if (sscanf(Businesses[id][Business_Owner], "u", playerid)) {
					continue;
				}

				new amount = Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade];
				GivePlayerBankMoney(playerid, amount);
				Lang_SendText(playerid, "BUSINESS_COLLECT", Businesses[id][Business_Name], amount);
				Businesses[id][Business_Vault] = 0;
			}
		}
	}
}

DialogResponse:BusinessInfo(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	Dialog_Show(playerid, Dialog:BusinessMenu);
	return 1;
}

DialogCreate:BusinessMenu(playerid)
{
	new
		id = GetPlayerToBusinessID(playerid),
		playername[MAX_PLAYER_NAME+1];

	GetPlayerName(playerid, playername, sizeof(playername));

	new head[MAX_STRING];
	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_HEAD", head, _, Businesses[id][Business_Name]);

	if (!strcmp(Businesses[id][Business_Owner], playername, true)) {
		if (Businesses[id][Business_Upgrade] >= MAX_BUSINESS_LEVEL) {
			Dialog_Open(playerid, Dialog:BusinessMenu, DIALOG_STYLE_LIST,
			            head,
			            "BUSINESS_DIALOG_LIST_SELL",
			            "BUSINESS_DIALOG_BUTTON_OK", "BUSINESS_DIALOG_BUTTON_CANCEL",
			            MDIALOG_NOTVAR_CAPTION,
			            Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade],
			            Businesses[id][Business_Upgrade]);
		} else {
			Dialog_Open(playerid, Dialog:BusinessMenu, DIALOG_STYLE_LIST,
			            head,
			            "BUSINESS_DIALOG_LIST",
			            "BUSINESS_DIALOG_BUTTON_OK", "BUSINESS_DIALOG_BUTTON_CANCEL",
			            MDIALOG_NOTVAR_CAPTION,
			            Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade],
			            Businesses[id][Business_Upgrade],
			            business_GetUpgradeCost(id));
		}
	} else {
		Dialog_Open(playerid, Dialog:BusinessMenu, DIALOG_STYLE_LIST,
		            head,
		            "BUSINESS_DIALOG_LIST_BUY",
		            "BUSINESS_DIALOG_BUTTON_OK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		            MDIALOG_NOTVAR_CAPTION);
	}
}

DialogResponse:BusinessMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	switch (listitem) {
		// продать/купить
		case 0: {
			if (!strcmp(Businesses[ GetPlayerToBusinessID(playerid) ][Business_Owner], ret_GetPlayerName(playerid), true)) {
				Dialog_Show(playerid, Dialog:BusinessSellAccept);
			} else {
				bis_Buy(playerid);
			}
			return 1;
		}
		// забрать прибыль
		case 1: {
			bis_Collect(playerid);
			return 1;
		}
		// увеличить уровень
		case 2: {
			bis_buyUpgrade(playerid);
			return 1;
		}
	}
	return 1;
}

DialogCreate:BusinessSellAccept(playerid)
{
	Dialog_Open(playerid, Dialog:BusinessSellAccept, DIALOG_STYLE_MSGBOX,
	            "BUSINESS_ACCEPT_HEADER",
	            "BUSINESS_ACCEPT_INFO",
	            "BUSINESS_DIALOG_BUTTON_SELL", "BUSINESS_DIALOG_BUTTON_BACK");
}

DialogResponse:BusinessSellAccept(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:BusinessMenu);
		return 1;
	}

	bis_Sell(playerid);
	return 1;
}

DialogResponse:BusinessMessage(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	Dialog_Show(playerid, Dialog:BusinessMenu);
	return 1;
}

DialogCreate:BusinessPlayerOwned(playerid)
{
	static
		string[MAX_STRING * MAX_PLAYER_BUSINESS],
		count = 0,
		playername[MAX_PLAYER_NAME+1];

	count = 0;

	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_LIST_ITEM_HEAD", string);

	GetPlayerName(playerid, playername, sizeof(playername));

	for (new id = 0; id < sizeof(Businesses); id++) {
		if (!strcmp(Businesses[id][Business_Owner], playername, true)) {
			count++;
			Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_LIST_ITEM", string, _,
			                   string,
			                   Businesses[id][Business_Name],
			                   Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade]);
		}
	}

	if (count < 1) {
		Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
		            "BUSINESS_DIALOG_CAPTION",
		            "BUSINESS_DIALOG_NO_BUSINESS",
		            "BUSINESS_DIALOG_BACK", "BUSINESS_DIALOG_CANCEL");
		return 1;
	}

	Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_TABLIST_HEADERS,
	            "BUSINESS_DIALOG_CAPTION",
	            string,
	            "BUSINESS_DIALOG_BACK", "BUSINESS_DIALOG_CANCEL",
	            MDIALOG_NOTVAR_INFO);
	return 1;
}

business_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (!PRESSED(KEY_USING)) {
		return 0;
	}

	if (!IsPlayerAtBusiness(playerid)) {
		return 0;
	}

	new id = GetBusinessID(playerid);
	if (id <= -1)
	{
		return Lang_SendText(playerid, "BUSINESS_ERROR");
	}

	SetPlayerToBusinessID(playerid, id);

	new head[MAX_LANG_VALUE_STRING];
	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_HEAD", head, _, Businesses[id][Business_Name]);

	if (!strcmp(Businesses[id][Business_Owner], ret_GetPlayerName(playerid), true))
	{
		// если мой
		Dialog_Open(playerid, Dialog:BusinessInfo, DIALOG_STYLE_MSGBOX,
		            head,
		            "BUSINESS_DIALOG_INFO_SELF",
		            "BUSINESS_DIALOG_BUTTON_ACTION", "BUSINESS_DIALOG_BUTTON_BACK",
		            MDIALOG_NOTVAR_CAPTION,
		            ((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100,
		            Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade],
		            Businesses[id][Business_Upgrade]);
	}
	else
	{
		if (!strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			// если ничей
			Dialog_Open(playerid, Dialog:BusinessInfo, DIALOG_STYLE_MSGBOX,
			            head,
			            "BUSINESS_DIALOG_INFO_FOREIGN",
			            "BUSINESS_DIALOG_BUTTON_ACTION", "BUSINESS_DIALOG_BUTTON_BACK",
			            MDIALOG_NOTVAR_CAPTION,
			            Businesses[id][Business_Cost],
			            Businesses[id][Business_Level],
			            Businesses[id][Business_Value],
			            Businesses[id][Business_Upgrade]);
		}
		else
		{
			// если кого-то
			Dialog_Open(playerid, Dialog:BusinessInfo, DIALOG_STYLE_MSGBOX,
			            head,
			            "BUSINESS_DIALOG_INFO_PLAYER",
			            "BUSINESS_DIALOG_BUTTON_ACTION", "BUSINESS_DIALOG_BUTTON_BACK",
			            MDIALOG_NOTVAR_CAPTION,
			            Businesses[id][Business_Owner],
			            Businesses[id][Business_Cost] + Businesses[id][Business_Buyout],
			            Businesses[id][Business_Level],
			            Businesses[id][Business_Value],
			            Businesses[id][Business_Upgrade]);
		}
	}
	return 1;
}

stock bis_Buy(playerid)
{
	new playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, playername, sizeof(playername));
	for (new i = 0, pl_business = 0; i < sizeof(Businesses); i++)
	{
		if (!strcmp(Businesses[i][Business_Owner], playername, true))
		{
			pl_business++;
			if (pl_business >= MAX_PLAYER_BUSINESS)
			{
				return Lang_SendText(playerid, "BUSINESS_MAX_COUNT");
			}
		}
	}
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return Lang_SendText(playerid, "BUSINESS_ERROR");
	}
	new head[MAX_STRING];
	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_HEAD", head, _, Businesses[id][Business_Name]);
	if (GetPlayerLevel(playerid) < Businesses[id][Business_Level])
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_LEVEL_ERROR",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION,
		                 Businesses[id][Business_Level]);
		return 1;
	}
	new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
	if (GetPlayerMoney(playerid) < price)
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_NO_MONEY",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION);
		return 1;
	}

	if (!strcmp(Businesses[id][Business_Owner], playername, true))
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_YOU_BUSINESS",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION);
	}
#if !defined BUY_ALL_BUSINESS
	else if (strcmp(Businesses[id][Business_Owner], "Unknown", true))
	{
		Lang_SendText(playerid, "BUSINESS_FOREIGN_BUSINESS");
	}
#endif
	else
	{
	#if defined BUY_ALL_BUSINESS
		foreach (Player, ownerid)
		{
			if (!strcmp(Businesses[id][Business_Owner], ret_GetPlayerName(ownerid), true))
			{
				Lang_SendText(ownerid, "BUSINESS_OUTBIDDING", playername, Businesses[id][Business_Name]);
				GivePlayerMoney(ownerid, price);
				break;
			}
		}
	#endif
		GivePlayerMoney(playerid, -price);

		strcpy(Businesses[id][Business_Owner], playername, MAX_PLAYER_NAME);
		Businesses[id][Business_Buyout] = 0;

		business_UpdateLabel(id);

		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_DIALOG_INFO_BUY",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION,
		                 Businesses[id][Business_Name],
		                 Businesses[id][Business_Vault]);

		Log(mainlog, INFO, "player: %s(%d): bought the '%s' (business)", playername, playerid, Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Sell(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return Lang_SendText(playerid, "BUSINESS_ERROR");
	}
	new head[MAX_LANG_VALUE_STRING];
	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_HEAD", head, _, Businesses[id][Business_Name]);
	if (strcmp(Businesses[id][Business_Owner], ret_GetPlayerName(playerid), true))
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_NOT_YOU",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION);
	}
	else
	{
		new price = ((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100;
		GivePlayerMoney(playerid, price);

		strcpy(Businesses[id][Business_Owner], "Unknown", MAX_PLAYER_NAME);

		Businesses[id][Business_Buyout] = 0;
		Businesses[id][Business_Upgrade] = 1;

		business_UpdateLabel(id);

		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_DIALOG_INFO_SELL",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION,
		                 Businesses[id][Business_Name],
		                 Businesses[id][Business_Vault]);

		Log(mainlog, INFO, "player: %s(%d): sold the '%s' (business)", ret_GetPlayerName(playerid), playerid, Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Collect(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return Lang_SendText(playerid, "BUSINESS_ERROR");
	}

	new head[MAX_LANG_VALUE_STRING];
	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_HEAD", head, _, Businesses[id][Business_Name]);

	if (strcmp(Businesses[id][Business_Owner], ret_GetPlayerName(playerid), true))
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_NOT_YOU",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION);
	}
	else
	{
		if (Businesses[id][Business_Vault] > 0)
		{
			GivePlayerMoney(playerid, Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade]);
			Businesses[id][Business_Vault] = 0;

			Dialog_MessageEx(playerid, Dialog:BusinessMessage,
			                 head,
			                 "BUSINESS_TOOK_VAULT",
			                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
			                 MDIALOG_NOTVAR_CAPTION);
		}
		else
		{
			Dialog_MessageEx(playerid, Dialog:BusinessMessage,
			                 head,
			                 "BUSINESS_NO_VAULT",
			                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
			                 MDIALOG_NOTVAR_CAPTION);
		}
	}
	return 1;
}

stock bis_buyUpgrade(playerid)
{
	new
		id = GetPlayerToBusinessID(playerid),
		head[MAX_LANG_VALUE_STRING];

	Lang_GetPlayerText(playerid, "BUSINESS_DIALOG_HEAD", head, _, Businesses[id][Business_Name]);

	if (Businesses[id][Business_Upgrade] >= MAX_BUSINESS_LEVEL) {
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_UPGRADE_MAX",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION,
		                 Businesses[id][Business_Name]);
		return 1;
	}
	new price = business_GetUpgradeCost(id);

	if (GetPlayerMoney(playerid) < price) {
		Dialog_MessageEx(playerid, Dialog:BusinessMessage,
		                 head,
		                 "BUSINESS_UPGRADE_NO_MONEY",
		                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_CAPTION,
		                 price);
		return 1;
	}

	GivePlayerMoney(playerid, -price);
	Businesses[id][Business_Upgrade]++;
	business_UpdateLabel(id);

	Dialog_MessageEx(playerid, Dialog:BusinessMessage,
	                 head,
	                 "BUSINESS_UPGRADE_UP",
	                 "BUSINESS_DIALOG_BUTTON_BACK", "BUSINESS_DIALOG_BUTTON_CANCEL",
	                 MDIALOG_NOTVAR_CAPTION,
	                 Businesses[id][Business_Name], Businesses[id][Business_Upgrade]);
	return 1;
}

stock business_GetUpgradeCost(id)
{
	return (Businesses[id][Business_Upgrade] + 1) * Businesses[id][Business_Value] * UPGRADE_TARIF;
}

static stock business_DestroyPlayerLabel(playerid, id)
{
	DestroyDynamic3DTextLabel(gLabelID[id][playerid]);
	gLabelID[id][playerid] = Text3D:INVALID_3DTEXT_ID;
}

static stock business_CreatePlayerLabel(playerid, id)
{
	new string[MAX_LANG_VALUE_STRING * 2];
	business_GetLabelString(id, Lang_GetPlayerLang(playerid), string);
	gLabelID[id][playerid] = CreateDynamic3DTextLabel(string, COLOR_WHITE,
		Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]+0.75,
		BusinessDistanceOfShowLabel, .testlos = 1, .playerid = playerid);
}

static stock business_GetLabelString(id, Lang:lang, string[], const size = sizeof(string))
{
	Lang_GetText(lang, "BUSINESS_TEXT_LABEL", string, size,
	             Businesses[id][Business_Name],
	             Businesses[id][Business_Cost],
	             Businesses[id][Business_Value],
	             Businesses[id][Business_Level]);

	if (strcmp(Businesses[id][Business_Owner], "Unknown", false)) {
		Lang_GetText(lang, "BUSINESS_TEXT_LABEL_INFO", string, size,
		             string,
		             Businesses[id][Business_Upgrade],
		             Businesses[id][Business_Owner]);
	}
}

stock business_UpdatePlayerLabel(playerid, id)
{
	new string[MAX_LANG_VALUE_STRING * 2];
	business_GetLabelString(id, Lang_GetPlayerLang(playerid), string);
	UpdateDynamic3DTextLabelText(gLabelID[id][playerid], COLOR_WHITE, string);
}

stock business_UpdateLabel(id)
{
	foreach (new playerid : Player) {
		business_UpdatePlayerLabel(playerid, id);
	}
}

stock business_RenameOwner(old_name[MAX_PLAYER_NAME+1], new_name[MAX_PLAYER_NAME+1])
{
	for (new i = 0; i < sizeof(Businesses); i++)
	{
		if (!strcmp(Businesses[i][Business_Owner], old_name, true))
		{
			strcpy(Businesses[i][Business_Owner], new_name, MAX_PLAYER_NAME);
			business_UpdateLabel(i);
			return 1;
		}
	}
	return 0;
}

stock CheckBusinessOwners()
{
	new
		result[e_Account_Info];

	for (new i = 0; i < sizeof(Businesses); i++) {
		if (strcmp(Businesses[i][Business_Owner], "Unknown", false) == 0) {
			continue;
		}

		Account_LoadData(Businesses[i][Business_Owner], result);

		if (IsDateExpired(result[e_aPremiumTime]) && gettime() > result[e_aLoginTime] + BUSINESS_UNLOGIN_SELL_DAYS * 24 * 60 * 60) {
			Log(mainlog, INFO, "Business has been free. Owner '%s'. BUSINESS_UNLOGIN_SELL_DAYS = %d", Businesses[i][Business_Owner], BUSINESS_UNLOGIN_SELL_DAYS);

			strcpy(Businesses[i][Business_Owner], "Unknown");
			Businesses[i][Business_Buyout] = 0;
			Businesses[i][Business_Upgrade] = 1;
			business_UpdateLabel(i);
		}
	}
}
