//
// Created:		09.11.09
// Aurthor:	ElBI
//


#define GetPlayerClickedPlayerid(%0) GetPVarInt(%0,"ClickedPlayerid")
#define SetPlayerClickedPlayerid(%0,%1) SetPVarInt(%0,"ClickedPlayerid",%1)

new ListResponseType[MAX_PLAYERS];

stock click_OnPlayerClickPlayer(playerid, clickedplayerid)
{
	if(playerid == clickedplayerid) return show_menu(playerid);
	SetPlayerClickedPlayerid(playerid,clickedplayerid);
	new listitems[256]; // не менять!
	listitems = "Отправить денег\nЛичное сообщение\nЖалоба";
	if(IsPlayerMod(playerid)) strcat(listitems, "\nКикнуть\nЗаткнуть\nРазоткнуть\nВ тюрьму\nИз тюрьмы", sizeof(listitems));
	if(IsPlayerAdm(playerid)) strcat(listitems, "\nИнфо\nУбить\nТелепорт к\nТелепортировать к себе\nЗадать здоровье\nЗадать броню\nДать опыт\nДать денег\nЗаморозить\nРазморозить\nNetStats", sizeof(listitems));
	ShowPlayerDialog(playerid,click_DialogID,DIALOG_STYLE_LIST,"Выберите действие",listitems," Выбрать"," Отмена");
	return 1;
}

forward PlayerNetStatsShow(playerid,receiverid);
public PlayerNetStatsShow(playerid,receiverid)
{
	new netstats_str[2048+1];
	GetPlayerNetworkStats(receiverid,netstats_str, 2048);
	ShowPlayerDialog(playerid,click_Resp_DialogID,DIALOG_STYLE_MSGBOX,"Player NetStats",netstats_str,"ОК","");
}

stock click_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	if(dialogid == click_DialogID)
	{
		if(!response) return 1;
		ListResponseType[playerid] = listitem;
		new GuiHeader[32],GuiText[32];
		if(listitem == 0)
		{
			GuiHeader = " Отправить деньги";
			GuiText =  " Введите сумму $\n\n";
		}
		if(listitem == 1)
		{
			GuiHeader = " Личное сообщение";
			GuiText =  " Введите сообщение\n\n";
		}
		if(listitem == 2)
		{
			GuiHeader = " Жалоба на игрока";
			GuiText =  " Введите текст\n\n";
		}
		if(listitem == 3)
		{
			GuiHeader = " Кикнуть игрока";
			GuiText =  " Введите причину\n\n";
		}
		if(listitem == 4)
		{
			GuiHeader = " Заткнуть игрока";
			GuiText =  " Введите время затыкания\n\n";
		}
		if(listitem == 5)
		{
			new unmuteid = GetPlayerClickedPlayerid(playerid);
			if(IsPlayerConnected(unmuteid) == 0)
			{
				SendClientMessage(playerid,COLOUR_RED,lang_texts[12][26]);
				return 1;
			}
			SetPlayerMuteTime(unmuteid,0);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][27],oGetPlayerName(playerid));
			SendClientMessage(unmuteid,COLOUR_GREEN,string);
			SetPlayerWantedLevel(unmuteid, 0);
			format(string,sizeof(string),lang_texts[12][51],oGetPlayerName(unmuteid));
			SendClientMessageToAll(COLOUR_WHITE, string);
			printf("SERVER : %s Has Been UNMuted by %s",oGetPlayerName(unmuteid),oGetPlayerName(playerid));
			return 1;
		}
		if(listitem == 6)
		{
			GuiHeader = " В тюрьму игрока";
			GuiText =  " Введите время заключения\n\n";
		}
		if(listitem == 7)
		{
			new unjailid = GetPlayerClickedPlayerid(playerid);
			if((IsPlayerRconAdmin(unjailid) && !IsPlayerRconAdmin(playerid)) || (IsPlayerAdm(unjailid) && IsPlayerMod(playerid)))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][19]);
			}
			if (!IsPlayerConnected(unjailid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][20]);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][21],oGetPlayerName(playerid));
			SendClientMessage(unjailid,COLOUR_GREEN,string);
			UnJailPlayer(unjailid);
			return 1;
		}
		if(listitem == 8)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][34],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_YELLOW,string);
			switch( GetPlayerStatus(receiverid) )
			{
				case 1: set(string,lang_texts[12][87]);
				case 2: set(string,lang_texts[12][86]);
				case 3: set(string,lang_texts[12][94]);
				default: set(string,lang_texts[12][88]);
			}
			SendClientMessage(playerid,COLOUR_LIGHTRED,string);
			format(string,sizeof(string),lang_texts[12][35],GetPlayerLevel(receiverid),GetPlayerXP(receiverid));	//Level, XP
			SendClientMessage(playerid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][36],oGetPlayerMoney(receiverid),GetPlayerBankMoney(receiverid));	//Money, Bank
			SendClientMessage(playerid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][38],GetPlayerDeaths(receiverid),GetPlayerKills(receiverid));	//Kills, Deaths
			SendClientMessage(playerid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][71],GetPlayerJailed(receiverid),GetPlayerMuted(receiverid));	//Jailed, Muted
			SendClientMessage(playerid,COLOUR_WHITE,string);
			new playerIP[16];
			GetPVarString(receiverid,"IP",playerIP,sizeof(playerIP));
			format(string,sizeof(string),lang_texts[12][72],GetPlayerPing(receiverid),playerIP);
			SendClientMessage(playerid,COLOUR_WHITE,string);
			return 1;
		}
		if(listitem == 9)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if(IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			}
			if(IsPlayerConnected(receiverid) == 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			}
			oSetPlayerHealth(receiverid,0);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][62],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			format(string,sizeof(string),lang_texts[12][63],oGetPlayerName(playerid));
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
			return 1;
		}
		if(listitem == 10)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if(!IsPlayerConnected(receiverid))
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			new Float:X,Float:Y,Float:Z,Float:Angle;
			if(IsPlayerInAnyVehicle(playerid))
			{
				new VehicleID = GetPlayerVehicleID(playerid);
				GetPlayerPos(receiverid,X,Y,Z);
				GetPlayerFacingAngle(receiverid,Angle);
				SetVehiclePos(VehicleID,X+3.01,Y+3.01,Z+1);
				SetPlayerFacingAngle(playerid,Angle);
			}
			else
			{
				GetPlayerPos(receiverid,X,Y,Z);
				GetPlayerFacingAngle(receiverid,Angle);
				SetPlayerFacingAngle(playerid,Angle);
				X=X+(random(2)-random(4));Y=Y+(random(2)-random(4));
				oSetPlayerPos(playerid, X, Y, Z + 1);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][66],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			return 1;
		}
		if(listitem == 11)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if(IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			}
			if((IsPlayerConnected(receiverid) == 0) && !IsPlayerInAnyDM(receiverid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			}
			new Float:X,Float:Y,Float:Z,Float:Angle;
			if(IsPlayerInAnyVehicle(receiverid))
			{
				GetPlayerPos(playerid,X,Y,Z);
				new VehicleID = GetPlayerVehicleID(receiverid);
				SetVehiclePos(VehicleID,X+3.01,Y+3.01,Z+1);
				GetPlayerFacingAngle(playerid,Angle);
				SetPlayerFacingAngle(receiverid,Angle);
			}
			else
			{
				GetPlayerPos(playerid,X,Y,Z);
				GetPlayerFacingAngle(playerid,Angle);
				SetPlayerFacingAngle(receiverid,Angle);
				X=X+(random(2)-random(4));
				Y=Y+(random(2)-random(4));
				oSetPlayerPos(receiverid,X,Y,Z+1);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][66],oGetPlayerName(playerid),playerid);
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
			format(string,sizeof(string),lang_texts[12][67],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			return 1;
		}
		if(listitem == 12)
		{
			GuiHeader = " Задать здоровье";
			GuiText =  " Введите значение\n\n";
		}
		if(listitem == 13)
		{
			GuiHeader = " Задать броню";
			GuiText =  " Введите значение\n\n";
		}
		if(listitem == 14)
		{
			GuiHeader = " Дать опыт";
			GuiText =  " Введите значение\n\n";
		}
		if(listitem == 15)
		{
			GuiHeader = " Дать денег";
			GuiText =  " Введите значение$$\n\n";
		}
		if(listitem == 16)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if(IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
			}
			TogglePlayerControllable(receiverid,0);
			new string[MAX_STRING];
			format(string,sizeof(string), lang_texts[12][54] ,oGetPlayerName(playerid));
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
			format(string,sizeof(string), lang_texts[12][55] ,oGetPlayerName(receiverid));
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			return 1;
		}
		if(listitem == 17)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
			}
			TogglePlayerControllable(receiverid,1);
			new string[MAX_STRING];
			format(string,sizeof(string), lang_texts[12][56] ,oGetPlayerName(playerid));
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
			format(string,sizeof(string), lang_texts[12][57] ,oGetPlayerName(receiverid));
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			return 1;
		}
		if(listitem == 18) // NetStats
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			PlayerNetStatsShow(playerid,receiverid);
			SetPVarInt(playerid,
				"NetStats_TimerID",
				SetTimerEx("PlayerNetStatsShow",3000,1,"dd",playerid,receiverid)
			);
			return 1;
		}
		ShowPlayerDialog(playerid,click_Resp_DialogID,DIALOG_STYLE_INPUT,GuiHeader,GuiText,"Отправить","Отмена");
	}
	if(dialogid == click_Resp_DialogID)
	{
		// для однокнопочных
		switch(ListResponseType[playerid])
		{
			case 18:
			{
				new timer = GetPVarInt(playerid,"NetStats_TimerID");
				if(timer != 0) KillTimer(timer);
				return 1;
			}
		}
		//
		if(!response) return 1;
		if(strlen(inputtext) == 0) return 1;
		if(strlen(inputtext) > MAX_SEND_SYMBOLS)
		{
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][92],MAX_SEND_SYMBOLS);
			return SendClientMessage(playerid,COLOUR_MISC,string);
		}
		if(ListResponseType[playerid] == 0)
		{
			new giveplayer[MAX_PLAYER_NAME],sendername[MAX_PLAYER_NAME],string[MAX_STRING];
			new moneys = strval(inputtext);
			GetPlayerName(GetPlayerClickedPlayerid(playerid),giveplayer,sizeof(giveplayer));
			GetPlayerName(playerid,sendername,sizeof(sendername));
			if(oGetPlayerMoney(playerid) < strval(inputtext) || !IsNumeric(inputtext) || strval(inputtext) < 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[15][93]);
			}
			oGivePlayerMoney(playerid,-moneys,1);
			oGivePlayerMoney(GetPlayerClickedPlayerid(playerid),moneys,1);
			format(string,sizeof(string),lang_texts[15][79],giveplayer,GetPlayerClickedPlayerid(playerid), moneys);
			SendClientMessage(playerid,COLOUR_MONEY_GOOD, string);
			format(string,sizeof(string),lang_texts[15][80],moneys, sendername, playerid);
			SendClientMessage(GetPlayerClickedPlayerid(playerid),COLOUR_MONEY_GOOD,string);
		}
		if(ListResponseType[playerid] == 1) // pm
		{
			return SendPM(playerid,GetPlayerClickedPlayerid(playerid),inputtext);
		}
		if(ListResponseType[playerid] == 2) // report
		{
			new sendername[MAX_PLAYER_NAME],abusename[MAX_PLAYER_NAME],string[MAX_STRING];
			GetPlayerName(GetPlayerClickedPlayerid(playerid),abusename,sizeof(abusename));
			GetPlayerName(playerid,sendername,sizeof(sendername));
			format(string,sizeof(string),lang_texts[15][91],abusename,GetPlayerClickedPlayerid(playerid),inputtext);
			SendClientMessage(playerid,COLOUR_RED,string);
			format(string,sizeof(string),lang_texts[15][92],sendername,playerid,abusename,GetPlayerClickedPlayerid(playerid),inputtext);
			foreach(Player,id)
			{
				if(IsPlayerMod(id))
					SendClientMessage(id,COLOUR_RED,string);
			}
		}
		if(ListResponseType[playerid] == 3)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if(IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			}
			if(!IsPlayerConnected(receiverid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][60],oGetPlayerName(receiverid),receiverid,inputtext);
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			format(string,sizeof(string),lang_texts[12][61],oGetPlayerName(receiverid),receiverid,oGetPlayerName(playerid),inputtext);
			SendClientMessageToAll(COLOUR_WHITE,string);
			KickPlayer(receiverid,"");
			print(string);
		}
		if(ListResponseType[playerid] == 4)
		{
			new muteid = GetPlayerClickedPlayerid(playerid);
			new mutetime = strval(inputtext);
			if(mutetime < 1)
			{
				return SendClientMessage(playerid,COLOUR_RED,"Не правильное время затычки!");
			}
			if(IsPlayerConnected(muteid) == 1)
			{
				new string[MAX_STRING];
				format(string, sizeof(string)," %s заткнул %s на %d минут",oGetPlayerName(playerid),oGetPlayerName(muteid),mutetime);
				SendClientMessageToAll(COLOUR_YELLOW,string);
			}
			SetPlayerMuteTime(muteid,mutetime);
			SetPlayerMuted(muteid,GetPlayerMuted(muteid) + 1);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][24],oGetPlayerName(playerid));
			SendClientMessage(muteid,COLOUR_RED,string);
			format(string,sizeof(string),lang_texts[12][50],oGetPlayerName(muteid));
			SendClientMessageToAll(COLOUR_WHITE,string);
		}
		if(ListResponseType[playerid] == 6)
		{
			new jailid = GetPlayerClickedPlayerid(playerid);
			if(!IsPlayerConnected(jailid) || (IsPlayerRconAdmin(jailid) && !IsPlayerRconAdmin(playerid)))
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			
			new jailt = strval(inputtext);
			
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][18],oGetPlayerName(playerid));
			SendClientMessage(jailid,COLOUR_RED,string);
			
			new string2[40];
			if(1 <= jailt <= 4320)
				format(string2,sizeof(string2),"на %d минут",jailt);
			else
				set(string2,"навсегда");
			format(string,sizeof(string),"SERVER: %s был арестован %s",oGetPlayerName(jailid),string2);
			SendClientMessageToAll(COLOUR_WHITE,string);
			GameTextForPlayer(playerid,"~w~ You has been ~r~Jailed~w~!",5000,4);
			JailPlayer(jailid,jailt);
		}
		if(ListResponseType[playerid] == 12)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			}
			new hltamount = strval(inputtext);
			if ((hltamount < 10) || (hltamount > 300))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][44]);
			}
			oSetPlayerHealth(receiverid, hltamount);
		}
		if(ListResponseType[playerid] == 13)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			}
			if (!IsPlayerConnected(receiverid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			}
			new armamount = strval(inputtext);
			if ((armamount < 0) || (armamount > 300))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][44]);
			}
			oSetPlayerArmour(receiverid, armamount);
		}
		if(ListResponseType[playerid] == 14)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			}
			new xpamount = strval(inputtext);
			if ((xpamount == 0) || (xpamount > MAX_XP))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][4]);
			}
			new xpamountkeep = GetPlayerXP(receiverid)+xpamount;
			if(xpamountkeep > MAX_XP)
			{
				SendClientMessage(playerid,COLOUR_RED,lang_texts[12][40]);
				xpamount = xpamountkeep - MAX_XP;
			}
			SetPlayerXP(receiverid,GetPlayerXP(receiverid)+xpamount);
			CheckPlayerLevel(receiverid);
			new string[MAX_STRING];
			if (xpamount > 0)
			{
				format(string,sizeof(string), lang_texts[12][5] ,oGetPlayerName(playerid),xpamount);
				SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
				format(string,sizeof(string), lang_texts[12][6] ,oGetPlayerName(receiverid),xpamount,GetPlayerXP(receiverid));
				SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			}
			else
			{
				format(string,sizeof(string), lang_texts[12][7] ,oGetPlayerName(playerid),xpamount);
				SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
				format(string,sizeof(string), lang_texts[12][8] ,oGetPlayerName(receiverid),xpamount,GetPlayerXP(receiverid));
				SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			}
		}
		if(ListResponseType[playerid] == 15)
		{
			new receiverid = GetPlayerClickedPlayerid(playerid);
			if (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
			}
			new cashamount = strval(inputtext);
			if ((cashamount < -999999) || (cashamount == 0) || (cashamount > 999999))
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][11]);
			}
			oGivePlayerMoney(receiverid,cashamount,1);
			new string[MAX_STRING];
			if (cashamount > 0)
			{
				format(string,sizeof(string), lang_texts[12][12] ,oGetPlayerName(playerid),cashamount);
				SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
				format(string,sizeof(string), lang_texts[12][13] ,oGetPlayerName(receiverid),cashamount,oGetPlayerMoney(receiverid));
				SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			}
			else
			{
				format(string,sizeof(string), lang_texts[12][14] ,oGetPlayerName(playerid),cashamount);
				SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
				format(string,sizeof(string), lang_texts[12][15] ,oGetPlayerName(receiverid),cashamount,oGetPlayerMoney(receiverid));
				SendClientMessage(playerid,COLOUR_XP_GOOD,string);
			}
		}

	}
	return 1;
}
