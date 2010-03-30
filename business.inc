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
#define MAX_BUSINESSES 31
#define MAX_UPGRADELEVEL 30
#define TurnAroundDelay 200000 // Delay between businesses turnaround
#define BusinessDialog 4

enum BusinessInfo {
	Business_Name[MAX_STRING],  // business name
	Business_Cost,     // cost
	Business_Value,        // value
	Business_Level,            // minumum level
	Business_Owner[MAX_NAME],   // owner
	Business_Buyout,            // Buyout price
	Business_Vault,              // Vault
	Business_Upgrade,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z
}

new Businesses[MAX_BUSINESSES][BusinessInfo] = {
	//NAME,COST,VALUE,LEVEL,OWNER,BUYOUT,Vault,Upgrade,X,Y,Z
	{"Botique",35000,800,5,"Unknown",0,0,0,				418.5547,-80.1667,1001.8047},
	{"Shithole Bar",25000,1300,8,"Unknown",0,0,0,		501.4927,-75.4323,998.7578},
	{"Tattoo Parlor",40000,600,3,"Unknown",0,0,0,		-203.4864,-41.2045,1002.2734},
	{"Binco Clothing",15000,800,5,"Unknown",0,0,0,		207.5640,-97.8188,1005.2578},
	{"Train Hard Clothing",15000,800,5,"Unknown",0,0,0,	210.1111,-129.1273,1003.5152},
	{"Urban Clothing",15000,800,5,"Unknown",0,0,0,		203.8557,-40.4717,1001.8047},
	{"Victim Clothing",15000,800,5,"Unknown",0,0,0,		210.3724,-8.1825,1005.2109},
	{"Zip Clothing",15000,800,5,"Unknown",0,0,0,		161.1875,-79.9915,1001.8047},
	{"DidierSachs Clothing",20000,950,7,"Unknown",0,0,0,204.2810,-157.2614,1000.5234},
	{"Gym",20000,1000,7,"Unknown",0,0,0,				773.6138,-72.1616,1000.6484},
	{"Zero RC",10000,900,3,"Unknown",0,0,0,				-2231.2478,131.9623,1035.4141},
	{"Zombotech",35000,1600,12,"Unknown",0,0,0,			-1951.5938,704.3567,46.5625},
	{"Verdant Meadows Air Strip",20000,1200,7,"Unknown",0,0,0,418.1210,2536.8762,10.0000},
	{"Blueberry Liquor Store",15000,850,5,"Unknown",0,0,0,252.4851,-57.3515,1.5703},
	{"Club-Disco",50000,1600,10,"Unknown",0,0,0,		500.7049,-13.3777,1000.6797},
	{"Lil Probe-Inn",47000,1100,6,"Unknown",0,0,0,		-225.7790,1403.9459,27.7734},
	{"Sex Shop",40000,950,7,"Unknown",0,0,0,			-103.5525,-22.4661,1000.7188},
	{"Strip Club",60000,1400,10,"Unknown",0,0,0,		1212.3918,-10.9459,1000.9219},
	{"The Big Spread Ranch",48000,1450,9,"Unknown",0,0,0,1208.5027,-32.6044,1000.9531},
	{"Jizzys",58000,1675,13,"Unknown",0,0,0,			-2650.0667,1409.2106,906.2734},
	{"The Sherman Dam",58000,1800,16,"Unknown",0,0,0,	-955.5327,1948.5414,9.0000},
	{"Cobra Marital Arts",15000,800,5,"Unknown",0,0,0,	768.2014,-36.9461,1000.6865},
	{"City Planning Department",35000,1600,12,"Unknown",0,0,0,357.0584,173.5787,1008.3820},
	{"Inside Track",20000,1200,7,"Unknown",0,0,0,		823.1819,3.2361,1004.1797},
	{"Coutt and Schutz Auto",25000,1250,8,"Unknown",0,0,0,2131.6455,-1148.2836,24.3898},
	{"Ottos Auto",25000,1250,8,"Unknown",0,0,0,			-1657.8334,1207.3041,7.2500},
	{"Wang Cars",25000,1250,8,"Unknown",0,0,0,			-1957.5327,300.2131,35.4688},
	{"Emerald Isle",160000,4100,17,"Unknown",0,0,0,		2127.5940,2370.4255,10.8203},
	{"The Visage",145000,3800,17,"Unknown",0,0,0,		2022.5179,1916.6848,12.3397},
	{"Caligulas",3750000,19900,20,"Unknown",0,0,0,		2235.5408,1679.0402,1008.3594},
	{"Four Dragons",2050000,16000,20,"Unknown",0,0,0,	1993.9758,1017.8945,994.8906}
};

new BusinessPick[MAX_BUSINESSES];
new BusinessDB[MAX_STRING] = "GTO/Business/";

BusinessLoadAll()
{
    new file;
	if(!ini_Exist(ConfigDB)) {new File:fgh = fopen(ConfigDB,io_write);fclose(fgh);}
	file = ini_Open(ConfigDB);
	ini_Get(file,"Business_DB",BusinessDB);
	ini_Close(file);
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
		file = ini_Open(businessdbname);
		ini_Get(file,"Name",Businesses[i][Business_Name],MAX_STRING);
        ini_GetInt(file,"Cost",Businesses[i][Business_Cost]);
        ini_GetInt(file,"Value",Businesses[i][Business_Value]);
		ini_GetInt(file,"Level",Businesses[i][Business_Level]);
        ini_Get(file,"Owner",Businesses[i][Business_Owner],MAX_STRING);
        ini_GetInt(file,"Buyout",Businesses[i][Business_Buyout]);
        ini_GetInt(file,"Vault",Businesses[i][Business_Vault]);
        ini_GetInt(file,"Upgrade",Businesses[i][Business_Upgrade]);
		ini_GetFloat(file,"Coord_X",Businesses[i][Coord_X]);
		ini_GetFloat(file,"Coord_Y",Businesses[i][Coord_Y]);
		ini_GetFloat(file,"Coord_Z",Businesses[i][Coord_Z]);
		ini_Close(file);
	}
	return;
}

BusinessSaveAll()
{
	new file;
	if(ini_Exist(ConfigDB))
	{
		file = ini_Open(ConfigDB);
		ini_Set(file,"Business_DB",BusinessDB);
		ini_Close(file);
	}
	for(new i=0;i<MAX_BUSINESSES;i++)
	{
		new businessdbname[MAX_STRING];
		format(businessdbname,sizeof(businessdbname),"%s%s.txt",BusinessDB,Businesses[i][Business_Name]);
		if(!ini_Exist(businessdbname)) {new File:fgh = fopen(businessdbname,io_write);fclose(fgh);}
		file = ini_Open(businessdbname);
		ini_Set(file,"Name",Businesses[i][Business_Name]);
		ini_SetInt(file,"Cost",Businesses[i][Business_Cost]);
        ini_SetInt(file,"Value",Businesses[i][Business_Value]);
		ini_SetInt(file,"Level",Businesses[i][Business_Level]);
		ini_Set(file,"Owner",Businesses[i][Business_Owner]);
        ini_SetInt(file,"Buyout",Businesses[i][Business_Buyout]);
        ini_SetInt(file,"Vault",Businesses[i][Business_Vault]);
        ini_SetInt(file,"Upgrade",Businesses[i][Business_Upgrade]);
		ini_SetFloat(file,"Coord_X",Businesses[i][Coord_X]);
		ini_SetFloat(file,"Coord_Y",Businesses[i][Coord_Y]);
		ini_SetFloat(file,"Coord_Z",Businesses[i][Coord_Z]);
		ini_Close(file);
	}
	return;
}

stock business_OnGameModeInit()
{
	BusinessLoadAll();
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		BusinessPick[id] = CreatePickup(1274,49,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]+0.5);	//синий домик
		CreateStreamMapIcon(52,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]);
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
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,2,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z])) return 1;
	}
	return 0;
}

stock GetBusinessID(playerid)
{
	new lookupid=-1;
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,2,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z])) lookupid = id;
	}
	return lookupid;
}

stock TurnAround()
{
	new file,vaultamount,vaultmax;
	for(new id=0;id<MAX_BUSINESSES;id++)
	{
		if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			vaultmax = Businesses[id][Business_Value] * 5;
			vaultamount = Businesses[id][Business_Vault] + Businesses[id][Business_Value] * Businesses[id][Business_Upgrade];
			if(vaultamount < vaultmax)
			{
				Businesses[id][Business_Vault] = vaultamount;
				new filename[MAX_STRING];
				format(filename,sizeof(filename),"%s%s.txt",BusinessDB,Businesses[id][Business_Name]);
				if(ini_Exist(filename))
				{
				    file = ini_Open(filename);
					ini_SetInt(file,"Vault",Businesses[id][Business_Vault]);
					ini_Close(file);
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

	if (!strcmp("/bmenu", cmd, true, 5))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][2]  , 0,COLOUR_RED);
			return 1;
		}
		else BisMenuInit(playerid);
	}
	//-- businessinfo --
	if((!strcmp(cmd, "/businessinfo", true)) || (!strcmp(cmd, "/binfo", true)))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][2]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisInfo(playerid);
	}

	//-- buybusiness --
	if((!strcmp(cmd, "/businessbuy", true)) || (!strcmp(cmd, "/bbuy", true)))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][19]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisBuy(playerid);
	}

	//-- sellbusiness --
	if((!strcmp(cmd, "/businesssell", true)) || (!strcmp(cmd, "/bsell", true)))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][28]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisSell(playerid);
	}
	//-- collect vault --
	if(!strcmp(cmd, "/collect", true))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][46]  , 0,COLOUR_RED);
			return 1;
		}
		else FBisCollect(playerid);
	}

	//-- businesses list --
	if(!strcmp(cmd, "/businesses", true)) FBisBises(playerid);
	//--  my businesses list --
	if(!strcmp(cmd, "/mybusinesses", true)) FBisMyBises(playerid);
	// -- upgrade businesess
	if(!strcmp(cmd, "/buyupgrade", true))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
		SendPlayerFormattedText(playerid,  lang_texts[3][46]  , 0,COLOUR_RED);
		return 1;
		}
		else BbuyUpgrade(playerid);
	return 1;
	}
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
				SendClientMessage(playerid,COLOUR_GREEN,lang_texts[3][10]);
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
	if(Player[playerid][Money] >= price) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][24]);
	if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(playerid), true))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[3][20]);
	}
	else
	{
		new owner=-1;
		for(new ownerid = 0; ownerid < MAX_PLAYERS; ownerid++)
		{
			if(IsPlayerConnected(ownerid))
			{
				if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(ownerid), true))
				{
					format(temp,sizeof(temp),lang_texts[3][21],Businesses[id][Business_Name],oGetPlayerName(playerid));
					SendClientMessage(ownerid,COLOUR_RED,temp);
					owner=ownerid;
				}
			}
		}
		oGivePlayerMoney(playerid,0-price,1);
		if(owner > -1) oGivePlayerMoney(owner,0+price,1);
		else
		{
			new filename[MAX_STRING];
			format(filename,sizeof(filename),"%s%s.txt",PlayerDB,EncodeName(Businesses[id][Business_Owner]));
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
		Businesses[id][Business_Upgrade] = 0;
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
		if(Businesses[id][Business_Upgrade] == MAX_UPGRADELEVEL) oGivePlayerMoney(playerid,-35000,1);
		else if(Businesses[id][Business_Upgrade] > 20) oGivePlayerMoney(playerid,-28000,1);
		else if(Businesses[id][Business_Upgrade] > 10) oGivePlayerMoney(playerid,-22000,1);
		else if(Businesses[id][Business_Upgrade] < 10) oGivePlayerMoney(playerid,-15000,1);
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
	#pragma unused oldkeys
	if(newkeys == KEY_USING)
	{
		for(new id=0;id<MAX_BUSINESSES;id++)
		{
			if(IsPlayerInRangeOfPoint(playerid,10,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]))
			{
				new temp[MAX_STRING];
				format(temp,sizeof(temp),lang_texts[3][1],Businesses[id][Business_Name]);
				BisMenuInit(playerid);
				SendClientMessage(playerid,COLOUR_GREEN,temp);
			}
		}
	}
	return 1;
}

