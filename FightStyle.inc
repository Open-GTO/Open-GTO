//
// Created:   09.11.09 - /20/11/09.
// Aurthor:   heufix
// GTO Russian team(R).

#include "player"

forward AnimNPCforPlayer();
forward KungFu();

#define MAX_PICK 4                       // Max pick fight in sport club
#define FightDialog 8                    // Id Dialoga for Commands FightStyle NOT change
#define BoxTimeFightLessons 5000         // Сколько времени проходит между показами движений Боксёра
#define KunTimeFightLessons 5000         // Сколько времени проходит между показами движений КунгФу
#define KneTimeFightLessons 5000         // Сколько времени проходит между показами движений KneeHead
#define GrabTimeFightLessons 5000        // Сколько времени проходит между показами движений GrabKick
#define LevelBox 5                       // С какого уровня доступно изучить стиль Боксёра
#define LevelKungFu 7                    // С какого уровня доступно изучить стиль КунгФу
#define LevelKneeHead 10                 // С какого уровня доступно изучить стиль KneeHead
#define LevelGrabKick 15                 // С какого уровня доступно изучить стиль GrabKick

new Fightcount[4],                       // variable for count movement
	fighttimers[4]                          // variable for timer four fight style
/*n1,n2,n3,n4*/;                                    // check style
new bot[4]=0,                           // four bot for show movement
	KungFuTimer,
	Pupil[4]=0;                             // pupil player
new SelectForPlayer[MAX_PLAYERS]=0,        // check style
	InLessons[MAX_PLAYERS]=0;                // check playerid in lessons


stock FightStyle_OnPlayerSpawn(playerid)
{
	if(IsPlayerNPC(playerid))
    {
		if(!strcmp(oGetPlayerName(playerid), "FyghtStyle0", true))                  // get npc name
        {
			fighttimers[0] = SetTimer("AnimNPCforPlayer",BoxTimeFightLessons,1); 
			bot[0] = playerid;                                                  
			SetPlayerSkin(bot[0],81);                                         
  			return 1;
        }
        else if(!strcmp(oGetPlayerName(playerid), "FyghtStyle1", true))
        {
			KungFuTimer = SetTimer("KungFu",KunTimeFightLessons,1);     // start timer
			bot[1] = playerid;
			SetPlayerSkin(bot[1],203);
  			return 1;
        }
        else if(!strcmp(oGetPlayerName(playerid), "FyghtStyle2", true))
        {
			fighttimers[2] = SetTimer("AnimNPCforPlayer",KneTimeFightLessons,1);
			bot[2] = playerid;                                                        //  set player to bot var
			SetPlayerSkin(bot[2],203);
  			return 1;
        }
        else if(!strcmp(oGetPlayerName(playerid), "FightStyle3", true))
        {
			fighttimers[3] = SetTimer("AnimNPCforPlayer",GrabTimeFightLessons,1);
			bot[3] = playerid;
			SetPlayerSkin(bot[3],203);                                               // change bot skin
  			return 1;
        }
        return 1;
	}
    return 1;
}

stock FightStyle_OnGameModeInit()
{
	CreatePickup(1314, 2, 759.4146,5.1854,1000.7054, 0); // 1 los santos
	CreatePickup(1314, 2, 766.8073,-22.8696,1000.5859, 0); // 2 San fiero
	CreatePickup(1314, 2, 774.8115,-62.2324,1000.7184, 0); // 3 las venturas
	CreatePickup(1314, 2, 1233.0804,-757.3818,1084.0150, 0); // LS secren pick on holm
}

stock FightSty_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys
	if(newkeys == KEY_USING)
	{
		if(IsPlayerInRangeOfPoint(playerid,2,759.4146,5.1854,1000.7054))// BOXING
		{
			if(GetPlayerLevel(playerid) < LevelBox){new bs[MAX_STRING];format(bs,sizeof(bs),lang_texts[19][2],LevelBox);SendClientMessage(playerid,COLOUR_GREY,bs); return 0;}
			if(Player[playerid][FightStyle] >= 1) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][6]); // if already
			ConnectNPC("FyghtStyle0","FightLs1");
			Pupil[0] = playerid;
			InLessons[playerid] = 1;
			SetPlayerCameraPos(Pupil[0],760.6954,7.4912,1002.1100);
			SetPlayerCameraLookAt(Pupil[0],760.6954,8.4912,1001.9100);
			SelectForPlayer[playerid]=1;
		}
		else if(IsPlayerInRangeOfPoint(playerid,2,766.8073,-22.8696,1000.5859))
		{
			if(GetPlayerLevel(playerid) < LevelKungFu){new ks[MAX_STRING];format(ks,sizeof(ks),lang_texts[19][3],LevelKungFu);SendClientMessage(playerid,COLOUR_GREY,ks); return 0;}
			if(Player[playerid][FightStyle] >= 2) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][6]); // if already
			if(Player[playerid][FightStyle] < 1 ) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][7]); // if n'studied the previous style
			ConnectNPC("FyghtStyle1","FightSF2");
			Pupil[1] = playerid;
			InLessons[playerid] = 1;
			SetPlayerCameraPos(Pupil[1],768.1960,-27.8999,1001.3459);
			SetPlayerCameraLookAt(Pupil[1],768.1960,-30.0,1001.3459);
		}
		else if(IsPlayerInRangeOfPoint(playerid,2,774.8115,-62.2324,1000.7184))
		{
			if(GetPlayerLevel(playerid) < LevelKneeHead){new nks[MAX_STRING];format(nks,sizeof(nks),lang_texts[19][4],LevelKneeHead);SendClientMessage(playerid,COLOUR_GREY,nks); return 0;}
			if(Player[playerid][FightStyle] >= 3) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][6]); // if already
			if(Player[playerid][FightStyle] < 2 ) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][8]); // if n'studied the previous style
			ConnectNPC("FyghtStyle2","FightLV3");
			Pupil[2] = playerid;
			InLessons[playerid] = 1;
			SetPlayerCameraPos(playerid,763.2036,-73.1794,1002.6563);
			SetPlayerCameraLookAt(playerid,765.1000,-70.1794,1002.6563);
			SelectForPlayer[playerid]=3;
		}
		else if(IsPlayerInRangeOfPoint(playerid,2,1233.0804,-757.3818,1084.0150))
		{
			if(GetPlayerLevel(playerid) < LevelGrabKick){new gs[MAX_STRING];format(gs,sizeof(gs),lang_texts[19][5],LevelGrabKick);SendClientMessage(playerid,COLOUR_GREY,gs); return 1;}
			if(Player[playerid][FightStyle] >= 4) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][6]);  // if already
			if(Player[playerid][FightStyle] < 3 ) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[19][9]); // if n'studied the previous style
			ConnectNPC("FightStyle3","FightLS4");
			Pupil[3] = playerid;
			InLessons[playerid] = 1;
			SetPlayerCameraPos(playerid,1233.6950,-770.8999,1083.7300);
			SetPlayerCameraLookAt(playerid,1233.6950,-669.8999,1083.5300);
			SelectForPlayer[playerid]=4;
		}
	}
	return 1;
}

public AnimNPCforPlayer()
{
	print("AnimNPCforPlayer\n");
	if(SelectForPlayer[Pupil[0]]==1)
	{
		if(!IsPlayerConnected(bot[1])){ KillTimer(fighttimers[1]);  return 0;}
		new stringBox[256];
		Fightcount[0]++;
		if(Fightcount[0] == 1)
		{
			ApplyAnimation(bot[0],"FIGHT_B","FightB_1",4.1,0,1,1,1,1);
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 2)
		{
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			ApplyAnimation(bot[0],"FIGHT_B","FightB_2",4.1,0,1,1,1,1);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 3)
		{
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			ApplyAnimation(bot[0],"FIGHT_B","FightB_3",4.1,0,1,1,1,1);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 4)
		{
			ApplyAnimation(bot[0],"FIGHT_B","FightB_block",4.1,0,1,1,1,1);
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 5)
		{
			ApplyAnimation(bot[0],"FIGHT_B","FightB_G",4.1,0,1,1,1,1);
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 6)
		{
			ApplyAnimation(bot[0],"FIGHT_B","FightB_IDLE",4.1,0,1,1,1,1);
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 7)
		{
			ApplyAnimation(bot[0],"FIGHT_B","FightB_M",4.1,0,1,1,1,1);
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 8)
		{
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
			ApplyAnimation(bot[0],"FIGHT_B","HitB_1",4.1,0,1,1,1,1);
		}
		else if(Fightcount[0] == 9)
		{
			ApplyAnimation(bot[0],"FIGHT_B","HitB_2",4.1,0,1,1,1,1);
			format(stringBox,sizeof(stringBox),lang_texts[19][10],Fightcount[0]);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 10)
		{
			ApplyAnimation(bot[0],"FIGHT_B","HitB_3",4.1,0,1,1,1,1);
			SetPlayerChatBubble(bot[0], stringBox, COLOUR_WHITE, 100.0, BoxTimeFightLessons);
		}
		else if(Fightcount[0] == 11)
		{
			KillTimer(fighttimers[0]);
			Player[Pupil[0]][FightStyle] = 1;
			SetPlayerFightingStyle(Pupil[0],Player[Pupil[0]][FightStyle]);
			SendClientMessage(Pupil[0],COLOUR_YELLOW,lang_texts[19][16]);
			SetCameraBehindPlayer(Pupil[0]);
			Kick(bot[0]);
			Fightcount[0]=0,InLessons[Pupil[0]] = 0,Pupil[0]=0,bot[0]=0;
		}
	}
	if(SelectForPlayer[Pupil[2]]==3)
	{                                                                                   // fightstyle 3
		if(!IsPlayerConnected(bot[2])){ KillTimer(fighttimers[2]);  return 0;}
		Fightcount[2]++;
		new stringKneehead[MAX_STRING];
		if(Fightcount[2] == 1)
		{
			ApplyAnimation(bot[2],"FIGHT_D","FightD_1",4.1,0,1,1,1,1);
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
		}
		else if(Fightcount[2] == 2)
		{
			ApplyAnimation(bot[2],"FIGHT_D","FightD_2",4.1,0,1,1,1,1);
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
		}
		else if(Fightcount[2] == 3)
		{
			ApplyAnimation(bot[2],"FIGHT_D","FightD_3",4.1,0,1,1,1,1);
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
		}
		else if(Fightcount[2] == 4)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","FightD_block",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 5)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","FightD_G",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 6)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","FightD_IDLE",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 7)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","FightD_M",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 8)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","HitD_1",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 9)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","HitD_2",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 10)
		{
			format(stringKneehead,sizeof(stringKneehead),lang_texts[19][12],Fightcount[2]);
			SetPlayerChatBubble(bot[2], stringKneehead, COLOUR_WHITE, 100.0, KneTimeFightLessons);
			ApplyAnimation(bot[2],"FIGHT_D","HitD_3",4.1,0,1,1,1,1);
		}
		else if(Fightcount[2] == 11)
		{
			KillTimer(fighttimers[2]);
			if(IsPlayerConnected(Pupil[2]))
			{
				Player[Pupil[2]][FightStyle]=3;
				SetPlayerFightingStyle(Pupil[2],Player[Pupil[2]][FightStyle]);
				SetCameraBehindPlayer(Pupil[2]);
				SendClientMessage(Pupil[2],COLOUR_YELLOW,"Ты выучил стиль!Поздравляем!");
				Kick(bot[2]);
				InLessons[Pupil[2]] = 0,Fightcount[2]=0,bot[2]=-1,Pupil[2]=-1;
			}
			else
			{
				Kick(bot[2]);
				Fightcount[2]=0;
				bot[2]=0;
				Pupil[2]=0;
			}
		}
	}
	if(SelectForPlayer[Pupil[3]]==4)
	{
		if(!IsPlayerConnected(bot[3]))
		{ 
			KillTimer(fighttimers[3]);  
			return 0;
		}                                                                      // fightstyle  4
		Fightcount[3]++;
		new grabstring[256];
		if(Fightcount[3] == 1)
		{
			ApplyAnimation(bot[3],"FIGHT_E","FightKick",4.1,0,1,1,1,1);
			format(grabstring,sizeof(grabstring),lang_texts[19][13],Fightcount[3]);
			SetPlayerChatBubble(bot[3], grabstring, COLOUR_WHITE, 100.0, GrabTimeFightLessons);
		}
		else if(Fightcount[3] == 2)	
		{
			ApplyAnimation(bot[3],"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
			format(grabstring,sizeof(grabstring),lang_texts[19][13],Fightcount[3]);
			SetPlayerChatBubble(bot[3], grabstring, COLOUR_WHITE, 100.0, GrabTimeFightLessons);
		}
		else if(Fightcount[3] == 3)
		{
			ApplyAnimation(bot[3],"FIGHT_E","Hit_fightkick",4.1,0,1,1,1,1);
			format(grabstring,sizeof(grabstring),lang_texts[19][13],Fightcount[3]);
			SetPlayerChatBubble(bot[3], grabstring, COLOUR_WHITE, 100.0, GrabTimeFightLessons);
		}
		else if(Fightcount[3] == 4)
		{
			ApplyAnimation(bot[3],"FIGHT_E","Hit_fightkick_B",4.1,0,1,1,1,1);
			format(grabstring,sizeof(grabstring),lang_texts[19][13],Fightcount[3]);
			SetPlayerChatBubble(bot[3], grabstring, COLOUR_WHITE, 100.0, GrabTimeFightLessons);
		}
		else if(Fightcount[3] == 5)
		{
			KillTimer(fighttimers[3]);
			if(IsPlayerConnected(Pupil[3]))
			{
				Player[Pupil[3]][FightStyle]=4;
				SetCameraBehindPlayer(Pupil[3]);
				SendClientMessage(Pupil[3],COLOUR_YELLOW,lang_texts[19][14]);
				InLessons[Pupil[3]] = 0,Fightcount[3]=0,bot[3]=0,Pupil[3]=-1;
				Kick(bot[3]);
				SetPlayerFightingStyle(Pupil[3],Player[Pupil[3]][FightStyle]);
			}
			else
			{
				Fightcount[3]=0;
				bot[3]=0;
				Pupil[3]=0;
				Kick(bot[3]);
			}
		}
	}
	return 1;
}

stock FightStyle_OnPlayerCommandText(playerid,cmdtext[]) // process player commands
{
	if(!strcmp("/fight",cmdtext,true,5)) ShowPlayerDialog(playerid,FightDialog,DIALOG_STYLE_LIST,"Выберите один из стилей борьбы","Boxing style\nKungfu style\nKneHead style\nGrab style\nNormal style","Выбрать","Отмена");
	if(!strcmp("/fighttest",cmdtext,true,7))
	{
		new s[MAX_STRING],sssb[MAX_PLAYER_NAME],sssbb[MAX_PLAYER_NAME];
		GetPlayerName(bot[0],sssb,sizeof(sssb));
		GetPlayerName(bot[1],sssbb,sizeof(sssbb));
		format(s,sizeof(s)," SforP:%d  Pupil0:%d Pupil1:%d | b0:%s b1:%s ",SelectForPlayer[playerid],Pupil[0],Pupil[1],sssb,sssbb);
		SendClientMessage(playerid,COLOUR_GREEN,s);
		return 1;
	}
	return 0;
}

stock FightStyle_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
    if(dialogid == FightDialog)
    {
        if(response)
		{
			if(listitem == 0)
			{
				if(Player[playerid][FightStyle] < 1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[19][1]);
				else SetPlayerFightingStyle(playerid, 5);
			}
			if(listitem == 1)
			{
				if(Player[playerid][FightStyle] < 2) return SendClientMessage(playerid,COLOUR_RED,lang_texts[19][1]);// kungfu
				else SetPlayerFightingStyle (playerid, 6);
			}
			if(listitem == 2) if(Player[playerid][FightStyle] < 3) return SendClientMessage(playerid,COLOUR_RED,lang_texts[19][1]);// kneehaed
			else SetPlayerFightingStyle (playerid, 7);
			if(listitem == 3)
			{
				if(Player[playerid][FightStyle] < 4) return SendClientMessage(playerid,COLOUR_RED,lang_texts[19][1]); // grab
				else SetPlayerFightingStyle (playerid, 15);
			}
			if(listitem == 4) return SetPlayerFightingStyle (playerid, 4); // normal
		}
		return 1;
 	}
    return 1;
}

public KungFu()
{
	new stringCun[256];
	Fightcount[1]++;
	if(Fightcount[1] == 1)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_1",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 2)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_2",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 3)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_3",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 4)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_block",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 5)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_blocking",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 6)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_G",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 7)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_IDLE",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 8)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_M",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 9)
	{
		ApplyAnimation(bot[1],"FIGHT_C","FightC_Spar",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 10)
	{
		ApplyAnimation(bot[1],"FIGHT_C","HitC_1",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 11)
	{
		ApplyAnimation(bot[1],"FIGHT_C","HitC_2",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 12)
	{
		ApplyAnimation(bot[1],"FIGHT_C","HitC_3",4.1,0,1,1,1,1);
		format(stringCun,sizeof(stringCun),lang_texts[19][11],Fightcount[1]);
		SetPlayerChatBubble(bot[1], stringCun, COLOUR_WHITE, 100.0, KunTimeFightLessons);
	}
	else if(Fightcount[1] == 13)
	{
		KillTimer(KungFuTimer);
		Player[Pupil[1]][FightStyle]=2;
		SendClientMessage(Pupil[1],COLOUR_YELLOW,lang_texts[19][15]);
		SetCameraBehindPlayer(Pupil[1]);
		Kick(bot[1]);
		Fightcount[1]=0;
		InLessons[Pupil[1]] = 0;
		bot[1]=0;
		Pupil[1] =0;
	}
	return 1;
}