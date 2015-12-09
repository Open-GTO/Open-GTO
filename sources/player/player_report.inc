/*

	About: player report system
	Author:	ziggi

*/

#if defined _pl_report_included
	#endinput
#endif

#define _pl_report_included
#pragma library pl_report

stock pl_report_GetMaxCount()
{
	return (MIN_REPORTS_FOR_JAIL + GetPlayersCount() / 10);
}

stock player_SetReportCount(playerid, buf)
{
	SetPVarInt(playerid, "playerReports", buf);
}

stock player_GetReportCount(playerid)
{
	return GetPVarInt(playerid, "playerReports");
}
