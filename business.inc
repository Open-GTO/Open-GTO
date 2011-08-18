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

new Text3D:Business3DTextLabel[ sizeof(Businesses) ],Float:BusinessDistanceOfShowLabel=20.0;

BusinessLoadAll()
{
	if(ini_Exist(ConfigDB))
	{
		new file_business_cfg = ini_Open(ConfigDB);
		ini_Get(file_business_cfg,"Business_DB",BusinessDB);
		ini_GetFloat(file_business_cfg,"Business_DistanceOfShowLabel",BusinessDistanceOfShowLabel);
		ini_Close(file_business_cfg);
	}
	for(new i=0,file_business,businessdbname[MAX_STRING];i<sizeof(Businesses);i++)
	{
		format(businessdbname,sizeof(businessdbname),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[i][Business_Name]);
		if(!ini_Exist(businessdbname)) continue;
		file_business = ini_Open(businessdbname);
		ini_Get(file_business,"Name",Businesses[i][Business_Name],MAX_NAME);
        ini_GetInt(file_business,"Cost",Businesses[i][Business_Cost]);
        ini_GetInt(file_business,"Value",Businesses[i][Business_Value]);
		ini_GetInt(file_business,"Level",Businesses[i][Business_Level]);
        ini_Get(file_business,"Owner",Businesses[i][Business_Owner],MAX_NAME);
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
	if(ini_Exist(ConfigDB))
	{
		new file_business_cfg = ini_Open(ConfigDB);
		ini_Set(file_business_cfg,"Business_DB",BusinessDB);
		ini_SetFloat(file_business_cfg,"Business_DistanceOfShowLabel",BusinessDistanceOfShowLabel);
		ini_Close(file_business_cfg);
	}
	for(new i=0,file_business,businessdbname[MAX_STRING];i<sizeof(Businesses);i++)
	{
		format(businessdbname,sizeof(businessdbname),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[i][Business_Name]);
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
	for(new id=0;id<sizeof(Businesses);id++)
	{
		CreatePickup(1274,49,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]+0.5);	//синий домик
		if(Businesses[id][Business_ShowIcon] == 1)
			CreateStreamMapIcon(52,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]);
		format(string,sizeof(string),"Бизнес: %s\nСтоимость: %d\nПрибыль: %d\nСейчас принадлежит: %s\nМин.Уровень: %d\nУровень бизнеса: %d",Businesses[id][Business_Name],Businesses[id][Business_Cost],Businesses[id][Business_Value],Businesses[id][Business_Owner],Businesses[id][Business_Level],Businesses[id][Business_Upgrade]);
		Business3DTextLabel[id] = Create3DTextLabel(string,COLOUR_WHITE,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]+0.75,BusinessDistanceOfShowLabel,0,1);
	}
	GameMSG(lang_texts[3][0]);
}

stock bis_ShowDialog_General(playerid)
{
	new temp[MAX_STRING];
	format(temp,sizeof(temp),lang_texts[3][3],Businesses[ GetBusinessID(playerid) ][Business_Name]);
	ShowPlayerDialog(playerid,bis_DialogID,DIALOG_STYLE_LIST,
		temp,
		"Купить\n\
		Продать\n\
		Забрать Прибыль\n\
		Увеличить Уровень\n\
		Мой бизнесс\n\
		Все бизнессы",
		"Ok","Отмена"
	);
	return 1;
}

stock IsPlayerAtBusiness(playerid)
{
	for(new id=0;id<sizeof(Businesses);id++)
		if(IsPlayerInRangeOfPoint(playerid,2,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]))
			return 1;
	return 0;
}

stock GetBusinessID(playerid)
{
	for(new id=0;id<sizeof(Businesses);id++)
		if(IsPlayerInRangeOfPoint(playerid,2,Businesses[id][Coord_X],Businesses[id][Coord_Y],Businesses[id][Coord_Z]))
			return id;
	return -1;
}

stock TurnAround()
{
	for(new id=0;id<sizeof(Businesses);id++)
	{
		if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			if(Businesses[id][Business_Vault] < Businesses[id][Business_Value] * Businesses[id][Business_Upgrade] * 50)
			{
				Businesses[id][Business_Vault] += (Businesses[id][Business_Value] * Businesses[id][Business_Upgrade]) / 1000;
			}
		}
	}
}

stock business_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch(dialogid)
	{
		case bis_DialogID:
		{
			if(!response) return 1;
			switch(listitem)
			{
				case 0:bis_Buy(playerid);
				case 1:bis_Sell(playerid);
				case 2:bis_Collect(playerid);
				case 3:{bis_buyUpgrade(playerid);bis_ShowDialog_General(playerid);}
				case 4:bis_ShowDialog_MyBises(playerid);
				case 5:bis_ShowDialog_Bises(playerid);
			}
		}
		case bis_Info_DialogID,bis_Msg_DialogID:
		{
			if(!response) return 1;
			return bis_ShowDialog_General(playerid);
		}
	}
    return 1;
}

stock business_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	bis_ShowDialog_Info(playerid);
	return 1;
}

stock bis_ShowDialog_Info(playerid)
{
	new id = GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][18]);
	
	new head[MAX_STRING],string[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[id][Business_Name]);
	
	if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(playerid), true))
	{
		// если мой
		format(string,sizeof(string),lang_texts[3][11],
			((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100,
			Businesses[id][Business_Vault],
			Businesses[id][Business_Upgrade]
		);
			
		ShowPlayerDialog(playerid,bis_Info_DialogID,DIALOG_STYLE_MSGBOX,
			head,
			string,
			"Действия","Отмена"
		);
	}
	else
	{
		if(!strcmp(Businesses[id][Business_Owner],"Unknown", true))
		{
			// если ничей
			format(string,sizeof(string),lang_texts[3][13],
				Businesses[id][Business_Cost],
				Businesses[id][Business_Level],
				Businesses[id][Business_Value],
				Businesses[id][Business_Upgrade]
			);

			ShowPlayerDialog(playerid,bis_Info_DialogID,DIALOG_STYLE_MSGBOX,
				head,
				string,
				"Действия","Отмена"
			);
		}
		else
		{
			// если кого-то
			format(string,sizeof(string),lang_texts[3][4],
				Businesses[id][Business_Owner],
				Businesses[id][Business_Cost] + Businesses[id][Business_Buyout],
				Businesses[id][Business_Level],
				Businesses[id][Business_Value],
				Businesses[id][Business_Upgrade]
			);
			
			ShowPlayerDialog(playerid,bis_Info_DialogID,DIALOG_STYLE_MSGBOX,
				head,
				string,
				"Действия","Отмена"
			);
		}
	}
	return 1;
}

stock bis_Buy(playerid)
{
	new id = GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][27]);
	if(GetPlayerLevel(playerid) < Businesses[id][Business_Level])
	{
		new temp[MAX_STRING];
		format(temp,sizeof(temp),lang_texts[3][25],Businesses[id][Business_Level]);
		bis_ShowDialog_Msg(playerid,id,temp);
		return 1;
	}
	new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
	if(oGetPlayerMoney(playerid) < price)
		return bis_ShowDialog_Msg(playerid,id,lang_texts[3][24]);
	
	if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(playerid),true))
		bis_ShowDialog_Msg(playerid,id,lang_texts[3][20]);
#if !defined BUY_ALL_BUSINESS
	else if(strcmp(Businesses[id][Business_Owner],"Unknown",true))
		SendClientMessage(playerid,COLOUR_RED,lang_texts[3][12]);
#endif
	else
	{
	#if defined BUY_ALL_BUSINESS
		for(new ownerid=0;ownerid<=GetPlayerLastID();ownerid++)
		{
			if(IsPlayerConnected(ownerid) && !IsPlayerNPC(ownerid) && !strcmp(Businesses[id][Business_Owner],oGetPlayerName(ownerid),true))
			{
				new temp[MAX_STRING];
				format(temp,sizeof(temp),lang_texts[3][21],oGetPlayerName(playerid),Businesses[id][Business_Name]);
				SendClientMessage(ownerid,COLOUR_RED,temp);
				oGivePlayerMoney(ownerid,price,1);
				break;
			}
		}
	#endif
		oGivePlayerMoney(playerid,-price,1);
		set(Businesses[id][Business_Owner],oGetPlayerName(playerid));
		Businesses[id][Business_Buyout] = 0;
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[3][22],
			Businesses[id][Business_Name],
			Businesses[id][Business_Vault]
		);
		bis_ShowDialog_Msg(playerid,id,string);
		
		GameMSG("player: %s(%d): bought the '%s' (business)",oGetPlayerName(playerid),playerid,Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Sell(playerid)
{
	new id = GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][36]);
	if(strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		bis_ShowDialog_Msg(playerid,id,lang_texts[3][29]);
	}
	else
	{
		new price = ((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100;
		oGivePlayerMoney(playerid,price,1);
		set(Businesses[id][Business_Owner],"Unknown");
		Businesses[id][Business_Buyout] = 0;
		Businesses[id][Business_Upgrade] = 1;
		
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[3][30],
			Businesses[id][Business_Name],
			Businesses[id][Business_Vault]
		);
		bis_ShowDialog_Msg(playerid,id,string);

		GameMSG("player: %s(%d): sold the '%s' (business)",oGetPlayerName(playerid),playerid,Businesses[id][Business_Name]);
	}
	return 1;
}

stock bis_Collect(playerid)
{
	new id = GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][52]);
	if(strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
	{
		bis_ShowDialog_Msg(playerid,id,lang_texts[3][47]);
	}
	else
	{
		if(Businesses[id][Business_Vault] > 0)
		{
			oGivePlayerMoney(playerid,Businesses[id][Business_Vault] * Businesses[id][Business_Upgrade],1);
			bis_ShowDialog_Msg(playerid,id,lang_texts[3][48]);
			Businesses[id][Business_Vault] = 0;
		}
		else
		{
			bis_ShowDialog_Msg(playerid,id,lang_texts[3][49]);
		}
	}
	return 1;
}

stock bis_ShowDialog_Bises(playerid)
{
	new temp[MAX_STRING],string[2048];
	for(new id=0;id<sizeof(Businesses);id++)
	{
		if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			format(temp,sizeof(temp),lang_texts[3][53],Businesses[id][Business_Name],Businesses[id][Business_Owner]);
			strcat(string,temp,sizeof(string));
		}
		else
		{
			format(temp,sizeof(temp),lang_texts[3][54],Businesses[id][Business_Name]);
			strcat(string,temp,sizeof(string));
		}
	}
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[ GetBusinessID(playerid) ][Business_Name]);
	return ShowPlayerDialog(playerid,bis_Msg_DialogID,DIALOG_STYLE_LIST,
		head,
		string,
		"Назад","Отмена"
	);
}

stock bis_ShowDialog_MyBises(playerid)
{
	new temp[MAX_STRING],string[2048],count = 0;
	for(new id=0;id<sizeof(Businesses);id++)
	{
		if(!strcmp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true))
		{
			count++;
			format(temp,sizeof(temp),lang_texts[3][55],Businesses[id][Business_Name],Businesses[id][Business_Vault]);
			strcat(string,temp,sizeof(string));
		}
	}
	new id = GetBusinessID(playerid);
	if(count < 1)
	{
		return bis_ShowDialog_Msg(playerid,id,lang_texts[3][56]);
	}
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[id][Business_Name]);
	return ShowPlayerDialog(playerid,bis_Msg_DialogID,DIALOG_STYLE_LIST,
		head,
		string,
		"Назад","Отмена"
	);
}

stock bis_buyUpgrade(playerid)
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
		new string[MAX_STRING];
		new price = (Businesses[id][Business_Upgrade]+1) * Businesses[id][Business_Value] * UPGRADE_TARIF;
		
		if(oGetPlayerMoney(playerid) < price)
		{
			format(string,sizeof(string),lang_texts[3][61],price);
			return SendClientMessage(playerid,COLOUR_RED,string);
		}
		
		oGivePlayerMoney(playerid,-price,0);
		Businesses[id][Business_Upgrade]++;
		format(string,sizeof(string),lang_texts[3][58],Businesses[id][Business_Name],Businesses[id][Business_Upgrade]);
		SendClientMessage(playerid,COLOUR_GREEN,string);
	}
	return 1;
}

stock business_Update3DTextLabelText()
{
	new string[MAX_STRING];
	for(new id=0;id<sizeof(Businesses);id++)
	{
		format(string,sizeof(string),"Бизнес: %s\nСтоимость: %d\nПрибыль: %d\nСейчас принадлежит: %s\nМин.Уровень: %d\nУровень бизнеса: %d",Businesses[id][Business_Name],Businesses[id][Business_Cost],Businesses[id][Business_Value],Businesses[id][Business_Owner],Businesses[id][Business_Level],Businesses[id][Business_Upgrade]);
		Update3DTextLabelText(Business3DTextLabel[id],COLOUR_WHITE,string);
	}
	return 1;
}

stock bis_ShowDialog_Msg(playerid,bis_id,string[])
{
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[bis_id][Business_Name]);
	return ShowPlayerDialog(playerid,bis_Msg_DialogID,DIALOG_STYLE_MSGBOX,
		head,
		string,
		"Назад","Отмена"
	);
}

stock business_RenameOwner(old_name[MAX_PLAYER_NAME],new_name[MAX_PLAYER_NAME])
{
	for(new i=0;i<sizeof(Businesses);i++)
	{
		if( !strcmp(Businesses[i][Business_Owner],old_name,true) )
		{
			set(Businesses[i][Business_Owner],new_name);
			return 1;
		}
	}
	return 0;
}