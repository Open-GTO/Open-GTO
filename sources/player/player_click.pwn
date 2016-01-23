/*

	Title: player click system
	Created: 14.01.2014
	Author: ziggi

*/

PlayerClick_OnGameModeInit()
{
	Click_AddItem(DIALOG_STYLE_INPUT,
	              _(CLICK_SENDCASH_DIALOG_HEADER),
	              _(CLICK_SENDCASH_DIALOG_MESSAGE),
	              _(CLICK_SENDCASH_DIALOG_BUTTON_SEND), _(CLICK_SENDCASH_DIALOG_BUTTON_BACK),
	              PlayerPrivilegePlayer,
	              "pl_click_SendCash");

	Click_AddItem(DIALOG_STYLE_INPUT,
	              _(CLICK_PM_DIALOG_HEADER),
	              _(CLICK_PM_DIALOG_MESSAGE),
	              _(CLICK_PM_DIALOG_BUTTON_SEND), _(CLICK_PM_DIALOG_BUTTON_BACK),
	              PlayerPrivilegePlayer,
	              "pl_click_SendMessage");

	Click_AddItem(DIALOG_STYLE_INPUT,
	              _(CLICK_REPORT_DIALOG_HEADER),
	              _(CLICK_REPORT_DIALOG_MESSAGE),
	              _(CLICK_REPORT_DIALOG_BUTTON_SEND), _(CLICK_REPORT_DIALOG_BUTTON_BACK),
	              PlayerPrivilegePlayer,
	              "pl_click_SendReport");
}

forward pl_click_SendCash(playerid, clickedid, listitem, inputtext[]);
public pl_click_SendCash(playerid, clickedid, listitem, inputtext[])
{
	new giveplayer[MAX_PLAYER_NAME + 1],
		sendername[MAX_PLAYER_NAME + 1],
		string[MAX_STRING],
		money = strval(inputtext);

	GetPlayerName(clickedid, giveplayer, sizeof(giveplayer));
	GetPlayerName(playerid, sendername, sizeof(sendername));

	if (GetPlayerMoney(playerid) < money || !IsNumeric(inputtext) || money < 0) {
		SendClientMessage(playerid, COLOR_RED, _(CLICK_SENDCASH_NOT_VALID));
		return 0;
	}

	GivePlayerMoney(playerid, -money);
	GivePlayerMoney(clickedid, money);

	format(string, sizeof(string), _(CLICK_SENDCASH_GIVE), giveplayer, clickedid, money);
	SendClientMessage(playerid, COLOR_MONEY_GOOD, string);

	format(string, sizeof(string), _(CLICK_SENDCASH_GET), money, sendername, playerid);
	SendClientMessage(clickedid, COLOR_MONEY_GOOD, string);
	return 1;
}

forward pl_click_SendMessage(playerid, clickedid, listitem, inputtext[]);
public pl_click_SendMessage(playerid, clickedid, listitem, inputtext[])
{
	return pl_pm_Send(playerid, clickedid, inputtext);
}

forward pl_click_SendReport(playerid, clickedid, listitem, inputtext[]);
public pl_click_SendReport(playerid, clickedid, listitem, inputtext[])
{
	new sendername[MAX_PLAYER_NAME + 1],
		abusename[MAX_PLAYER_NAME + 1],
		string[MAX_STRING];

	GetPlayerName(clickedid, abusename, sizeof(abusename));

	format(string, sizeof(string), _(CLICK_REPORT_SELF), abusename, clickedid, inputtext);
	SendClientMessage(playerid, COLOR_RED, string);

	GetPlayerName(playerid, sendername, sizeof(sendername));
	format(string, sizeof(string), _(CLICK_REPORT_PLAYER), sendername, playerid, abusename, clickedid, inputtext);
	
	new admin_count = 0;

	foreach (new id : Player) {
		if (IsPlayerHavePrivilege(id, PlayerPrivilegeModer)) {
			admin_count++;
			SendClientMessage(id, COLOR_RED, string);
		}
	}

	if (admin_count == 0) {
		new reports = player_GetReportCount(playerid) + 1;
		player_SetReportCount(clickedid, reports);

		new reports_max = pl_report_GetMaxCount();

		format(string, sizeof(string), _(CLICK_REPORT_MESSAGE), reports, reports_max, abusename, clickedid, inputtext);
		SendClientMessageToAll(COLOR_WHITE, string);

		if (reports >= reports_max) {
			new jail_time = 0;
			JailPlayer(clickedid, jail_time);
			player_SetReportCount(clickedid, 0);

			format(string, sizeof(string), _(CLICK_REPORT_BY_MINUTE), jail_time);
			format(string, sizeof(string), _(CLICK_REPORT_SERVER), ReturnPlayerName(clickedid), string);
			SendClientMessageToAll(COLOR_WHITE, string);
		}
	}
	return 1;
}
