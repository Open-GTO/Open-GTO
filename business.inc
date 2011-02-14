//
// Created:     27.11.06
// Aurthor:     Peter Steenbergen
//
#if defined _business_included
#endinput
#endif

#define _business_included
#pragma library business

#include "base"
#include "utils\gtoutils"
#include "account"
#include "player"
#include "world"

#define MAX_BUSINESBUYSELL 1000000
#define MAX_UPGRADELEVEL 30
#define TurnAroundDelay 200000 // Delay between businesses turnaround
#define BusinessDialog 4

new Text3D:Business3DTextLabel[MAX_BUSINESSES],Float:BusinessDistanceOfShowLabel=20.0;

BusinessLoadAll()
{
    new file_business;
	if(ini_Exist(ConfigDB))
	{
		file_business = ini_Open(ConfigDB);
		ini_Get(file_business,"Business_DB",BusinessDB);
		ini_GetFloat(file_business,"Business_DistanceOfShowLabel",BusinessDistanceOfShowLabel);
		ini_Close(file_business);
	}
	if(!ini_Exist(BusinessDB))
	{
		BusinessSaveAll();
		return;
	}
	for(new i=0;i<MAX_BUSINESSES;i++)
	{
		new businessdbname[MAX_STRING];
		format(businessdbname,sizeof(businessdbname),"%s%s.txt",BusinessDB,Businesses[i][Business_Name]);
		if(!ini_Exist(businessdbname))
		{
			new File:fb = fopen(businessdbname,io_write);
			fclose(fb);
			BusinessSaveAll();
		}
		file_business = ini_Open(businessdbname);
		ini_Get(file_business,"Name",Businesses[i][Business_Name],MAX_STRING);
        ini_GetInt(file_business,"Cost",Businesses[i][Business_Cost]);
        ini_GetInt(file_business,"Value",Businesses[i][Business_Value]);
		ini_GetInt(file_business,"Level",Businesses[i][Business_Level]);
        ini_Get(file_business,"Owner",Businesses[i][Business_Owner],MAX_STRING);
        ini_GetInt(file_business,"Buyout",Businesses[i][Business_Buyout]);
        ini_GetInt(file_business,"Vault",Businesses[i][Business_Vault]);
        ini_GetInt(file_business,"Upgrade",Businesses[i][Business_Upgrade]);
		ini_GetInt(file_business,"ShowIcon",Businesses[i][Business_ShowIcon]);
		ini_GetFloat(file_business,"Coord_X",Businesses[i][Coord_X]);
		ini_GetFloat(file_business,"Coord_Y",Businesses[i][Coord_Y]);
		ini_GetFloat(file_business,"Coord_Z",Businesses[i][Coord_Z]);
		ini_Close(file_business);
	}
	return;
}

BusinessSaveAll()
{
	new file_business;
	if(ini_Exist(ConfigDB))
	{
		file_business = ini_Open(ConfigDB);
		ini_Set(file_business,"Business_DB",BusinessDB);
		ini_SetFloat(file_business,"Business_DistanceOfShowLabel",BusinessDistanceOfShowLabel);
		ini_Close(file_business);
	}
	for(new i=0;i<MAX_BUSINESSES;i++)
	{
		new businessdbname[MAX_STRING];
		format(businessdbname,sizeof(businessdbname),"%s%s.txt",BusinessDB,Businesses[i][Business_Name]);
		file_business = (!ini_Exist(businessdbname)) ? ini_Create(businessdbname) : ini_Open(businessdbname);
		ini_Set(file_business,"Name",Businesses[i][Business_Name]);
		ini_SetInt(file_business,"Cost",Businesses[i][Business_Cost]);
        ini_SetInt(file_business,"Value",Businesses[i][Business_Value]);
		ini_SetInt(file_business,"Level",Businesses[i][Business_Level]);
		ini_Set(file_business,"Owner",Businesses[i][Business_Owner]);
        ini_SetInt(file_business,"Buyout",Businesses[i][Business_Buyout]);
        ini_SetInt(file_business,"Vault",Businesses[i][Business_Vault]);
        ini_SetInt(file_business,"Upgrade",Businesses[i][Business_Upgrade]);
		ini_SetInt(file_business,"ShowIcon",Businesses[i][Business_ShowIcon]);
		ini_SetFloat(file_business,"Coord_X",Businesses[i][Coord_X]);
		ini_SetFloat(file_business,"Coord_Y",Businesses[i][Coord_Y]);
		ini_SetFloat(file_business,"Coord_Z",Businesses[i][Coord_Z]);
		ini_Close(file_business);
	}
	return;
}

stock business_OnGameModeInit()
{
	BusinessLoadAll();
	new string[MAX_STRING];
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		CreatePickup(1274,49,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]+0.5);	//синий домик
		if(Businesses[id][Business_ShowIcon] == 1)
			CreateStreamMapIcon(52,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]);
		format(string,sizeof(string),"Бизнес: %s\nСтоимость: %d\nПрибыль: %d\nСейчас принадлежит: %s\nМин.Уровень: %d\nУровень бизнеса: %d",Businesses[id][Business_Name],Businesses[id][Business_Cost],Businesses[id][Business_Value],Businesses[id][Business_Owner],Businesses[id][Business_Level],Businesses[id][Business_Upgrade]);
		Business3DTextLabel[id] = Create3DTextLabel(string,COLOUR_WHITE,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]+0.75,BusinessDistanceOfShowLabel,0,1);
	}
	WriteLog(GameLog,"Business initialised");
}

stock BisMenuInit(playerid)
{
	if(IsPlayerNPC(playerid)) return 0;
	new id=GetBusinessID(playerid),temp[MAX_STRING];
	format(temp,sizeof(temp), "Вы в бизнессе: %s",Businesses[id][Business_Name]);
	ShowPlayerDialog(playerid,BusinessDialog,DIALOG_STYLE_LIST,temp,"Инфо\nКупить\nПродать\nЗабрать Прибыль\nУвеличить Уровень\nМой бизнесс\nВсе бизнессы","Ok","Отмена");
	return 1;
}

stock IsPlayerAtBusiness(playerid)
{
	for(new id=0;id<MAX_BUSINESSES;id++) if(IsPlayerInRangeOfPoint(playerid,2,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z])) return 1;
	return 0;
}

stock GetBusinessID(playerid)
{
	new lookupid=-1;
	for(new id=0;id<MAX_BUSINESSES;id++) if(IsPlayerInRangeOfPoint(playerid,2,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z])) lookupid = id;
	return lookupid;
}

stock TurnAround()
{
	new file_busin;
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			if(Businesses[id][Business_Vault] < Businesses[id][Business_Value] * Businesses[id][Business_Upgrade] * 50)
			{
				Businesses[id][Business_Vault] += (Businesses[id][Business_Value] * Businesses[id][Business_Upgrade]) / 1000;
				new filename_busin[MAX_STRING];
				format(filename_busin,sizeof(filename_busin),"%s%s.txt",BusinessDB,Businesses[id][Business_Name]);
				if(ini_Exist(filename_busin))
				{
					file_busin = ini_Open(filename_busin);
					ini_SetInt(file_busin,"Vault",Businesses[id][Business_Vault]);
					ini_Close(file_busin);
				}
			}
		}
	}
}

stock business_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if (!IsPlayerConnected(playerid)) return 0;
	new cmd[20],idx;

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

/*	if(!strcmp("/bmenu", cmd, true, 5))
	{
		if(!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][2]  , 0,COLOUR_RED);
			return 1;
		}
		else BisMenuInit(playerid);
	}
	if((!strcmp(cmd,"/businessinfo",true)) || (!strcmp(cmd,"/binfo",true)))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][2]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisInfo(playerid);
	}
	if((!strcmp(cmd, "/businessbuy", true)) || (!strcmp(cmd, "/bbuy", true)))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][19]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisBuy(playerid);
	}
	if((!strcmp(cmd, "/businesssell", true)) || (!strcmp(cmd, "/bsell", true)))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][28]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisSell(playerid);
	}
	if(!strcmp(cmd,"/collect",true))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][46]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisCollect(playerid);
	}
	if(!strcmp(cmd,"/businesses",true)) FBisBises(playerid);
	if(!strcmp(cmd,"/mybusinesses",true)) FBisMyBises(playerid);
	if(!strcmp(cmd,"/buyupgrade",true))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][46]  , 0,COLOUR_RED);
			return 1;
		}
		else BbuyUpgrade(playerid);
		return 1;
	}*/
	return 0;
}
//----------------RECOMPILED BUSINESSES----------------------------------------
stock FBisInfo(playerid)
{
	new id=GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][18]);
	new temp[MAX_STRING];
	format(temp,sizeof(temp),lang_texts[3][3],Businesses[id][Business_Name]);
	SendClientMessage(playerid,COLOUR_GREEN,temp);
	if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
	{
		format(temp,sizeof(temp), lang_texts[3][4] ,Businesses[id][Business_Owner]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
		if(strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
		{
			format(temp,sizeof(temp),lang_texts[3][5],price);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[3][6],Businesses[id][Business_Level]);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[3][7],Businesses[id][Business_Value]);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[3][57],Businesses[id][Business_Upgrade]);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[3][8]);
		}
		else
		{
			price = (price * Businesses[id][Business_Upgrade]) / 100;
			if(Businesses[id][Business_Vault] > 0)
			{
				format(temp,sizeof(temp), lang_texts[3][9] ,Businesses[id][Business_Vault]);
				SendClientMessage(playerid,COLOUR_GREEN,temp);
			}
			format(temp,sizeof(temp),lang_texts[3][57],Businesses[id][Business_Upgrade]);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp), lang_texts[3][11] ,price);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[3][12]);
		}
	}
	else
	{
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[3][13]);
		format(temp,sizeof(temp), lang_texts[3][14] ,Businesses[id][Business_Cost]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(temp,sizeof(temp), lang_texts[3][15] ,Businesses[id][Business_Level]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(temp,sizeof(temp), lang_texts[3][16] ,Businesses[id][Business_Value]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(temp,sizeof(temp), lang_texts[3][57] ,Businesses[id][Business_Upgrade]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[3][17]);
	}
	return 1;
}

stock FBisBuy(playerid)
{
	new id = GetBusinessID(playerid),temp[MAX_STRING];
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][27]);
	if(!IsPlayerRegistered(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][26]);
	if(Player[playerid][Level] < Businesses[id][Business_Level])
	{
		format(temp,sizeof(temp), lang_texts[3][25],Businesses[id][Business_Level]);
		SendClientMessage(playerid,COLOUR_RED,temp);
		return 1;
	}
	new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
	if(Player[playerid][Money] < price) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][24]);
	if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(playerid),true)) SendClientMessage(playerid,COLOUR_RED,lang_texts[3][20]);
	else
	{
		new owner=-1;
		for(new ownerid=0;ownerid<MAX_PLAYERS;ownerid++)
		{
			if(IsPlayerConnected(ownerid) && !strcmp(Businesses[id][Business_Owner],oGetPlayerName(ownerid),true))
			{
				format(temp,sizeof(temp),lang_texts[3][21],Businesses[id][Business_Name],oGetPlayerName(playerid));
				SendClientMessage(ownerid,COLOUR_RED,temp);
				owner=ownerid;
			}
		}
		oGivePlayerMoney(playerid,0-price,1);
		if(owner > -1) oGivePlayerMoney(owner,0+price,1);
		else
		{
			new filename[MAX_STRING];
			format(filename,sizeof(filename),"%s%s.txt",PlayerDB,Businesses[id][Business_Owner]);
			if(ini_Exist(filename))
			{
				new tempmoney,file = ini_Open(filename);
				ini_GetInt(file,"Money",tempmoney);
				ini_SetInt(file,"Money",tempmoney+price);
				ini_Close(file);
			}
		}
		set(Businesses[id][Business_Owner],oGetPlayerName(playerid));
		Businesses[id][Business_Buyout] = 0;
		new filename[MAX_STRING];
		format(filename,sizeof(filename),"%s%s.txt",BusinessDB,Businesses[id][Business_Name]);
		if(ini_Exist(filename))
		{
			new file = ini_Open(filename);
			ini_Set(file,"Owner",Businesses[id][Business_Owner]);
			ini_SetInt(file,"Buyout",Businesses[id][Business_Buyout]);
			ini_Close(file);
		}
		format(temp,sizeof(temp),lang_texts[3][22],Businesses[id][Business_Name]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);

		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (business)",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name]);
		WriteLog(GameLog,logstring);

		if(Businesses[id][Business_Vault] > 0)
		{
			format(temp,sizeof(temp),lang_texts[3][23] ,Businesses[id][Business_Vault]);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
		}
	}
	return 1;
}

stock FBisSell(playerid)
{
	new id=GetBusinessID(playerid),temp[MAX_STRING];
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][36]);
	if(!IsPlayerRegistered(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[3][34]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[3][35]);
		return 1;
	}
	if(strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		SendPlayerFormattedText(playerid,  lang_texts[3][29] , 0,COLOUR_RED);
	}
	else
	{
		new price = ((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100;
		oGivePlayerMoney(playerid,0+price,1);
		set(Businesses[id][Business_Owner],"Unknown");
		Businesses[id][Business_Buyout] = 0;
		Businesses[id][Business_Upgrade] = 1;
		new filename[MAX_STRING];
		format(filename,sizeof(filename),"%s%s.txt",BusinessDB,Businesses[id][Business_Name]);
		if(ini_Exist(filename))
		{
			new file = ini_Open(filename);
			ini_Set(file,"Owner","Unknown");
			ini_SetInt(file,"Buyout",Businesses[id][Business_Buyout]);
			ini_SetInt(file,"Upgrade",Businesses[id][Business_Upgrade]);
			ini_Close(file);
		}
		format(temp,sizeof(temp), lang_texts[3][30] ,Businesses[id][Business_Name]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[3][31]);
		if(Businesses[id][Business_Vault] > 0)
		{
			format(temp,sizeof(temp),lang_texts[3][32],Businesses[id][Business_Vault]);
			SendClientMessage(playerid,COLOUR_RED,temp);
			SendClientMessage(playerid,COLOUR_RED,lang_texts[3][33]);
		}
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (business)",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name]);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock FBisCollect(playerid)
{
	new id = GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][52]);
	if(!IsPlayerRegistered(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[3][50]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[3][51]);
		return 1;
	}
	if(strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		SendPlayerFormattedText(playerid,  lang_texts[3][47] , 0,COLOUR_RED);
	}
	else
	{
		if(Businesses[id][Business_Vault] > 0)
		{
			oGivePlayerMoney(playerid,0+Businesses[id][Business_Vault]*Businesses[id][Business_Upgrade],1);
			SendPlayerFormattedText(playerid,  lang_texts[3][48] , 0,COLOUR_GREEN);
			Businesses[id][Business_Vault] = 0;
			new filename[MAX_STRING];
			format(filename,sizeof(filename),"%s%s.txt",BusinessDB,Businesses[id][Business_Name]);
			if(ini_Exist(filename))
			{
				new file = ini_Open(filename);
				ini_SetInt(file,"Vault",Businesses[id][Business_Vault]);
				ini_Close(file);
			}
		}
		else SendPlayerFormattedText(playerid,  lang_texts[3][49] , 0,COLOUR_RED);
	}
	return 1;
}

stock FBisBises(playerid)
{
	new temp[MAX_STRING];
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			format(temp,sizeof(temp),lang_texts[3][53],Businesses[id][Business_Name],Businesses[id][Business_Owner]);
			SendPlayerScrollingText(playerid,temp);
		}
		else
		{
			format(temp,sizeof(temp),lang_texts[3][54],Businesses[id][Business_Name]);
			SendPlayerScrollingText(playerid,temp);
		}
	}
	return 1;
}

stock FBisMyBises(playerid)
{
	new temp[MAX_STRING],count = 0;
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(!strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
		{
			count++;
			format(temp,sizeof(temp),lang_texts[3][55],Businesses[id][Business_Name],Businesses[id][Business_Vault]);
			SendPlayerScrollingText(playerid,temp);
		}
	}
	if(count < 1) SendClientMessage(playerid,COLOUR_RED,lang_texts[3][56]);
	return 1;
}

stock BbuyUpgrade(playerid)
{
	if(!IsPlayerRegistered(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][59]);
	new id=GetBusinessID(playerid);
	if(strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][47]);
	}
	else
	{
		if(Businesses[id][Business_Upgrade] == 30)
		{
			new st[MAX_STRING];
			format(st,sizeof(st),lang_texts[3][60],Businesses[id][Business_Name]);
			SendClientMessage(playerid,COLOUR_RED,st);
			return 1;
		}
		new price;
		if(Businesses[id][Business_Upgrade] == MAX_UPGRADELEVEL) price = 35000;
		else if(Businesses[id][Business_Upgrade] > 20) price = 28000;
		else if(Businesses[id][Business_Upgrade] > 10) price = 22000;
		else if(Businesses[id][Business_Upgrade] < 10) price = 15000;
		if(Player[playerid][Money] < price) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][61]);
		oGivePlayerMoney(playerid,-price,0);
		new filename[MAX_STRING];
		format(filename,sizeof(filename),"%s%s.txt",BusinessDB,Businesses[id][Business_Name]);
		Businesses[id][Business_Upgrade]++;
		if(ini_Exist(filename))
		{
		    new file = ini_Open(filename);
			ini_SetInt(file,"Upgrade",Businesses[id][Business_Upgrade]);
			ini_Close(file);
		}
		new str[MAX_STRING];
		format(str,sizeof(str),lang_texts[3][58],Businesses[id][Business_Name],Businesses[id][Business_Upgrade]);
		SendClientMessage(playerid,COLOUR_GREEN,str);
	}
	return 1;
}

stock business_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
    if(dialogid == BusinessDialog)
    {
		if(!response) return 1;
		switch(listitem)
		{
			case 0:{FBisInfo(playerid); BisMenuInit(playerid);}
			case 1:FBisBuy(playerid);
			case 2:FBisSell(playerid);
			case 3:FBisCollect(playerid);
			case 4:BbuyUpgrade(playerid);
			case 5:{FBisMyBises(playerid); BisMenuInit(playerid);}
			case 6:{FBisBises(playerid); BisMenuInit(playerid);}
		}
		return 1;
 	}
    return 1;
}

stock business_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	new id = IsPlayerAtBusinesses(playerid);
	new temp[MAX_STRING];
	format(temp,sizeof(temp),lang_texts[3][1],Businesses[id][Business_Name]);
	BisMenuInit(playerid);
	SendClientMessage(playerid,COLOUR_GREEN,temp);
	return 1;
}

stock IsPlayerAtBusinesses(playerid)
{
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,10,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]))
			return id;
	}
	return -1;
}

stock business_Update3DTextLabelText()
{
	new string[MAX_STRING];
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		format(string,sizeof(string),"Бизнес: %s\nСтоимость: %d\nПрибыль: %d\nСейчас принадлежит: %s\nМин.Уровень: %d\nУровень бизнеса: %d",Businesses[id][Business_Name],Businesses[id][Business_Cost],Businesses[id][Business_Value],Businesses[id][Business_Owner],Businesses[id][Business_Level],Businesses[id][Business_Upgrade]);
		Update3DTextLabelText(Business3DTextLabel[id],COLOUR_WHITE,string);
	}
	return 1;
}