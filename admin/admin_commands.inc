//
// Created: 04.11.06
// Aurthor: Iain Gilbert, represent by Dmitry Frolov (FP)
//

#if defined _admin_commands_included
	#endinput
#endif

#define _admin_commands_included
#pragma library admin_commands

#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"
#include "race"
#include "payday"

new Float:PointsH[][CoordInfo] = {
{2647.597656,-2003.409667,13.382812},		//1 LL tune
{1044.264282,-1048.066284,31.700469},
{-2689.126220,212.037155,4.328125},		//0 WAA tune
{-1910.156005,210.557525,35.156250},
{2401.584472,1026.608032,10.812986},

{-1346.315307,492.034332,11.202690},		//Military 2
{2832.146972,-2529.524658,17.919458},		//6
{1738.811523,-2543.253173,13.546875},
{-1282.327880,-166.490951,14.148437},
{1432.760986,1490.453857,10.820312},

{364.079864,2504.678222,16.484375},		//10
{1116.208496,-2021.855346,74.429687},
{2737.499023,-1760.086181,44.143890},
{1223.016357,-1791.427124,33.635219},
{1544.125122,-1353.947753,329.473236},

{914.587585,-1021.723815,111.054687},
{-2233.071289,-1736.994506,480.829284},		//Mount Chilliad 2
{1976.502441,1916.305297,130.934509},		//Highs with parashute 3
{302.717712,-1623.151123,114.416259},
{2065.416503,2366.291992,150.476562},

{2729.579833,2686.808349,59.023437},
{-1818.642211,581.224853,234.890625},
{2658.831054,-1532.073120,47.054687},
{-341.302612,-172.889511,57.487934},
{1832.951538,-1315.967773,131.734375},

{-1941.218261,134.969512,41.000000},
{-2824.375488,2176.859375,180.767562},
{1481.328125,-1790.289794,156.753326},		//Bank 1
{1103.387573,-836.532775,112.982246},
{-2667.522460,1595.073852,217.273895}
};


stock Admin_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerRconAdmin(playerid)) return 0;
	new cmd[20],idx;
	cmd = strcharsplit(text, idx,' ');
	
	if(!strcmp(cmd,"/cmdlist",true))
	{
		SendClientMessage(playerid, COLOUR_GREEN, "Administrator RCON:______________________________________________________________");
		SendClientMessage(playerid, COLOUR_WHITE, "R: '/ssay <text>', '/an <type> <LATtext>', '/setlvl <ID> <lvl>', '/setstatus <ID> <status> (0,1-10,11-256)'");
		SendClientMessage(playerid, COLOUR_WHITE, "R: '/disarm <ID>', '/disarmall', '/skydiveall', '/paralyzeall', '/deparalyzeall', '/remcash <ID>', '/remcashall'");
		SendClientMessage(playerid, COLOUR_WHITE, "R: '/payday', /go <point> (0,1, 2-4(tune) - 27)', '/allowport <ID> <allow>', '/setskin <ID> <IDskin>'");
		SendClientMessage(playerid, COLOUR_WHITE, "R: '/carrep', '/carinfo', '/int', '/boom <IDboom>'");
		SendClientMessage(playerid, COLOUR_YELLOW, "S: '/sys cmdlist', '/sys about', '/sys info'");
		return 1;
	}
	if(!strcmp(cmd,"/about",true))
	{
		SendClientMessage(playerid, COLOUR_GREEN, lang_texts[15][55]);
		SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][51]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][52]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][53]);
		SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][54]);
		return 1;
	}
	if((!strcmp(cmd,"/carinfo",true)) && IsPlayerInAnyVehicle(playerid))
	{
		new Float:x3,Float:y3,Float:z3,Float:zangle3,Float:health3;
		new veh_id = GetPlayerVehicleID(playerid);
		GetVehiclePos(veh_id,x3,y3,z3);
		GetVehicleZAngle(veh_id,zangle3);
		GetVehicleHealth(veh_id,health3);
		new string[MAX_STRING];
		format(string,sizeof(string),
			"*** Car %s(ID %d, HL %d): %f, %f, %f, %f ***",
			GetVehicleName( GetVehicleModel(veh_id) ),veh_id,health3,x3,y3,z3,zangle3
		);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		return 1;
	}
	if((!strcmp(cmd,"/carrep",true)) && IsPlayerInAnyVehicle(playerid))
	{
		new veh_id = GetPlayerVehicleID(playerid);
		RepairVehicle(veh_id);
		new string[MAX_STRING];
		format(string,sizeof(string),"*** Траспорт (ИД %d) полностью починен",veh_id);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		return 1;
	}
	if(!strcmp(cmd,"/go",true))
	{
		new pointt = strval(strcharsplit(text, idx,' '));
		if(pointt < 0 || pointt >= sizeof(PointsH)) return 1;
		
		if(IsPlayerInAnyVehicle(playerid)) 
			SetVehiclePos( GetPlayerVehicleID(playerid),PointsH[pointt][Coord_X],PointsH[pointt][Coord_Y],PointsH[pointt][Coord_Z]);
		else
			oSetPlayerPos(playerid,PointsH[pointt][Coord_X],PointsH[pointt][Coord_Y],PointsH[pointt][Coord_Z]);
		
		new string[MAX_STRING];
		format(string,sizeof(string),"*** Вы были перемещены в точку с ИД %d",pointt);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		return 1;
	}
	if(!strcmp(cmd,"/an",true))
	{
		new mtype = strval(strcharsplit(text, idx,' '));
		if((mtype < 0) || (mtype == 2) || (mtype > 9)) return 1;
		new string[MAX_STRING];
		set(string,text);
		if(string[0] == '~') return 1; // не дадим крэшнуть:)
		strdel(string,0,idx);
		format(string,sizeof(string),"%s",string);
		GameTextForAll(string,4999,mtype);
		return 1;
	}
	if(!strcmp(cmd,"/payday",true))
	{
		GivePay();
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[12][90]);
		return 1;
	}
	if(!strcmp(cmd,"/boom", true))
	{
		new bomamount = strval(strcharsplit(text, idx,' '));
		new Float:X,Float:Y,Float:Z;
		GetPlayerPos(playerid,X,Y,Z);
		CreateExplosion((X+20),Y,(Z+2),bomamount,30);
		return 1;
	}
	if(!strcmp(cmd,"/setskin", true))
	{
		SendMessageToAdmins(playerid,COLOUR_LIGHTGREEN, text);
		new receiverid = INVALID_PLAYER_ID;
		receiverid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid)))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);

		new sknamount = strval(strcharsplit(text, idx,' '));
		if(!IsSkinValid(sknamount))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][41]);

		new sknkeep = GetPlayerSkin(receiverid);
		SetPlayerSkin(receiverid, sknamount);
		new string[MAX_STRING];
		format(string,sizeof(string), lang_texts[12][42] ,oGetPlayerName(playerid),sknamount);
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
		format(string,sizeof(string), lang_texts[12][43] ,oGetPlayerName(receiverid),sknamount,sknkeep);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
	
		SetPlayerSkinModel(playerid,sknamount);
		player_save_db_ini(receiverid);
		return 1;
	}
	if(!strcmp(cmd,"/ssay",true))
	{
		new string[MAX_STRING];
		set(string,text);
		strdel(string,0,idx);
		format(string,sizeof(string),lang_texts[12][1],string);
		SendClientMessageToAll(COLOUR_YELLOW, string);
		return 1;
	}
	if(!strcmp(cmd,"/skydiveall",true))
	{
		new string[MAX_STRING];
		new Float:playerx,Float:playery,Float:playerz;
		foreach(Player,id)
		{
			if(!GetPlayerJailed(id) && !IsPlayerInAnyDM(id))
			{
				oGivePlayerWeapon(id,46,1);
				GetPlayerPos(id,playerx,playery,playerz);
				PlayerPlaySound(id,1058,0,0,0);
				oSetPlayerPos(id,playerx,playery,playerz+1200);
			}
			if(GetPlayerJailed(id) == 1)
				SendClientMessage(id,COLOUR_LIGHTRED,lang_texts[1][22]);
		}
		SendClientMessageToAll(COLOUR_WHITE,lang_texts[1][19]);
		format(string,sizeof(string), lang_texts[1][20]);
		GameTextForAll(string,5000,6);
		return 1;
	}
	if(!strcmp(cmd,"/disarm",true))
	{
		new receiverid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(receiverid) && !IsPlayerInAnyDM(receiverid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][10]);

		ac_ResetPlayerWeapons(receiverid);
		DropAmmo(receiverid);
		new string[MAX_STRING];
		format(string,sizeof(string), lang_texts[12][48] ,oGetPlayerName(playerid));			//игроку
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
		format(string,sizeof(string), lang_texts[12][49] ,oGetPlayerName(receiverid));
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/disarmall",true))
	{
		new string[MAX_STRING];
		foreach(Player,id)
		{
			if(id == playerid || GetPlayerJailed(id) != 0 || IsPlayerInAnyDM(id) || IsPlayerRconAdmin(id))
				continue;
			ac_ResetPlayerWeapons(id);
			DropAmmo(id);
			format(string,sizeof(string),lang_texts[12][48],oGetPlayerName(playerid));	//игроку
			SendClientMessage(id,COLOUR_XP_GOOD,string);
		}
		format(string,sizeof(string),lang_texts[12][75]);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/paralyzeall",true))
	{
		new string[MAX_STRING];
		foreach(Player,id)
		{
			if(id == playerid || GetPlayerJailed(id) != 0 || IsPlayerInAnyDM(id) || IsPlayerRconAdmin(id))
				continue;
			TogglePlayerControllable(id,0);
			format(string,sizeof(string),lang_texts[12][54] ,oGetPlayerName(playerid));//игроку
			SendClientMessage(id,COLOUR_XP_GOOD,string);
		}
		format(string,sizeof(string), lang_texts[12][76]);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/deparalyzeall",true))
	{
		new string[MAX_STRING];
		foreach(Player,id)
		{
			if(id == playerid || GetPlayerJailed(id) != 0 || IsPlayerInAnyDM(id) || IsPlayerRconAdmin(id))
				continue;
			TogglePlayerControllable(id,1);
			format(string,sizeof(string),lang_texts[12][56],oGetPlayerName(playerid));
			SendClientMessage(id,COLOUR_XP_GOOD,string);
		}
		format(string,sizeof(string),lang_texts[12][77]);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/remcash",true))
	{
		new receiverid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid)))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][9]);

		ResetPlayerMoney(receiverid);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][58],oGetPlayerName(playerid));			//игроку
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
		format(string,sizeof(string),lang_texts[12][59],oGetPlayerName(receiverid));
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/remcashall",true))
	{
		new string[MAX_STRING];
		foreach(Player,id)
		{
			if(id == playerid || GetPlayerJailed(id) != 0 || IsPlayerRconAdmin(id)) continue;
			ResetPlayerMoney(id);
			format(string,sizeof(string),lang_texts[12][58],oGetPlayerName(playerid));
			SendClientMessage(id,COLOUR_XP_GOOD,string);
		}
		format(string,sizeof(string),lang_texts[12][78]);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/setlvl",true))
	{
		new receiverid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid)))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);

		new lvlamount = strval(strcharsplit(text, idx,' '));
		if(0 > lvlamount > MAX_LEVEL)
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][33]);

		new lvlthisday = CalculatePlayerLevel(receiverid);
		SetPlayerXP(receiverid,LevelList[lvlamount][level_xp]);
		CheckPlayerLevel(receiverid);
		new string[MAX_STRING];
		format(string,sizeof(string),lang_texts[12][31],oGetPlayerName(playerid),lvlamount);
		SendClientMessage(receiverid,COLOUR_XP_GOOD,string);
		format(string,sizeof(string),lang_texts[12][32],oGetPlayerName(receiverid),lvlamount,lvlthisday);
		SendClientMessage(playerid,COLOUR_XP_GOOD,string);
		return 1;
	}
	if(!strcmp(cmd,"/setstatus",true))
	{
		new receiverid = strval(strcharsplit(text, idx,' '));
		if(!IsPlayerConnected(receiverid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		
		new sid = strval(strcharsplit(text, idx,' '));
		if((sid < 0) || (sid > 3))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][44]);
		
		if(IsPlayerRconAdmin(receiverid) && !IsPlayerAdmin(playerid))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][89]);
		
		new string[MAX_STRING],sidstring[MAX_STRING];
		
		switch(sid)
		{
			case 0: format(sidstring,sizeof(sidstring),lang_texts[12][88]);
			case 1: format(sidstring,sizeof(sidstring),lang_texts[12][87],sid);
			case 2: format(sidstring,sizeof(sidstring),lang_texts[12][86],sid);
			case 3: format(sidstring,sizeof(sidstring),lang_texts[12][86],sid);
		}
		if(sid > 1)
		{
			format(string,sizeof(string),lang_texts[12][82],oGetPlayerName(playerid), playerid, sidstring);
			SendClientMessage(receiverid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][83],oGetPlayerName(receiverid), receiverid, sidstring);
			SendClientMessage(playerid,COLOUR_WHITE,string);
		} 
		else 
		{
			format(string,sizeof(string),lang_texts[12][84],oGetPlayerName(playerid),playerid);
			SendClientMessage(receiverid,COLOUR_WHITE,string);
			format(string,sizeof(string),lang_texts[12][85],oGetPlayerName(receiverid),receiverid);
			SendClientMessage(playerid,COLOUR_WHITE,string);
		}
		SetPlayerStatus(receiverid,sid);
		return 1;
	}
	if(!strcmp(cmd, "/allowport", true))
	{
		new receiverid = strval(strcharsplit(text,idx,' '));
		if(!IsPlayerConnected(receiverid) || (IsPlayerRconAdmin(receiverid) && !IsPlayerRconAdmin(playerid)))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[12][2]);
		new setport = strval(strcharsplit(text, idx,' '));
		if((setport < 0) || (setport > 1)) return 0;
		AllowPlayerTeleport(receiverid,setport);
		new string[MAX_STRING];
		format(string,sizeof(string), "*** Вы разрешили %s(%d) телепорт по карте", oGetPlayerName(receiverid), receiverid );
		SendClientMessage(playerid,COLOUR_WHITE,string);
		return 1;
	}
	if(!strcmp(cmd,"/setvip",true))
	{
		new giveid = strval(strcharsplit(text,idx,' '));
		if(!IsPlayerConnected(giveid)) 
			return SendClientMessage(playerid,COLOUR_WHITE,"Некорректный ввод, '/setvip id dd.mm.yyyy'");
		
		new string[MAX_STRING];
		string = strcharsplit(text,idx,' ');
		
		if(strlen(string) < 10)
			return SendClientMessage(playerid,COLOUR_WHITE,"Некорректный ввод, '/setvip id dd.mm.yyyy'");
		
		SetPlayerVIPDate(giveid,string);
		if(VIPCheck(string))
			SetPlayerVIP(giveid,1);
		else
			SetPlayerVIP(giveid,0);
		
		format(string,sizeof(string),"Вы установили VIP статус %s(%d), до %s",oGetPlayerName(giveid),giveid,GetPlayerVIPDate(giveid));
		SendClientMessage(playerid,COLOUR_WHITE,string);
		format(string,sizeof(string),"Вам установили VIP статус до %s",GetPlayerVIPDate(giveid));
		SendClientMessage(giveid,COLOUR_WHITE,string);
		return 1;	
	}
	if(!strcmp(cmd,"/underwater",true)) 
	{ 
		if(IsPlayerInAnyVehicle(playerid)) 
		{
			new veh_id = GetPlayerVehicleID(playerid);
			SetVehiclePos(veh_id,-2888.677978,1423.499023,-36.533981); 
			SetVehicleZAngle(veh_id,244.428359); 
			LinkVehicleToInterior(veh_id,0); 
		} 
		else
		{ 
			oSetPlayerPos(playerid,-2888.677978,1423.499023,-36.533981); 
			SetPlayerFacingAngle(playerid,244.428359); 
		} 
		SetPlayerInterior(playerid,0); 
		SendClientMessage(playerid,0x0000FFAA,"Вы под водой."); 
		return 1;
	}
	if(!strcmp(cmd,"/ahideme",true))
	{
		SetPVarInt(playerid,"Hide",1);
		PushHide(playerid);
		SendClientMessage(playerid,COLOUR_WHITE,"*** Вы скрыты на радаре");
		return 1;
	}
	if(!strcmp(cmd,"/ashowme",true))
	{
		SetPVarInt(playerid,"Hide",0);
		PushHide(playerid);
		SendClientMessage(playerid,COLOUR_WHITE,"*** Вы видны на радаре");
		return 1;	
	}
	return 0;
}
