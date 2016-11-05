/*

	About: gang user menu
	Author: ziggi

*/

#if defined _gang_menu_included
	#endinput
#endif

#define _gang_menu_included

/*
	Vars
*/

static
	gWorkingMemberID[MAX_PLAYERS];

/*
	Dialogs
*/

DialogCreate:GangMenu(playerid)
{
	static
		temp_str[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * 7];

	string[0] = '\0';

	if (!IsPlayerInGang(playerid)) {
		Lang_GetPlayerText(playerid, "GANG_MENU_LIST_INFO", string);

		Lang_GetPlayerText(playerid, "GANG_MENU_LIST_CREATE", temp_str, _, Gang_GetCreateCost());
		strcat(string, temp_str);

		if (IsPlayerInvitedInAnyGang(playerid)) {
			Lang_GetPlayerText(playerid, "GANG_MENU_LIST_ACCEPT", temp_str);
			strcat(string, temp_str);
		}
	} else {
		Lang_GetPlayerText(playerid, "GANG_MENU_LIST_INFO_SELF", temp_str);
		strcat(string, temp_str);

		Lang_GetPlayerText(playerid, "GANG_MENU_LIST_EXIT", temp_str);
		strcat(string, temp_str);

		new gangid = GetPlayerGangID(playerid);
		new memberid = GetPlayerGangMemberID(playerid);

		if (GangMember_IsHaveRank(gangid, memberid, GangMemberInviter)) {
			Lang_GetPlayerText(playerid, "GANG_MENU_LIST_INVITE", temp_str);
			strcat(string, temp_str);
		}

		if (GangMember_IsHaveRank(gangid, memberid, GangMemberHelper)) {
			Lang_GetPlayerText(playerid, "GANG_MENU_LIST_KICK", temp_str);
			strcat(string, temp_str);

			Lang_GetPlayerText(playerid, "GANG_MENU_LIST_MOTD", temp_str);
			strcat(string, temp_str);
		}

		if (GangMember_IsHaveRank(gangid, memberid, GangMemberLeader)) {
			Lang_GetPlayerText(playerid, "GANG_MENU_LIST_RANK", temp_str);
			strcat(string, temp_str);

			Lang_GetPlayerText(playerid, "GANG_MENU_LIST_COLOR", temp_str, _, Gang_GetColorCost());
			strcat(string, temp_str);
		}
	}

	Dialog_Open(playerid, Dialog:GangMenu, DIALOG_STYLE_LIST,
	            "GANG_MENU_HEADER",
	            string,
	            "BUTTON_OK", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:GangMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerMenu);
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);
	new memberid = GetPlayerGangMemberID(playerid);

	if (gangid == INVALID_GANG_ID) {
		switch (listitem) {
			// информаци€
			case 0: {
				Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				                 "GANG_MENU_INFO_HEADER",
				                 "GANG_MENU_INFO",
				                 "BUTTON_BACK", "BUTTON_CANCEL");
				return 1;
			}
			// создать банду
			case 1: {
				if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
					                 "GANG_CREATE_HEADER",
					                 "GANG_CREATE_MONEY_ERROR",
					                 "BUTTON_BACK", "BUTTON_CANCEL",
					                 MDIALOG_NOTVAR_NONE,
					                 Gang_GetCreateCost());
					return 1;
				}
				Dialog_Show(playerid, Dialog:GangCreateName);
				return 1;
			}
			// прин€ть приглашение
			case 2: {
				if (!IsPlayerInvitedInAnyGang(playerid)) {
					Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
					                 "GANG_CREATE_HEADER",
					                 "GANG_INVITE_NOT_INVITED",
					                 "BUTTON_BACK", "BUTTON_CANCEL");
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
					members[64 * MAX_GANG_SIZE];

				GetPlayerGangMemberListString(playerid, members);

				Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
				                 "GANG_MENU_INFO_HEADER_SELF",
				                 "GANG_MENU_INFO_MESSAGE",
				                 "BUTTON_BACK", "BUTTON_CANCEL",
				                 MDIALOG_NOTVAR_NONE,
				                 Gang_GetKills(gangid), FormatNumber(Gang_GetMoney(gangid)), Gang_GetScore(gangid),
				                 GetGangLevel(gangid), GetGangXP(gangid), GetXPToGangLevel(GetGangLevel(gangid) + 1),
				                 Gang_GetOnlineCount(gangid), Gang_GetExistsCount(gangid),
				                 members);
				return 1;
			}
			// выйти из банды
			case 1: {
				if (GangMember_IsHaveRank(gangid, memberid, GangMemberLeader)) {
					Dialog_Show(playerid, Dialog:GangExitAccept);
				} else {
					new
						player_name[MAX_PLAYER_NAME + 1],
						gang_name[MAX_GANG_NAME + 1];

					GetPlayerName(playerid, player_name, sizeof(player_name));
					Gang_GetName(gangid, gang_name);

					Gang_MemberRemove(gangid, memberid);

					Dialog_Message(playerid, "GANG_EXIT_HEADER", "GANG_EXIT_INFO", "BUTTON_OK");
					Gang_SendLangMessage(gangid, "GANG_MEMBER_LEAVE", _, player_name);
					Log(mainlog, INFO, "Action <GangMenu>: %s(%d) have leaved from '%s' gang.", player_name, playerid, gang_name);
				}
				return 1;
			}
			// пригласить игрока
			case 2: {
				if (!GangMember_IsHaveRank(gangid, memberid, GangMemberInviter)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangInvite);
				return 1;
			}
			// выгнать игрока
			case 3: {
				if (!GangMember_IsHaveRank(gangid, memberid, GangMemberHelper)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangKick);
				return 1;
			}
			// изменить объ€вление
			case 4: {
				if (!GangMember_IsHaveRank(gangid, memberid, GangMemberHelper)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangMotd);
				return 1;
			}
			// изменить звание
			case 5: {
				if (!GangMember_IsHaveRank(gangid, memberid, GangMemberLeader)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangRank);
				return 1;
			}
			// изменить цвет
			case 6: {
				if (!GangMember_IsHaveRank(gangid, memberid, GangMemberLeader)) {
					return 0;
				}

				Dialog_Show(playerid, Dialog:GangColor);
				return 1;
			}
		}
	}
	return 1;
}

DialogCreate:GangExitAccept(playerid)
{
	Dialog_Open(playerid, Dialog:GangExitAccept, DIALOG_STYLE_MSGBOX,
	            "GANG_REMOVE_HEADER",
	            "GANG_REMOVE_INFO",
	            "GANG_REMOVE_BUTTON", "BUTTON_BACK");
}

DialogResponse:GangExitAccept(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	new gangid = GetPlayerGangID(playerid);
	new is_ok = Gang_Remove(gangid);
	if (is_ok) {
		new
			player_name[MAX_PLAYER_NAME + 1],
			gang_name[MAX_GANG_NAME];

		GetPlayerName(playerid, player_name, sizeof(player_name));
		Gang_GetName(gangid, gang_name, sizeof(gang_name));

		Gang_SendLangMessage(gangid, "GANG_REMOVED", _, player_name, playerid, gang_name);
		Dialog_Message(playerid, "GANG_EXIT_HEADER", "GANG_EXIT_REMOVED", "BUTTON_OK",
		               MDIALOG_NOTVAR_NONE, gang_name);
	} else {
		Dialog_Message(playerid, "GANG_EXIT_HEADER", "GANG_EXIT_REMOVE_ERROR", "BUTTON_OK");
	}
	return 1;
}

DialogCreate:GangCreateName(playerid)
{
	Dialog_Open(playerid, Dialog:GangCreateName, DIALOG_STYLE_INPUT,
	            "GANG_CREATE_HEADER",
	            "GANG_CREATE_NAME_MESSAGE",
	            "BUTTON_OK", "BUTTON_BACK");
}

DialogResponse:GangCreateName(playerid, response, listitem, inputtext[])
{
	if (!response) {
		GangMenu_CleanCreateGarbage(playerid);
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_CREATE_HEADER",
		                 "GANG_CREATE_MONEY_ERROR",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 Gang_GetCreateCost());
		return 1;
	}

	new len = strlen(inputtext);
	if (len == 0) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn, "GANG_CREATE_HEADER", "GANG_CREATE_NAME_NOT_INPUT", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	if (len > MAX_GANG_NAME || len < MIN_GANG_NAME) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn,
		                 "GANG_CREATE_HEADER",
		                 "GANG_CREATE_NAME_ERROR",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 MIN_GANG_NAME, MAX_GANG_NAME);
		return 1;
	}

	if (!IsValidChars(inputtext) || !strcmp(inputtext, "Unknown", true)) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn,
		                 "GANG_CREATE_HEADER",
		                 "GANG_CREATE_NAME_SYMBOLS_ERROR",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_INFO,
		                 ALLOWED_STRING_SYMBOLS_STR);
		return 1;
	}

	GangMenu_SetCreateName(playerid, inputtext);

	Dialog_Show(playerid, Dialog:GangCreateColor);
	return 1;
}

DialogCreate:GangCreateColor(playerid)
{
	new string[(MAX_COLOR_NAME + 8 + 2 + 1) * MAX_COLOR_COUNT];

	for (new i = 1; i < MAX_COLOR_COUNT; i++) {
		format(string, sizeof(string), "%s{%s}%s\n", string, Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
	}

	Dialog_Open(playerid, Dialog:GangCreateColor, DIALOG_STYLE_LIST,
	            "GANG_CREATE_HEADER",
	            string,
	            "BUTTON_OK", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:GangCreateColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		GangMenu_CleanCreateGarbage(playerid);
		Dialog_Show(playerid, Dialog:GangCreateName);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetCreateCost()) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_CREATE_HEADER",
		                 "GANG_CREATE_MONEY_ERROR",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 Gang_GetCreateCost());
		return 1;
	}

	new colorid = Color_GetIdByName(inputtext);
	if (colorid == -1) {
		Dialog_MessageEx(playerid, Dialog:Gang_CreateReturn, "GANG_CREATE_HEADER", "GANG_COLOR_INCORRECT", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	new gangcolor = Color_GetCode(colorid);

	new gangname[MAX_NAME];
	GangMenu_GetCreateName(playerid, gangname);

	new gangid = Gang_Create(playerid, gangname, gangcolor);

	if (gangid != INVALID_GANG_ID) {
		GivePlayerMoney(playerid, -Gang_GetCreateCost());
		Lang_SendText(playerid, "GANG_CREATE_SUCCESS");
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_CREATE_HEADER", "GANG_CREATE_SUCCESS", "BUTTON_BACK", "BUTTON_CANCEL");
		Log(mainlog, INFO, "Action <GangCreateColor>: %s(%d) created gang '%s'.", ret_GetPlayerName(playerid), playerid, gangname);
	} else {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_CREATE_HEADER", "GANG_CREATE_ERROR", "BUTTON_BACK", "BUTTON_CANCEL");
	}

	GangMenu_CleanCreateGarbage(playerid);
	return 1;
}

stock GangMenu_CleanCreateGarbage(playerid)
{
	DeletePVar(playerid, "GangMenu_CreateStep");
	DeletePVar(playerid, "GangMenu_GangName");
}

stock GangMenu_SetCreateName(playerid, gangname[])
{
	SetPVarString(playerid, "GangMenu_GangName", gangname);
}

stock GangMenu_GetCreateName(playerid, gangname[], const size = sizeof(gangname))
{
	GetPVarString(playerid, "GangMenu_GangName", gangname, size);
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
	static
		string[(MAX_LANG_VALUE_STRING + 1) * (1 + MAX_GANG_INVITES) + 1],
		temp_str[MAX_LANG_VALUE_STRING + 1],
		invited_gangid[MAX_GANG_INVITES],
		remaining_times[MAX_GANG_INVITES],
		size;

	Lang_GetPlayerText(playerid, "GANG_INVITE_ACCEPT_LIST_HEADER", string);
	strcat(string, "\n");

	GetPlayerInvitedGangArrayInfo(playerid, invited_gangid, .remaining_times = remaining_times, .size = size);

	for (new i = 0; i < size; i++) {
		Gang_GetName(invited_gangid[i], temp_str);

		Lang_GetPlayerText(playerid, "GANG_INVITE_ACCEPT_LIST", temp_str, _, temp_str, remaining_times[i]);

		strcat(string, temp_str);
		strcat(string, "\n");
	}

	Dialog_Open(playerid, Dialog:GangInviteAccept, DIALOG_STYLE_TABLIST_HEADERS,
	            "GANG_INVITE_HEADER",
	            string,
	            "BUTTON_ACCEPT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:GangInviteAccept(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	new gangid = Gang_GetID(inputtext);

	if (!IsPlayerInvitedInGang(playerid, gangid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_INVITE_HEADER", "GANG_INVITE_TIME_SPENT", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	new
		player_name[MAX_PLAYER_NAME + 1],
		gang_name[MAX_GANG_NAME + 1];

	GetPlayerName(playerid, player_name, sizeof(player_name));
	Gang_GetName(gangid, gang_name);

	if (Gang_MemberJoin(gangid, playerid) == 1) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_INVITE_HEADER",
		                 "GANG_INVITE_YOU_IN",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 gang_name);

		Gang_SendLangMessage(gangid, "GANG_INVITE_PLAYER_IN", _, player_name);

		Log(mainlog, INFO, "Action <GangInviteAccept>: %s(%d) have joined '%s' gang.", player_name, playerid, gang_name);
	} else {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_INVITE_HEADER", "GANG_INVITE_ERROR", "BUTTON_BACK", "BUTTON_CANCEL", MDIALOG_NOTVAR_NONE, gang_name);
	}

	return 1;
}

DialogCreate:GangInvite(playerid)
{
	new gangid = GetPlayerGangID(playerid);

	if (Gang_IsGangFull(gangid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_INVITE_HEADER",
		                 "GANG_INVITE_IS_FULL",
		                 "BUTTON_OK", "");
	} else {
		Dialog_Open(playerid, Dialog:GangInvite, DIALOG_STYLE_INPUT,
		            "GANG_INVITE_HEADER",
		            "GANG_INVITE_INPUT",
		            "BUTTON_OK", "BUTTON_BACK");
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
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_INVITE_HEADER", "GANG_INVITE_PLAYER_ERROR", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	if (IsPlayerInGang(inviteid)) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_INVITE_HEADER", "GANG_INVITE_ALREADY_IN_GANG", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	new
		is_error,
		gangid = GetPlayerGangID(playerid);

	is_error = IsPlayerInvitedInGang(inviteid, gangid);
	if (is_error) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_INVITE_HEADER", "GANG_INVITE_ALREADY_INVITED", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	is_error = !SetPlayerInvitedGangID(inviteid, gangid, true);
	if (is_error) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_INVITE_HEADER", "GANG_INVITE_LIST_IS_FULL", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	new
		player_name[MAX_PLAYER_NAME + 1],
		invite_name[MAX_PLAYER_NAME + 1],
		gang_name[MAX_GANG_NAME + 1];

	GetPlayerName(playerid, player_name, sizeof(player_name));
	GetPlayerName(inviteid, invite_name, sizeof(invite_name));
	Gang_GetName(gangid, gang_name);

	Lang_SendText(inviteid, "GANG_INVITE_MESSAGE", player_name, gang_name);

	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
	                 "GANG_INVITE_HEADER",
	                 "GANG_INVITE_MESSAGE_SELF",
	                 "BUTTON_BACK", "BUTTON_CANCEL",
	                 MDIALOG_NOTVAR_NONE,
	                 invite_name, gang_name);

	Log(mainlog, INFO, "Action <GangInvite>: %s(%d) has invited %s(%d) to join gang '%s'.", player_name, playerid, invite_name, inviteid, gang_name);
	return 1;
}

DialogCreate:GangKick(playerid)
{
	new members[64 * MAX_GANG_SIZE];
	GetPlayerGangMemberListString(playerid, members);

	Dialog_Open(playerid, Dialog:GangKick, DIALOG_STYLE_LIST,
	            "GANG_KICK_HEADER",
	            members,
	            "GANG_KICK_BUTTON", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
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
	new memberid = GetGangMemberID(gangid, listitem);

	new member_name[MAX_PLAYER_NAME + 1];
	GangMember_GetName(gangid, memberid, member_name);

	Gang_MemberRemove(gangid, memberid);

	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
	                 "GANG_KICK_HEADER",
	                 "GANG_KICK_PLAYER_KICKED",
	                 "BUTTON_BACK", "BUTTON_CANCEL",
	                 MDIALOG_NOTVAR_NONE,
	                 member_name);
	return 1;
}

DialogCreate:GangRank(playerid)
{
	new members[64 * MAX_GANG_SIZE];
	GetPlayerGangMemberListString(playerid, members);

	Dialog_Open(playerid, Dialog:GangRank, DIALOG_STYLE_LIST,
	            "GANG_RANK_HEADER",
	            members,
	            "GANG_RANK_BUTTON", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:GangRank(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return;
	}

	new
		working_memberid,
		gangid;

	gangid = GetPlayerGangID(playerid);
	working_memberid = GetGangMemberID(gangid, listitem);

	if (GangMember_IsHaveRank(gangid, working_memberid, GangMemberLeader)) {
		Dialog_Show(playerid, Dialog:GangRank);
		return;
	}

	SetPlayerWorkingMemberID(playerid, working_memberid);

	Dialog_Show(playerid, Dialog:GangRankList);
}

DialogCreate:GangRankList(playerid)
{
	new
		temp_string[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * MAX_GANG_RANK_COUNT];

	Lang_GetPlayerText(playerid, "GANG_RANK_LEADER", temp_string);
	strcat(string, temp_string);
	strcat(string, "\n");

	Lang_GetPlayerText(playerid, "GANG_RANK_HELPER", temp_string);
	strcat(string, temp_string);
	strcat(string, "\n");

	Lang_GetPlayerText(playerid, "GANG_RANK_PAYMASTER", temp_string);
	strcat(string, temp_string);
	strcat(string, "\n");

	Lang_GetPlayerText(playerid, "GANG_RANK_INVITER", temp_string);
	strcat(string, temp_string);
	strcat(string, "\n");

	Lang_GetPlayerText(playerid, "GANG_RANK_SOLDIER", temp_string);
	strcat(string, temp_string);
	strcat(string, "\n");

	Dialog_Open(playerid, Dialog:GangRankList, DIALOG_STYLE_LIST,
	            "GANG_RANK_HEADER",
	            string,
	            "GANG_RANK_BUTTON", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:GangRankList(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangRank);
		return;
	}

	new
		GangMemberRank:new_rank,
		working_memberid,
		memberid,
		gangid;

	new_rank = GangMemberRank:listitem;
	working_memberid = GetPlayerWorkingMemberID(playerid);
	memberid = GetPlayerGangMemberID(playerid);
	gangid = GetPlayerGangID(playerid);

	if (new_rank == GangMemberLeader) {
		GangMember_SetRank(gangid, memberid, GangMemberHelper);
	}

	GangMember_SetRank(gangid, working_memberid, new_rank);

	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_RANK_HEADER", "GANG_RANK_CHANGED_MESSAGE", "BUTTON_BACK", "BUTTON_CANCEL");
}

DialogCreate:GangColor(playerid)
{
	new
		temp_str[MAX_LANG_VALUE_STRING],
		string[(MAX_COLOR_NAME + 8 + 2 + 1 + 1) * MAX_COLOR_COUNT];

	for (new i = 1; i < MAX_COLOR_COUNT; i++) {
		format(temp_str, sizeof(temp_str), "{%s}%s", Color_ReturnEmbeddingCode(i), Color_ReturnName(i));
		strcat(string, temp_str);
		strcat(string, "\n");
	}

	Dialog_Open(playerid, Dialog:GangColor, DIALOG_STYLE_LIST,
	            "GANG_COLOR_HEADER",
	            string,
	            "BUTTON_OK", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:GangColor(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	if (GetPlayerMoney(playerid) < Gang_GetColorCost()) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_COLOR_HEADER",
		                 "GANG_COLOR_MONEY_ERROR",
		                 "BUTTON_BACK", "BUTTON_CANCEL",
		                 MDIALOG_NOTVAR_NONE,
		                 Gang_GetColorCost());
		return 1;
	}

	if (strlen(inputtext) == 0) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_COLOR_HEADER",
		                 "GANG_COLOR_INCORRECT",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	new colorid = Color_GetIdByName(inputtext);
	if (colorid == -1) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
		                 "GANG_COLOR_HEADER",
		                 "GANG_COLOR_INCORRECT",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	new gangcolor = Color_GetCode(colorid);

	Gang_SetColor(GetPlayerGangID(playerid), gangcolor);
	GivePlayerMoney(playerid, -Gang_GetColorCost());

	Dialog_MessageEx(playerid, Dialog:GangReturnMenu,
	                 "GANG_COLOR_HEADER",
	                 "GANG_COLOR_SUCCESS",
	                 "BUTTON_BACK", "BUTTON_CANCEL");
	return 1;
}

DialogCreate:GangMotd(playerid)
{
	new
		gangid = GetPlayerGangID(playerid),
		motd[MAX_GANG_MOTD + MAX_LANG_VALUE_STRING];

	Gang_GetMotd(gangid, motd);

	if (strlen(motd) == 0) {
		Dialog_Open(playerid, Dialog:GangMotd, DIALOG_STYLE_INPUT,
		            "GANG_MOTD_HEADER",
		            "GANG_MOTD_MESSAGE",
		            "BUTTON_OK", "BUTTON_BACK");
	} else {
		Dialog_Open(playerid, Dialog:GangMotd, DIALOG_STYLE_INPUT,
		            "GANG_MOTD_HEADER",
		            "GANG_MOTD_MESSAGE_OLD",
		            "BUTTON_OK", "BUTTON_BACK",
		            MDIALOG_NOTVAR_NONE,
		            motd);
	}
}

DialogResponse:GangMotd(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:GangMenu);
		return 1;
	}

	new len = strlen(inputtext);
	if (len > MAX_GANG_MOTD) {
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_MOTD_HEADER", "GANG_MOTD_LENGTH_ERROR", "BUTTON_BACK", "BUTTON_CANCEL", MDIALOG_NOTVAR_NONE, MAX_GANG_MOTD);
		return 1;
	}

	new
		gangid = GetPlayerGangID(playerid);

	if (len == 0) {
		Gang_SetMotd(gangid, "");

		Gang_SendLangMessage(gangid, "GANG_MOTD_REMOVED");
		Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_MOTD_HEADER", "GANG_MOTD_REMOVED", "BUTTON_BACK", "BUTTON_CANCEL");
		return 1;
	}

	Gang_SetMotd(gangid, inputtext);

	Dialog_MessageEx(playerid, Dialog:GangReturnMenu, "GANG_MOTD_HEADER", "GANG_MOTD_CHANGED_MSG", "BUTTON_BACK", "BUTTON_CANCEL", MDIALOG_NOTVAR_NONE, inputtext);
	Gang_SendLangMessage(gangid, "GANG_MOTD_CHANGED", _, ret_GetPlayerName(playerid), playerid, inputtext);
	return 1;
}

DialogResponse:GangReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:GangMenu);
	}
	return 1;
}

/*
	GetGangMemberID
*/

static stock GetGangMemberID(gangid, listitem)
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

	return INVALID_MEMBER_ID;
}

/*
	GetGangMemberListString
*/

static stock GetPlayerGangMemberListString(playerid, members[], const size = sizeof(members))
{
	new
		gangid,
		Lang:lang;

	gangid = GetPlayerGangID(playerid);
	lang = Lang_GetPlayerLang(playerid);

	GetGangMemberListString(gangid, lang, members, size);
}

static stock GetGangMemberListString(gangid, Lang:lang, members[], const size = sizeof(members))
{
	new
		account_data[e_Account_Info],
		member_playerid,
		member_name[MAX_PLAYER_NAME + 1],
		member_rank_name[MAX_GANG_RANK_NAME],
		member_str[MAX_PLAYER_NAME + MAX_GANG_RANK_NAME + 64];

	for (new memberid = 0; memberid < MAX_GANG_SIZE; memberid++) {
		if (!GangMember_IsExists(gangid, memberid)) {
			continue;
		}

		GangMember_GetName(gangid, memberid, member_name);
		GangMember_GetRankName(gangid, memberid, lang, member_rank_name);

		if (GangMember_IsActive(gangid, memberid)) {
			member_playerid = GangMember_GetID(gangid, memberid);

			Lang_GetText(lang, "GANG_MENU_PLAYER_LIST_ITEM_ON", member_str, sizeof(member_str),
			             memberid,
			             member_name,
			             member_rank_name,
			             member_playerid,
			             GetPlayerLevel(member_playerid));

			strcat(members, member_str, size);
			strcat(members, "\n", size);
		} else {
			Account_LoadData(member_name, account_data);

			Lang_GetText(lang, "GANG_MENU_PLAYER_LIST_ITEM_OFF", member_str, sizeof(member_str),
			             memberid,
			             member_name,
			             member_rank_name,
			             timestamp_to_format_date(account_data[e_aLoginTime]));

			strcat(members, member_str, size);
			strcat(members, "\n", size);
		}
	}
}

/*
	Working member id
*/

static stock SetPlayerWorkingMemberID(playerid, memberid)
{
	gWorkingMemberID[playerid] = memberid;
}

static stock GetPlayerWorkingMemberID(playerid)
{
	return gWorkingMemberID[playerid];
}
