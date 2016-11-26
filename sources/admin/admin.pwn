/*

	Author: ziggi

*/

#if defined _admin_included
	#endinput
#endif

#define _admin_included

stock SendClientMessageToAdmins(senderid, text[])
{
	new
		sender_name[MAX_PLAYER_NAME + 1],
		count;

	GetPlayerName(senderid, sender_name, sizeof(sender_name));

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeAdmin)) {
			Lang_SendText(adminid, "ADMIN_CHAT_MESSAGE_TO_ADMIN", sender_name, senderid, text);
			count++;
		}
	}

	if (count == 0) {
		Lang_SendText(senderid, "ADMIN_CHAT_NO_ADMIN");
	} else {
		Lang_SendText(senderid, "ADMIN_CHAT_PLAYER_TO_ADMIN", text);
	}
	return 0;
}

stock SendClientMessageToModers(senderid, text[])
{
	new
		sender_name[MAX_PLAYER_NAME + 1],
		count;

	GetPlayerName(senderid, sender_name, sizeof(sender_name));

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
			Lang_SendText(adminid, "ADMIN_CHAT_MESSAGE_TO_MODER", sender_name, senderid, text);
			count++;
		}
	}

	if (count == 0) {
		Lang_SendText(senderid, "ADMIN_CHAT_NO_MODER");
	} else {
		Lang_SendText(senderid, "ADMIN_CHAT_PLAYER_TO_MODER", text);
	}
	return 0;
}

stock Admin_SendProtectReport(issuerid, var[], va_args<>)
{
	static
		text[MAX_LANG_VALUE_STRING],
		success,
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(issuerid, playername, sizeof(playername));
	success = Lang_GetText(Lang_GetDefaultLang(), var, text);
	if (!success) {
		return 0;
	}

	va_format(text, sizeof(text), text, va_start<2>);

	foreach (new adminid : Player) {
		if (!IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
			continue;
		}

		Lang_SendText(adminid, "ADMIN_PROTECTION_REPORT", playername, issuerid, text);
	}

	Log(playerlog, INFO, "Player %s [id: %d] possible use cheats. Protection: %s", playername, issuerid, text);
	return 1;
}
