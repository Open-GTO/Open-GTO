/*

	About: admin login system
	Author:	ziggi

*/

#if defined _admin_login_included
	#endinput
#endif

#define _admin_login_included


stock AdminLogin_OnRconLoginAttempt(ip[], password[], success)
{
	switch (success) {
		case 0: {
			pt_rcon_OnRconLoginAttempt(ip, password, success);
		}
		case 1: {
			// если игрок заходит ркон админом, то дадим ему полные права
			new player_ip[MAX_IP];

			foreach (new playerid : Player) {
				Player_GetIP(playerid, player_ip);

				if (!strcmp(ip, player_ip, false)) {
					SetPlayerPrivilege(playerid, PlayerPrivilegeRcon);
					break;
				}
			}
		}
	}
	return 1;
}
