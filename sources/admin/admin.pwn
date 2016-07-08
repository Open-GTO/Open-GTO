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
		message[MAX_LANG_VALUE_STRING],
		count;

	GetPlayerName(senderid, sender_name, sizeof(sender_name));

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeAdmin)) {
			format(message, sizeof(message), _(adminid, ADMIN_CHAT_MESSAGE_TO_ADMIN), sender_name, senderid, text);
			SendClientMessage(adminid, -1, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, -1, _(senderid, ADMIN_CHAT_NO_ADMIN));
	} else {
		format(message, sizeof(message), _(senderid, ADMIN_CHAT_PLAYER_TO_ADMIN), text);
		SendClientMessage(senderid, -1, message);
	}
	return 0;
}

stock SendClientMessageToModers(senderid, text[])
{
	new
		sender_name[MAX_PLAYER_NAME + 1],
		message[MAX_LANG_VALUE_STRING],
		count;

	GetPlayerName(senderid, sender_name, sizeof(sender_name));

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
			format(message, sizeof(message), _(adminid, ADMIN_CHAT_MESSAGE_TO_MODER), sender_name, senderid, text);
			SendClientMessage(adminid, -1, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, -1, _(senderid, ADMIN_CHAT_NO_MODER));
	} else {
		format(message, sizeof(message), _(senderid, ADMIN_CHAT_PLAYER_TO_MODER), text);
		SendClientMessage(senderid, -1, message);
	}
	return 0;
}

stock Admin_SendProtectReport(issuerid, text[], {Float, _}:...)
{
	new
		message[MAX_LANG_VALUE_STRING],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(issuerid, playername, sizeof(playername));

	static const STATIC_ARGS = 2;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if (n)
	{
		new fstring[144], arg_start, arg_end;
		#emit CONST.alt			text
		#emit LCTRL				5
		#emit ADD
		#emit STOR.S.pri		arg_start

		#emit LOAD.S.alt		n
		#emit ADD
		#emit STOR.S.pri		arg_end
		do
		{
			#emit LOAD.I
			#emit PUSH.pri
			arg_end -= BYTES_PER_CELL;
			#emit LOAD.S.pri	arg_end
		}
		while(arg_end > arg_start);

		#emit PUSH.S			text
		#emit PUSH.C			144
		#emit PUSH.ADR			fstring

		n += BYTES_PER_CELL * 3;
		#emit PUSH.S			n
		#emit SYSREQ.C			format

		n += BYTES_PER_CELL;
		#emit LCTRL				4
		#emit LOAD.S.alt		n
		#emit ADD
		#emit SCTRL				4

		foreach (new adminid : Player) {
			if (!IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
				continue;
			}

			format(message, sizeof(message), _(adminid, ADMIN_PROTECTION_REPORT), playername, issuerid, fstring);
			SendClientMessage(adminid, COLOR_RED, message);
		}

		format(message, sizeof(message), _d(ADMIN_PROTECTION_REPORT), playername, issuerid, fstring);
	}
	else
	{
		foreach (new adminid : Player) {
			if (!IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
				continue;
			}

			format(message, sizeof(message), _(adminid, ADMIN_PROTECTION_REPORT), playername, issuerid, text);
			SendClientMessage(adminid, COLOR_RED, message);
		}

		format(message, sizeof(message), _d(ADMIN_PROTECTION_REPORT), playername, issuerid, text);
	}

	Log_Player(message);
}
