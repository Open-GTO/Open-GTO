/*

	Author: ziggi

*/

#if defined _admin_included
	#endinput
#endif

#define _admin_included
#pragma library admin


stock admin_SaveConfig(file_config)
{
	adm_maptp_SaveConfig(file_config);
}

stock admin_LoadConfig(file_config)
{
	adm_maptp_LoadConfig(file_config);
}

stock admin_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	adm_maptp_OnPlayerClickMap(playerid, fX, fY, fZ);
	return 1;
}

stock admin_OnRconLoginAttempt(ip[], password[], success)
{
	adm_login_OnRconLoginAttempt(ip, password, success);
	return 1;
}

stock SendClientMessageToAdmins(senderid, text[])
{
	new
		message[MAX_LANG_VALUE_STRING],
		count;
	
	GetPlayerName(senderid, message, sizeof(message));
	format(message, sizeof(message), _(ADMIN_CHAT_MESSAGE_TO_ADMIN), message, senderid, text);

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeAdmin)) {
			SendClientMessage(adminid, -1, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, -1, _(ADMIN_CHAT_NO_ADMIN));
	} else {
		format(message, sizeof(message), _(ADMIN_CHAT_PLAYER_TO_ADMIN), text);
		SendClientMessage(senderid, -1, message);
	}
	return 0;
}

stock SendClientMessageToModers(senderid, text[])
{
	new
		message[MAX_LANG_VALUE_STRING],
		count;

	GetPlayerName(senderid, message, sizeof(message));
	format(message, sizeof(message), _(ADMIN_CHAT_MESSAGE_TO_MODER), message, senderid, text);

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
			SendClientMessage(adminid, -1, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, -1, _(ADMIN_CHAT_NO_MODER));
	} else {
		format(message, sizeof(message), _(ADMIN_CHAT_PLAYER_TO_MODER), text);
		SendClientMessage(senderid, -1, message);
	}
	return 0;
}

stock Admin_SendProtectReport(issuerid, text[], {Float, _}:...)
{
	new
		message[144],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(issuerid, playername, sizeof(playername));

	format(message, sizeof(message), _(ADMIN_PROTECTION_REPORT), playername, issuerid);

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

		strcat(message, fstring);
	}
	else
	{
		strcat(message, text);
	}

	foreach (new adminid : Player) {
		if (IsPlayerHavePrivilege(adminid, PlayerPrivilegeModer)) {
			SendClientMessage(adminid, COLOR_RED, message);
		}
	}

	Log_Player(message);
}
