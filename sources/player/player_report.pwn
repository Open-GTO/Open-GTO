/*

	About: player report system
	Author:	ziggi

*/

#if defined _pl_report_included
	#endinput
#endif

#define _pl_report_included

/*
	Vars
*/

static
	gPlayerReports[MAX_PLAYERS];

/*
	Report functions
*/

stock GetMaxReportsCount()
{
	return (MIN_REPORTS_FOR_JAIL + GetPlayersCount() / 10);
}

/*
	Player report functions
*/

stock SetPlayerReportsCount(playerid, count)
{
	gPlayerReports[playerid] = count;
}

stock GetPlayerReportsCount(playerid)
{
	return gPlayerReports[playerid];
}
