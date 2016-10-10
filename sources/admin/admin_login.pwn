/*

	About: admin login system
	Author:	ziggi

*/

#if defined _admin_login_included
	#endinput
#endif

#define _admin_login_included

/*
	OnRconLoginAttempt
*/

public OnRconLoginAttempt(ip[], password[], success)
{
	if (success) {
		new player_ip[MAX_IP];

		foreach (new playerid : Player) {
			Player_GetIP(playerid, player_ip);

			if (!strcmp(ip, player_ip, false)) {
				SetPlayerPrivilege(playerid, PlayerPrivilegeRcon);
				break;
			}
		}
	}
	#if defined AdminLogin_OnRconLoginAttempt
		return AdminLogin_OnRconLoginAttempt(ip, password, success);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnRconLoginAttempt
	#undef OnRconLoginAttempt
#else
	#define _ALS_OnRconLoginAttempt
#endif

#define OnRconLoginAttempt AdminLogin_OnRconLoginAttempt
#if defined AdminLogin_OnRconLoginAttempt
	forward AdminLogin_OnRconLoginAttempt(ip[], password[], success);
#endif
