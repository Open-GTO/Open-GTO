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
	new
		temp_str[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * 6];
	
	if (!IsPlayerInGang(playerid)) {
		strcat(string, _(GANG_MENU_LIST_INFO), sizeof(string));

		format(temp_str, sizeof(temp_str), _(GANG_MENU_LIST_CREATE), Gang_GetCreateCost());
		strcat(string, temp_str, sizeof(string));

		if (IsPlayerInvitedInAnyGang(playerid)) {
			strcat(string, _(GANG_MENU_LIST_ACCEPT), sizeof(string));
		}
	} else {
		strcat(string, _(GANG_MENU_LIST_INFO_SELF), sizeof(string));
		strcat(string, _(GANG_MENU_LIST_EXIT), sizeof(string));

		if (Gang_IsPlayerLeader(playerid)) {
			strcat(string, _(GANG_MENU_LIST_INVITE), sizeof(string));
			strcat(string, _(GANG_MENU_LIST_KICK), sizeof(string));
		
			format(temp_str, sizeof(temp_str), _(GANG_MENU_LIST_COLOR), Gang_GetColorCost());
			strcat(string, temp_str, sizeof(string));

			strcat(string, _(GANG_MENU_LIST_MOTD), sizeof(string));
		}
	}
	
	Dialog_Open(playerid, Dialog:GangMenu, DIALOG_STYLE_LIST,
			_(GANG_MENU_HEADER),
			string,
			_(GANG_MENU_BUTTON_OK), _(GANG_MENU_BUTTON_BACK)
		);
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
			// информаци€
			case 0: {
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
						_(GANG_MENU_INFO_HEADER),
						_m(GANG_MENU_INFO),
						_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
					);
				return 1;
			}
			// создать банду
			case 1: {
				if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
					new string[MAX_STRING];
					format(string, sizeof(string), _(GANG_CREATE_MONEY_ERROR), Gang_GetCreateCost());
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
							_(GANG_CREATE_HEADER),
							string,
							_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
						);
					return 1;
				}
				Dialog_Show(playerid, Dialog:GangCreateName);
				return 1;
			}
			// прин€ть приглашение
			case 2: {
				if (!IsPlayerInvitedInAnyGang(playerid)) {
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
							_(GANG_CREATE_HEADER),
							_(GANG_INVITE_NOT_INVITED),
							_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
						);
					return 0;
				}
				
				Dialog_Show(playerid, Dialog:GangInviteAccept);
				return 1;
			}
		}
	} else {
		switch (listitem) {
			// информаци€ о банде
			case 0: {
				new
					member_playerid,
					member_name[MAX_PLAYER_NAME + 1],
					member_rank_name[MAX_GANG_RANK_NAME],
					members[64 * MAX_GANG_SIZE];

				for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
					if (!GangMember_IsExists(gangid, memberid)) {
						continue;
					}

					GangMember_GetName(gangid, memberid, member_name);
					GangMember_GetRankName(gangid, memberid, member_rank_name);

					if (GangMember_IsActive(gangid, memberid)) {
						member_playerid = GangMember_GetID(gangid, memberid);

						format(members, sizeof(members),
								_(GANG_MENU_PLAYER_LIST_ITEM_ON),
								members,
								memberid,
								member_name,
								member_rank_name,
								member_playerid,
								GetPlayerLevel(member_playerid)
							);
					} else {
						format(members, sizeof(members), _(GANG_MENU_PLAYER_LIST_ITEM_OFF),
								members,
								memberid,
								member_name,
								member_rank_name
							);
					}
				}

				new
					gang_money[16],
					string[MAX_STRING + sizeof(members)];

				InsertSpacesInInt(Gang_GetMoney(gangid), gang_money);

				format(string, sizeof(string),
						_m(GANG_MENU_INFO_MESSAGE),
						Gang_GetKills(gangid), gang_money, Gang_GetScore(gangid),
						GetGangLevel(gangid), GetGangXP(gangid), GetXPToGangLevel(GetGangLevel(gangid) + 1),
						Gang_GetOnlineCount(gangid), Gang_GetExistsCount(gangid),
						members
					);
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
						_(GANG_MENU_INFO_HEADER_SELF),
						string,
						_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
					);
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
			// изменить объ€вление
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
		_(GANG_REMOVE_HEADER),
		_m(GANG_REMOVE_INFO),
		_(GANG_REMOVE_BUTTON), _(GANG_MENU_BUTTON_BACK)
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

	format(string, sizeof(string), _(GANG_REMOVED), string, playerid, gang_name);
	Gang_SendMessage(gangid, string);

	new is_ok = Gang_Remove(gangid);
	if (is_ok) {
		format(string, sizeof(string), _(GANG_EXIT_REMOVED), gang_name);
		Dialog_Message(playerid, _(GANG_EXIT_HEADER), string, _(GANG_MENU_BUTTON_OK));
	} else {
		Dialog_Message(playerid, _(GANG_EXIT_HEADER), _(GANG_EXIT_REMOVE_ERROR), _(GANG_MENU_BUTTON_OK));
	}

	return 1;
}

DialogCreate:GangCreateName(playerid)
{
	Dialog_Open(playerid, Dialog:GangCreateName, DIALOG_STYLE_INPUT,
			_(GANG_CREATE_HEADER),
			_(GANG_CREATE_NAME_MESSAGE),
			_(GANG_MENU_BUTTON_OK), _(GANG_MENU_BUTTON_BACK)
		);
}

DialogResponse:GangCreateName(playerid, response, listitem, inputtext[])
{
	if (!response) {
		gangmenu_CleanCreateGarbage(playerid);
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}
	
	if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(GANG_CREATE_MONEY_ERROR), Gang_GetCreateCost());
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_CREATE_HEADER),
				string,
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}

	new len = strlen(inputtext);
	if (len == 0) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn,
				_(GANG_CREATE_HEADER),
				_(GANG_CREATE_NAME_NOT_INPUT),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	if (len > MAX_GANG_NAME || len < MIN_GANG_NAME) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(GANG_CREATE_NAME_ERROR), MIN_GANG_NAME, MAX_GANG_NAME);
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn,
				_(GANG_CREATE_HEADER),
				string,
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	if (!IsValidChars(inputtext) || !strcmp(inputtext, "Unknown", true)) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(GANG_CREATE_NAME_SYMBOLS_ERROR), ALLOWED_STRING_SYMBOLS_STR);
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn,
				_(GANG_CREATE_HEADER),
				string,
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	gangmenu_SetCreateName(playerid, inputtext);

	Dialog_Show(playerid, Dialog:GangCreateColor);
	return 1;
}

DialogCreate:GangCreateColor(playerid)
{
	new string[(MAX_COLOR_NAME + 8 + 2 + 1) * MAX_COLOR_COUNT];

	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		format(string, sizeof(string), "%s{%s}%s\n", string, Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
	}

	Dialog_Open(playerid, Dialog:GangCreateColor, DIALOG_STYLE_LIST, _(GANG_CREATE_HEADER), string, _(GANG_MENU_BUTTON_OK), _(GANG_MENU_BUTTON_BACK));
}

DialogResponse:GangCreateColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		gangmenu_CleanCreateGarbage(playerid);
		Dialog_Show(playerid, Dialog:GangCreateName);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(GANG_CREATE_MONEY_ERROR), Gang_GetCreateCost());
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_CREATE_HEADER),
				string,
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}

	new colorid = Color_GetIdByName(inputtext);
	if (colorid == -1) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn,
				_(GANG_CREATE_HEADER),
				_(GANG_COLOR_INCORRECT),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}

	new gangcolor = Color_GetCode(colorid);

	new gangname[MAX_NAME];
	gangmenu_GetCreateName(playerid, gangname);

	new gangid = Gang_Create(playerid, gangname, gangcolor);

	if (gangid != INVALID_GANG_ID) {
		GivePlayerMoney(playerid, -Gang_GetCreateCost(), 1);
		SendClientMessage(playerid, COLOR_GREEN, _(GANG_CREATE_SUCCESS));
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_CREATE_HEADER),
				_(GANG_CREATE_SUCCESS),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		Log_Game("player: %s(%d): created gang '%s' ", ReturnPlayerName(playerid), playerid, gangname);
	} else {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_CREATE_HEADER),
				_(GANG_CREATE_ERROR),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
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
		Dialog_Show(playerid, Dialog:GangCreateName);
	}
	return 1;
}

DialogCreate:GangInviteAccept(playerid)
{
	new
		string[(MAX_GANG_NAME + 1) * MAX_GANG_INVITES],
		gang_name[MAX_GANG_NAME + 1],
		invited_gang[MAX_GANG_INVITES],
		size;

	GetPlayerInvitedGangID(playerid, invited_gang, size);

	for (new i = 0; i < size; i++) {
		Gang_GetName(invited_gang[i], gang_name);

		strcat(string, gang_name);
		strcat(string, "\n");
	}

	Dialog_Open(playerid, Dialog:GangInvite, DIALOG_STYLE_INPUT,
		_(GANG_INVITE_HEADER),
		string,
		_(GANG_MENU_BUTTON_ACCEPT), _(GANG_MENU_BUTTON_BACK)
	);
}

DialogResponse:GangInviteAccept(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	new
		invited_gang[MAX_GANG_INVITES],
		size;

	GetPlayerInvitedGangID(playerid, invited_gang, size);

	new gangid = invited_gang[listitem];

	if (Gang_IsGangFull(gangid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				_(GANG_INVITE_IS_FULL),
				_(GANG_MENU_INVITE_BUTTON_BACK), _(GANG_MENU_INVITE_BUTTON_CANCEL)
			);

		SetPlayerInvitedGangID(playerid, gangid, false);
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		player_name[MAX_PLAYER_NAME + 1],
		gang_name[MAX_GANG_NAME + 1];

	GetPlayerName(playerid, player_name, sizeof(player_name));
	Gang_GetName(gangid, gang_name);

	if (Gang_MemberJoin(gangid, playerid) == 1) {
		format(string, sizeof(string), _(GANG_INVITE_YOU_IN), gang_name);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				string,
				_(GANG_MENU_INVITE_BUTTON_BACK), _(GANG_MENU_INVITE_BUTTON_CANCEL)
			);
		
		format(string, sizeof(string), _(GANG_INVITE_PLAYER_IN), player_name);
		Gang_SendMessage(gangid, string, COLOR_GANG);
		
		Log_Game("Action <GangInviteAccept>: %s(%d) have joined '%s' gang.", player_name, playerid, gang_name);
	} else {
		format(string, sizeof(string), _(GANG_INVITE_ERROR), gang_name);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				string,
				_(GANG_MENU_INVITE_BUTTON_BACK), _(GANG_MENU_INVITE_BUTTON_CANCEL)
			);
	}

	return 1;
}

DialogCreate:GangInvite(playerid)
{
	new gangid = GetPlayerGangID(playerid);

	if (Gang_IsGangFull(gangid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				_(GANG_INVITE_IS_FULL),
				_(GANG_MENU_BUTTON_OK), ""
			);
	} else {
		Dialog_Open(playerid, Dialog:GangInvite, DIALOG_STYLE_INPUT,
				_(GANG_INVITE_HEADER),
				_(GANG_INVITE_INPUT),
				_(GANG_MENU_BUTTON_OK), _(GANG_MENU_BUTTON_BACK)
			);
	}
}

DialogResponse:GangInvite(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}
	
	new inviteid;

	if (sscanf(inputtext, "u", inviteid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				_(GANG_INVITE_PLAYER_ERROR),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	if (IsPlayerInGang(inviteid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				_(GANG_INVITE_ALREADY_IN_GANG),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	new gangid = GetPlayerGangID(playerid);

	new is_invited = SetPlayerInvitedGangID(inviteid, gangid, true);

	if (!is_invited) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_INVITE_HEADER),
				_(GANG_INVITE_LIST_IS_FULL),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	new
		player_name[MAX_PLAYER_NAME + 1],
		invite_name[MAX_PLAYER_NAME + 1],
		gang_name[MAX_GANG_NAME + 1],
		string[MAX_STRING];

	GetPlayerName(playerid, player_name, sizeof(player_name));
	GetPlayerName(inviteid, invite_name, sizeof(invite_name));
	Gang_GetName(gangid, gang_name);

	format(string, sizeof(string), _(GANG_INVITE_MESSAGE), player_name, gang_name);
	SendClientMessage(inviteid, COLOR_GANG, string);

	format(string, sizeof(string), _(GANG_INVITE_MESSAGE_SELF), invite_name, gang_name);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
			_(GANG_INVITE_HEADER),
			string,
			_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
		);

	Log_Game("Action <GangInvite>: %s(%d) has invited %s(%d) to join gang '%s'.",
			player_name, playerid, invite_name, inviteid, gang_name
		);
	return 1;
}

DialogCreate:GangKick(playerid)
{
	new
		member_playerid,
		member_name[MAX_PLAYER_NAME + 1],
		member_rank_name[MAX_GANG_RANK_NAME],
		members[64 * MAX_GANG_SIZE],
		gangid = GetPlayerGangID(playerid);

	for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
		if (!GangMember_IsExists(gangid, memberid)) {
			continue;
		}

		GangMember_GetName(gangid, memberid, member_name);
		GangMember_GetRankName(gangid, memberid, member_rank_name);

		if (GangMember_IsActive(gangid, memberid)) {
			member_playerid = GangMember_GetID(gangid, memberid);

			format(members, sizeof(members),
					_(GANG_MENU_PLAYER_LIST_ITEM_ON),
					members,
					memberid,
					member_name,
					member_rank_name,
					member_playerid,
					GetPlayerLevel(member_playerid)
				);
		} else {
			format(members, sizeof(members), _(GANG_MENU_PLAYER_LIST_ITEM_OFF),
					members,
					memberid,
					member_name,
					member_rank_name
				);
		}
	}

	Dialog_Open(playerid, Dialog:GangKick, DIALOG_STYLE_LIST,
			_(GANG_KICK_HEADER),
			members,
			_(GANG_KICK_BUTTON), _(GANG_MENU_BUTTON_BACK)
		);
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
	
	new string[MAX_LANG_VALUE_STRING];
	format(string, sizeof(string), _(GANG_KICK_PLAYER_KICKED), member_name);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
			_(GANG_KICK_HEADER),
			string,
			_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
		);
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
	new
		temp_str[MAX_LANG_VALUE_STRING],
		string[(MAX_COLOR_NAME + 8 + 2 + 1 + 1) * MAX_COLOR_COUNT];

	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		format(temp_str, sizeof(temp_str), "{%s}%s", Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
		strcat(string, temp_str, sizeof(string));
		strcat(string, "\n", sizeof(string));
	}

	Dialog_Open(playerid, Dialog:GangColor, DIALOG_STYLE_LIST,
			_(GANG_COLOR_HEADER),
			string,
			_(GANG_MENU_BUTTON_OK), _(GANG_MENU_BUTTON_BACK)
		);
}

DialogResponse:GangColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetColorCost()) {
		new string[MAX_STRING];
		format(string, sizeof(string), _(GANG_COLOR_MONEY_ERROR), Gang_GetColorCost());
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_COLOR_HEADER),
				string,
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}

	if (strlen(inputtext) == 0) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_COLOR_HEADER),
				_(GANG_COLOR_INCORRECT),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}

	new colorid = Color_GetIdByName(inputtext);
	if (colorid == -1) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_COLOR_HEADER),
				_(GANG_COLOR_INCORRECT),
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}

	new gangcolor = Color_GetCode(colorid);

	Gang_SetColor(GetPlayerGangID(playerid), gangcolor);
	GivePlayerMoney(playerid, -Gang_GetColorCost(), 1);

	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
			_(GANG_COLOR_HEADER),
			_(GANG_COLOR_SUCCESS),
			_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
		);
	return 1;
}

DialogCreate:GangMotd(playerid)
{
	new
		gangid = GetPlayerGangID(playerid),
		string[MAX_LANG_STRING + MAX_GANG_MOTD];

	Gang_GetMotd(gangid, string);

	if (strlen(string) == 0) {
		strcat(string, _(GANG_MOTD_MESSAGE));
	} else {
		format(string, sizeof(string), _(GANG_MOTD_MESSAGE_OLD), string);
	}
	
	Dialog_Open(playerid, Dialog:GangMotd, DIALOG_STYLE_INPUT,
			_(GANG_MOTD_HEADER),
			string,
			_(GANG_MENU_BUTTON_OK), _(GANG_MENU_BUTTON_BACK)
		);
}

DialogResponse:GangMotd(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	new string[MAX_LANG_STRING + MAX_GANG_MOTD];

	new len = strlen(inputtext);
	if (len > MAX_GANG_MOTD) {
		format(string, sizeof(string), _(GANG_MOTD_LENGTH_ERROR), MAX_GANG_MOTD);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				_(GANG_MOTD_HEADER),
				string,
				_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
			);
		return 1;
	}
	
	new
		gangid = GetPlayerGangID(playerid);
	
	if (len == 0) {
		Gang_SetMotd(gangid, "");

		Gang_SendMessage(gangid, _(GANG_MOTD_REMOVED), COLOR_GANG);
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, _(GANG_MOTD_HEADER), _(GANG_MOTD_REMOVED), _(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL));
		return 1;
	}

	Gang_SetMotd(gangid, inputtext);

	format(string, sizeof(string), _(GANG_MOTD_CHANGED_MSG), inputtext);
	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
			_(GANG_MOTD_HEADER),
			string,
			_(GANG_MENU_BUTTON_BACK), _(GANG_MENU_BUTTON_CANCEL)
		);
	
	format(string, sizeof(string), _(GANG_MOTD_CHANGED), ReturnPlayerName(playerid), playerid, inputtext);
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
