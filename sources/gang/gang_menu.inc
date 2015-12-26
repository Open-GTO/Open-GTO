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
	new string[MAX_STRING * 6];
	
	if (!IsPlayerInGang(playerid)) {
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
		if (Gang_IsPlayerLeader(playerid)) {
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

	new gangid = GetPlayerGangID(playerid);
	if (gangid == INVALID_GANG_ID) {
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
				if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
					new string[MAX_STRING];
					format(string, sizeof(string), lang_texts[6][34], Gang_GetCreateCost());
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], string, "Назад", "Отмена");
					return 1;
				}
				Dialog_Show(playerid, Dialog:Gang_CreateName);
				return 1;
			}
			// принять приглашение
			case 2: {
				/*
				// TODO
				SetPlayerInvitedGangID(playerid, gangid, false);
				IsPlayerInvitedInGang(playerid, gangid)
				new
					invited_gGang[MAX_GANG_INVITES],
					size;

				GetPlayerInvitedGangID(playerid, invited_gang, size);*/

				if (GetPVarInt(playerid, "GangInvite") == 0) {
					return 0;
				}
				
				gangid = GetPVarInt(playerid, "GangInvite");

				new string[MAX_STRING];

				if (Gang_GetOnlineCount(gangid) >= MAX_GANG_SIZE) {
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", lang_texts[6][46], "Назад", "Отмена");
					SetPVarInt(playerid, "GangInvite", 0);
					return 1;
				}

				new
					player_name[MAX_PLAYER_NAME + 1],
					gang_name[MAX_GANG_NAME + 1];

				GetPlayerName(playerid, player_name, sizeof(player_name));
				Gang_GetName(gangid, gang_name);

				if (Gang_MemberJoin(gangid, playerid) == 1) {
					format(string, sizeof(string), lang_texts[6][47], gang_name);
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", string, "Назад", "Отмена");
					
					format(string, sizeof(string), lang_texts[6][48], player_name);
					Gang_SendMessage(gangid, string, COLOR_GANG);
					
					Log_Game("player: %d:  %s: have joined '%s' gang.", playerid, player_name, gang_name);
				} else {
					format(string, sizeof(string), lang_texts[6][49], gang_name);
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", string, "Назад", "Отмена");
				}
				return 1;
			}
		}
	} else {
		switch (listitem) {
			// информация о банде
			case 0: {
				new
					member_playerid,
					member_name[MAX_PLAYER_NAME + 1],
					members[64 * MAX_GANG_SIZE];

				for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
					if (!GangMember_IsExists(gangid, memberid)) {
						continue;
					}

					GangMember_GetName(gangid, memberid, member_name);

					if (GangMember_IsActive(gangid, memberid)) {
						member_playerid = GangMember_GetID(gangid, memberid);

						format(members, sizeof(members),
							"%s   {AFAFAF}%02d\t{33AA33}%s\t{AFAFAF}ID: {FFFFFF}%d\t{AFAFAF}Уровень: {FFFFFF}%d\n",
							members,
							memberid,
							member_name,
							member_playerid,
							GetPlayerLevel(member_playerid)
						);
					} else {
						format(members, sizeof(members), "%s   {AFAFAF}%02d\t{AA3333}%s      \n",
							members, memberid, member_name
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
					Gang_GetKills(gangid), Gang_GetMoney(gangid), Gang_GetScore(gangid),
					Gang_GetXP(gangid), Gang_GetLevel(gangid),
					Gang_GetOnlineCount(gangid), members
				);
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Информация о банде", string, "Назад", "Отмена");
				return 1;
			}
			// выйти из банды
			case 1: {
				if (Gang_IsPlayerLeader(playerid)) {
					Dialog_Show(playerid, Dialog:GangExitAccept);
				} else {
					Gang_MemberRemove(gangid, GetPlayerGangMemberID(playerid));
				}
				return 1;
			}
			// пригласить игрока
			case 2: {
				if (!Gang_IsPlayerLeader(playerid)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangInvite);
				return 1;
			}
			// выгнать игрока
			case 3: {
				if (!Gang_IsPlayerLeader(playerid)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangKick);
				return 1;
			}
			// изменить цвет
			case 4: {
				if (!Gang_IsPlayerLeader(playerid)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangColor);
				return 1;
			}
			// изменить объявление
			case 5: {
				if (!Gang_IsPlayerLeader(playerid)) {
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

	new
		string[MAX_LANG_VALUE_STRING],
		gang_name[MAX_GANG_NAME],
		gangid;

	GetPlayerName(playerid, string, MAX_PLAYER_NAME + 1);
	gangid = GetPlayerGangID(playerid);
	Gang_GetName(gangid, gang_name, sizeof(gang_name));

	format(string, sizeof(string), "Лидер банды %s(%d) удалил банду '%s'.", string, playerid, gang_name);
	Gang_SendMessage(gangid, string);

	new is_ok = Gang_Remove(gangid);
	if (is_ok) {
		format(string, sizeof(string), "Вы удалили банду '%s'!", gang_name);
		Dialog_Message(playerid, "Банда", string, "ОК");
	} else {
		Dialog_Message(playerid, "Банда", "Ошибка. Что-то пошло не так...", "ОК");
	}

	return 1;
}

DialogCreate:Gang_CreateName(playerid)
{
	Dialog_Open(playerid, Dialog:Gang_CreateName, DIALOG_STYLE_INPUT, lang_texts[6][9], "Введите имя банды", "ОК", "Назад");
}

DialogResponse:Gang_CreateName(playerid, response, listitem, inputtext[])
{
	if (!response) {
		gangmenu_CleanCreateGarbage(playerid);
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}
	
	if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][34], Gang_GetCreateCost());
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], string, "Назад", "Отмена");
		return 1;
	}

	new len = strlen(inputtext);
	if (len == 0) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn, lang_texts[6][9], lang_texts[6][12], "Назад", "Отмена");
		return 1;
	}
	
	if (len > MAX_GANG_NAME || len < MIN_GANG_NAME) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn, lang_texts[6][9], lang_texts[6][39], "Назад", "Отмена");
		return 1;
	}
	
	if (!IsValidChars(inputtext) || !strcmp(inputtext, "Unknown", true)) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][41], ALLOWED_STRING_SYMBOLS_STR);
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn, lang_texts[6][9], string, "Назад", "Отмена");
		return 1;
	}
	
	gangmenu_SetCreateName(playerid, inputtext);

	Dialog_Show(playerid, Dialog:Gang_CreateColor);
	return 1;
}

DialogCreate:Gang_CreateColor(playerid)
{
	new string[(MAX_COLOR_NAME + 8 + 2 + 1) * MAX_COLOR_COUNT];

	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		format(string, sizeof(string), "%s{%s}%s\n", string, Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
	}

	Dialog_Open(playerid, Dialog:Gang_CreateColor, DIALOG_STYLE_LIST, lang_texts[6][9], string, "ОК", "Назад");
}

DialogResponse:Gang_CreateColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		gangmenu_CleanCreateGarbage(playerid);
		Dialog_Show(playerid, Dialog:Gang_CreateName);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][34], Gang_GetCreateCost());
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], string, "Назад", "Отмена");
		return 1;
	}

	new colorid = Color_GetIdByName(inputtext);
	if (colorid == -1) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn, lang_texts[6][9], lang_texts[6][10], "Назад", "Отмена");
		return 1;
	}

	new gangcolor = Color_GetCode(colorid);

	new gangname[MAX_NAME];
	gangmenu_GetCreateName(playerid, gangname);

	new gangid = Gang_Create(playerid, gangname, gangcolor);

	if (gangid != INVALID_GANG_ID) {
		GivePlayerMoney(playerid, -Gang_GetCreateCost(), 1);
		SendClientMessage(playerid, COLOR_GREEN, lang_texts[6][42]);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], lang_texts[6][42], "Назад", "Отмена");
		Log_Game("player: %s(%d): created gang '%s' ", ReturnPlayerName(playerid), playerid, gangname);
	} else {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][9], lang_texts[6][11], "Назад", "Отмена");
	}
	
	gangmenu_CleanCreateGarbage(playerid);
	return 1;
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

DialogResponse:Gang_CreateReturn(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:Gang_CreateName);
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
	
	if (GetPlayerGangID(inviteid) != INVALID_GANG_ID) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", lang_texts[6][57], "Назад", "Отмена");
		return 1;
	}
	
	new gangid = GetPlayerGangID(playerid);
	new is_invited = SetPlayerInvitedGangID(playerid, gangid, true);

	if (!is_invited) {
		// TODO
		return 0;
	}
	
	new
		player_name[MAX_PLAYER_NAME + 1],
		invite_name[MAX_PLAYER_NAME + 1],
		gang_name[MAX_GANG_NAME + 1],
		string[MAX_STRING];

	GetPlayerName(playerid, player_name, sizeof(player_name));
	GetPlayerName(inviteid, invite_name, sizeof(invite_name));
	Gang_GetName(gangid, gang_name);

	format(string, sizeof(string), lang_texts[6][58], player_name, gang_name);
	SendClientMessage(inviteid, COLOR_GANG, string);

	format(string, sizeof(string), lang_texts[6][59], invite_name, gang_name);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Приглашение в банду", string, "Назад", "Отмена");

	Log_Game("player: %d:  %s: has invited %s to join gang '%s'.", playerid, player_name, invite_name, gang_name);
	return 1;
}

DialogCreate:GangKick(playerid)
{
	new
		member_playerid,
		member_name[MAX_PLAYER_NAME + 1],
		members[64 * MAX_GANG_SIZE],
		gangid = GetPlayerGangID(playerid);

	for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
		if (!GangMember_IsExists(gangid, memberid)) {
			continue;
		}

		GangMember_GetName(gangid, memberid, member_name);

		if (GangMember_IsActive(gangid, memberid)) {
			member_playerid = GangMember_GetID(gangid, memberid);

			format(members, sizeof(members),
				"%s{AFAFAF}%02d\t{33AA33}%s\t{AFAFAF}ID: {FFFFFF}%d\t{AFAFAF}Уровень: {FFFFFF}%d\n",
				members,
				memberid,
				member_name,
				member_playerid,
				GetPlayerLevel(member_playerid)
			);
		} else {
			format(members, sizeof(members), "%s{AFAFAF}%02d\t{AA3333}%s\n",
				members, memberid, member_name
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

	new gangid = GetPlayerGangID(playerid);
	new memberid = gangmenu_GetMemberID(gangid, listitem);

	new member_name[MAX_PLAYER_NAME + 1];
	GangMember_GetName(gangid, memberid, member_name);

	Gang_MemberRemove(gangid, memberid);
	
	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][62], member_name);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Выгнать игрока", string, "Назад", "Отмена");
	return 1;
}

stock gangmenu_GetMemberID(gangid, listitem)
{
	new id = 0;

	for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
		if (!GangMember_IsExists(gangid, memberid)) {
			continue;
		}

		if (id == listitem) {
			return memberid;
		}

		id++;
	}

	return -1;
}

DialogCreate:GangColor(playerid)
{
	new string[(MAX_COLOR_NAME + 8 + 2 + 1) * MAX_COLOR_COUNT];

	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		format(string, sizeof(string), "%s{%s}%s\n", string, Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
	}

	Dialog_Open(playerid, Dialog:GangColor, DIALOG_STYLE_LIST, "Изменение цвета банды", string, "ОК", "Назад");
}

DialogResponse:GangColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetColorCost()) {
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[6][65], Gang_GetColorCost());
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

   	Gang_SetColor(GetPlayerGangID(playerid), gangcolor);
   	GivePlayerMoney(playerid, -Gang_GetColorCost(), 1);
   	
   	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[6][69], Gang_GetColorCost());
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "Изменение цвета", string, "Назад", "Отмена");
	return 1;
}

DialogCreate:GangMotd(playerid)
{
	new
		gangid = GetPlayerGangID(playerid),
		string[128];

	Gang_GetMotd(gangid, string);

	if (strlen(string) == 0) {
		strcat(string, lang_texts[6][90]);
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
	if (len > MAX_GANG_MOTD) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][86], lang_texts[6][85], "Назад", "Отмена");
		return 1;
	}
	
	new string[MAX_STRING + MAX_GANG_MOTD],
		gangid = GetPlayerGangID(playerid);
	
	if (len == 0) {
		Gang_SetMotd(gangid, "");

		Gang_SendMessage(gangid, lang_texts[6][87], COLOR_GANG);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][86], lang_texts[6][87], "Назад", "Отмена");
		return 1;
	}

	Gang_SetMotd(gangid, inputtext);

	format(string, sizeof(string), lang_texts[6][88], inputtext);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, lang_texts[6][86], string, "Назад", "Отмена");
	
	format(string, sizeof(string), lang_texts[6][81], ReturnPlayerName(playerid), playerid, inputtext);
	Gang_SendMessage(gangid, string, COLOR_GANG);
	return 1;
}

DialogResponse:GangReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:GangMenu);
	}
	return 1;
}
