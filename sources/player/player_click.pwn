/*

	About: player click system
	Author: ziggi

*/

#if defined _player_click_included
	#endinput
#endif

#define _player_click_included

PlayerClick_OnGameModeInit()
{
	Click_AddItem(DIALOG_STYLE_INPUT,
	              "CLICK_SENDCASH_DIALOG_HEADER",
	              "CLICK_SENDCASH_DIALOG_MESSAGE",
	              "BUTTON_SEND",
	              "BUTTON_BACK",
	              PlayerPrivilegePlayer,
	              "PlayerClick_SendCash");

	Click_AddItem(DIALOG_STYLE_INPUT,
	              "CLICK_PM_DIALOG_HEADER",
	              "CLICK_PM_DIALOG_MESSAGE",
	              "BUTTON_SEND",
	              "BUTTON_BACK",
	              PlayerPrivilegePlayer,
	              "PlayerClick_SendMessage");

	Click_AddItem(DIALOG_STYLE_INPUT,
	              "CLICK_REPORT_DIALOG_HEADER",
	              "CLICK_REPORT_DIALOG_MESSAGE",
	              "BUTTON_SEND",
	              "BUTTON_BACK",
	              PlayerPrivilegePlayer,
	              "PlayerClick_SendReport");
}

forward PlayerClick_SendCash(playerid, clickedid, listitem, inputtext[]);
public PlayerClick_SendCash(playerid, clickedid, listitem, inputtext[])
{
	new
		clickedname[MAX_PLAYER_NAME + 1],
		sendername[MAX_PLAYER_NAME + 1],
		money = strval(inputtext);

	GetPlayerName(clickedid, clickedname, sizeof(clickedname));
	GetPlayerName(playerid, sendername, sizeof(sendername));

	if (GetPlayerMoney(playerid) < money || !IsNumeric(inputtext) || money < 0) {
		Lang_SendText(playerid, "CLICK_SENDCASH_NOT_VALID");
		return 0;
	}

	GivePlayerMoney(playerid, -money);
	GivePlayerMoney(clickedid, money);

	Lang_SendText(playerid, "CLICK_SENDCASH_GIVE", clickedname, clickedid, money);
	Lang_SendText(clickedid, "CLICK_SENDCASH_GET", money, sendername, playerid);
	return 1;
}

forward PlayerClick_SendMessage(playerid, clickedid, listitem, inputtext[]);
public PlayerClick_SendMessage(playerid, clickedid, listitem, inputtext[])
{
	return SendPlayerPrivateMessage(playerid, clickedid, inputtext);
}

forward PlayerClick_SendReport(playerid, clickedid, listitem, inputtext[]);
public PlayerClick_SendReport(playerid, clickedid, listitem, inputtext[])
{
	new
		clickedname[MAX_PLAYER_NAME + 1],
		sendername[MAX_PLAYER_NAME + 1],
		players[MAX_PLAYERS],
		admin_count;

	GetPlayerName(clickedid, clickedname, sizeof(clickedname));
	GetPlayerName(playerid, sendername, sizeof(sendername));
	admin_count = GetPlayersWithPrivilege(PlayerPrivilegeModer, players);

	Lang_SendText(playerid, "CLICK_REPORT_SELF", clickedname, clickedid, inputtext);
	Lang_SendTextToPlayers(players, "CLICK_REPORT_PLAYER", sendername, playerid, clickedname, clickedid, inputtext);

	if (admin_count == 0) {
		new reports = GetPlayerReportsCount(playerid) + 1;
		SetPlayerReportsCount(clickedid, reports);

		new reports_max = GetMaxReportsCount();

		Lang_SendTextToAll("CLICK_REPORT_MESSAGE", reports, reports_max, clickedname, clickedid, inputtext);

		if (reports >= reports_max) {
			new jail_time = 0;
			JailPlayer(clickedid, jail_time);
			SetPlayerReportsCount(clickedid, 0);

			Lang_SendTextToAll("CLICK_REPORT_SERVER", clickedname, jail_time);
		}
	}
	return 1;
}
