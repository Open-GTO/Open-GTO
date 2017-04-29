/*

	About: player ip
	Author:	ziggi

*/

#if defined _player_ip_included
	#endinput
#endif

#define _player_ip_included

/*
	Vars
*/

static
	gPlayerIP[MAX_PLAYERS][MAX_IP];

/*
	Functions
*/

stock Player_GetIP(playerid, ip[], const size = sizeof(ip))
{
	strcpy(ip, gPlayerIP[playerid], size);
}

stock Player_UpdateIP(playerid)
{
	GetPlayerIp(playerid, gPlayerIP[playerid], MAX_IP);
}
