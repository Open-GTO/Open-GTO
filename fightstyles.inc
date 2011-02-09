/*
 Created: 29.05.2010
 Author: ZiGGi
*/

#if defined _fightstyles_included
  #endinput
#endif

#define _fightstyles_included
#pragma library fightstyles

#define fightstyles_DialogID 8
#define MAX_FIGHT 4

enum FightInfo {
	Style,
	StyleName[MAX_STRING],
	Cost,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z,
};

new FightTreners[MAX_FIGHT][FightInfo] = {
	{FIGHT_STYLE_NORMAL,"нормальный",0,0.0,0.0,0.0},
	{FIGHT_STYLE_BOXING,"бокс",1000,767.6851,12.8269,1000.7025}, // LS
	{FIGHT_STYLE_KNEEHEAD,"колено-голова",2000,768.5967,-22.9764,1000.5859}, // SF
	{FIGHT_STYLE_KUNGFU,"кунг-фу",3000,766.5240,-76.6523,1000.6563} // LV
};

stock fights_OnGameModeInit()
{
	for(new fightid=1;fightid<MAX_FIGHT;fightid++)
	{
	    CreatePickup(1314,23,FightTreners[fightid][Coord_X],FightTreners[fightid][Coord_Y],FightTreners[fightid][Coord_Z]);
	}
	print("Загрузка: Система стилей борьбы запущена.");
	return 1;
}

stock fights_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	new string[MAX_STRING];
	new TrenerID = GetPlayerFightTrenerID(playerid);
	if(FightTreners[TrenerID][Style] <= Player[playerid][FightStyle])
	{
		format(string,sizeof(string),"Вы уже выучили стиль: %s(%d)",FightTreners[TrenerID][StyleName],FightTreners[TrenerID][Style]);
		return SendClientMessage(playerid,COLOUR_WHITE,string);
	}
	if(FightTreners[TrenerID][Style]-1 > Player[playerid][FightStyle])
	{
		return SendClientMessage(playerid,COLOUR_WHITE,"Вы сначала, должны выучить предидущие стили!");
	}
	format(string,sizeof(string),"Выучить стиль борьбы %s(%d), за %d$",FightTreners[TrenerID][StyleName],FightTreners[TrenerID][Style],FightTreners[TrenerID][Cost]);
	ShowPlayerDialog(playerid,fightstyles_DialogID,DIALOG_STYLE_MSGBOX,"Тренер",string,"Выучить","Отмена");
	return 1;
}

stock fights_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext,listitem
    new string[MAX_STRING];
	if(dialogid == fightstyles_DialogID)
	{
	    if(!response) return 1;
		new TrenerID = GetPlayerFightTrenerID(playerid);
		if(oGetPlayerMoney(playerid) < FightTreners[TrenerID][Cost])
		{
			format(string,sizeof(string),"Вам недостаточно денег, нужно %d$",FightTreners[TrenerID][Cost]);
			return SendClientMessage(playerid,COLOUR_WHITE,string);
		}
		Player[playerid][FightStyle] = FightTreners[TrenerID][Style];
		oGivePlayerMoney(playerid,-FightTreners[TrenerID][Cost],0);
		format(string,sizeof(string),"Вы изучили стиль борьбы: %s(%d), за: %d$. Включить стиль: '/f [4-7]'",FightTreners[TrenerID][StyleName],FightTreners[TrenerID][Style],FightTreners[TrenerID][Cost]);
		SendClientMessage(playerid,COLOUR_WHITE,string);
		return 1;
	}
	return 1;
}

stock GetPlayerFightTrenerID(playerid)
{
	for(new fightid=1;fightid<MAX_FIGHT;fightid++)
		if(IsPlayerInRangeOfPoint(playerid,2.0,FightTreners[fightid][Coord_X],FightTreners[fightid][Coord_Y],FightTreners[fightid][Coord_Z]))
			return fightid;
	return -1;
}

stock ConvertFightIdToName(fstyleid)
{
	new fname[MAX_STRING];
	switch(fstyleid)
	{
		case FIGHT_STYLE_NORMAL: set(fname,FightTreners[0][StyleName]);
		case FIGHT_STYLE_BOXING: set(fname,FightTreners[1][StyleName]);
		case FIGHT_STYLE_KNEEHEAD: set(fname,FightTreners[2][StyleName]);
		case FIGHT_STYLE_KUNGFU: set(fname,FightTreners[3][StyleName]);
	}
	return fname;
}