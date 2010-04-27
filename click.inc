//
// Created:     09.11.09
// Aurthor:   ElBI
//

#define ClickDialog 16
#define ClickDialogResp 17
#define AdminDialogid 18

#define MAX_SEND_SYMBOLS 64

forward ClickPlayer(playerid, clickedplayerid);
forward PlayerClickResponse(playerid,dialogid,response,listitem,inputtext[]);
new ListResponseType[MAX_PLAYERS];

stock click_OnPlayerClickPlayer(playerid, clickedplayerid)
{
	Player[playerid][ClickedPlayerid] = clickedplayerid;
	new listitems[MAX_STRING];
	listitems = "Отправить денег\nЛичное сообщение\nЖалоба";
	if(IsPlayerMod(playerid) || IsPlayerAdmin(playerid)) strcat(listitems, "\nKick\nMute\nUnMute\nJail\nUnJail", sizeof(listitems));
	if(IsPlayerAdm(playerid) || IsPlayerAdmin(playerid)) strcat(listitems, "\nИнфо\nУбить\nTele to\nTele here\nSet health\nSet arm\nGive XP\nДать денег\nFreeze\nUnfreeze", sizeof(listitems));
	ShowPlayerDialog(playerid,ClickDialog,DIALOG_STYLE_LIST,"Выберите действие",listitems," Выбрать"," Отмена");
}

stock click_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	if(dialogid == ClickDialog)
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
			GuiHeader = " Kick player";
		    GuiText =  " Введите причину\n\n";
		}
		if(listitem == 4)
		{
			GuiHeader = " Mute player";
		    GuiText =  " Введите время затыкания\n\n";
		}
		if(listitem == 5)
		{
			new unmuteid = Player[playerid][ClickedPlayerid];
			if(IsPlayerConnected(unmuteid) == 0)
			{
				SendPlayerFormattedText(playerid,lang_texts[12][26],0,COLOUR_RED);
				return 1;
			}
			Player[unmuteid][MuteTime] = 0;
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][27],oGetPlayerName(playerid));
			SendPlayerFormattedText(unmuteid,string, 0,COLOUR_GREEN);
			SetPlayerWantedLevel(unmuteid, 0);
			format(string,sizeof(string),lang_texts[12][51],oGetPlayerName(unmuteid));
			SendClientMessageToAll(COLOUR_WHITE, string);
			printf("SERVER : %s Has Been UNMuted by %s",oGetPlayerName(unmuteid),oGetPlayerName(playerid));
			return 1;
		}
		if(listitem == 6)
		{
			GuiHeader = " Jail player";
		    GuiText =  " Введите время заключения\n\n";
		}
		if(listitem == 7)
		{
			new unjailid = Player[playerid][ClickedPlayerid];
			if(unjailid < 0 || unjailid >= MAX_PLAYERS || (IsPlayerAdmin(unjailid) && !IsPlayerAdmin(playerid)) || (IsPlayerAdm(unjailid) && IsPlayerMod(playerid)))
			{
				SendPlayerFormattedText(playerid,lang_texts[12][19],0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(unjailid) == 0)
			{
				SendPlayerFormattedText(playerid,lang_texts[12][20],0,COLOUR_RED);
				return 1;
			}
			Player[unjailid][Jailed] = 0;
			Player[unjailid][JailTime] = 0;
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][21],oGetPlayerName(playerid));
			SendPlayerFormattedText(unjailid,string, 0,COLOUR_GREEN);
			UnJailPlayer(unjailid);
			return 1;
		}
		if(listitem == 8)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][34],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_YELLOW,string);
			new sid = Player[receiverid][Status];
			if(sid > 0 && sid < 10) format(string,sizeof(string),lang_texts[12][87],sid);
			else if(sid > 10) format(string,sizeof(string),lang_texts[12][86],sid);
			else format(string,sizeof(string),lang_texts[12][88]);
			SendClientMessage(playerid,COLOUR_LIGHTRED,string);
			format(string,sizeof(string),lang_texts[12][35],Player[receiverid][Level],Player[receiverid][XP]);	//Level, XP
			SendClientMessage(playerid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][36],Player[receiverid][Money],Player[receiverid][Bank]);	//Money, Bank
			SendClientMessage(playerid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][38],Player[receiverid][Deaths],Player[receiverid][Kills]);	//Kills, Deaths
			SendClientMessage(playerid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][71],Player[receiverid][Jailed],Player[receiverid][Muted]);	//Jailed, Muted
			SendClientMessage(playerid,COLOUR_WHITE,string);
			new PlayerIP[MAX_STRING];
			GetPlayerIp(receiverid,PlayerIP,sizeof(PlayerIP));
			format(string,sizeof(string),lang_texts[12][72],GetPlayerPing(receiverid),PlayerIP);
			SendClientMessage(playerid,COLOUR_WHITE,string);
			return 1;
		}
		if(listitem == 9)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
			{
				SendPlayerFormattedText(playerid,lang_texts[12][2],0,COLOUR_RED);
				return 1;
			}
			if(IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid,lang_texts[12][3],0,COLOUR_RED);
				return 1;
			}
			SetPlayerHealth(receiverid,0);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][62],oGetPlayerName(receiverid),receiverid);
			SendPlayerFormattedText(playerid,string,0,COLOUR_XP_GOOD);
			format(string,sizeof(string),lang_texts[12][63],oGetPlayerName(playerid));
			SendPlayerFormattedText(receiverid,string,0,COLOUR_XP_GOOD);
			return 1;
		}
		if(listitem == 10)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if(IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][3] , 0,COLOUR_RED);
				return 1;
			}
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
				SetPlayerPos(playerid, X, Y, Z + 1);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][66],oGetPlayerName(receiverid),receiverid);
			SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			return 1;
		}
		if(listitem == 11)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
			{
				SendPlayerFormattedText(playerid,lang_texts[12][2],0,COLOUR_RED);
				return 1;
			}
			if((IsPlayerConnected(receiverid) == 0) && !IsPlayerInAnyDM(receiverid))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][3] , 0,COLOUR_RED);
				return 1;
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
				SetPlayerPos(receiverid,X,Y,Z+1);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][66],oGetPlayerName(playerid),playerid);
			SendPlayerFormattedText(receiverid,string,0,COLOUR_XP_GOOD);
			format(string,sizeof(string),lang_texts[12][67],oGetPlayerName(receiverid),receiverid);
			SendPlayerFormattedText(playerid,string,0,COLOUR_XP_GOOD);
			return 1;
		}
		if(listitem == 12)
		{
			GuiHeader = " Set Health";
		    GuiText =  " Введите значение\n\n";
		}
		if(listitem == 13)
		{
			GuiHeader = " Set Armour";
		    GuiText =  " Введите значение\n\n";
		}
		if(listitem == 14)
		{
			GuiHeader = " Give EXP";
		    GuiText =  " Введите значение\n\n";
		}
		if(listitem == 15)
		{
			GuiHeader = " Дать денег";
		    GuiText =  " Введите значение$$\n\n";
		}
		if(listitem == 16)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if ((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
			{
				SendPlayerFormattedText(playerid, lang_texts[12][9] , 0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][10] , 0,COLOUR_RED);
				return 1;
			}
			TogglePlayerControllable(receiverid,0);
			new string[MAX_STRING];
			format(string,sizeof(string), lang_texts[12][54] ,oGetPlayerName(playerid));
			SendPlayerFormattedText(receiverid,string, 0,COLOUR_XP_GOOD);
			format(string,sizeof(string), lang_texts[12][55] ,oGetPlayerName(receiverid));
			SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			return 1;
		}
		if(listitem == 17)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if ((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
			{
				SendPlayerFormattedText(playerid, lang_texts[12][9] , 0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][10] , 0,COLOUR_RED);
				return 1;
			}
			TogglePlayerControllable(receiverid,1);
			new string[MAX_STRING];
			format(string,sizeof(string), lang_texts[12][56] ,oGetPlayerName(playerid));
			SendPlayerFormattedText(receiverid,string, 0,COLOUR_XP_GOOD);
			format(string,sizeof(string), lang_texts[12][57] ,oGetPlayerName(receiverid));
			SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			return 1;
		}
		ShowPlayerDialog(playerid,ClickDialogResp,DIALOG_STYLE_INPUT,GuiHeader,GuiText,"Отправить","Отмена");
	}
	if(dialogid == ClickDialogResp)
	{
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
			GetPlayerName(Player[playerid][ClickedPlayerid],giveplayer,sizeof(giveplayer));
			GetPlayerName(playerid,sendername,sizeof(sendername));
			if(oGetPlayerMoney(playerid) < strval(inputtext) || !isNumeric(inputtext) || strval(inputtext) < 0)
			{
				return SendClientMessage(playerid,COLOUR_RED,lang_texts[15][93]);
			}
			oGivePlayerMoney(playerid,-moneys,1);
			oGivePlayerMoney(Player[playerid][ClickedPlayerid],moneys,1);
			format(string,sizeof(string),lang_texts[15][79],giveplayer,Player[playerid][ClickedPlayerid], moneys);
			SendClientMessage(playerid,COLOUR_MONEY_GOOD, string);
			format(string,sizeof(string),lang_texts[15][80],moneys, sendername, playerid);
			SendClientMessage(Player[playerid][ClickedPlayerid],COLOUR_MONEY_GOOD,string);
		}
		if(ListResponseType[playerid] == 1)
		{
			new sendername[MAX_PLAYER_NAME],receivename[MAX_PLAYER_NAME],string[MAX_STRING];
			GetPlayerName(Player[playerid][ClickedPlayerid],receivename,sizeof(receivename));
			GetPlayerName(playerid,sendername,sizeof(sendername));
			format(string,sizeof(string),lang_texts[9][31],receivename,Player[playerid][ClickedPlayerid],inputtext);
			SendClientMessage(playerid,0xFFCC22FF,string);
			format(string,sizeof(string),lang_texts[9][32],sendername,playerid,inputtext);
			SendClientMessage(Player[playerid][ClickedPlayerid],0xFFFF22FF,string);
		}
		if(ListResponseType[playerid] == 2)
		{
			new sendername[MAX_PLAYER_NAME],abusename[MAX_PLAYER_NAME],string[MAX_STRING];
			GetPlayerName(Player[playerid][ClickedPlayerid],abusename,sizeof(abusename));
			GetPlayerName(playerid,sendername,sizeof(sendername));
			format(string,sizeof(string),lang_texts[15][91],abusename,Player[playerid][ClickedPlayerid],inputtext);
			SendClientMessage(playerid,COLOUR_RED,string);
			format(string,sizeof(string),lang_texts[15][92],sendername,playerid,abusename,Player[playerid][ClickedPlayerid],inputtext);
    		for(new id = 0; id < MAX_PLAYERS; id++)
			{
			if(IsPlayerMod(id) || IsPlayerAdm(id) || IsPlayerAdmin(id)) SendClientMessage(id,COLOUR_RED,string);
			}
		}
		if(ListResponseType[playerid] == 3)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
			{
				SendPlayerFormattedText(playerid,lang_texts[12][2],0,COLOUR_RED);
				return 1;
			}
			if(!IsPlayerConnected(receiverid))
			{
				SendPlayerFormattedText(playerid,lang_texts[12][3],0,COLOUR_RED);
				return 1;
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][60],oGetPlayerName(receiverid),receiverid,inputtext);
			SendPlayerFormattedText(playerid,string,0,COLOUR_XP_GOOD);
			format(string,sizeof(string),lang_texts[12][61],oGetPlayerName(receiverid),receiverid,oGetPlayerName(playerid),inputtext);
			SendClientMessageToAll(COLOUR_WHITE,string);
			Kick(receiverid);
			print(string);
		}
		if(ListResponseType[playerid] == 4)
		{
			new muteid = Player[playerid][ClickedPlayerid];
			if((muteid < 0) || (muteid >= MAX_PLAYERS))
			{
				SendPlayerFormattedText(playerid,lang_texts[12][2],0,COLOUR_RED);
				return 1;
			}
			new mutetime = strval(inputtext);
			if(mutetime < 1)
			{
				SendPlayerFormattedText(playerid," Не правильное время затычки!", 0,COLOUR_RED);
				return 1;
			}
			if(IsPlayerConnected(muteid) == 1)
			{
				new string[MAX_STRING];
				format(string, sizeof(string)," %s заткнул %s на %d минут",oGetPlayerName(playerid),oGetPlayerName(muteid),mutetime);
				SendClientMessageToAll(COLOUR_YELLOW,string);
			}
			Player[muteid][MuteTime] = mutetime;
			Player[muteid][Muted]++;
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][24],oGetPlayerName(playerid));
			SendPlayerFormattedText(muteid,string,0,COLOUR_RED);
			format(string,sizeof(string),lang_texts[12][50],oGetPlayerName(muteid));
			SendClientMessageToAll(COLOUR_WHITE,string);
		}
		if(ListResponseType[playerid] == 6)
		{
			new jailid = Player[playerid][ClickedPlayerid];
			if((jailid < 0) || (jailid >= MAX_PLAYERS) || (IsPlayerAdmin(jailid) && !IsPlayerAdmin(playerid)) || (IsPlayerAdm(jailid) && IsPlayerMod(playerid)) )
			{
				SendPlayerFormattedText(playerid,lang_texts[12][2],0,COLOUR_RED);
				return 1;
			}
			if(IsPlayerConnected(jailid) == 0)
			{
				SendPlayerFormattedText(playerid,lang_texts[12][3],0,COLOUR_RED);
				return 1;
			}
			static jailt=4320;
			jailt = strval(inputtext);
			if(jailt>1 && jailt<=4320) Player[jailid][JailTime] = jailt;
			Player[jailid][Jailed] = 1;
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][18],oGetPlayerName(playerid));
			SendPlayerFormattedText(jailid,string, 0,COLOUR_RED);
			format(string,sizeof(string),"~r~%s ~w~has been Jailed",oGetPlayerName(jailid));
			GameTextForAll(string,5000,4);
			new string2[40];
			if(jailt > 1 && jailt<=4320) format(string2,sizeof(string2),"%d мин",jailt);
			else format(string2,sizeof(string2),"пожизненно");
			format(string,sizeof(string),"SERVER: %s был Арестован на %s минут",oGetPlayerName(jailid),string2);
			print(string);
			SendClientMessageToAll(COLOUR_WHITE,string);
			JailPlayer(jailid);
		}
		if(ListResponseType[playerid] == 12)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if ((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][2] , 0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][3] , 0,COLOUR_RED);
				return 1;
			}
			new hltamount = strval(inputtext);
			if ((hltamount < 10) || (hltamount > 300))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][44] , 0,COLOUR_RED);
				return 1;
			}
			SetPlayerHealth(receiverid, hltamount);
		}
		if(ListResponseType[playerid] == 13)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if ((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][2] , 0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][3] , 0,COLOUR_RED);
				return 1;
			}
			new armamount = strval(inputtext);
			if ((armamount < 0) || (armamount > 300))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][44] , 0,COLOUR_RED);
				return 1;
			}
			SetPlayerArmour(receiverid, armamount);
		}
		if(ListResponseType[playerid] == 14)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if ((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
			{
				SendPlayerFormattedText(playerid, lang_texts[12][2] , 0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][3] , 0,COLOUR_RED);
				return 1;
			}
			new xpamount = strval(inputtext);
			if ((xpamount == 0) || (xpamount > MAX_LVLXP))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][4] , 0,COLOUR_RED);
				return 1;
			}
			new xpamountkeep = GetPlayerXP(receiverid)+xpamount;
			if ((!IsPlayerAdmin(receiverid) && xpamountkeep >MAX_LVLXP) || (IsPlayerAdmin(receiverid) && xpamountkeep > MAX_XP))
			{
    			SendPlayerFormattedText(playerid, lang_texts[12][40] , 0,COLOUR_RED);
				if (IsPlayerAdmin(receiverid))
				{
					xpamount = xpamountkeep-MAX_XP;
				} else {
					xpamount = xpamountkeep-MAX_LVLXP;
				}
			}
			SetPlayerXP(receiverid,Player[receiverid][XP]+xpamount);
			Player[receiverid][XP]=GetPlayerXP(receiverid);
			CheckPlayerLevel(receiverid);
			new string[MAX_STRING];
			if (xpamount > 0)
			{
				format(string,sizeof(string), lang_texts[12][5] ,oGetPlayerName(playerid),xpamount);
				SendPlayerFormattedText(receiverid,string, 0,COLOUR_XP_GOOD);
				format(string,sizeof(string), lang_texts[12][6] ,oGetPlayerName(receiverid),xpamount,GetPlayerXP(receiverid));
				SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			}
			else
			{
				format(string,sizeof(string), lang_texts[12][7] ,oGetPlayerName(playerid),xpamount);
				SendPlayerFormattedText(receiverid,string, 0,COLOUR_XP_GOOD);
				format(string,sizeof(string), lang_texts[12][8] ,oGetPlayerName(receiverid),xpamount,GetPlayerXP(receiverid));
				SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			}
		}
		if(ListResponseType[playerid] == 15)
		{
			new receiverid = Player[playerid][ClickedPlayerid];
			if ((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
			{
				SendPlayerFormattedText(playerid, lang_texts[12][9] , 0,COLOUR_RED);
				return 1;
			}
			if (IsPlayerConnected(receiverid) == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[12][10] , 0,COLOUR_RED);
				return 1;
			}
			new cashamount = strval(inputtext);
			if ((cashamount < -999999) || (cashamount == 0) || (cashamount > 999999))
			{
				SendPlayerFormattedText(playerid, lang_texts[12][11] , 0,COLOUR_RED);
				return 1;
			}
			oGivePlayerMoney(receiverid,cashamount,1);
			new string[MAX_STRING];
			if (cashamount > 0)
			{
				format(string,sizeof(string), lang_texts[12][12] ,oGetPlayerName(playerid),cashamount);
				SendPlayerFormattedText(receiverid,string, 0,COLOUR_XP_GOOD);
				format(string,sizeof(string), lang_texts[12][13] ,oGetPlayerName(receiverid),cashamount,GetPlayerMoney(receiverid));
				SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			}
			else
			{
				format(string,sizeof(string), lang_texts[12][14] ,oGetPlayerName(playerid),cashamount);
				SendPlayerFormattedText(receiverid,string, 0,COLOUR_XP_GOOD);
				format(string,sizeof(string), lang_texts[12][15] ,oGetPlayerName(receiverid),cashamount,GetPlayerMoney(receiverid));
				SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
			}
		}

	}
	return 1;
}
