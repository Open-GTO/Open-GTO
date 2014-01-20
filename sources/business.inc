/*
	Created by Peter Steenbergen
	Modified by ZiGGi
*/

#if defined _business_included
	#endinput
#endif

#define _business_included
#pragma library business


#define SetPlayerToBusinessID(%0,%1) SetPVarInt(%0, "BusID",%1)
#define GetPlayerToBusinessID(%0) GetPVarInt(%0, "BusID")
new business_icon[ sizeof(Businesses) ], Text3D:Business3DTextLabel[ sizeof(Businesses) ], Float:BusinessDistanceOfShowLabel=20.0;
#define show_Business_MsgDialog(%0,%1,%2) ShowPlayerDialog(%0, bis_Msg_DialogID, DIALOG_STYLE_MSGBOX,%1,%2, lang_texts[3][10], lang_texts[3][8])

stock business_LoadConfig(file_config)
{
	ini_getString(file_config, "Business_DB", BusinessDB);
	ini_getFloat(file_config, "Business_DistanceOfShowLabel", BusinessDistanceOfShowLabel);
}

stock business_SaveConfig(file_config)
{
	ini_setString(file_config, "Business_DB", BusinessDB);
	ini_setFloat(file_config, "Business_DistanceOfShowLabel", BusinessDistanceOfShowLabel);
}

stock business_LoadAll()
{
	new file_business, businessdbname[MAX_STRING];
	for (new i = 0;i < sizeof(Businesses); i++)
	{
		format(businessdbname, sizeof(businessdbname), "%s%s"GTO_FILES_FORMAT, BusinessDB, Businesses[i][Business_Name]);
		if (!ini_fileExist(businessdbname))
		{
			continue;
		}
		file_business = ini_openFile(businessdbname);
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
	new file_business, businessdbname[MAX_STRING];
	for (new i = 0; i < sizeof(Businesses); i++)
	{
		format(businessdbname, sizeof(businessdbname), "%s%s"GTO_FILES_FORMAT, BusinessDB, Businesses[i][Business_Name]);
		file_business = (!ini_fileExist(businessdbname)) ? ini_createFile(businessdbname) : ini_openFile(businessdbname);
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
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		CreatePickup(1274, 49, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]+0.5);	//синий домик
		if (Businesses[id][Business_ShowIcon] == 1)
		{
			business_icon[id] = CreateStreamMapIcon(52, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]);
		}
		format(string, sizeof(string), lang_texts[3][1], Businesses[id][Business_Name], Businesses[id][Business_Cost], Businesses[id][Business_Value], Businesses[id][Business_Owner], Businesses[id][Business_Level], Businesses[id][Business_Upgrade]);
		Business3DTextLabel[id] = Create3DTextLabel(string, COLOUR_WHITE, Businesses[id][Coord_X], Businesses[id][Coord_Y], Businesses[id][Coord_Z]+0.75, BusinessDistanceOfShowLabel, 0, 1);
	}
	GameMSG(lang_texts[3][0]);
}

stock show_Business_Menu(playerid)
{
	new id = GetPlayerToBusinessID(playerid),
		string[MAX_STRING],
		playername[MAX_PLAYER_NAME+1];
	
	GetPlayerName(playerid, playername, sizeof(playername));
	
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	
	if (!strcmp(Businesses[id][Business_Owner], playername, true))
	{
		format(string, sizeof(string), lang_texts[3][6], Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade], Businesses[id][Business_Upgrade]);
	}
	else
	{
		strcat(string, lang_texts[3][5], sizeof(string));
	}
	
	return ShowPlayerDialog(playerid, bis_DialogID, DIALOG_STYLE_LIST,
		head,
		string,
		lang_texts[3][7], lang_texts[3][8]
	);
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
					SendClientMessage(playerid, COLOUR_RED, string);
					Businesses[id][Business_Vault] = 0;
				}
			}
		}
	}
}

stock show_BisSellAccept_Dialog(playerid)
{
	return ShowPlayerDialog(playerid, bis_sell_accept_DialogID, DIALOG_STYLE_MSGBOX,
		"ѕродать бизнес?",
		"¬ы действительно хотите продать этот бизнес?",
		"ѕродать", "Ќазад"
	);
}

stock business_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch (dialogid)
	{
		case bis_DialogID:
		{
			if (!response) {
				return 1;
			}
			switch (listitem)
			{
				case 0: // продать/купить
				{
					if (!strcmp(Businesses[ GetPlayerToBusinessID(playerid) ][Business_Owner], oGetPlayerName(playerid), true))
					{
						show_BisSellAccept_Dialog(playerid);
					}
					else
					{
						bis_Buy(playerid);
					}
					return 1;
				}
				case 1: // забрать прибыль
				{
					return bis_Collect(playerid);
				}
				case 2: // увеличить уровень
				{
					bis_buyUpgrade(playerid);
					return show_Business_Menu(playerid);
				}
			}
		}
		case bis_Msg_DialogID: {
			if (!response) {
				return 1;
			}
			return show_Business_Menu(playerid);
		}
		case bis_sell_accept_DialogID: {
			if (!response) {
				return show_Business_Menu(playerid);
			}
			return bis_Sell(playerid);
		}
	}
    return 1;
}

stock business_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	new id = GetBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOUR_RED, lang_texts[3][2]);
	}

	SetPlayerToBusinessID(playerid, id);

	new head[MAX_STRING], string[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	
	if (!strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		// если мой
		format(string, sizeof(string), lang_texts[3][11],
			((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100,
			Businesses[id][Business_Vault],
			Businesses[id][Business_Upgrade]
		);
			
		ShowPlayerDialog(playerid, bis_Msg_DialogID, DIALOG_STYLE_MSGBOX,
			head,
			string,
			lang_texts[3][9], lang_texts[3][8]
		);
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

			ShowPlayerDialog(playerid, bis_Msg_DialogID, DIALOG_STYLE_MSGBOX,
				head,
				string,
				lang_texts[3][9], lang_texts[3][8]
			);
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
			
			ShowPlayerDialog(playerid, bis_Msg_DialogID, DIALOG_STYLE_MSGBOX,
				head,
				string,
				lang_texts[3][9], lang_texts[3][8]
			);
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
				return SendClientMessage(playerid, COLOUR_RED, lang_texts[3][27]);
			}
		}
	}
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOUR_RED, lang_texts[3][2]);
	}
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	if (GetPlayerLevel(playerid) < Businesses[id][Business_Level])
	{
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[3][18], Businesses[id][Business_Level]);
		return show_Business_MsgDialog(playerid, head, string);
	}
	new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
	if (oGetPlayerMoney(playerid) < price)
	{
		return show_Business_MsgDialog(playerid, head, lang_texts[3][17]);
	}
	
	if (!strcmp(Businesses[id][Business_Owner], playername, true))
	{
		show_Business_MsgDialog(playerid, head, lang_texts[3][14]);
	}
#if !defined BUY_ALL_BUSINESS
	else if (strcmp(Businesses[id][Business_Owner], "Unknown", true))
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[3][12]);
	}
#endif
	else
	{
	#if defined BUY_ALL_BUSINESS
		foreach (Player, ownerid)
		{
			if (!strcmp(Businesses[id][Business_Owner], oGetPlayerName(ownerid), true))
			{
				new temp[MAX_STRING];
				format(temp, sizeof(temp), lang_texts[3][15], playername, Businesses[id][Business_Name]);
				SendClientMessage(ownerid, COLOUR_RED, temp);
				oGivePlayerMoney(ownerid, price, 1);
				break;
			}
		}
	#endif
		oGivePlayerMoney(playerid,-price, 1);
		set(Businesses[id][Business_Owner], playername);
		Businesses[id][Business_Buyout] = 0;
		UpdateStreamMapIcon(business_icon[id]);
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[3][16],
			Businesses[id][Business_Name],
			Businesses[id][Business_Vault]
		);
		show_Business_MsgDialog(playerid, head, string);
		
		GameMSG("player: %s(%d): bought the '%s' (business)", playername, playerid, Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Sell(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOUR_RED, lang_texts[3][2]);
	}
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	if (strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		show_Business_MsgDialog(playerid, head, lang_texts[3][19]);
	}
	else
	{
		new price = ((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100;
		oGivePlayerMoney(playerid, price, 1);
		set(Businesses[id][Business_Owner], "Unknown");
		Businesses[id][Business_Buyout] = 0;
		Businesses[id][Business_Upgrade] = 1;
		UpdateStreamMapIcon(business_icon[id]);
		
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[3][20],
			Businesses[id][Business_Name],
			Businesses[id][Business_Vault]
		);
		show_Business_MsgDialog(playerid, head, string);

		GameMSG("player: %s(%d): sold the '%s' (business)", oGetPlayerName(playerid), playerid, Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Collect(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (id <= -1)
	{
		return SendClientMessage(playerid, COLOUR_RED, lang_texts[3][2]);
	}
	new head[MAX_STRING];
	format(head, sizeof(head), lang_texts[3][3], Businesses[id][Business_Name]);
	if (strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		show_Business_MsgDialog(playerid, head, lang_texts[3][19]);
	}
	else
	{
		if (Businesses[id][Business_Vault] > 0)
		{
			oGivePlayerMoney(playerid, Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade], 1);
			show_Business_MsgDialog(playerid, head, lang_texts[3][21]);
			Businesses[id][Business_Vault] = 0;
		}
		else
		{
			show_Business_MsgDialog(playerid, head, lang_texts[3][22]);
		}
	}
	return 1;
}

stock bis_buyUpgrade(playerid)
{
	new id = GetPlayerToBusinessID(playerid);
	if (strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		return SendClientMessage(playerid, COLOUR_RED, lang_texts[3][47]);
	}
	else
	{
		new string[MAX_STRING];
		if (Businesses[id][Business_Upgrade] >= MAX_BUSINESS_LEVEL)
		{
			format(string, sizeof(string), lang_texts[3][25], Businesses[id][Business_Name]);
			SendClientMessage(playerid, COLOUR_RED, string);
			return 1;
		}
		new price = (Businesses[id][Business_Upgrade]+1) * Businesses[id][Business_Value] * UPGRADE_TARIF;
		
		if (oGetPlayerMoney(playerid) < price)
		{
			format(string, sizeof(string), lang_texts[3][26], price);
			return SendClientMessage(playerid, COLOUR_RED, string);
		}
		
		oGivePlayerMoney(playerid,-price, 0);
		Businesses[id][Business_Upgrade]++;
		format(string, sizeof(string), lang_texts[3][24], Businesses[id][Business_Name], Businesses[id][Business_Upgrade]);
		SendClientMessage(playerid, COLOUR_GREEN, string);
	}
	return 1;
}

stock business_Update3DTextLabelText()
{
	new string[MAX_STRING];
	for (new id = 0; id < sizeof(Businesses); id++)
	{
		format(string, sizeof(string), lang_texts[3][1], Businesses[id][Business_Name], Businesses[id][Business_Cost], Businesses[id][Business_Value], Businesses[id][Business_Owner], Businesses[id][Business_Level], Businesses[id][Business_Upgrade]);
		Update3DTextLabelText(Business3DTextLabel[id], COLOUR_WHITE, string);
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
	new filename_account[MAX_STRING], file_account, account_lastlogin[32], idx,
		year, month, day, account_year, account_month, account_day;
	
	getdate(year, month, day);
	for (new i = 0; i < sizeof(Businesses); i++)
	{
		if (!strcmp(Businesses[i][Business_Owner], "Unknown", false))
		{
			continue;
		}
		
		format(filename_account, sizeof(filename_account), "%s%s"GTO_FILES_FORMAT, AccountDB, Businesses[i][Business_Owner]);
		if (!ini_fileExist(filename_account))
		{
			continue;
		}
		
		file_account = ini_openFile(filename_account);
		ini_getString(file_account, "Last_Login", account_lastlogin);
		ini_closeFile(file_account);
		
		idx = 0;
		account_day = strval( strcharsplit(account_lastlogin, idx, '.') );
		account_month = strval( strcharsplit(account_lastlogin, idx, '.') );
		account_year = strval( strcharsplit(account_lastlogin, idx, ',') );
		
		if (DateToNum(year, month, day) > DateToNum(account_year, account_month, account_day) + BUSINESS_UNLOGIN_SELL_DAYS * 24 * 60 * 60)
		{
			GameMSG("Business has been free. Owner '%s'. BUSINESS_UNLOGIN_SELL_DAYS = %d",
				Businesses[i][Business_Owner], BUSINESS_UNLOGIN_SELL_DAYS
			);
			GameMSG("\tcurrent date[%02d.%02d.%d]: %d, last login[%02d.%02d.%d]: %d",
				day, month, year, DateToNum(year, month, day),
				account_day, account_month, account_year, DateToNum(account_year, account_month, account_day)
			);
			
			set(Businesses[i][Business_Owner], "Unknown");
			Businesses[i][Business_Buyout] = 0;
			Businesses[i][Business_Upgrade] = 1;
			UpdateStreamMapIcon(business_icon[i]);
		}
	}
}
