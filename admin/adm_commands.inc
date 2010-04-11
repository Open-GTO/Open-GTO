//
// Created: 18.12.2007
// Aurthor: Dmitry Frolov (FP)
//

#if defined _adm_commands_included
#endinput
#endif

#define _adm_commands_included
#pragma library adm_commands
#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "race"

stock Adm_OnPlayerCommandTextr(playerid,text[]) // process player commands
{
	if(!IsPlayerConnected(playerid)) return 0;
	if((!IsPlayerAdmin(playerid)) && (!IsPlayerAdm(playerid))) return 0;
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(!strcmp(cmd,"/cmdlist",true))
	{
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,"A: '/say <text>', '/pinfo <ID>'");
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,"A: '/akill <ID>', '/paralyze <ID>', '/deparalyze <ID>'");
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,"A: '/tele-set', '/tele-loc', '/tele-to <ID>', '/tele-here <ID>', '/tele-hereall'");
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,"A: '/sethealth <ID>', '/setarm <ID>', '/givexp <ID> <xp>', '/agivecash <ID> <cash>', '/givegun <ID> <IDgun>'");
		return 1;
	}
	if(!strcmp(cmd,"/about",true))
	{
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,lang_texts[15][47]);
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,lang_texts[15][48]);
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,lang_texts[15][49]);
		return 1;
	}
	if(!strcmp(cmd,"/say",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new string[MAX_STRING];
		set(string,text);
		strdel(string, 0, idx);
		format(string,sizeof(string),lang_texts[12][37],string);
		SendClientMessageToAll(COLOUR_BLUE,string);
		return 1;
	}
	if(!strcmp(cmd,"/pinfo",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new receiverid = INACTIVE_PLAYER_ID;
		receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		if(IsPlayerConnected(receiverid) == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][34],oGetPlayerName(receiverid),receiverid);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_YELLOW);
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
	//Добавить сюда REPLACE Запрещенных символов
	if(!strcmp(cmd,"/admincnn",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new string[MAX_STRING];
		set(string,text);
		strdel(string, 0, idx);
		format(string,sizeof(string), "%s" ,string);
		GameTextForAll(string,4000,6);
		return 1;
	}
	if(!strcmp(cmd,"/akill",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new receiverid = INACTIVE_PLAYER_ID;
		receiverid = strval(strcharsplit(text,idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		SetPlayerHealth(receiverid,0);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][62],oGetPlayerName(receiverid),receiverid);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		format(string,sizeof(string),lang_texts[12][63],oGetPlayerName(playerid));
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/tele-set",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		GetPlayerPos(playerid,LocX[playerid],LocY[playerid],LocZ[playerid] );
		GetPlayerFacingAngle(playerid,Ang[playerid]);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][68],LocX[playerid],LocY[playerid],LocZ[playerid],Ang[playerid]);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/tele-loc",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, cmd);
		if(LocX[playerid] == 0.0 && LocY[playerid] == 0.0 && LocZ[playerid] == 0.0) 
		{
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][69]);
			SendClientMessage(playerid,COLOUR_RED,string);
		} 
		else 
		{
			if(IsPlayerInAnyVehicle(playerid)) 
			{
				new VehicleID = GetPlayerVehicleID(playerid);
				SetVehiclePos(VehicleID,LocX[playerid],LocY[playerid],LocZ[playerid]);
				SetPlayerFacingAngle(playerid,Ang[playerid]);
			} 
			else 
			{
				SetPlayerPos(playerid,LocX[playerid],LocY[playerid],LocZ[playerid]);
				SetPlayerFacingAngle(playerid,Ang[playerid]);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[12][70]);
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		}
		return 1;
	}
	if(!strcmp(cmd,"/tele-to",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
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
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/tele-here",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		}
		if(!IsPlayerConnected(receiverid) || IsPlayerNPC(receiverid) || IsPlayerInAnyDM(receiverid) || IsPlayerInAnyRace(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
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
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);

		format(string,sizeof(string),lang_texts[12][67],oGetPlayerName(receiverid),receiverid);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/tele-hereall",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN,text);
		new Float:X,Float:Y,Float:Z,Float:Angle;
		GetPlayerPos(playerid,X,Y,Z);
		GetPlayerFacingAngle(playerid,Angle);
		new string[MAX_STRING];
		for(new id=0;id<MAX_PLAYERS;id++)
		{
			if((id < playerid) || (id > playerid))
			{
				if(IsPlayerConnected(id) && !IsPlayerNPC(playerid) && (Player[id][Jailed] == 0) && !IsPlayerAdmin(id)  && !IsPlayerInAnyDM(id))
				{
					if(IsPlayerConnected(id) == 0) return 0;
					SetPlayerFacingAngle(id,Angle);
					X=X+(random(2)-random(4));Y=Y+(random(2)-random(4));	
					SetPlayerPos(id,X,Y,Z);
					format(string,sizeof(string),lang_texts[12][66] ,oGetPlayerName(playerid),playerid);
					SendPlayerFormattedText(id,string, 0,COLOUR_XP_GOOD);
				}
			}
		}
		format(string,sizeof(string), lang_texts[12][74]);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}

	if(!strcmp(cmd, "/sethealth", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		new hltamount = strval(strcharsplit(text, idx,strchar(" ")));
		if((hltamount < 10) || (hltamount > 300)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][44]);
		SetPlayerHealth(receiverid, hltamount);
		return 1;
	}

	if(!strcmp(cmd, "/setarm", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);

		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)))
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		new armamount = strval(strcharsplit(text, idx,strchar(" ")));
		if((armamount < 0) || (armamount > 300)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][44]);
		SetPlayerArmour(receiverid, armamount);
		return 1;
	}
	if(!strcmp(cmd, "/givexp", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);

		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][3]);
		new xpamount = strval(strcharsplit(text, idx,strchar(" ")));
		if((xpamount == 0) || (xpamount > MAX_LVLXP)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][4]);
		new xpamountkeep = GetPlayerXP(receiverid)+xpamount;
		if((!IsPlayerAdmin(receiverid) && xpamountkeep >MAX_LVLXP) || (IsPlayerAdmin(receiverid) && xpamountkeep > MAX_XP))
		{
			SendPlayerFormattedText(playerid, lang_texts[12][40] , 0,COLOUR_RED);
			if(IsPlayerAdmin(receiverid)) xpamount = xpamountkeep-MAX_XP;
			else xpamount = xpamountkeep-MAX_LVLXP;
		}
		SetPlayerXP(receiverid,Player[receiverid][XP]+xpamount);
		Player[receiverid][XP]=GetPlayerXP(receiverid);
		CheckPlayerLevel(receiverid);
		new string[MAX_STRING];
		if(xpamount > 0)
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
		return 1;
	}


	if(!strcmp(cmd, "/agivecash", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
		new cashamount = strval(strcharsplit(text, idx,strchar(" ")));
		if((cashamount < -999999) || (cashamount == 0) || (cashamount > 999999))
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][11]);
		}
		oGivePlayerMoney(receiverid,cashamount,1);
		new string[MAX_STRING];
		if(cashamount > 0)
		{
			format(string,sizeof(string),lang_texts[12][12],oGetPlayerName(playerid),cashamount);
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);

			format(string,sizeof(string),lang_texts[12][13],oGetPlayerName(receiverid),cashamount,GetPlayerMoney(receiverid));
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		}
		else
		{
			format(string,sizeof(string), lang_texts[12][14] ,oGetPlayerName(playerid),cashamount);
			SendClientMessage(receiverid,COLOUR_XP_GOOD,string);

			format(string,sizeof(string), lang_texts[12][15] ,oGetPlayerName(receiverid),cashamount,GetPlayerMoney(receiverid));
			SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		}
		return 1;
	}

//----------------------------------------------------------------------------------------------GUN
	if(!strcmp(cmd,"/givegun",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
		new ammamount = strval(strcharsplit(text, idx,strchar(" ")));
		if((ammamount < 0) || (ammamount > 46)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][45]);
		GivePlayerWeapon(receiverid,ammamount,1000);
		new string[MAX_STRING];
		format(string,sizeof(string), lang_texts[12][46] ,oGetPlayerName(playerid),oGetWeaponName(ammamount), ammamount,1000);			//игроку
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
		
		format(string,sizeof(string), lang_texts[12][47] ,oGetPlayerName(receiverid),receiverid,oGetWeaponName(ammamount) ,ammamount,1000);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}

	if(!strcmp(cmd,"/paralyze",true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);

		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
		TogglePlayerControllable(receiverid,0);

		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][54],oGetPlayerName(playerid));			//игроку
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
		
		format(string,sizeof(string),lang_texts[12][55],oGetPlayerName(receiverid));
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}

	if(!strcmp(cmd, "/deparalyze", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = strval(strcharsplit(text, idx,strchar(" ")));
		if((receiverid < 0) || (receiverid >= MAX_PLAYERS) || (IsPlayerAdmin(receiverid) && !IsPlayerAdmin(playerid)) )
		{
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);
		}
		if(!IsPlayerConnected(receiverid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);
		TogglePlayerControllable(receiverid,1);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][56],oGetPlayerName(playerid));			//игроку
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);

		format(string,sizeof(string),lang_texts[12][57],oGetPlayerName(receiverid));
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/ban",true))
	{
		new tmp[MAX_STRING],var[32],string[MAX_STRING],params[MAX_STRING];
		tmp	= strtok(cmd,idx);
		if(isNumeric(tmp))
		{
			format(string,sizeof(string),"GTO/Bans/Names/%s.txt",oGetPlayerName(strval(tmp)));
			new File:file = fopen(string,io_append);
			if(file)
			{
			    format(var,32,"Banned");
			    fwrite(file,var);
			    fclose(file);
			}
			set(params,oGetPlayerName(strval(tmp)));
		}
		else
		{
			format(string,sizeof(string),"GTO/Bans/Names/%s.txt",tmp);
			new File:file = fopen(string,io_append);
			if(file)
			{
			    format(var,32,"Banned");
			    fwrite(file,var);
			    fclose(file);
			}
			set(params,tmp);
		}
		new ye,mo,da,ho,mi,se;
		getdate(ye,mo,da);
		gettime(ho,mi,se);
		format(string,sizeof(string),"%s забанил никнейм %s. Команда: /ban (%02d.%02d.%02d, %02d:%02d)",oGetPlayerName(playerid),params,da,mo,ye,ho,mi);
		new entry[MAX_STRING];
		format(entry,sizeof(entry),"%s\r\n",string);
		new File:open3 = fopen("GTO/Bans/Log.txt",io_append);
		if(open3)
		{
		    fwrite(open3,entry);
		    fclose(open3);
		}
		return 1;
	}
	return 0;
}

stock BanCheck(playerid)
{
    new player[MAX_PLAYER_NAME],string[MAX_STRING],filestr[MAX_STRING];
    GetPlayerName(playerid,player,sizeof player);
	format(string,sizeof(string),"GTO/Bans/Names/%s.txt",player);
	format(player,sizeof(player),"Banned");
	new File:file = fopen(string,io_read);
	if(file)
	{
	    while(fread(file,filestr,sizeof(filestr)))
	    {
	    	if(strfind(filestr,"Banned",true) != -1)
	    	{
	    	    SendClientMessage(playerid,red,"XpaП: Ou caaaiai ia na?aa?a NOOEA!");
	    	    return Kick(playerid);
	    	}
		}
	}
	return true;
}
