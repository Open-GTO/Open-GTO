// created by asturel based on business system, thx to Peter Steenbergen

#if defined _housing_included
#endinput
#endif

#define _housing_included
#pragma library housing


#include "base"
#include "account"
#include "player"
#include "world"

new Hous[MAX_PLAYERS],Haus[MAX_PLAYERS];
new house_icon[sizeof(Houses)],Text3D:House3DTextLabel[sizeof(Houses)],Float:HouseDistanceOfShowLabel=20.0;

stock HousesLoadAll()
{
	if(ini_Exist(ConfigDB))
	{
		new file_housing_cfg = ini_Open(ConfigDB);
		ini_Get(file_housing_cfg,"Houses_DB",HousesDB);
		ini_GetFloat(file_housing_cfg,"House_DistanceOfShowLabel",HouseDistanceOfShowLabel);
		ini_Close(file_housing_cfg);
	}
	for(new i=0,file_housing;i<sizeof(Houses);i++)
	{
		new housesdbname[MAX_STRING];
		format(housesdbname,sizeof(housesdbname),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[i][Houses_Name]);
		if(!ini_Exist(housesdbname)) continue;
		file_housing = ini_Open(housesdbname);
		ini_Get(file_housing,"Name",Houses[i][Houses_Name],MAX_STRING);
		ini_GetInt(file_housing,"Cost",Houses[i][Houses_Cost]);
		ini_Get(file_housing,"Owner",Houses[i][Houses_Owner],MAX_NAME);
		ini_Get(file_housing,"Gang",Houses[i][Houses_Gang],MAX_NAME);
		ini_GetInt(file_housing,"UpKeep",Houses[i][Houses_UpKeep]);
		ini_GetInt(file_housing,"UpKeepLeft",Houses[i][Houses_UpKeepLeft]);
		ini_GetInt(file_housing,"Buyout",Houses[i][Houses_Buyout]);
		ini_GetInt(file_housing,"Interior",Houses[i][Houses_Interior]);
		ini_GetInt(file_housing,"Car",Houses[i][Houses_Car]);
		ini_GetInt(file_housing,"Lock",Houses[i][Houses_Lock]);
		ini_GetInt(file_housing,"Rentabil",Houses[i][Houses_Rentabil]);
		ini_Get(file_housing,"RentName",Houses[i][Houses_RentName],MAX_NAME);
		ini_GetInt(file_housing,"RentCost",Houses[i][Houses_RentCost]);
		ini_GetInt(file_housing,"HealUpgrade",Houses[i][Houses_HealUpgrade]);
		ini_GetFloat(file_housing,"Coord_X",Houses[i][Houses_EnterX]);
		ini_GetFloat(file_housing,"Coord_Y",Houses[i][Houses_EnterY]);
		ini_GetFloat(file_housing,"Coord_Z",Houses[i][Houses_EnterZ]);
		ini_Close(file_housing);
	}
	return;
}

stock HousesSaveAll()
{
	new file_housing = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_housing,"Houses_DB",HousesDB);
	ini_SetFloat(file_housing,"House_DistanceOfShowLabel",HouseDistanceOfShowLabel);
	ini_Close(file_housing);
	for(new housid=0;housid<sizeof(Houses);housid++) SaveHous(housid);
	return 1;
}

stock SaveHous(housid)
{
	new housesdbname[MAX_STRING];
	format(housesdbname,sizeof(housesdbname),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[housid][Houses_Name]);
	new file_housing = (!ini_Exist(housesdbname)) ? ini_Create(housesdbname) : ini_Open(housesdbname);
	ini_Set(file_housing,"Name",Houses[housid][Houses_Name]);
	ini_SetInt(file_housing,"Cost",Houses[housid][Houses_Cost]);
	ini_Set(file_housing,"Owner",Houses[housid][Houses_Owner]);
	ini_Set(file_housing,"Gang",Houses[housid][Houses_Gang]);
	ini_SetInt(file_housing,"UpKeep",Houses[housid][Houses_UpKeep]);
	ini_SetInt(file_housing,"UpKeepLeft",Houses[housid][Houses_UpKeepLeft]);
	ini_SetInt(file_housing,"Buyout",Houses[housid][Houses_Buyout]);
	ini_SetInt(file_housing,"Interior",Houses[housid][Houses_Interior]);
	ini_SetInt(file_housing,"Car",Houses[housid][Houses_Car]);
	ini_SetInt(file_housing,"Lock",Houses[housid][Houses_Lock]);
	ini_SetInt(file_housing,"Rentabil",Houses[housid][Houses_Rentabil]);
	ini_Set(file_housing,"RentName",Houses[housid][Houses_RentName]);
	ini_SetInt(file_housing,"RentCost",Houses[housid][Houses_RentCost]);
	ini_SetFloat(file_housing,"Coord_X",Houses[housid][Houses_EnterX]);
	ini_SetFloat(file_housing,"Coord_Y",Houses[housid][Houses_EnterY]);
	ini_SetFloat(file_housing,"Coord_Z",Houses[housid][Houses_EnterZ]);
	ini_Close(file_housing);
	return 1;
}
stock housing_OnGameModeInit()
{
	HousesLoadAll();
	new string[MAX_STRING];
	for(new id=0;id<sizeof(Houses);id++)
	{
		CreatePickup(1273,1,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]+0.5);	//зеленый домик
		CreatePickup(1559,1,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ]+0.5);
		house_icon[id] = CreateStreamMapIcon(31,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]);
		format(string,sizeof(string),"Дом: %s\nСтоимость: %d\nКуплен: %s\nБанда: %s\nАрендуется: %s",Houses[id][Houses_Name],Houses[id][Houses_Cost],Houses[id][Houses_Owner],Houses[id][Houses_Gang],Houses[id][Houses_RentName]);
		House3DTextLabel[id] = Create3DTextLabel(string,COLOUR_WHITE,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]+0.75,HouseDistanceOfShowLabel,0,1);
	}
	GameMSG("SERVER: Housing module init");
}

stock IsPlayerAtHouse(playerid)
{
	for(new id=0;id<sizeof(Houses);id++)
	{
		if(
		IsPlayerInRangeOfPoint(playerid,2,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ])
		||
		IsPlayerInRangeOfPoint(playerid,2,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ])
		) return 1;
	}
	return 0;
}

stock GetHouseID(playerid)
{
	new lookupid=-1;
	for(new id=0;id<sizeof(Houses);id++) if(IsPlayerInRangeOfPoint(playerid,1.5,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ])) lookupid = id;
	return lookupid;
}

stock IsPlayerRenter(playerid)
{
	for(new id=0;id<sizeof(Houses);id++) if(strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true) == 0) return true;
	return false;
}

stock housing_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused inputtext
	if(dialogid == house_DialogID)
	{
		if(!response)
		{
			Hous[playerid] = 0;
			return 0;
		}
		switch(Hous[playerid])
		{
			case 1:ShowPlayerDialog(playerid,houses_DialogID,DIALOG_STYLE_LIST,"Действия:","Купить дом\nВойти в дом\nАрендовать комнату\n","Выбрать","Отмена");
			case 2:ShowPlayerDialog(playerid,houses_DialogID,DIALOG_STYLE_LIST,"Действия:","Продать дом\nВойти в дом\nВыгнать арендатора\n","Выбрать","Отмена");
			case 3:ShowPlayerDialog(playerid,houses_DialogID,DIALOG_STYLE_LIST,"Действия:","Купить дом\nВойти в дом\nАрендовать комнату\n","Выбрать","Отмена");
			case 4:ShowPlayerDialog(playerid,houses_DialogID,DIALOG_STYLE_LIST,"Действия:","Купить дом\nВойти в дом\nПрекратить арендовать\nЗаплатить за аренду","Выбрать","Отмена");
		}
		return 1;
	}
	if(dialogid == houses_DialogID)
	{
		if(!response) return 0;
		if(listitem == 0)
		{
			if(Hous[playerid] != 2)	FHouseBuy(playerid);
			else FHouseSell(playerid);
		}
		if(listitem == 1) 
		{
			new id = GetHouseID(playerid);
			if(id > -1)
			{
				if(Houses[id][Houses_Lock] == 1) return SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][56]);
				else 
				{
					oSetPlayerPos(playerid,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ],0,Houses[id][Houses_Interior]);
					Haus[playerid] = id;
					SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][73]);
				}
			}
		}
		if(listitem == 2)
		{
			switch(Hous[playerid])
			{
				case 1,3:RentRoom(playerid);
				case 2,4:DeleteRenter(playerid);
			}
		}
		Hous[playerid] = 0;
		return 1;
	}
	return 1;
}

stock RentRoom(playerid)
{
	new id = GetHouseID(playerid),string[MAX_STRING];
	if(id <= -1) return 1;
	if(IsPlayerRenter(playerid)) return 1;
	if(Houses[id][Houses_Rentabil] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][68]);
	if(strlen(GetPlayerGangName(playerid)) > 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][79]);
	if(!strcmp(Houses[id][Houses_RentName],"Unknown",true))
	{
		if(oGetPlayerMoney(playerid) < 50000) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][57]);
		set(Houses[id][Houses_RentName],oGetPlayerName(playerid));
		oGivePlayerMoney(playerid,-Houses[id][Houses_RentCost],0);
		format(string,sizeof(string),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
		new file_housing = ini_Open(string);
		ini_SetInt(file_housing,"Rentabil",1);
		ini_Set(file_housing,"RentName",oGetPlayerName(playerid));
		ini_Close(file_housing);
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][55]);
		UpdatePlayerStreamMapIcon(playerid,house_icon[id]);
	}
	else
	{
		format(string,sizeof(string),lang_texts[8][54],Houses[id][Houses_RentName]);
		SendClientMessage(playerid,COLOUR_RED,string);
	}
	return 1;
}

stock DeleteRenter(playerid)
{
	new id = GetHouseID(playerid);
	if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true) == 0) 
	{
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[8][58]);
		for(new i=0;i<=GetPlayerLastID();i++)
			if(IsPlayerConnected(i) && !strcmp(Houses[id][Houses_RentName],oGetPlayerName(i),true))
				SendClientMessage(i,COLOUR_LIGHTRED,lang_texts[8][60]);
	}
	else if(!strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true))
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][59]);
	if(id > -1) set(Houses[id][Houses_RentName],"Unknown");
	UpdatePlayerStreamMapIcon(playerid,house_icon[id]);
	return 1;
}

stock housing_OnPlayerCommandText(playerid,text[]) 
{
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,' '));
	if(strlen(cmd) == 0) return 0;

	if((!strcmp(cmd,"/houseinfo",true)) || (!strcmp(cmd,"/hinfo",true)))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][2]);
		FHouseInfo(playerid);
		return 1;
	}
	if((!strcmp(cmd,"/housebuy",true)) || (!strcmp(cmd,"/hbuy",true)))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][14]);
		FHouseBuy(playerid);
		return 1;
	}
	if((!strcmp(cmd,"/housesell",true)) || (!strcmp(cmd,"/hsell",true)))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][22]);
		FHouseSell(playerid);
		return 1;
	}
	if(!strcmp(cmd,"/myhouses",true)) FHouseMyHouses(playerid);
	if(!strcmp(cmd,"/houses",true)) FHouseHouses(playerid);
	if(!strcmp(cmd,"/upkeep",true))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][43]); 
		FHouseKeep(playerid);
		return 1;
	}
	if(!strcmp(cmd,"/heal",true))
	{
		if(Haus[playerid] <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][61]);
		new id = Haus[playerid];
		if(Houses[id][Houses_HealUpgrade] != 0)
		{
			SetPlayerArmour(playerid,100);
			SetPlayerHealth(playerid,1000);
		}
		else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][62]);
		return 1;
	}
	if(!strcmp(cmd,"/houseupgrade",true))
	{
		if(Haus[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][61]);
		new id = Haus[playerid];
		set(cmd,strcharsplit(text, idx,' '));
		if(strlen(cmd) == 0) SendClientMessage(playerid,COLOUR_GREY,lang_texts[8][66]);
		if(!strcmp(cmd,"heal",true))
		{
			if(GetPlayerMoney(playerid) >= 100000)
			{
				if(Houses[id][Houses_HealUpgrade] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][65]);
				Houses[id][Houses_HealUpgrade] = 1;
				oGivePlayerMoney(playerid,-100000,0);
				SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][64]);
			}
			else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][63]);
		}
		return 1;
	}
	if(!strcmp(cmd,"/setrentcost",true))
	{
		new id = GetHouseID(playerid),string[MAX_STRING];
		if((id > -1 || Haus[playerid] > -1) && !strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true))
		{
			id = Haus[playerid];
			new rentcost = strval(strcharsplit(text, idx,' '));
			if(rentcost < 1) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[8][71]);
			Houses[id][Houses_RentCost] = rentcost;
			format(string,sizeof(string),lang_texts[8][72],rentcost);
			SendClientMessage(playerid,COLOUR_LIGHTRED,string);
		}
		return 1;
	}
	if(!strcmp(cmd,"/hlock",true))
	{
		if(Haus[playerid] > -1) if(!strcmp(Houses[Haus[playerid]][Houses_Owner],oGetPlayerName(playerid),true)) Houses[Haus[playerid]][Houses_Lock] = 1;
		return 1;
	}
	if(!strcmp(cmd,"/hunlock",true))
	{
		new id = GetHouseID(playerid);
		if(id > -1) if(!strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true)) Houses[id][Houses_Lock] = 0;
		return 1;
	}
	if(!strcmp(cmd,"/rent",true))
	{
		new id = GetHouseID(playerid);
		set(cmd,strcharsplit(text, idx,' '));
		if(strlen(cmd) < 1) SendClientMessage(playerid,COLOUR_RED,lang_texts[8][76]);
		else if(!strcmp(cmd,"on",true))
		{
			Houses[id][Houses_Rentabil] = 1;
			new filename_housing[MAX_STRING];
			format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
			if(ini_Exist(filename_housing))
			{
				new file_housing = ini_Open(filename_housing);
				ini_SetInt(file_housing,"Rentabil",Houses[id][Houses_Rentabil]);
				ini_Close(file_housing);
			}
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][74]);
			return 1;
		}
		else if(!strcmp(cmd,"off",true))
		{
			Houses[id][Houses_Rentabil] = 0;
			new filename_housing[MAX_STRING];
			format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
			if(ini_Exist(filename_housing))
			{
				new file_housing = ini_Open(filename_housing);
				ini_SetInt(file_housing,"Rentabil",Houses[id][Houses_Rentabil]);
				ini_Close(file_housing);
			}
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][75]);
			return 1;
		}
		return 1;
	}
	return 0;
}

stock FHouseInfo(playerid) 
{
	new id=GetHouseID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][13]);
	new temp[MAX_STRING];
	format(temp,sizeof(temp),lang_texts[8][3],Houses[id][Houses_Name]);
	SendClientMessage(playerid,COLOUR_GREEN,temp);
	if(strcmp(Houses[id][Houses_Owner],"Unknown",true) && strcmp(Houses[id][Houses_Gang],"Unknown",true))
	{
		format(temp,sizeof(temp),lang_texts[8][4],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		new price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
		new UpKeep = Houses[id][Houses_UpKeep];
		if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true) || strcmp(Houses[id][Houses_Gang], GetPlayerGangName(playerid), true))
		{
			format(temp,sizeof(temp),lang_texts[8][5],price);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[8][6],UpKeep);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[8][7],Houses[id][Houses_UpKeepLeft],UpKeep*2800);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
		}
	}
	else
	{
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][9]);
		format(temp,sizeof(temp),lang_texts[8][10],Houses[id][Houses_Cost]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(temp,sizeof(temp),lang_texts[8][11],Houses[id][Houses_UpKeep],Houses[id][Houses_UpKeepLeft]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
	}
	return 1;
}

stock FHouseHouses(playerid)
{
	new temp[MAX_STRING];
	for(new id=0;id<sizeof(Houses);id++)
	{
		if(strcmp(Houses[id][Houses_Owner],"Unknown",true))
		{
			format(temp,sizeof(temp),lang_texts[8][38],Houses[id][Houses_Name],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
			SendClientMessage(playerid,COLOUR_MISC,temp);
		} 
		else 
		{
			format(temp,sizeof(temp),lang_texts[8][39],Houses[id][Houses_Name]);
			SendClientMessage(playerid,COLOUR_MISC,temp);
		}
	}
	return 1;
}

stock FHouseBuy(playerid) 
{
	for(new i=0;i<sizeof(Houses);i++)
		if(!strcmp(oGetPlayerName(playerid),Houses[i][Houses_Owner],true))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][78]);
	new temp[MAX_STRING];
	new id = GetHouseID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][21]);
	if(!IsPlayerRegistered(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][20]);
	if(GetPVarInt(playerid,"GangID") == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][19]);
	new price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
	if(oGetPlayerMoney(playerid) < price) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][18]);
	if(!strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true))
		SendClientMessage(playerid,COLOUR_RED,lang_texts[8][15]);
#if !defined BUY_ALL_HOUSES
	else if(strcmp(Houses[id][Houses_Owner],"Unknown",true))
		SendClientMessage(playerid,COLOUR_RED,"Этот дом уже куплен!");
#endif
	else
	{
		new owner=-1;
		for(new ownerid=0;ownerid<=GetPlayerLastID();ownerid++)
			if(IsPlayerConnected(ownerid)) if(!strcmp(Houses[id][Houses_Owner],oGetPlayerName(ownerid),true))
				owner = ownerid;
		oGivePlayerMoney(playerid,0-price,1);
		if(owner > -1)
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][77]);
		else
		{
			new filename_housing[MAX_STRING];
			format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,PlayerDB,Houses[id][Houses_Owner]);
			if(ini_Exist(filename_housing))
			{
				new file_housing = ini_Open(filename_housing);
				new tempmoney;
				ini_GetInt(file_housing,"Money",tempmoney);
				ini_SetInt(file_housing,"Money",tempmoney+price);
				ini_Close(file_housing);
			}
		}
		set(Houses[id][Houses_Owner],oGetPlayerName(playerid));
		set(Houses[id][Houses_Gang],GetPlayerGangName(playerid));
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][69]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][70]);
		Houses[id][Houses_Buyout] = 0;
		UpdateStreamMapIcon(house_icon[id]);
		new filename_housing[MAX_STRING],logstring[MAX_STRING];
		format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
		if(ini_Exist(filename_housing))
		{
		    new file_housing = ini_Open(filename_housing);
			ini_Set(file_housing,"Owner",Houses[id][Houses_Owner]);
			ini_Set(file_housing,"Gang",Houses[id][Houses_Gang]);
			ini_SetInt(file_housing,"Buyout",Houses[id][Houses_Buyout]);
			ini_Close(file_housing);
		}
		format(temp,sizeof(temp),lang_texts[8][17],Houses[id][Houses_Name]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(logstring,sizeof(logstring),"player: %d:  %s: bought the '%s' (house)",playerid,oGetPlayerName(playerid),Houses[id][Houses_Name]);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock FHouseSell(playerid) 
{
	FHouseKeep(playerid);
	new temp[MAX_STRING];
	new id = GetHouseID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][28]);
	if(!IsPlayerRegistered(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[8][26]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[8][27]);
		return 1;
	}
	if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true))
		SendClientMessage(playerid,COLOUR_RED,lang_texts[8][23]);
	else
	{
		new price = ((Houses[id][Houses_Cost] + Houses[id][Houses_Buyout]) * 85) / 100;
		oGivePlayerMoney(playerid,price,1);
		set(Houses[id][Houses_Owner],"Unknown");
		set(Houses[id][Houses_Gang],"Unknown");
		Houses[id][Houses_Buyout] = 0;
		UpdateStreamMapIcon(house_icon[id]);
		SaveHous(id);
		format(temp,sizeof(temp),lang_texts[8][24],Houses[id][Houses_Name]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][25]);
		new logstring[MAX_STRING];
		format(logstring,sizeof(logstring),"Player: %s [ID %d]: sold the '%s' (house)",oGetPlayerName(playerid),playerid,Houses[id][Houses_Name]);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock FHouseMyHouses(playerid) 
{
	new temp[MAX_STRING];
	new count = 0;
	for(new id=0;id<sizeof(Houses);id++)
	{
		if(!strcmp(Houses[id][Houses_Gang],GetPlayerGangName(playerid),true))
		{
			count++;
			format(temp,sizeof(temp),lang_texts[8][40],Houses[id][Houses_Name],Houses[id][Houses_Owner],Houses[id][Houses_Cost]);
			SendClientMessage(playerid,COLOUR_MISC,temp);
			format(temp,sizeof(temp),lang_texts[8][41],Houses[id][Houses_UpKeep], Houses[id][Houses_UpKeepLeft]);
			SendClientMessage(playerid,COLOUR_MISC,temp);
		}
	}
	if(count < 1) SendClientMessage(playerid,COLOUR_RED,lang_texts[8][42]);
	return 1;
}

stock FHouseKeep(playerid) 
{
	new id=GetHouseID(playerid);
	if(id > -1)
	{
		if(IsPlayerRegistered(playerid))
		{
			if(strcmp(Houses[id][Houses_Gang],GetPlayerGangName(playerid),true) == 0)
			{
				if(oGetPlayerMoney(playerid) > Houses[id][Houses_UpKeepLeft]) 
				{
					if(Houses[id][Houses_UpKeepLeft] > 0)
					{
						oGivePlayerMoney(playerid,-Houses[id][Houses_UpKeepLeft],0);
						SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][44]);
						Houses[id][Houses_UpKeepLeft] = 0;
						new filename_housing[MAX_STRING];
						format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
						if(ini_Exist(filename_housing))
						{
							new file_housing = ini_Open(filename_housing);
							ini_SetInt(file_housing,"UpKeepLeft",Houses[id][Houses_UpKeepLeft]);
							ini_Close(file_housing);
						}
					} 
					else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][45]);
				}
				else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][46]);
			} 
			else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][47]);
		} 
		else 
		{
			SendClientMessage(playerid,COLOUR_RED,lang_texts[8][48]);
			SendClientMessage(playerid,COLOUR_RED,lang_texts[8][49]);
		}
	} 
	else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][50]);
}

stock ganghouse(playerid) 
{
	new hova = -1,tmp[MAX_STRING];
	if(!IsPlayerRegistered(playerid) || !strlen( GetPlayerGangName(playerid) )) return hova;
	for(new id=0;id<sizeof(Houses);id++)
	{
		if(!strcmp(Houses[id][Houses_Gang],GetPlayerGangName(playerid),true)) 
		{
			hova = id;
			format(tmp,sizeof(tmp),lang_texts[8][51],Houses[id][Houses_Name],Houses[id][Houses_UpKeepLeft],Houses[id][Houses_UpKeep]*2800);
			SendClientMessage(playerid,COLOUR_RED,tmp);
		}
		if(!strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true)) hova = id;
	}
	return hova;
}

stock gang2house(playerid,i)
{
	SetPlayerPos(playerid,Houses[i][Houses_PickupX],Houses[i][Houses_PickupY],Houses[i][Houses_PickupZ]+0.5);
}

stock HouseKeepUp()
{
	new upkeepamount,upkeep,upkeepleft;
	for(new id=0;id<sizeof(Houses);id++)
	{
		if(strcmp(Houses[id][Houses_Gang],"Unknown",true) != 0) 
		{
			upkeep = Houses[id][Houses_UpKeep];
			upkeepleft = Houses[id][Houses_UpKeepLeft];
			upkeepamount = upkeep + upkeepleft;
			if(upkeepamount < upkeep*2800) 
			{
				Houses[id][Houses_UpKeepLeft] = upkeepamount;
				new filename_housing[MAX_STRING];
				format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
				if(ini_Exist(filename_housing))
				{
					new file_housing = ini_Open(filename_housing);
					ini_SetInt(file_housing,"UpKeepLeft",upkeepamount);
					ini_Close(file_housing);
				}
			} 
			else 
			{ 
				new filename_housing[MAX_STRING];
				format(filename_housing,sizeof(filename_housing),"%s%s"GTO_FILES_FORMAT,HousesDB,Houses[id][Houses_Name]);
				if(ini_Exist(filename_housing))
				{
					new file_housing = ini_Open(filename_housing);
					ini_Set(file_housing,"Gang","Unknown");
					ini_Set(file_housing,"Owner","Unknown");
					ini_Set(file_housing,"UpKeepLeft","0");
					ini_Close(file_housing);
				}   
			}
		} 
	}
}

stock housing_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	for(new id=0;id<sizeof(Houses);id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,3,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]+0.5))
		{
			new string[MAX_STRING],string2[MAX_STRING],string3[MAX_STRING],string4[MAX_STRING],string5[MAX_STRING],string6[MAX_STRING],dialog[MAX_STRING];
			format(string,sizeof(string),lang_texts[8][3],Houses[id][Houses_Name]);
			new price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
			new UpKeep = Houses[id][Houses_UpKeep];
			if(strcmp(Houses[id][Houses_Owner],"Unknown",true) && strcmp(Houses[id][Houses_Gang],"Unknown",true))
			{
				if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true) || strcmp(Houses[id][Houses_Gang],GetPlayerGangName(playerid),true))
				{
					if(strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true) != 0) Hous[playerid] = 1;
					else Hous[playerid] = 4;
					format(string2,sizeof(string2),lang_texts[8][5],price);
					format(string3,sizeof(string3),lang_texts[8][6],UpKeep);
					format(string4,sizeof(string4),lang_texts[8][7],Houses[id][Houses_UpKeepLeft],UpKeep*2800);
					format(string5,sizeof(string5),lang_texts[8][4],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
					if(strcmp(Houses[id][Houses_RentName],"Unknown",true) == 0) format(string6,sizeof(string6),lang_texts[8][53],Houses[id][Houses_RentCost]);
					else format(string6,sizeof(string6),lang_texts[8][52],Houses[id][Houses_RentName]);
					format(dialog,sizeof(dialog),"%s\n%s\n%s\n%s\n%s\n",string2,string3,string4,string5,string6);
					ShowPlayerDialog(playerid,house_DialogID,DIALOG_STYLE_MSGBOX,string,dialog,"Действия","Отмена");
				}
				else
				{
					format(string2,sizeof(string2),lang_texts[8][5],price);
					format(string3,sizeof(string3),lang_texts[8][6],UpKeep);
					if(strcmp(Houses[id][Houses_RentName],"Unknown",true) == 0) format(string4,sizeof(string4),lang_texts[8][53],Houses[id][Houses_RentCost]);
					else format(string4,sizeof(string4),lang_texts[8][52],Houses[id][Houses_RentName]);
					format(dialog,sizeof(dialog),"%s\n%s\n%s\n",string2,string3,string4);
					Hous[playerid] = 2;
					ShowPlayerDialog(playerid,house_DialogID,DIALOG_STYLE_MSGBOX,string,dialog,"Действия","Отмена");
				}
			}
			else
			{
				if(strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true) != 0) Hous[playerid] = 3;
				else Hous[playerid] = 4;
				format(string2,sizeof(string2),lang_texts[8][9]);
				format(string3,sizeof(string3),lang_texts[8][10],Houses[id][Houses_Cost]);
				format(string4,sizeof(string4),lang_texts[8][11],Houses[id][Houses_UpKeep],Houses[id][Houses_UpKeepLeft]);
				format(string5,sizeof(string5),lang_texts[8][53],Houses[id][Houses_RentCost]);
				format(dialog,sizeof(dialog),"%s\n%s\n%s\n%s\n",string2,string3,string4,string5);
				ShowPlayerDialog(playerid,house_DialogID,DIALOG_STYLE_MSGBOX,string,dialog,"Действия","Отмена");
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid,2,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ]))
		{
			SetPlayerInterior(playerid,0);
			gang2house(playerid,Haus[playerid]);
			Haus[playerid] = -1;
		}
	}
	return 1;
}

stock housing_Update3DTextLabelText()
{
	new string[MAX_STRING];
	for(new id=0;id<sizeof(Houses);id++)
	{
		format(string,sizeof(string),"Дом: %s\nСтоимость: %d\nКуплен: %s\nБанда: %s\nАрендуется: %s",Houses[id][Houses_Name],Houses[id][Houses_Cost],Houses[id][Houses_Owner],Houses[id][Houses_Gang],Houses[id][Houses_RentName]);
		Update3DTextLabelText(House3DTextLabel[id],COLOUR_WHITE,string);
	}
	return 1;
}
