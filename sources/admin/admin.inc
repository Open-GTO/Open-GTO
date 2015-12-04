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

stock admin_OnPlayerText(playerid, text[])
{
	new mute_result = adm_mute_OnPlayerText(playerid, text);
	if (mute_result == 0) {
		SendClientMessage(playerid, COLOR_RED, _(MUTED_HELP_MESSAGE));
	}
	return mute_result;
}

stock admin_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	adm_godmod_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
	return 1;
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
	new message[MAX_STRING],
		count = 0;
	
	format(message, sizeof(message), lang_texts[12][28], ReturnPlayerName(senderid), senderid, text);
	foreach (new adminid : Player) {
		if (IsPlayerAdm(adminid)) {
			SendClientMessage(adminid, COLOR_RED, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, COLOR_RED, lang_texts[12][29]);
	} else {
		format(message, sizeof(message), lang_texts[12][30], text);
		SendClientMessage(senderid, COLOR_GREEN, message);
	}
	return 0;
}

stock SendClientMessageToModers(senderid, text[])
{
	new message[MAX_STRING],
		count = 0;

	format(message, sizeof(message), lang_texts[12][79], ReturnPlayerName(senderid), senderid, text);
	foreach (new adminid : Player) {
		if (IsPlayerMod(adminid)) {
			SendClientMessage(adminid, COLOR_RED, message);
			count++;
		}
	}

	if (count == 0) {
		SendClientMessage(senderid, COLOR_RED, lang_texts[12][80]);
	} else {
		format(message, sizeof(message), lang_texts[12][81], text);
		SendClientMessage(senderid, COLOR_GREEN, message);
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
		if (IsPlayerMod(adminid)) {
			SendClientMessage(adminid, COLOR_RED, message);
		}
	}

	Log_Player(message);
}
