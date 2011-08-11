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

new Text3D:Business3DTextLabel[sizeof(Businesses)],Float:BusinessDistanceOfShowLabel=20.0;

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
		ini_Get(file_business,"Name",Businesses[i][Business_Name],MAX_STRING);
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
	GameMSG("SERVER: Business module init");
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
	new file_busin;
	for(new id=0;id<sizeof(Businesses);id++)
	{
		if(strcmp(Businesses[id][Business_Owner], "Unknown", true))
		{
			if(Businesses[id][Business_Vault] < Businesses[id][Business_Value] * Businesses[id][Business_Upgrade] * 50)
			{
				Businesses[id][Business_Vault] += (Businesses[id][Business_Value] * Businesses[id][Business_Upgrade]) / 1000;
				new filename_busin[MAX_STRING];
				format(filename_busin,sizeof(filename_busin),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[id][Business_Name]);
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
	
	
	new temp[MAX_STRING],tmp[64],string[MAX_STRING];
	format(temp,sizeof(temp),lang_texts[3][3],Businesses[id][Business_Name]);
	
	if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(playerid), true))
	{
		// если мой
		set(string,"Это ваш бизнес");
		
		format(tmp,sizeof(tmp),lang_texts[3][11],((Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]) * 85) / 100);
		strcat(string,"\n",sizeof(string));
		strcat(string,tmp,sizeof(string));
		
		if(Businesses[id][Business_Vault] > 0)
		{
			format(tmp,sizeof(tmp),lang_texts[3][9],Businesses[id][Business_Vault]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
		}
		format(tmp,sizeof(tmp),lang_texts[3][57],Businesses[id][Business_Upgrade]);
		strcat(string,"\n",sizeof(string));
		strcat(string,tmp,sizeof(string));
			
		ShowPlayerDialog(playerid,bis_Info_DialogID,DIALOG_STYLE_MSGBOX,
			temp,
			string,
			"Действия","Отмена"
		);
	}
	else
	{
		if(!strcmp(Businesses[id][Business_Owner],"Unknown", true))
		{
			// если ничей
			strcat(string,lang_texts[3][13],sizeof(string));
			
			format(tmp,sizeof(tmp), lang_texts[3][14] ,Businesses[id][Business_Cost]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
			
			format(tmp,sizeof(tmp), lang_texts[3][15] ,Businesses[id][Business_Level]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
			
			format(tmp,sizeof(tmp), lang_texts[3][16] ,Businesses[id][Business_Value]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
			
			format(tmp,sizeof(tmp), lang_texts[3][57] ,Businesses[id][Business_Upgrade]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));

			ShowPlayerDialog(playerid,bis_Info_DialogID,DIALOG_STYLE_MSGBOX,
				temp,
				string,
				"Действия","Отмена"
			);
		}
		else
		{
			// если кого-то
			
			format(tmp,sizeof(tmp),lang_texts[3][4],Businesses[id][Business_Owner]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
		
			format(tmp,sizeof(tmp),lang_texts[3][5],Businesses[id][Business_Cost] + Businesses[id][Business_Buyout]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));

			format(tmp,sizeof(tmp),lang_texts[3][6],Businesses[id][Business_Level]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
			
			format(tmp,sizeof(tmp),lang_texts[3][7],Businesses[id][Business_Value]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
			
			format(tmp,sizeof(tmp),lang_texts[3][57],Businesses[id][Business_Upgrade]);
			strcat(string,"\n",sizeof(string));
			strcat(string,tmp,sizeof(string));
			
			ShowPlayerDialog(playerid,bis_Info_DialogID,DIALOG_STYLE_MSGBOX,
				temp,
				string,
				"Действия","Отмена"
			);
		}
	}
	return 1;
}

stock bis_Buy(playerid)
{
	new id = GetBusinessID(playerid),temp[MAX_STRING];
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][27]);
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[id][Business_Name]);
	if(GetPlayerLevel(playerid) < Businesses[id][Business_Level])
	{
		format(temp,sizeof(temp), lang_texts[3][25],Businesses[id][Business_Level]);
		bis_ShowDialog_Msg(playerid,id,temp);
		return 1;
	}
	new price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
	if(oGetPlayerMoney(playerid) < price)
		return bis_ShowDialog_Msg(playerid,id,lang_texts[3][24]);
	
	if(!strcmp(Businesses[id][Business_Owner],oGetPlayerName(playerid),true))
	{
		bis_ShowDialog_Msg(playerid,id,lang_texts[3][20]);
	}
	else
	{
		new owner=-1;
		new string[MAX_STRING];
		for(new ownerid=0;ownerid<=GetPlayerLastID();ownerid++)
		{
			if(!IsPlayerConnected(ownerid) || IsPlayerNPC(ownerid) || strcmp(Businesses[id][Business_Owner],oGetPlayerName(ownerid),true))
				continue;
			format(temp,sizeof(temp),lang_texts[3][21],Businesses[id][Business_Name],oGetPlayerName(playerid));
			strcat(string,temp,sizeof(string));
			strcat(string,"\n",sizeof(string));
			owner=ownerid;
			break;
		}
		oGivePlayerMoney(playerid,0-price,1);
		if(owner > -1)
		{
			oGivePlayerMoney(owner,price,1);
		}
		else
		{
			new filename[MAX_STRING];
			format(filename,sizeof(filename),"%s%s"GTO_FILES_FORMAT,PlayerDB,Businesses[id][Business_Owner]);
			if(ini_Exist(filename))
			{
				new tempmoney,file_busin = ini_Open(filename);
				ini_GetInt(file_busin,"Money",tempmoney);
				ini_SetInt(file_busin,"Money",tempmoney+price);
				ini_Close(file_busin);
			}
		}
		set(Businesses[id][Business_Owner],oGetPlayerName(playerid));
		Businesses[id][Business_Buyout] = 0;
		new filename[MAX_STRING];
		format(filename,sizeof(filename),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[id][Business_Name]);
		if(ini_Exist(filename))
		{
			new file_busin = ini_Open(filename);
			ini_Set(file_busin,"Owner",Businesses[id][Business_Owner]);
			ini_SetInt(file_busin,"Buyout",Businesses[id][Business_Buyout]);
			ini_Close(file_busin);
		}
		format(temp,sizeof(temp),lang_texts[3][22],Businesses[id][Business_Name]);
		strcat(string,temp,sizeof(string));

		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (business)",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name]);
		WriteLog(GameLog,logstring);

		if(Businesses[id][Business_Vault] > 0)
		{
			format(temp,sizeof(temp),lang_texts[3][23] ,Businesses[id][Business_Vault]);
			strcat(string,"\n",sizeof(string));
			strcat(string,temp,sizeof(string));
		}
		bis_ShowDialog_Msg(playerid,id,string);
	}
	return 1;
}

stock bis_Sell(playerid)
{
	new id = GetBusinessID(playerid),temp[MAX_STRING];
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][36]);
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[id][Business_Name]);
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
		new filename[MAX_STRING];
		format(filename,sizeof(filename),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[id][Business_Name]);
		if(ini_Exist(filename))
		{
			new file_busin = ini_Open(filename);
			ini_Set(file_busin,"Owner","Unknown");
			ini_SetInt(file_busin,"Buyout",Businesses[id][Business_Buyout]);
			ini_SetInt(file_busin,"Upgrade",Businesses[id][Business_Upgrade]);
			ini_Close(file_busin);
		}
		new string[MAX_STRING];
		format(temp,sizeof(temp), lang_texts[3][30] ,Businesses[id][Business_Name]);
		strcat(string,temp,sizeof(string));
		strcat(string,"\n",sizeof(string));
		strcat(string,lang_texts[3][31],sizeof(string));
		if(Businesses[id][Business_Vault] > 0)
		{
			format(temp,sizeof(temp),lang_texts[3][32],Businesses[id][Business_Vault]);
			strcat(string,temp,sizeof(string));
			strcat(string,"\n",sizeof(string));
			strcat(string,lang_texts[3][33],sizeof(string));
		}
		bis_ShowDialog_Msg(playerid,id,string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (business)",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name]);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock bis_Collect(playerid)
{
	new id = GetBusinessID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[3][52]);
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[id][Business_Name]);
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
			new filename[MAX_STRING];
			format(filename,sizeof(filename),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[id][Business_Name]);
			if(ini_Exist(filename))
			{
				new file_busin = ini_Open(filename);
				ini_SetInt(file_busin,"Vault",Businesses[id][Business_Vault]);
				ini_Close(file_busin);
			}
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
			strcat(string,"\n",sizeof(string));
		}
		else
		{
			format(temp,sizeof(temp),lang_texts[3][54],Businesses[id][Business_Name]);
			strcat(string,temp,sizeof(string));
			strcat(string,"\n",sizeof(string));
		}
	}
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[GetBusinessID(playerid)][Business_Name]);
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
			strcat(string,"\n",sizeof(string));
		}
	}
	if(count < 1) strcat(string,lang_texts[3][56],sizeof(string));
	new head[MAX_STRING];
	format(head,sizeof(head),lang_texts[3][3],Businesses[GetBusinessID(playerid)][Business_Name]);
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
		new str[MAX_STRING];
		new price = (Businesses[id][Business_Upgrade]+1) * Businesses[id][Business_Value] * UPGRADE_TARIF;
		
		if(oGetPlayerMoney(playerid) < price)
		{
			format(str,sizeof(str),lang_texts[3][61],price);
			return SendClientMessage(playerid,COLOUR_RED,str);
		}
		
		oGivePlayerMoney(playerid,-price,0);
		new filename[MAX_STRING];
		format(filename,sizeof(filename),"%s%s"GTO_FILES_FORMAT,BusinessDB,Businesses[id][Business_Name]);
		Businesses[id][Business_Upgrade]++;
		if(ini_Exist(filename))
		{
		    new file_busin = ini_Open(filename);
			ini_SetInt(file_busin,"Upgrade",Businesses[id][Business_Upgrade]);
			ini_Close(file_busin);
		}
		
		format(str,sizeof(str),lang_texts[3][58],Businesses[id][Business_Name],Businesses[id][Business_Upgrade]);
		SendClientMessage(playerid,COLOUR_GREEN,str);
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