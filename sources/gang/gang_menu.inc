/*

	About: gang user menu
	Author: ziggi
	
*/

#if defined _gang_menu_included
  #endinput
#endif

#define _gang_menu_included
#pragma library gang_menu


DialogCreate:GangMenu(playerid)
{
	new string[MAX_STRING * 6],
		gangid = player_GetGangID(playerid);
	
	if (gangid == 0) {
		strcat(string, "Информация о бандах\n", sizeof(string));
		strcat(string, "Создать банду\n", sizeof(string));
		if (GetPVarInt(playerid, "GangInvite") != 0) {
			strcat(string, "Принять приглашение\n", sizeof(string));
		}
	} else {
		strcat(string,
			"Информация о банде\n\
			Выйти из банды\n",
			sizeof(string)
		);
		if (gang_IsLeader(playerid)) {
			strcat(string,
				"Пригласить игрока\n\
				Выгнать игрока\n\
				Изменить цвет\n\
				Изменить объявление",
				sizeof(string)
			);
		}
	}
	
	Dialog_Open(playerid, Dialog:GangMenu, DIALOG_STYLE_LIST, "Меню банды", string, "ОК", "Назад");
}

DialogResponse:GangMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	new gangid = player_GetGangID(playerid);
	if (gangid == 0) {
		switch (listitem) {
			// информация
			case 0: {
				new string[MAX_STRING * 4];
				strcat(string, lang_texts[6][5], sizeof(string));
				strcat(string, lang_texts[6][6], sizeof(string));
				strcat(string, lang_texts[6][7], sizeof(string));
				strcat(string, lang_texts[6][8], sizeof(string));
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][4], string, "Назад", "Отмена");
				return 1;
			}
			// создать банду
			case 1: {
				if (GetPlayerMoney(playerid) < Gang_Create_Cost) {
					new string[MAX_STRING];
					format(string, sizeof(string), lang_texts[6][34], Gang_Create_Cost);
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], string, "Назад", "Отмена");
					return 1;
				}
				Dialog_Show(playerid, Dialog:GangCreate);
				return 1;
			}
			// принять приглашение
			case 2: {
				if (GetPVarInt(playerid, "GangInvite") == 0) {
					return 0;
				}
				
				gangid = GetPVarInt(playerid, "GangInvite");

				new string[MAX_STRING];

				if (GangOnlineCount(gangid) >= MAX_GANG_SIZE) {
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", lang_texts[6][46], "Назад", "Отмена");
					SetPVarInt(playerid, "GangInvite", 0);
					return 1;
				}

				if (GangJoinMember(gangid, playerid) == 1) {
					format(string, sizeof(string), lang_texts[6][47], Gangs[gangid][gang_name]);
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", string, "Назад", "Отмена");
					
					format(string, sizeof(string), lang_texts[6][48], ReturnPlayerName(playerid));
					SendGangMessage(gangid, string, COLOR_GANG);
					
					Log_Game("player: %d:  %s: have joined '%s' gang.", playerid, ReturnPlayerName(playerid), Gangs[gangid][gang_name]);
				} else {
					format(string, sizeof(string), lang_texts[6][49], Gangs[gangid][gang_name]);
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", string, "Назад", "Отмена");
				}
				return 1;
			}
		}
	} else {
		switch (listitem) {
			// информация о банде
			case 0: {
				new members[64 * MAX_GANG_SIZE];
				for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++) {
					if (strlen(GangMemberInfo[gangid][gangmember][member_name]) <= 0) {
						continue;
					}

					if (gang_IsPlayerConnect(gangid, gangmember)) {
						format(members, sizeof(members),
							"%s   {AFAFAF}%02d\t{33AA33}%s\t{AFAFAF}ID: {FFFFFF}%d\t{AFAFAF}Уровень: {FFFFFF}%d\n",
							members,
							gangmember,
							GangMemberInfo[gangid][gangmember][member_name],
							GangMemberInfo[gangid][gangmember][member_playerid],
							GetPlayerLevel(GangMemberInfo[gangid][gangmember][member_playerid])
						);
					} else {
						format(members, sizeof(members), "%s   {AFAFAF}%02d\t{AA3333}%s      \n",
							members, gangmember, GangMemberInfo[gangid][gangmember][member_name]
						);
					}
				}

				new string[MAX_STRING + sizeof(members)];
				format(string, sizeof(string), "\
					Убийства банды: %d\n\
					Деньги банды: $%d\n\
					Очки банды: %d\n\
					XP банды: %d\n\
					Уровень банды: %d\n\
					Ваших онлайн: %d\n\n\
					%s",
					Gangs[gangid][gang_kills], Gangs[gangid][gang_money], Gangs[gangid][gang_score],
					Gangs[gangid][gang_xp], Gangs[gangid][gang_level],
					GangOnlineCount(gangid), members
				);
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Информация о банде", string, "Назад", "Отмена");
				return 1;
			}
			// выйти из банды
			case 1: {
				if (gang_IsLeader(playerid)) {
					Dialog_Show(playerid, Dialog:GangExitAccept);
				} else {
					GangRemoveMember(gangid, ReturnPlayerName(playerid));
				}
				return 1;
			}
			// пригласить игрока
			case 2: {
				if (!gang_IsLeader(playerid)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangInvite);
				return 1;
			}
			// выгнать игрока
			case 3: {
				if (!gang_IsLeader(playerid)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangKick);
				return 1;
			}
			// изменить цвет
			case 4: {
				if (!gang_IsLeader(playerid)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangColor);
				return 1;
			}
			// изменить объявление
			case 5: {
				if (!gang_IsLeader(playerid)) {
					return 0;
				}
				
				Dialog_Show(playerid, Dialog:GangMotd);
				return 1;
			}
		}
	}
	return 1;
}

DialogCreate:GangExitAccept(playerid)
{
	Dialog_Open(playerid, Dialog:GangExitAccept, DIALOG_STYLE_MSGBOX,
		"Удалить банду?",
		"Если вы выйдете из банды, то банда будет безвозвратно удалена!\n\
		Вы проверили банк банды на наличие денег?\n\
		\n\
		Вы действительно хотите удалить банду?",
		"Удалить", "Назад"
	);
}

DialogResponse:GangExitAccept(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	GangRemoveMember(player_GetGangID(playerid), ReturnPlayerName(playerid));
	return 1;
}

DialogCreate:GangCreate(playerid)
{
	const
		COLORS_IN_STRING = 5;

	new
		string[(MAX_NAME + 8 + 2) * MAX_COLOR_COUNT + 2 * (MAX_COLOR_COUNT / COLORS_IN_STRING + 1) + 1],
		step = gangmenu_GetCreateStep(playerid);

	switch (step) {
		// имя
		case 0: {
			set(string, "Введите имя банды");
		}
		// цвет
		case 1: {
			set(string, "Введите цвет банды:\n\t");

			for (new i = 0; i < MAX_COLOR_COUNT; i++) {
				format(string, sizeof(string), "%s  {%s}%s", string, Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
				
				if ((i+1) % COLORS_IN_STRING == 0) {
					strcat(string, "\n\t", sizeof(string));
				}
			}
		}
	}

	Dialog_Open(playerid, Dialog:GangCreate, DIALOG_STYLE_INPUT, lang_texts[6][9], string, "ОК", "Назад");
}

stock gangmenu_SetCreateStep(playerid, step)
{
	SetPVarInt(playerid, "gangmenu_CreateStep", step);
}

stock gangmenu_GetCreateStep(playerid)
{
	return GetPVarInt(playerid, "gangmenu_CreateStep");
}

stock gangmenu_CleanCreateGarbage(playerid)
{
	DeletePVar(playerid, "gangmenu_CreateStep");
	DeletePVar(playerid, "gangmenu_GangName");
}

stock gangmenu_SetCreateName(playerid, gangname[])
{
	SetPVarString(playerid, "gangmenu_GangName", gangname);
}

stock gangmenu_GetCreateName(playerid, gangname[])
{
	GetPVarString(playerid, "gangmenu_GangName", gangname, MAX_NAME);
}

DialogResponse:GangCreate(playerid, response, listitem, inputtext[])
{
	switch (gangmenu_GetCreateStep(playerid)) {
		// имя
		case 0: {
			if (!response) {
				gangmenu_CleanCreateGarbage(playerid);
				Dialog_Show(playerid, Dialog:GangMenu);
				return 1;
			}
			
			if (GetPlayerMoney(playerid) < Gang_Create_Cost) {
				new string[MAX_STRING];
				format(string, sizeof(string), lang_texts[6][34], Gang_Create_Cost);
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], string, "Назад", "Отмена");
				return 1;
			}

			new len = strlen(inputtext);
			if (len == 0) {
				Dialog_MessageEx(playerid, Dialog:GangCreateReturn, lang_texts[6][9], lang_texts[6][12], "Назад", "Отмена");
				return 1;
			}
			
			if (len > MAX_GANG_NAME_LEN || len < MIN_GANG_NAME_LEN) {
				Dialog_MessageEx(playerid, Dialog:GangCreateReturn, lang_texts[6][9], lang_texts[6][39], "Назад", "Отмена");
				return 1;
			}
			
			if (!IsValidChars(inputtext) || !strcmp(inputtext, "Unknown", true)) {
				new string[MAX_STRING];
				format(string, sizeof(string), lang_texts[6][41], ALLOWED_STRING_SYMBOLS_STR);
				Dialog_MessageEx(playerid, Dialog:GangCreateReturn, lang_texts[6][9], string, "Назад", "Отмена");
				return 1;
			}
			
			gangmenu_SetCreateName(playerid, inputtext);
			gangmenu_SetCreateStep(playerid, 1);

			Dialog_Show(playerid, Dialog:GangCreate);
			return 1;
		}
		// цвет
		case 1: {
			if (!response) {
				gangmenu_CleanCreateGarbage(playerid);
				gangmenu_SetCreateStep(playerid, 0);
				Dialog_Show(playerid, Dialog:GangCreate);
				return 1;
			}

			new colorid = Color_GetIdByName(inputtext);
			if (colorid == -1) {
				Dialog_MessageEx(playerid, Dialog:GangCreateReturn, lang_texts[6][9], lang_texts[6][10], "Назад", "Отмена");
				return 1;
			}

			if (GetPlayerMoney(playerid) < Gang_Create_Cost) {
				new string[MAX_STRING];
				format(string, sizeof(string), lang_texts[6][34], Gang_Create_Cost);
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], string, "Назад", "Отмена");
				return 1;
			}

			new gangcolor = Color_GetCode(colorid);

			new gangname[MAX_NAME];
			gangmenu_GetCreateName(playerid, gangname);

			if (GangCreate(playerid, gangname, gangcolor) != 0) {
				GivePlayerMoney(playerid,-Gang_Create_Cost, 1);
				SendClientMessage(playerid, COLOR_GREEN, lang_texts[6][42]);
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], lang_texts[6][42], "Назад", "Отмена");
				Log_Game("player: %s(%d): created gang '%s' ", ReturnPlayerName(playerid), playerid, gangname);
			} else {
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], lang_texts[6][11], "Назад", "Отмена");
			}
			
			gangmenu_CleanCreateGarbage(playerid);
			return 1;
		}
	}
	return 1;
}

DialogResponse:GangCreateReturn(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:GangCreate);
	}
	return 1;
}

DialogCreate:GangInvite(playerid)
{
	Dialog_Open(playerid, Dialog:GangInvite, DIALOG_STYLE_INPUT,
		"Приглашение игрока",
		"Введите ID игрока, которого вы хотите пригласить",
		"ОК", "Назад"
	);
}

DialogResponse:GangInvite(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}
	
	new inviteid = strval(inputtext);
	if (strlen(inputtext) == 0 || !IsPlayerConnected(inviteid) || !player_IsLogin(inviteid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", lang_texts[6][55], "Назад", "Отмена");
		return 1;
	}
	
	if (player_GetGangID(inviteid) != 0) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", lang_texts[6][57], "Назад", "Отмена");
		return 1;
	}
	
	new gangid = player_GetGangID(playerid);
	SetPVarInt(inviteid, "GangInvite", gangid);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][58], ReturnPlayerName(playerid), Gangs[gangid][gang_name]);
	SendClientMessage(inviteid, COLOR_GANG, string);

	format(string, sizeof(string), lang_texts[6][59], ReturnPlayerName(inviteid), Gangs[gangid][gang_name]);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", string, "Назад", "Отмена");

	Log_Game("player: %d:  %s: has invited %s to join gang '%s'.", playerid, ReturnPlayerName(playerid), ReturnPlayerName(inviteid), Gangs[gangid][gang_name]);
	return 1;
}

DialogCreate:GangKick(playerid)
{
	new members[64 * MAX_GANG_SIZE],
		gangid = player_GetGangID(playerid);

	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++) {
		if (strlen(GangMemberInfo[gangid][gangmember][member_name]) <= 0) {
			continue;
		}

		if (gang_IsPlayerConnect(gangid, gangmember)) {
			format(members, sizeof(members),
				"%s{AFAFAF}%02d\t{33AA33}%s\t{AFAFAF}ID: {FFFFFF}%d\t{AFAFAF}Уровень: {FFFFFF}%d\n",
				members,
				gangmember,
				GangMemberInfo[gangid][gangmember][member_name],
				GangMemberInfo[gangid][gangmember][member_playerid],
				GetPlayerLevel(GangMemberInfo[gangid][gangmember][member_playerid])
			);
		} else {
			format(members, sizeof(members), "%s{AFAFAF}%02d\t{AA3333}%s\n",
				members, gangmember, GangMemberInfo[gangid][gangmember][member_name]
			);
		}
	}

	Dialog_Open(playerid, Dialog:GangKick, DIALOG_STYLE_LIST, "Выгнать игрока", members, "Выгнать", "Назад");
}

DialogResponse:GangKick(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	if (listitem == 0) {
		Dialog_Show(playerid, Dialog:GangKick);
		return 1;
	}

	new gangid = player_GetGangID(playerid);
	new memberid = gangmenu_GetMemberID(gangid, listitem);
	
	new kickplayername[MAX_PLAYER_NAME + 1];
	set(kickplayername, GangMemberInfo[gangid][memberid][member_name]);

	GangRemoveMember(gangid, kickplayername);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][62], kickplayername);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Выгнать игрока", string, "Назад", "Отмена");
	return 1;
}

stock gangmenu_GetMemberID(gangid, listitem)
{
	new id = 0;

	for (new gangmember = 0; gangmember < MAX_GANG_SIZE; gangmember++) {
		if (strlen(GangMemberInfo[gangid][gangmember][member_name]) <= 0) {
			continue;
		}

		if (id == listitem) {
			return gangmember;
		}

		id++;
	}
	return -1;
}

DialogCreate:GangColor(playerid)
{
	new string[MAX_STRING + MAX_NAME * MAX_COLOR_COUNT] = "Введите название цвета, который вы хотите установить для банды. Доступные:\n\t";

	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		format(string, sizeof(string), "%s  {%s}%s", string, Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
		
		if ((i+1) % 5 == 0) {
			strcat(string, "\n\t", sizeof(string));
		}
	}

	Dialog_Open(playerid, Dialog:GangColor, DIALOG_STYLE_INPUT, "Изменение цвета банды", string, "ОК", "Назад");
}

DialogResponse:GangColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}
	
	if (GetPlayerMoney(playerid) < Gang_Color_Cost) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][65], Gang_Color_Cost);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Изменение цвета", string, "Назад", "Отмена");
		return 1;
   	}

   	if (!strlen(inputtext)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Изменение цвета", "Неверный цвет.", "Назад", "Отмена");
		return 1;
	}

	new colorid = Color_GetIdByName(inputtext);
	if (colorid == -1) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Изменение цвета", "Неверный цвет.", "Назад", "Отмена");
		return 1;
	}

	new gangcolor = Color_GetCode(colorid);

   	SetGangColour(player_GetGangID(playerid), gangcolor);
   	GivePlayerMoney(playerid, -Gang_Color_Cost, 1);
   	
   	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][69], Gang_Color_Cost);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Изменение цвета", string, "Назад", "Отмена");
	return 1;
}

DialogCreate:GangMotd(playerid)
{
	new string[256];

	set(string, Gangs[ player_GetGangID(playerid) ][gang_motd]);

	if (strlen(string) == 0) {
		set(string, lang_texts[6][90]);
	} else {
		format(string, sizeof(string), lang_texts[6][89], string);
	}
	
	Dialog_Open(playerid, Dialog:GangMotd, DIALOG_STYLE_INPUT, lang_texts[6][86], string, "ОК", "Назад");
}

DialogResponse:GangMotd(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	new len = strlen(inputtext);
	if (len > MAX_GANG_MOTD_LEN) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][86], lang_texts[6][85], "Назад", "Отмена");
		return 1;
	}
	
	new string[MAX_STRING + MAX_GANG_MOTD_LEN],
		gangid = player_GetGangID(playerid);
	
	if (len == 0) {
		Gangs[gangid][gang_motd][0] = '\0';
		SendGangMessage(gangid, lang_texts[6][87], COLOR_GANG);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][86], lang_texts[6][87], "Назад", "Отмена");
		return 1;
	}

	set(Gangs[gangid][gang_motd], inputtext);

	format(string, sizeof(string), lang_texts[6][88], Gangs[gangid][gang_motd]);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][86], string, "Назад", "Отмена");
	
	format(string, sizeof(string), lang_texts[6][81], ReturnPlayerName(playerid), playerid, Gangs[gangid][gang_motd]);
	SendGangMessage(gangid, string, COLOR_GANG);
	return 1;
}

DialogResponse:GangReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:GangMenu);
	}
	return 1;
}
