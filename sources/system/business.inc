/*
	Created by Peter Steenbergen
	Modified by ZiGGi
*/

#if defined _business_included
	#endinput
#endif

#define _business_included
#pragma library business

enum BusinessInfo {
	Business_Name[MAX_NAME],
	Business_Cost,
	Business_Value,
	Business_Level, // minumum level
	Business_Owner[MAX_NAME],
	Business_Buyout, // Buyout price
	Business_Vault,
	Business_Upgrade,
	Business_ShowIcon,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z
}
new Businesses[][BusinessInfo] = {
	//NAME, COST, VALUE, LEVEL, OWNER, BUYOUT, upgrade, x, y, z, icon
	{"Botique", 35000, 800, 5, "Unknown", 0, 0, 1, 0, 418.5547, -80.1667, 1001.8047},
	{"Shithole Bar", 25000, 1300, 8, "Unknown", 0, 0, 1, 0, 501.4927, -75.4323, 998.7578},
	{"Tattoo Parlor", 40000, 600, 3, "Unknown", 0, 0, 1, 0, -203.4864, -41.2045, 1002.2734},
	{"Binco Clothing", 15000, 800, 5, "Unknown", 0, 0, 1, 0, 207.5640, -97.8188, 1005.2578},
	{"Train Hard Clothing", 15000, 800, 5, "Unknown", 0, 0, 1, 0, 210.1111, -129.1273, 1003.5152},
	{"Urban Clothing", 15000, 800, 5, "Unknown", 0, 0, 1, 0, 203.8557, -40.4717, 1001.8047},
	{"Victim Clothing", 15000, 800, 5, "Unknown", 0, 0, 1, 0, 210.3724, -8.1825, 1005.2109},
	{"Zip Clothing", 15000, 800, 5, "Unknown", 0, 0, 1, 0,  161.1875, -79.9915, 1001.8047},
	{"DidierSachs Clothing", 20000, 950, 7, "Unknown", 0, 0, 1, 0, 204.2810, -157.2614, 1000.5234},
	{"Gym", 20000, 1000, 7, "Unknown", 0, 0, 1, 0, 773.6138, -72.1616, 1000.6484},
	{"Zero RC", 10000, 900, 3, "Unknown", 0, 0, 1, 0, -2231.2478, 131.9623, 1035.4141},
	{"Zombotech", 35000, 1600, 12, "Unknown", 0, 0, 1, 1, -1951.5938, 704.3567, 46.5625},
	{"Verdant Meadows Air Strip", 20000, 1200, 7, "Unknown", 0, 0, 1, 0, 418.1210, 2536.8762, 10.0000},
	{"Blueberry Liquor Store", 15000, 850, 5, "Unknown", 0, 0, 1, 0, 252.4851, -57.3515, 1.5703},
	{"Club-Disco", 50000, 1600, 10, "Unknown", 0, 0, 1, 0, 500.7049, -13.3777, 1000.6797},
	{"Lil Probe-Inn", 47000, 1100, 6, "Unknown", 0, 0, 1, 0, -225.7790, 1403.9459, 27.7734},
	{"Sex Shop", 40000, 950, 7, "Unknown", 0, 0, 1, 0, -103.5525, -22.4661, 1000.7188},
	{"Strip Club", 60000, 1400, 10, "Unknown", 0, 0, 1, 0, 1212.3918, -10.9459, 1000.9219},
	{"The Big Spread Ranch", 48000, 1450, 9, "Unknown", 0, 0, 1, 0, 1208.5027, -32.6044, 1000.9531},
	{"Jizzys", 58000, 1675, 13, "Unknown", 0, 0, 1, 0, -2650.0667, 1409.2106, 906.2734},
	{"Cobra Marital Arts", 15000, 800, 5, "Unknown", 0, 0, 1, 0, 768.2014, -36.9461, 1000.6865},
	{"City Planning Department", 35000, 1600, 12, "Unknown", 0, 0, 1, 0, 357.0584, 173.5787, 1008.3820},
	{"Inside Track", 20000, 1200, 7, "Unknown", 0, 0, 1, 1, 823.1819, 3.2361, 1004.1797},
	{"Coutt and Schutz Auto", 25000, 1250, 8, "Unknown", 0, 0, 1, 0, 2131.6455, -1148.2836, 24.3898},
	{"Ottos Auto", 25000, 1250, 8, "Unknown", 0, 0, 1, 1, -1657.8334, 1207.3041, 7.2500},
	{"Wang Cars", 25000, 1250, 8, "Unknown", 0, 0, 1, 1, -1957.5327, 300.2131, 35.4688},
	{"Emerald Isle", 160000, 4100, 17, "Unknown", 0, 0, 1, 1, 2127.5940, 2370.4255, 10.8203},
	{"The Visage", 145000, 3800, 17, "Unknown", 0, 0, 1, 1, 2022.5179, 1916.6848, 12.3397},
	{"Caligulas", 3750000, 19900, 20, "Unknown", 0, 0, 1, 0, 2235.5408, 1679.0402, 1008.3594},
	{"Four Dragons", 2050000, 16000, 20, "Unknown", 0, 0, 1, 0,	1993.9758, 1017.8945, 994.8906},
	{"Casino", 50000, 8000, 20, "Unknown", 0, 0, 1, 0, 1130.7843, -2.0449, 1000.6797},
	{"Restroom", 100000, 3300, 20, "Unknown", 0, 0, 1, 0, -789.4709, 509.9566, 1367.3745},
	{"Reeces", 100000, 3300, 20, "Unknown", 0, 0, 1, 0, 414.4313, -10.6664, 1001.8120},
	{"Tattos", 100000, 3300, 20, "Unknown", 0, 0, 1, 0, -201.9593, -25.0057, 1002.2734},
	{"Ferris Wheel", 100000, 3300, 26, "Unknown", 0, 0, 1, 1, 385.4720, -2028.4464, 7.8359},
	{"Beacon", 100000, 3300, 27, "Unknown", 0, 0, 1, 1, 154.2354, -1945.4606, 4.6698},
	{"Submarine", 100000, 3300, 28, "Unknown", 0, 0, 1, 1, -1878.2374, 1456.0652, 8.3669},
	{"San Fierro Building", 100000, 3300, 29, "Unknown", 0, 0, 1, 1, -2072.1067, 225.7475, 36.0393},
	{"San Fierro City Hall", 100000, 3300, 30, "Unknown", 0, 0, 1, 1, -2764.7234, 375.7569, 6.3419},
	{"Avispa Country Club", 100000, 3300, 30, "Unknown", 0, 0, 1, 1, -2720.6208, -318.2194, 7.8438},
	{"Easter Bay Chemicals", 100000, 3300, 20, "Unknown", 0, 0, 1, 1, -1054.9453, -694.9460, 32.3516},
	{"Las Venturas Bandits", 100000, 3300, 30, "Unknown", 0, 0, 1, 1, 1477.8231, 2248.4409, 11.0234}
};

#define SetPlayerToBusinessID(%0,%1) SetPVarInt(%0, "BusID",%1)
#define GetPlayerToBusinessID(%0) GetPVarInt(%0, "BusID")
new business_icon[ sizeof(Businesses) ], Text3D:Business3DTextLabel[ sizeof(Businesses) ], Float:BusinessDistanceOfShowLabel=20.0;

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
		ini_getString(file_business, "Owner", Businesses[i][Business_Owner], MAX_NAME);
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

stock business_OnGameModeInit()
{
	business_LoadAll();
	new
		string[MAX_STRING * 2],
		icon_type;
	
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		CreatePickup(1274, 49, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z], -1);
		
		if (Businesses[id][Business_ShowIcon] == 1)
		{
			if (strcmp(Businesses[id][Business_Owner], "Unknown")) {
				icon_type = 36;
			} else {
				icon_type = 52;
			}
			business_icon[id] = Mapicon_Create(Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z], icon_type, 0);
		}

		business_Create3DTextLabelStr(id, string);

		Business3DTextLabel[id] = Create3DTextLabel(string, COLOR_WHITE, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]+0.75, BusinessDistanceOfShowLabel, 0, 1);
	}
	Log_Game(lang_texts[3][0]);
}

stock business_MapiconPlayerSync(playerid)
{
	new playername[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, playername, sizeof(playername));

	for (new id = 0; id < sizeof(Businesses); id++) {
		if (!strcmp(Businesses[id][Business_Owner], playername)) {
			Mapicon_UpdateTypeForPlayer(playerid, business_icon[id], 38);
		}
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
				new playerid = GetPlayerID(Businesses[id][Business_Owner]);
				if (playerid != INVALID_PLAYER_ID)
				{
					new amount = Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade];
					GivePlayerBankMoney(playerid, amount);
					new string[MAX_STRING];
					format(string, sizeof(string), lang_texts[3][28], Businesses[id][Business_Name], amount);
					SendClientMessage(playerid, COLOR_RED, string);
					Businesses[id][Business_Vault] = 0;
				}
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
	new id = GetPlayerToBusinessID(playerid),
		string[MAX_STRING * 2],
		playername[MAX_PLAYER_NAME+1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	
	if (!strcmp(Businesses[id][Business_Owner], playername, true)) {
		if (Businesses[id][Business_Upgrade] >= MAX_BUSINESS_LEVEL) {
			format(string, sizeof(string), lang_texts[3][30], Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade], Businesses[id][Business_Upgrade]);
		} else {
			format(string, sizeof(string), lang_texts[3][6], Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade], Businesses[id][Business_Upgrade], business_GetUpgradeCost(id));
		}
	} else {
		strcat(string, lang_texts[3][5], sizeof(string));
	}
	
	Dialog_Open(playerid, Dialog:BusinessMenu, DIALOG_STYLE_LIST, head, string, lang_texts[3][7], lang_texts[3][8]);
}

DialogResponse:BusinessMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	switch (listitem) {
		// продать/купить
		case 0: {
			if (!strcmp(Businesses[ GetPlayerToBusinessID(playerid) ][Business_Owner], ReturnPlayerName(playerid), true)) {
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
	Dialog_Open(playerid, Dialog:BusinessSellAccept, DIALOG_STYLE_MSGBOX, "ѕродать бизнес?", "¬ы действительно хотите продать этот бизнес?", "ѕродать", "Ќазад");
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
	new
		string[MAX_STRING * MAX_PLAYER_BUSINESS],
		count = 0,
		playername[MAX_PLAYER_NAME+1];
	
	string = _(BUSINESS_DIALOG_LIST_ITEM_HEAD);

	GetPlayerName(playerid, playername, sizeof(playername));

	for (new id = 0; id < sizeof(Businesses); id++) {
		if (!strcmp(Businesses[id][Business_Owner], playername, true)) {
			count++;
			format(string, sizeof(string),
				_(BUSINESS_DIALOG_LIST_ITEM),
				string,
				Businesses[id][Business_Name],
				Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade]
			);
		}
	}

	if (count < 1) {
		Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_MSGBOX,
			_(BUSINESS_DIALOG_CAPTION),
			_(BUSINESS_DIALOG_NO_BUSINESS),
			_(BUSINESS_DIALOG_BACK), _(BUSINESS_DIALOG_CANCEL)
		);
		return 1;
	}

	Dialog_Open(playerid, Dialog:PlayerReturnMenu, DIALOG_STYLE_TABLIST_HEADERS,
		_(BUSINESS_DIALOG_CAPTION),
		string,
		_(BUSINESS_DIALOG_BACK), _(BUSINESS_DIALOG_CANCEL)
	);
	return 1;
}

stock business_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	new id = GetBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOR_RED, lang_texts[3][2]);
	}

	SetPlayerToBusinessID(playerid, id);

	new head[MAX_STRING], string[MAX_STRING * 2];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	
	if (!strcmp(Businesses[id][Business_Owner], ReturnPlayerName(playerid), true))
	{
		// если мой
		format(string, sizeof(string), lang_texts[3][11],
			((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100,
			Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade],
			Businesses[id][Business_Upgrade]
		);
		
		Dialog_Open(playerid, Dialog:BusinessInfo, DIALOG_STYLE_MSGBOX, head, string, lang_texts[3][9], lang_texts[3][8]);
	}
	else
	{
		if (!strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			// если ничей
			format(string, sizeof(string), lang_texts[3][13],
				Businesses[id][Business_Cost],
				Businesses[id][Business_Level],
				Businesses[id][Business_Value],
				Businesses[id][Business_Upgrade]
			);

			Dialog_Open(playerid, Dialog:BusinessInfo, DIALOG_STYLE_MSGBOX, head, string, lang_texts[3][9], lang_texts[3][8]);
		}
		else
		{
			// если кого-то
			format(string, sizeof(string), lang_texts[3][4],
				Businesses[id][Business_Owner],
				Businesses[id][Business_Cost] + Businesses[id][Business_Buyout],
				Businesses[id][Business_Level],
				Businesses[id][Business_Value],
				Businesses[id][Business_Upgrade]
			);
			
			Dialog_Open(playerid, Dialog:BusinessInfo, DIALOG_STYLE_MSGBOX, head, string, lang_texts[3][9], lang_texts[3][8]);
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
				return SendClientMessage(playerid, COLOR_RED, lang_texts[3][27]);
			}
		}
	}
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOR_RED, lang_texts[3][2]);
	}
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	if (GetPlayerLevel(playerid) < Businesses[id][Business_Level])
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[3][18], Businesses[id][Business_Level]);
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, string, lang_texts[3][10], lang_texts[3][8]);
		return 1;
	}
	new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
	if (GetPlayerMoney(playerid) < price)
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, lang_texts[3][17], lang_texts[3][10], lang_texts[3][8]);
		return 1;
	}
	
	if (!strcmp(Businesses[id][Business_Owner], playername, true))
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, lang_texts[3][14], lang_texts[3][10], lang_texts[3][8]);
	}
#if !defined BUY_ALL_BUSINESS
	else if (strcmp(Businesses[id][Business_Owner], "Unknown", true))
	{
		SendClientMessage(playerid, COLOR_RED, lang_texts[3][12]);
	}
#endif
	else
	{
	#if defined BUY_ALL_BUSINESS
		foreach (Player, ownerid)
		{
			if (!strcmp(Businesses[id][Business_Owner], ReturnPlayerName(ownerid), true))
			{
				new temp[MAX_STRING];
				format(temp, sizeof(temp), lang_texts[3][15], playername, Businesses[id][Business_Name]);
				SendClientMessage(ownerid, COLOR_RED, temp);
				GivePlayerMoney(ownerid, price, 1);
				break;
			}
		}
	#endif
		GivePlayerMoney(playerid, -price, 1);

		set(Businesses[id][Business_Owner], playername);
		Businesses[id][Business_Buyout] = 0;

		Mapicon_Update(business_icon[id], 36);
		Mapicon_UpdateTypeForPlayer(playerid, business_icon[id], 38);

		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[3][16],
			Businesses[id][Business_Name],
			Businesses[id][Business_Vault]
		);
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, string, lang_texts[3][10], lang_texts[3][8]);
		
		Log_Game("player: %s(%d): bought the '%s' (business)", playername, playerid, Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Sell(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOR_RED, lang_texts[3][2]);
	}
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	if (strcmp(Businesses[id][Business_Owner], ReturnPlayerName(playerid), true))
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, lang_texts[3][19], lang_texts[3][10], lang_texts[3][8]);
	}
	else
	{
		new price = ((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100;
		GivePlayerMoney(playerid, price, 1);

		set(Businesses[id][Business_Owner], "Unknown");

		Businesses[id][Business_Buyout] = 0;
		Businesses[id][Business_Upgrade] = 1;

		Mapicon_Update(business_icon[id], 52);
		Mapicon_ResetTypeForPlayer(playerid, business_icon[id]);
		
		new string[MAX_STRING * 2];
		format(string, sizeof(string), lang_texts[3][20],
			Businesses[id][Business_Name],
			Businesses[id][Business_Vault]
		);
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, string, lang_texts[3][10], lang_texts[3][8]);

		Log_Game("player: %s(%d): sold the '%s' (business)", ReturnPlayerName(playerid), playerid, Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Collect(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOR_RED, lang_texts[3][2]);
	}
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	if (strcmp(Businesses[id][Business_Owner], ReturnPlayerName(playerid), true))
	{
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, lang_texts[3][19], lang_texts[3][10], lang_texts[3][8]);
	}
	else
	{
		if (Businesses[id][Business_Vault] > 0)
		{
			GivePlayerMoney(playerid, Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade], 1);
			Businesses[id][Business_Vault] = 0;

			Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, lang_texts[3][21], lang_texts[3][10], lang_texts[3][8]);
		}
		else
		{
			Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, lang_texts[3][22], lang_texts[3][10], lang_texts[3][8]);
		}
	}
	return 1;
}

stock bis_buyUpgrade(playerid)
{
	new
		id = GetPlayerToBusinessID(playerid),
		string[MAX_STRING],
		head[MAX_STRING];

	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);

	if (Businesses[id][Business_Upgrade] >= MAX_BUSINESS_LEVEL) {
		format(string, sizeof(string), lang_texts[3][25], Businesses[id][Business_Name]);
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, string, lang_texts[3][10], lang_texts[3][8]);
		return 1;
	}
	new price = business_GetUpgradeCost(id);
	
	if (GetPlayerMoney(playerid) < price) {
		format(string, sizeof(string), lang_texts[3][26], price);
		Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, string, lang_texts[3][10], lang_texts[3][8]);
		return 1;
	}
	
	GivePlayerMoney(playerid, -price, 0);
	Businesses[id][Business_Upgrade]++;
	format(string, sizeof(string), lang_texts[3][24], Businesses[id][Business_Name], Businesses[id][Business_Upgrade]);
	Dialog_MessageEx(playerid, Dialog:BusinessMessage, head, string, lang_texts[3][10], lang_texts[3][8]);
	return 1;
}

stock business_GetUpgradeCost(id)
{
	return (Businesses[id][Business_Upgrade] + 1) * Businesses[id][Business_Value] * UPGRADE_TARIF;
}

stock business_Update3DTextLabelText()
{
	new string[MAX_STRING * 2];
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		business_Create3DTextLabelStr(id, string);
		Update3DTextLabelText(Business3DTextLabel[id], COLOR_WHITE, string);
	}
	return 1;
}

stock business_Create3DTextLabelStr(id, string[MAX_STRING * 2])
{
	format(string, sizeof(string), lang_texts[3][1], Businesses[id][Business_Name], Businesses[id][Business_Cost], Businesses[id][Business_Value], Businesses[id][Business_Level]);
	if (strcmp(Businesses[id][Business_Owner], "Unknown", false)) {
		format(string, sizeof(string), lang_texts[3][29], string, Businesses[id][Business_Upgrade], Businesses[id][Business_Owner]);
	}
	return 1;
}

stock business_RenameOwner(old_name[MAX_PLAYER_NAME+1], new_name[MAX_PLAYER_NAME+1])
{
	for (new i = 0; i < sizeof(Businesses); i++)
	{
		if (!strcmp(Businesses[i][Business_Owner], old_name, true))
		{
			set(Businesses[i][Business_Owner], new_name);
			return 1;
		}
	}
	return 0;
}

stock CheckBusinessOwners()
{
	new filename_account[MAX_STRING], file_account, timestamp, premium_date[11];
	
	for (new i = 0; i < sizeof(Businesses); i++) {
		if (!strcmp(Businesses[i][Business_Owner], "Unknown", false)) {
			continue;
		}
		
		format(filename_account, sizeof(filename_account), "%s%s"DATA_FILES_FORMAT, db_account, Businesses[i][Business_Owner]);
		if (!ini_fileExist(filename_account)) {
			continue;
		}
		
		file_account = ini_openFile(filename_account);
		ini_getInteger(file_account, "Login_Date", timestamp);
		ini_getString(file_account, "Premium_Date", premium_date);
		ini_closeFile(file_account);
		
		if (Premium_GetDateStatus(premium_date) == 0 && gettime() > timestamp + BUSINESS_UNLOGIN_SELL_DAYS * 24 * 60 * 60) {
			Log_Game("Business has been free. Owner '%s'. BUSINESS_UNLOGIN_SELL_DAYS = %d",
				Businesses[i][Business_Owner], BUSINESS_UNLOGIN_SELL_DAYS
			);
			
			set(Businesses[i][Business_Owner], "Unknown");
			Businesses[i][Business_Buyout] = 0;
			Businesses[i][Business_Upgrade] = 1;
			Mapicon_Update(business_icon[i], 52);
		}
	}
}
