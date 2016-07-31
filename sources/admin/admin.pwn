/*

	Author: ziggi

*/

#if defined _admin_included
	#endinput
#endif

#define _admin_included


stock Admin_SaveConfig(file_config)
{
	AdminMapTP_SaveConfig(file_config);
}

stock Admin_LoadConfig(file_config)
{
	AdminMapTP_LoadConfig(file_config);
}

stock Admin_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	AdminMapTP_OnPlayerClickMap(playerid, fX, fY, fZ);
	return 1;
}

stock Admin_OnRconLoginAttempt(ip[], password[], success)
{
	AdminLogin_OnRconLoginAttempt(ip, password, success);
	return 1;
}

stock SendClientMessageToAdmins(senderid, text[])
{
	new
		sender_name[MAX_PLAYER_NAME + 1],
		count;

	GetPlayerName(senderid, sender_name, sizeof(sender_name));

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeAdmin)) {
			Lang_SendText(adminid, "ACCOUNT_LOGIN_FIRST", sender_name, senderid, text);
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

stock Admin_SendProtectReport(issuerid, var[], {Float, _}:...)
{
	static
		text[MAX_LANG_VALUE_STRING],
		success,
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(issuerid, playername, sizeof(playername));
	success = Lang_GetText(Lang_GetDefaultLang(), var, text);
	va_format(text, sizeof(text), text, va_start<2>);

	foreach (new adminid : Player) {
		if (!IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
			continue;
		}

		Lang_SendText(adminid, "ADMIN_PROTECTION_REPORT", playername, issuerid, fstring);
	}

	Log_Player("ADMIN_PROTECTION_REPORT", playername, issuerid, fstring);
}
